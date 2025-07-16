#include "texture.hpp"

Texture2D::Texture2D() {
	glGenTextures(1, &ID_);
	glBindTexture(GL_TEXTURE_2D, this->ID_);
	// defaults
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Wrap_S(unsigned int wrap_s) {
	glBindTexture(GL_TEXTURE_2D, this->ID_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Wrap_T(unsigned int wrap_t) {
	glBindTexture(GL_TEXTURE_2D, this->ID_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Filter_Min(unsigned int filter_min) {
	glBindTexture(GL_TEXTURE_2D, this->ID_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Filter_Max(unsigned int filter_max) {
	glBindTexture(GL_TEXTURE_2D, this->ID_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned int format, unsigned int mipMapCount, unsigned int blockSize, unsigned char* data) {
	this->width = width;
	this->height = height;
	this->internalFormat = format;
	// bind the texture
	// make it complete by specifying all needed parameters and ensuring all mipmaps are filled
	glBindTexture(GL_TEXTURE_2D, this->ID_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);	// opengl likes array length of mipmaps

	// prepare some variables
	unsigned int offset = 0;
	unsigned int size = 0;
	unsigned int w = this->width;
	unsigned int h = this->height;

	// loop through sending block at a time with the magic formula
	// upload to opengl properly, note the offset transverses the pointer
	// assumes each mipmap is 1/2 the size of the previous mipmap
	for (unsigned int i = 0; i < mipMapCount; i++) {
		if (w == 0 || h == 0) {	 // discard any odd mipmaps 0x1 0x2 resolutions
			mipMapCount--;
			continue;
		}
		size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, i, this->internalFormat, w, h, 0, size, data + offset);
		offset += size;
		w /= 2;
		h /= 2;
	}
	// discard any odd mipmaps, ensure a complete texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
	glBindTexture(GL_TEXTURE_2D, this->ID_);
}
