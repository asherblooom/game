#ifndef SOUND_SYSTEM_HPP
#define SOUND_SYSTEM_HPP

#include <AL/alc.h>
#include "sound.hpp"

// TODO: make singleton??
class SoundSystem {
public:
	// set blocking to true if you want game to pause until sound has finished
	void Play(Sound sound, bool blocking = false);
};

#endif
