#pragma once
#include <stdafx.h>

namespace ssuge
{
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// This namespace is for the C implementation of functions and methods we're exposing in the (lua) ssuge module
	/// created by the ScriptManager.  It's kind of a pain, but when adding a new function, add the declaration 
	/// here, put the body in script_functions.cpp and also add an entry in the array of luaL_Reg 
	/// structures (at the top of script_manager.cpp)
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	namespace script
	{
		/// lua_usage: createGameObject("bob's-group", "bob") -> game_object_table
		///			   createGameObject("bob's-group", "bob", "bob_script.lua") -> game_object_table
		int createGameObject(lua_State * L);

		/// lua_usage: getGameObject("bob's-group", "bob") -> game_object_table (or nil if not found)
		int getGameObject(lua_State * L);

		/// lua_usage: isKeyDown("a") -> boolean
		///			   isKeyDown("space") -> boolean
		int isKeyDown(lua_State * L);

		/// lua_usage: isMouseButtonDown("Left/Right/Middle") -> boolean
		int isMouseButtonDown(lua_State * L);

		int setBGM(lua_State * L);

		int deleteGameObject(lua_State * L);

		/// lua_usage:
		int getMousePos(lua_State * L);

		int isControllerButtonDown(lua_State * L);

		int getControllerAxisOrTriggerValue(lua_State * L);

		/// lua_usage: loadScene("my_level.scene") -> nil
		int loadScene(lua_State * L);

		/// lua_usage: log("my log message!")
		int log(lua_State * L);

		/// lua_usage: registerInputListener(game_object_table)
		int registerInputListener(lua_State * L);

		/// lua_usage: setBackgroundColour(0.3, 0.8, 1.0) -> nil         -- 0.0...1.0 range (float arguments)
		///			   setBackgroundColour(45, 200, 255) -> nil			 -- 0...255 range (int arguments)
		int setBackgroundColour(lua_State * L);
	}
}
