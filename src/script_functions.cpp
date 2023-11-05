#include <stdafx.h>
#include <log_manager.h>
#include <application.h>
#include <game_object_manager.h>
#include <game_object.h>
#include <script_functions.h>
#include <irrKlang.h>

int ssuge::script::createGameObject(lua_State * L)
{
	// Get the parameters from Lua
	if (lua_gettop(L) == 2)
		lua_pushnil(L);				// The user didn't give us a script -- just push a nil to indicate this.
	if (lua_gettop(L) != 3)
		luaL_error(L, "you must pass at a group name and an object name and a script name (or nil if no script)");
	const char * gname = luaL_checkstring(L, 1);
	const char * oname = luaL_checkstring(L, 2);
	std::string script_name = "";
	if (lua_isstring(L, 3))
		script_name = std::string(lua_tostring(L, 3));

	// Create the game object in C++.  This call will also indirectly call SCRIPT_MANAGER->createGameObject, which 
	// will create the lua counter-part to this game object (the lua instance table will be the top thing on the
	// lua stack -- this is the thing we want to return)
	ssuge::GameObject * game_obj = GAME_OBJECT_MANAGER->createGameObject(gname, oname, NULL, script_name, 42);

	// The stack should now hold: [gname, oname, new-gobj-table]
	return 1;
}


int ssuge::script::getGameObject(lua_State * L)
{
	if (lua_gettop(L) == 2 && lua_isstring(L, 1) && lua_isstring(L, 2))
	{
		const char * gname = lua_tostring(L, 1);
		const char * oname = lua_tostring(L, 2);

		// This function puts either a game_object_instance_table or nil onto the stack
		SCRIPT_MANAGER->getLuaGameObject(std::string(gname), std::string(oname));
		return 1;
	}
	else
	{
		luaL_error(L, "you must pass a group name (string) and object name (string) to this function");
		return 0;
	}
}


int ssuge::script::isKeyDown(lua_State * L)
{
	if (lua_gettop(L) != 1 || !lua_isstring(L, 1))
		luaL_error(L, "you must pass a string (key name) to this function");
	const char * keyname = luaL_checkstring(L, 1);
	if (INPUT_MANAGER->isKeyDown(keyname))
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}

int ssuge::script::isMouseButtonDown(lua_State * L)
{
	if (lua_gettop(L) != 1 || !lua_isstring(L, 1))
		luaL_error(L, "you must pass a string (mouse button name) to this function");
	const char * mousebutton = luaL_checkstring(L, 1);
	if (strcmp(mousebutton, "Left") == 0)
	{
		if (INPUT_MANAGER->isMouseButtonDown("Left"))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	}

	if (strcmp(mousebutton, "Middle") == 0)
	{
		if (INPUT_MANAGER->isMouseButtonDown("Middle"))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	}

	if (strcmp(mousebutton, "Right") == 0)
	{
		if (INPUT_MANAGER->isMouseButtonDown("Right"))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	}
	return 1;
}

int ssuge::script::setBGM(lua_State * L)
{
	if (lua_gettop(L) != 2 || !lua_isstring(L, 1) || !lua_isnumber(L, 2))
		luaL_error(L, "you must pass a string (music file path) and number from 0 to 1 for volume to this function");
	auto device = irrklang::createIrrKlangDevice();
	float vol = lua_tonumber(L, 2);
	device->setSoundVolume(vol);
	device->play2D(lua_tostring(L, 1), true, false, true);
	return 0;
}

int ssuge::script::deleteGameObject(lua_State * L)
{
	if (lua_gettop(L) != 2 || !lua_isstring(L, 1) || !lua_isstring(L, 2))
		luaL_error(L, "you must pass a string for group name and string for object name to delete to this function");
	GAME_OBJECT_MANAGER->destroyGameObject(lua_tostring(L, 1), lua_tostring(L, 2));
	return 0;
}

int ssuge::script::getMousePos(lua_State * L)
{
	lua_pushnumber(L, INPUT_MANAGER->getMousePos().x);
	lua_pushnumber(L, INPUT_MANAGER->getMousePos().y);
	return 2;
}

int ssuge::script::isControllerButtonDown(lua_State * L)
{
	if (lua_gettop(L) != 2)
		luaL_error(L, "You must pass controller number and the name of the button state to get value");
	int num = (int)lua_tonumber(L, 1);
	const char * name = luaL_checkstring(L, 2);
	lua_pushboolean(L, INPUT_MANAGER->getControllerButtonInput(num - 1, static_cast<std::string>(name)));
	return 1;
}

int ssuge::script::getControllerAxisOrTriggerValue(lua_State * L)
{
	if (lua_gettop(L) != 2)
		luaL_error(L, "You must pass in controller number and name of trigger or stick axis!");
	int con = lua_tonumber(L, 1);
	std::string name = luaL_checkstring(L, 2);
	lua_pushnumber(L, INPUT_MANAGER->getControllerAxisOrTriggerValue(con - 1, name));
	return 1;
}

int ssuge::script::loadScene(lua_State * L)
{
	if (lua_gettop(L) != 1 || !lua_isstring(L, 1))
		luaL_error(L, "You must pass a (string) file name for a scene (.xml) file");
	const char * fname = luaL_checkstring(L, 1);
	GAME_OBJECT_MANAGER->parseSceneFile(std::string(fname));
	return 0;
}


int ssuge::script::log(lua_State * L)
{
	const char * s = luaL_checkstring(L, 1);
	LOG_MANAGER->log(std::string(s));
	return 0;
}


int ssuge::script::registerInputListener(lua_State * L)
{
	if (lua_gettop(L) != 1 || !lua_istable(L, 1))
		luaL_error(L, "you must pass a GameObject instance table to this function");

	lua_getfield(L, 1, "_udata_");
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		luaL_error(L, "you must pass a GameObject instance table (with a _udata_ userdata field) to this function");
	}
	else
	{
		ssuge::GameObject* gobj = (ssuge::GameObject*)lua_touserdata(L, -1);
		INPUT_MANAGER->registerListener(gobj);
	}

	return 0;
}


int ssuge::script::setBackgroundColour(lua_State * L)
{
	if (lua_gettop(L) == 3 && lua_isinteger(L, 1) && lua_isinteger(L, 2) && lua_isinteger(L, 3))
	{
		int r, g, b;
		r = (int)lua_tointeger(L, 1);
		g = (int)lua_tointeger(L, 2);
		b = (int)lua_tointeger(L, 3);
		APPLICATION->setBackgroundColor(Ogre::ColourValue(r / 255.0f, g / 255.0f, b / 255.0f));
	}
	else if (lua_gettop(L) == 3 && lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3))
	{
		float r, g, b;
		r = (float)lua_tonumber(L, 1);
		g = (float)lua_tonumber(L, 2);
		b = (float)lua_tonumber(L, 3);
		APPLICATION->setBackgroundColor(Ogre::ColourValue(r, g, b));
	}
	else
		luaL_error(L, "Invalid parameters!");
	return 0;
}