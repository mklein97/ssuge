#include <stdafx.h>
#include <script_manager.h>
#include <log_manager.h>
#include <script_functions.h>
#include <script_game_object.h>
#include <game_object.h>

// The icky template-specialization / static-initializer for Application's msSingleton attribute
template<> ssuge::ScriptManager* ssuge::Singleton<ssuge::ScriptManager>::msSingleton = NULL;

// The "table of contents" for all top-level ssuge functions
struct luaL_Reg ssuge_funcs[] =
{
	{"createGameObject", ssuge::script::createGameObject},
	{"getGameObject", ssuge::script::getGameObject},
	{"isKeyDown", ssuge::script::isKeyDown},
	{"isMouseButtonDown", ssuge::script::isMouseButtonDown},
	{"getMousePos", ssuge::script::getMousePos},
	{"isControllerButtonDown", ssuge::script::isControllerButtonDown},
	{"getControllerAxisOrTriggerValue", ssuge::script::getControllerAxisOrTriggerValue},
	{"loadScene", ssuge::script::loadScene},
	{"setBGM", ssuge::script::setBGM},
	{"deleteGameObject", ssuge::script::deleteGameObject},
	{"log", ssuge::script::log},
	{"setBackgroundColour", ssuge::script::setBackgroundColour},
	{"registerInputListener", ssuge::script::registerInputListener},
	{NULL, NULL}
};

// A similar one for the methods of the game object "class"
struct luaL_Reg ssuge_game_object_methods[] =
{
	{"__index", ssuge::script::script_GameObject__index},
	{"createMeshComponent", ssuge::script::script_GameObject_createMeshComponent},
	{"createSoundComponent", ssuge::script::script_GameObject_createSoundComponent},
	{"playSoundComponent", ssuge::script::script_GameObject_playSoundComponent},
	{"rotate", ssuge::script::script_GameObject_rotate},
	{"isColliding", ssuge::script::script_GameObject_isColliding},
	{"getName", ssuge::script::script_GameObject_getName},
	{"getGroupName", ssuge::script::script_GameObject_getGroupName},
	{"setPosition", ssuge::script::script_GameObject_setPosition},
	{"getPosition", ssuge::script::script_GameObject_getPosition},
	{"setScale", ssuge::script::script_GameObject_setScale},
	{"translate", ssuge::script::script_GameObject_translate},
	{NULL, NULL}
};


int ssuge::ScriptManager::error_handler(lua_State * L)
{
	// Reference: https://stackoverflow.com/questions/12256455/print-stacktrace-from-c-code-with-embedded-lua
	lua_getglobal(L, "debug");
	lua_getfield(L, -1, "traceback");
	lua_pushvalue(L, 1);
	lua_pushinteger(L, 2);
	lua_call(L, 2, 1);
	std::string msg = std::string(lua_tostring(L, -1));

	LOG_MANAGER->log("SCRIPT-ERROR: " + msg);
	return 0;
}


void ssuge::ScriptManager::stack_inspector(lua_State * L)
{
	std::stringstream ss;
	ss << "Stack:[";
	for (int i = 1; i <= lua_gettop(L); i++)
	{
		if (lua_isboolean(L, i))
			ss << "<boolean:" << (lua_toboolean(L, i) ? "true" : "false") << ">";
		else if (lua_iscfunction(L, i))
			ss << "<c-function>";
		else if (lua_isfunction(L, i))
			ss << "<lua-function>";
		else if (lua_isinteger(L, i))
			ss << "<integer:" << lua_tointeger(L, i) << ">";
		else if (lua_islightuserdata(L, i))
			ss << "<light-userdata>";
		else if (lua_isnil(L, i))
			ss << "<nil>";
		else if (lua_isnone(L, i))
			ss << "<none>";
		else if (lua_isnumber(L, i))
			ss << "<number:" << lua_tonumber(L, i) << ">";
		else if (lua_isstring(L, i))
			ss << "<string:" << lua_tostring(L, i) << ">";
		else if (lua_istable(L, i))
			ss << "<table>";
		else if (lua_isuserdata(L, i))
			ss << "<userdata>";
		else
			ss << "<???>";
	}
	ss << "]";
	LOG_MANAGER->log(ss.str());
}


