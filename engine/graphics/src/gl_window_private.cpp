#include "gl_window_private.h"

#include <core/macros.h>

#include "GLFW/glfw3.h"
#include "common/input/key_defs.h"
#include "core/logger.h"
#include "gl.h"
#include "graphics/gl_window.h"
#include "graphics/window_data.h"

using namespace bz::engine::errors;

namespace bz::engine::graphics {

GLWindow::GLWindowPrivate::GLWindowPrivate() : GLWindowPrivate(WindowData{}) {}

GLWindow::GLWindowPrivate::GLWindowPrivate(const WindowData &windowData)
	: _data(windowData) {

	// Increment the instance count
	_instanceCount.fetch_add(1);
	bzTrace() << "Window created. Instance count: " << _instanceCount.load();
}

GLWindow::GLWindowPrivate::~GLWindowPrivate() {
	// If window is nullptr, then the pointer was moved
	if (!_window) {
		return;
	}

	// If we are the last instance, terminate GLFW
	if (_instanceCount.fetch_sub(1) == 1) {
		glfwTerminate();
		bzTrace() << "GLFW terminated";
	}

	bzTrace() << "Window destroyed. Instance count: " << _instanceCount.load();
}

core::Result<GLWindow::GLWindowPrivate, GLWindowError>
GLWindow::GLWindowPrivate::create(const WindowData &windowData) {
	/**
	 * Initialize GLFW
	 */

	if (const auto status = glfwInit(); status != GL_TRUE) {
		return GLFWInitFailed{};
	}

	// Start with default window hints
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// We want OpenGL 3.3 or higher
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	// Enable core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if BZ_PLATFORM_(MACOS)
	// OS X requires forward compatibility to be set
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a windowed mode window and its OpenGL context
	auto window = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(
		glfwCreateWindow(windowData.width, windowData.height, windowData.title,
	                     nullptr, nullptr));

	if (!window) {
		return GLFWCreateFailed{};
	}

	glfwMakeContextCurrent(window.get());

	/**
	 * Initialize GLEW
	 */

	// Needed for core profile
	glewExperimental = GL_TRUE;

	if (const auto status = glewInit(); status != GLEW_OK) {
		return GLEWInitFailed{};
	}

	auto impl = std::make_unique<GLWindowPrivate>(windowData);

	// Store the user pointer
	glfwSetWindowUserPointer(window.get(), impl.get());

	// Set callbacks
	glfwSetKeyCallback(window.get(), &GLWindowPrivate::_keyCallback);
	glfwSetFramebufferSizeCallback(window.get(),
	                               &GLWindowPrivate::_resizeCallback);

	// Move the window to the impl
	impl->_window = std::move(window);

	return std::move(*impl);
}

const WindowData &GLWindow::GLWindowPrivate::data() const { return _data; }

void GLWindow::GLWindowPrivate::open() { glfwShowWindow(_window.get()); }
bool GLWindow::GLWindowPrivate::isOpen() const {
	return glfwGetWindowAttrib(_window.get(), GLFW_VISIBLE) == GL_TRUE;
}

void GLWindow::GLWindowPrivate::hide() { glfwHideWindow(_window.get()); }
bool GLWindow::GLWindowPrivate::isHidden() const {
	return glfwGetWindowAttrib(_window.get(), GLFW_VISIBLE) == GL_FALSE;
}

void GLWindow::GLWindowPrivate::shouldClose() {
	glfwSetWindowShouldClose(_window.get(), GL_TRUE);
}
void GLWindow::GLWindowPrivate::cancelClose() {
	glfwSetWindowShouldClose(_window.get(), GL_FALSE);
}
bool GLWindow::GLWindowPrivate::isClosing() const {
	return glfwWindowShouldClose(_window.get()) == GL_TRUE;
}

void GLWindow::GLWindowPrivate::maximise() {
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
	glfwMaximizeWindow(_window.get());
}
bool GLWindow::GLWindowPrivate::isMaximised() const {
	return glfwGetWindowAttrib(_window.get(), GLFW_MAXIMIZED) == GL_TRUE;
}

void GLWindow::GLWindowPrivate::restore() {
	glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
	glfwRestoreWindow(_window.get());
}
bool GLWindow::GLWindowPrivate::isRestored() const {
	return glfwGetWindowAttrib(_window.get(), GLFW_MAXIMIZED) == GL_FALSE;
}

void GLWindow::GLWindowPrivate::fullscreen() {
	glfwWindowHint(GLFW_MAXIMIZED, GL_FALSE);
	glfwSetWindowMonitor(_window.get(), nullptr, 0, 0, _data.width,
	                     _data.height, GLFW_DONT_CARE);
}

bool GLWindow::GLWindowPrivate::isFullscreen() const {
	return glfwGetWindowMonitor(_window.get()) != nullptr;
}

void GLWindow::GLWindowPrivate::onResize(std::function<void()> onResize) {
	_onResize = std::move(onResize);
}

void GLWindow::GLWindowPrivate::onKeyInput(
	std::function<void(const common::KeyInput &)> onKeyInput) {
	_onKey = std::move(onKeyInput);
}

void GLWindow::GLWindowPrivate::swapBuffers() {
	glfwSwapBuffers(_window.get());
}

// NOLINTBEGIN(readability-convert-member-functions-to-static)
void GLWindow::GLWindowPrivate::pollEvents() { glfwPollEvents(); }
// NOLINTEND(readability-convert-member-functions-to-static)

void GLWindow::GLWindowPrivate::_keyCallback(GLFWwindow *window, int key,
                                             int scancode, int action,
                                             int mods) {
	auto *impl =
		static_cast<GLWindowPrivate *>(glfwGetWindowUserPointer(window));
	impl->_keyCallback(key, scancode, action, mods);
}

void GLWindow::GLWindowPrivate::_keyCallback(int key, int scancode, int action,
                                             int mods) {
	if (_onKey) {
		common::KeyModifiers keyMods;
		if ((mods & GLFW_MOD_SHIFT) != 0) {
			keyMods |= common::KeyModifier::Shift;
		}

		_onKey({static_cast<common::Key>(key),
		        {static_cast<common::KeyModifier>(mods)},
		        static_cast<common::Action>(action)});
	}
}

void GLWindow::GLWindowPrivate::_resizeCallback(GLFWwindow *window, int width,
                                                int height) {
	auto *impl =
		static_cast<GLWindowPrivate *>(glfwGetWindowUserPointer(window));
	impl->_resizeCallback(width, height);
}

void GLWindow::GLWindowPrivate::_resizeCallback(int width, int height) {
	_data.width = width;
	_data.height = height;

	if (_onResize) {
		_onResize();
	}
}

} // namespace bz::engine::graphics
