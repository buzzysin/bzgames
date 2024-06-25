#pragma once

namespace bz::engine::graphics {

class VertexBuffer {
public:
	VertexBuffer() = default;
	virtual ~VertexBuffer() = default;

	VertexBuffer(const VertexBuffer &) = delete;
	VertexBuffer &operator=(const VertexBuffer &) = delete;

	VertexBuffer(VertexBuffer &&) = default;
	VertexBuffer &operator=(VertexBuffer &&) = default;
};

} // namespace bz::engine::graphics