#pragma once

#include "core_traits.h"
#include <type_traits>
#include <utility>

namespace bz::core {

template <typename T, typename E>
class Result;

template <typename R>
struct IsResult_ : std::false_type {};

template <typename T, typename E>
struct IsResult_<Result<T, E>> : std::true_type {};

template <typename R>
struct ResultValueType_ {
    using type = void;
};

template <typename T, typename E>
struct ResultValueType_<Result<T, E>> {
    using type = T;
};

template <typename R>
using ResultValueType = typename ResultValueType_<R>::type;

template <typename R>
struct ResultErrorType_ {
    using type = void;
};

template <typename T, typename E>
struct ResultErrorType_<Result<T, E>> {
    using type = E;
};

template <typename R>
using ResultErrorType = typename ResultErrorType_<R>::type;

template <typename R>
constexpr bool IsResult = IsResult_<R>::value;

template <typename R, typename T>
constexpr bool IsResultVType =
    IsResult<R> && !IsResult<T> && std::is_convertible_v<T, ResultValueType<R>>;

template <typename R, typename E>
constexpr bool IsResultEType =
    IsResult<R> && !IsResult<E> && std::is_convertible_v<E, ResultErrorType<R>>;

template <typename R, typename T, typename E>
constexpr bool IsResultFullType = IsResultVType<R, T> && IsResultEType<R, E>;

template <typename R>
using ResultErrorType = typename ResultErrorType_<R>::type;

//
// Value Transforms
//

template <typename F, typename R, typename Args, typename = void>
constexpr bool IsFreeTransformV = false;

template <typename F, typename R, typename... Args>
using FreeTransformResultV = decltype(std::declval<NoCVRef<F>>()(
    std::declval<ResultValueType<R>>(), std::declval<Args>()...));

template <typename F, typename R, typename... Args>
constexpr bool IsFreeTransformV<
    F, R, Pack<Args...>, std::void_t<FreeTransformResultV<F, R, Args...>>> =
    true;

template <typename M, typename R, typename Args, typename = void>
constexpr bool IsBoundTransformV = false;

template <typename M, typename R, typename... Args>
using BoundTransformResultV =
    decltype((std::declval<ResultValueType<R>>().*
              std::declval<NoCVRef<M>>())(std::declval<Args>()...));

template <typename M, typename R, typename... Args>
constexpr bool IsBoundTransformV<
    M, R, Pack<Args...>, std::void_t<BoundTransformResultV<M, R, Args...>>> =
    true;

template <typename C, typename M, typename R, typename Args, typename = void>
constexpr bool IsContextTransformV = false;

template <typename C, typename M, typename R, typename... Args>
using ContextTransformResultV =
    decltype((std::declval<C *>()->*std::declval<NoCVRef<M>>())(
        std::declval<ResultValueType<R>>(), std::declval<Args>()...));

template <typename C, typename M, typename R, typename... Args>
constexpr bool IsContextTransformV<
    C, M, R, Pack<Args...>,
    std::void_t<ContextTransformResultV<C, M, R, Args...>>> = true;

//
// Error Transforms
//

template <typename F, typename R, typename Args, typename = void>
constexpr bool IsFreeTransformE = false;

template <typename F, typename R, typename... Args>
using FreeTransformResultE = decltype(std::declval<NoCVRef<F>>()(
    std::declval<ResultErrorType<R>>(), std::declval<Args>()...));
template <typename F, typename R, typename... Args>
constexpr bool IsFreeTransformE<
    F, R, Pack<Args...>, std::void_t<FreeTransformResultE<F, R, Args...>>> =
    true;

template <typename M, typename R, typename Args, typename = void>
constexpr bool IsBoundTransformE = false;

template <typename M, typename R, typename... Args>
using BoundTransformResultE =
    decltype((std::declval<ResultErrorType<R>>().*
              std::declval<NoCVRef<M>>())(std::declval<Args>()...));

template <typename M, typename R, typename... Args>
constexpr bool IsBoundTransformE<
    M, R, Pack<Args...>, std::void_t<BoundTransformResultE<M, R, Args...>>> =
    true;

template <typename C, typename M, typename R, typename Args, typename = void>
constexpr bool IsContextTransformE = false;

template <typename C, typename M, typename R, typename... Args>
using ContextTransformResultE =
    decltype((std::declval<C *>()->*std::declval<NoCVRef<M>>())(
        std::declval<ResultErrorType<R>>(), std::declval<Args>()...));

template <typename C, typename M, typename R, typename... Args>
constexpr bool IsContextTransformE<
    C, M, R, Pack<Args...>,
    std::void_t<ContextTransformResultE<C, M, R, Args...>>> = true;

} // namespace bz::core