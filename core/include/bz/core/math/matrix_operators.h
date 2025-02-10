#pragma once

#include "matrix.h"

//
// Matrix product
//

template <std::size_t N, std::size_t M, typename T, std::size_t N1,
          std::size_t M1, typename U>
constexpr auto operator*=(bz::core::math::Matrix<N, M, T> &lhs,
                          const bz::core::math::Matrix<N1, M1, U> &rhs) {
	return lhs.matrixProduct(rhs);
}

template <std::size_t N, std::size_t M, typename T, std::size_t N1,
          std::size_t M1, typename U>
constexpr auto operator*(const bz::core::math::Matrix<N, M, T> &lhs,
                         const bz::core::math::Matrix<N1, M1, U> &rhs) {
	auto result = lhs;
	result.matrixProduct(rhs);
	return result;
}

//
// Matrix scalar product
//

template <std::size_t N, std::size_t M, typename T, typename U,
          std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
constexpr auto operator*=(bz::core::math::Matrix<N, M, T> &lhs, U scalar) {
	return lhs.scalarOp([scalar](T value) { return value * scalar; });
}

template <std::size_t N, std::size_t M, typename T, typename U,
          std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
constexpr auto operator*(const bz::core::math::Matrix<N, M, T> &lhs, U scalar) {
	auto result = lhs;
	result *= scalar;
	return result;
}

//
// Matrix scalar division
//

template <std::size_t N, std::size_t M, typename T, typename U,
          std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
constexpr auto operator/=(bz::core::math::Matrix<N, M, T> &lhs, U scalar) {
	return lhs.scalarOp([scalar](T value) { return value / scalar; });
}

template <std::size_t N, std::size_t M, typename T, typename U,
          std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
constexpr auto operator/(const bz::core::math::Matrix<N, M, T> &lhs, U scalar) {
	auto result = lhs;
	result /= scalar;
	return result;
}

//
// Matrix addition
//

template <std::size_t N, std::size_t M, typename T, typename U>
constexpr auto operator+=(bz::core::math::Matrix<N, M, T> &lhs,
                          const bz::core::math::Matrix<N, M, U> &rhs) {
	return lhs.matrixOp(rhs, [](T value, U other) { return value + other; });
}

template <std::size_t N, std::size_t M, typename T, typename U>
constexpr auto operator+(const bz::core::math::Matrix<N, M, T> &lhs,
                         const bz::core::math::Matrix<N, M, U> &rhs) {
	auto result = lhs;
	result += rhs;
	return result;
}

// Matrix scalar addition

template <std::size_t N, std::size_t M, typename T, typename U,
          std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
constexpr auto operator+=(bz::core::math::Matrix<N, M, T> &lhs, U scalar) {
	return lhs.scalarOp([scalar](T value) { return value + scalar; });
}

template <std::size_t N, std::size_t M, typename T, typename U,
          std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
constexpr auto operator+(const bz::core::math::Matrix<N, M, T> &lhs, U scalar) {
	auto result = lhs;
	result += scalar;
	return result;
}

//
// Matrix subtraction
//

template <std::size_t N, std::size_t M, typename T, typename U>
constexpr auto operator-=(bz::core::math::Matrix<N, M, T> &lhs,
                          const bz::core::math::Matrix<N, M, U> &rhs) {
	return lhs.matrixOp([&rhs](T value) { return value - rhs; });
}

template <std::size_t N, std::size_t M, typename T, typename U>
constexpr auto operator-(const bz::core::math::Matrix<N, M, T> &lhs,
                         const bz::core::math::Matrix<N, M, U> &rhs) {
	auto result = lhs;
	result -= rhs;
	return result;
}

//
// Matrix scalar subtraction
//

template <std::size_t N, std::size_t M, typename T, typename U,
          std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
constexpr auto operator-=(bz::core::math::Matrix<N, M, T> &lhs, U scalar) {
	return lhs.scalarOp([scalar](T value) { return value - scalar; });
}

template <std::size_t N, std::size_t M, typename T, typename U,
          std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
constexpr auto operator-(const bz::core::math::Matrix<N, M, T> &lhs, U scalar) {
	auto result = lhs;
	result -= scalar;
	return result;
}

//
// Comparison
//
template <std::size_t N, std::size_t M, typename T, std::size_t N1,
          std::size_t M1, typename U>
constexpr auto operator==(const bz::core::math::Matrix<N, M, T> &lhs,
                          const bz::core::math::Matrix<N1, M1, U> &rhs) {
	if constexpr (N != N1 || M != M1) {
		return false;
	} else {
		for (std::size_t i = 0; i < N; ++i) {
			for (std::size_t j = 0; j < M; ++j) {
				if (lhs.at(i, j) != rhs.at(i, j)) {
					return false;
				}
			}
		}
		return true;
	}
}