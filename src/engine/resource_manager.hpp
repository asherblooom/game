
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "render/font.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"

// A static singleton ResourceManager class that hosts several functions to load Textures and Shaders.
// Each loaded texture and/or shader is also stored for future reference by string handles.
// All functions and resources are static and no public constructor is defined.
class ResourceManager {
public:
	// resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static std::map<std::string, Texture2DArray> TextureArrays;
	static std::map<std::string, Font> Fonts;

	// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code.
	// If gShaderFile is not nullptr, it also loads a geometry shader
	static Shader &LoadShader(std::string name, std::string vShaderFile, std::string fShaderFile, std::string gShaderFile = "");
	// retrieves a stored shader
	static Shader &GetShader(std::string name);

	// loads (and generates) a texture from file
	static Texture2D &LoadDDSTexture(std::string name, std::string ddsFile, bool mipmaps = true);
	// retrieves a stored texture
	static Texture2D &GetTexture(std::string name);

	// loads (and generates) an array texture from file
	static Texture2DArray &LoadDDSTextureArray(std::string name, std::vector<std::string> ddsFiles, bool mipmaps = true);
	// retrieves a stored array texture
	static Texture2DArray &GetTextureArray(std::string name);

	// loads (and generates) a font from file
	static Font &LoadFont(std::string name, std::string fontFile, unsigned int defaultFontSize);
	// retrieves a stored font
	static Font &GetFont(std::string name);

	// properly de-allocates all loaded resources
	static void Clear();

private:
	// private constructor, that is we do not want any actual resource manager objects.
	// Its members and functions should be publicly available (static).
	ResourceManager() {}
};

#endif
