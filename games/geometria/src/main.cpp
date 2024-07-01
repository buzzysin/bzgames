
#include <iostream>

#include <core/macros.h>
#include <engine/engine.h>

#include "geometria.h"

using namespace bz::engine;
using namespace bz::core;

int main(BZ_UNUSED int argc, BZ_UNUSED char *argv[]) {
	std::cout << "Hello, World!" << std::endl;

	auto engineData = EngineData{};
	engineData.window.title = "Hello, World!";
	engineData.window.width = 800;
	engineData.window.height = 600;

	auto game = std::make_unique<Geometria>();

	auto tryEngine = Engine::create(engineData, std::move(game));

	if (tryEngine.hasError()) {
		std::cerr << "Failed to create engine: " << tryEngine.error().what()
				  << std::endl;
		return 1;
	}

	auto engine = std::move(*tryEngine);

	engine->run();

	return 0;
}