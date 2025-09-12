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

// TODO: ADD ANIMATIONS!!!!!
void CardObject::Flip() {
	animations[FLIP] = std::make_unique<FlipAnimation>(Position, Size, TextureIndex, cardIndex, backIndex, 20);
	if (Side == FACEUP) {
		// TextureIndex = backIndex;
		Side = FACEDOWN;
	} else if (Side == FACEDOWN) {
		// TextureIndex = cardIndex;
		Side = FACEUP;
	}
}
void CardObject::FlipTo(CardSide side) {
	if (side == FACEDOWN && Side == FACEUP) {
		animations[FLIP] = std::make_unique<FlipAnimation>(Position, Size, TextureIndex, cardIndex, backIndex, 20);
		// TextureIndex = backIndex;
		Side = FACEDOWN;
	} else if (side == FACEUP && Side == FACEDOWN) {
		animations[FLIP] = std::make_unique<FlipAnimation>(Position, Size, TextureIndex, cardIndex, backIndex, 20);
		// TextureIndex = cardIndex;
		Side = FACEUP;
	}
}
