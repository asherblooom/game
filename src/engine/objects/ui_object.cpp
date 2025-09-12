#include "ui_object.hpp"
#include "../input_manager.hpp"
#include "game_object.hpp"

#include <GLFW/glfw3.h>

UIObject::UIObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color)
	: GameObject{pos, size, texture, color}, State{INACTIVE} {}
UIObject::UIObject(glm::vec2 pos, glm::vec2 size, Texture2DArray textureArray, int textureIndex, glm::vec3 color)
	: GameObject{pos, size, textureArray, textureIndex, color}, State{INACTIVE} {}

Button::Button(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color)
	: UIObject(pos, size, texture, color) {}

void Button::Update() {
	if (DetectMouseOver()) {
		if (InputManager::MouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
			State = ACTIVE;
			InputManager::MouseButtons[GLFW_MOUSE_BUTTON_LEFT] = false;
		} else {
			State = HOVERED;
			if (colorChanged == false) {
				Color -= glm::vec3(0.2);
				colorChanged = true;
			}
		}
	} else {
		State = INACTIVE;
		if (colorChanged == true) {
			Color += glm::vec3(0.2);
			colorChanged = false;
		}
	}
}
