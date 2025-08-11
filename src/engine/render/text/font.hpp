#ifndef FONT_HPP
#define FONT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

/// Holds all state information relevant to a character as loaded using FreeType
struct CharacterData {
	glm::ivec2 Position;   // position of glyph texture in texture atlas
	glm::ivec2 Size;	   // size of glyph
	glm::ivec2 Bearing;	   // offset from baseline to left/top of glyph
	unsigned int Advance;  // horizontal offset to advance to next glyph
};

struct Font {
	std::map<char, CharacterData> Characters;
	GLuint TextureAtlas;
};

#endif
