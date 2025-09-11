
#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum ShaderType {
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	PROGRAM
};

// General purpose object for collection of shaders (program).
// Compiles from source code, generates compile/link-time error messages
// and hosts several utility functions for easy management.
class Shader {
   public:
	Shader() : ID_{glCreateProgram()} {}
	// will compile and link the shaders from given source code
	// geometry source code is optional
	void Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);
	// sets the current shaders as active
	void Use() { glUseProgram(this->ID_); }
	// utility functions
	const GLuint &ID() const { return ID_; }
	void SetFloat(const char *name, float value, bool useShader = false);
	void SetInteger(const char *name, int value, bool useShader = false);
	void SetVector2f(const char *name, float x, float y, bool useShader = false);
	void SetVector2f(const char *name, const glm::vec2 &value, bool useShader = false);
	void SetVector3f(const char *name, float x, float y, float z, bool useShader = false);
	void SetVector3f(const char *name, const glm::vec3 &value, bool useShader = false);
	void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader = false);
	void SetVector4f(const char *name, const glm::vec4 &value, bool useShader = false);
	void SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader = false);

   private:
	// program id
	GLuint ID_;
	// checks if compilation or linking failed and if so, print the error logs
	void checkCompileErrors(GLuint object, ShaderType type);
};

#endif
