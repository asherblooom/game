#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
GLuint compile_shaders(void);

int main(void)
{
	// Initialise glfw and set options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window =
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fragment Shader Example", NULL, NULL);
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

	// Initialise vertex data
	const GLfloat positions[] = {0, 0, 0, 0.5, 0.5, 0, 0, 0.5, 0.5};
	GLuint buffers[2];

	// get names for 2 buffers, point opengl to the beginning of the first elem of the array
	// opengl will just assume the array has enough space and will fill it up
	glGenBuffers(2, &buffers[0]);
	// let opengl know we want to transfer data from buffer[0] to vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	// fill buffer with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);

	GLuint vertex_array_object;
	glGenVertexArrays(1, &vertex_array_object);
	// let opengl know this vertex array is the one we want to use for input into vertex shader (when we draw the next polygon)
	glBindVertexArray(vertex_array_object);
	// fill vertex array index 0 with data from whatever is currently bound to GL_ARRAY_BUFFER
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	// tell opengl to use the vertex array when we request input at location 0
	glEnableVertexAttribArray(0);

	// don't add buffer data as we will do that in the event loop
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	// Initialse shaders etc.
	GLuint rendering_program = compile_shaders();

	// Event loop
	while(!glfwWindowShouldClose(window))
	{
		GLfloat time = {float(glfwGetTime())};

		process_input(window);

		GLfloat background_color[] = {
			std::sin(time) * 0.2f + 0.4f, std::cos(time) * 0.3f + 0.4f, 0.3f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color);

		glUseProgram(rendering_program);

		GLfloat colors[] = {1 + std::sin(time * 2),
							0,
							0,
							0,
							1 - std::sin(time * 2),
							0,
							0,
							0,
							1 - std::sin(time * 2)};
		GLfloat pos_offset[] = {std::sin(time) * 0.5f, std::cos(time) * 0.6f, 0.0f, 0.0f};

		// fill buffers[1] with data
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &colors, GL_STATIC_DRAW);

		glVertexAttrib4fv(2, pos_offset);

		glDrawArrays(GL_TRIANGLES, 0, 3);

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

	GLint success;
	char info_log[512];

	static const GLchar* vertex_shader_source = R"glsl(
		#version 430 core

		layout (location = 0) in vec3 pos;
		layout (location = 1) in vec3 color;
		layout (location = 2) in vec4 offset;
		out vec4 vs_color;

		void main(void)
		{
			gl_Position = vec4(pos.x, pos.y, pos.z, 1) + offset;
			vs_color = vec4(color.r, color.g, color.b, 1);
		}

	)glsl";

	static const GLchar* fragment_shader_source = R"glsl(
		#version 430 core

		in vec4 vs_color;
		out vec4 color;
		
		void main(void)
		{
			color = vs_color;
		}

	)glsl";

	// Create and compile vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	// check for compile-time errors
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << "\n";
	}

	// Create and compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	// check for compile-time errors
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << "\n";
	}

	// Create program, attach shaders to it, and link it
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	// check for linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(program, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << "\n";
	}

	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}
