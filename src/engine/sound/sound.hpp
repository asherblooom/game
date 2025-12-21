#ifndef SOUND_HPP
#define SOUND_HPP

#include <AL/al.h>
#include <AL/alc.h>

// Struct to hold fmt chunk data for WAVE files.
struct WAVEFormat {
	short audioFormat;
	short numChannels;
	unsigned int sampleRate;
	unsigned int byteRate;
	short blockAlign;
	short bitsPerSample;
};

class Sound {
	friend class ResourceManager;
	friend class SoundSystem;

public:
	// set blocking to true if you want game to pause until sound has finished
	void Play(bool blocking = false);
	ALint State;

protected:
	Sound(WAVEFormat fmt, int size, char* data);

private:
	WAVEFormat fmt;
	float length;
	int size;
	char* data;

	ALuint buffer;
	ALuint source;

	ALenum OALFormat();
};

#endif
