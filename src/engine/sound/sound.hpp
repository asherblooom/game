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
	virtual void Play() = 0;
	ALint State;

	// set looping to true if you want sound to loop
	bool Looping = false;
	// set blocking to true if you want game to pause until sound has finished
	bool Blocking = false;

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
	void Play() override;

private:
	Sound(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data);
	ALuint buffer;
};

class SoundStream : public BaseSound {
	friend class ResourceManager;
	friend class SoundSystem;

public:
	void Play() override;

private:
	SoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, int size, char* data);
	ALuint buffers[NUM_BUFFERS];
	int cursor;
	std::vector<char> transferBuffer;

	void updateStream();
};

#endif
