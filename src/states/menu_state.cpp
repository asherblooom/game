#include "menu_state.hpp"

#include "../engine/resource_manager.hpp"

MenuState::MenuState(StateManager& manager) : manager{manager} {
	background = GameObject({0, 0}, {manager.Width, manager.Width * (9.0 / 16.0)}, ResourceManager::GetTexture("background"));
	buttons.emplace_back(glm::vec2{manager.Width / 2 - 168 / 2, manager.Height / 2 - 88 / 2}, glm::vec2{168, 88}, ResourceManager::GetTexture("button"));
}

void MenuState::ProcessInput(float dt) {
}
void MenuState::Update(float dt) {
	for (Button button : buttons) {
		button.Update();
		if (button.State == HOVERED) {
			button.Color -= glm::vec3(0.2);
		}
		if (button.State == ACTIVE) {
			manager.PushState(GAME);
		}
	}
}
void MenuState::Render() {
	manager.spriteRenderer.Draw(&background);
	for (Button button : buttons) {
		manager.spriteRenderer.Draw(&button);
	}
}
