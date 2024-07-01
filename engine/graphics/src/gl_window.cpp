#include "graphics/gl_window.h"

#include "core/logger.h"
#include "core/pimpl.h"
#include "gl_window_private.h"

using namespace bz::core;
using namespace bz::engine::errors;

namespace bz::engine::graphics {

GLWindow::GLWindow() { bzLog() << "GLWindow created"; }

GLWindow::~GLWindow() { bzLog() << "GLWindow destroyed"; }

GLWindow::GLWindow(GLWindowPrivate &&pImpl) noexcept
	: _pImpl(std::move(pImpl)) {
	bzLog() << "GLWindow created";
}

Result<std::unique_ptr<GLWindow>, errors::GLWindowError>
GLWindow::create(const WindowData &windowData) {
	// Creation of the window can fail, so we need to return a Result
	auto tryImpl = GLWindowPrivate::create(windowData);
	if (tryImpl.hasError()) {
		return tryImpl.error();
	}

	auto window = std::make_unique<GLWindow>(std::move(*tryImpl));
	return window;
}

const WindowData &GLWindow::data() const { return _pImpl->data(); }

void GLWindow::open() { _pImpl->open(); }
bool GLWindow::isOpen() const { return _pImpl->isOpen(); }

void GLWindow::shouldClose() { _pImpl->shouldClose(); }
void GLWindow::cancelClose() { _pImpl->cancelClose(); }
bool GLWindow::isClosing() const { return _pImpl->isClosing(); }

void GLWindow::hide() { _pImpl->hide(); }
bool GLWindow::isHidden() const { return _pImpl->isHidden(); }

void GLWindow::restore() { _pImpl->restore(); }
bool GLWindow::isRestored() const { return _pImpl->isRestored(); }

void GLWindow::maximise() { _pImpl->maximise(); }
bool GLWindow::isMaximised() const { return _pImpl->isMaximised(); }

void GLWindow::fullscreen() { return _pImpl->fullscreen(); }
bool GLWindow::isFullscreen() const { return _pImpl->isFullscreen(); }

void GLWindow::onResize(std::function<void()> onResize) {
	_pImpl->onResize(std::move(onResize));
}

void GLWindow::onKeyInput(
	std::function<void(const common::KeyInput &)> onKeyInput) {
	_pImpl->onKeyInput(std::move(onKeyInput));
}

void GLWindow::swapBuffers() { _pImpl->swapBuffers(); }

void GLWindow::pollEvents() { _pImpl->pollEvents(); }

} // namespace bz::engine::graphics
