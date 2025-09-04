#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "engine/render/sprite_renderer.hpp"
#include "engine/render/text_renderer.hpp"
#include "resource_loader.hpp"
#include "states/state_interface.hpp"

class GameState;

class StateManager {
public:
	// the max values in the coordinate system for x and y respectively
	unsigned int Width, Height;
	SpriteRenderer spriteRenderer;
	TextRenderer textRenderer;

	StateManager(unsigned int width, unsigned int height)
		: Width{width}, Height{height}, spriteRenderer{SpriteRenderer(width, height)}, textRenderer{TextRenderer(width, height)} {
		// TODO: make these static??? they only use width/height for projection matrix.... put that somewhere else??
		// Then we don't need to initialise? what about destructing them before glfwTerminate?...
	}

	void Start() {
		ResourceLoader::LoadAll();
		currentState = new GameState(*this);
	}
	// TODO: load all textures/fonts here!!?!?!?!
	// TODO: get rid of input manager????

	// TODO: add cleanup functions to each state?

	void ProcessInput(float dt) { currentState->ProcessInput(dt); }
	void Update(float dt) {
		StateInterface* newState = currentState->Update(dt);
		// done like this so we don't delete the currentState until we have returned from it's method
		if (newState != nullptr) {
			delete currentState;
			currentState = newState;
		}
	}
	void Render() { currentState->Render(); }

private:
	StateInterface* currentState;

	void initialiseStates() {
	};
};
#endif
