#ifndef GAMEBASE_HPP
#define GAMEBASE_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "engine/objects/card_object.hpp"
#include "engine/render/sprite_renderer.hpp"
#include "engine/render/text/text_renderer.hpp"
#include "engine/resource_manager.hpp"

class BaseGame {
public:
	bool Keys[1024];
	bool MouseButtons[3];
	glm::vec2 MousePos;
	glm::vec2 ChangeInMousePos;
	BaseGame(unsigned int width, unsigned int height)
		: width{width}, height{height} {
		// create default renderers
		spriteRenderer = new SpriteRenderer(width, height);
		textRenderer = new TextRenderer(width, height);
		// load default assets
		LoadCardTextures();
		ResourceManager::LoadFont("default", "OpenSans-Regular.ttf", 32);
	}
	virtual ~BaseGame() {
		delete spriteRenderer;
		delete textRenderer;
	}

	virtual void Init() = 0;
	virtual void ProcessInput(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	// the max values in the coordinate system for x and y respectively
	unsigned int width, height;
	// renderers
	// these are pointers so that we can choose when to destruct them (that is, before glfwTerminate is called)
	SpriteRenderer* spriteRenderer;
	TextRenderer* textRenderer;

	// basic utility functions for loading and getting card textures
	void LoadCardTextures() {
		ResourceManager::LoadDDSTexture("JOKER-BLACKJOKER", "JOKER-BLACKJOKER.dds");
		ResourceManager::LoadDDSTexture("JOKER-REDJOKER", "JOKER-REDJOKER.dds");
		std::string suits[] = {"SPADES", "HEARTS", "DIAMONDS", "CLUBS"};
		std::string values[] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
								"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};
		for (std::string suit : suits) {
			for (std::string value : values) {
				std::string name = value + "-" + suit;
				ResourceManager::LoadDDSTexture(name, (name + ".dds"), false);
			}
		}
	}
	Texture2D& GetCardTexture(CardValue value, CardSuit suit) {
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
};
#endif
