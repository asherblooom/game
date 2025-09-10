#include "window.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include "input_manager.hpp"
#include "resource_manager.hpp"

unsigned int Window::scrWidth = 0;
unsigned int Window::scrHeight = 0;
unsigned int Window::xStart = 0;
unsigned int Window::yStart = 0;

Window::Window(std::string name, unsigned int width, unsigned int height) {
	scrWidth = width;
	scrHeight = height;
	// Initialise glfw and set options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		throw;
	}
	glfwMakeContextCurrent(window);

	// initialise GLAD - manages function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		throw;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set initial viewport
	int currentW, currentH;
	glfwGetWindowSize(window, &currentW, &currentH);
	framebufferSizeCallback(window, currentW, currentH);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
}

Window::~Window() {
	ResourceManager::Clear();
	glfwTerminate();
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(window);
}

float Window::GetElapsedFrameTime() {
	float currentFrameTime = glfwGetTime();
	float deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
	return deltaTime;
}

void Window::PollEvents() {
	glfwPollEvents();
}

void Window::SetBackground(float red, float green, float blue) {
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(window);
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
	RenderBounds bounds = calculateRenderBounds(width, height);
	scrWidth = bounds.width;
	scrHeight = bounds.height;
	xStart = bounds.x;
	yStart = bounds.y;
	glViewport(bounds.x, bounds.y, bounds.width, bounds.height);
}

void Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			InputManager::Keys[key] = true;
		else if (action == GLFW_RELEASE)
			InputManager::Keys[key] = false;
	}
}

void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (button >= 0 && button <= 2) {
		if (action == GLFW_PRESS)
			InputManager::MouseButtons[button] = true;
		else if (action == GLFW_RELEASE)
			InputManager::MouseButtons[button] = false;
	}
}

void Window::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
	InputManager::SetScreenMousePos(xpos, ypos);
}

RenderBounds Window::calculateRenderBounds(int windowWidth, int windowHeight) {
	float gameAspectRatio = 16.0f / 9.0f;
	float windowAspectRatio = float(windowWidth) / float(windowHeight);
	if (windowAspectRatio < gameAspectRatio) {
		// need black bars top and bottom to preserve game aspect ratio
		float heightRange = windowWidth * (1 / gameAspectRatio);
		float heightOffset = (windowHeight - heightRange) / 2;
		return RenderBounds{0, heightOffset, (float)windowWidth, windowHeight - heightOffset * 2};

	} else if (windowAspectRatio > gameAspectRatio) {
		// need black bars left and right to preserve game aspect ratio
		float widthRange = windowHeight * gameAspectRatio;
		float widthOffset = (windowWidth - widthRange) / 2;
		return RenderBounds{widthOffset, 0, windowWidth - widthOffset * 2, (float)windowHeight};

	} else
		return RenderBounds{0, 0, (float)windowWidth, (float)windowHeight};
}
