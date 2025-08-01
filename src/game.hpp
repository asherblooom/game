#ifndef GAME_HPP
#define GAME_HPP

#include "basegame.hpp"
#include "engine/objects/card_object.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class Game : public BaseGame {
public:
	Game(unsigned int width, unsigned int height) : BaseGame(width, height) {}
	// initialize game state
	void Init() override;
	// game loop
	void ProcessInput(float dt) override;
	void Update(float dt) override;
	void Render() override;

private:
	std::vector<CardObject> Cards;
	// stores a pointer to the card in the cards vector when said card is hovered over
	// set to nullptr if no card is hovered over
	CardObject* selectedCard = nullptr;
	CardObject& makeCard(CardValue value, CardSuit suit, glm::vec2 pos);

	// used for iterating through all the cards
	int cardCount = 0;
	int suitCount = 0;
};

#endif
