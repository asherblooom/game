#include "input_manager.hpp"

bool InputManager::Keys[1024];
bool InputManager::MouseButtons[3];
glm::vec2 InputManager::ScreenMousePos;
// glm::vec2 InputManager::ChangeInMousePos;

glm::vec2 InputManager::GetWorldMousePos(const unsigned int gameWidth, const unsigned int gameHeight,
										 const unsigned int windowXStart, const unsigned int windowYStart,
										 const unsigned int windowWidth, const unsigned int windowHeight) {
	float xpos = ScreenMousePos.x;
	float ypos = ScreenMousePos.y;
	// transform coordinates from screen space into world space

	// move top left to match beginning of render area
	xpos = xpos - windowXStart;
	ypos = ypos - windowYStart;
	// we know world space has coordinates (0, 0) to (gameWidth, gameHeight)
	double xRatio = gameWidth / (double)windowWidth;
	double yRatio = gameHeight / (double)windowHeight;
	return {xpos * xRatio, ypos * yRatio};
}
