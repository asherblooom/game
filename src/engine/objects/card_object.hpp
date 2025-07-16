#ifndef CARDOBJECT_HPP
#define CARDOBJECT_HPP

#include "../render/shader.hpp"
#include "../render/texture.hpp"
#include "game_object.hpp"

enum CardValue {
	ACE,
	ONE,
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
	KING,
	JOKER
};

enum CardSuit {
	SPADES,
	HEARTS,
	DIAMONDS,
	CLUBS
};

class CardObject : public GameObject {
public:
	CardValue Value;
	CardSuit Suit;

	CardObject(CardValue value,
			   CardSuit suit,
			   Texture2D texture,
			   Shader shader,
			   glm::vec2 pos,
			   glm::vec2 size = {50, 50},
			   glm::vec3 color = glm::vec3(1.0f));
};

#endif
