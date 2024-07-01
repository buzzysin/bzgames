#include "engine/engine.h"

#include "core/logger.h"
#include "engine/engine_data.h"
#include <graphics/gl_window.h>
#include <iostream>

using namespace bz::core;
using namespace bz::engine::errors;
using namespace bz::engine::graphics;

namespace bz::engine {

Engine::Engine() = default;

Engine::~Engine() {
	_running = false;
	if (_thread.joinable()) {
		_thread.join();
	}
}

Result<std::unique_ptr<Engine>, EngineError>
Engine::create(const EngineData &data, std::unique_ptr<Game> game) {
	// Make the window
	auto tryWindow = GLWindow::create(data.window);

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

void Engine::run() {
	_game->startup(this, _window.get(), _scene.get(), _renderer.get());

	_running = true;
	// _thread = std::thread(&Engine::_run, this);
	// _thread.join();

	while (_running) {
		_window->pollEvents();

		auto then = std::chrono::high_resolution_clock::now();
		_game->input(_window.get(), _scene.get(), false);
		_renderer->render(_window.get(), _scene.get());
		auto now = std::chrono::high_resolution_clock::now();

		if (_window->isClosing()) {
			_running = false;
		}

		// Print fps and cls
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - then);
		auto fps = 1000.0 / duration.count();
		system("clear");
		std::cout << "FPS: " << fps << std::endl;
	}
}

void Engine::setScene(std::unique_ptr<Scene> scene) {
	_scene = std::move(scene);
}

void Engine::setRenderer(std::unique_ptr<Renderer> renderer) {
	_renderer = std::move(renderer);
}

void Engine::_run() {
	// TODO: Implement the engine loop
}

} // namespace bz::engine