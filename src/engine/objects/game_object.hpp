#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/glm.hpp>

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
	Texture2D Texture;

	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color = glm::vec3(1.0f));

	bool DetectMouseOver(glm::vec2 mousePos);
};

#endif
