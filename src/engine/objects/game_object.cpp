#include "game_object.hpp"

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color)
	: Color(color), Size(size), Position(pos), Rotation(0), Texture(texture) {}

bool GameObject::DetectMouseOver(glm::vec2 mousePos) {
	glm::vec2 minBounds = Position;
	glm::vec2 maxBounds = Position + Size;
	if (minBounds.x <= mousePos.x && mousePos.x < maxBounds.x &&
		minBounds.y <= mousePos.y && mousePos.y < maxBounds.y) {
		return true;
	} else
		return false;
}
