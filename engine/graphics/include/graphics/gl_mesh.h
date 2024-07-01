#pragma once

#include "graphics/gl_vertex_array.h"
#include "graphics/gl_vertex_buffer.h"
#include "graphics/mesh.h"
#include "graphics/mesh_data.h"
#include <memory>

namespace bz::engine::errors {
struct GLMeshError : core::Error {
	GLMeshError(const GLVertexArrayError &error) : Error{error.what()} {}
	GLMeshError(const GLVertexBufferError &error) : Error{error.what()} {}
};

} // namespace bz::engine::errors

namespace bz::engine::graphics {

class GLMesh : public Mesh {
public:
	GLMesh(std::size_t vertexCount, GLVertexArray vao,
	       std::vector<GLBuffer> vbos);
	~GLMesh() override;

	GLMesh(const GLMesh &) = delete;
	GLMesh &operator=(const GLMesh &) = delete;

	GLMesh(GLMesh &&) noexcept;
	GLMesh &operator=(GLMesh &&) noexcept;

public:
	static core::Result<std::unique_ptr<GLMesh>, errors::GLMeshError>
	create(const MeshData &data);

public:
	GLVertexArray &getVertexArray();
	[[nodiscard]] std::size_t getVertexCount() const override;

	std::vector<GLBuffer> &getVertexBuffers();

private:
	std::size_t _vertexCount;
	GLVertexArray _vao;          // vertex array object
	std::vector<GLBuffer> _vbos; // vertex buffer objects
};

} // namespace bz::engine::graphics