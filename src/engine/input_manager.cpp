#include "input_manager.hpp"

bool InputManager::Keys[1024];
bool InputManager::MouseButtons[3];
glm::vec2 InputManager::MousePos = {0, 0};
glm::vec2 InputManager::ChangeInMousePos = {0, 0};

glm::vec2 InputManager::screenMousePos = {0, 0};
glm::vec2 InputManager::oldMouse = {0, 0};

void InputManager::SetScreenMousePos(unsigned int xpos, unsigned int ypos) {
	screenMousePos = {xpos, ypos};
}

void InputManager::UpdateWorldMousePos(const unsigned int gameWidth, const unsigned int gameHeight,
									   const unsigned int windowXStart, const unsigned int windowYStart,
									   const unsigned int windowWidth, const unsigned int windowHeight) {
	float xpos = screenMousePos.x;
	float ypos = screenMousePos.y;
	// transform coordinates from screen space into world space

	// move top left to match beginning of render area
	xpos = xpos - windowXStart;
	ypos = ypos - windowYStart;
	// we know world space has coordinates (0, 0) to (gameWidth, gameHeight)
	double xRatio = gameWidth / (double)windowWidth;
	double yRatio = gameHeight / (double)windowHeight;
	MousePos = {xpos * xRatio, ypos * yRatio};
	ChangeInMousePos = MousePos - oldMouse;
	oldMouse = MousePos;
}
