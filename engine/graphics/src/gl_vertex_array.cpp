#include "graphics/gl_vertex_array.h"

#include "core/logger.h"
#include "gl.h"

using namespace bz::core;
using namespace bz::engine::errors;

namespace bz::engine::graphics {

GLVertexArray::GLVertexArray(unsigned int id) : _id(id) {
	// bzTrace() << "GLVertexArray " << id << " created";
}

GLVertexArray::GLVertexArray(GLVertexArray &&other) noexcept {
	// Move the id
	_id = std::exchange(other._id, 0);
};

GLVertexArray &GLVertexArray::operator=(GLVertexArray &&other) noexcept {
	// Move the id
	_id = std::exchange(other._id, 0);
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
	bzTrace() << "GLVertexArray " << _id << " bound";
}

void GLVertexArray::unbind() {
	glBindVertexArray(0);
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

} // namespace bz::engine::graphics