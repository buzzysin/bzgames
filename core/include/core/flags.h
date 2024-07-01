#pragma once

#include <unordered_set>

namespace bz::core {

// For an enum formatted with bit flags, this class will be used to store the
// flags.
template <typename T>
class Flags {
public:
	constexpr Flags() {}
	constexpr Flags(const T &data) : _data(static_cast<std::size_t>(data)) {}

public:
	constexpr void set(const T &flag) {
		_data |= static_cast<std::size_t>(flag);
	}
	constexpr void unset(const T &flag) {
		_data &= ~static_cast<std::size_t>(flag);
	}
	constexpr void toggle(const T &flag) {
		_data ^= static_cast<std::size_t>(flag);
	}
	constexpr bool has(const T &flag) const {
		return _data & static_cast<std::size_t>(flag);
	}

	constexpr T data() const { return _data; }

public:
	friend constexpr Flags &operator|=(Flags &lhs, const T &rhs) {
		lhs.set(rhs);
		return lhs;
	}

	friend constexpr Flags operator|(const Flags &lhs, const T &rhs) {
		Flags result = lhs;
		return result |= rhs;
	}

	friend constexpr Flags &operator&=(Flags &lhs, const T &rhs) {
		lhs.unset(rhs);
		return lhs;
	}

	friend constexpr Flags operator&(const Flags &lhs, const T &rhs) {
		Flags result = lhs;
		return result &= rhs;
	}

	friend constexpr Flags &operator^=(Flags &lhs, const T &rhs) {
		lhs.toggle(rhs);
		return lhs;
	}

	friend constexpr Flags operator^(const Flags &lhs, const T &rhs) {
		Flags result = lhs;
		return result ^= rhs;
	}

private:
	std::size_t _data{};
};

// This class is different from Flags in that it will maintain an unordered
// set of arbitrary values. It is a more general purpose class that only works
// with std::hash'able types.
template <typename T>
class AutoFlags {
public:
	constexpr AutoFlags() = default;
	constexpr AutoFlags(const T &data)
		: _data({static_cast<std::size_t>(data)}) {}

public:
	constexpr void set(const T &flag) {
		_data.insert(flag);
	}
	constexpr void unset(const T &flag) { _data.erase(flag); }
	constexpr void toggle(const T &flag) {
		if (has(flag)) {
			unset(flag);
		} else {
			set(flag);
		}
	}
	constexpr bool has(const T &flag) const {
		return _data.find(flag) != _data.end();
	}

public:
	friend constexpr AutoFlags &operator|=(AutoFlags &lhs, const T &rhs) {
		lhs.set(rhs);
		return lhs;
	}

	friend constexpr AutoFlags operator|(const AutoFlags &lhs, const T &rhs) {
		AutoFlags result = lhs;
		return result |= rhs;
	}

	friend constexpr AutoFlags &operator&=(AutoFlags &lhs, const T &rhs) {
		lhs.unset(rhs);
		return lhs;
	}

	friend constexpr AutoFlags operator&(const AutoFlags &lhs, const T &rhs) {
		AutoFlags result = lhs;
		return result &= rhs;
	}

	friend constexpr AutoFlags &operator^=(AutoFlags &lhs, const T &rhs) {
		lhs.toggle(rhs);
		return lhs;
	}

	friend constexpr AutoFlags operator^(const AutoFlags &lhs, const T &rhs) {
		AutoFlags result = lhs;
		return result ^= rhs;
	}

private:
	std::unordered_set<T> _data;
};

// TestFlags2
enum class TestFlags2 { test1, test2, test3 };

constexpr Flags<TestFlags2> test_(TestFlags2::test1);

} // namespace bz::core