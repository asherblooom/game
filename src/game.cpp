#include "game.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

void Game::Init() {
	// load shaders
	ResourceManager::LoadShader("sprite", "src/shaders/sprite.vert", "src/shaders/sprite.frag");
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	Shader& spriteShader = ResourceManager::GetShader("sprite");
	spriteShader.Use();
	spriteShader.SetInteger("image", 0);
	spriteShader.SetMatrix4("projection", projection);
	// set render-specific controls
	renderer = new SpriteRenderer();
	// load textures
	LoadCardTextures();
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
	for (CardObject& card : Cards) {
		card.Draw(*renderer);
	}
}

CardObject& Game::makeCard(CardValue value, CardSuit suit, glm::vec2 pos) {
	Texture2D cardTex = GetCardTexture(value, suit);
	Shader cardShader = ResourceManager::GetShader("sprite");

	if (selectedCard) {
		int selectedLoc = 0;
		for (size_t i = 0; i < Cards.size(); i++) {
			if (&Cards.at(i) == selectedCard) {
				selectedLoc = i;
				selectedCard = nullptr;
			}
		}
		Cards.emplace_back(value, suit, cardTex, cardShader, pos);
		selectedCard = &Cards.at(selectedLoc);
	} else
		Cards.emplace_back(value, suit, cardTex, cardShader, pos);
	return Cards.back();
}

void Game::LoadCardTextures() {
	ResourceManager::LoadDDSTexture("JOKER-BLACKJOKER", "media/textures/JOKER-BLACKJOKER.dds");
	ResourceManager::LoadDDSTexture("JOKER-REDJOKER", "media/textures/JOKER-REDJOKER.dds");
	std::string suits[] = {"SPADES", "HEARTS", "DIAMONDS", "CLUBS"};
	std::string values[] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
							"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};
	for (std::string suit : suits) {
		for (std::string value : values) {
			std::string name = value + "-" + suit;
			ResourceManager::LoadDDSTexture(name, ("media/textures/" + name + ".dds").c_str(), false);
		}
	}
}

Texture2D Game::GetCardTexture(CardValue value, CardSuit suit) {
	if (value == JOKER) {
		if (suit == BLACKJOKER)
			return ResourceManager::GetTexture("JOKER-BLACKJOKER");
		else if (suit == REDJOKER)
			return ResourceManager::GetTexture("JOKER-REDJOKER");
	}
	std::string suits[] = {"SPADES", "HEARTS", "DIAMONDS", "CLUBS"};
	std::string values[] = {"JOKER", "ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
							"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};
	return ResourceManager::GetTexture(values[value] + "-" + suits[suit]);
}
