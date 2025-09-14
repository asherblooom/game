#ifndef FONT_HPP
#define FONT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

/// Holds all state information relevant to a character as loaded using FreeType
struct CharacterData {
	friend class ResourceManager;
	friend class TextRenderer;

protected:
	CharacterData(glm::vec2 texPos, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance)
		: TexPos{texPos}, Size{size}, Bearing{bearing}, Advance{advance} {}

	glm::vec2 TexPos;	   // position of glyph texture in texture atlas
	glm::ivec2 Size;	   // size of glyph
	glm::ivec2 Bearing;	   // offset from baseline to left/top of glyph
	unsigned int Advance;  // horizontal offset to advance to next glyph
};

struct Font {
	friend class ResourceManager;
	friend class TextRenderer;

protected:
	std::map<char, CharacterData> Characters;
	GLuint TextureAtlas;
	glm::ivec2 AtlasSize;
};

#endif
