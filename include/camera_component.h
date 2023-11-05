#ifndef _CAMERA_COMPONENT_H_
#define _CAMERA_COMPONENT_H_
#include <stdafx.h>
#include <component.h>

namespace ssuge
{
	// Foward declaration to GameObject
	class GameObject;

	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// A 3d camera public Component
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class CameraComponent : public Component
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The ogre entity
		Ogre::Camera * mCamera;

	// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		CameraComponent(GameObject * gobj);

		virtual ~CameraComponent();

	// ***** GETTER / SETTERS *****
	public:
		ComponentType getType() override
		{
			return ComponentType::CAMERA;
		}

		/// Sets the near and far clip distance
		void setClipDistances(float near, float far);

		/// sets the fov
		void setFieldOfViewAngle(float radians)
		{
			mCamera->setFOVy(Ogre::Radian(radians));
		}

	// ***** FRIENDS *****
	friend class Application;
	};
}

#endif

