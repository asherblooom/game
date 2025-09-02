#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

#include "engine/render/sprite_renderer.hpp"
#include "engine/render/text_renderer.hpp"
#include "engine/resource_manager.hpp"
#include "states/state_interface.hpp"

class StateManager {
public:
	std::vector<std::unique_ptr<StateInterface>> States;
	// the max values in the coordinate system for x and y respectively
	unsigned int Width, Height;
	// renderers
	// these are pointers so that we can choose when to destruct them (that is, before glfwTerminate is called)
	SpriteRenderer* spriteRenderer;
	TextRenderer* textRenderer;

	StateManager(unsigned int width, unsigned int height)
		: Width{width}, Height{height} {
		// create default renderers
		spriteRenderer = new SpriteRenderer(width, height);
		textRenderer = new TextRenderer(width, height);
		// load assets needed across all states
		ResourceManager::LoadFont("default", "OpenSans-Regular.ttf", 32);
	}
	virtual ~StateManager() {
		delete spriteRenderer;
		delete textRenderer;
	}

private:
	StateInterface* currentState;

	void initialiseStates() {
	};
};
#endif
