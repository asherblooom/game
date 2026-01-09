#ifndef SOUND_HPP
#define SOUND_HPP

#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include <fstream>
#include <memory>
#include <vector>

// constants for streaming sounds
const int NUM_BUFFERS = 4;
const int BUFFER_SIZE = 65536;	// 32kb of data in each buffer

enum UseStreaming {
	AUTOMATIC,
	STREAMING_ON,
	STREAMING_OFF
};

class BaseSound {
public:
	virtual void Play() = 0;
	void Pause();
	void Resume();
	void Stop();
	void FadeOut();

	ALint State;
	// set looping to true if you want sound to loop
	bool Looping = false;

protected:
	BaseSound(short numChannels, unsigned int sampleRate, short bitsPerSample, long size);
	virtual ~BaseSound() = default;

	short numChannels;
	unsigned int sampleRate;
	short bitsPerSample;
	long size;
	ALuint source;
	bool fadeOut = false;

	ALenum OALFormat();
	// updates State member variable with current OpenAL state of source; must be called every frame
	void updateState();
	// for volume changes that are meant to happen over time; must be called every frame
	void updateVolume();
};

class Sound : public BaseSound {
	friend class ResourceManager;

public:
	void Play() override;

private:
	Sound(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, char* data);
	ALuint buffer;
	std::vector<char> data;
};

class SoundStream : public BaseSound {
	friend class ResourceManager;

public:
	virtual ~SoundStream() = default;

protected:
	SoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size);
	ALuint buffers[NUM_BUFFERS];
	std::vector<char> transferBuffer;

	// updates any processed buffers with new data; must be called every frame
	virtual void updateStream() = 0;
	virtual void closeFile() = 0;
};

class WaveSoundStream : public SoundStream {
	friend class ResourceManager;

public:
	void Play() override;

private:
	WaveSoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, std::unique_ptr<std::ifstream>& file, std::streampos soundDataStartPos);
	std::unique_ptr<std::ifstream> file;
	const std::streampos soundDataStartPos;
	int cursor;

	void updateStream() override;
	void closeFile() override { file->close(); }
};

class OggSoundStream : public SoundStream {
	friend class ResourceManager;

public:
	void Play() override;

private:
	// vorbisFile MUST be heap-allocated
	OggSoundStream(short numChannels, unsigned int sampleRate, short bitsPerSample, long size, OggVorbis_File* vorbisFile);
	OggVorbis_File* vorbisFile;

	void updateStream() override;
	void closeFile() override {
		ov_clear(vorbisFile);
		delete vorbisFile;
	}
};

#endif
