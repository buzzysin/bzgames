#pragma once

#include <core/flags.h>

namespace bz::engine::common {

enum class Key {
	Space = 32,
	// Alphabetic keys
	A = 65,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	// Functional keys
	Escape = 256,
	Enter,
	Tab,
	Backspace,
	Insert,
	Delete,
	Right,
	Left,
	Down,
	Up,
	PageUp,
	PageDown,
	Home,
	End,
	// Lock keys
	CapsLock = 280,
	ScrollLock,
	NumLock,
	PrintScreen,
	Pause,
	// Function keys
	F1 = 290,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12z

};

enum class KeyModifier {
	Ctrl,
	Alt,
	Shift,
};
using KeyModifiers = core::AutoFlags<KeyModifier>;

enum class Action {
	Press,
	Release,
	Repeat,
};

} // namespace bz::engine::common
