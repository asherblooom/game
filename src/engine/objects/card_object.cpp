#include "card_object.hpp"
#include "../resource_manager.hpp"

CardObject::CardObject(CardValue value,
					   CardSuit suit,
					   Texture2DArray textureArray,
					   int textureIndex,
					   glm::vec2 pos,
					   CardSide side,
					   glm::vec3 color,
					   glm::vec2 size)
	: GameObject{pos, size, textureArray, textureIndex, color},
	  Value{value},
	  Suit{suit},
	  Side{side},
	  cardIndex(textureIndex),
	  backIndex(ResourceManager::GetArrayItemIndex("cards", "BACK")) {
	if (side == FACEDOWN)
		TextureIndex = backIndex;
}

void CardObject::Flip() {
	if (Side == FACEUP) {
		TextureIndex = backIndex;
		Side = FACEDOWN;
	} else if (Side == FACEDOWN) {
		TextureIndex = cardIndex;
		Side = FACEUP;
	}
}
void CardObject::FlipTo(CardSide side) {
	if (side == FACEDOWN) {
		TextureIndex = backIndex;
		Side = FACEDOWN;
	} else if (side == FACEUP) {
		TextureIndex = cardIndex;
		Side = FACEUP;
	}
}
