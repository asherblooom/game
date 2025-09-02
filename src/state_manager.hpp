#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

#include "engine/render/sprite_renderer.hpp"
#include "engine/render/text_renderer.hpp"
#include "resource_loader.hpp"
#include "states/state_interface.hpp"

class StateManager {
public:
	std::vector<std::unique_ptr<StateInterface>> States;
	// the max values in the coordinate system for x and y respectively
	unsigned int Width, Height;
	SpriteRenderer spriteRenderer;
	TextRenderer textRenderer;

	StateManager(unsigned int width, unsigned int height)
		: Width{width}, Height{height}, spriteRenderer{SpriteRenderer(width, height)}, textRenderer{TextRenderer(width, height)} {
		// TODO: make these static??? they only use width/height for projection matrix.... put that somewhere else??
		// Then we don't need to initialise? what about destructing them before glfwTerminate?...
	}

	void Start(StateInterface* startState) {
		currentState = startState;
	}
	// TODO: load all textures/fonts here!!?!?!?!
	// TODO: get rid of input manager????

	void ChangeState(StateInterface* state);
	// TODO: add cleanup functions to each state?
	// implement this function!

	void ProcessInput(float dt) { currentState->ProcessInput(dt); }
	void Update(float dt) { currentState->Update(dt); }
	void Render() { currentState->Render(); }

private:
	StateInterface* currentState;

	void initialiseStates() {
	};
};
#endif
