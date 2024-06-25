#pragma once

#include "graphics/mesh.h"
#include "graphics/scene.h"
#include "graphics/window.h"

namespace bz::engine::graphics {

class Renderer {
public:
	Renderer() = default;
	virtual ~Renderer() = default;

	Renderer(const Renderer &) = delete;
	Renderer &operator=(const Renderer &) = delete;

	Renderer(Renderer &&) = default;
	Renderer &operator=(Renderer &&) = default;

public:
	[[deprecated]] virtual void render(Mesh *mesh) = 0;
	virtual void render(Window *window, Scene *scene) = 0;
};

} // namespace bz::engine::graphics