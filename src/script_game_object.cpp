#include <stdafx.h>
#include <game_object.h>
#include <script_manager.h>
#include <script_game_object.h>

// A helper method to get a C++ GameObject from a lua object at the given stack position.
// Returns NULL if something goes wrong (it'll probably never reach the return though --
// lua does a longjump somewhere else...)
ssuge::GameObject * ssuge::script::getGameObject(lua_State * L, int stack_pos)
{
	lua_getfield(L, stack_pos, "_udata_");
	if (lua_isnil(L, -1))
	{
		luaL_error(L, "Your table doesn't have a '_udata_' field -- was it not set up with ssuge.createGameObject?");
		return NULL;
	}
	else
	{
		ssuge::GameObject * gobj = (ssuge::GameObject*)lua_touserdata(L, -1);
		lua_pop(L, 1);
		return gobj;
	}
}

int ssuge::script::script_GameObject__index(lua_State * L)
{
	// The stack: [table, key]
	// This might need to change once we integrate user-defined "classes".  For now, 
	// just get the field from the GameObject table, if it has one.

	// Stack = [table, key]
	const char * key = lua_tostring(L, -1);
	lua_getglobal(L, "GameObject");
	lua_insert(L, 1);					// Stack = [GameObject "class", table, key]
	lua_rawget(L, 1);					// Stack = [GameObject "class", table, result]
	if (lua_isnil(L, -1))
	{
		// Get the C++ game object
		lua_getfield(L, 2, "_udata_");	// Stack = [GameObject "class", table, result, light-userdata]
		if (!lua_isnil(L, -1))
		{
			ssuge::GameObject * gobj = (ssuge::GameObject*)lua_touserdata(L, -1);

			// See if it's one of these special values
			if (!strcmp(key, "name"))
			{
				lua_pushstring(L, gobj->getName().c_str());
				return 1;
			}
			else if (!strcmp(key, "group"))
			{
				lua_pushstring(L, gobj->getGroupName().c_str());
				return 1;
			}

			lua_pop(L, 1);		// Pops off the light user-data
		}
	}
	return 1;
}

int ssuge::script::script_GameObject_createMeshComponent(lua_State * L)
{
	// Stack should have a [GameObject-"instance", mesh-fname]
	if (lua_gettop(L) != 2 || !lua_istable(L, 1) || !lua_isstring(L, 2))
		luaL_error(L, "you must pass the GameObject-table and mesh-name");
	ssuge::GameObject* gobj = getGameObject(L, 1);
	const char * mesh_name = lua_tostring(L, 2);
	gobj->createMeshComponent(std::string(mesh_name));
	return 0;
}

int ssuge::script::script_GameObject_createSoundComponent(lua_State * L)
{
	if (lua_gettop(L) != 2 || !lua_istable(L, 1) || !lua_isstring(L, 2))
		luaL_error(L, "you must pass the GameObject-table and sound path");
	ssuge::GameObject* gobj = getGameObject(L, 1);
	std::string sname = lua_tostring(L, 2);
	gobj->createSoundComponent(sname);
	return 0;
}

int ssuge::script::script_GameObject_playSoundComponent(lua_State * L)
{
	if (lua_gettop(L) != 2 || !lua_istable(L, 1) || !lua_isboolean(L, 2))
		luaL_error(L, "you must pass the GameObject-table and boolean");
	bool b = lua_toboolean(L, 2);
	ssuge::GameObject* gobj = getGameObject(L, 1);
	gobj->getSoundComponent()->play(b);
	return 0;
}


int ssuge::script::script_GameObject_rotate(lua_State * L)
{
	// Stack should have a [GameObject-"instance", degrees, vx, vy, vz]
	if (lua_gettop(L) != 5 || !lua_istable(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || 
		!lua_isnumber(L, 4) || !lua_isnumber(L, 5))
		luaL_error(L, "you must pass the GameObject-table, degrees, vx, vy, vz");
	ssuge::GameObject* gobj = getGameObject(L, 1);
	float deg = (float)lua_tonumber(L, 2);
	float vx = (float)lua_tonumber(L, 3);
	float vy = (float)lua_tonumber(L, 4);
	float vz = (float)lua_tonumber(L, 5);
	gobj->rotateWorld(deg, vx, vy, vz);
	return 0;
}

int ssuge::script::script_GameObject_getName(lua_State * L)
{
	if (lua_gettop(L) != 1 || !lua_istable(L, 1))
		luaL_error(L, "must pass the game object table to get its name");
	GameObject* gobj = getGameObject(L, 1);
	lua_pushstring(L, gobj->getName().c_str());
	return 1;
}

int ssuge::script::script_GameObject_getGroupName(lua_State * L)
{
	if (lua_gettop(L) != 1 || !lua_istable(L, 1))
		luaL_error(L, "must pass the game object table to get its group name");
	GameObject* gobj = getGameObject(L, 1);
	lua_pushstring(L, gobj->getGroupName().c_str());
	return 1;
}

int ssuge::script::script_GameObject_isColliding(lua_State * L)
{
	if (lua_gettop(L) != 2 || !lua_istable(L, 1) || !lua_istable(L, 2))
		luaL_error(L, "must pass the game object table and the table to compare collision to");
	GameObject* gobj = getGameObject(L, 1);
	GameObject* gobj2 = getGameObject(L, 2);
	if (gobj->getAABB().intersects(gobj2->getAABB()))
		lua_pushboolean(L, true);
	else
		lua_pushboolean(L, false);
	return 1;
}

int ssuge::script::script_GameObject_setPosition(lua_State * L)
{
	// Stack should have a [GameObject-"instance", x, y, z]
	if (lua_gettop(L) != 4 || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		luaL_error(L, "you must pass the GameObject-table and an x, y, and z value");
	ssuge::GameObject* gobj = getGameObject(L, 1);
	float x = (float)lua_tonumber(L, 2);
	float y = (float)lua_tonumber(L, 3);
	float z = (float)lua_tonumber(L, 4);
	gobj->setPosition(x, y, z);
	return 0;
}

int ssuge::script::script_GameObject_getPosition(lua_State * L)
{
	if (lua_gettop(L) != 1 || !lua_istable(L, 1))
		luaL_error(L, "must pass the table to get position");
	GameObject* gobj = getGameObject(L, 1);
	float x = gobj->getPosition().x;
	float y = gobj->getPosition().y;
	float z = gobj->getPosition().z;
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);
	return 3;
}

int ssuge::script::script_GameObject_setScale(lua_State * L)
{
	// Stack should have a [GameObject-"instance", x, y, z]
	if (lua_gettop(L) != 4 || !lua_istable(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		luaL_error(L, "you must pass the GameObject-table and an x, y, and z value");
	ssuge::GameObject* gobj = getGameObject(L, 1);
	float x = (float)lua_tonumber(L, 2);
	float y = (float)lua_tonumber(L, 3);
	float z = (float)lua_tonumber(L, 4);
	gobj->setScale(x, y, z);
	return 0;
}

int ssuge::script::script_GameObject_translate(lua_State * L)
{
	// Stack should have a [GameObject-"instance", x, y, z]
	if (lua_gettop(L) != 4 || !lua_istable(L, 1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3) || !lua_isnumber(L, 4))
		luaL_error(L, "you must pass the GameObject-table and an x, y, and z value");
	ssuge::GameObject* gobj = getGameObject(L, 1);
	float x = (float)lua_tonumber(L, 2);
	float y = (float)lua_tonumber(L, 3);
	float z = (float)lua_tonumber(L, 4);
	gobj->translateLocal(x, y, z);
	return 0;
}