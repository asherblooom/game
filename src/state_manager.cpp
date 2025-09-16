#include "state_manager.hpp"
#include <iostream>

StateManager::StateManager(unsigned int gameWidth, unsigned int gameHeight)
	: Width{gameWidth},
	  Height{gameHeight},
	  spriteRenderer{SpriteRenderer(gameWidth, gameHeight)},
	  textRenderer{TextRenderer(gameWidth, gameHeight)} {}

void StateManager::Add(States name, std::unique_ptr<StateInterface> state) {
	states.emplace(name, std::move(state));
}

void StateManager::PushState(States stateName) {
	StateInterface* state = &*states.at(stateName);
	stack.push_back(state);
	currentState = state;
	state->OnEnter();
}

void StateManager::PopState() {
	if (stack.size() <= 1) {
		std::cerr << "ERROR::STATE_MANAGER: Can't pop from a stack of size " << stack.size() << "\n";
		return;
	}
	stack.back()->OnExit();
	stack.pop_back();
	currentState = stack.back();
}

void StateManager::ProcessInput(float dt) {
	currentState->ProcessInput(dt);
}

void StateManager::Update(float dt) {
	currentState->Update(dt);
}

void StateManager::Render() {
	currentState->Render();
}
