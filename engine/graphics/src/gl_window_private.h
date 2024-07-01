#pragma once

#include "graphics/gl_window.h"
#include "graphics/window_data.h"

#include "gl.h"

#include <atomic>
#include <mutex>

namespace bz::engine::graphics {

struct GLFWwindowDeleter {
	void operator()(GLFWwindow *window) const { glfwDestroyWindow(window); }
};
using GLFWWindowPtr = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>;

class GLWindow::GLWindowPrivate {
public:
	GLWindowPrivate();
	GLWindowPrivate(const WindowData &windowData);
	~GLWindowPrivate();

	GLWindowPrivate(const GLWindowPrivate &) = delete;
	GLWindowPrivate &operator=(const GLWindowPrivate &) = delete;

	GLWindowPrivate(GLWindowPrivate &&) noexcept = default;
	GLWindowPrivate &operator=(GLWindowPrivate &&) noexcept = default;

public:
	static core::Result<GLWindowPrivate, errors::GLWindowError>
	create(const WindowData &windowData);

	// Developer API
public:
	const WindowData &data() const;
	
	void open();
	[[nodiscard]] bool isOpen() const;

	void hide();
	[[nodiscard]] bool isHidden() const;

	void shouldClose();
	void cancelClose();
	[[nodiscard]] bool isClosing() const;

	void maximise();
	[[nodiscard]] bool isMaximised() const;

	void restore();
	[[nodiscard]] bool isRestored() const;

	void fullscreen();
	[[nodiscard]] bool isFullscreen() const;

	void onResize(std::function<void()> onResize);
	void onKeyInput(std::function<void(const common::KeyInput &)> onKeyInput);

	// Engine API
public:
	void swapBuffers();
	void pollEvents();

	// Callbacks
private:
	static void _keyCallback(GLFWwindow *window, int key, int scancode,
	                         int action, int mods);
	static void _resizeCallback(GLFWwindow *window, int width, int height);

	void _keyCallback(int key, int scancode, int action, int mods);
	void _resizeCallback(int width, int height);

private:
	std::uint8_t _id{0};
	WindowData _data{};

	static inline std::atomic_uint _instanceCount = ATOMIC_VAR_INIT(0);

	GLFWWindowPtr _window{nullptr};

	std::function<void(const common::KeyInput &)> _onKey;
	std::function<void()> _onResize;
};

} // namespace bz::engine::graphics