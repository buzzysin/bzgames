#pragma once

#include "graphics/gl_scene.h"

class DummyScene : public bz::engine::graphics::GLScene {
public:
	DummyScene();
	~DummyScene() override = default;

	DummyScene(const DummyScene &) = delete;
	DummyScene &operator=(const DummyScene &) = delete;

	DummyScene(DummyScene &&) = default;
	DummyScene &operator=(DummyScene &&) = default;

public:
};