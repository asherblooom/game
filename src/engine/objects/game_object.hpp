#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/glm.hpp>

#include "../render/shader.hpp"
#include "../render/sprite_renderer.hpp"
#include "../render/texture.hpp"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject {
public:
	glm::vec3 Color;
	glm::vec2 Size;
	glm::vec2 Position;
	float Rotation;

	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, Shader shader, glm::vec3 color = glm::vec3(1.0f));

	void Draw(SpriteRenderer& renderer);

protected:
	Texture2D texture;
	Shader shader;
};

#endif
