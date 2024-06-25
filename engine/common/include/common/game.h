#pragma once

#include <core/result.h>
#include <memory>

namespace bz::engine::common {

namespace errors {
struct GameError {};

} // namespace errors

class Game {
public:
	Game() = default;
	virtual ~Game() = default;

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	Game(Game &&) = default;
	Game &operator=(Game &&) = default;

public:
	
};

} // namespace bz::engine::common