#include "resource_manager.hpp"

#include <freetype2/ft2build.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "sound/sound.hpp"
#include FT_FREETYPE_H

// Instantiate (global) static variables
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Texture2DArray> ResourceManager::TextureArrays;
std::map<std::string, std::map<std::string, int>> ResourceManager::ArrayItemNames;
std::map<std::string, Font> ResourceManager::Fonts;
std::map<std::string, Sound> ResourceManager::Sounds;
std::map<std::string, SoundStream> ResourceManager::SoundStreams;

Shader &ResourceManager::GetShader(std::string name) {
	if (!Shaders.contains(name))
		std::cerr << "ERROR::SHADER: Can't find shader: " << name << "\n";
	return Shaders.at(name);
}

Shader &ResourceManager::LoadShader(std::string name, std::string vShaderFile, std::string fShaderFile, std::string gShaderFile) {
	// retrieve the vertex/fragment source code from files
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try {
		if (Shaders.contains(name))
			throw std::invalid_argument("ERROR::SHADER: There already exists a shader with name '" + name + "'");

		// open files
		// first try to open with default path
		std::string defaultPath = "src/engine/shaders/";
		std::ifstream vertexShaderFile{defaultPath + vShaderFile};
		std::ifstream fragmentShaderFile{defaultPath + fShaderFile};
		// otherwise assume input is a full path itself and try to open
		if (!vertexShaderFile || !fragmentShaderFile) {
			std::ifstream vertexShaderFile{vShaderFile};
			std::ifstream fragmentShaderFile{fShaderFile};
			// otherwise error
			if (!vertexShaderFile)
				std::cerr << "ERROR::SHADER: Vertex shader file not found: " << vShaderFile << "\n";
			if (!fragmentShaderFile)
				std::cerr << "ERROR::SHADER: Fragment shader file not found: " << fShaderFile << "\n";
		}

		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// if geometry shader path is present, also load a geometry shader
		if (gShaderFile != "") {
			std::ifstream geometryShaderFile{defaultPath + gShaderFile};
			if (!geometryShaderFile) {
				std::ifstream geometryShaderFile{gShaderFile};
				if (!geometryShaderFile)
					std::cerr << "ERROR::SHADER: Geometry shader file not found: " << gShaderFile << "\n";
			}
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	} catch (std::exception_ptr p) {
		std::cerr << "ERROR::SHADER: Failed to read shader files \n";
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	const char *gShaderCode = geometryCode.c_str();
	// now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != "" ? gShaderCode : nullptr);
	Shaders.insert(std::make_pair(name, shader));
	return Shaders.at(name);
}

Texture2D &ResourceManager::LoadDDSTexture(std::string name, std::string ddsFile, bool mipmaps) {
	// allocate new unsigned char space with 4 (file code) + 124 (header size) bytes
	unsigned char *header = new unsigned char[128];

	unsigned int width;
	unsigned int height;
	unsigned int mipMapCount;

	unsigned int blockSize;
	unsigned int format;

	unsigned char *buffer = 0;

	std::FILE *f;

	// open the DDS file for binary reading and get file size
	// first try to open with default path
	try {
		if (Textures.contains(name))
			throw("ERROR::TEXTURE: There already exists a texture with name '" + name + "'\n");

		std::string extension = ddsFile.substr(ddsFile.length() - 3, 3);
		if (extension != "dds")
			throw("ERROR::TEXTURE: Invalid file extension: " + extension + "\n");

		std::string defaultPath = "media/textures/";
		f = std::fopen((defaultPath + ddsFile).c_str(), "rb");
		if (f == nullptr) {
			// otherwise assume input is a full path itself and try to open
			f = std::fopen(ddsFile.c_str(), "rb");
			if (f == nullptr) {
				std::cerr << ddsFile << " ";
				throw "ERROR::TEXTURE: incorrect file name";
			}
		}
		std::fseek(f, 0, SEEK_END);
		long file_size = ftell(f);
		std::fseek(f, 0, SEEK_SET);

		// read in 128 bytes from the file
		std::fread(header, 1, 128, f);

		// compare the `DDS ` signature
		if (std::memcmp(header, "DDS ", 4) != 0) {
			throw "ERROR::TEXTURE: incorrect DDS signature";
		}

		// extract height, width, and amount of mipmaps - yes it is stored height then width
		height = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
		width = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
		if (mipmaps)
			mipMapCount = (header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);
		else
			mipMapCount = 1;

		// figure out what format to use for what fourCC file type it is
		// block size is about physical chunk storage of compressed data in file (important)
		if (header[84] == 'D') {
			switch (header[87]) {
				case '1':  // DXT1
					format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
					blockSize = 8;
					break;
				case '3':  // DXT3
					format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
					blockSize = 16;
					break;
				case '5':  // DXT5
					format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
					blockSize = 16;
					break;
				case '0':  // DX10
						   // unsupported, else will error
						   // as it adds sizeof(struct DDS_HEADER_DXT10) between pixels
						   // so, buffer = malloc((file_size - 128) - sizeof(struct DDS_HEADER_DXT10));
				default:
					throw "ERROR::TEXTURE: unsupported compression";
			}
		} else {  // BC4U/BC4S/ATI2/BC55/R8G8_B8G8/G8R8_G8B8/UYVY-packed/YUY2-packed unsupported
			throw "ERROR::TEXTURE: unsupported compression";
		}

		// read rest of file
		buffer = new unsigned char[file_size - 128];
		if (buffer == 0) {
			throw "ERROR::TEXTURE: memory allocation failed";
		}
		fread(buffer, 1, file_size, f);

		// now generate texture
		Texture2D texture;
		texture.Generate(width, height, format, mipMapCount, blockSize, buffer);
		Textures.insert(std::make_pair(name, texture));

		delete[] (buffer);
		delete[] (header);
		fclose(f);
		return Textures.at(name);

	} catch (const char *e) {
		delete[] (buffer);
		delete[] (header);
		if (f) fclose(f);
		std::cerr << e << "\n";
		throw;
	}
}

Texture2D &ResourceManager::GetTexture(std::string name) {
	if (!Textures.contains(name))
		std::cerr << "ERROR::TEXTURE: Can't find texture: " << name << "\n";
	return Textures.at(name);
}

Texture2DArray &ResourceManager::LoadDDSTextureArray(std::string arrayName, std::vector<std::string> itemNames, std::vector<std::string> ddsFiles, bool mipmaps) {
	unsigned int width;
	unsigned int height;
	unsigned int mipMapCount;
	unsigned int blockSize;
	unsigned int format;
	std::FILE *f;
	std::vector<unsigned char *> data;
	unsigned char *header;
	unsigned char *buffer;
	std::map<std::string, int> indexMap{};

	try {
		if (TextureArrays.contains(arrayName))
			throw("ERROR::TEXTURE: There already exists a texture array with name '" + arrayName + "'\n");
		if (itemNames.size() != ddsFiles.size())
			throw "ERROR::TEXTURE: itemNames must be same size as ddsFiles for texture arrays";

		for (std::size_t i = 0; i < ddsFiles.size(); i++) {
			std::string extension = ddsFiles.at(i).substr(ddsFiles.at(i).length() - 3, 3);
			if (extension != "dds")
				throw("ERROR::TEXTURE: Invalid file extension: " + extension + "\n");

			std::string file = ddsFiles.at(i);
			// allocate new unsigned char space with 4 (file code) + 124 (header size) bytes
			header = new unsigned char[128];
			buffer = 0;

			// open the DDS file for binary reading and get file size
			// first try to open with default path
			std::string defaultPath = "media/textures/";
			f = std::fopen((defaultPath + file).c_str(), "rb");
			if (f == nullptr) {
				// otherwise assume input is a full path itself and try to open
				f = std::fopen(file.c_str(), "rb");
				if (f == nullptr) {
					std::cerr << file << " ";
					throw "ERROR::TEXTURE: incorrect file name";
				}
			}
			std::fseek(f, 0, SEEK_END);
			long file_size = ftell(f);
			std::fseek(f, 0, SEEK_SET);

			// read in 128 bytes from the file
			std::fread(header, 1, 128, f);

			// compare the `DDS ` signature
			if (std::memcmp(header, "DDS ", 4) != 0) {
				throw "ERROR::TEXTURE: incorrect DDS signature";
			}

			// for first texture,
			// extract height, width, and amount of mipmaps - yes it is stored height then width
			if (i == 0) {
				height = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
				width = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
				if (mipmaps)
					mipMapCount = (header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);
				else
					mipMapCount = 1;

				// figure out what format to use for what fourCC file type it is
				// block size is about physical chunk storage of compressed data in file (important)
				if (header[84] == 'D') {
					switch (header[87]) {
						case '1':  // DXT1
							format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
							blockSize = 8;
							break;
						case '3':  // DXT3
							format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
							blockSize = 16;
							break;
						case '5':  // DXT5
							format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
							blockSize = 16;
							break;
						case '0':  // DX10
								   // unsupported, else will error
								   // as it adds sizeof(struct DDS_HEADER_DXT10) between pixels
								   // so, buffer = malloc((file_size - 128) - sizeof(struct DDS_HEADER_DXT10));
						default:
							throw "ERROR::TEXTURE: unsupported compression";
					}
				} else {  // BC4U/BC4S/ATI2/BC55/R8G8_B8G8/G8R8_G8B8/UYVY-packed/YUY2-packed unsupported
					throw "ERROR::TEXTURE: unsupported compression";
				}
			}
			// check if all other textures match size/mip map count/format
			else {
				unsigned int newHeight = (unsigned int)((header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24));
				unsigned int newWidth = (unsigned int)((header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24));
				if (height != newHeight || width != newWidth) {
					std::cerr << "ERROR::TEXTURE: all textures in an array must have size " << width << "x" << height
							  << ", but " << itemNames[i] << " has size " << newWidth << "x" << newHeight;
					throw "";
				}
				if (mipmaps && mipMapCount != (unsigned int)((header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24)))
					throw "ERROR::TEXTURE: all textures in an array must have same mip map count";
				if (header[84] == 'D') {
					switch (header[87]) {
						case '1':  // DXT1
							if (format != GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
								throw "ERROR::TEXTURE: all textures in an array must use same compression format";
							break;
						case '3':  // DXT3
							if (format != GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
								throw "ERROR::TEXTURE: all textures in an array must use same compression format";
							break;
						case '5':  // DXT5
							if (format != GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
								throw "ERROR::TEXTURE: all textures in an array must use same compression format";
							break;
						default:
							throw "ERROR::TEXTURE: unsupported compression";
					}
				} else {  // BC4U/BC4S/ATI2/BC55/R8G8_B8G8/G8R8_G8B8/UYVY-packed/YUY2-packed unsupported
					throw "ERROR::TEXTURE: unsupported compression";
				}
			}

			// read rest of file
			buffer = new unsigned char[file_size - 128];
			if (buffer == 0) {
				throw "ERROR::TEXTURE: memory allocation failed";
			}
			fread(buffer, 1, file_size, f);
			data.emplace_back(buffer);
			indexMap.emplace(itemNames.at(i), i);

			delete[] (header);
			fclose(f);
		}
		// now generate texture array
		Texture2DArray textureArray;
		textureArray.Generate(width, height, format, mipMapCount, blockSize, data);
		TextureArrays.insert(std::make_pair(arrayName, textureArray));
		ArrayItemNames.insert(std::make_pair(arrayName, std::move(indexMap)));

		for (unsigned char *buffer : data)
			delete[] (buffer);
		return TextureArrays.at(arrayName);

	} catch (const char *e) {
		for (unsigned char *buff : data)
			delete[] (buff);
		if (header) delete[] (header);
		if (f) fclose(f);
		std::cerr << e << "\n";
		throw;
	}
}

Texture2DArray &ResourceManager::GetTextureArray(std::string name) {
	if (!TextureArrays.contains(name))
		std::cerr << "ERROR::TEXTURE: Can't find texture array: " << name << "\n";
	return TextureArrays.at(name);
}

int &ResourceManager::GetArrayItemIndex(std::string arrayName, std::string itemName) {
	if (!TextureArrays.contains(arrayName))
		std::cerr << "ERROR::TEXTURE: Can't find texture array: " << arrayName << "\n";
	if (!ArrayItemNames.at(arrayName).contains(itemName))
		std::cerr << "ERROR::TEXTURE: Can't find item: " << itemName << " in texture array: " << arrayName << "\n";
	return ArrayItemNames.at(arrayName).at(itemName);
}

Font &ResourceManager::LoadFont(std::string name, std::string fontFile, unsigned int defaultFontSize) {
	if (Fonts.contains(name))
		throw std::invalid_argument("ERROR::FONT: There already exists a font with name '" + name + "'");
	std::string extension = fontFile.substr(fontFile.length() - 3, 3);
	if (extension != "ttf")
		throw std::invalid_argument("ERROR::FONT: Invalid file extension: " + extension);

	// initialize and load the FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))	// all functions return a value different than 0 whenever an error occurred
		throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
	// load font as face
	FT_Face face;
	// first try to open with default path
	std::string defaultPath = "media/fonts/";
	if (FT_New_Face(ft, (defaultPath + fontFile).c_str(), 0, &face)) {
		// otherwise assume input is a full path itself and try to open
		if (FT_New_Face(ft, fontFile.c_str(), 0, &face))
			throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
	}
	FT_Set_Pixel_Sizes(face, 0, defaultFontSize);

	// initialise texture atlas
	Font font;
	unsigned int width = 0;
	unsigned int height = 0;
	for (GLubyte c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYPE: Failed to load Glyph \n";
			continue;
		}
		if (face->glyph->bitmap.rows > height) height = face->glyph->bitmap.rows;
		width += (face->glyph->bitmap.width + 1);
	}
	font.AtlasSize = {width, height};
	glGenTextures(1, &font.TextureAtlas);
	glBindTexture(GL_TEXTURE_2D, font.TextureAtlas);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// disable byte-alignment restriction
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height,
				 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// for the first 128 ASCII characters, pre-load/compile their characters and store them
	int pos = 0;
	for (GLubyte c = 32; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

		CharacterData characterData = {
			{pos, 0},
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x};
		font.Characters.insert(std::make_pair(c, characterData));

		glTexSubImage2D(GL_TEXTURE_2D, 0, pos, 0, characterData.Size.x,
						characterData.Size.y, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		// prevent issues with linear filtering pulling from next character's texture
		pos += (characterData.Size.x + 1);
	}
	Fonts.insert(std::make_pair(name, font));
	glBindTexture(GL_TEXTURE_2D, 0);
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	return Fonts.at(name);
}

Font &ResourceManager::GetFont(std::string name) {
	if (!Fonts.contains(name))
		std::cerr << "ERROR::FONT: Can't find font: " << name << "\n";
	return Fonts.at(name);
}

BaseSound *ResourceManager::LoadSound(std::string name, std::string soundFile, bool useStreaming) {
	if (Sounds.contains(name) || SoundStreams.contains(name))
		throw std::invalid_argument("ERROR::SOUND: There already exists a sound with name '" + name + "'");

	std::string extension = soundFile.substr(soundFile.length() - 3, 3);
	if (extension == "wav") {
		if (useStreaming)
			return LoadWaveFile(name, soundFile, true);
		else
			return LoadWaveFile(name, soundFile, false);
	} else if (extension == "ogg")
		if (useStreaming)
			return LoadOggFileStream(name, soundFile);
		else
			return LoadOggFile(name, soundFile);
	else
		throw std::invalid_argument("ERROR::SOUND: Invalid file extension: " + extension);
}

BaseSound *ResourceManager::LoadWaveFile(std::string name, std::string wavFile, bool useStreaming) {
	// first try to open with default path
	std::ifstream f;
	std::string defaultPath = "media/sound/";
	f.open((defaultPath + wavFile).c_str(), std::ios::in | std::ios::binary);
	if (!f) {
		// otherwise assume input is a full path itself and try to open
		f.open(wavFile.c_str(), std::ios::in | std::ios::binary);
		if (!f)
			throw std::invalid_argument("ERROR::SOUND: incorrect file name: " + wavFile);
	}
	std::string chunkName;
	unsigned int chunkSize;

	char *data;
	int size;
	short audioFormat;
	short numChannels;
	unsigned int sampleRate;
	unsigned int byteRate;
	short blockAlign;
	short bitsPerSample;

	bool riffRead = false;
	bool fmtRead = false;
	bool dataRead = false;

	while (true) {
		// load wave chunk info
		char chunk[4];
		f.read((char *)&chunk, 4);
		f.read((char *)&chunkSize, 4);
		chunkName = std::string(chunk, 4);

		if (f.eof()) break;

		if (chunkName == "RIFF") {
			f.seekg(4, std::ios_base::cur);
			riffRead = true;
		} else if (chunkName == "fmt ") {
			f.read((char *)&audioFormat, 2);
			f.read((char *)&numChannels, 2);
			f.read((char *)&sampleRate, 4);
			f.read((char *)&byteRate, 4);
			f.read((char *)&blockAlign, 2);
			f.read((char *)&bitsPerSample, 2);
			// skip over any extra bytes
			if (chunkSize > 16) f.seekg(chunkSize - 16, std::ios_base::cur);
			fmtRead = true;
		} else if (chunkName == "data") {
			size = chunkSize;
			data = new char[size];
			f.read((char *)data, chunkSize);
			dataRead = true;
		} else if (riffRead && fmtRead && dataRead) {
			break;
		} else {
			f.seekg(chunkSize, std::ios_base::cur);
		}
	}
	if (!riffRead || !fmtRead || !dataRead) {
		f.close();
		throw std::runtime_error("ERROR::SOUND: Failed to load sound: cannot find correct chunks in WAVE file");
	}
	f.close();
	// FIXME: size cant be smaller than numbuffers * buffersize - check this is correct for ogg too!!
	// SWITCH TO STREAMING ON AUTO IF SIZE IS TOO BIG INSTEAD OF MAKING USER CHOOSE!?!?!?!?!?!??!
	if (useStreaming && size > NUM_BUFFERS * BUFFER_SIZE) {
		SoundStream sound{numChannels, sampleRate, bitsPerSample, size, data};
		SoundStreams.insert(std::make_pair(name, sound));
		return &SoundStreams.at(name);
	} else {
		Sound sound{numChannels, sampleRate, bitsPerSample, size, data};
		Sounds.insert(std::make_pair(name, sound));
		return &Sounds.at(name);
	}
}

BaseSound *ResourceManager::LoadOggFile(std::string name, std::string oggFile) {
	// first try to open with default path
	FILE *f;
	std::string defaultPath = "media/sound/";
	f = fopen((defaultPath + oggFile).c_str(), "rb");
	if (!f) {
		// otherwise assume input is a full path itself and try to open
		f = fopen(oggFile.c_str(), "rb");
		if (!f)
			throw std::invalid_argument("ERROR::SOUND: incorrect file name: " + oggFile);
	}
	OggVorbis_File vorbisFile;
	if (ov_open(f, &vorbisFile, NULL, 0) < 0) {
		fclose(f);
		throw std::runtime_error("ERROR::SOUND: failed to open OGG file");
	}

	vorbis_info *vorbisInfo = ov_info(&vorbisFile, -1);
	short bitsPerSample = 16;
	short numChannels = vorbisInfo->channels;
	unsigned int sampleRate = vorbisInfo->rate;

	std::vector<char> data;
	const int bufferSize = 4096;  // Read 4KB at a time
	char buffer[bufferSize];
	int currentSection;
	long bytesRead = 0;

	do {
		bytesRead = ov_read(&vorbisFile, buffer, bufferSize, 0, 2, 1, &currentSection);
		if (bytesRead < 0) {
			// Error in the stream
			ov_clear(&vorbisFile);
			throw std::runtime_error("ERROR::SOUND: could not decode OGG bitstream");
		} else if (bytesRead > 0) {
			data.insert(data.end(), buffer, buffer + bytesRead);
		}
	} while (bytesRead > 0);

	// ov_clear closes the file handle internally, so we don't need to close the FILE*
	ov_clear(&vorbisFile);
	Sound sound{numChannels, sampleRate, bitsPerSample, (int)data.size(), data.data()};
	Sounds.insert(std::make_pair(name, sound));
	return &Sounds.at(name);
}

BaseSound *ResourceManager::LoadOggFileStream(std::string name, std::string oggFile) {}
// 	// first try to open with default path
// 	FILE *f;
// 	std::string defaultPath = "media/sound/";
// 	f = fopen((defaultPath + oggFile).c_str(), "rb");
// 	if (!f) {
// 		// otherwise assume input is a full path itself and try to open
// 		f = fopen(oggFile.c_str(), "rb");
// 		if (!f)
// 			throw std::invalid_argument("ERROR::SOUND: incorrect file name: " + oggFile);
// 	}
//
// 	if (ov_open(f, &streamHandle, NULL, 0) < 0) {
// 		throw std::runtime_error("ERROR::SOUND: failed to get OGG stream handle");
// 	}
// 	vorbis_info *vorbisInfo = ov_info(&streamHandle, -1);
// 	bitsPerSample = 16;
// 	numChannels = vorbisInfo->channels;
// 	sampleRate = (float)vorbisInfo->rate;
// 	length = (float)ov_time_total(&streamHandle, -1) * 1000.0f;
//
// 	ov_time_seek(&streamHandle, 0);
// }

BaseSound *ResourceManager::GetSound(std::string name) {
	if (!Sounds.contains(name)) {
		if (!SoundStreams.contains(name)) {
			std::cerr << "ERROR::SOUND: Can't find sound: " << name << "\n";
		}
		return &SoundStreams.at(name);
	}
	return &Sounds.at(name);
}

void ResourceManager::updateAllSoundStreams() {
	for (auto &sound : SoundStreams) {
		sound.second.updateStream();
	}
}

void ResourceManager::Clear() {
	// (properly) delete all resources
	for (auto &shader : Shaders) {
		glDeleteProgram(shader.second.ID());
	}
	for (auto &texture : Textures) {
		glDeleteTextures(1, &texture.second.ID());
	}
	for (auto &textureArr : TextureArrays) {
		glDeleteTextures(1, &textureArr.second.ID());
	}
	for (auto &font : Fonts) {
		glDeleteTextures(1, &font.second.TextureAtlas);
	}
	for (auto &sound : Sounds) {
		alDeleteSources(1, &sound.second.source);
		alDeleteBuffers(1, &sound.second.buffer);
	}
	for (auto &sound : SoundStreams) {
		alDeleteSources(1, &sound.second.source);
		alDeleteBuffers(NUM_BUFFERS, &sound.second.buffers[0]);
	}
}
