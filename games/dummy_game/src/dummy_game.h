#pragma once

#include <core/result.h>
#include <engine/game.h>

#include <memory>

class DummyGame : public bz::engine::Game {
public:
	DummyGame();
	virtual ~DummyGame();

	DummyGame(const DummyGame &) = delete;
	DummyGame &operator=(const DummyGame &) = delete;

	DummyGame(DummyGame &&) = default;
	DummyGame &operator=(DummyGame &&) = default;

public:
	static bz::core::Result<std::unique_ptr<DummyGame>,
	                        bz::engine::errors::GameError>
	create();

public:
	void startup(bz::engine::Engine *engine,
	             bz::engine::graphics::Window *window,
	             bz::engine::graphics::Scene *scene,
	             bz::engine::graphics::Renderer *renderer) override;

	void input(bz::engine::graphics::Window *window,
	           bz::engine::graphics::Scene *scene, bool inputHandled) override;
};