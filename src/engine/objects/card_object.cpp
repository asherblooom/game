#include "card_object.hpp"

CardObject::CardObject(CardValue value,
					   CardSuit suit,
					   Texture2DArray textureArray,
					   int textureIndex,
					   glm::vec2 pos,
					   glm::vec2 size,
					   glm::vec3 color)
	: GameObject{pos, size, textureArray, textureIndex, color}, Value{value}, Suit{suit} {}
