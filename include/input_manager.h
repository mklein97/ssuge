#pragma once
#include <stdafx.h>
#include <singleton.h>
#include <input_observer.h>

#define INPUT_MANAGER ssuge::InputManager::getSingletonPtr()

namespace ssuge
{
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// The InputManager is responsible for all input-related processing (currently using
	/// SDL as the back-end).  There are two ways to interact with the InputManager:
	///   a. Using polling: e.g. getKeyState -- returns true if that key is down.
	///   b. Using an Observer pattern: the user registers an InputObserver-derived
	///      class (like GameObject) with the InputManager and recevies notification
	///      when that key / button / etc. is pressed
	/// The InputManager also supports some abstraction -- instead of referring to key-names 
	/// only (although low-level access like this is supported), the user can opt to use
	/// action / axis names so they need not be aware of what input is actually driving
	/// the InputManager (among keys, mouse, gamepads, etc.).  For example, the action name
	/// "attack" can be bound to the key SDLK_SPACE and the gamepad "A" key.  Then the
	/// person receiving notifications need not worry whether the player is using a gamepad
	/// or keyboard -- they'll both work with no changes.
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class InputManager : public Singleton<InputManager>
	{
	// ***** ATTRIBUTES *****
	protected:
		/// All regsitered listeners -- each of these will be notified by the InputManager
		/// when an input-event happens.
		std::set<InputObserver*> mListeners;
		//mouse button states
		bool bLeft;
		bool bMiddle;
		bool bRight;
		//Controller states
		std::map<std::string, int> Con1;
		std::map<std::string, int> Con2;
		std::map<std::string, int> Con3;
		std::map<std::string, int> Con4;
		std::vector<std::map<std::string, int>> Controllers;
	// ***** CONSTRUCTORS / DESTRUCTOR *****
	public:
		/// Default constructor
		InputManager();

		/// Destructor
		~InputManager();

	// ***** METHODS *****
	public:
		/// Process all waiting input -- returns false if the user closed the main window
		bool update(float dt);

		/// Registers this object as a listener
		void registerListener(InputObserver * o);

		/// Removes this object as a listener
		void deregisterListener(InputObserver * o);

		/// Gets the human-readable string from a keycode
		std::string getKeyName(SDL_Keycode k);

		/// Gets mouse position
		Ogre::Vector2 getMousePos();

		bool isMouseButtonDown(std::string button);

		int getControllerButtonInput(int conNum, std::string button);

		long int getControllerAxisOrTriggerValue(int conNum, std::string name);

		void updateController(int conNum, std::string button, int isPressed);

	protected:
		/// Send a key-down event to all listeners
		void broadcastKeyEvent(SDL_Keycode k, bool key_down);

	// ***** DEVICE-POLLING METHODS *****
	public:
		/// Returns true if this key is currently pressed
		bool isKeyDown(SDL_Keycode k);

		/// Returns true if this key is currently pressed.  Uses the SDL names from https://wiki.libsdl.org/SDL_Keycode 
		bool isKeyDown(std::string keyname);
	};
}
