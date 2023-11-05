#pragma once
#include <stdafx.h>
#include <component.h>
#include <irrKlang.h>

namespace ssuge
{
	using namespace irrklang;
	// Foward declaration to GameObject
	class GameObject;

	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// Sound component for a game object through irrKlang
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class SoundComponent : public Component
	{
		// ***** ATTRIBUTES *****
	protected:
		ISoundEngine* mEngine;
		std::string sName;
		ISound* sound;

		// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		SoundComponent(GameObject * gobj, std::string sname);

		virtual	~SoundComponent();

		// ***** GETTER / SETTERS *****
	public:
		ComponentType getType() override
		{
			return ComponentType::SOUND;
		}
		void setPosition(int x, int y, int z);
		void play(bool isPlaying);
	};
}
