#ifndef GAME_HPP
#define GAME_HPP

#include "../engine/objects/card_object.hpp"
#include "../engine/objects/ui_object.hpp"
#include "../state_manager.hpp"
#include "state_interface.hpp"

#include <vector>

// done to preserve pointers to CardObjects in the cards vector
// (MAX_CARDS space is reserved on construction of GameState so no reallocation will occur)
const unsigned int MAX_CARDS = 54;

class Game : public StateInterface {
public:
	Game(StateManager& manager);

	void ProcessInput(float dt) override;
	void Update(float dt) override;
	void Render() override;

	void OnEnter() override;
	void OnExit() override { Clear(); }
	void Clear();

private:
	StateManager& manager;

	std::vector<CardObject> cards;
	// stores a pointer to the card in the cards vector when said card is hovered over
	// set to nullptr if no card is hovered over
	CardObject* selectedCard = nullptr;
	GameObject background;
	Button pauseButton;
	// used for iterating through all the cards
	int cardCount = 0;
	int suitCount = 0;

	int GetCardTextureIndex(CardValue value, CardSuit suit);
	CardObject& makeCard(CardValue value, CardSuit suit, glm::vec2 pos);
};

#endif
