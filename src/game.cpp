#include "game.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include "engine/objects/game_object.hpp"
#include "engine/render/text/box_packer.hpp"
#include "engine/resource_manager.hpp"

void Game::Init() {
	ResourceManager::LoadDDSTexture("background", "background.dds");
	background = new GameObject({0, 0}, {width, width * (9.0 / 16.0)}, ResourceManager::GetTexture("background"));
	BoxPacker pack(100, 100);
	glm::vec2 loc;
	loc = pack.AddBox({80, 10});
	std::cout << "position: (" << loc.x << ", " << loc.y << ") ";
	std::cout << "box: (" << 80 << ", " << 10 << ")\n";

	loc = pack.AddBox({10, 5});
	std::cout << "position: (" << loc.x << ", " << loc.y << ") ";
	std::cout << "box: (" << 10 << ", " << 5 << ")\n";

	loc = pack.AddBox({10, 10});
	std::cout << "position: (" << loc.x << ", " << loc.y << ") ";
	std::cout << "box: (" << 10 << ", " << 10 << ")\n";

	loc = pack.AddBox({80, 10});
	std::cout << "position: (" << loc.x << ", " << loc.y << ") ";
	std::cout << "box: (" << 80 << ", " << 10 << ")\n";

	loc = pack.AddBox({80, 10});
	std::cout << "position: (" << loc.x << ", " << loc.y << ") ";
	std::cout << "box: (" << 80 << ", " << 10 << ")\n";
}

void Game::ProcessInput(float dt) {
	// add new card
	if (MouseButtons[GLFW_MOUSE_BUTTON_LEFT] && !selectedCard) {
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
		makeCard((CardValue)cardCount, (CardSuit)suitCount, MousePos);
		// only want one card per button press
		MouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
	}
	// if plus key, increase size
	if (Keys[GLFW_KEY_EQUAL] && (Keys[GLFW_KEY_LEFT_SHIFT] || Keys[GLFW_KEY_RIGHT_SHIFT])) {
		if (selectedCard) {
			selectedCard->Size += glm::vec2{21.6, 31.36};
		}
		Keys[GLFW_KEY_EQUAL] = false;
	}
	// if minus key (not underscore) decrease size
	if (Keys[GLFW_KEY_MINUS] && !(Keys[GLFW_KEY_LEFT_SHIFT] || Keys[GLFW_KEY_RIGHT_SHIFT])) {
		if (selectedCard) {
			selectedCard->Size -= glm::vec2{21.6, 31.36};
		}
		Keys[GLFW_KEY_MINUS] = false;
	}
	// delete selected card
	if (MouseButtons[GLFW_MOUSE_BUTTON_RIGHT]) {
		if (selectedCard) {
			for (size_t i = 0; i < Cards.size(); i++) {
				if (&Cards.at(i) == selectedCard) {
					Cards.erase(Cards.begin() + i);
					selectedCard = nullptr;
				}
			}
		}
		MouseButtons[GLFW_MOUSE_BUTTON_RIGHT] = false;
	}
	if (Keys[GLFW_KEY_C]) {
		selectedCard = nullptr;
		Cards.clear();
		cardCount = 0;
		suitCount = 0;
		Keys[GLFW_KEY_C] = false;
	}
	// if there is a selected card and the mouse button is released, deselect it
	if (selectedCard && !MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
		selectedCard->Color += glm::vec3(0.2);
		selectedCard = nullptr;
	}
	// if no card currently selected, select a card which is over the mouse pointer
	// loop through cards in reverse order, so as to pick the one on top (drawn last) if any overlap
	if (!selectedCard) {
		for (int i = Cards.size() - 1; i >= 0; i--) {
			CardObject& card = Cards[i];
			if (card.DetectMouseOver(MousePos)) {
				selectedCard = &card;
				// make selected card darker
				card.Color -= glm::vec3(0.2);
				// only select one card
				break;
			}
		}
	}
}

void Game::Update(float dt) {
	// if there is a selected card and the mouse is down, make it follow the mouse pointer
	if (selectedCard && MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
		selectedCard->Position += ChangeInMousePos;
	}
}

void Game::Render() {
	spriteRenderer->Draw(background);
	textRenderer->RenderText("hello", 100, 100, 1, ResourceManager::GetFont("default"));
	for (CardObject& card : Cards) {
		spriteRenderer->Draw(&card);
	}
}

CardObject& Game::makeCard(CardValue value, CardSuit suit, glm::vec2 pos) {
	Texture2D cardTex = GetCardTexture(value, suit);
	if (selectedCard) {
		int selectedLoc = 0;
		for (size_t i = 0; i < Cards.size(); i++) {
			if (&Cards.at(i) == selectedCard) {
				selectedLoc = i;
				selectedCard = nullptr;
			}
		}
		Cards.emplace_back(value, suit, cardTex, pos);
		selectedCard = &Cards.at(selectedLoc);
	} else
		Cards.emplace_back(value, suit, cardTex, pos);
	return Cards.back();
}
