#include "common/input/key_input.h"

namespace bz::engine::common {

KeyInput::KeyInput(Key key, KeyModifiers modifiers, Action action)
	: _key(key), _modifiers(std::move(modifiers)), _action(action) {}

Key KeyInput::key() const { return _key; }

KeyModifiers KeyInput::modifiers() const { return _modifiers; }

Action KeyInput::action() const { return _action; }

bool KeyInput::pressed(const std::optional<Key> &key) const {
	return key.value_or(_key) == _key && _action == Action::Press;
}

bool KeyInput::released(const std::optional<Key> &key) const {
	return key.value_or(_key) == _key && _action == Action::Release;
}

bool KeyInput::held(const std::optional<Key> &key) const {
	return key.value_or(_key) == _key && _action == Action::Repeat;
}

} // namespace bz::engine::common