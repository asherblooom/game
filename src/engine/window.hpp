#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>

#include <AL/alc.h>
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
	Window(std::string name, unsigned int width = 1920, unsigned int height = 1080);
	~Window();

	bool ShouldClose();
	float GetElapsedFrameTime();
	void PollEvents();
	void SetBackground(float red, float green, float blue);
	void SwapBuffers();

	// getters for variables that relate to size and position of screen inside of (black) bounding bars

	// gets the top left x coordinate of usable screen space
	const unsigned int &XStart() const { return xStart; }
	// gets the top left y coordinate of usable screen space
	const unsigned int &YStart() const { return yStart; }
	// gets the width of usable screen space
	const unsigned int &Width() const { return scrWidth; }
	// gets the height of usable screen space
	const unsigned int &Height() const { return scrHeight; }

private:
	ALCdevice *device;
	ALCcontext *context;

	GLFWwindow *window;
	static unsigned int scrWidth;
	static unsigned int scrHeight;
	static unsigned int xStart;
	static unsigned int yStart;

	float lastFrameTime = 0.0f;

	// callback for updating window size
	static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
	// callback for processing input
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
	// callback for processing mouse input
	static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	// callback for processing mouse movement
	static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
	// calculates the boundaries of the screen space needed to maintain a constant
	// aspect ratio and keep everything displayed
	static RenderBounds calculateRenderBounds(int windowWidth, int windowHeight);
};

#endif
