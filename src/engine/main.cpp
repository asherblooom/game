#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "../game.hpp"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
Game *cardGame;

// used to calculate change in mouse position
glm::vec2 oldMousePosWorld = {0, 0};  // world space position
int xposOld, yposOld;				  // screen space position

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
				  int mode);
void mouse_button_callback(GLFWwindow *window, int button, int action,
						   int mods);
void updateMousePosition(GLFWwindow *window, double xpos, double ypos);

int main() {
	// Initialise glfw and set options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	// minimal version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // required on Mac/WSLg

	// create window
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Card Game", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialise GLAD - manages function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		return -1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set initial viewport
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	framebuffer_size_callback(window, width, height);

	// here width and height are the max values for the x and y coords respectively
	cardGame = new Game(SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	cardGame->Init();

	float deltaTime, lastFrame = 0.0f;
	double xpos, ypos;

	// Event loop
	while (!glfwWindowShouldClose(window)) {
		// calculate delta time
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// manage user input
		// -----------------
		glfwGetCursorPos(window, &xpos, &ypos);
		if (xpos - xposOld != 0 && ypos - yposOld != 0) {
			updateMousePosition(window, xpos, ypos);
		} else {
			cardGame->ChangeInMousePos = glm::vec2(0);
		}
		cardGame->ProcessInput(deltaTime);

		// update game state
		// -----------------
		cardGame->Update(deltaTime);

		// render
		// ------
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		cardGame->Render();

		glfwSwapBuffers(window);
	}
	ResourceManager::Clear();
	delete cardGame;
	glfwTerminate();
	return 0;
}

// calculates the boundaries of the screen space needed to maintain a constant
// aspect ratio and keep everything displayed
auto calculateRenderBounds(int windowWidth, int windowHeight) {
	struct bounds {
		float x;
		float y;
		float width;
		float height;
	};
	float gameAspectRatio = 16.0f / 9.0f;
	float windowAspectRatio = float(windowWidth) / float(windowHeight);
	if (windowAspectRatio < gameAspectRatio) {
		// need black bars top and bottom to preserve game aspect ratio
		float heightRange = windowWidth * (1 / gameAspectRatio);
		float heightOffset = (windowHeight - heightRange) / 2;
		return bounds{0, heightOffset, (float)windowWidth,
					  windowHeight - heightOffset * 2};

	} else if (windowAspectRatio > gameAspectRatio) {
		// need black bars left and right to preserve game aspect ratio
		float widthRange = windowHeight * gameAspectRatio;
		float widthOffset = (windowWidth - widthRange) / 2;
		return bounds{widthOffset, 0, windowWidth - widthOffset * 2,
					  (float)windowHeight};

	} else
		return bounds{0, 0, (float)windowWidth, (float)windowHeight};
}

// callback for updating window size
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	auto [x, y, gameWidth, gameHeight] = calculateRenderBounds(width, height);
	glViewport(x, y, gameWidth, gameHeight);
}

// callback for processing input
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			cardGame->Keys[key] = true;
		else if (action == GLFW_RELEASE)
			cardGame->Keys[key] = false;
	}
}

void updateMousePosition(GLFWwindow *window, double xpos, double ypos) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	auto [x, y, gameWidth, gameHeight] = calculateRenderBounds(width, height);

	// transform coordinates from screen space into world space

	// move top left to match beginning of render area
	xpos = xpos - x;
	ypos = ypos - y;
	// we know world space has coordinates (0, 0) to (SCR_WIDTH, SCR_HEIGHT)
	//  as those constants are what we used to construct our Game object with
	double xRatio = SCR_WIDTH / gameWidth;
	double yRatio = SCR_HEIGHT / gameHeight;
	glm::vec2 mousePos = {xpos * xRatio, ypos * yRatio};

	cardGame->MousePos = mousePos;
	cardGame->ChangeInMousePos = mousePos - oldMousePosWorld;
	oldMousePosWorld = mousePos;
}

// callback for processing mouse input
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	if (button >= 0 && button <= 2) {
		if (action == GLFW_PRESS)
			cardGame->MouseButtons[button] = true;
		else if (action == GLFW_RELEASE)
			cardGame->MouseButtons[button] = false;
	}
}
