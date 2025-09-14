#include "sound.hpp"

#include <fstream>
#include <iostream>

void Sound::LoadFromWAV(std::string filename) {
	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
	if (!file) {
		std::cerr << "ERROR::SOUND: Failed to load WAV file '" << filename << "'\n";
		return;
	}
	std::string chunkName;
	unsigned int chunkSize;

	while (!file.eof()) {
		LoadWAVChunkInfo(file, chunkName, chunkSize);
		if (chunkName == "RIFF") {
			file.seekg(4, std::ios_base::cur);
		} else if (chunkName == "fmt ") {
			WAVEFormat fmt;

			file.read((char *)&fmt, sizeof(WAVEFormat));

			bitRate = fmt.bitsPerSample;
			freqRate = (float)fmt.sampleRate;
			channels = fmt.numChannels;
		} else if (chunkName == "data") {
			size = chunkSize;
			data = new char[size];
			file.read((char *)data, chunkSize);
		} else {
			file.seekg(chunkSize, std::ios_base::cur);
		}
	}
	file.close();
	length = (float)size / (channels * freqRate * (bitRate / 8.0f)) * 1000.0f;
}

void Sound::LoadWAVChunkInfo(std::ifstream &file, std::string &name, unsigned int &size) {
	char chunk[4];
	file.read((char *)&chunk, 4);
	file.read((char *)&size, 4);
	name = std::string(chunk, 4);
}

ALenum Sound::OALFormat() {
	if (bitRate == 16) {
		return channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

	} else if (bitRate == 8) {
		return channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	}
	return AL_FORMAT_MONO8;
}
