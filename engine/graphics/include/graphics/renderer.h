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

	// Developer interface
public:
	void setShader(std::unique_ptr<Shader> shader);
	Shader *shader();

	// Engine interface
public:
	virtual void render(Window *window, Scene *scene) = 0;

private:
	std::unique_ptr<Shader> _shader{nullptr};
};

} // namespace bz::engine::graphics