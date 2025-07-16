#include "game.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

int cardCount = 0;
int suitCount = 0;

Game::Game(unsigned int width, unsigned int height)
	: width(width), height(height) {
}

void Game::Init() {
	// load shaders
	ResourceManager::LoadShader("sprite", "src/shaders/sprite.vs", "src/shaders/sprite.frag");
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
	if (Keys[GLFW_KEY_C]) {
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
		makeCard((CardValue)cardCount, (CardSuit)suitCount, {width / 2 - 108, 0});
		// only want one card per key press
		Keys[GLFW_KEY_C] = false;
	}
	// if there is a selected card and the mouse button is no longer held, make it react to forces
	// if the mouse pointer is no longer over it, deselect it
	if (selectedCard && !MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
		if (!(selectedCard->DetectMouseOver(MousePos))) {
			selectedCard->Color -= glm::vec3(0.1);
			selectedCard = nullptr;
		}
	}
	// if no card currently selected, select a card which is over the mouse pointer
	// loop through cards in reverse order, so as to pick the one on top (drawn last) if any overlap
	if (!selectedCard) {
		for (int i = Cards.size() - 1; i >= 0; i--) {
			auto& card = Cards[i];
			if (card.DetectMouseOver(MousePos)) {
				selectedCard = &card;
				// make selected card brighter
				card.Color += glm::vec3(0.1);
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
	for (int i = 0; i < (int)Cards.size() - 1; i++) {
		Cards.at(i).Position.y += 1;
	}
}

void Game::Render() {
	for (CardObject& card : Cards) {
		card.Draw(*renderer);
	}
}

CardObject& Game::makeCard(CardValue value, CardSuit suit, glm::vec2 pos) {
	Texture2D cardTex = GetCardTexture(value, suit);
	// Texture2D cardTexBack = ResourceManager::GetTexture("BACK");
	Shader cardShader = ResourceManager::GetShader("sprite");

	if (selectedCard) {
		int selectedLoc = 0;
		for (int i = 0; i < (int)Cards.size(); i++) {
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
	// ResourceManager::LoadDDSTexture("BACK", "textures/BACK.dds");
	ResourceManager::LoadDDSTexture("JOKER-BLACKJOKER", "textures/JOKER-BLACKJOKER.dds");
	ResourceManager::LoadDDSTexture("JOKER-REDJOKER", "textures/JOKER-REDJOKER.dds");
	std::string suits[] = {"SPADES", "HEARTS", "DIAMONDS", "CLUBS"};
	std::string values[] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
							"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};
	for (std::string suit : suits) {
		for (std::string value : values) {
			std::string name = value + "-" + suit;
			ResourceManager::LoadDDSTexture(name, ("textures/" + name + ".dds").c_str());
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
