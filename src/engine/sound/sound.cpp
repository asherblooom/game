#include "sound.hpp"
#include <AL/al.h>
#include <vorbis/vorbisfile.h>
#include <fstream>
#include <iostream>
#include <stdexcept>

BaseSound::BaseSound(short numChannels, unsigned int sampleRate, short bitsPerSample, long size)
	: State{AL_INITIAL},
	  numChannels{numChannels},
	  sampleRate{sampleRate},
	  bitsPerSample{bitsPerSample},
	  size{size} {}

ALenum BaseSound::OALFormat() {
	if (bitsPerSample == 16) {
		return numChannels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	} else if (bitsPerSample == 8) {
		return numChannels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	}
	throw std::runtime_error("ERROR::SOUND: Unrecognised wave format");
}

// FIXME: add something in updateStream() functions for these
void BaseSound::Pause() {
	if (State == AL_PLAYING) alSourcePause(source);
}
void BaseSound::Resume() {
	if (State == AL_PAUSED) alSourcePlay(source);
}
void BaseSound::Stop() {
	if (State == AL_PLAYING) alSourceStop(source);
}

void BaseSound::FadeOut() {
	if (State != AL_PLAYING) return;

	float currentGain;
	alGetSourcef(source, AL_GAIN, &currentGain);
	if (currentGain > 0.0)
		alSourcef(source, AL_GAIN, currentGain - 0.01);	 // Fade out over roughly 1 second (assuming 60fps)
	else
		alSourceStop(source);
}

Sound::Sound(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, char* data)
	: BaseSound{numChannels, sampleRate, bitsPerSample, size}, data{data, data + size} {
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
}

SoundStream::SoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size)
	: BaseSound{numChannels, sampleRate, bitsPerSample, size}, transferBuffer(BUFFER_SIZE) {}

WaveSoundStream::WaveSoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, std::unique_ptr<std::ifstream>& file, std::streampos soundDataStartPos)
	: SoundStream{numChannels, sampleRate, bitsPerSample, size}, file{std::move(file)}, soundDataStartPos{soundDataStartPos}, cursor{0} {
	if (size < NUM_BUFFERS * BUFFER_SIZE) {
		closeFile();
		throw std::invalid_argument("ERROR::SOUND: sound too small to use streaming");
	}

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

void WaveSoundStream::Play() {
	// clear and reset queue
	alSourceStop(source);
	alSourcei(source, AL_BUFFER, 0);  // Removing the buffers from the source clears the queue
	if (file->eof()) file->clear();
	file->seekg(soundDataStartPos);
	// (re)fill buffers and initialise queue
	for (int i = 0; i < NUM_BUFFERS; i++) {
		file->read(transferBuffer.data(), BUFFER_SIZE);
		alBufferData(buffers[i], OALFormat(), transferBuffer.data(), BUFFER_SIZE, sampleRate);
		alSourceQueueBuffers(source, 1, &buffers[i]);
	}
	cursor = BUFFER_SIZE * NUM_BUFFERS;

	alSourcePlay(source);
}

void WaveSoundStream::updateStream() {
	ALint buffersProcessed = 0;
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

	while (buffersProcessed--) {
		ALuint buffer;
		alSourceUnqueueBuffers(source, 1, &buffer);

		// if we have reached end of file and we are not looping, but there are queued buffers
		// waiting to be filled (buffersProcessed > 0), do nothing with the unqueued buffer
		if (!Looping && cursor >= size)
			continue;

		int totalBytesRead = 0;
		while (totalBytesRead < BUFFER_SIZE) {
			// How much data is available to read from the current cursor?
			int bytesRemaningInSource = size - cursor;
			// How much space is left in the buffer?
			int bytesSpaceInBuffer = BUFFER_SIZE - totalBytesRead;
			// Read whichever is smaller
			int bytesToRead = std::min(bytesRemaningInSource, bytesSpaceInBuffer);
			file->read(transferBuffer.data() + totalBytesRead, bytesToRead);
			cursor += bytesToRead;
			totalBytesRead += bytesToRead;

			// If we hit the end of the source data... (cannot use file.eof() since end of data section might not be end of file)
			if (cursor >= size) {
				if (Looping) {
					// Loop: Reset cursor and continue filling the same buffer (avoid extra unneeded silence!)
					if (file->eof()) file->clear();
					file->seekg(soundDataStartPos);
					cursor = 0;
				} else
					break;
			}
		}
		alBufferData(buffer, OALFormat(), transferBuffer.data(), totalBytesRead, sampleRate);
		alSourceQueueBuffers(source, 1, &buffer);
	}
}

OggSoundStream::OggSoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, OggVorbis_File* vorbisFile)
	: SoundStream(numChannels, sampleRate, bitsPerSample, size), vorbisFile{vorbisFile} {
	if (size < NUM_BUFFERS * BUFFER_SIZE) {
		closeFile();
		throw std::invalid_argument("ERROR::SOUND: sound too small to use streaming");
	}
	alGenBuffers(NUM_BUFFERS, &buffers[0]);

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1.0f);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_FALSE);
}

void OggSoundStream::Play() {
	// clear and reset queue
	alSourceStop(source);
	alSourcei(source, AL_BUFFER, 0);  // Removing the buffers from the source clears the queue
	ov_pcm_seek(vorbisFile, 0);
	// (re)fill buffers and initialise queue
	int bytesRead = 0;
	int currentSection;
	for (int i = 0; i < NUM_BUFFERS; i++) {
		int totalBytesRead = 0;
		while (totalBytesRead < BUFFER_SIZE) {
			bytesRead = ov_read(vorbisFile, transferBuffer.data() + totalBytesRead, BUFFER_SIZE - totalBytesRead, 0, 2, 1, &currentSection);
			if (bytesRead > 0)
				totalBytesRead += bytesRead;
			else if (bytesRead < 0) {
				// Error in the stream
				closeFile();
				throw std::runtime_error("ERROR::SOUND: could not decode OGG bitstream");
			}
		}
		alBufferData(buffers[i], OALFormat(), transferBuffer.data(), totalBytesRead, sampleRate);
		alSourceQueueBuffers(source, 1, &buffers[i]);
	}

	alSourcePlay(source);
}

void OggSoundStream::updateStream() {
	int bytesRead = 0;
	int section;

	ALint buffersProcessed = 0;
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

	while (buffersProcessed--) {
		ALuint buffer;
		alSourceUnqueueBuffers(source, 1, &buffer);

		// refill the buffer with new ogg data
		int totalBytesRead = 0;
		while (totalBytesRead < BUFFER_SIZE) {
			bytesRead = ov_read(vorbisFile, transferBuffer.data() + totalBytesRead, BUFFER_SIZE - totalBytesRead, 0, 2, 1, &section);
			if (bytesRead > 0)
				totalBytesRead += bytesRead;
			else if (bytesRead < 0) {
				// Error in the stream
				closeFile();
				throw std::runtime_error("ERROR::SOUND: could not decode OGG bitstream");
			} else if (bytesRead == 0) {
				// end of file reached
				if (Looping)
					ov_pcm_seek(vorbisFile, 0);
				else
					break;
			}
		}
		if (totalBytesRead > 0) {
			alBufferData(buffer, OALFormat(), transferBuffer.data(), totalBytesRead, sampleRate);
			alSourceQueueBuffers(source, 1, &buffer);
		}
	}
}
