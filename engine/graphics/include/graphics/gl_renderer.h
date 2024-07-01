#pragma once

#include "graphics/gl_shader_program.h"
#include "graphics/renderer.h"
#include "graphics/scene.h"

#include <core/error.h>
#include <core/result.h>

#include <memory>

namespace bz::engine::errors {
struct RendererError : core::Error {};
} // namespace bz::engine::errors

namespace bz::engine::graphics {

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
	void render(Window *window, Scene *scene) override;

private:
	void reset(Window *window);
};

} // namespace bz::engine::graphics