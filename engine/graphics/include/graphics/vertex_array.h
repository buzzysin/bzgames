#pragma once

namespace bz::engine::graphics {

class VertexArray {
public:
	VertexArray() = default;
	virtual ~VertexArray() = default;

	VertexArray(const VertexArray &) = delete;
	VertexArray &operator=(const VertexArray &) = delete;

	VertexArray(VertexArray &&) = default;
	VertexArray &operator=(VertexArray &&) = default;
};

} // namespace bz::engine::graphics