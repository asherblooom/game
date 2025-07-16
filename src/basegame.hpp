#ifndef GAMEBASE_HPP
#define GAMEBASE_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "engine/resource_manager.hpp"

class BaseGame {
public:
	bool Keys[1024];
	bool MouseButtons[3];
	glm::vec2 MousePos;
	glm::vec2 ChangeInMousePos;
	BaseGame(unsigned int width, unsigned int height);
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

private:
	void Run();
	void framebuffer_size_callback(GLFWwindow *window, int width, int height);
	void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
	void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
	void updateMousePosition(GLFWwindow *window, double xpos, double ypos);
};
#endif
