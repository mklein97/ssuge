#pragma once
#include <stdafx.h>

namespace ssuge
{
	// Forward declaration to GameObject class
	class GameObject;
	
	// This is where I'll place declarations for all lua game-object methods.  Put the 
	// declaration here, the body in script_game_object.cpp, and add an entry to the
	// array of luaL_Reg structures at the top of script_manager.cpp (below the top-
	// level function array
	namespace script
	{
		/// this method isn't directly exposed to lua.  It's used as a convenience
		/// function by the other methods.  Its role is to get the _udata_ field 
		/// from the game-object-instance table (at stack position stack_pos), 
		/// then get the C++ game object pointer from that and return it.  NULL
		/// is returned if this fails.  This function should leave the stack
		/// unchanged.
		ssuge::GameObject * getGameObject(lua_State * L, int stack_pos);

		/// This method also isn't exposed directly to lua.  It's used as a metamethod
		/// of the global (lua) GameObject "class".  Its job is to see first if the GameObject
		/// class has that field.  If so, it is returned.  If not, a few special names
		/// are checked (like the name of group of the game object) -- these are returned
		/// in lua using C++ values.  If those both fail, nil is returned on the stack.
		/// Coming into this function, it is expected that the stack just have a table (the 
		/// game object instance table) and a key (a string) on the stack.
		int script_GameObject__index(lua_State * L);

		/// lua_usage: gameobject:createMeshComponent("robot.mesh") -> nil
		int script_GameObject_createMeshComponent(lua_State * L);

		/// lua_usage: gameobject:createSoundComponent("explosion.wav") -> nil
		int script_GameObject_createSoundComponent(lua_State * L);

		int script_GameObject_playSoundComponent(lua_State * L);

		/// lua_usage: gameobject:rotate(degrees, axis_x, axis_y, axis_y) -> nil    (rotates in world space)
		int script_GameObject_rotate(lua_State * L);

		int script_GameObject_getName(lua_State * L);

		int script_GameObject_getGroupName(lua_State * L);

		int script_GameObject_isColliding(lua_State * L);

		/// lua_usage: gameobject:setPosition(x, y, z) -> nil  (sets position relative to parent's coordinate space)
		int script_GameObject_setPosition(lua_State * L);

		int script_GameObject_getPosition(lua_State * L);

		/// lua_usage: gameobject:setScale(x, y, z) -> nil     (sets scale relative to parent's coordinate space)
		int script_GameObject_setScale(lua_State * L);

		/// lua_usage: gameobject:translate(x, y, z) -> nil    (translates relative to this object's local coordinate space)
		int script_GameObject_translate(lua_State * L);
	}
}