ssuge::ScriptManager::ScriptManager() : mStarted(false)
{
	// Create the interpreter
	mLuaInterpreter = luaL_newstate();
	if (mLuaInterpreter == NULL)
		LOG_MANAGER->log("CRITICAL ERROR: Unable to create lua interpreter!");
	else
	{
		// Open the standard lua libraries
		luaL_openlibs(mLuaInterpreter);

		// Load our ssuge module as a global
		luaL_newlib(mLuaInterpreter, ssuge_funcs);
		lua_setglobal(mLuaInterpreter, "ssuge");

		// Create a global "GameObject" table that will act as a metatable for all 
		// instance of GameObject in lua
		luaL_newlib(mLuaInterpreter, ssuge_game_object_methods);		
		lua_setglobal(mLuaInterpreter, "GameObject");

		// Create a global table-of-tables for all game objects.  This mimics the 
		// structure of the GameObjectManager (the keys in the table are the group
		// names; the value is another table [key = object-name, value = table])
		lua_newtable(mLuaInterpreter);
		lua_setglobal(mLuaInterpreter, "_allGameObjects");
	}
}



ssuge::ScriptManager::~ScriptManager()
{
	if (mLuaInterpreter)
		lua_close(mLuaInterpreter);
}



void ssuge::ScriptManager::loadScript(std::string fname)
{
	if (mLuaInterpreter)
	{
		// "Compile" the module
		lua_pushcfunction(mLuaInterpreter, error_handler);
		int handler_pos = lua_gettop(mLuaInterpreter);
		int status = luaL_loadfile(mLuaInterpreter, fname.c_str());
		if (status == LUA_OK)
		{
			// Stack = [..., handler_function, code-module]
			LOG_MANAGER->log("successfully loaded '" + fname + "'");

			// Actually run the method.  If anything goes wrong, control will jump to the error_handler
			// function (above), where we'll dump a stack trace to the log.  I'm expecting one thing
			// to come back from this function (the module, usually containing a few functions)
			status = lua_pcall(mLuaInterpreter, 0, 1, -2);

			// Remove the error handler
			lua_remove(mLuaInterpreter, handler_pos);

			if (status == LUA_OK)
			{
				// SUCCESS!
			}
			else if (status == LUA_ERRRUN)
				LOG_MANAGER->log("encountered a run-time error in '" + fname + "'");
			else if (status == LUA_ERRERR)
				LOG_MANAGER->log("problem encountered when running message handler for '" + fname + "'");
			else if (status == LUA_ERRMEM)
				LOG_MANAGER->log("error allocating memory in '" + fname + "'");
			else if (status == LUA_ERRGCMM)
				LOG_MANAGER->log("error in garbage-collection when evaluating '" + fname + "'");
		}
		else if (status == LUA_ERRSYNTAX)
			LOG_MANAGER->log("syntax error in '" + fname + "'");
		else if (status == LUA_ERRMEM)
			LOG_MANAGER->log("error allocating memory in '" + fname + "'");
		else if (status == LUA_ERRGCMM)
			LOG_MANAGER->log("error in garbage-collection when evaluating '" + fname + "'");
	}
}


void ssuge::ScriptManager::appendScriptModule(std::string group_name, std::string object_name, std::string script_fname)
{
	int stack_pos = lua_gettop(mLuaInterpreter);

	// Get the lua game object
	if (getLuaGameObject(group_name, object_name))
	{
		// Stack = [..., game-object-table]

		// Call load script to "execute" the stack.  Assuming that function worked 
		// and the script-writer returned a module-table, we now go through all elements
		// of that module table (presumably functions) and add them to the game object at the top of the stack
		loadScript(script_fname);

		// Stack = [..., game-object-table, module-table]
		lua_pushnil(mLuaInterpreter);		// Stack = [..., game-object-table, module-table, nil]
		while (lua_next(mLuaInterpreter, -2) != NULL)
		{
			// Stack = [..., game-object-table, module-table, current-key, current-value]
			const char * key = lua_tostring(mLuaInterpreter, -2);
			lua_setfield(mLuaInterpreter, -4, key);
			// Stack = [..., game-object-table, module-table, current-key]
		}
	}

	
	// Reset the stack
	lua_settop(mLuaInterpreter, stack_pos);
}



