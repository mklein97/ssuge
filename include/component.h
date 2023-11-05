#pragma once
#include <stdafx.h>

namespace ssuge
{
	/// This is the "master" list of all component types.  It feels
	/// kind of icky to do it here, but I can't think of a better place...
	enum class ComponentType { MESH, CAMERA, LIGHT, SOUND };

	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// A forward declaration to GameObject.  Since we're not actually using
	/// any methods of GameObject here, this is sufficient to be able to 
	/// declare pointers to GameObjects.  If we had included GameObject.h here
	/// (which also includes component.h) we'd have a nasty circular dependency loop
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class GameObject;

	/// The component class is the base for all "actual" components
	class Component
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The containing game object
		GameObject * mOwner;

	// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		Component(GameObject * gobj) : mOwner(gobj)
		{
		
		}

		virtual ~Component() 
		{
		
		}

	// ***** GETTERS / SETTERS *****
	public:
		/// Gets the type of component this really is
		virtual ComponentType getType() = 0;

		/// Gets the parent game object
		GameObject * getOwner()
		{
			return mOwner;
		}
	};
}
