#ifndef SOUND_HPP
#define SOUND_HPP

#include <AL/al.h>
#include <AL/alc.h>

// Struct to hold fmt chunk data for WAVE files.
struct WAVEFormat {
	short audioFormat;
	short numChannels;
	unsigned long sampleRate;
	unsigned long byteRate;
	short blockAlign;
	short bitsPerSample;
};

class Sound {
	friend class ResourceManager;
	friend class SoundSystem;

public:
	ALenum OALFormat();

	ALint State;
	// const float& Length() const;

protected:
	Sound(WAVEFormat fmt, int size, char* data);

private:
	WAVEFormat fmt;
	float length;
	int size;
	char* data;

	ALuint buffer;
	ALuint source;
};

#endif
