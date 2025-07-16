#ifndef GAME_HPP
#define GAME_HPP

#include "engine/objects/card_object.hpp"
#include "engine/render/sprite_renderer.hpp"
#include "engine/resource_manager.hpp"

#include <GLFW/glfw3.h>
#include <initializer_list>
#include <vector>

class Game {
private:
	// the max values in the coordinate system for x and y respectively
	unsigned int width, height;
	// this is a pointer so that we can choose when to destruct it (that is, before glfwTerminate is called)
	SpriteRenderer* renderer;
	std::vector<CardObject> Cards;
	// stores a pointer to the card in the cards vector when said card is hovered over
	// set to nullptr if no card is hovered over
	CardObject* selectedCard = nullptr;

	CardObject& makeCard(CardValue value, CardSuit suit, glm::vec2 pos);
	//TODO: implement this
	void LoadCardTextures(std::initializer_list<std::string> names);

public:
	bool Keys[1024];
	bool MouseButtons[3];
	glm::vec2 MousePos;
	glm::vec2 ChangeInMousePos;
	Game(unsigned int width, unsigned int height);
	~Game() {
		delete renderer;
		ResourceManager::Clear();
	}
	// initialize game state (load all shaders/textures/levels)
	void Init();
	// game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
};

#endif
