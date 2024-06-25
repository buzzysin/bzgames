#include "graphics/gl_vertex_array.h"

#include "gl.h"

using namespace bz::core;
using namespace bz::engine::graphics::errors;

namespace bz::engine::graphics {

GLVertexArray::~GLVertexArray() { glDeleteVertexArrays(1, &_id); }

void GLVertexArray::bind() { glBindVertexArray(_id); }

void GLVertexArray::unbind() { glBindVertexArray(0); }

Result<GLVertexArray, GLVertexArrayError> GLVertexArray::create() {
	unsigned int id{0};
	glGenVertexArrays(1, &id);
	if (id == 0) {
		return GLVertexArrayGeneralError{};
	}

	return GLVertexArray{id};
}

GLVertexArray::GLVertexArray(unsigned int id) : _id(id){};
int GLVertexArray::id() const { return _id; }
} // namespace bz::engine::graphics