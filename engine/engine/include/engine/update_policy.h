#pragma once

#include "game.h"
#include "graphics/scene.h"
#include "graphics/window.h"

namespace bz::engine {

class UpdatePolicy {
public:
	UpdatePolicy() = default;
	virtual ~UpdatePolicy() = default;

	UpdatePolicy(const UpdatePolicy &) = delete;
	UpdatePolicy &operator=(const UpdatePolicy &) = delete;

	UpdatePolicy(UpdatePolicy &&) = default;
	UpdatePolicy &operator=(UpdatePolicy &&) = default;

public:
	virtual void operator()(bool running, graphics::Window *window,
	                        graphics::Scene *scene, Game *game);
};

class VSyncUpdatePolicy : public UpdatePolicy {
public:
	void operator()(bool running, graphics::Window *window,
	                graphics::Scene *scene, Game *game) override;
};

class FPSUpdatePolicy : public UpdatePolicy {
public:
	FPSUpdatePolicy(unsigned int fps) : _fps{fps} {}

	void operator()(bool running, graphics::Window *window,
	                graphics::Scene *scene, Game *game) override;

private:
	static constexpr const unsigned int _defaultFps{60};

private:
	unsigned int _fps{_defaultFps};
};

} // namespace bz::engine