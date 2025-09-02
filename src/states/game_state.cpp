#include "game_state.hpp"

#include <GLFW/glfw3.h>
#include "../engine/input_manager.hpp"
#include "../engine/resource_manager.hpp"

GameState::GameState(const StateManager& manager) : manager{manager} {
	background = GameObject({0, 0}, {manager.Width, manager.Width * (9.0 / 16.0)}, ResourceManager::GetTexture("background"));
}

void GameState::ProcessInput(float dt) {
	// add new card
	if (InputManager::MouseButtons[GLFW_MOUSE_BUTTON_LEFT] && !selectedCard) {
		cardCount += 1;
		// draw jokers once end of normal cards is reached
		if (cardCount == 14 && suitCount == 3) {
			// make black joker
			suitCount = 4;
			cardCount = 0;
		} else if (cardCount == 1 && suitCount == 4) {
			// make red joker
			suitCount = 5;
			cardCount = 0;
		} else if (suitCount == 5) {
			//reset
			suitCount = 0;
			cardCount = 1;
		}
		// switch to next suit
		if (cardCount == 14) {
			cardCount = 1;
			suitCount += 1;
		}
		makeCard((CardValue)cardCount, (CardSuit)suitCount, InputManager::MousePos);
		// only want one card per button press
		InputManager::MouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
	}
	// if plus key, increase size
	if (InputManager::Keys[GLFW_KEY_EQUAL] && (InputManager::Keys[GLFW_KEY_LEFT_SHIFT] || InputManager::Keys[GLFW_KEY_RIGHT_SHIFT])) {
		if (selectedCard) {
			selectedCard->Size += glm::vec2{21.6, 31.36};
		}
		InputManager::Keys[GLFW_KEY_EQUAL] = false;
	}
	// if minus key (not underscore) decrease size
	if (InputManager::Keys[GLFW_KEY_MINUS] && !(InputManager::Keys[GLFW_KEY_LEFT_SHIFT] || InputManager::Keys[GLFW_KEY_RIGHT_SHIFT])) {
		if (selectedCard) {
			selectedCard->Size -= glm::vec2{21.6, 31.36};
		}
		InputManager::Keys[GLFW_KEY_MINUS] = false;
	}
	// delete selected card
	if (InputManager::MouseButtons[GLFW_MOUSE_BUTTON_RIGHT]) {
		if (selectedCard) {
			for (size_t i = 0; i < cards.size(); i++) {
				if (&cards.at(i) == selectedCard) {
					cards.erase(cards.begin() + i);
					selectedCard = nullptr;
				}
			}
		}
		InputManager::MouseButtons[GLFW_MOUSE_BUTTON_RIGHT] = false;
	}
	if (InputManager::Keys[GLFW_KEY_C]) {
		selectedCard = nullptr;
		cards.clear();
		cardCount = 0;
		suitCount = 0;
		InputManager::Keys[GLFW_KEY_C] = false;
	}
	// if there is a selected card and the mouse button is released, deselect it
	if (selectedCard && !InputManager::MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
		selectedCard->Color += glm::vec3(0.2);
		selectedCard = nullptr;
	}
	// if no card currently selected, select a card which is over the mouse pointer
	// loop through cards in reverse order, so as to pick the one on top (drawn last) if any overlap
	if (!selectedCard) {
		for (int i = cards.size() - 1; i >= 0; i--) {
			CardObject& card = cards[i];
			if (card.DetectMouseOver()) {
				selectedCard = &card;
				// make selected card darker
				card.Color -= glm::vec3(0.2);
				// only select one card
				break;
			}
		}
	}
}

void GameState::Update(float dt) {
	// if there is a selected card and the mouse is down, make it follow the mouse pointer
	if (selectedCard && InputManager::MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
		selectedCard->Position += InputManager::ChangeInMousePos;
	}
}

void GameState::Render() {
	manager.spriteRenderer->Draw(&background);
	manager.textRenderer->RenderText("abcdefghijklmnopqrstuvwxyz", 100, 100, 1, ResourceManager::GetFont("default"));
	manager.textRenderer->RenderText("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 100, 150, 1, ResourceManager::GetFont("default"));
	for (CardObject& card : cards) {
		manager.spriteRenderer->Draw(&card);
	}
}

CardObject& GameState::makeCard(CardValue value, CardSuit suit, glm::vec2 pos) {
	Texture2DArray cardTexArray = ResourceManager::GetTextureArray("cards");
	int cardIndex = GetCardTextureIndex(value, suit);
	if (selectedCard) {
		int selectedLoc = 0;
		for (size_t i = 0; i < cards.size(); i++) {
			if (&cards.at(i) == selectedCard) {
				selectedLoc = i;
				selectedCard = nullptr;
			}
		}
		cards.emplace_back(value, suit, cardTexArray, cardIndex, pos);
		selectedCard = &cards.at(selectedLoc);
	} else
		cards.emplace_back(value, suit, cardTexArray, cardIndex, pos);
	return cards.back();
}

int GameState::GetCardTextureIndex(CardValue value, CardSuit suit) {
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
