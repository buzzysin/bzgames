#pragma once

#include <type_traits>

namespace bz::core {

template <typename T>
using NoRef = std::remove_reference_t<T>;

template <typename T>
using NoConst = std::remove_const_t<T>;

template <typename T>
using NoVolatile = std::remove_volatile_t<T>;

template <typename T>
using NoCRef = NoConst<NoRef<T>>;

template <typename T>
using NoCVRef = NoVolatile<NoCRef<T>>;

// NoPtr
template <typename T>
using NoPtr = std::remove_pointer_t<T>;

template<typename ...Args>
using Pack = std::tuple<Args...>;

} // namespace bz::core