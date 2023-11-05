#include <stdafx.h>
#include <game_object_manager.h>
#include <game_object.h>
#include <log_manager.h>
#include <application.h>
#include <script_manager.h>

// The icky template-specialization / static-initializer for GOM's msSingleton attribute
template<> ssuge::GameObjectManager* ssuge::Singleton<ssuge::GameObjectManager>::msSingleton = NULL;

ssuge::GameObjectManager::GameObjectManager()
{
	// Nothing needed here -- yet.
}

ssuge::GameObjectManager::~GameObjectManager()
{
	// Destroy all game objects
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		groupDestroy(it->first, true);
		SCRIPT_MANAGER->removeGroup(it->first);
		++it;
	}
}

ssuge::GameObject * ssuge::GameObjectManager::getGameObject(std::string game_object_name)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::map<std::string, GameObject*>::iterator gobj_iter = it->second.find(game_object_name);
		if (gobj_iter != it->second.end())
		{
			return gobj_iter->second;
		}
		++it;
	}
	return NULL;
}


ssuge::GameObject * ssuge::GameObjectManager::getGameObject(std::string game_object_name, std::string group_name)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.find(group_name);
	if (it != mGameObjects.end())
	{
		std::map<std::string, GameObject*>::iterator gobj_iter = it->second.find(game_object_name);
		if (gobj_iter != it->second.end())
		{
			return gobj_iter->second;
		}
	}
	return NULL;
}

void ssuge::GameObjectManager::getGameObjects(std::string group_name, std::vector<ssuge::GameObject*>& result)
{
	result.clear();
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.find(group_name);
	if (it != mGameObjects.end())
	{
		std::map<std::string, GameObject*>::iterator gobj_iter = it->second.begin();
		while (gobj_iter != it->second.end())
		{
			result.push_back(gobj_iter->second);
			++gobj_iter;
		}
	}
}

bool ssuge::GameObjectManager::hasGroup(std::string group_name)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.find(group_name);
	return it != mGameObjects.end();
}




ssuge::GameObject* ssuge::GameObjectManager::processGameObjectNode(std::string group_name, tinyxml2::XMLElement * node, GameObject * current_parent)
{
	// Get the name of the game object
	std::map<std::string, std::string> gobj_attribs;
	getNodeAttributes(node, gobj_attribs);
	std::map<std::string, std::string>::iterator it = gobj_attribs.find("name");
	std::string name = "";
	if (it != gobj_attribs.end())
		name = it->second;
	std::string script_path = "";
	
	// See if we have a script user property field
	tinyxml2::XMLElement * udata = node->FirstChildElement("userData");
	if (udata != NULL)
	{
		udata = udata->FirstChildElement("property");
		const tinyxml2::XMLAttribute * uatt = udata->FindAttribute("name");
		if (uatt != NULL && !strcmp(uatt->Value(), "script"))
			script_path = std::string(udata->FindAttribute("data")->Value());
	}

	// Create the Game Object
	GameObject * gobj = createGameObject(group_name, name, current_parent, script_path);

	// Get the "normal" data about the game object
	tinyxml2::XMLElement * temp_elem;
	if ((temp_elem = node->FirstChildElement("position")))  // Note the *disgusting* assignement + use notation being used...
		gobj->setPosition(processVector3(temp_elem));
	if ((temp_elem = node->FirstChildElement("scale")))
		gobj->setScale(processVector3(temp_elem));
	if ((temp_elem = node->FirstChildElement("rotation")))
		gobj->setOrientation(processQuaternion(temp_elem));


	// Look for components to add to the game object
	// ... mesh component
	if ((temp_elem = node->FirstChildElement("entity")))
	{
		std::map<std::string, std::string> attribs;
		getNodeAttributes(temp_elem, attribs);
		std::map<std::string, std::string>::iterator it = attribs.find("meshFile");
		if (it != attribs.end())
			gobj->createMeshComponent(it->second);
	}
	// ... light component
	if ((temp_elem = node->FirstChildElement("light")))
		processLight(gobj, temp_elem);
	// ... camera component
	if ((temp_elem = node->FirstChildElement("camera")))
		processCamera(gobj, temp_elem);
	
	return gobj;
}


