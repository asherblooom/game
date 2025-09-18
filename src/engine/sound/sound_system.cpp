#include "sound_system.hpp"
#include <stdexcept>

SoundSystem::SoundSystem() {
	device = alcOpenDevice(nullptr);
	if (!device)
		throw std::runtime_error("ERROR::SOUNDSYSTEM: Could not find an audio device");

	context = alcCreateContext(device, nullptr);
	if (!context)
		throw std::runtime_error("ERROR::SOUNDSYSTEM: Could not create audio context");

	if (!alcMakeContextCurrent(context))
		throw std::runtime_error("ERROR::SOUNDSYSTEM: Could not make audio context current");
}

SoundSystem::~SoundSystem() {
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void SoundSystem::Play(Sound sound) {
	alSourcePlay(sound.source);

	sound.State = AL_PLAYING;

	while (sound.State == AL_PLAYING) {
		alGetSourcei(sound.source, AL_SOURCE_STATE, &sound.State);
	}
}
