#include "sound.hpp"

Sound::Sound(WAVEFormat fmt, int size, char* data)
	: fmt{fmt}, size{size}, data{data} {
	length = (float)size / (fmt.numChannels * fmt.sampleRate * (fmt.bitsPerSample / 8.0f)) * 1000.0f;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, OALFormat(), data, size, (ALsizei)fmt.sampleRate);
}

ALenum Sound::OALFormat() {
	if (fmt.bitsPerSample == 16) {
		return fmt.numChannels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

	} else if (fmt.bitsPerSample == 8) {
		return fmt.numChannels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	}
	return AL_FORMAT_MONO8;
}
