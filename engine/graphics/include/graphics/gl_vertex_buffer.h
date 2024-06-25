#pragma once

#include "core/result.h"
#include "graphics/vertex_buffer.h"

#include <core/error.h>
#include <vector>

namespace bz::engine::graphics {

namespace errors {
struct GLVertexBufferError : public core::Error {
	using Error::Error;
};

struct GLVertexBufferGeneralError : public GLVertexBufferError {
	GLVertexBufferGeneralError()
		: GLVertexBufferError{"GLVertexBuffer general error"} {}
};
} // namespace errors

class GLVertexBuffer : public VertexBuffer {
public:
	~GLVertexBuffer() override;

	GLVertexBuffer(const GLVertexBuffer &) = delete;
	GLVertexBuffer &operator=(const GLVertexBuffer &) = delete;

	GLVertexBuffer(GLVertexBuffer &&) = default;
	GLVertexBuffer &operator=(GLVertexBuffer &&) = default;

private:
	GLVertexBuffer(unsigned int id);

public:
	static core::Result<GLVertexBuffer, errors::GLVertexBufferError> create();

	unsigned int id() const;

public:
	void bind();
	void unbind();

private:
	unsigned int _id{0};
};

class GLVertexBufferCtx {
public:
	GLVertexBufferCtx(GLVertexBuffer &vbo) : _vbo{vbo} { _vbo.bind(); }
	~GLVertexBufferCtx() { _vbo.unbind(); }

	GLVertexBufferCtx(const GLVertexBufferCtx &) = delete;
	GLVertexBufferCtx &operator=(const GLVertexBufferCtx &) = delete;

	GLVertexBufferCtx(GLVertexBufferCtx &&) = delete;
	GLVertexBufferCtx &operator=(GLVertexBufferCtx &&) = delete;

private:
	GLVertexBuffer &_vbo;
};

} // namespace bz::engine::graphics
