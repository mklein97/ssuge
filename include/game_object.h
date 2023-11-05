#pragma once
#include <stdafx.h>
#include <component.h>
#include <mesh_component.h>
#include <light_component.h>
#include <camera_component.h>
#include <sound_component.h>
#include <input_observer.h>

namespace ssuge
{
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// A THING in our game (bullet, player, trigger-point, etc.) Contains a collection of 
	/// components that give it functionality. By itself, the game object is basically a node 
	/// within a scene graph (and so has a position, orientation, scale, and the ability
	/// to child to another GameObject) and not much else.  GameObject inherits from 
	/// InputObjserver so a particular game object
	/// can be registered as a listener with the InputManager
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class GameObject : public InputObserver
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The name of this object 
		std::string mName;

		/// The group this game object belongs to.  I'm not crazy about having this here.  Currently,
		/// I need it to find the lua game object by the same name.
		std::string mGroupName;

		/// The Ogre Scene Node storing our position / orientation / scale and spot within the hierarchy
		Ogre::SceneNode * mSceneNode;

		/// The integer tag -- we'll use this later...
		int mTag;

		/// Set to false initially -- if the setScriptAware is called with true, this game object
		/// will attempt to call lua script for key events (like onUpdate)
		bool mScriptAware;

		/// The collection of components.  
		std::map<ComponentType, Component*> mComponents;

	// ***** CONSTRUCTORS / DESTRUCTORS *****
	protected:
		/// Constructor
		GameObject(std::string name, std::string group_name, int tag, GameObject * parent = NULL);

		/// Destructor
		virtual ~GameObject();

	// ***** COMPONENT GETTERS *****
	public:
		/// Get the mesh component (or NULL if there is none)
		MeshComponent * getMeshComponent();

		/// Get the light component (or NULL if there is none)
		LightComponent * getLightComponent();

		/// Get the camera component (or NULL if there is none)
		CameraComponent * getCameraComponent();

		/// Get the sound component (or NULL if there is none)
		SoundComponent * getSoundComponent();

	// ***** COMPONENT CREATORS *****
	public:
		/// Create a mesh component and add it to our master list
		MeshComponent * createMeshComponent(std::string fname);

		/// Create a light component and add it to our master list
		LightComponent * createLightComponent(Ogre::Light::LightTypes lt);

		/// Create a camera component and add it to our master list
		CameraComponent * createCameraComponent(bool make_active);

		/// Create a sound component and add it to our master list
		SoundComponent * createSoundComponent(std::string sname);

	// ***** TRANSFORMATION (ABSOLUTE) SETTERS *****
	public:
		/// Makes this game object a child of the given game object, detaching it from its current parent (if any).
		void setParent(GameObject* parent);

		/// Set the positional offset (relative to parent game object) 
		void setPosition(float x, float y, float z) { mSceneNode->setPosition(Ogre::Vector3(x, y, z)); }

		/// Set the positional offset (relative to parent game object) 
		void setPosition(const Ogre::Vector3& v) { mSceneNode->setPosition(v); }

		/// Set the positional offset (relative to world) 
		void setPositionWorld(float x, float y, float z) { mSceneNode->setPosition(mSceneNode->getParentSceneNode()->convertWorldToLocalPosition(Ogre::Vector3(x, y, z))); }

		/// Set the positional offset (relative to world) 
		void setPositionWorld(const Ogre::Vector3& v) { mSceneNode->setPosition(mSceneNode->getParentSceneNode()->convertWorldToLocalPosition(v)); }

		/// Sets the scale offset (relative to the parent game object)
		void setScale(float sx, float sy, float sz) { mSceneNode->setScale(Ogre::Vector3(sx, sy, sz)); }

		/// Sets the scale offset (relative to the parent game object)
		void setScale(const Ogre::Vector3& s) { mSceneNode->setScale(s); }

		/// Sets the rotational offset (relative to the parent game object)
		void setOrientation(float degrees, float vx, float vy, float vz) { mSceneNode->setOrientation(Ogre::Quaternion(Ogre::Degree(degrees), Ogre::Vector3(vx, vy, vz))); }

		/// Sets the rotational offset (relative to the parent game object)
		void setOrientation(const Ogre::Quaternion& q) { mSceneNode->setOrientation(q); }

		/// Makes this object's negative z-axis point towards the given world space position
		void lookAt(float x, float y, float z) { mSceneNode->lookAt(Ogre::Vector3(x, y, z), Ogre::Node::TS_WORLD); }

