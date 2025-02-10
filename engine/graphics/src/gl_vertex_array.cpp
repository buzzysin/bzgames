#include "bz/graphics/gl_vertex_array.h"

#include "bz/core/logger.h"
#include "gl.h"

using namespace bz::core;
using namespace bz::engine::errors;

namespace bz::engine::graphics {

GLVertexArray::GLVertexArray(unsigned int id) : _id(id) {
	// bzTrace() << "GLVertexArray " << id << " created";
}

GLVertexArray::GLVertexArray(GLVertexArray &&other) noexcept
	: _id(std::exchange(other._id, 0)), _buffers(std::move(other._buffers)){};

GLVertexArray &GLVertexArray::operator=(GLVertexArray &&other) noexcept {
	// Move the id and buffers
	_id = std::exchange(other._id, 0);
	_buffers = std::move(other._buffers);

	return *this;
}

GLVertexArray::~GLVertexArray() {
	if (_id == 0) {
		return;
	}

	glDeleteVertexArrays(1, &_id);
	bzTrace() << "GLVertexArray " << _id << " deleted";
}

void GLVertexArray::bind() {
	glBindVertexArray(_id);
	_bound = true;
	bzTrace() << "GLVertexArray " << _id << " bound";
}

void GLVertexArray::unbind() {
	glBindVertexArray(0);
	_bound = false;
	bzTrace() << "GLVertexArray " << _id << " unbound";
}

Result<GLVertexArray, GLVertexArrayError> GLVertexArray::create() {
	unsigned int id{};
	glGenVertexArrays(1, &id);

	// It turns out that the glGenVertexArrays function can't fail, so we don't
	// need to check the id

	// if (id == 0) {
	// return GLVertexArrayGeneralError{};
	// }

	return GLVertexArray{id};
}

int GLVertexArray::id() const { return _id; }

core::Result<GLBuffer *, errors::GLVertexBufferError>
GLVertexArray::addBuffer(unsigned int type) {
	bool _dynamicBinding = !_bound;

	if (_dynamicBinding) {
		bind();
	}

	auto tryBuffer = GLBuffer::create(type);
	if (tryBuffer.hasError()) {
		return tryBuffer.error();
	}

	_buffers.push_back(std::move(*tryBuffer));

	if (_dynamicBinding) {
		unbind();
	}

	return &_buffers.back();
}
} // namespace bz::engine::graphics