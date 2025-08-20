#include "resource_manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Define missing DDS texture format constants
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif

// Define FourCC codes for DDS file parsing
#ifndef FOURCC_DXT1
#define FOURCC_DXT1 0x31545844 // "DXT1"
#endif
#ifndef FOURCC_DXT3
#define FOURCC_DXT3 0x33545844 // "DXT3"
#endif
#ifndef FOURCC_DXT5
#define FOURCC_DXT5 0x35545844 // "DXT5"
#endif

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader& ResourceManager::LoadShader(std::string name, const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D& ResourceManager::LoadTexture(std::string name, const char *file, bool alpha)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D& ResourceManager::LoadDDSTexture(std::string name, const char* file, bool mipmaps)
{
    (void)mipmaps; // Unused parameter
    Textures[name] = loadDDSFromFile(file);
    return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID()); // Correct: Call ID() as a function
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID_); // Correct: Access ID_ via friend status
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files: " << e.what() << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    Texture2D texture;
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data)
    {
        if (nrChannels == 4 && alpha)
        {
            texture.internalFormat = GL_RGBA;
            texture.imageFormat = GL_RGBA;
        }
        else
        {
            texture.internalFormat = GL_RGB;
            texture.imageFormat = GL_RGB;
        }
        texture.Generate(width, height, data);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture from file: " << file << std::endl;
    }
    return texture;
}

Texture2D ResourceManager::loadDDSFromFile(const char* file) {
    unsigned char header[124];
    FILE *fp;

    fp = fopen(file, "rb");
    if (fp == NULL){
        printf("%s could not be opened.\n", file);
        return Texture2D();
    }

    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        printf("Not a DDS file: %s\n", file);
        return Texture2D();
    }

    fread(&header, 124, 1, fp);

    unsigned int height      = *(unsigned int*)&(header[8]);
    unsigned int width         = *(unsigned int*)&(header[12]);
    unsigned int linearSize  = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);

    unsigned char *buffer;
    unsigned int bufsize;
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    fclose(fp);

    unsigned int format;
    switch(fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    default:
        free(buffer);
        printf("Unsupported DDS format in %s\n", file);
        return Texture2D();
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);
        offset += size;
        width  /= 2;
        height /= 2;
        if(width < 1) width = 1;
        if(height < 1) height = 1;
    }

    free(buffer);

    Texture2D texture;
    texture.ID_ = textureID; // Correct: Assign to the private member via friend status
    
    glBindTexture(GL_TEXTURE_2D, texture.ID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    return texture;
}
