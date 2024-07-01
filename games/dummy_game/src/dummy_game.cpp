#include "dummy_game.h"

#include "core/logger.h"
#include "dummy_renderer.h"
#include "dummy_scene.h"

#include "engine/engine.h"
#include <core/result.h>
#include <memory>

using namespace bz::core;
using namespace bz::engine;
using namespace bz::engine::common;
using namespace bz::engine::graphics;

DummyGame::DummyGame() = default;

DummyGame::~DummyGame() = default;

Result<std::unique_ptr<DummyGame>, errors::GameError> DummyGame::create() {
	return std::make_unique<DummyGame>();
}

void DummyGame::startup(Engine *engine, Window *window, Scene *scene,
                        Renderer *renderer) {

	engine->setScene(std::make_unique<DummyScene>());
	engine->setRenderer(std::make_unique<DummyRenderer>());

	window->onResize([]() { bzTrace() << "TODO: Resize event"; });

	window->onKeyInput([window](const KeyInput &keyInput) {
		bzTrace() << "Key: " << static_cast<int>(keyInput.key())
				  << " Action: " << static_cast<int>(keyInput.action());

		// TODO: Create a "ControlScheme" class that maps keys to actions

		if (keyInput.key() == Key::Escape) {
			window->shouldClose();
		}

		if (keyInput.key() == Key::F11) {
			if (window->isFullscreen()) {
				window->restore();
			} else {
				window->fullscreen();
			}
		}
	});

	window->open();
}

void DummyGame::input(Window *window, Scene *scene, bool inputHandled) {}
