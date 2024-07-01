#include "graphics/gl_mesh.h"

#include <core/error.h>
#include <cstring>

#include "core/logger.h"
#include "gl.h"
#include "graphics/gl_vertex_array.h"
#include "graphics/mesh_data.h"

using namespace bz::core;
using namespace bz::engine::errors;

namespace bz::engine::graphics {

GLMesh::GLMesh(std::size_t vertexCount, GLVertexArray vao,
               std::vector<GLBuffer> vbos)
	: _vertexCount{vertexCount}, _vao{std::move(vao)}, _vbos{std::move(vbos)} {
	bzTrace() << "Created mesh with " << vertexCount << " vertices";
}

GLMesh::GLMesh(GLMesh &&other) noexcept
	: _vertexCount{std::exchange(other._vertexCount, 0)},
	  _vao{std::move(other._vao)}, _vbos{std::move(other._vbos)} {}

GLMesh &GLMesh::operator=(GLMesh &&other) noexcept {
	_vertexCount = std::exchange(other._vertexCount, 0);
	_vao = std::move(other._vao);
	_vbos = std::move(other._vbos);
	return *this;
}

GLMesh::~GLMesh() {
	if (_vertexCount == 0) {
		return;
	}

	bzTrace() << "Destroyed mesh with " << _vertexCount << " vertices";
}

core::Result<std::unique_ptr<GLMesh>, errors::GLMeshError>
GLMesh::create(const MeshData &data) {

	auto tryVao = GLVertexArray::create();
	if (tryVao.hasError()) {
		return GLMeshError{tryVao.error()};
	}
	auto vao = std::move(*tryVao);

	// Vertex buffer
	auto tryVertexVbo = GLBuffer::create(GL_ARRAY_BUFFER);
	if (tryVertexVbo.hasError()) {
		return GLMeshError{tryVertexVbo.error()};
	}
	auto vertexVbo = std::move(*tryVertexVbo);

	// Colour buffer
	auto tryColourVbo = GLBuffer::create(GL_ARRAY_BUFFER);
	if (tryColourVbo.hasError()) {
		return GLMeshError{tryColourVbo.error()};
	}
	auto colourVbo = std::move(*tryColourVbo);

	// Index buffer
	auto tryIndexVbo = GLBuffer::create(GL_ELEMENT_ARRAY_BUFFER);
	if (tryIndexVbo.hasError()) {
		return GLMeshError{tryIndexVbo.error()};
	}
	auto indexVbo = std::move(*tryIndexVbo);

	{ // Bind the vertex array
		GLVertexArrayCtx vaoCtx{vao};
		{ // Bind the vertex buffer
			vaoCtx.add(vertexVbo);

			// Define the vertex buffer data
			glBufferData(
				GL_ARRAY_BUFFER,
				static_cast<GLsizeiptr>(data.vertices.size() * sizeof(float)),
				data.vertices.data(), GL_STATIC_DRAW);

			// Vertices are at location 0
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			bzTrace() << "Wrote " << data.vertices.size() * sizeof(float)
					  << " bytes to vertex buffer";
		}

		{ // Bind the colour buffer
			vaoCtx.add(colourVbo);

			// Define the colour buffer data
			glBufferData(
				GL_ARRAY_BUFFER,
				static_cast<GLsizeiptr>(data.colours.size() * sizeof(float)),
				data.colours.data(), GL_STATIC_DRAW);

			// Colours are at location 1
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			bzTrace() << "Wrote " << data.colours.size() * sizeof(float)
					  << " bytes to colour buffer";
		}

		{ // Bind the index buffer
			vaoCtx.add(indexVbo);

			// Define the index buffer data
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			             static_cast<GLsizeiptr>(data.indices.size() *
			                                     sizeof(unsigned int)),
			             data.indices.data(), GL_STATIC_DRAW);

			bzTrace() << "Wrote " << data.indices.size() * sizeof(unsigned int)
					  << " bytes to index buffer";
		}
	} // Context is closed, unbinding all buffers implicitly

	std::vector<GLBuffer> vbos;
	vbos.push_back(std::move(vertexVbo));
	vbos.push_back(std::move(colourVbo));
	vbos.push_back(std::move(indexVbo));

	// const auto vertexCount = data.vertices.size() / 3;
	const auto vertexCount = data.indices.size();

	return std::make_unique<GLMesh>(
		std::move(GLMesh(vertexCount, std::move(vao), std::move(vbos))));
}

GLVertexArray &GLMesh::getVertexArray() { return _vao; }

std::vector<GLBuffer> &GLMesh::getVertexBuffers() { return _vbos; }

std::size_t GLMesh::getVertexCount() const { return _vertexCount; }

} // namespace bz::engine::graphics