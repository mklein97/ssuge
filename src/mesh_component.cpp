#include <stdafx.h>
#include <mesh_component.h>
#include <application.h>
#include <game_object.h>

ssuge::MeshComponent::MeshComponent(GameObject * gobj, std::string fname) : ssuge::Component(gobj)
{
	mEntity = APPLICATION->getSceneManager()->createEntity(gobj->getName() + "_entity", fname);
	mEntity->setCastShadows(true);
	mOwner->getSceneNode()->attachObject(mEntity);
}

ssuge::MeshComponent::~MeshComponent()
{
	if (mEntity)
	{
		Ogre::SceneNode * snode = mEntity->getParentSceneNode();	// Should be mOwner->getSceneNode()
		snode->detachObject(mEntity);
		APPLICATION->getSceneManager()->destroyEntity(mEntity);
	}
}
