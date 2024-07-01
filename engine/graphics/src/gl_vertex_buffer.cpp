#include "graphics/gl_vertex_buffer.h"

#include "core/logger.h"
#include "gl.h"

using namespace bz::engine::errors;
using namespace bz::core;

namespace bz::engine::graphics {

GLBuffer::GLBuffer(unsigned int id, unsigned int type) : _id{id}, _type{type} {
	bzTrace() << "GLVertexBuffer " << id << " created, type: " << type;
}

GLBuffer::GLBuffer(GLBuffer &&other) noexcept {
	// Move the id
	_id = std::exchange(other._id, 0);
	_type = std::exchange(other._type, 0);
};

GLBuffer &GLBuffer::operator=(GLBuffer &&other) noexcept {
	// Move the id
	_id = std::exchange(other._id, 0);
	_type = std::exchange(other._type, 0);
	return *this;
}

GLBuffer::~GLBuffer() {
	if (_id == 0) {
		return;
	}

	// Delete the buffer
	glDeleteBuffers(1, &_id);
	bzTrace() << "GLVertexBuffer " << _id << " deleted";
}

Result<GLBuffer, GLVertexBufferError> GLBuffer::create(unsigned int type) {
	unsigned int id{0};
	glGenBuffers(1, &id);

	// This operation is guaranteed to succeed, so we don't need to check the id
	// if (id == 0) {
	// return GLVertexBufferCreateError{};
	// }

	return GLBuffer{id, type};
}

void GLBuffer::bind() {
	glBindBuffer(_type, _id);
	bzTrace() << "GLVertexBuffer " << _id << " bound, type: " << _type;
}

void GLBuffer::unbind() {
	glBindBuffer(_type, 0);
	bzTrace() << "GLVertexBuffer " << _id << " unbound, type: " << _type;
}

unsigned int GLBuffer::id() const { return _id; }

} // namespace bz::engine::graphics