#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D {
private:
	// holds the ID of the texture object, used for all texture operations to reference to this particular texture
	GLuint ID_;
	unsigned int width, height;	  // width and height of loaded image in pixels
	unsigned int internalFormat;  // format of texture object

public:
	Texture2D();
	// generates texture from image data
	void Generate(unsigned int width, unsigned int height, unsigned int format, unsigned int mipMapCount, unsigned int blockSize, unsigned char* data);
	// binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
	const GLuint& ID() const { return ID_; }
	// texture configuration
	void Wrap_S(unsigned int wrap_s);		   // set wrapping mode on S axis
	void Wrap_T(unsigned int wrap_t);		   // set wrapping mode on T axis
	void Filter_Min(unsigned int filter_min);  // set filtering mode if texture pixels < screen pixels
	void Filter_Max(unsigned int filter_max);  // set filtering mode if texture pixels > screen pixels
};

#endif
