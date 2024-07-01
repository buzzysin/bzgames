#include "dummy_renderer.h"

#include <GL/gl.h>

#include <core/logger.h>
#include <graphics/gl_shader_program.h>

using namespace bz::engine::graphics;

DummyRenderer::DummyRenderer() {
	// Create a shader
	auto tryShaderProgram = GLShaderProgram::create({{
		{GL_VERTEX_SHADER, "resources/shaders/basic/basic.vert"},
		{GL_FRAGMENT_SHADER, "resources/shaders/basic/basic.frag"},
	}});

	if (tryShaderProgram.hasError()) {
		bzLog() << "Failed to create shader: " << tryShaderProgram.error().what();
		return;
	}

	this->setShader(std::move(*tryShaderProgram));

	bzLog() << "DummyRenderer created";
}