#include "shader.hpp"
#include <iostream>

void Shader::Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource) {
	GLuint vertex_shader, fragment_shader, geometry_shader;

	// Create and compile vertex shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexSource, NULL);
	glCompileShader(vertex_shader);
	checkCompileErrors(vertex_shader, ShaderType(VERTEX));

	// Create and compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentSource, NULL);
	glCompileShader(fragment_shader);
	checkCompileErrors(fragment_shader, ShaderType(FRAGMENT));

	// if geometry shader source code is given, also compile geometry shader
	if (geometrySource != nullptr) {
		geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_shader, 1, &geometrySource, NULL);
		glCompileShader(geometry_shader);
		checkCompileErrors(geometry_shader, ShaderType(GEOMETRY));
	}

	// attach shaders to program and link it
	glAttachShader(this->ID_, vertex_shader);
	glAttachShader(this->ID_, fragment_shader);
	if (geometrySource != nullptr)
		glAttachShader(this->ID_, geometry_shader);
	glLinkProgram(this->ID_);
	checkCompileErrors(this->ID_, ShaderType(PROGRAM));

	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	if (geometrySource != nullptr)
		glDeleteShader(geometry_shader);
}

void Shader::checkCompileErrors(GLuint object, ShaderType type) {
	int success;
	char infoLog[1024];
	if (type != ShaderType(PROGRAM)) {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
					  << infoLog << "\n -- --------------------------------------------------- -- "
					  << std::endl;
		}
	} else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
					  << infoLog << "\n -- --------------------------------------------------- -- "
					  << std::endl;
		}
	}
}

void Shader::SetFloat(const char *name, float value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->ID_, name), value);
}
void Shader::SetInteger(const char *name, int value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->ID_, name), value);
}
void Shader::SetVector2f(const char *name, float x, float y, bool useShader) {
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID_, name), x, y);
}
void Shader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID_, name), value.x, value.y);
}
void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader) {
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID_, name), x, y, z);
}
void Shader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID_, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader) {
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID_, name), x, y, z, w);
}
void Shader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID_, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader) {
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID_, name), 1, false, glm::value_ptr(matrix));
}
