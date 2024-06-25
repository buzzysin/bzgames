#pragma once

#include <core/error.h>

namespace bz::engine::graphics {


class Shader {
public:
	Shader() = default;
	virtual ~Shader() = default;

	Shader(const Shader &) = delete;
	Shader &operator=(const Shader &) = delete;

	Shader(Shader &&) = default;
	Shader &operator=(Shader &&) = default;

public:
	virtual void bind() = 0;
	virtual void unbind() = 0;
};

} // namespace bz::engine::graphics