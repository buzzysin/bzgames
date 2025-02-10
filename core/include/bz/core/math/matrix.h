#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <type_traits>

namespace bz::core::math {

template <std::size_t N, std::size_t M, typename T>
class Matrix {
public:
	template <std::size_t N1, std::size_t M1, typename T1>
	friend class Matrix;

	// Construction
public:
	constexpr Matrix() = default;
	~Matrix() = default;

	constexpr Matrix(std::array<T, M * N> data) : _data(data) {}

	constexpr Matrix(const Matrix &) = default;
	constexpr Matrix &operator=(const Matrix &) = default;

	constexpr Matrix(Matrix &&) = default;
	constexpr Matrix &operator=(Matrix &&) = default;

	// Getters
public:
	constexpr T &operator()(std::size_t i, std::size_t j) {
		return _data.at(i * N + j);
	}

	constexpr const T &operator()(std::size_t i, std::size_t j) const {
		return _data.at(i * N + j);
	}

	constexpr T &at(std::size_t i, std::size_t j) {
		return _data.at(i * N + j);
	}

	constexpr const T &at(std::size_t i, std::size_t j) const {
		return _data.at(i * N + j);
	}

	constexpr T *data() { return _data.data(); }

	constexpr const T *data() const { return _data.data(); }

	constexpr std::size_t size() const { return _data.size(); }

	constexpr std::size_t rows() const { return N; }

	constexpr std::size_t cols() const { return M; }

	constexpr auto begin() { return _data.begin(); }
	constexpr auto begin() const { return _data.begin(); }

	constexpr auto end() { return _data.end(); }
	constexpr auto end() const { return _data.end(); }

	constexpr auto cbegin() const { return _data.cbegin(); }
	constexpr auto cend() const { return _data.cend(); }

	// Properties
public:
	constexpr bool square() const { return N == M; }

	template <std::size_t N1 = N, std::size_t M1 = M,
	          typename = std::enable_if_t<N1 == M1, int>>
	static constexpr Matrix<N, M, T> identity() {
		Matrix<N, M, T> result;
		for (std::size_t i = 0; i < N; ++i) {
			result.at(i, i) = 1;
		}
		return result;
	}

	// Operations
public:
	template <std::size_t N1, std::size_t M1,
	          typename = std::enable_if_t<M == N1, int>>
	constexpr Matrix &matrixProduct(const Matrix<N1, M1, T> &other) {
		Matrix<N, M1, T> result{};

		for (std::size_t i = 0; i < N; ++i) {
			for (std::size_t j = 0; j < M1; ++j) {
				T sum = 0;
				for (std::size_t k = 0; k < M; ++k) {
					sum += _data.at(i * M + k) * other._data.at(k * M1 + j);
				}
				result.at(i, j) = sum;
			}
		}

		*this = result;
		return *this;
	}

	template <std::size_t N1, std::size_t M1,
	          typename = std::enable_if_t<M == N1, int>>
	constexpr auto matrixProduct(const Matrix<N1, M1, T> &other) const {
		Matrix<N, M1, T> result = *this;
		result.matrixProduct(other);
		return result;
	}

	template <typename Op>
	constexpr Matrix &scalarOp(Op &&op) {
		for (std::size_t x = 0; x < N * M; ++x) {
			_data[x] = static_cast<T>(std::forward<Op>(op)(_data[x]));
		}

		return *this;
	}

	template <typename Op>
	constexpr auto scalarOp(Op &&op) {
		using U = decltype(std::forward<Op>(op)(std::declval<T>()));

		Matrix<N, M, U> result;
		for (std::size_t x = 0; x < N * M; ++x) {
			result._data[x] = static_cast<U>(std::forward<Op>(op)(_data[x]));
		}

		return result;
	}

	template <typename U, typename Op>
	constexpr Matrix &matrixOp(const Matrix<N, M, U> &mat, Op &&op) {
		for (std::size_t x = 0; x < N * M; ++x) {
			_data[x] =
				static_cast<T>(std::forward<Op>(op)(_data[x], mat._data[x]));
		}

		return *this;
	}

	template <typename Op>
	constexpr auto matrixOp(Op &&op) {
		using U = decltype(std::forward<Op>(op)(std::declval<T>()));

		Matrix<N, M, U> result;
		for (std::size_t x = 0; x < N * M; ++x) {
			result._data[x] = static_cast<U>(std::forward<Op>(op)(_data[x]));
		}

		return result;
	}

