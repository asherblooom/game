
#ifndef SPRITE_RENDERER_HPP
#define SPRITE_RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture.hpp"

class SpriteRenderer {
public:
	SpriteRenderer();
	~SpriteRenderer();
	// Renders a defined quad textured with given texture and using given shader
	void DrawSprite(glm::vec2 pos, glm::vec2 size, glm::vec3 color, float rotation, Texture2D texture, Shader shader);

private:
	// Render state
	unsigned int quadVAO;
	// Initializes and configures quadVAO
	void initRenderData();
};

#endif
