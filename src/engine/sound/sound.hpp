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

/*
 * Struct to hold the data chunk of the wave file
 */
struct WAVEData {
	char subChunkID[4];			  // should contain the word data
	unsigned long subChunk2Size;  // Stores the size of the data block
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
	Sound() : data{nullptr}, length{0}, bitRate{0}, freqRate{0}, size{0}, channels{0}, buffer{0} {}
	// ~Sound() { delete data; }
	void LoadFromWAV(std::string filename);
	void LoadWAVChunkInfo(std::ifstream& file, std::string& name, unsigned int& size);

	char* data;

	float length;
	int bitRate;
	float freqRate;
	int size;
	int channels;

	ALuint buffer;
};

#endif
