#include <stdafx.h>
#include <sound_component.h>
#include <game_object.h>

ssuge::SoundComponent::SoundComponent(GameObject * gobj, std::string sname) : ssuge::Component(gobj)
{
	mEngine = createIrrKlangDevice();
	mEngine->setSoundVolume(1.0f);
	mEngine->setDefault3DSoundMaxDistance(1000);
	this->sName = sname;
	sound = mEngine->play3D(sName.c_str(), vec3df(mOwner->getPosition().x, mOwner->getPosition().y, -mOwner->getPosition().z), false, false, true);
}

ssuge::SoundComponent::~SoundComponent()
{
	if (sound)
		sound->drop();
	mEngine->drop();
}

void ssuge::SoundComponent::setPosition(int x, int y, int z)
{
	sound->setPosition(vec3df(x, y, z));
}

void ssuge::SoundComponent::play(bool isPlaying)
{
	if (isPlaying)
		sound->setIsPaused(false);

	if (!isPlaying)
		sound->setIsPaused(true);
}