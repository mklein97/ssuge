#ifndef _LIGHT_COMPONENT_H_
#define _LIGHT_COMPONENT_H_
#include <stdafx.h>
#include <component.h>

namespace ssuge
{
	// Foward declaration to GameObject
	class GameObject;

	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	/// A light instance (point, spotlight and directional are supported)
	/// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
	class LightComponent : public Component
	{
		// ***** ATTRIBUTES *****
	protected:
		/// The ogre entity
		Ogre::Light * mLight;

		// ***** CONSTRUCTOR / DESTRUCTOR *****
	public:
		LightComponent(GameObject * gobj, Ogre::Light::LightTypes light_type);

		virtual ~LightComponent();

		// ***** GETTER / SETTERS *****
	public:
		ComponentType getType() override
		{
			return ComponentType::LIGHT;
		}

		void setDiffuseColour(Ogre::ColourValue c);

		void setSpecularColour(Ogre::ColourValue c);

		void setCastShadows(bool do_cast);

		void setSpotlightAngles(float inner_degrees, float outer_degrees, float falloff);

		void setLightAttenuation(float constant, float linear, float quadratic, float range);
	};
}

#endif

