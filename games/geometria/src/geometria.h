#pragma once

#include <common/game.h>
#include <core/result.h>

#include <memory>

class Geometria : public bz::engine::common::Game {
public:
	Geometria() = default;
	~Geometria() override = default;

	Geometria(const Geometria &) = delete;
	Geometria &operator=(const Geometria &) = delete;

	Geometria(Geometria &&) = default;
	Geometria &operator=(Geometria &&) = default;

public:
	static bz::core::Result<std::unique_ptr<Geometria>,
	                        bz::engine::common::errors::GameError>
	create();
};