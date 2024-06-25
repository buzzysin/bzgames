#include "engine/engine.h"

#include "engine/engine_data.h"
#include <graphics/gl_window.h>

using namespace bz::core;
using namespace bz::engine::common;
using namespace bz::engine::engine::errors;

namespace bz::engine::engine {

Engine::Engine() = default;

Engine::~Engine() {
	if (_thread.joinable()) {
		_thread.join();
	}
}

Result<std::unique_ptr<Engine>, errors::EngineError>
Engine::create(const EngineData &data, std::unique_ptr<Game> game) {
	// Make the window
	auto tryWindow = graphics::GLWindow::create(data.window);

	if (tryWindow.hasError()) {
		return tryWindow.error();
	}

	// Create the engine
	auto engine = std::make_unique<Engine>();
	engine->_data = data;
	engine->_window = std::move(tryWindow.value());
	engine->_game = std::move(game);

	return engine;
}

void Engine::run() { _thread = std::thread(&Engine::_run, this); }

void Engine::_run() {
	// TODO: Implement the engine loop
}

} // namespace bz::engine::engine