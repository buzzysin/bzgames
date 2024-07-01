#pragma once

#include <sstream>

namespace bz::core {
template <typename T>
struct IsOstreamCompatible_ {
	static auto test(T *) -> decltype(std::declval<std::ostream &>()
	                                      << std::declval<std::decay_t<T>>(),
	                                  std::true_type{});

	static auto test(...) -> std::false_type;

	static constexpr bool value = decltype(test(nullptr))::value;
};

template <>
struct IsOstreamCompatible_<void> : std::false_type {};

template <typename T>
constexpr bool IsOstreamCompatible = IsOstreamCompatible_<T>::value;

static_assert(IsOstreamCompatible<int>);
static_assert(IsOstreamCompatible<std::string>);
static_assert(IsOstreamCompatible<std::string_view>);
static_assert(IsOstreamCompatible<char *>);
static_assert(IsOstreamCompatible<char[1]>); // NOLINT
static_assert(IsOstreamCompatible<float>);
static_assert(IsOstreamCompatible<double>);
static_assert(!IsOstreamCompatible<void>);

template <typename T>
struct LogFormat;

template <typename T>
struct LogFormat {
	static std::enable_if_t<IsOstreamCompatible<T>, std::string>
	format(const T &value) {
		std::ostringstream stream;
		stream << value;
		return stream.str();
	}
};

// SFINAE to check if a type is formattable
template <typename T>
struct IsFormattable_ {
	template <typename U>
	static auto test(U *) -> decltype(LogFormat<U>::format(std::declval<U>()),
	                                  std::true_type{});

	template <typename U>
	static auto test(...) -> std::false_type;

	static constexpr bool value = decltype(test<T>(nullptr))::value;
};

template <typename T>
constexpr bool IsFormattable = IsFormattable_<T>::value;
} // namespace bz::core