#pragma once

#include "core/result.h"
#include "graphics/vertex_buffer.h"

#include <core/error.h>
#include <functional>
#include <vector>

namespace bz::engine::errors {
struct GLVertexBufferError : public core::Error {
	using Error::Error;
};

struct GLVertexBufferCreateError : public GLVertexBufferError {
	GLVertexBufferCreateError()
		: GLVertexBufferError{"Failed to create GLVertexBuffer"} {}
};
} // namespace bz::engine::errors

namespace bz::engine::graphics {

class GLBuffer : public VertexBuffer {
public:
	~GLBuffer() override;

	GLBuffer(const GLBuffer &) = delete;
	GLBuffer &operator=(const GLBuffer &) = delete;

	GLBuffer(GLBuffer &&) noexcept;
	GLBuffer &operator=(GLBuffer &&) noexcept;

private:
	GLBuffer(unsigned int id, unsigned int type);

public:
	static core::Result<GLBuffer, errors::GLVertexBufferError>
	create(unsigned int type);

	unsigned int id() const;
	unsigned int type() const;

public:
	void bind();
	void unbind();

private:
	unsigned int _id{0};
	unsigned int _type{0};
};

class GLBufferCtx {
public:
	GLBufferCtx(GLBuffer &buffer) : _buffer{buffer} { _buffer.bind(); }
	~GLBufferCtx() { _buffer.unbind(); }

	GLBufferCtx(const GLBufferCtx &) = delete;
	GLBufferCtx &operator=(const GLBufferCtx &) = delete;

	GLBufferCtx(GLBufferCtx &&) = delete;
	GLBufferCtx &operator=(GLBufferCtx &&) = delete;

private:
	GLBuffer &_buffer;
};

} // namespace bz::engine::graphics
