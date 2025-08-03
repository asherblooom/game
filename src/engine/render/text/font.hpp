#ifndef FONT_HPP
#define FONT_HPP
#include <glm/glm.hpp>
#include <map>

/// Holds all state information relevant to a character as loaded using FreeType
struct CharacterData {
	unsigned int TextureID;	 // ID handle of the glyph texture
	glm::ivec2 Size;		 // size of glyph
	glm::ivec2 Bearing;		 // offset from baseline to left/top of glyph
	unsigned int Advance;	 // horizontal offset to advance to next glyph
};

using Font = std::map<char, CharacterData>;

#endif