void ssuge::ScriptManager::createLuaGameObject(GameObject* gobj, std::string group_name, std::string object_name, std::string script_path)
{
	//															// Stack = [...]
	// Create a lua table that holds a C-value
	lua_newtable(mLuaInterpreter);								// Stack = [..., instance-table]
	lua_pushlightuserdata(mLuaInterpreter, (void*)gobj);		// Stack = [..., instance-table, user_data]
	lua_setfield(mLuaInterpreter, -2, "_udata_");				// Stack = [..., instance-table]

	// Get the global 'GameObject' "class" to act as the new table's metatable
	lua_getglobal(mLuaInterpreter, "GameObject");				// Stack = [..., instance-table, GameObject class]
	if (!lua_isnil(mLuaInterpreter, -1))
		lua_setmetatable(mLuaInterpreter, -2);
	else
		lua_pop(mLuaInterpreter, 1);
																// Stack = [..., instance-table]
		
	// Put the GameObject in our master GOM-shadow
	lua_getglobal(mLuaInterpreter, "_allGameObjects");			// Stack = [..., instance-table, _allGameObjects]
	lua_getfield(mLuaInterpreter, -1, group_name.c_str());		// Stack = [..., instance-table, _allGameObjects, _allGameObjects_group_tbl]
	if (lua_isnil(mLuaInterpreter, -1))
	{
		// That group doesn't exist yet -- create it.
		lua_pop(mLuaInterpreter, 1);							// Stack = [..., instance-table, _allGameObjects]
		lua_newtable(mLuaInterpreter);							// Stack = [..., instance-table, _allGameObjects, _allGameObjects_group_tbl]
		lua_pushvalue(mLuaInterpreter, -1);						// Stack = [..., instance-table, _allGameObjects, _allGameObjects_group_tbl, _allGameObjects_group_tbl]
		lua_setfield(mLuaInterpreter, -3, group_name.c_str());	// Stack = [..., instance-table, _allGameObjects, _allGameObjects_group_tbl]
	}
	lua_pushvalue(mLuaInterpreter, -3);							// Stack = [..., instance-table, _allGameObjects, _allGameObjects_group_tbl, instance-table]
	lua_setfield(mLuaInterpreter, -2, object_name.c_str());		// Stack = [..., instance-table, _allGameObjects, _allGameObjects_group_tbl]
	lua_pop(mLuaInterpreter, 2);								// Stack = [..., instance-table]

	// If the user passed a 3rd string parameter, attach the script contents
	if (script_path != "")
		SCRIPT_MANAGER->appendScriptModule(group_name, object_name, script_path);

	// Queue this object so its onStart method can be called.
	mNewScriptAwareObjects.push_back(gobj);
	//std::map<std::string, std::string> empty_map;
	//SCRIPT_MANAGER->executeMethod(group_name, object_name, "onStart", empty_map);
}


void ssuge::ScriptManager::removeGameObject(std::string group_name, std::string object_name)
{
	lua_getglobal(mLuaInterpreter, "_allGameObjects");			// Stack = [..., _allGameObjects]
	lua_getfield(mLuaInterpreter, -1, group_name.c_str());		// Stack = [..., _allGameObjects, group]
	if (!lua_isnil(mLuaInterpreter, -1))
	{
		lua_pushnil(mLuaInterpreter);							// Stack = [..., _allGameObjects, group, nil]
		lua_setfield(mLuaInterpreter, -2, object_name.c_str());	// Stack = [..., _allGameObjects, group]
	}
	lua_pop(mLuaInterpreter, 2);								// Stack = [...]
}


void ssuge::ScriptManager::removeGroup(std::string group_name)
{
	lua_getglobal(mLuaInterpreter, "_allGameObjects");			// Stack = [..., _allGameObjects]
	lua_pushnil(mLuaInterpreter);								// Stack = [..., _allGameObjects, nil]
	lua_setfield(mLuaInterpreter, -2, group_name.c_str());		// Stack = [..., _allGameObjects]
	lua_pop(mLuaInterpreter, 1);								// Stack = [...]
}


