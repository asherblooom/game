#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

#include "../shader.hpp"
#include "font.hpp"

// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer {
public:
	TextRenderer(unsigned int width, unsigned int height);
	// renders a string of text using the precompiled list of characters
	void RenderText(std::string text, float x, float y, float scale, Font font, glm::vec3 color = glm::vec3(1.0f));

private:
	unsigned int VAO, VBO;
	Shader textShader;
};

#endif
