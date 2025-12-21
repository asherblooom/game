#include "sound_system.hpp"

void SoundSystem::Play(Sound sound, bool blocking) {
	alGetError();
	alSourcePlay(sound.source);
	if (blocking) {
		sound.State = AL_PLAYING;
		while (sound.State == AL_PLAYING) {
			alGetSourcei(sound.source, AL_SOURCE_STATE, &sound.State);
		}
	}
}
