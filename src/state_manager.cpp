#include "state_manager.hpp"
#include <iostream>

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

void StateManager::Add(States name, StateInterface* state) {
	states.emplace(name, state);
}

void StateManager::PushState(States stateName) {
	StateInterface* state = states.at(stateName);
	stack.push_back(state);
	currentState = state;
}

StateInterface* StateManager::PopState() {
	if (stack.size() <= 1) {
		std::cerr << "ERROR::STATE_MANAGER: Can't pop from a stack of size " << stack.size() << "\n";
		throw;
	}
	StateInterface* state = stack.back();
	stack.pop_back();
	currentState = stack.back();
	return state;
}

void StateManager::ProcessInput(float dt) {
	currentState->ProcessInput(dt);
}

void StateManager::Update(float dt) {
	currentState->Update(dt);
	for (auto iter = currentState->Animations.begin(); iter < currentState->Animations.end(); iter++) {
		auto& animation = *iter;
		if (animation->Finished)
			currentState->Animations.erase(iter);
		else
			animation->Run();
	}
}

void StateManager::Render() {
	currentState->Render();
}
