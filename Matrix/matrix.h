#ifndef MATRIX_H
#define MATRIX_H

#include <type_traits>
#include <vector>

#include "Big_int/rational.h"

namespace Mtx
{
	template <size_t M, size_t N, typename Field = Rational>
	class Matrix
	{
	public:
		Matrix();
		Matrix(const std::vector<Field>& vec);
		Matrix(const std::vector<std::vector<int>>& vec);
		Matrix(const Matrix& other) = default;
		Matrix& operator=(const Matrix& other) = default;
		~Matrix() = default;

		bool operator==(const Matrix& other) const;

		std::vector<Field>& operator[](size_t index);
		const std::vector<Field>& operator[](size_t index) const;

		Matrix& operator+=(const Matrix& other);
		Matrix& operator-=(const Matrix& other);
		Matrix& operator*=(int number);
		Matrix& operator*=(const Matrix& other);

		[[nodiscard]] Matrix<N, M, Field> transposed() const;
		//[[nodiscard]] Matrix inverted() const;
		//Matrix& invert();
		//[[nodiscard]] size_t rank() const;
		[[nodiscard]] Field det() const;
		[[nodiscard]] Field trace() const;
		[[nodiscard]] std::vector<Field> get_row(size_t index) const;
		[[nodiscard]] std::vector<Field> get_column(size_t index) const;
		void swap_row(size_t index, size_t with_index) noexcept;

	private:
		std::vector<std::vector<Field>> _matrix;
	};

	template <size_t M, typename Field = Rational>
	using Square_matrix = Matrix<M, M, Field>;

	template <size_t M, size_t N, typename Field>
	Matrix<M, N, Field>::Matrix()
		: _matrix(M, std::vector<Field>(N, 0)) {}

	template <size_t M, size_t N, typename Field>
	Matrix<M, N, Field>::Matrix(const std::vector<Field>& vec)
		: _matrix(M, std::vector<Field>(N))
	{
		for (size_t m = 0; m < M; ++m) {
			for (size_t n = 0; n < N; ++n) {
				_matrix[m][n] = vec[m * N + n];
			}
		}
	}

	template <size_t M, size_t N, typename Field>
	Matrix<M, N, Field>::Matrix(const std::vector<std::vector<int>>& vec)
		: _matrix(M, std::vector<Field>(N))
	{
		for (size_t m = 0; m < M; ++m) {
			for (size_t n = 0; n < N; ++n) {
				_matrix[m][n] = vec[m][n];
			}
		}
	}

	template <size_t M, size_t N, typename Field>
	bool Matrix<M, N, Field>::operator==(const Matrix& other) const
	{
		for (size_t m = 0; m < M; ++m) {
			for (size_t n = 0; n < N; ++n) {
				if (_matrix[m][n] != other._matrix[m][n]) {
					return false;
				}
			}
		}
		return true;
	}

	template <size_t M, size_t N, typename Field>
	std::vector<Field>& Matrix<M, N, Field>::operator[](size_t index)
	{
		return _matrix[index];
	}

	template <size_t M, size_t N, typename Field>
	const std::vector<Field>& Matrix<M, N, Field>::operator[](size_t index) const
	{
		return _matrix[index];
	}

	template <size_t M, size_t N, typename Field>
	Matrix<M, N, Field>& Matrix<M, N, Field>::operator+=(const Matrix& other)
	{
		for (size_t m = 0; m < M; ++m) {
			for (size_t n = 0; n < N; ++n) {
				_matrix[m][n] += other._matrix[m][n];
			}
		}
		return *this;
	}

	template <size_t M, size_t N, typename Field>
	Matrix<M, N, Field>& Matrix<M, N, Field>::operator-=(const Matrix& other)
	{
		for (size_t m = 0; m < M; ++m) {
			for (size_t n = 0; n < N; ++n) {
				_matrix[m][n] -= other._matrix[m][n];
			}
		}
		return *this;
	}

	template <size_t M, size_t N, typename Field>
	Matrix<M, N, Field>& Matrix<M, N, Field>::operator*=(int number)
	{
		for (size_t m = 0; m < M; ++m) {
			for (size_t n = 0; n < N; ++n) {
				_matrix[m][n] *= number;
			}
		}
		return *this;
	}

