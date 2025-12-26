#ifndef SOUND_HPP
#define SOUND_HPP

#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

// constants for streaming sounds
const int NUM_BUFFERS = 4;
const int BUFFER_SIZE = 65536;	// 32kb of data in each buffer

class BaseSound {
public:
	// set blocking to true if you want game to pause until sound has finished
	virtual void Play(bool blocking = false) = 0;
	ALint State;

protected:
	BaseSound(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data);

	short numChannels;
	unsigned int sampleRate;
	short bitsPerSample;
	float length;

	int size;
	std::vector<char> data;

	ALuint source;

	ALenum OALFormat();
};

class Sound : public BaseSound {
	friend class ResourceManager;
	friend class SoundSystem;

public:
	// set blocking to true if you want game to pause until sound has finished
	void Play(bool blocking = false) override;

protected:
	Sound(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data);

private:
	ALuint buffer;
};

class SoundStream : public BaseSound {
	friend class ResourceManager;
	friend class SoundSystem;

public:
	// set blocking to true if you want game to pause until sound has finished
	void Play(bool blocking = false) override;

protected:
	SoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data);

private:
	ALuint buffers[NUM_BUFFERS];
	int cursor;

	void updateStream();
};

#endif
