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
	Window(std::string name, unsigned int width = 1920, unsigned int height = 1080);
	~Window();

	bool ShouldClose();
	float GetElapsedFrameTime();
	void PollEvents();
	// TODO: can we turn this into a callback especially now that input manager is it's own thing???
	void UpdateMousePosition();
	void SetBackground(float red, float green, float blue);
	void SwapBuffers();

private:
	GLFWwindow *window;
	const unsigned int width;
	const unsigned int height;

	float lastFrameTime = 0.0f;

	// used to calculate change in mouse position
	glm::vec2 oldMousePosWorld = {0, 0};  // world space position
	double xposOld, yposOld;			  // screen space position
	double xpos, ypos;

	// callback for updating window size
	static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
	// callback for processing input
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
	// callback for processing mouse input
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	// calculates the boundaries of the screen space needed to maintain a constant
	// aspect ratio and keep everything displayed
	static RenderBounds calculateRenderBounds(int windowWidth, int windowHeight);
};

#endif
