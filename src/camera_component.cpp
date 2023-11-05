#include <stdafx.h>
#include <camera_component.h>
#include <application.h>
#include <game_object.h>

ssuge::CameraComponent::CameraComponent(GameObject * gobj) : ssuge::Component(gobj)
{
	mCamera = APPLICATION->getSceneManager()->createCamera(mOwner->getName() + "_camera");
	mOwner->getSceneNode()->attachObject(mCamera);
}

ssuge::CameraComponent::~CameraComponent()
{
	if (mCamera)
	{
		mOwner->getSceneNode()->detachObject(mCamera);
		APPLICATION->getSceneManager()->destroyCamera(mCamera);
	}
}



void ssuge::CameraComponent::setClipDistances(float near, float far)
{
	if (mCamera)
	{
		mCamera->setNearClipDistance(near);
		mCamera->setFarClipDistance(far);
	}
}