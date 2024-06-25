#pragma once

#include <core/error.h>
#include <core/macros.h>
#include <core/pimpl.h>
#include <core/result.h>

#include "graphics/window.h"
#include "graphics/window_data.h"

namespace bz::engine::graphics {

namespace errors {
struct GLWindowError : bz::core::Error {
	GLWindowError() = default;
	GLWindowError(const char *what) : Error(what) {}
	GLWindowError(const std::string_view &what) : Error(what) {}
	GLWindowError(const std::string &what) : Error(what) {}
};
struct GLFWInitFailed : public GLWindowError {
	GLFWInitFailed() : GLWindowError("GLFW initialization failed") {}
};
struct GLFWCreateFailed : public GLWindowError {
	GLFWCreateFailed() : GLWindowError("GLFW window creation failed") {}
};
struct GLEWInitFailed : public GLWindowError {
	GLEWInitFailed() : GLWindowError("GLEW initialization failed") {}
};
} // namespace errors

class GLWindow : public Window {

public:
	GLWindow();
	~GLWindow() override;

	GLWindow(const GLWindow &) = delete;
	GLWindow &operator=(const GLWindow &) = delete;

	GLWindow(GLWindow &&) = default;
	GLWindow &operator=(GLWindow &&) = default;

private:
	BZ_PIMPL(GLWindow)
public:
	GLWindow(GLWindowPrivate &&pImpl) noexcept;

public:
	static core::Result<std::unique_ptr<GLWindow>, errors::GLWindowError>
	create(const WindowData &windowData);

public:
	void open() override;
	[[nodiscard]] bool isOpen() const override;

	void shouldClose() override;
	void cancelClose() override;
	[[nodiscard]] bool isClosing() const override;

	void hide() override;
	[[nodiscard]] bool isHidden() const override;

	void restore() override;
	[[nodiscard]] bool isRestored() const override;

	void maximise() override;
	[[nodiscard]] bool isMaximised() const override;

	void fullscreen() override;
	[[nodiscard]] bool isFullscreen() const override;

public:
	void onResize(std::function<void()> onResize) override;

public:
	void swapBuffers() override;
	void pollEvents() override;
};

} // namespace bz::engine::graphics