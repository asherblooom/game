#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

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

    // loads (and generates) a shader program from file
    static Shader &LoadShader(std::string name, const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // retrieves a stored shader
    static Shader &GetShader(std::string name);

    // loads (and generates) a standard texture from file
    static Texture2D &LoadTexture(std::string name, const char *file, bool alpha);
    // loads (and generates) a DDS texture from file
    static Texture2D &LoadDDSTexture(std::string name, const char *ddsFile, bool mipmaps = true);
    // retrieves a stored texture
    static Texture2D &GetTexture(std::string name);

    // properly de-allocates all loaded resources
    static void Clear();

private:
    // private constructor
    ResourceManager() {}

    // private static utility functions to load from file
    static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile);
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
    static Texture2D loadDDSFromFile(const char *file);
};

#endif
