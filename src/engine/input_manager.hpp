#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <glm/glm.hpp>

struct InputManager {
	static bool Keys[1024];
	static bool MouseButtons[3];
	static glm::vec2 MousePos;
	static glm::vec2 ChangeInMousePos;
};

#endif
