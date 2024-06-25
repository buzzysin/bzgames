#pragma once

namespace bz::engine::graphics {

struct WindowData {
	int width;
	int height;
	const char *title;
	bool resizable;
	bool fullscreen;
	bool maximsed;
	int fps;
	bool vsync;
};

} // namespace bz::engine::graphics