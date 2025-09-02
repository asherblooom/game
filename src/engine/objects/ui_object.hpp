#ifndef UIOBJECT_HPP
#define UIOBJECT_HPP

#include "game_object.hpp"

enum UIState {
	INACTIVE,
	HOVERED,
	ACTIVE
};

class UIObject : public GameObject {
public:
	UIObject(glm::vec2 pos, glm::vec2 size, Texture2D texture, glm::vec3 color = glm::vec3(1.0f));
	UIObject(glm::vec2 pos, glm::vec2 size, Texture2DArray textureArray, int textureIndex, glm::vec3 color = glm::vec3(1.0f));
	UIState State;
	// update state based on mouse input
	virtual void Update() = 0;
};

class Button : public UIObject {
	void Update() override;
};

class TextBox : public UIObject {
};

class ToggleButton : public UIObject {
};

class Slider : public UIObject {
};

#endif
