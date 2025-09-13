#ifndef CARDOBJECT_HPP
#define CARDOBJECT_HPP

#include "../render/texture.hpp"
#include "game_object.hpp"

enum CardValue {
	JOKER,
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING
};

enum CardSuit {
	SPADES,
	HEARTS,
	DIAMONDS,
	CLUBS,
	BLACKJOKER,
	REDJOKER
};

enum CardSide {
	FACEUP,
	FACEDOWN
};

class CardObject : public GameObject {
public:
	CardValue Value;
	CardSuit Suit;
	CardSide Side;

	CardObject(CardValue value,
			   CardSuit suit,
			   Texture2DArray textureArray,
			   int textureIndex,
			   glm::vec2 pos,
			   CardSide side = FACEDOWN,
			   glm::vec3 color = glm::vec3(1.0f),
			   glm::vec2 size = {200, 280});

	void Flip(float speed = 20);
	void FlipTo(CardSide side, float speed = 20);

private:
	int cardIndex;
	int backIndex;
};

#endif
