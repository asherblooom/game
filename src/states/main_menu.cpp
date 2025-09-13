#include "main_menu.hpp"

#include "../engine/resource_manager.hpp"

MainMenu::MainMenu(StateManager& manager)
	: manager{manager},
	  background{{0, 0}, {manager.Width, manager.Width * (9.0 / 16.0)}, ResourceManager::GetTexture("background0")} {
	buttons["play"] = std::make_unique<Button>(glm::vec2{manager.Width / 2 - 168 / 2, manager.Height / 2 - 88 / 2},
											   glm::vec2{168, 88},
											   ResourceManager::GetTexture("play-button"));
}

void MainMenu::ProcessInput(float dt) {
}
void MainMenu::Update(float dt) {
	for (auto& pair : buttons) {
		auto& button = pair.second;
		button->Update();
		if (button->State == HOVERED) {
			button->Color = glm::vec3(0.7);
		} else if (button->State == ACTIVE) {
			if (pair.first == "play")
				manager.PushState(GAME);
		} else if (button->State == INACTIVE) {
			button->Color = glm::vec3(1);
		}
	}
}
void MainMenu::Render() {
	manager.spriteRenderer.Draw(&background);
	for (auto& pair : buttons) {
		manager.spriteRenderer.Draw(&*pair.second);
	}
	manager.textRenderer.RenderText("Card Game", manager.Width / 2.0 - 255, 100, 1, ResourceManager::GetFont("default-100"));
	manager.textRenderer.RenderText("Play", manager.Width / 2.0 - 49, manager.Height / 2.0 - 100, 1, ResourceManager::GetFont("default-50"), {0.9, 0.15, 0.33});
}
