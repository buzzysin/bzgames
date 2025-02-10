#pragma once

// (T)hread (S)afe (V)alue.
// I want this to be similar or equivalent to Rust's Arc<Mutex<T>>.
// This is a simple wrapper around a value that is thread safe.

#include <mutex>
#include <type_traits>
#include <utility>

namespace bz::core {

template <typename T>
class TSV {

public:
	// Construct
	constexpr TSV() = delete;

	template <typename U,
	          std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
	explicit TSV(U &&value);

	// Copy
	TSV(const TSV &other);

	// Move
	TSV(TSV &&other) noexcept;

	TSV &operator=(const TSV &other);

	TSV &operator=(TSV &&other) noexcept;

	// Get
	T get() const;

	// Set
	template <typename U,
	          std::enable_if_t<std::is_convertible_v<U, T>, int> = 0>
	void set(U &&value);

private:
	T _value;
	mutable std::mutex _mutex;
};

} // namespace bz::core

// Implementation

namespace bz::core {

template <typename T>
template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int>>
TSV<T>::TSV(U &&value) : _value(std::forward<U>(value)) {}

template <typename T>
TSV<T>::TSV(const TSV &other) {
	std::lock_guard<std::mutex> lock(other._mutex);
	_value = other._value;
}

template <typename T>
TSV<T>::TSV(TSV &&other) noexcept {
	std::lock_guard<std::mutex> lock(other._mutex);
	_value = std::move(other._value);
}

template <typename T>
TSV<T> &TSV<T>::operator=(const TSV &other) {
	if (this == &other) {
		return *this;
	}

	std::lock(_mutex, other._mutex);
	std::lock_guard<std::mutex> selfLock(_mutex, std::adopt_lock);
	std::lock_guard<std::mutex> otherLock(other._mutex, std::adopt_lock);

	_value = other._value;

	return *this;
}

template <typename T>
TSV<T> &TSV<T>::operator=(TSV &&other) noexcept {
	if (this == &other) {
		return *this;
	}

	std::lock(_mutex, other._mutex);
	std::lock_guard<std::mutex> selfLock(_mutex, std::adopt_lock);
	std::lock_guard<std::mutex> otherLock(other._mutex, std::adopt_lock);

	_value = std::move(other._value);

	return *this;
}

template <typename T>
T TSV<T>::get() const {
	std::lock_guard<std::mutex> lock(_mutex);
	return _value;
}

template <typename T>
template <typename U, std::enable_if_t<std::is_convertible_v<U, T>, int>>
void TSV<T>::set(U &&value) {
	std::lock_guard<std::mutex> lock(_mutex);
	_value = std::forward<U>(value);
}

} // namespace bz::core