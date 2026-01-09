#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <list>
#include <map>
#include <memory>

#include "engine/render/sprite_renderer.hpp"
#include "engine/render/text_renderer.hpp"
#include "states/state_interface.hpp"

enum States {
	MAIN_MENU,
	GAME,
	PAUSE_MENU
};

class StateManager {
public:
	// the max values in the coordinate system for x and y respectively
	unsigned int Width, Height;
	// used by all states so stored here
	SpriteRenderer spriteRenderer;
	TextRenderer textRenderer;

	StateManager(unsigned int gameWidth, unsigned int gameHeight);

	// pass heap allocated states for the manager to use
	// will be freed by the manager on destruction
	void Add(States name, std::unique_ptr<StateInterface> state);
	// sets currentState to pushed state
	void PushState(States stateName);
	void PopState();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

private:
	std::map<States, std::unique_ptr<StateInterface>> states;
	std::list<StateInterface*> stack;
	// pointer to the top of stack
	StateInterface* currentState;
};
#endif
