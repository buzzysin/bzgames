#pragma once

#include "bz/graphics/gl_vertex_buffer.h"
#include "bz/graphics/vertex_array.h"
#include <bz/core/error.h>
#include <bz/core/result.h>
#include <functional>

namespace bz::engine::errors {
struct GLVertexArrayError : public core::Error {
	using Error::Error;
};

struct GLVertexArrayCreateError : public GLVertexArrayError {
	GLVertexArrayCreateError()
		: GLVertexArrayError("Failed to create GLVertexArray") {}
};
} // namespace bz::engine::errors

namespace bz::engine::graphics {

class GLVertexArray : public VertexArray {
private:
	GLVertexArray(unsigned int id);

public:
	~GLVertexArray();

	GLVertexArray(const GLVertexArray &) = delete;
	GLVertexArray &operator=(const GLVertexArray &) = delete;

	GLVertexArray(GLVertexArray &&) noexcept;
	GLVertexArray &operator=(GLVertexArray &&) noexcept;

public:
	static core::Result<GLVertexArray, errors::GLVertexArrayError> create();

	// Actions
public:
	void bind();
	void unbind();

	core::Result<GLBuffer *, errors::GLVertexBufferError>
	addBuffer(unsigned int type);

	// Accessors
public:
	int id() const;

	// Data
private:
	unsigned int _id{0};
	std::vector<GLBuffer> _buffers;

	// Status
private:
	bool _bound{false};
};

class GLVertexArrayCtx {
public:
	GLVertexArrayCtx(GLVertexArray &vao) : _vao{vao} { _vao.bind(); }
	~GLVertexArrayCtx() {
		_vao.unbind();
		for (auto &vbo : _vbos) {
			vbo.get().unbind();
		}
	}

	void add(GLBuffer &vbo) {
		vbo.bind();
		_vbos.emplace_back(vbo);
	}

	GLVertexArrayCtx(const GLVertexArrayCtx &) = delete;
	GLVertexArrayCtx &operator=(const GLVertexArrayCtx &) = delete;

	GLVertexArrayCtx(GLVertexArrayCtx &&) = delete;
	GLVertexArrayCtx &operator=(GLVertexArrayCtx &&) = delete;

private:
	GLVertexArray &_vao;
	std::vector<std::reference_wrapper<GLBuffer>> _vbos;
};

} // namespace bz::engine::graphics