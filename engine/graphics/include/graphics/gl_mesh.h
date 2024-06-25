#pragma once

#include "graphics/gl_vertex_array.h"
#include "graphics/gl_vertex_buffer.h"
#include "graphics/mesh.h"

namespace bz::engine::graphics {

namespace errors {
struct GLMeshError : core::Error {
	GLMeshError(const GLVertexArrayError &error) : Error{error.what()} {}
	GLMeshError(const GLVertexBufferError &error) : Error{error.what()} {}
};
} // namespace errors

class GLMesh : public Mesh {
public:
	~GLMesh() override;

	GLMesh(const GLMesh &) = delete;
	GLMesh &operator=(const GLMesh &) = delete;

	GLMesh(GLMesh &&) = default;
	GLMesh &operator=(GLMesh &&) = default;

private:
	GLMesh(std::size_t vertexCount, GLVertexArray vao, GLVertexBuffer vbo);

public:
	static core::Result<GLMesh, errors::GLMeshError>
	create(const std::vector<float> &vertices);

public:
	GLVertexArray &getVertexArray();
	std::size_t getVertexCount() const override;

private:
	std::size_t _vertexCount;
	GLVertexArray _vao;
	GLVertexBuffer _vbo;
};

} // namespace bz::engine::graphics