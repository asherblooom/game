#include "texture.hpp"
#include <iostream>

Texture2D::Texture2D()
    : width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), ID_(0)
{
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->width = width;
    this->height = height;

    // create Texture
    glGenTextures(1, &this->ID_);
    glBindTexture(GL_TEXTURE_2D, this->ID_);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);

    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->ID_);
}

void Texture2D::Wrap_S(unsigned int wrap_s) {
    glBindTexture(GL_TEXTURE_2D, this->ID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
}

void Texture2D::Wrap_T(unsigned int wrap_t) {
    glBindTexture(GL_TEXTURE_2D, this->ID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
}

void Texture2D::Filter_Min(unsigned int filter_min) {
    glBindTexture(GL_TEXTURE_2D, this->ID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
}

void Texture2D::Filter_Max(unsigned int filter_max) {
    glBindTexture(GL_TEXTURE_2D, this->ID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
}
