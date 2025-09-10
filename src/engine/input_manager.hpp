#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <glm/glm.hpp>

struct InputManager {
	static bool Keys[1024];
	static bool MouseButtons[3];
	static glm::vec2 ScreenMousePos;
	// static glm::vec2 ChangeInMousePos;

	static glm::vec2 GetWorldMousePos(const unsigned int gameWidth, const unsigned int gameHeight,
									  const unsigned int windowXStart, const unsigned int windowYStart,
									  const unsigned int windowWidth, const unsigned int windowHeight);
};

#endif
