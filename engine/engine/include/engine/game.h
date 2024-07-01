#pragma once

#include "graphics/renderer.h"
#include "graphics/scene.h"

#include <core/result.h>

#include <memory>

namespace bz::engine::errors {
struct GameError {};
} // namespace bz::engine::errors

namespace bz::engine {
class Engine;
}

namespace bz::engine {

class Game {
public:
	Game() = default;
	virtual ~Game() = default;

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	Game(Game &&) = default;
	Game &operator=(Game &&) = default;

public:
	virtual void startup(engine::Engine *, graphics::Window *window,
	                     graphics::Scene *scene,
	                     graphics::Renderer *renderer) = 0;

	virtual void input(graphics::Window *window, graphics::Scene *scene,
	                   bool inputHandled) = 0;
};

} // namespace bz::engine