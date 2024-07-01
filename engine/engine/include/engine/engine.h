#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include <core/error.h>
#include <graphics/gl_window.h>
#include <graphics/window_data.h>

#include "engine_data.h"
#include "game.h"
#include "graphics/renderer.h"

namespace bz::engine::errors {

class EngineError : public bz::core::Error {
public:
	EngineError(const errors::GLWindowError &error) : Error{error.what()} {}
};

} // namespace bz::engine::errors

namespace bz::engine {

class Engine {
public:
	Engine();
	~Engine();

	Engine(const Engine &) = delete;
	Engine &operator=(const Engine &) = delete;

	Engine(Engine &&) = default;
	Engine &operator=(Engine &&) = default;

	// Creation
public:
	static bz::core::Result<std::unique_ptr<Engine>, errors::EngineError>
	create(const EngineData &data, std::unique_ptr<Game> game);

	// Engine api
public:
	void run();
	void setScene(std::unique_ptr<graphics::Scene> scene);
	void setRenderer(std::unique_ptr<graphics::Renderer> renderer);

	// Private api
private:
	void _run();

private:
	EngineData _data{};
	std::unique_ptr<graphics::Window> _window{nullptr};
	std::unique_ptr<graphics::Renderer> _renderer{nullptr};
	std::unique_ptr<graphics::Scene> _scene{nullptr};
	std::unique_ptr<Game> _game{nullptr};
	std::thread _thread;
	std::atomic_bool _running{false};
};

} // namespace bz::engine