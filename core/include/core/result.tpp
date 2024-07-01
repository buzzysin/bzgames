#pragma once

// False positive:
// NOLINTBEGIN(bugprone-forwarding-reference-overload)

#include "result.h"

#include <stdexcept>
#include <type_traits>

namespace bz::core {

// Construct

template <typename T, typename E>
template <typename U, std::enable_if_t<IsResultVType<Result<T, E>, U>, int>>
constexpr Result<T, E>::Result(const U &value) : _value(value) {}

template <typename T, typename E>
template <typename U, std::enable_if_t<IsResultVType<Result<T, E>, U>, int>>
constexpr Result<T, E>::Result(U &&value) : _value(std::forward<U>(value)) {}

template <typename T, typename E>
template <typename F, std::enable_if_t<IsResultEType<Result<T, E>, F>, int>>
constexpr Result<T, E>::Result(const F &error) : _error(error) {}

template <typename T, typename E>
template <typename F, std::enable_if_t<IsResultEType<Result<T, E>, F>, int>>
constexpr Result<T, E>::Result(F &&error) : _error(std::forward<F>(error)) {}

// Copy

template <typename T, typename E>
template <typename U, typename F,
          std::enable_if_t<IsResultFullType<Result<T, E>, U, F>, int>>
constexpr Result<T, E>::Result(const Result<U, F> &other)
	: _value(other._value), _error(other._error) {}

// Move

template <typename T, typename E>
template <typename U, typename F,
          std::enable_if_t<IsResultFullType<Result<T, E>, U, F>, int>>
constexpr Result<T, E>::Result(Result<U, F> &&other) noexcept
	: _value(std::move(other._value)), _error(std::move(other._error)) {}

template <typename T, typename E>
template <typename U, typename F,
          std::enable_if_t<IsResultFullType<Result<T, E>, U, F>, int>>
constexpr Result<T, E> &Result<T, E>::operator=(Result<U, F> &&other) noexcept {
	_value = std::move(other._value);
	_error = std::move(other._error);
	return *this;
}

template <typename T, typename E>
template <typename U, std::enable_if_t<IsResultVType<Result<T, E>, U>, int>>
constexpr Result<T, E> &Result<T, E>::operator=(U &&value) {
	_value = std::forward<U>(value);
	_error = std::nullopt;
	return *this;
}

template <typename T, typename E>
template <typename F, std::enable_if_t<IsResultEType<Result<T, E>, F>, int>>
constexpr Result<T, E> &Result<T, E>::operator=(F &&error) {
	_error = std::forward<F>(error);
	_value = std::nullopt;
	return *this;
}

// Check/Get

template <typename T, typename E>
constexpr bool Result<T, E>::hasValue() const {
	return _value.has_value();
}
template <typename T, typename E>
constexpr bool Result<T, E>::hasError() const {
	return _error.has_value();
}

template <typename T, typename E>
constexpr const T &Result<T, E>::value() const & {
	return *_value;
}
template <typename T, typename E>
constexpr T &Result<T, E>::value() & {
	return *_value;
}

template <typename T, typename E>
constexpr const T &&Result<T, E>::value() const && {
	return std::move(*_value);
}
template <typename T, typename E>
constexpr T &&Result<T, E>::value() && {
	return std::move(*_value);
}

template <typename T, typename E>
constexpr const E &Result<T, E>::error() const & {
	return *_error;
}
template <typename T, typename E>
constexpr E &Result<T, E>::error() & {
	return *_error;
}

template <typename T, typename E>
constexpr const E &&Result<T, E>::error() const && {
	return std::move(*_error);
}
template <typename T, typename E>
constexpr E &&Result<T, E>::error() && {
	return std::move(*_error);
}

// Transformers

template <typename T, typename E>
template <typename U>
constexpr Result<U, E> Result<T, E>::and_(const Result<U, E> &other) const {
	if (hasValue()) {
		return other;
	}

	return value();
}

template <typename T, typename E>
template <typename U>
constexpr Result<U, E> Result<T, E>::and_(Result<U, E> &&other) const {
	if (hasValue()) {
		return std::move(other);
	}

	return value();
}

// Value Transformers

template <typename T, typename E>
template <
	typename F, typename... Args,
	std::enable_if_t<IsFreeTransformV<F, Result<T, E>, Pack<Args...>>, int>>
constexpr FreeTransformResultV<F, Result<T, E>, Args...>
Result<T, E>::andThen(F &&transform, Args &&...args) const {
	if (hasValue()) {
		return (std::forward<F>(transform))(value(),
		                                    std::forward<Args>(args)...);
	}

	return error();
}

template <typename T, typename E>
template <
	typename F, typename... Args,
	std::enable_if_t<IsFreeTransformV<F, Result<T, E>, Pack<Args...>>, int>>
constexpr FreeTransformResultV<F, Result<T, E>, Args...>
Result<T, E>::andThen(F &&transform, Args &&...args) {
	if (hasValue()) {
		return (std::forward<F>(transform))(std::forward<T>(value()),
		                                    std::forward<Args>(args)...);
	}

	return std::forward<E>(error());
}

template <typename T, typename E>
template <
	typename M, typename... Args,
	std::enable_if_t<IsBoundTransformV<M, Result<T, E>, Pack<Args...>>, int>>
constexpr BoundTransformResultV<M, Result<T, E>, Args...>
Result<T, E>::andThen(M &&transform, Args &&...args) const {
	if (hasValue()) {
		return (value().*
		        (std::forward<M>(transform)))(std::forward<Args>(args)...);
	}

	return error();
}

template <typename T, typename E>
template <typename C, typename M, typename... Args,
          std::enable_if_t<
			  IsContextTransformV<C, M, Result<T, E>, Pack<Args...>>, int>>
constexpr ContextTransformResultV<C, M, Result<T, E>, Args...>
Result<T, E>::andThen(C *&&context, M &&transform, Args &&...args) const {
	if (hasValue()) {
		return (std::forward<C *>(context)->*(std::forward<M>(transform)))(
			value(), std::forward<Args>(args)...);
	}

	return error();
}

//
// Error transformers
//

template <typename T, typename E>
template <
	typename F, typename... Args,
	std::enable_if_t<IsFreeTransformE<F, Result<T, E>, Pack<Args...>>, int>>
constexpr FreeTransformResultE<F, Result<T, E>, Args...>
Result<T, E>::orElse(F &&transform, Args &&...args) const {
	if (hasError()) {
		return (std::forward<F>(transform))(error(),
		                                    std::forward<Args>(args)...);
	}

	return value();
}

template <typename T, typename E>
template <
	typename M, typename... Args,
	std::enable_if_t<IsBoundTransformE<M, Result<T, E>, Pack<Args...>>, int>>
constexpr BoundTransformResultE<M, Result<T, E>, Args...>
Result<T, E>::orElse(M &&transform, Args &&...args) const {
	if (hasError()) {
		return (error().*
		        (std::forward<M>(transform)))(std::forward<Args>(args)...);
	}

	return value();
}

template <typename T, typename E>
template <typename C, typename M, typename... Args,
          std::enable_if_t<
			  IsContextTransformE<C, M, Result<T, E>, Pack<Args...>>, int>>
constexpr ContextTransformResultE<C, M, Result<T, E>, Args...>
Result<T, E>::orElse(C *&&context, M &&transform, Args &&...args) const {
	if (hasError()) {
		return (std::forward<C *>(context)->*(std::forward<M>(transform)))(
			error(), std::forward<Args>(args)...);
	}

	return value();
}

// Operators

template <typename T, typename E>
constexpr T &Result<T, E>::operator*() {
	if (hasValue()) {
		return value();
	}

	throw std::runtime_error("Result does not contain a value");
}
template <typename T, typename E>
constexpr const T &Result<T, E>::operator*() const {
	if (hasValue()) {
		return value();
	}

	throw std::runtime_error("Result does not contain a value");
}

template <typename T, typename E>
constexpr T *Result<T, E>::operator->() {
	if (hasValue()) {
		return &value();
	}

	throw std::runtime_error("Result does not contain a value");
}

template <typename T, typename E>
constexpr const T *Result<T, E>::operator->() const {
	if (hasValue()) {
		return &value();
	}

	throw std::runtime_error("Result does not contain a value");
}

template <typename T, typename E>
template <typename U, typename F,
          std::enable_if_t<IsResultFullType<Result<T, E>, U, F>, int>>
constexpr bool Result<T, E>::operator==(const Result<U, F> &other) const {
	if (hasValue() && other.hasValue()) {
		return value() == other.value();
	}

	if (hasError() && other.hasError()) {
		return error() == other.error();
	}

	return false;
}

template <typename T, typename E>
template <typename U, typename F,
          std::enable_if_t<IsResultFullType<Result<T, E>, U, F>, int>>
constexpr bool Result<T, E>::operator!=(const Result<U, F> &other) const {
	return !(*this == other);
}

template <typename T, typename E>
template <typename U, std::enable_if_t<IsResultVType<Result<T, E>, U>, int>>
constexpr bool Result<T, E>::operator==(const U &value) const {
	return hasValue() && this->value() == value;
}

template <typename T, typename E>
template <typename F, std::enable_if_t<IsResultEType<Result<T, E>, F>, int>>
constexpr bool Result<T, E>::operator==(const F &error) const {
	return hasError() && this->error() == error;
}

template <typename T, typename E>
template <typename U, std::enable_if_t<IsResultVType<Result<T, E>, U>, int>>
constexpr bool Result<T, E>::operator!=(const U &value) const {
	return !(*this == value);
}

template <typename T, typename E>
template <typename F, std::enable_if_t<IsResultEType<Result<T, E>, F>, int>>
constexpr bool Result<T, E>::operator!=(const F &error) const {
	return !(*this == error);
}

template <typename T, typename E>
constexpr Result<T, E>::operator bool() const {
	return hasValue();
}

} // namespace bz::core

// NOLINTEND(bugprone-forwarding-reference-overload)