#include "state_manager.hpp"

StateManager::StateManager(unsigned int gameWidth, unsigned int gameHeight)
	: Width{gameWidth},
	  Height{gameHeight},
	  spriteRenderer{SpriteRenderer(gameWidth, gameHeight)},
	  textRenderer{TextRenderer(gameWidth, gameHeight)} {}

StateManager::~StateManager() {
	for (auto state : states) {
		delete (state.second);
	}
}

void StateManager::Add(std::string name, StateInterface* state) {
	states.emplace(name, state);
}

void StateManager::SetStart(std::string stateName) {
	currentState = states.at(stateName);
}

void StateManager::ProcessInput(float dt) {
	currentState->ProcessInput(dt);
}

void StateManager::Update(float dt) {
	StateInterface* newState = currentState->Update(dt);
	// done like this so we don't delete the currentState until we have returned from it's method
	if (newState != nullptr) {
		delete currentState;
		currentState = newState;
	}
}

void StateManager::Render() {
	currentState->Render();
}
