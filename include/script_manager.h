#pragma once
#include <stdafx.h>
#include <singleton.h>

#define SCRIPT_MANAGER ssuge::ScriptManager::getSingletonPtr()

namespace ssuge
{
	// Forward declaration to GameObject class
	class GameObject;

	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// The script manager houses the lua interpreter and has methods for accessing the lua
	/// world from c++ world.
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class ScriptManager : public Singleton<ScriptManager>
	{
	protected:
		lua_State * mLuaInterpreter;

		bool mStarted;
		std::vector<GameObject*> mNewScriptAwareObjects;
	public:
		ScriptManager();
		~ScriptManager();
		
		/// This method just opens, compiles, and runs a lua script.
		void loadScript(std::string fscript_name);

		/// This method is similar to loadScript, but it cycles through all things in the script referenced
		/// by script_fname and appends them to the lua game-object table.  
		void appendScriptModule(std::string group_name, std::string object_name, std::string script_fname);

		/// This method creates a lua game object to mirror the C++ game object that is passed to this method.
		void createLuaGameObject(GameObject* gobj, std::string group_name, std::string object_name, std::string script_path="");

		/// This method removes the lua counterpart to the given game object
		void removeGameObject(std::string group_name, std::string object_name);

		/// This method removes an entire group of lua game objects from _allGameObjects
		void removeGroup(std::string group_name);

		/// Executes a script method of the given game object (if it exists).  The elements in params (if any) are used to create a lua table
		/// that can be used to pass parameters to the script.
		void executeMethod(std::string group_name, std::string object_name, std::string method_name, std::map<std::string, std::string>& params);
	
		/// Used as an error-handler in lua_pcall calls
		static int error_handler(lua_State * L);

		/// Used to generate a stack-message in the ssuge log
		static void stack_inspector(lua_State * L);

		/// Attempts to find the lua game object with the given group and object name.  This method
		/// adds one element to the stack (a lua Game-Object table if it was found, or nil if not).  The
		/// function returns true if the game object was found.
		bool getLuaGameObject(std::string group_name, std::string object_name);

		/// Called every frame by the Application class
		int update(float dt);
	};
}
