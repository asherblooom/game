#ifndef SPRITE_RENDERER_HPP
#define SPRITE_RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../objects/game_object.hpp"
#include "shader.hpp"

class SpriteRenderer {
public:
	SpriteRenderer(unsigned int gameWidth, unsigned int gameHeight);
	~SpriteRenderer();
	// Renders a defined quad textured with given texture using the renderer's shader
	void Draw(GameObject* object);

private:
	Shader shader;
	unsigned int VAO;
};

#endif
