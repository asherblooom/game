#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// global variables
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

GLint mv_location;
GLint proj_location;
float aspect;
glm::mat4 proj_matrix;
GLuint vertex_array_object;
GLuint buffer;

// callback for updating window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	// recalculate projection matrix
	aspect = (float)width / (float)height;
	proj_matrix = glm::perspective(glm::radians(50.0f), aspect, 0.1f, 1000.0f);
}

// callback for processing input
void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// returns a program object
GLuint compile_shaders(void) {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	GLint success;
	char info_log[512];

	static const GLchar* vertex_shader_source = R"glsl(
		#version 430 core

		layout (location = 0) in vec3 pos;
		layout (location = 1) in vec3 color;
		layout (location = 2) in vec4 offset;
		out vec4 vs_color;

		void main(void) {
			gl_Position = vec4(pos.x, pos.y, pos.z, 1) + offset;
			vs_color = vec4(color.r, color.g, color.b, 1);
		}

	)glsl";

	static const GLchar* fragment_shader_source = R"glsl(
		#version 430 core

		in vec4 vs_color;
		out vec4 color;
		
		void main(void) {
			color = vs_color;
		}

	)glsl";

	// Create and compile vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	// check for compile-time errors
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << info_log << "\n";
	}

	// Create and compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	// check for compile-time errors
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << info_log << "\n";
	}

	// Create program, attach shaders to it, and link it
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	// check for linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << info_log << "\n";
	}

	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

GLFWwindow* initWindow(int openglVersionMajor, int openglVersionMinor, int width, int height, std::string title) {
	// Initialise glfw and set options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window =
		glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	// initialise GLAD - manages function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		return nullptr;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

void initVAO() {
	// Initialise vertex array object
	glGenVertexArrays(1, &vertex_array_object);
	// let opengl know this vertex array is the one we want to use for input
	// into vertex shader (when we draw the next polygon)
	glBindVertexArray(vertex_array_object);

	// Initialise vertex data for cube
	static const GLfloat vertex_positions[] =
		{
			-0.25f, 0.25f, -0.25f,
			-0.25f, -0.25f, -0.25f,
			0.25f, -0.25f, -0.25f,

			0.25f, -0.25f, -0.25f,
			0.25f, 0.25f, -0.25f,
			-0.25f, 0.25f, -0.25f,

			0.25f, -0.25f, -0.25f,
			0.25f, -0.25f, 0.25f,
			0.25f, 0.25f, -0.25f,

			0.25f, -0.25f, 0.25f,
			0.25f, 0.25f, 0.25f,
			0.25f, 0.25f, -0.25f,

			0.25f, -0.25f, 0.25f,
			-0.25f, -0.25f, 0.25f,
			0.25f, 0.25f, 0.25f,

			-0.25f, -0.25f, 0.25f,
			-0.25f, 0.25f, 0.25f,
			0.25f, 0.25f, 0.25f,

			-0.25f, -0.25f, 0.25f,
			-0.25f, -0.25f, -0.25f,
			-0.25f, 0.25f, 0.25f,

			-0.25f, -0.25f, -0.25f,
			-0.25f, 0.25f, -0.25f,
			-0.25f, 0.25f, 0.25f,

			-0.25f, -0.25f, 0.25f,
			0.25f, -0.25f, 0.25f,
			0.25f, -0.25f, -0.25f,

			0.25f, -0.25f, -0.25f,
			-0.25f, -0.25f, -0.25f,
			-0.25f, -0.25f, 0.25f,

			-0.25f, 0.25f, -0.25f,
			0.25f, 0.25f, -0.25f,
			0.25f, 0.25f, 0.25f,

			0.25f, 0.25f, 0.25f,
			-0.25f, 0.25f, 0.25f,
			-0.25f, 0.25f, -0.25f};

	// get names for buffer
	glGenBuffers(1, &buffer);
	// let opengl know we want to transfer data from buffer to vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// fill buffer with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), &vertex_positions, GL_STATIC_DRAW);
	// fill vertex array index 0 with data from whatever is currently bound to GL_ARRAY_BUFFER
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	// tell opengl to use the vertex array when we request input at location 0
	glEnableVertexAttribArray(0);
}

int main(void) {
	GLFWwindow* window = initWindow(4, 3, SCR_WIDTH, SCR_HEIGHT, "Spinning Cubes Example");

	// calculate projection matrix
	aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	proj_matrix = glm::perspective(glm::radians(50.0f), aspect, 0.1f, 1000.0f);

	initVAO();

	// Initialse shaders etc.
	GLuint rendering_program = compile_shaders();

	// Event loop
	while (!glfwWindowShouldClose(window)) {
		GLfloat time = {float(glfwGetTime())};

		process_input(window);

		GLfloat background_color[] = {
			std::sin(time) * 0.2f + 0.4f, std::cos(time) * 0.3f + 0.4f, 0.3f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color);

		glUseProgram(rendering_program);

		// create model-view matrix
		float f = time * M_PI * 0.1f;
		auto mv_matrix = glm::mat4(1.0f);
		mv_matrix = glm::rotate(mv_matrix, glm::radians(time * 81.0f), glm::vec3{1.0f, 0.0f, 0.0f});
		mv_matrix = glm::rotate(mv_matrix, glm::radians(time * 45.0f), glm::vec3{0.0f, 1.0f, 0.0f});
		mv_matrix = glm::translate(mv_matrix,
								   glm::vec3{std::sin(2.1f * f) * 0.5f, std::cos(1.7f * f) * 0.5f, std::sin(1.3f * f) * std::cos(1.5f * f) * 2.0f});
		mv_matrix = glm::translate(mv_matrix, glm::vec3{0.0f, 0.0f, -4.0f});

		// Set the model-view and projection matrices
		glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(mv_matrix));
		glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj_matrix));

		// draw 6 faces of 2 triangles of 3 vertices each = 36 vertices
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
	glfwTerminate();
	return 0;
}
