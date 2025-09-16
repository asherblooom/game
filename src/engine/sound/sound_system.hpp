#ifndef SOUND_SYSTEM_HPP
#define SOUND_SYSTEM_HPP

#include <AL/alc.h>

// TODO: make singleton??
class SoundSystem {
public:
	SoundSystem();
	~SoundSystem();

private:
	ALCdevice* device;
	ALCcontext* context;
};

#endif
