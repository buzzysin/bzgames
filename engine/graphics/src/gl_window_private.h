#pragma once

#include "graphics/gl_window.h"
#include "graphics/window_data.h"

#include "gl.h"

#include <atomic>

namespace bz::engine::graphics {

class GLWindow::GLWindowPrivate {
public:
	GLWindowPrivate();
	GLWindowPrivate(const WindowData &windowData);
	~GLWindowPrivate();

	GLWindowPrivate(const GLWindowPrivate &) = delete;
	GLWindowPrivate &operator=(const GLWindowPrivate &) = delete;

	GLWindowPrivate(GLWindowPrivate &&) = default;
	GLWindowPrivate &operator=(GLWindowPrivate &&) = default;

public:
	static core::Result<GLWindowPrivate, errors::GLWindowError>
	create(const WindowData &windowData);

public:
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

public:
	void onResize(std::function<void()> onResize);
	void update();
	void pollEvents();
	bool shouldClose() const;

private:
	static void _keyCallback(GLFWwindow *window, int key, int scancode,
	                         int action, int mods);
	void _keyCallback(int key, int scancode, int action, int mods);

	static void _resizeCallback(GLFWwindow *window, int width, int height);
	void _resizeCallback(int width, int height);

private:
	using GLFWWindowPtr =
		std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;

	std::uint8_t _id{0};
	WindowData _data{};

	static inline std::atomic_uint _instanceCount = ATOMIC_VAR_INIT(0);
	static inline std::unordered_map<uint, void *> _instances;
	GLFWWindowPtr _window{nullptr, glfwDestroyWindow};

	std::function<void()> _onKey;
	std::function<void()> _onResize;
};

} // namespace bz::engine::graphics