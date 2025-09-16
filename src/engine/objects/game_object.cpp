#include "game_object.hpp"
#include <memory>
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

void GameObject::MoveTo(glm::vec2 location, float speed) {
	animations[MOVETO] = std::make_unique<MoveToAnimation>(Position, Size, location, speed);
}

void GameObject::RotateTo(float newRotation, float speed) {
	animations[ROTATE] = std::make_unique<RotateAnimation>(Rotation, newRotation, speed);
}

void GameObject::Rotate(int degrees, float speed) {
	animations[ROTATE] = std::make_unique<RotateAnimation>(Rotation, Rotation + degrees, speed);
}

void GameObject::Animate() {
	for (auto iter = animations.begin(), nextIter = iter; iter != animations.end(); iter = nextIter) {
		++nextIter;
		auto& animation = iter->second;
		if (animation->Finished)
			animations.erase(iter);
		else
			animation->Run();
	}
}

bool GameObject::HasAnimations() {
	return !animations.empty();
}
