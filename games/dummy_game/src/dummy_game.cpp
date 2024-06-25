#include "dummy_game.h"

#include <core/result.h>
#include <memory>

using namespace bz::core;
using namespace bz::engine::common;

Result<std::unique_ptr<DummyGame>, errors::GameError> DummyGame::create() {
	return std::make_unique<DummyGame>();
}