	// ***** TRANSFORMATION (RELATIVE) SETTERS *****
	public:
		/// Adjusts the rotational offset (relative to the parent game object) -- the passed rotation information is relative to the world axes
		void rotateWorld(float degrees, float vx, float vy, float vz) { mSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(degrees), Ogre::Vector3(vx, vy, vz)), Ogre::Node::TS_WORLD); }

		/// Adjusts the rotational offset (relative to the parent game object) -- the passed rotation information is relative to the world axes
		void rotateWorld(const Ogre::Quaternion& q) { mSceneNode->rotate(q, Ogre::Node::TS_WORLD); }

		/// Adjusts the rotational offset (relative to the parent game object) -- the passed rotation information is relative to the local axes
		void rotateLocal(float degrees, float vx, float vy, float vz) { mSceneNode->rotate(Ogre::Quaternion(Ogre::Degree(degrees), Ogre::Vector3(vx, vy, vz)), Ogre::Node::TS_LOCAL); }

		/// Adjusts the rotational offset (relative to the parent game object) -- the passed rotation information is relative to the local axes
		void rotateLocal(const Ogre::Quaternion& q) { mSceneNode->rotate(q, Ogre::Node::TS_LOCAL); }

		/// Adjusts the positional offset (relative to the parent game object) -- the passed translation information is relative to the world axes
		void translateWorld(float tx, float ty, float tz) { mSceneNode->translate(Ogre::Vector3(tx, ty, tz), Ogre::Node::TS_WORLD); }

		/// Adjusts the positional offset (relative to the parent game object) -- the passed translation information is relative to the world axes
		void translateWorld(const Ogre::Vector3& v) { mSceneNode->translate(v, Ogre::Node::TS_WORLD); }

		/// Adjusts the positional offset (relative to the parent game object) -- the passed translation information is relative to the local axes
		void translateLocal(float tx, float ty, float tz) { mSceneNode->translate(Ogre::Vector3(tx, ty, tz), Ogre::Node::TS_LOCAL); }

		/// Adjusts the positional offset (relative to the parent game object) -- the passed translation information is relative to the local axes
		void translateLocal(const Ogre::Vector3& v) { mSceneNode->translate(v, Ogre::Node::TS_LOCAL); }

		/// Adjusts the scale offset (relative to the parent game object)
		void scale(float sx, float sy, float sz) { mSceneNode->scale(Ogre::Vector3(sx, sy, sz)); }

		/// Adjusts the scale offset (relative to the parent game object)
		void scale(const Ogre::Vector3& s) { mSceneNode->scale(s); }

	// ***** TRANSFORMATION GETTERS *****
	public:
		/// Gets the position of this object relative to the world axes.
		Ogre::Vector3 getWorldPosition() { return mSceneNode->_getDerivedPosition(); }

		/// Gets the position of this object relative to the parent game object's axes.
		Ogre::Vector3 getPosition() { return mSceneNode->getPosition(); }

		/// Gets the orientation of this object relative to the world axes.
		Ogre::Quaternion getWorldOrientation() { return mSceneNode->_getDerivedOrientation(); }

		/// Gets the orientation of this object relative to the parent game object's axes.
		Ogre::Quaternion getOrientation() { return mSceneNode->getOrientation(); }

		/// Gets the scale of this object relative to the world axes.
		Ogre::Vector3 getWorldScale() { return mSceneNode->_getDerivedScale(); }

		/// Gets the scale offset from the parent game object
		Ogre::Vector3 getScale() { return mSceneNode->getScale(); }

	// ***** OTHER GETTERS *****
	public:
		/// Gets the ogre scene node this object is based upon.
		Ogre::SceneNode* getSceneNode() { return mSceneNode; }

		/// Gets the name of this game object
		std::string getName() { return mName; }

		/// Gets the group name of this object
		std::string getGroupName() { return mGroupName;  }

		/// Gets the group name (as a string) of this game object in the GOM
		int getTag() { return mTag;}

		/// Gets the Axis Aligned Bounding Box of the game object
		Ogre::AxisAlignedBox getAABB() { return mSceneNode->_getWorldAABB(); }

	// ***** OVERRIDES from InputObserver *****
	protected:
		/// This method will be called by the IM when a key goes down
		virtual void onKeyDown(SDL_Keycode & k) override;

		/// This method will be called by the IM when a key goes up
		virtual void onKeyUp(SDL_Keycode & k) override;


	// ***** OTHER METHODS *****
	public:
		/// Makes all components of this object active / visible (or not, if the parameter is false)
		void setVisibility(bool is_visible);

		/// Updates this game object (usually called by the GOM)
		void update(float dt);

		/// Makes this game object "script-aware" (so it attempts to call script methods)
		void setScriptAware(bool is_script_aware) { mScriptAware = is_script_aware;  }

	// ***** FRIENDS *****
		/// This is so the GameObjectManager (GOM) can create instances of GameObject
		friend class GameObjectManager;
	};
}
