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

void Sound::Play() {
	Looping ? alSourcei(source, AL_LOOPING, AL_TRUE) : alSourcei(source, AL_LOOPING, AL_FALSE);
	alSourcePlay(source);
	if (Blocking) {
		State = AL_PLAYING;
		while (State == AL_PLAYING) {
			alGetSourcei(source, AL_SOURCE_STATE, &State);
		}
	}
}

SoundStream::SoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data)
	: BaseSound{numChannels, sampleRate, bitsPerSample, size, data}, cursor{0}, transferBuffer(BUFFER_SIZE) {
	alGenBuffers(NUM_BUFFERS, &buffers[0]);

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1.0f);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	// this is always false, we deal with looping in updateStream() to avoid silence at end of audio track
	// when we haven't got enough data to completely fill last buffer
	alSourcei(source, AL_LOOPING, AL_FALSE);
}

void SoundStream::Play() {
	// clear and reset queue
	alSourceStop(source);
	alSourcei(source, AL_BUFFER, 0);  // Removing the buffers from the source clears the queue
	// (re)fill buffers and initialise queue
	for (int i = 0; i < NUM_BUFFERS; i++)
		alBufferData(buffers[i], OALFormat(), &data[i * BUFFER_SIZE], BUFFER_SIZE, sampleRate);
	alSourceQueueBuffers(source, NUM_BUFFERS, &buffers[0]);
	cursor = BUFFER_SIZE * NUM_BUFFERS;

	alSourcePlay(source);
	if (Blocking) {
		State = AL_PLAYING;
		while (State == AL_PLAYING) {
			updateStream();
			alGetSourcei(source, AL_SOURCE_STATE, &State);
		}
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

		if (!Looping && cursor >= size)
			continue;

		int bytesWritten = 0;
		while (bytesWritten < BUFFER_SIZE) {
			// How much data is available to read from the current cursor?
			int bytesRemaningInSource = size - cursor;
			// How much space is left in our OpenAL buffer?
			int bytesSpaceInBuffer = BUFFER_SIZE - bytesWritten;
			// Copy whichever is smaller
			int bytesToCopy = std::min(bytesRemaningInSource, bytesSpaceInBuffer);

			memcpy(&transferBuffer[bytesWritten], &data[cursor], bytesToCopy);
			cursor += bytesToCopy;
			bytesWritten += bytesToCopy;

			// If we hit the end of the source data...
			if (cursor >= size) {
				if (Looping) {
					// Loop: Reset cursor and continue filling the same buffer (avoid extra unneeded silence!)
					cursor = 0;
				} else {
					// No Loop: Fill the rest with silence and stop
					memset(&transferBuffer[bytesWritten], 0, BUFFER_SIZE - bytesWritten);
					break;
				}
			}
		}
		alBufferData(buffer, OALFormat(), transferBuffer.data(), BUFFER_SIZE, sampleRate);
		alSourceQueueBuffers(source, 1, &buffer);
	}
}
