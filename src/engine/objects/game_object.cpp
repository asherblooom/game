#include "game_object.hpp"

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, Shader shader, glm::vec3 color)
	: Color(color), Size(size), Position(pos), Rotation(0), texture(texture), shader(shader) {}

void GameObject::Draw(SpriteRenderer& renderer) {
	renderer.DrawSprite(Position,
						Size,
						Color,
						Rotation,
						texture,
						shader);
}
