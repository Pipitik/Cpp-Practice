#ifndef RATIONAL_H
#define RATIONAL_H

#include <iostream>
#include <compare>
#include <numeric>
#include <string>
#include "Big_int.h"

class Rational
{
public:
	using value_type = Big_int;

	Rational(int numerator = 0, int denominator = 1);
	Rational(const value_type& numerator, const value_type& denominator = 1);
	Rational(const Rational&) = default;
	Rational& operator=(const Rational&) = default;

	Rational& operator+=(const Rational& rhs);
	Rational& operator-=(const Rational& rhs);
	Rational& operator*=(const Rational& rhs);
	Rational& operator/=(const Rational& rhs);

	[[nodiscard]] Rational operator+() const;
	[[nodiscard]] Rational operator-() const;

	Rational& operator++();
	Rational operator++(int);
	Rational& operator--();
	Rational operator--(int);

	bool operator==(const Rational& rhs) const = default;
	std::strong_ordering operator<=>(const Rational& rhs) const;

	[[nodiscard]] std::string to_string() const;
	[[nodiscard]] std::string as_decimal(size_t precision = 0) const;
	[[nodiscard]] value_type numerator() const;
	[[nodiscard]] value_type denominator() const;

	explicit operator double() const;

private:
	value_type _numerator;
	value_type _denominator;

	static void _rounding(std::string& str);
	bool _sign() const;
	bool _is_not_integer() const;
	void _correct_sign();
	void _simplify();
};

Rational operator+(const Rational& lhs, const Rational& rhs);
Rational operator-(const Rational& lhs, const Rational& rhs);
Rational operator*(const Rational& lhs, const Rational& rhs);
Rational operator/(const Rational& lhs, const Rational& rhs);

#endif
