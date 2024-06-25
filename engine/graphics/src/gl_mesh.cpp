#include "graphics/gl_mesh.h"

#include <core/error.h>

#include "gl.h"
#include "graphics/gl_vertex_array.h"

using namespace bz::core;
using namespace bz::engine::graphics::errors;

namespace bz::engine::graphics {

GLMesh::~GLMesh() {}

GLMesh::GLMesh(std::size_t vertexCount, GLVertexArray vao, GLVertexBuffer vbo)
	: _vertexCount{vertexCount}, _vao{std::move(vao)}, _vbo{std::move(vbo)} {}

core::Result<GLMesh, errors::GLMeshError>
GLMesh::create(const std::vector<float> &vertices) {

	auto tryVao = GLVertexArray::create();
	if (tryVao.hasError()) {
		return GLMeshError{tryVao.error()};
	}
	auto vao = std::move(tryVao.value());

	auto tryVbo = GLVertexBuffer::create();
	if (tryVbo.hasError()) {
		return GLMeshError{tryVbo.error()};
	}
	auto vbo = std::move(tryVbo.value());

	{
		// Bind the vertex array
		GLVertexArrayCtx vaoCtx{vao};
		{
			// Bind the vertex buffer
			GLVertexBufferCtx vboCtx{vbo};

			// Send the vertices to the GPU
			glBufferData(
				GL_ARRAY_BUFFER,
				static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
				vertices.data(), GL_STATIC_DRAW);

			// Set the vertex attribute pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			                      nullptr);
		}
	}

	const auto vertexCount = vertices.size() / 3;
	return GLMesh{vertexCount, std::move(vao), std::move(vbo)};
}

GLVertexArray &GLMesh::getVertexArray() { return _vao; }

std::size_t GLMesh::getVertexCount() const { return _vertexCount; }

} // namespace bz::engine::graphics