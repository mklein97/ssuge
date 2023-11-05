#pragma once
#include <stdafx.h>

namespace ssuge
{
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// This is the base class for any class which wishes to register itself
	/// as a listener with the InputManager (IM) class.  
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class InputObserver
	{
	public:
		/// This method will be called by the IM when a key goes down
		virtual void onKeyDown(SDL_Keycode & k) {}

		/// This method will be called by the IM when a key goes up
		virtual void onKeyUp(SDL_Keycode & k) {}
	};
}
