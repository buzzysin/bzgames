#pragma once

#include "graphics/window_data.h"
#include <common/input/key_input.h>
#include <core/result.h>
#include <functional>

namespace bz::engine::graphics {

class Window {
public:
	Window() = default;
	virtual ~Window() = default;

	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;

	Window(Window &&) = default;
	Window &operator=(Window &&) = default;

public:
	virtual const WindowData &data() const = 0;

	virtual void open() = 0;
	[[nodiscard]] virtual bool isOpen() const = 0;

	virtual void shouldClose() = 0;
	virtual void cancelClose() = 0;
	[[nodiscard]] virtual bool isClosing() const = 0;

	virtual void maximise() = 0;
	[[nodiscard]] virtual bool isMaximised() const = 0;

	virtual void restore() = 0;
	[[nodiscard]] virtual bool isRestored() const = 0;

	virtual void fullscreen() = 0;
	[[nodiscard]] virtual bool isFullscreen() const = 0;

	virtual void hide() = 0;
	[[nodiscard]] virtual bool isHidden() const = 0;

	virtual void onResize(std::function<void()> onResize) = 0;
	virtual void
	onKeyInput(std::function<void(const common::KeyInput &)> onKeyInput) = 0;

public:
	virtual void swapBuffers() = 0;
	virtual void pollEvents() = 0;
};

} // namespace bz::engine::graphics