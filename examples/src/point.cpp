#include <glad/glad.hpp>

#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const GLfloat background_colour[] = {0.2f, 0.3f, 0.3f, 1.0f};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
GLuint compile_shaders(void);

int main()
{
	// Initialise glfw and set options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Point Example", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialise GLAD - manages function pointers for OpenGL
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialse shaders etc.
	GLuint rendering_program = compile_shaders();
	GLuint vertex_array_object;
	glCreateVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	// Event loop
	while(!glfwWindowShouldClose(window))
	{
		process_input(window);
		glClearBufferfv(GL_COLOR, 0, background_colour);

		glUseProgram(rendering_program);

		glPointSize(40.0f);
		glDrawArrays(GL_POINTS, 0, 1);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
	glfwTerminate();
	return 0;
}

// callback for updating window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// callback for processing input
void process_input(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
	   glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// returns a program object
GLuint compile_shaders(void)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	static const GLchar* vertex_shader_source[] = {
		"#version 450 core                                  \n"
		"                                                   \n"
		"void main(void)                                    \n"
		"{                                                  \n"
		"    gl_Position = vec4(0.0, 0.0, 0.5, 1.0);        \n"
		"}                                                  \n"};

	static const GLchar* fragment_shader_source[] = {
		"#version 450 core                                  \n"
		"                                                   \n"
		"out vec4 color;                                    \n"
		"                                                   \n"
		"void main(void)                                    \n"
		"{                                                  \n"
		"    color = vec4(0.0, 0.8, 1.0, 1.0);              \n"
		"}                                                  \n"};

	// Create and compile vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// Create and compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// Create program, attach shaders to it, and link it
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}
