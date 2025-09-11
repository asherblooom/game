#include "resource_manager.hpp"

#include <freetype2/ft2build.h>
#include <cstdio>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include FT_FREETYPE_H

// Instantiate (global) static variables
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Font> ResourceManager::Fonts;

Shader &ResourceManager::GetShader(std::string name) {
	if (!Shaders.contains(name))
		std::cerr << "ERROR: Can't find shader: " << name << "\n";
	return Shaders.at(name);
}

Shader &ResourceManager::LoadShader(std::string name, std::string vShaderFile, std::string fShaderFile, std::string gShaderFile) {
	// 1. retrieve the vertex/fragment source code from files
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try {
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
				std::cout << "ERROR::SHADER: Vertex shader file not found: " << vShaderFile << std::endl;
			if (!fragmentShaderFile)
				std::cout << "ERROR::SHADER: Fragment shader file not found: " << fShaderFile << std::endl;
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
					std::cout << "ERROR::SHADER: Geometry shader file not found: " << gShaderFile << std::endl;
			}
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	} catch (std::exception_ptr p) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	const char *gShaderCode = geometryCode.c_str();
	// 2. now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != "" ? gShaderCode : nullptr);
	Shaders[name] = shader;
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

	// open the DDS file for binary reading and get file size
	// first try to open with default path
	std::string defaultPath = "media/textures/";
	std::FILE *f = std::fopen((defaultPath + ddsFile).c_str(), "rb");
	try {
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
		Textures[name] = texture;

		delete[] (buffer);
		delete[] (header);
		fclose(f);
		return Textures.at(name);

	} catch (const char *e) {
		delete[] (buffer);
		delete[] (header);
		if (f) fclose(f);
		std::cerr << e << "\n";
		throw(e);
	}
}

Texture2D &ResourceManager::GetTexture(std::string name) {
	if (!Textures.contains(name))
		std::cerr << "ERROR: Can't find texture: " << name << "\n";
	return Textures.at(name);
}

Font &ResourceManager::LoadFont(std::string name, std::string fontFile, unsigned int defaultFontSize) {
	// initialize and load the FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {  // all functions return a value different than 0 whenever an error occurred
		std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	// load font as face
	FT_Face face;
	// first try to open with default path
	std::string defaultPath = "media/fonts/";
	if (FT_New_Face(ft, (defaultPath + fontFile).c_str(), 0, &face)) {
		// otherwise assume input is a full path itself and try to open
		if (FT_New_Face(ft, fontFile.c_str(), 0, &face)) {
			std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
		}
	}
	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, defaultFontSize);
	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// then for the first 128 ASCII characters, pre-load/compile their characters and store them
	Font font;
	for (GLubyte c = 0; c < 128; c++) {
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// now store character for later use
		CharacterData characterData = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x};
		font[c] = characterData;
	}
	Fonts[name] = font;
	glBindTexture(GL_TEXTURE_2D, 0);
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	return Fonts.at(name);
}

Font &ResourceManager::GetFont(std::string name) {
	if (!Fonts.contains(name))
		std::cerr << "ERROR: Can't find font: " << name << "\n";
	return Fonts.at(name);
}

void ResourceManager::Clear() {
	// (properly) delete all shaders
	for (auto &shader : Shaders) {
		glDeleteProgram(shader.second.ID());
	}
	// (properly) delete all textures
	for (auto &texture : Textures) {
		glDeleteTextures(1, &texture.second.ID());
	}
	// (properly) delete all font textures
	for (auto &font : Fonts) {
		for (auto &c : font.second)
			glDeleteTextures(1, &c.second.TextureID);
	}
}
