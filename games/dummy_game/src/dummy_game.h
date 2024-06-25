#pragma once

#include <common/game.h>
#include <core/result.h>

#include <memory>

class DummyGame : public bz::engine::common::Game {
public:
	DummyGame() = default;
	virtual ~DummyGame() = default;

	DummyGame(const DummyGame &) = delete;
	DummyGame &operator=(const DummyGame &) = delete;

	DummyGame(DummyGame &&) = default;
	DummyGame &operator=(DummyGame &&) = default;

public:
	static bz::core::Result<std::unique_ptr<DummyGame>,
	                        bz::engine::common::errors::GameError>
	create();
};