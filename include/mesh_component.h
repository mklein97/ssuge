#pragma once
#include <stdafx.h>
#include <component.h>

namespace ssuge
{
	// Foward declaration to GameObject
	class GameObject;

	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// A mesh instance of an ogre .mesh binary file.  A single .mesh file can be instanced
	/// multiple times to save memory (this is done automatically by Ogre)
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class MeshComponent : public Component
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The ogre entity
		Ogre::Entity * mEntity;

	// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		MeshComponent(GameObject * gobj, std::string fname);

		virtual ~MeshComponent();

	// ***** GETTER / SETTERS *****
	public:
		ComponentType getType() override
		{
			return ComponentType::MESH;
		}
	};
}
