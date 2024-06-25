#include "graphics/gl_vertex_buffer.h"

#include "gl.h"

namespace bz::engine::graphics {

GLVertexBuffer::GLVertexBuffer(unsigned int id) : _id(id) {}

core::Result<GLVertexBuffer, errors::GLVertexBufferError>
GLVertexBuffer::create() {
	unsigned int id{0};
	glGenBuffers(1, &id);
	if (id == 0) {
		return errors::GLVertexBufferGeneralError{};
	}

	return GLVertexBuffer{id};
}

GLVertexBuffer::~GLVertexBuffer() {
	// Delete the buffer
	glDeleteBuffers(1, &_id);
}

void GLVertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, _id); }

void GLVertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

unsigned int GLVertexBuffer::id() const { return _id; }

} // namespace bz::engine::graphics