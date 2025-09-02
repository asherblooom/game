#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <memory>
#include "../engine/objects/card_object.hpp"
#include "../state_manager.hpp"
#include "state_interface.hpp"

class GameState : public StateInterface {
public:
	GameState(const StateManager& manager) : manager{manager} {
		LoadCardTextures();
		ResourceManager::LoadDDSTexture("background", "background.dds");
		background = GameObject({0, 0}, {manager.Width, manager.Width * (9.0 / 16.0)}, ResourceManager::GetTexture("background"));
	}

	void Init() override;
	void ProcessInput(float dt) override;
	void Update(float dt) override;
	void Render() override;

private:
	const StateManager& manager;

	std::vector<CardObject> cards;
	// stores a pointer to the card in the cards vector when said card is hovered over
	// set to nullptr if no card is hovered over
	CardObject* selectedCard = nullptr;
	GameObject background;
	// used for iterating through all the cards
	int cardCount = 0;
	int suitCount = 0;

	// basic utility functions for loading and getting card textures
	void LoadCardTextures() {
		// ResourceManager::LoadDDSTexture("JOKER-BLACKJOKER", "JOKER-BLACKJOKER.dds");
		// ResourceManager::LoadDDSTexture("JOKER-REDJOKER", "JOKER-REDJOKER.dds");
		std::vector<std::string> names{"JOKER-BLACKJOKER", "JOKER-REDJOKER"};
		std::vector<std::string> fileNames{"JOKER-BLACKJOKER.dds", "JOKER-REDJOKER.dds"};
		std::string suits[] = {"SPADES", "HEARTS", "DIAMONDS", "CLUBS"};
		std::string values[] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
								"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};
		for (std::string suit : suits) {
			for (std::string value : values) {
				std::string name = value + "-" + suit;
				names.emplace_back(name);
				fileNames.emplace_back(name + ".dds");
				// ResourceManager::LoadDDSTexture(name, (name + ".dds"), false);
			}
		}
		ResourceManager::LoadDDSTextureArray("cards", names, fileNames, false);
	}
	int GetCardTextureIndex(CardValue value, CardSuit suit) {
		if (value == JOKER) {
			if (suit == BLACKJOKER)
				return ResourceManager::GetArrayItemIndex("cards", "JOKER-BLACKJOKER");
			else if (suit == REDJOKER)
				return ResourceManager::GetArrayItemIndex("cards", "JOKER-REDJOKER");
		}
		std::string suits[] = {"SPADES", "HEARTS", "DIAMONDS", "CLUBS"};
		std::string values[] = {"JOKER", "ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
								"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};
		return ResourceManager::GetArrayItemIndex("cards", values[value] + "-" + suits[suit]);
	}
	CardObject& makeCard(CardValue value, CardSuit suit, glm::vec2 pos);
};

#endif
