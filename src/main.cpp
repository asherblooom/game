#include "engine/input_manager.hpp"
#include "engine/window.hpp"
#include "resource_loader.hpp"
#include "state_manager.hpp"
#include "states/game_state.hpp"
#include "states/menu_state.hpp"

int main() {
	// size for game world; also initial size of window
	const unsigned int GAME_WIDTH = 1920;
	const unsigned int GAME_HEIGHT = 1080;
	Window window{"Card Game", GAME_WIDTH, GAME_HEIGHT};
	ResourceLoader::LoadAll();

	StateManager manager{GAME_WIDTH, GAME_HEIGHT};
	// manager.Add("main menu", new MenuState(manager));
	manager.Add("game", new GameState(manager));
	manager.SetStart("game");

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
}
