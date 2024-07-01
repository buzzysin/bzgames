#include "graphics/gl_renderer.h"

#include "gl.h"

#include "core/logger.h"
#include "core/macros.h"
#include "graphics/gl_mesh.h"
#include "graphics/gl_scene.h"
#include "graphics/gl_shader_program.h"
#include "graphics/gl_vertex_array.h"
#include "graphics/gl_window.h"
#include <cstring>

namespace bz::engine::graphics {

void GLRenderer::render(Window *window, BZ_UNUSED Scene *scene) {
	auto *shader = this->shader();
	if (shader == nullptr) {
		bzError() << "no shader set";
		return;
	}

	auto *glWindow = dynamic_cast<GLWindow *>(window);
	if (glWindow == nullptr) {
		bzError() << "window is not a GLWindow";
		return;
	}

	auto *glScene = dynamic_cast<GLScene *>(scene);
	if (glScene == nullptr) {
		bzError() << "scene is not a GLScene";
		return;
	}

	auto *glShaderProgram = dynamic_cast<GLShaderProgram *>(shader);
	if (glShaderProgram == nullptr) {
		bzError() << "shader is not a GLShader";
		return;
	}

	// Clear the screen, resize the viewport and so on
	reset(glWindow);

	// Convert to GLMeshes
	auto meshes = glScene->meshes();
	std::vector<GLMesh *> glMeshes;
	glMeshes.reserve(meshes.size());
	for (const auto &mesh : meshes) {
		auto *glMesh = dynamic_cast<GLMesh *>(mesh.get());
		if (glMesh == nullptr) {
			bzTrace() << "GLScene::render: mesh is not a GLMesh";
			continue;
		}

		glMeshes.push_back(glMesh);
	}

	GLShaderProgramCtx ctx(*glShaderProgram);
	bzTrace() << "Using shader program: " << logProgram(glShaderProgram->id());

	enum AttribArray { Vertices, Colours };
	for (const auto &glMesh : glMeshes) {
		GLVertexArrayCtx vaoCtx(glMesh->getVertexArray());
		bzTrace() << "Enabling vertex attributes";

		// Enable the vertex attributes
		glEnableVertexAttribArray(AttribArray::Vertices);
		glEnableVertexAttribArray(AttribArray::Colours);

		glDrawElements(GL_TRIANGLES, static_cast<int>(glMesh->getVertexCount()),
		               GL_UNSIGNED_INT, nullptr);

		glDisableVertexAttribArray(AttribArray::Vertices);
		glDisableVertexAttribArray(AttribArray::Colours);

		bzTrace() << "Disabling vertex attributes";
	}

	glWindow->swapBuffers();
}

void GLRenderer::reset(Window *window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->data().width, window->data().height);
}

} // namespace bz::engine::graphics