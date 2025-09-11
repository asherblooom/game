#include "menu_state.hpp"

#include "../engine/resource_manager.hpp"

MenuState::MenuState(StateManager& manager) : manager{manager} {
	background = GameObject({0, 0}, {manager.Width, manager.Width * (9.0 / 16.0)}, ResourceManager::GetTexture("background"));
	buttons.emplace_back(glm::vec2{100, 100}, glm::vec2{168, 88}, ResourceManager::GetTexture("button"));
}

void MenuState::ProcessInput(float dt) {
}
void MenuState::Update(float dt) {
}
void MenuState::Render() {
	manager.spriteRenderer.Draw(&background);
	for (Button button : buttons) {
		manager.spriteRenderer.Draw(&button);
	}
}
