#pragma once

#include "result_traits.h"

#include <optional>

namespace bz::core {

template <typename T, typename E>
class Result {

public:
	// Friends

	template <typename U, typename F>
	friend class Result;

	// Construct
	constexpr Result() = delete;

	template <typename U, std::enable_if_t<IsResultVType<Result, U>, int> = 0>
	constexpr Result(const U &value);

	template <typename U, std::enable_if_t<IsResultVType<Result, U>, int> = 0>
	constexpr Result(U &&value);

	template <typename F, std::enable_if_t<IsResultEType<Result, F>, int> = 0>
	constexpr Result(const F &error);

	template <typename F, std::enable_if_t<IsResultEType<Result, F>, int> = 0>
	constexpr Result(F &&error);

	// Copy

	template <typename U, typename F,
	          std::enable_if_t<IsResultFullType<Result, U, F>, int> = 0>
	constexpr Result(const Result<U, F> &other);

	// Move

	template <typename U, typename F,
	          std::enable_if_t<IsResultFullType<Result, U, F>, int> = 0>
	constexpr Result(Result<U, F> &&other) noexcept;

	template <typename U, typename F,
	          std::enable_if_t<IsResultFullType<Result, U, F>, int> = 0>
	constexpr Result &operator=(Result<U, F> &&other) noexcept;

	template <typename U, std::enable_if_t<IsResultVType<Result, U>, int> = 0>
	constexpr Result &operator=(U &&value);

	template <typename F, std::enable_if_t<IsResultEType<Result, F>, int> = 0>
	constexpr Result &operator=(F &&error);

	// Check/Get

	constexpr bool hasValue() const;
	constexpr bool hasError() const;

	constexpr const T &value() const &;
	constexpr T &value() &;

	constexpr const T &&value() const &&;
	constexpr T &&value() &&;

	constexpr const E &error() const &;
	constexpr E &error() &;

	constexpr const E &&error() const &&;
	constexpr E &&error() &&;

	// Transformers

	template <typename U>
	constexpr Result<U, E> and_(const Result<U, E> &other) const;

	template <typename U>
	constexpr Result<U, E> and_(Result<U, E> &&other) const;

	//
	// Value transformers
	//

	template <
		typename F, typename... Args,
		std::enable_if_t<IsFreeTransformV<F, Result, Pack<Args...>>, int> = 0>
	constexpr FreeTransformResultV<F, Result, Args...>
	andThen(F &&transform, Args &&...args) const;

	template <
		typename F, typename... Args,
		std::enable_if_t<IsFreeTransformV<F, Result, Pack<Args...>>, int> = 0>
	constexpr FreeTransformResultV<F, Result, Args...> andThen(F &&transform,
	                                                           Args &&...args);

	template <
		typename M, typename... Args,
		std::enable_if_t<IsBoundTransformV<M, Result, Pack<Args...>>, int> = 0>
	constexpr BoundTransformResultV<M, Result, Args...>
	andThen(M &&transform, Args &&...args) const;

	template <typename C, typename M, typename... Args,
	          std::enable_if_t<IsContextTransformV<C, M, Result, Pack<Args...>>,
	                           int> = 0>
	constexpr ContextTransformResultV<C, M, Result, Args...>
	andThen(C *&&context, M &&transform, Args &&...args) const;

	//
	// Error transformers
	//

	template <
		typename F, typename... Args,
		std::enable_if_t<IsFreeTransformE<F, Result, Pack<Args...>>, int> = 0>
	constexpr FreeTransformResultE<F, Result, Args...>
	orElse(F &&transform, Args &&...args) const;

	template <
		typename M, typename... Args,
		std::enable_if_t<IsBoundTransformE<M, Result, Pack<Args...>>, int> = 0>
	constexpr BoundTransformResultE<M, Result, Args...>
	orElse(M &&transform, Args &&...args) const;

	template <typename C, typename M, typename... Args,
	          std::enable_if_t<IsContextTransformE<C, M, Result, Pack<Args...>>,
	                           int> = 0>
	constexpr ContextTransformResultE<C, M, Result, Args...>
	orElse(C *&&context, M &&transform, Args &&...args) const;

	//
	// Operators
	//

	constexpr T &operator*();
	constexpr const T &operator*() const;

	constexpr T *operator->();
	constexpr const T *operator->() const;

	template <typename U, typename F,
	          std::enable_if_t<IsResultFullType<Result, U, F>, int> = 0>
	constexpr bool operator==(const Result<U, F> &other) const;

	template <typename U, typename F,
	          std::enable_if_t<IsResultFullType<Result, U, F>, int> = 0>
	constexpr bool operator!=(const Result<U, F> &other) const;

	template <typename U, std::enable_if_t<IsResultVType<Result, U>, int> = 0>
	constexpr bool operator==(const U &value) const;

	template <typename F, std::enable_if_t<IsResultEType<Result, F>, int> = 0>
	constexpr bool operator==(const F &error) const;

	template <typename U, std::enable_if_t<IsResultVType<Result, U>, int> = 0>
	constexpr bool operator!=(const U &value) const;

	template <typename F, std::enable_if_t<IsResultEType<Result, F>, int> = 0>
	constexpr bool operator!=(const F &error) const;

	constexpr operator bool() const;

private:
	std::optional<T> _value{std::nullopt};
	std::optional<E> _error{std::nullopt};
};

// Use in place of void. We don't want to use void because it's not a complete
// type and we can't use it in a template. (Without extra magic that I can't be
// bothered to do)
struct Empty {};

template <typename E>
using Fallible = Result<Empty, E>;

} // namespace bz::core

#include "result.tpp"