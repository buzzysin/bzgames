#pragma once

#include "graphics/gl_renderer.h"

class DummyRenderer : public bz::engine::graphics::GLRenderer {
public:
	DummyRenderer();
	~DummyRenderer() override = default;

	DummyRenderer(const DummyRenderer &) = delete;
	DummyRenderer &operator=(const DummyRenderer &) = delete;

	DummyRenderer(DummyRenderer &&) = default;
	DummyRenderer &operator=(DummyRenderer &&) = default;
};