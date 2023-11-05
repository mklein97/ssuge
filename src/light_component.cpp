#include <stdafx.h>
#include <light_component.h>
#include <application.h>
#include <game_object.h>

ssuge::LightComponent::LightComponent(GameObject * gobj, Ogre::Light::LightTypes light_type) : ssuge::Component(gobj)
{
	mLight = APPLICATION->getSceneManager()->createLight(mOwner->getName() + "_light");
	mLight->setType(light_type);
	//mLight->setCastShadows(true);
	mOwner->getSceneNode()->attachObject(mLight);
}


ssuge::LightComponent::~LightComponent()
{
	if (mLight)
	{
		mOwner->getSceneNode()->detachObject(mLight);
		APPLICATION->getSceneManager()->destroyLight(mLight);
	}
}


void ssuge::LightComponent::setDiffuseColour(Ogre::ColourValue c)
{
	if (mLight)
		mLight->setDiffuseColour(c);
}


void ssuge::LightComponent::setSpecularColour(Ogre::ColourValue c)
{
	if (mLight)
		mLight->setSpecularColour(c);
}



void ssuge::LightComponent::setCastShadows(bool do_cast) 
{ 
	if (mLight)
		mLight->setCastShadows(true); 
}



void ssuge::LightComponent::setSpotlightAngles(float inner_degrees, float outer_degrees, float falloff) 
{ 
	if (mLight)
		mLight->setSpotlightRange(Ogre::Degree(inner_degrees), Ogre::Degree(outer_degrees), falloff); 
}



void ssuge::LightComponent::setLightAttenuation(float constant, float linear, float quadratic, float range) 
{ 
	if (mLight)
		mLight->setAttenuation(range, constant, linear, quadratic);
}