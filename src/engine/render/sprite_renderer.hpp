
#ifndef SPRITE_RENDERER_HPP
#define SPRITE_RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../objects/game_object.hpp"
#include "shader.hpp"

struct Vertex {
	// Position
	float x;
	float y;
	//Texture coordinates
	float s;
	float t;
};

class SpriteRenderer {
public:
	SpriteRenderer(Shader shader);
	~SpriteRenderer();
	// Renders a defined quad textured with given texture using the renderer's shader
	void Draw(GameObject* object);

private:
	Shader shader;
	unsigned int VAO;
	// Initializes and configures VAO
	void initRenderData();
};

#endif
