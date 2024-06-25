#pragma once

#include <memory>
#include <thread>

#include <common/game.h>
#include <core/error.h>
#include <graphics/gl_window.h>
#include <graphics/window_data.h>

#include "engine/update_policy.h"
#include "engine_data.h"

namespace bz::engine::engine {

namespace errors {
class EngineError : public bz::core::Error {
public:
	EngineError(const graphics::errors::GLWindowError &error)
		: Error{error.what()} {}
};
} // namespace errors

class Engine {
public:
	Engine();
	~Engine();

	Engine(const Engine &) = delete;
	Engine &operator=(const Engine &) = delete;

	Engine(Engine &&) = default;
	Engine &operator=(Engine &&) = default;

public:
	static bz::core::Result<std::unique_ptr<Engine>, errors::EngineError>
	create(const EngineData &data, std::unique_ptr<common::Game> game);

public:
	void run();

private:
	void _run();

private:
	EngineData _data;
	std::unique_ptr<graphics::Window> _window{nullptr};
	std::unique_ptr<common::Game> _game{nullptr};
	std::thread _thread;
};

} // namespace bz::engine::engine