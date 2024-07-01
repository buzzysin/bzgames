#pragma once

#include "key_defs.h"
#include <optional>

namespace bz::engine::common {

class KeyInput {
public:
	KeyInput(Key key, KeyModifiers modifiers, Action action);

	[[nodiscard]] Key key() const;
	[[nodiscard]] KeyModifiers modifiers() const;
	[[nodiscard]] Action action() const;

	// Shortcut method for checking if a key is pressed
	[[nodiscard]] bool pressed(const std::optional<Key> &key = std::nullopt) const;

	// Shortcut method for checking if a key is released
	[[nodiscard]] bool released(const std::optional<Key> &key = std::nullopt) const;

	// Shortcut method for checking if a key is held
	[[nodiscard]] bool held(const std::optional<Key> &key = std::nullopt) const;

private:
	Key _key;
	KeyModifiers _modifiers;
	Action _action;
};

} // namespace bz::engine::common