	template <size_t M, size_t N, typename Field>
	Matrix<M, N, Field>& Matrix<M, N, Field>::operator*=(const Matrix& other)
	{
		static_assert(M == N, "The non-square matrix");
		return *this = *this * other;
	}

	template <size_t M, size_t N, typename Field>
	Matrix<N, M, Field> Matrix<M, N, Field>::transposed() const
	{
		Matrix<N, M, Field> ret;
		for (size_t m = 0; m < M; ++m) {
			for (size_t n = 0; n < N; ++n) {
				ret[n][m] = _matrix[m][n];
			}
		}
		return ret;
	}

	template <size_t M, size_t N, typename Field>
	Field Matrix<M, N, Field>::det() const
	{
		static_assert(M == N, "The non-square matrix");
		Matrix<M, N, Field> copy(*this);
		bool sign = false;
		Field prev = 1;
		for (size_t k = 0; k < N - 1; ++k) {
			if (copy._matrix[k][k] == 0) {
				size_t i = 0;
				while (copy._matrix[i][k] == 0 and i < N) {
					++i;
				}
				copy.swap_row(k, i);
				sign = !sign;
				if (copy._matrix[k][k] == 0) {
					return static_cast<Field>(0);
				}
			}
			for (size_t i = k + 1; i < N; ++i) {
				for (size_t j = k + 1; j < N; ++j) {
					copy._matrix[i][j] = (_matrix[i][j] * _matrix[k][k] - _matrix[i][k] * _matrix[k][j]) / prev;
				}
			}
			prev = copy._matrix[k][k];
		}
		return sign ? -copy._matrix[N - 1][N - 1] : copy._matrix[N - 1][N - 1];
	}

	template <size_t M, size_t N, typename Field>
	Field Matrix<M, N, Field>::trace() const
	{
		static_assert(M == N, "The non-square matrix");
		Field ret = 0;
		for (size_t i = 0; i < N; ++i) {
			ret += _matrix[i][i];
		}
		return ret;
	}

	template <size_t M, size_t N, typename Field>
	std::vector<Field> Matrix<M, N, Field>::get_row(size_t index) const
	{
		return _matrix[index];
	}

	template <size_t M, size_t N, typename Field>
	std::vector<Field> Matrix<M, N, Field>::get_column(size_t index) const
	{
		std::vector<Field> ret(N);
		for (size_t m = 0; m < M; ++m) {
			ret[m] = _matrix[m][index];
		}
		return ret;
	}

	template <size_t M, size_t N, typename Field>
	void Matrix<M, N, Field>::swap_row(size_t index, size_t with_index) noexcept
	{
		std::swap(_matrix[index], _matrix[with_index]);
	}

	template <size_t M, size_t N, typename Field = Rational>
	Matrix<M, N, Field> operator+(const Matrix<M, N, Field>& lhs, const Matrix<M, N, Field>& rhs)
	{
		Matrix<M, N, Field> ret(lhs);
		ret += rhs;
		return ret;
	}

	template <size_t M, size_t N, typename Field = Rational>
	Matrix<M, N, Field> operator-(const Matrix<M, N, Field>& lhs, const Matrix<M, N, Field>& rhs)
	{
		Matrix<M, N, Field> ret(lhs);
		ret -= rhs;
		return ret;
	}

	template <size_t M, size_t N, typename Field = Rational>
	Matrix<M, N, Field> operator*(const Matrix<M, N, Field>& matrix, int number)
	{
		Matrix<M, N, Field> ret(matrix);
		ret *= number;
		return ret;
	}

	template <size_t M, size_t N, typename Field = Rational>
	Matrix<M, N, Field> operator*(int number, const Matrix<M, N, Field>& matrix)
	{
		Matrix<M, N, Field> ret(matrix);
		ret *= number;
		return ret;
	}

	template <size_t M, size_t K, size_t N, typename Field = Rational>
	Matrix<M, N, Field> operator*(const Matrix<M, K, Field>& lhs, const Matrix<K, N, Field>& rhs)
	{
		Matrix<M, N, Field> ret;
		for (size_t m = 0; m < M; ++m) {
			for (size_t n = 0; n < N; ++n) {
				for (size_t k = 0; k < K; ++k) {
					ret[m][n] += lhs[m][k] * rhs[k][n];
				}
			}
		}
		return ret;
	}
}

#endif
