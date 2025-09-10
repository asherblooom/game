#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "engine/render/sprite_renderer.hpp"
#include "engine/render/text_renderer.hpp"
#include "states/state_interface.hpp"

class StateManager {
public:
	// the max values in the coordinate system for x and y respectively
	unsigned int Width, Height;
	// used by all states so stored here
	SpriteRenderer spriteRenderer;
	TextRenderer textRenderer;

	StateManager(unsigned int gameWidth, unsigned int gameHeight);
	~StateManager();

	// pass heap allocated states for the manager to use
	// will be freed by the manager on destruction
	void Add(std::string name, StateInterface* state);
	void SetStart(std::string stateName);

	// TODO: add start and cleanup functions to each state
	// TODO: stop making new states etc. for each change!
	// TODO: add a state stack!

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

private:
	std::map<std::string, StateInterface*> states;
	StateInterface* currentState;
};
#endif
