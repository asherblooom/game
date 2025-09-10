#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "../engine/objects/card_object.hpp"
#include "../state_manager.hpp"
#include "state_interface.hpp"

class GameState : public StateInterface {
public:
	GameState(StateManager& manager);

	void ProcessInput(float dt) override;
	void Update(float dt) override;
	void Render() override;

private:
	StateManager& manager;

	std::vector<CardObject> cards;
	// stores a pointer to the card in the cards vector when said card is hovered over
	// set to nullptr if no card is hovered over
	CardObject* selectedCard = nullptr;
	GameObject background;
	// used for iterating through all the cards
	int cardCount = 0;
	int suitCount = 0;

	int GetCardTextureIndex(CardValue value, CardSuit suit);
	CardObject& makeCard(CardValue value, CardSuit suit, glm::vec2 pos);
};

#endif
