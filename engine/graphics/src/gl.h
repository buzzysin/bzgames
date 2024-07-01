// Ensures the proper include order for glew.h and the rest of the OpenGL
// headers.

#pragma once

// IWYU pragma: begin_exports

// IWYU pragma: no_include <GL/glew.h>
// IWYU pragma: no_include <GL/glu.h>
// IWYU pragma: no_include <GLFW/glfw3.h>

// First
#include <GL/glew.h>
// Then
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

// It's probably bad practice to include header that I don't necessarily need,
// but the sensitivity of the order of these includes is important.

#include <string>

inline std::string logProgram(int programId) {
	std::string log;

	log += "\n";
	log += "Program " + std::to_string(programId) + ":\n";

	// Link status
	GLint linkStatus;
	glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
	log += "  Link status: " + std::to_string(linkStatus) + "\n";

	// Delete status
	GLint deleteStatus;
	glGetProgramiv(programId, GL_DELETE_STATUS, &deleteStatus);
	log += "  Delete status: " + std::to_string(deleteStatus) + "\n";

	// Validate status
	GLint validateStatus;
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &validateStatus);
	log += "  Validate status: " + std::to_string(validateStatus) + "\n";

	// Attached shaders
	GLint attachedShaders;
	glGetProgramiv(programId, GL_ATTACHED_SHADERS, &attachedShaders);
	log += "  Attached shaders: " + std::to_string(attachedShaders) + "\n";

	// Log length
	GLint logLength;
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
	log += "  Log length: " + std::to_string(logLength) + "\n";

	// Log
	if (logLength > 0) {
		std::string logStr(logLength, '\0');
		glGetProgramInfoLog(programId, logLength, nullptr, logStr.data());
		log += "  Log: " + logStr + "\n";
	}

	// Binary length
	GLint binaryLength;
	glGetProgramiv(programId, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
	log += "  Binary length: " + std::to_string(binaryLength) + "\n";

	return log;
}

inline std::string logShader(int shaderId, GLenum shaderType) {
	std::string log;

	log += "\n";
	log += "Shader " + std::to_string(shaderId) + ":\n";

	// Shader type
	GLint type;
	glGetShaderiv(shaderId, GL_SHADER_TYPE, &type);
	log += "  Type: " + std::to_string(type) + "\n";

	// Compile status
	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
	log += "  Compile status: " + std::to_string(compileStatus) + "\n";

	// Source length
	GLint sourceLength;
	glGetShaderiv(shaderId, GL_SHADER_SOURCE_LENGTH, &sourceLength);
	log += "  Source length: " + std::to_string(sourceLength) + "\n";

	// Delete status
	GLint deleteStatus;
	glGetShaderiv(shaderId, GL_DELETE_STATUS, &deleteStatus);
	log += "  Delete status: " + std::to_string(deleteStatus) + "\n";

	// Log length
	GLint logLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
	log += "  Log length: " + std::to_string(logLength) + "\n";

	// Log
	if (logLength > 0) {
		std::string logStr(logLength, '\0');
		glGetShaderInfoLog(shaderId, logLength, nullptr, logStr.data());
		log += "  Log: " + logStr + "\n";
	}

	return log;
}

inline std::string logVertexArray(int vertexArrayId) {
	std::string log;

	log += "\n";
	log += "Vertex array " + std::to_string(vertexArrayId) + ":\n";

	// I want to know if its enabled, its size and its type.
	// I also want to know if it's bound to a buffer.

	// Size
	GLint size;
	glGetVertexArrayiv(vertexArrayId, GL_VERTEX_ARRAY_SIZE, &size);
	log += "  Size: " + std::to_string(size) + "\n";

	// Type
	GLint type;
	glGetVertexArrayiv(vertexArrayId, GL_VERTEX_ARRAY_TYPE, &type);
	log += "  Type: " + std::to_string(type) + "\n";

	// Enabled
	GLint enabled;
	glGetVertexArrayiv(vertexArrayId, GL_VERTEX_ARRAY, &enabled);
	log += "  Enabled: " + std::to_string(enabled) + "\n";

	return log;	
}

inline std::string logBuffer(int bufferId) {
	std::string log;

	log += "\n";
	log += "Buffer " + std::to_string(bufferId) + ":\n";


	return log;
}

// IWYU pragma: end_exports