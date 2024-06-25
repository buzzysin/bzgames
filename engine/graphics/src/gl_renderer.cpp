#include "graphics/gl_renderer.h"

#include "core/macros.h"
#include "gl.h"
#include "graphics/gl_mesh.h"

#include <cassert>

namespace bz::engine::graphics {

void GLRenderer::render(Mesh *mesh) {
	auto *glMesh = dynamic_cast<GLMesh *>(mesh);
	assert(glMesh != nullptr && "Unexpected non-GL mesh for GLRenderer");

	// Clear the screen
	clear();
}

void GLRenderer::render(Window *window, BZ_UNUSED Scene *scene) {
	_window = window;

	// Create capabilities?
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Clear the screen
	clear();

	// Handle resizing

	// Swap the buffers
	_window->swapBuffers();
}

void GLRenderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

} // namespace bz::engine::graphics