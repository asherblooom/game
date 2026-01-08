#ifndef SOUND_HPP
#define SOUND_HPP

#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include <fstream>
#include <vector>

// constants for streaming sounds
const int NUM_BUFFERS = 4;
const int BUFFER_SIZE = 65536;	// 32kb of data in each buffer

class BaseSound {
public:
	virtual void Play() = 0;
	void Stop();
	void FadeOut();
	ALint State;

	// set looping to true if you want sound to loop
	bool Looping = false;
	// set blocking to true if you want game to pause until sound has finished
	bool Blocking = false;

protected:
	BaseSound(short numChannels, unsigned int sampleRate, short bitsPerSample, long size);

	short numChannels;
	unsigned int sampleRate;
	short bitsPerSample;
	long size;
	ALuint source;

	ALenum OALFormat();
};

class Sound : public BaseSound {
	friend class ResourceManager;
	friend class SoundSystem;

public:
	void Play() override;

private:
	Sound(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, char* data);
	ALuint buffer;
	std::vector<char> data;
};

class SoundStream : public BaseSound {
	friend class ResourceManager;
	friend class SoundSystem;

protected:
	SoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size);
	ALuint buffers[NUM_BUFFERS];
	std::vector<char> transferBuffer;

	virtual void updateStream() = 0;
	virtual void closeFile() = 0;
};

class WaveSoundStream : public SoundStream {
	friend class ResourceManager;
	friend class SoundSystem;

public:
	void Play() override;

private:
	WaveSoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, std::ifstream& file);
	std::ifstream& file;
	int cursor;

	void updateStream() override;
	void closeFile() override { file.close(); }
};

class OggSoundStream : public SoundStream {
	friend class ResourceManager;
	friend class SoundSystem;

public:
	void Play() override;

private:
	OggSoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, OggVorbis_File& streamHandle);
	OggVorbis_File& vorbisFile;

	void updateStream() override;
	void closeFile() override { ov_clear(&vorbisFile); }
};

#endif
