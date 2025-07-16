#include "card_object.hpp"

CardObject::CardObject(CardValue value,
					   CardSuit suit,
					   Texture2D texture,
					   Shader shader,
					   glm::vec2 pos,
					   glm::vec2 size,
					   glm::vec3 color)
	: GameObject{pos, size, texture, shader, color}, Value{value}, Suit{suit} {}
