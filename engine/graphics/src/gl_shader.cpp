#include "graphics/gl_shader.h"

#include "gl.h"

#include <fstream>
#include <sstream>

using namespace bz::core;
using namespace bz::engine::graphics::errors;

namespace bz::engine::graphics {

GLShader::GLShader(int programId, int vertexShaderId, int fragmentShaderId)
	: _programId(programId), _vertexShaderId(vertexShaderId),
	  _fragmentShaderId(fragmentShaderId) {}

GLShader::~GLShader() {
	// Free the shaders
	glDeleteShader(_vertexShaderId);
	glDeleteShader(_fragmentShaderId);

	// Free the program
	if (_programId != 0) {
		glDeleteProgram(_programId);
	}
}

Result<GLShader, GLShaderError> GLShader::create(const ShaderData &shaderData) {
	// Create the program
	int programId = glCreateProgram();
	if (programId == 0) {
		return GLShaderCreateProgramFailed{};
	}

	// Create the vertex shader
	auto tryVertexShader =
		_createShader(programId, GL_VERTEX_SHADER, shaderData.vertexShaderPath);
	if (tryVertexShader.hasError()) {
		return tryVertexShader.error();
	}

	int vertexShaderId = tryVertexShader.value();

	// Create the fragment shader
	auto tryFragmentShader = _createShader(programId, GL_FRAGMENT_SHADER,
	                                       shaderData.fragmentShaderPath);
	if (tryFragmentShader.hasError()) {
		return tryFragmentShader.error();
	}

	int fragmentShaderId = tryFragmentShader.value();

	// Link the shader
	auto tryLink = _linkShader(programId, vertexShaderId, fragmentShaderId);
	if (tryLink.hasError()) {
		return tryLink.error();
	}

	// Return the shader
	return GLShader{programId, vertexShaderId, fragmentShaderId};
}

void GLShader::bind() { glUseProgram(_programId); }

void GLShader::unbind() { glUseProgram(0); }

Result<int, GLShaderError>
GLShader::_createShader(int programId, int shaderType,
                        const std::string &shaderPath) {
	// Load the shader file
	std::ifstream shaderFile(shaderPath);
	if (!shaderFile.is_open()) {
		return GLShaderFileNotFound{shaderPath.c_str()};
	}

	std::string shaderSource;
	{
		std::stringstream ss;
		ss << shaderFile.rdbuf();
		shaderSource = ss.str();
	}

	const char *shaderSourceCStr = shaderSource.c_str();

	// Create the shader
	int shaderId = glCreateShader(shaderType);
	if (shaderId == 0) {
		return GLShaderCreateShaderFailed{shaderPath.c_str()};
	}

	// Compile the shader
	glShaderSource(shaderId, 1, &shaderSourceCStr, nullptr);
	glCompileShader(shaderId);

	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
		return GLShaderCompileFailed{shaderPath.c_str(), infoLog};
	}

	// Attach the shader to the program
	glAttachShader(programId, shaderId);

	// Return the shader id
	return shaderId;
}

Fallible<GLShaderError> GLShader::_linkShader(int programId, int vertexShaderId,
                                              int fragmentShaderId) {

	// Link the program
	glLinkProgram(programId);

	int success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(programId, 512, nullptr, infoLog);
		return GLShaderLinkFailed{};
	}

	// Validate the program
	glValidateProgram(programId);

	glGetProgramiv(programId, GL_VALIDATE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(programId, 512, nullptr, infoLog);
		return GLShaderValidateFailed{};
	}

	// Unlink the shaders
	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	return Empty{};
}

} // namespace bz::engine::graphics