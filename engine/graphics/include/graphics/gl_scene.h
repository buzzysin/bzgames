#pragma once

#include "graphics/scene.h"
namespace bz::engine::graphics {

class GLScene : public Scene {
public:
	GLScene() = default;
	virtual ~GLScene() = default;

	GLScene(const GLScene &) = delete;
	GLScene &operator=(const GLScene &) = delete;

	GLScene(GLScene &&) = default;
	GLScene &operator=(GLScene &&) = default;

	// Developer interface
public:
	// Engine interface
public:
	// void render(Window *window, Scene *scene) override;
};

} // namespace bz::engine::graphics