void ssuge::GameObjectManager::processLight(GameObject * gobj, tinyxml2::XMLElement* elem)
{
	std::map<std::string, std::string> attribs;
	getNodeAttributes(elem, attribs);
	std::map<std::string, std::string>::iterator it = attribs.find("type");
	if (it != attribs.end())
	{
		if (it->second == "point")
			gobj->createLightComponent(Ogre::Light::LightTypes::LT_POINT);
		else if (it->second == "spot")
			gobj->createLightComponent(Ogre::Light::LightTypes::LT_SPOTLIGHT);
	}
	LightComponent* lcomp = gobj->getLightComponent();
	lcomp->setCastShadows(true);
	if (lcomp != NULL)
	{
		tinyxml2::XMLElement * temp_elem;
		if ((temp_elem = elem->FirstChildElement("colourDiffuse")))
			lcomp->setDiffuseColour(processColour(temp_elem));
		if ((temp_elem = elem->FirstChildElement("colourSpecular")))
			lcomp->setSpecularColour(processColour(temp_elem));
		if ((temp_elem = elem->FirstChildElement("lightRange")))
		{
			float inner = 20.0f, outer = 50.0f, falloff = 1.0f;
			float rad_to_deg = 180.0f / 3.1416f;
			const tinyxml2::XMLAttribute * att = temp_elem->FindAttribute("inner");
			if (att != NULL)
				inner = att->FloatValue() * rad_to_deg;
			att = temp_elem->FindAttribute("outer");
			if (att != NULL)
				outer = att->FloatValue() * rad_to_deg;
			att = temp_elem->FindAttribute("falloff");
			if (att != NULL)
				falloff = att->FloatValue();
			lcomp->setSpotlightAngles(inner, outer, falloff);
		}
		if ((temp_elem = elem->FirstChildElement("lightAttenuation")))
		{
			float constant = 1.0f, linear = 0.0f, quadratic = 0.0f, range = 100.0f;
			const tinyxml2::XMLAttribute * att = temp_elem->FindAttribute("constant");
			if (att != NULL)		
				constant = att->FloatValue();
			att = temp_elem->FindAttribute("linear");
			if (att != NULL)
				linear = att->FloatValue();
			att = temp_elem->FindAttribute("quadratic");
			if (att != NULL)
				quadratic = att->FloatValue();
			att = temp_elem->FindAttribute("range");
			if (att != NULL)
				range = att->FloatValue();
			lcomp->setLightAttenuation(constant, linear, quadratic, range);
		}
	}
}


void ssuge::GameObjectManager::processCamera(GameObject * gobj, tinyxml2::XMLElement* elem)
{
	CameraComponent * cc = gobj->createCameraComponent(true);
	const tinyxml2::XMLAttribute * att = elem->FindAttribute("fov");
	if (att != NULL)
		cc->setFieldOfViewAngle(att->FloatValue() * 2.0f);
	tinyxml2::XMLElement * clip_elem = elem->FirstChildElement("clipping");
	if (clip_elem != NULL)
	{
		float far = 1000.0f, near = 1.0f;
		att = clip_elem->FindAttribute("far");
		if (att != NULL)
			far = att->FloatValue();
		att = clip_elem->FindAttribute("near");
		if (att != NULL)
			near = att->FloatValue();
		cc->setClipDistances(near, far);
	}
}


Ogre::Vector3 ssuge::GameObjectManager::processVector3(tinyxml2::XMLElement * node)
{
	Ogre::Vector3 v(0, 0, 0);
	const tinyxml2::XMLAttribute * attrib = node->FirstAttribute();
	while (attrib != NULL)
	{
		std::string attrib_name = attrib->Name();
		if (attrib_name == "x")
			v.x = attrib->FloatValue();
		else if (attrib_name == "y")
			v.y = attrib->FloatValue();
		else if (attrib_name == "z")
			v.z = attrib->FloatValue();
		attrib = attrib->Next();
	}
	return v;
}

Ogre::Quaternion ssuge::GameObjectManager::processQuaternion(tinyxml2::XMLElement * node)
{
	float qx = 1.0f, qy = 0.0f, qz = 0.0f, qw = 0.0f;
	// qw="0.570947" qx="0.169076" qy="0.272171" qz="0.755880"
	const tinyxml2::XMLAttribute * attrib = node->FirstAttribute();
	while (attrib != NULL)
	{
		std::string attrib_name = attrib->Name();
		if (attrib_name == "qx")
			qx = attrib->FloatValue();
		else if (attrib_name == "qy")
			qy = attrib->FloatValue();
		else if (attrib_name == "qz")
			qz = attrib->FloatValue();
		else if (attrib_name == "qw")
			qw = attrib->FloatValue();
		attrib = attrib->Next();
	}
	return Ogre::Quaternion(qw, qx, qy, qz);
}


Ogre::ColourValue ssuge::GameObjectManager::processColour(tinyxml2::XMLElement * node)
{
	Ogre::ColourValue c(0.0f, 0.0f, 0.0f);
	const tinyxml2::XMLAttribute * attrib = node->FirstAttribute();
	while (attrib != NULL)
	{
		std::string attrib_name = attrib->Name();
		if (attrib_name == "r")
			c.r = attrib->FloatValue();
		else if (attrib_name == "g")
			c.g = attrib->FloatValue();
		else if (attrib_name == "b")
			c.b = attrib->FloatValue();
		attrib = attrib->Next();
	}
	return c;
}


