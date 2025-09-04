#include "window.hpp"
#include <iostream>
#include "input_manager.hpp"
#include "resource_manager.hpp"

Window::Window(unsigned int width, unsigned int height, std::string name)
	: width{width}, height{height} {
	// Initialise glfw and set options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow *window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
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
}

Window::~Window() {
	ResourceManager::Clear();
	glfwTerminate();
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

// calculates the boundaries of the screen space needed to maintain a constant
// aspect ratio and keep everything displayed
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

// callback for updating window size
void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
	RenderBounds bounds = calculateRenderBounds(width, height);
	glViewport(bounds.x, bounds.y, bounds.width, bounds.height);
}

// callback for processing input
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

void Window::updateMousePosition(GLFWwindow *window, double xpos, double ypos) {
	int currentW, currentH;
	glfwGetWindowSize(window, &currentW, &currentH);
	RenderBounds bounds = calculateRenderBounds(currentW, currentH);

	// transform coordinates from screen space into world space

	// move top left to match beginning of render area
	xpos = xpos - bounds.x;
	ypos = ypos - bounds.y;
	// we know world space has coordinates (0, 0) to (SCR_WIDTH, SCR_HEIGHT)
	//  as those constants are what we used to construct our Game object with
	double xRatio = width / bounds.width;
	double yRatio = height / bounds.height;
	glm::vec2 mousePos = {xpos * xRatio, ypos * yRatio};

	InputManager::MousePos = mousePos;
	InputManager::ChangeInMousePos = mousePos - oldMousePosWorld;
	oldMousePosWorld = mousePos;
}

// callback for processing mouse input
void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if (button >= 0 && button <= 2) {
		if (action == GLFW_PRESS)
			InputManager::MouseButtons[button] = true;
		else if (action == GLFW_RELEASE)
			InputManager::MouseButtons[button] = false;
	}
}
