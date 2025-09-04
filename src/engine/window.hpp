#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

struct RenderBounds {
	float x;
	float y;
	float width;
	float height;
};

struct Window {
	// here width and height are the max values for the x and y coords respectively
	Window(unsigned int width, unsigned int height, std::string name);
	~Window();

	bool ShouldClose();
	float GetElapsedFrameTime();
	void PollEvents();
	void SetBackground(float red, float green, float blue);
	void SwapBuffers();

	// TODO: get rid of input manager and just have it all here???
	// MousePos UpdatemousePosition();
	// Input UpdateInput();

private:
	GLFWwindow *window;
	const unsigned int width = 1920;
	const unsigned int height = 1080;

	float lastFrameTime = 0.0f;

	// used to calculate change in mouse position
	glm::vec2 oldMousePosWorld = {0, 0};  // world space position
	int xposOld, yposOld;				  // screen space position
	double xpos, ypos;

	static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static RenderBounds calculateRenderBounds(int windowWidth, int windowHeight);
	void updateMousePosition(GLFWwindow *window, double xpos, double ypos);
};

#endif
