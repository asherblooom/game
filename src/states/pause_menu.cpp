#include "pause_menu.hpp"

#include "../engine/resource_manager.hpp"

PauseMenu::PauseMenu(StateManager& manager)
	: manager{manager},
	  background{{0, 0}, {manager.Width, manager.Width * (9.0 / 16.0)}, ResourceManager::GetTexture("background0"), glm::vec3(0.8)} {
	buttons["resume"] = std::make_unique<Button>(glm::vec2{manager.Width / 2 - 168 / 2, manager.Height / 2 - 88 / 2},
												 glm::vec2{168, 88},
												 ResourceManager::GetTexture("play-button"));
	buttons["return"] = std::make_unique<Button>(glm::vec2{manager.Width / 2 - 56 / 2, manager.Height / 2 - 57 / 2 + 200},
												 glm::vec2{56, 57},
												 ResourceManager::GetTexture("return-button"));
}

void PauseMenu::Update(float dt) {
	for (auto& pair : buttons) {
		auto& button = pair.second;
		button->Update();

		if (button->State == ACTIVE) {
			if (pair.first == "resume")
				manager.PopState();	 // pop pause state only, return to game
			else if (pair.first == "return") {
				// we want to be at main menu
				manager.PopState();	 // pop pause state
				manager.PopState();	 // pop game state
			}
		}
	}
}
void PauseMenu::Render() {
	manager.spriteRenderer.Draw(&background);
	for (auto& pair : buttons) {
		manager.spriteRenderer.Draw(&*pair.second);
	}
	manager.textRenderer.RenderText("Game Paused", manager.Width / 2.0 - 320, 100, 1, ResourceManager::GetFont("default-100"));
	manager.textRenderer.RenderText("Resume", manager.Width / 2.0 - 95, manager.Height / 2.0 - 100, 1, ResourceManager::GetFont("default-50"), {0.9, 0.15, 0.33});
	manager.textRenderer.RenderText("Quit", manager.Width / 2.0 - 52, manager.Height / 2.0 + 115, 1, ResourceManager::GetFont("default-50"), {0.9, 0.15, 0.33});
}