bool ssuge::ScriptManager::getLuaGameObject(std::string group_name, std::string object_name)
{
	// Stack = [...]

	lua_getglobal(mLuaInterpreter, "_allGameObjects");		// Stack = [..., _allGameObject]
	if (lua_isnil(mLuaInterpreter, -1))		// Shouldn't ever fail, but make sure we have _allGameObjects
	{
		return false;										// Stack = [..., nil]										
	}
	lua_getfield(mLuaInterpreter, -1, group_name.c_str());	// Stack = [..., _allGameObjects, group-table]
	if (lua_isnil(mLuaInterpreter, -1))		// Did we find the group?
	{
		lua_remove(mLuaInterpreter, -2);					// Stack = [..., nil]
		return false;
	}
	lua_getfield(mLuaInterpreter, -1, object_name.c_str());	// Stack = [..., _allGameObjects, group-table, game-object]
	if (lua_isnil(mLuaInterpreter, -1))		// Did we find the game object?
	{
		lua_remove(mLuaInterpreter, -3);					// Stack = [..., group-table, nil]
		lua_remove(mLuaInterpreter, -2);					// Stack = [..., nil]
		return false;
	}

	// Stack = [..., _allGameObjects, group-table, game-object]
	lua_remove(mLuaInterpreter, -3);						// Stack = [..., group-table, game-object]
	lua_remove(mLuaInterpreter, -2);						// Stack = [..., game-object]
	return true;
}


void ssuge::ScriptManager::executeMethod(std::string group_name, std::string object_name, 
	std::string method_name, std::map<std::string, std::string>& params)
{
	// Stack = [...]
	int init_stack_pos = lua_gettop(mLuaInterpreter);

	// Push the error-handler to the stack.  We might use it later
	lua_pushcfunction(mLuaInterpreter, error_handler);	// Stack = [..., error_handler]
	
	// Get the group from _allGameObjects
	if (getLuaGameObject(group_name, object_name))
	{
		// Stack = [..., error_handler, game-object]
		lua_getfield(mLuaInterpreter, -1, method_name.c_str());		// Stack = [..., error_handler, game-object, method]
		if (lua_isnil(mLuaInterpreter, -1))		// Did we find the method?
			return;

		// Make a copy of the game object
		lua_pushvalue(mLuaInterpreter, -2);		// Stack = [..., error_handler, game-object, method, game-object]

		// Build the parameters table
		lua_newtable(mLuaInterpreter);			// Stack = [..., error_handler, game-object, method, game-object, param-table]	
		std::map<std::string, std::string>::iterator it = params.begin();
		while (it != params.end())
		{
			lua_pushstring(mLuaInterpreter, it->second.c_str());
			// Stack = [..., error_handler, game-object, method, game-object, param-table, param-value]	
			lua_setfield(mLuaInterpreter, -2, it->first.c_str());
			// Stack = [..., error_handler, game-object, method, game-object, param-table]	
			++it;
		}


		// Call the method -- note to self: do we want to return something?
		int status = lua_pcall(mLuaInterpreter, 2, LUA_MULTRET, -5);
		std::string location = "_allGameObjects[" + group_name + "][" + object_name + "]." + method_name;

		if (status == LUA_OK)
		{
			// SUCCESS!
		}
		else if (status == LUA_ERRRUN)
			LOG_MANAGER->log("encountered a run-time error in '" + location + "'");
		else if (status == LUA_ERRERR)
			LOG_MANAGER->log("problem encountered when running message handler for '" + location + "'");
		else if (status == LUA_ERRMEM)
			LOG_MANAGER->log("error allocating memory in '" + location + "'");
		else if (status == LUA_ERRGCMM)
			LOG_MANAGER->log("error in garbage-collection when evaluating '" + location + "'");
		else
			LOG_MANAGER->log("unknown error when evaluation '" + location + "'");
	}

	// Put the stack back to the way we got it.
	lua_settop(mLuaInterpreter, init_stack_pos);
}


int ssuge::ScriptManager::update(float dt)
{
	// This is extermely lazy, but my stack has a lot of items left on it from running the initial stacks.  
	// To-do: find and fix what's causing that.  But for now...just clear the stack the first time we get here
	if (!mStarted)
	{
		mStarted = true;
		lua_settop(mLuaInterpreter, 0);
	}

	if (mNewScriptAwareObjects.size() > 0)
	{
		for (unsigned int i = 0; i < mNewScriptAwareObjects.size(); i++)
		{
			std::map<std::string, std::string> empty_map;
			SCRIPT_MANAGER->executeMethod(mNewScriptAwareObjects[i]->getGroupName(), mNewScriptAwareObjects[i]->getName(), "onStart", empty_map);
		}

		mNewScriptAwareObjects.clear();
	}

	return 0;
}