	// Allow in-place transpose only for square matrices
	template <std::size_t N1 = N, std::size_t M1 = M,
	          typename = std::enable_if_t<N1 == M1, int>>
	constexpr Matrix &transpose() {
		// More efficient to swap the elements in-place,
		// and only half of the elements need to be swapped
		for (std::size_t i = 0; i < N; ++i) {
			for (std::size_t j = i + 1; j < M; ++j) {
				std::swap(_data[i * M + j], _data[j * N + i]);
			}
		}

		return *this;
	}

	// Otherwise, return a copy with the right dimensions
	// (this also accounts for the case where N == M but the matrix is const)
	template <std::size_t N1 = N, std::size_t M1 = M,
	          typename = std::enable_if_t<N1 != M1, int>>
	constexpr Matrix<M, N, T> transpose() const {
		Matrix<M, N, T> result;
		for (std::size_t i = 0; i < N; ++i) {
			for (std::size_t j = 0; j < M; ++j) {
				result._data[j * N + i] = _data[i * M + j];
			}
		}
		return result;
	}

	// Operators
public:
private:
	std::array<T, M * N> _data;
};

using Mat3f = Matrix<3, 3, float>;
using Mat4f = Matrix<4, 4, float>;
using Vec3f = Matrix<3, 1, float>;
using Vec4f = Matrix<4, 1, float>;

template <std::size_t N, std::size_t M, typename T>
constexpr Matrix<N, M, T> MatrixFrom(std::array<T, M * N> data) {
	return Matrix<N, M, T>(data);
}

} // namespace bz::core::math

#include "matrix_operators.h"

namespace bz::core::math::tests {

constexpr void testMatrix() {
	constexpr std::array<int, 4> data1 = {1, 2, //
	                                      3, 4};
	constexpr std::array<int, 4> data2 = {5, 6, //
	                                      7, 8};

	constexpr Matrix<2, 2, int> matrix1(MatrixFrom<2, 2>(data1));
	constexpr Matrix<2, 2, int> matrix2(MatrixFrom<2, 2>(data2));

	constexpr auto result = matrix1.matrixProduct(matrix2);

	static_assert(result(0, 0) == 19);
	static_assert(result(0, 1) == 22);
	static_assert(result(1, 0) == 43);
	static_assert(result(1, 1) == 50);
}

void testMatrix2() {
	constexpr std::array<int, 4> data1 = {1, 2, //
	                                      3, 4};
	constexpr std::array<int, 4> data2 = {5, 6, //
	                                      7, 8};

	auto matrix3(MatrixFrom<2, 2>(data1));
	auto matrix4(MatrixFrom<2, 2>(data2));

	matrix3 = matrix3.matrixProduct(matrix4);

	assert(matrix3(0, 0) == 19);
	assert(matrix3(0, 1) == 22);
	assert(matrix3(1, 0) == 43);
	assert(matrix3(1, 1) == 50);
}

void testMatrixOperators() {
	constexpr std::array<int, 4> data1 = {1, 2, //
	                                      3, 4};
	constexpr std::array<int, 4> data2 = {5, 6, //
	                                      7, 8};

	constexpr Matrix<2, 2, int> matrix1(MatrixFrom<2, 2>(data1));
	constexpr Matrix<2, 2, int> matrix2(MatrixFrom<2, 2>(data2));

	constexpr auto resultAdd = matrix1 + matrix2;

	static_assert(resultAdd(0, 0) == 6);
	static_assert(resultAdd(0, 1) == 8);
	static_assert(resultAdd(1, 0) == 10);
	static_assert(resultAdd(1, 1) == 12);
}

} // namespace bz::core::math::tests

template <typename T>
constexpr T taylor_sin(T radians) {
	T result = radians;
	T term = radians;
	T sign = -1;
	for (int i = 3; i < 20; i += 2) {
		term *= radians * radians / (i * (i - 1));
		result += sign * term;
		sign *= -1;
	}
	return result;
}

template <typename T>
constexpr T taylor_cos(T radians) {
	T result = 1;
	T term = 1;
	T sign = -1;
	for (int i = 2; i < 20; i += 2) {
		term *= radians * radians / (i * (i - 1));
		result += sign * term;
		sign *= -1;
	}
	return result;
}
