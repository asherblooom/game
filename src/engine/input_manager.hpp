#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <glm/glm.hpp>

struct InputManager {
	static bool Keys[1024];
	static bool MouseButtons[3];

	// Refers to world (or game) coordinates.
	// Must be updated using InputManager::UpdateWorldMousePos before it can be used
	static glm::vec2 MousePos;
	// Refers to world (or game) coordinates.
	// Must be updated using InputManager::UpdateWorldMousePos before it can be used
	static glm::vec2 ChangeInMousePos;
	static void SetScreenMousePos(unsigned int xpos, unsigned int ypos);

	static void UpdateWorldMousePos(const unsigned int gameWidth, const unsigned int gameHeight,
									const unsigned int windowXStart, const unsigned int windowYStart,
									const unsigned int windowWidth, const unsigned int windowHeight);

private:
	static glm::vec2 oldMouse;
	static glm::vec2 screenMousePos;
};

#endif
