#ifndef GAME_HPP
#define GAME_HPP

#include "basegame.hpp"
#include "engine/objects/card_object.hpp"
#include "engine/render/sprite_renderer.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class Game : public BaseGame {
public:
	Game(unsigned int width, unsigned int height)
		: BaseGame(width, height) {}

	// initialize game state (load all shaders/textures/levels)
	void Init() override;
	// game loop
	void ProcessInput(float dt) override;
	void Update(float dt) override;
	void Render() override;

	// used to delete resources that must be deleted before GLFWTeminate() is called
	void Clear() override { delete renderer; }

private:
	// this is a pointer so that we can choose when to destruct it (that is, before glfwTerminate is called)
	SpriteRenderer* renderer;
	std::vector<CardObject> Cards;
	// stores a pointer to the card in the cards vector when said card is hovered over
	// set to nullptr if no card is hovered over
	CardObject* selectedCard = nullptr;

	CardObject& makeCard(CardValue value, CardSuit suit, glm::vec2 pos);
	void LoadCardTextures();
	Texture2D GetCardTexture(CardValue value, CardSuit suit);
};

#endif
