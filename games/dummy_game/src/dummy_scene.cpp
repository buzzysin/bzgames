#include "dummy_scene.h"

#include "core/logger.h"
#include "graphics/gl_mesh.h"
#include "graphics/gl_shader_program.h"

#include <GL/glew.h>

using namespace bz::engine::graphics;

DummyScene::DummyScene() {
	std::array vertices = {
		-0.5f, 0.5f,  0.0f, //
		-0.5f, -0.5f, 0.0f, //
		0.5f,  -0.5f, 0.0f, //
		0.5f,  0.5f,  0.0f, //
	};

	std::array colours = {
		0.5f, 0.0f, 0.0f, //
		0.0f, 0.5f, 0.0f, //
		0.0f, 0.0f, 0.5f, //
		0.0f, 0.5f, 0.5f, //
	};

	std::array indices = {
		0, 1, 3, //
		3, 1, 2, //
	};

	// Add some mesh
	auto tryMesh = GLMesh::create({
		.vertices = {vertices.begin(), vertices.end()},
		.colours = {colours.begin(), colours.end()},
		.indices = {indices.begin(), indices.end()},
	});

	if (tryMesh.hasError()) {
		bzLog() << "Failed to create mesh: " << tryMesh.error().what();
		return;
	}

	auto mesh = std::move(*tryMesh);
	this->addMesh("square", std::move(mesh));

	bzLog() << "DummyScene created";
}