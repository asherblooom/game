#ifndef SOUND_SYSTEM_HPP
#define SOUND_SYSTEM_HPP

#include <AL/alc.h>
#include "sound.hpp"

// TODO: make singleton??
class SoundSystem {
public:
	SoundSystem();
	~SoundSystem();

	void Play(Sound sound);

private:
	ALCdevice* device;
	ALCcontext* context;
};

#endif
