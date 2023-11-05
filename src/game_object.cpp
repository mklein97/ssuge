#include <stdafx.h>
#include <game_object.h>
#include <application.h>

ssuge::GameObject::GameObject(std::string name, std::string group_name, int tag, GameObject * parent) :
	mName(name), mGroupName(group_name), mTag(tag), mScriptAware(false)
{
	if (parent != NULL)
		mSceneNode = parent->getSceneNode()->createChildSceneNode();
	else
		mSceneNode = APPLICATION->getSceneManager()->getRootSceneNode()->createChildSceneNode();
}

ssuge::GameObject::~GameObject()
{
	// Destroy all components
	std::map<ComponentType, Component*>::iterator it = mComponents.begin();
	while (it != mComponents.end())
	{
		delete it->second;
		++it;
	}
	mComponents.clear();

	// Remove our scene node from the scene
	mSceneNode->getParentSceneNode()->removeChild(mSceneNode);
	APPLICATION->getSceneManager()->destroySceneNode(mSceneNode);
}

ssuge::MeshComponent * ssuge::GameObject::getMeshComponent()
{
	std::map<ComponentType, Component*>::iterator it = mComponents.find(ComponentType::MESH);
	if (it == mComponents.end())
		return NULL;
	else
		return (MeshComponent*)it->second;
}


ssuge::LightComponent * ssuge::GameObject::getLightComponent()
{
	std::map<ComponentType, Component*>::iterator it = mComponents.find(ComponentType::LIGHT);
	if (it == mComponents.end())
		return NULL;
	else
		return (LightComponent*)it->second;
}


ssuge::CameraComponent * ssuge::GameObject::getCameraComponent()
{
	std::map<ComponentType, Component*>::iterator it = mComponents.find(ComponentType::CAMERA);
	if (it == mComponents.end())
		return NULL;
	else
		return (CameraComponent*)it->second;
}

ssuge::SoundComponent * ssuge::GameObject::getSoundComponent()
{
	auto it = mComponents.find(ComponentType::SOUND);
	if (it == mComponents.end())
		return NULL;
	else
		return (SoundComponent*)it->second;
}


ssuge::MeshComponent * ssuge::GameObject::createMeshComponent(std::string fname)
{
	// Create the new component
	MeshComponent * comp = new MeshComponent(this, fname);

	// See if we currently have a mesh component -- if so, delete it
	std::map<ComponentType, Component*>::iterator it = mComponents.find(ComponentType::MESH);
	if (it != mComponents.end())
		delete it->second;
	
	// Add the new component to our map
	mComponents[ComponentType::MESH] = comp;

	// Return the new component
	return comp;
}


ssuge::LightComponent * ssuge::GameObject::createLightComponent(Ogre::Light::LightTypes lt)
{
	// Create the new component
	LightComponent * comp = new LightComponent(this, lt);

	// See if we currently have a light component -- if so, delete it
	std::map<ComponentType, Component*>::iterator it = mComponents.find(ComponentType::LIGHT);
	if (it != mComponents.end())
		delete it->second;

	// Add the new component to our map
	mComponents[ComponentType::LIGHT] = comp;

	// Return the new component
	return comp;
}

/// Create a camera component and add it to our master list
ssuge::CameraComponent * ssuge::GameObject::createCameraComponent(bool make_active)
{
	// Create the new component
	CameraComponent * comp = new CameraComponent(this);

	// See if we currently have a camera component -- if so, delete it
	std::map<ComponentType, Component*>::iterator it = mComponents.find(ComponentType::CAMERA);
	if (it != mComponents.end())
		delete it->second;

	// Add the new component to our map
	mComponents[ComponentType::CAMERA] = comp;

	// If this should be the active camera, notify the application
	if (make_active)
		APPLICATION->setActiveCamera(comp);

	// Return the new component
	return comp;
}

ssuge::SoundComponent * ssuge::GameObject::createSoundComponent(std::string sname)
{
	SoundComponent * comp = new SoundComponent(this, sname);
	mComponents[ComponentType::SOUND] = comp;
	return comp;
}


void ssuge::GameObject::setParent(GameObject * parent)
{
	// Remove this scene node from it's current parent scene node
	mSceneNode->getParentSceneNode()->removeChild(mSceneNode);

	// Now make it a parent of parent's scene node
	parent->getSceneNode()->addChild(mSceneNode);
}


void ssuge::GameObject::setVisibility(bool is_visible)
{
	mSceneNode->setVisible(is_visible, true);

	// Note: We might have to call a similar method on components.  All
	// Ogre objects should be handled by the above call, but non-Ogre things
	// will need some kind of notification.
}


void ssuge::GameObject::onKeyDown(SDL_Keycode & k)
{
	if (mScriptAware)
	{
		std::map<std::string, std::string> params;
		params["key"] = INPUT_MANAGER->getKeyName(k);
		SCRIPT_MANAGER->executeMethod(mGroupName, mName, "onKeyDown", params);
	}
}

/// This method will be called by the IM when a key goes up
void ssuge::GameObject::onKeyUp(SDL_Keycode & k)
{
	if (mScriptAware)
	{
		std::map<std::string, std::string> params;
		params["key"] = INPUT_MANAGER->getKeyName(k);
		SCRIPT_MANAGER->executeMethod(mGroupName, mName, "onKeyUp", params);
	}
}


void ssuge::GameObject::update(float dt)
{
	if (mScriptAware)
	{
		std::map<std::string, std::string> params;
		params["dt"] = std::to_string(dt);
		SCRIPT_MANAGER->executeMethod(mGroupName, mName, "onUpdate", params);
	}
}
