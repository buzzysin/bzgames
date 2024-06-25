#pragma once

#include "graphics/vertex_array.h"
#include <core/error.h>
#include <core/result.h>

namespace bz::engine::graphics {

namespace errors {
struct GLVertexArrayError : public core::Error {
	using Error::Error;
};

struct GLVertexArrayGeneralError : public GLVertexArrayError {
	GLVertexArrayGeneralError()
		: GLVertexArrayError("Failed to create GLVertexArray") {}
};
} // namespace errors

class GLVertexArray : public VertexArray {
public:
	~GLVertexArray();

	GLVertexArray(const GLVertexArray &) = delete;
	GLVertexArray &operator=(const GLVertexArray &) = delete;

	GLVertexArray(GLVertexArray &&) = default;
	GLVertexArray &operator=(GLVertexArray &&) = default;

private:
	GLVertexArray(unsigned int id);

public:
	static core::Result<GLVertexArray, errors::GLVertexArrayError> create();

public:
	void bind();
	void unbind();

	int id() const;

private:
	unsigned int _id{0};
};

class GLVertexArrayCtx {
public:
	GLVertexArrayCtx(GLVertexArray &vao) : _vao{vao} { _vao.bind(); }
	~GLVertexArrayCtx() { _vao.unbind(); }

	GLVertexArrayCtx(const GLVertexArrayCtx &) = delete;
	GLVertexArrayCtx &operator=(const GLVertexArrayCtx &) = delete;

	GLVertexArrayCtx(GLVertexArrayCtx &&) = delete;
	GLVertexArrayCtx &operator=(GLVertexArrayCtx &&) = delete;

private:
	GLVertexArray &_vao;
};

} // namespace bz::engine::graphics