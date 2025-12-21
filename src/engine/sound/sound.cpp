#include "sound.hpp"
#include <AL/al.h>
#include <iostream>
#include <stdexcept>

Sound::Sound(WAVEFormat fmt, int size, char* data)
	: State{AL_INITIAL}, fmt{fmt}, length{size / (fmt.numChannels * fmt.sampleRate * (fmt.bitsPerSample / 8.0f)) * 1000.0f}, size{size}, data{data} {
	alGenBuffers(1, &buffer);
	alBufferData(buffer, OALFormat(), data, size, (ALsizei)fmt.sampleRate);

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1.0f);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcei(source, AL_BUFFER, buffer);
}

ALenum Sound::OALFormat() {
	if (fmt.bitsPerSample == 16) {
		return fmt.numChannels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	} else if (fmt.bitsPerSample == 8) {
		return fmt.numChannels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	}
	throw std::runtime_error("ERROR::SOUND: Unrecognised wave format");
}

void Sound::Play(bool blocking) {
	alSourcePlay(source);
	if (blocking) {
		State = AL_PLAYING;
		while (State == AL_PLAYING) {
			alGetSourcei(source, AL_SOURCE_STATE, &State);
		}
	}
}
