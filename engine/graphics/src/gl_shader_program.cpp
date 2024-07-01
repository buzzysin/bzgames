#include "graphics/gl_shader_program.h"

#include "core/logger.h"
#include "gl.h"

#include <GL/gl.h>
#include <array>
#include <cassert>
#include <fstream>
#include <sstream>

using namespace bz::core;
using namespace bz::engine::errors;

namespace bz::engine::graphics {

GLShaderProgram::GLShaderProgram(int programId) : _programId(programId) {
	bzTrace() << "Created shader program " << _programId << ":"
			  << logProgram(_programId);

	assert(glIsProgram(_programId) && "Invalid program id");
}

GLShaderProgram::GLShaderProgram(GLShaderProgram &&other) noexcept {
	_programId = std::exchange(other._programId, 0);
}

GLShaderProgram &GLShaderProgram::operator=(GLShaderProgram &&other) noexcept {
	_programId = std::exchange(other._programId, 0);
	return *this;
}

GLShaderProgram::~GLShaderProgram() {
	if (_programId == 0) {
		return;
	}

	glDeleteProgram(_programId);
	bzTrace() << "Deleted shader progam: " << _programId;
}

Result<std::unique_ptr<GLShaderProgram>, GLShaderError>
GLShaderProgram::create(const ShaderData &shaderData) {

	// Create the shader modules
	std::vector<int> modules;

	for (const auto &shaderModule : shaderData.modules) {
		auto tryShader = _createShader(shaderModule.type, shaderModule.path);

		if (tryShader.hasError()) {
			return tryShader.error();
		}

		modules.push_back(*tryShader);
	}

	// Create the program
	int programId = glCreateProgram();
	if (programId == 0) { // Definitely checked: returns 0 on failure
		return GLShaderCreateProgramFailed{};
	}

	// Link the shaders
	auto tryLink = _linkShader(programId, modules);
	if (tryLink.hasError()) {
		return tryLink.error();
	}

	// Return the shader
	return std::make_unique<GLShaderProgram>(programId);
}

int GLShaderProgram::id() const { return _programId; }

void GLShaderProgram::bind() {
	bzTrace() << "Using shader: " << _programId; // noisy
	glUseProgram(_programId);
}

void GLShaderProgram::unbind() {
	bzTrace() << "Disusing shader: " << _programId; // noisy
	glUseProgram(0);
}

Result<int, GLShaderError>
GLShaderProgram::_createShader(int shaderType, const std::string &shaderPath) {
	// Load the shader file
	std::ifstream shaderFile(shaderPath);
	if (!shaderFile.is_open()) {
		return GLShaderFileNotFound{shaderPath.c_str()};
	}

	std::string shaderSource;
	{
		std::stringstream stream;
		stream << shaderFile.rdbuf();
		shaderSource = stream.str();
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

	int success{};
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		std::array<char, 512> infoLog{};
		glGetShaderInfoLog(shaderId, 512, nullptr, infoLog.data());
		return GLShaderCompileFailed{shaderPath.c_str(), infoLog.data()};
	}

	bzTrace() << logShader(shaderId, shaderType);

	// Return the shader id
	return shaderId;
}

Fallible<GLShaderError>
GLShaderProgram::_linkShader(int programId, const std::vector<int> &shaders) {
	for (const auto &id : shaders) {
		glAttachShader(programId, id);
		bzTrace() << "Attached shader: " << id << " to program: " << programId;
	}

	// Link the program
	glLinkProgram(programId);
	int success{};
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		std::array<char, 512> infoLog{};
		glGetProgramInfoLog(programId, 512, nullptr, infoLog.data());
		return GLShaderLinkFailed{};
	}
	bzTrace() << "Linked shader program: " << programId;

	// Validate the program
	glValidateProgram(programId);

	glGetProgramiv(programId, GL_VALIDATE_STATUS, &success);
	if (success != GL_TRUE) {
		std::array<char, 512> infoLog{};
		glGetProgramInfoLog(programId, 512, nullptr, infoLog.data());
		return GLShaderValidateFailed{};
	}
	bzTrace() << "Validated shader program: " << programId;

	// Unlink the shaders
	for (const auto &shaderId : shaders) {
		glDetachShader(programId, shaderId);
		glDeleteShader(shaderId);
		bzTrace() << "Detached and deleted shader: " << shaderId;
	}

	return Empty{};
}

} // namespace bz::engine::graphics