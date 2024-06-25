#pragma once

#include <cstddef>
namespace bz::engine::graphics {

class Mesh {
public:
	Mesh() = default;
	virtual ~Mesh() = default;

	Mesh(const Mesh &) = delete;
	Mesh &operator=(const Mesh &) = delete;

	Mesh(Mesh &&) = default;
	Mesh &operator=(Mesh &&) = default;

public:
	virtual std::size_t getVertexCount() const = 0;
};

} // namespace bz::engine::graphics