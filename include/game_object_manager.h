#ifndef _GAME_OBJECT_MANAGER_H_
#define _GAME_OBJECT_MANAGER_H_
#include <stdafx.h>
#include <singleton.h>

#define GAME_OBJECT_MANAGER ssuge::GameObjectManager::getSingletonPtr()

namespace ssuge
{
	// Forward declaration to GameObject
	class GameObject;

	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// The GameObjectManager (or GOM for short) is a collection of GameObjects.  This class
	/// is a "Factory" for GameObjects (which means it is the only way to create / destroy
	/// GameObjects, plus there are ways to access a GameObject via the GOM).  The organization
	/// is pretty simple for now -- we have any number of uniquely-named groups.  Within that group
	/// is a collection of uniquely named GameObjects (a map-of-maps) -- in theory a GameObject name
	/// only has to be unique within the group, but when finding a GameObject solely by name, it is 
	/// deterministic which one will be returned.
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class GameObjectManager : public Singleton<GameObjectManager>
	{
	// ***** ATTRIBUTES *****
	protected:
		/// The master list of game objects
		std::map<std::string, std::map<std::string, GameObject*>> mGameObjects;

	// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		/// Constructor
		GameObjectManager();

		/// Destructor
		~GameObjectManager();

	// ***** GETTERS / SETTERS *****
	public:
		/// Gets a game object (NULL if it doesn't exist) -- more costly: searches through all groups
		GameObject * getGameObject(std::string game_object_name);

		/// Gets a game object within a group (NULL if it doesn't exist)
		GameObject * getGameObject(std::string game_object_name, std::string group_name);

		/// Gets all game objects within a group.  result is an output parameter (and is cleared upon calling this)
		void getGameObjects(std::string group_name, std::vector<ssuge::GameObject*>& result);

		/// Returns true if there is a group by this name
		bool hasGroup(std::string group_name);

	// ***** METHODS ***** 
	protected:
		/// A recursive helper method to parse an xml file
		void parseSceneFileHelper(std::string group_name, tinyxml2::XMLElement * cur_elem, GameObject * current_parent = NULL);

		/// A helper function which processes a game-object creating node tag in the xml file.  Returns
		/// the game object that was created.
		GameObject* processGameObjectNode(std::string group_name, tinyxml2::XMLElement * node, GameObject * current_parent);

		/// Process a light sub-tree
		void processLight(GameObject * gobj, tinyxml2::XMLElement* elem);

		/// Process a camera sub-tree
		void processCamera(GameObject * gobj, tinyxml2::XMLElement * elem);

		/// A helper which pulls out a Vector3 from the given element
		Ogre::Vector3 processVector3(tinyxml2::XMLElement * node);

		/// A helper which pulls out a Quaternion from the given element
		Ogre::Quaternion processQuaternion(tinyxml2::XMLElement * node);

		/// A helper which pulls out a Colour from the given element
		Ogre::ColourValue processColour(tinyxml2::XMLElement * node);

		/// Get the attributes of the given node as a map of string => string
		void getNodeAttributes(tinyxml2::XMLElement * elem, std::map<std::string, std::string> & attribs_out);
	public:
		/// Parses a.scene (xml) file.  If the file is called foo.scene, creates a group by the name "foo" 
		/// and adds all objects discovered in the file to that group
		void parseSceneFile(std::string fname);

		/// Destroys a game object (faster version) if you know the group name
		bool destroyGameObject(std::string group_name, std::string gobj_name);

		/// Destroy a game object (slower version) if you don't know the group name.  Returns true if that game object was destroyed
		/// (i.e. was it found)
		bool destroyGameObject(std::string gobj_name);

		/// Destroys all game objects within the given group.  If destroy_group is true, also remove the group.  
		/// Returns true if that game object was destroyed (i.e. was it found).
		void groupDestroy(std::string group_name, bool destroy_group);

		/// Sets the visibility of all game objects within the given group.
		void setVisibility(std::string group_name, bool is_visible);

		/// Creates a new game object
		GameObject * createGameObject(std::string group_name, std::string object_name, GameObject * parent = NULL, std::string script_path = "", unsigned int tag = 0);

		/// Updates all game objects
		void update(float dt);
	};
}

#endif
