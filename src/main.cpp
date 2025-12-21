#include <iostream>
#include <memory>

#include "engine/input_manager.hpp"
#include "engine/window.hpp"
#include "resource_loader.hpp"
#include "state_manager.hpp"
#include "states/game.hpp"
#include "states/main_menu.hpp"
#include "states/pause_menu.hpp"

int main() {
	try {
		// size for game world; also initial size of window
		const unsigned int GAME_WIDTH = 1920;
		const unsigned int GAME_HEIGHT = 1080;
		// must create window before resources are loaded as we need opengl and openal contexts to be active
		Window window{"Card Game", GAME_WIDTH, GAME_HEIGHT};
		ResourceLoader::LoadAll();

		StateManager manager{GAME_WIDTH, GAME_HEIGHT};

		manager.Add(MAIN_MENU, std::make_unique<MainMenu>(manager));
		manager.Add(GAME, std::make_unique<Game>(manager));
		manager.Add(PAUSE_MENU, std::make_unique<PauseMenu>(manager));
		manager.PushState(MAIN_MENU);

		while (!window.ShouldClose()) {
			float dt = window.GetElapsedFrameTime();
			window.PollEvents();

			// mouse position is stored internally as a screen space coordinate, but we need it in world space
			// so we must give InputManager the size of our game world and the size of the screen/window,
			// and ask it to convert between the two and update its MousePos variable
			InputManager::UpdateWorldMousePos(GAME_WIDTH, GAME_HEIGHT, window.XStart(), window.YStart(), window.Width(), window.Height());
			manager.ProcessInput(dt);

			manager.Update(dt);

			window.SetBackground(0, 0, 0);
			manager.Render();

			window.SwapBuffers();
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
