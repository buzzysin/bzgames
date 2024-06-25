#pragma once

#include "graphics/renderer.h"
#include "graphics/scene.h"

#include <core/error.h>
#include <core/result.h>

#include <memory>

namespace bz::engine::graphics {

namespace errors {
struct RendererError : core::Error {};

} // namespace errors

class GLRenderer : public Renderer {
public:
	GLRenderer() = default;
	virtual ~GLRenderer() = default;

	GLRenderer(const GLRenderer &) = delete;
	GLRenderer &operator=(const GLRenderer &) = delete;

	GLRenderer(GLRenderer &&) = default;
	GLRenderer &operator=(GLRenderer &&) = default;

public:
	static core::Result<std::unique_ptr<GLRenderer>, errors::RendererError>
	create();

public:
	[[deprecated]] void render(Mesh *mesh) override;
	void render(Window *window, Scene *scene) override;

private:
	void clear();

private:
	Window *_window{nullptr};
};

} // namespace bz::engine::graphics