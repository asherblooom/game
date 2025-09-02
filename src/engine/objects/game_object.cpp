#include "game_object.hpp"
#include "../input_manager.hpp"

GameObject::GameObject() : Color{0}, Size{0}, Position{0}, Rotation{0} {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color)
	: Color(color), Size(size), Position(pos), Rotation(0), Texture(texture) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2DArray textureArray, int textureIndex, glm::vec3 color)
	: Color(color), Size(size), Position(pos), Rotation(0), TextureArray(textureArray), TextureIndex(textureIndex) {}

bool GameObject::DetectMouseOver() {
	glm::vec2 minBounds = Position;
	glm::vec2 maxBounds = Position + Size;
	if (minBounds.x <= InputManager::MousePos.x && InputManager::MousePos.x < maxBounds.x &&
		minBounds.y <= InputManager::MousePos.y && InputManager::MousePos.y < maxBounds.y) {
		return true;
	} else
		return false;
}
