#include "sound.hpp"
#include <AL/al.h>
#include <string.h>
#include <iostream>
#include <stdexcept>

ALenum BaseSound::OALFormat() {
	if (bitsPerSample == 16) {
		return numChannels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	} else if (bitsPerSample == 8) {
		return numChannels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	}
	throw std::runtime_error("ERROR::SOUND: Unrecognised wave format");
}

BaseSound::BaseSound(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data)
	: State{AL_INITIAL},
	  numChannels{numChannels},
	  sampleRate{sampleRate},
	  bitsPerSample{bitsPerSample},
	  length{size / (numChannels * sampleRate * (bitsPerSample / 8.0f)) * 1000.0f},
	  size{size},
	  data{data, data + size} {}

Sound::Sound(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data)
	: BaseSound{numChannels, sampleRate, bitsPerSample, size, data} {
	alGenBuffers(1, &buffer);
	alBufferData(buffer, OALFormat(), data, size, (ALsizei)sampleRate);

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1.0f);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcei(source, AL_BUFFER, buffer);
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

SoundStream::SoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data)
	: BaseSound{numChannels, sampleRate, bitsPerSample, size, data} {
	alGenBuffers(NUM_BUFFERS, &buffers[0]);
	// alBufferData(buffer, OALFormat(), data, size, (ALsizei)sampleRate);
	for (int i = 0; i < NUM_BUFFERS; i++) {
		alBufferData(buffers[i], OALFormat(), &data[i * BUFFER_SIZE], BUFFER_SIZE, sampleRate);
	}

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1.0f);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);

	alSourceQueueBuffers(source, NUM_BUFFERS, &buffers[0]);
}

// FIXME: what about blocking/non-blocking with this?????????
void SoundStream::Play(bool blocking) {
	alSourceQueueBuffers(source, NUM_BUFFERS, &buffers[0]);
	alSourcePlay(source);
	ALint state = AL_PLAYING;
	cursor = BUFFER_SIZE * NUM_BUFFERS;

	while (state == AL_PLAYING) {
		updateStream();
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	}
}

void SoundStream::updateStream() {
	ALint buffersProcessed = 0;
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

	if (buffersProcessed <= 0)
		return;

	while (buffersProcessed--) {
		ALuint buffer;
		alSourceUnqueueBuffers(source, 1, &buffer);

		ALsizei dataSize = BUFFER_SIZE;

		char* tempData = new char[dataSize];
		memset(tempData, 0, dataSize);

		std::size_t dataSizeToCopy = BUFFER_SIZE;
		if (cursor + BUFFER_SIZE > (int)data.size())
			dataSizeToCopy = data.size() - cursor;

		memcpy(&data[0], &data[cursor], dataSizeToCopy);
		cursor += dataSizeToCopy;

		if (dataSizeToCopy < BUFFER_SIZE) {
			cursor = 0;
			memcpy(&tempData[dataSizeToCopy], &data[cursor], BUFFER_SIZE - dataSizeToCopy);
			cursor = BUFFER_SIZE - dataSizeToCopy;
		}

		alBufferData(buffer, OALFormat(), tempData, BUFFER_SIZE, sampleRate);
		alSourceQueueBuffers(source, 1, &buffer);

		delete[] tempData;
	}
}