void ssuge::GameObjectManager::getNodeAttributes(tinyxml2::XMLElement * elem, std::map<std::string, std::string> & attribs_out)
{
	// Get the attributes of this tag.
	const tinyxml2::XMLAttribute * cur_attrib = elem->FirstAttribute();
	attribs_out.clear();
	while (cur_attrib != NULL)
	{
		attribs_out[cur_attrib->Name()] = cur_attrib->Value();
		cur_attrib = cur_attrib->Next();
	}
}


void ssuge::GameObjectManager::parseSceneFileHelper(std::string group_name, tinyxml2::XMLElement * cur_elem, GameObject * current_parent)
{
	// Get the name of this tag
	std::string tag_name = cur_elem->Value();

	// See if this is a node attribute
	if (tag_name == "node")
		current_parent = processGameObjectNode(group_name, cur_elem, current_parent);
	else if (tag_name == "colourBackground")
		APPLICATION->setBackgroundColor(processColour(cur_elem));

	// Process the children
	tinyxml2::XMLElement * child_elem = cur_elem->FirstChildElement();
	while (child_elem != NULL)
	{
		parseSceneFileHelper(group_name, child_elem, current_parent);
		child_elem = child_elem->NextSiblingElement();
	}
}

void ssuge::GameObjectManager::parseSceneFile(std::string fname)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError err = doc.LoadFile(fname.c_str());
	if (err != tinyxml2::XMLError::XML_SUCCESS)
		return;

	// Get the name of the file, minus path information and the extension -- use that as the group name
	int start = fname.rfind("/");
	if (start < 0)
		start = fname.rfind("\\");
	if (start < 0)
		start = 0;
	int end = fname.rfind(".");
	std::string group_name = fname.substr(start + 1, end - start - 1);
	
	tinyxml2::XMLElement * elem = doc.RootElement();
	parseSceneFileHelper(group_name, elem);
}


bool ssuge::GameObjectManager::destroyGameObject(std::string group_name, std::string gobj_name)
{
	bool destroyed = false;
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.find(group_name);
	if (it != mGameObjects.end())
	{
		std::map<std::string, GameObject*>::iterator gobj_iter = it->second.find(gobj_name);
		if (gobj_iter != it->second.end())
		{
			delete gobj_iter->second;
			SCRIPT_MANAGER->removeGameObject(it->first, gobj_iter->first);
			it->second.erase(gobj_iter);
			destroyed = true;
		}
	}
	return destroyed;
}



bool ssuge::GameObjectManager::destroyGameObject(std::string gobj_name)
{
	bool destroyed = false;
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.begin();
	while (it != mGameObjects.end())
	{
		std::map<std::string, GameObject*>::iterator gobj_iter = it->second.find(gobj_name);
		if (gobj_iter != it->second.end())
		{
			delete gobj_iter->second;
			SCRIPT_MANAGER->removeGameObject(it->first, gobj_iter->first);
			it->second.erase(gobj_iter);
			destroyed = true;
			break;
		}

		++it;
	}
	return destroyed;
}


void ssuge::GameObjectManager::groupDestroy(std::string group_name, bool destroy_group)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.find(group_name);
	if (it != mGameObjects.end())
	{
		std::map<std::string, GameObject*>::iterator gobj_iter = it->second.begin();
		while (gobj_iter != it->second.end())
		{
			delete gobj_iter->second;
			++gobj_iter;
		}
		it->second.clear();
		SCRIPT_MANAGER->removeGroup(it->first);		// Should "destroy" the lua game objects in this group

		if (destroy_group)
			mGameObjects.erase(it);
	}
}

	
void ssuge::GameObjectManager::setVisibility(std::string group_name, bool is_visible)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator it = mGameObjects.find(group_name);
	if (it != mGameObjects.end())
	{
		std::map<std::string, GameObject*>::iterator gobj_iter = it->second.begin();
		while (gobj_iter != it->second.end())
		{
			gobj_iter->second->setVisibility(is_visible);
			++gobj_iter;
		}
	}
}


ssuge::GameObject * ssuge::GameObjectManager::createGameObject(std::string group_name, std::string object_name, GameObject * parent, std::string script_path, unsigned int tag)
{
	// Make the C++ game object and add it to the map
	GameObject * gobj = new GameObject(object_name, group_name, tag, parent);
	mGameObjects[group_name][object_name] = gobj;

	// Create a lua counter-part for that game object
	SCRIPT_MANAGER->createLuaGameObject(gobj, group_name, object_name, script_path);

	// If this object has a script, make the game object "script-aware" and get the methods from the given lua script.
	if (script_path != "")
	{
		gobj->setScriptAware(true);
	}
	return gobj;
}



void ssuge::GameObjectManager::update(float dt)
{
	std::map<std::string, std::map<std::string, GameObject*>>::iterator giter = mGameObjects.begin();
	while (giter != mGameObjects.end())
	{
		std::map<std::string, GameObject*>::iterator oiter = giter->second.begin();
		while (oiter != giter->second.end())
		{
			oiter->second->update(dt);
			++oiter;
		}
		++giter;
	}
}
