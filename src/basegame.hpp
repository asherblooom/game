#ifndef GAMEBASE_HPP
#define GAMEBASE_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "engine/resource_manager.hpp"

class BaseGame {
public:
	bool Keys[1024];
	bool MouseButtons[3];
	glm::vec2 MousePos;
	glm::vec2 ChangeInMousePos;
	BaseGame(unsigned int width, unsigned int height)
		: width{width}, height{height} {}
	// used to delete resources that must be deleted before GLFWTeminate() is called
	virtual void Clear() {}
	virtual ~BaseGame() { ResourceManager::Clear(); }

	virtual void Init() = 0;
	virtual void ProcessInput(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

protected:
	// the max values in the coordinate system for x and y respectively
	unsigned int width, height;
};
#endif
