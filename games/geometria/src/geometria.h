#pragma once

#include <bz/core/result.h>
#include <bz/engine/game.h>

#include <memory>

class Geometria : public bz::engine::Game {
public:
	Geometria() = default;
	~Geometria() override = default;

	Geometria(const Geometria &) = delete;
	Geometria &operator=(const Geometria &) = delete;

	Geometria(Geometria &&) = default;
	Geometria &operator=(Geometria &&) = default;

public:
	static bz::core::Result<std::unique_ptr<Geometria>,
	                        bz::engine::errors::GameError>
	create();

public:
	void startup(bz::engine::Engine *, bz::engine::graphics::Window *window,
	             bz::engine::graphics::Scene *scene,
	             bz::engine::graphics::Renderer *renderer) override;

	void input(bz::engine::graphics::Window *window,
	           bz::engine::graphics::Scene *scene, bool inputHandled) override;
};