#ifndef SOUND_HPP
#define SOUND_HPP

#include <AL/al.h>
#include <AL/alc.h>
#include <string>

/*
 * Struct to hold fmt chunk data for WAVE files.
 */
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

public:
	// TODO: do we need these??
	//
	// const char* Data() const { return data; }
	// const int& BitRate() const { return bitRate; }
	// const float& Frequency() const { return freqRate; }
	// const int& Channels() const { return channels; }
	// const int& Size() const { return size; }
	// const ALuint& Buffer() const { return buffer; }

	ALenum OALFormat();
	// const float& Length() const;

protected:
	Sound(WAVEFormat fmt, int size, char* data);

private:
	WAVEFormat fmt;
	float length;
	int size;
	char* data;

	ALuint buffer;
};

#endif
