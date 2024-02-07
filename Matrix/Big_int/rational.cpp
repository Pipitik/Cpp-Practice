#include "rational.h"

void Rational::_rounding(std::string& str)
{
	using str_size = std::string::size_type;
	bool is_rounding = false;
	str_size reverse_i = str.size() - 1;
	do {
		if (str[reverse_i] == '9') {
			str[reverse_i] = '0';
		}
		else {
			++str[reverse_i];
			is_rounding = true;
		}
		--reverse_i;
	} while (!is_rounding and str[reverse_i] != '.');

	if (!is_rounding) {
		--reverse_i;
		if (str[reverse_i] == '9') {
			str[reverse_i] = '0';
			str.insert(reverse_i, "1");
		}
		else {
			++str[reverse_i];
		}
	}
}

bool Rational::_sign() const
{
	return _numerator < 0;
}

bool Rational::_is_not_integer() const
{
	return _denominator != 1;
}

void Rational::_correct_sign()
{
	if (_denominator < 0) {
		_numerator.negate();
		_denominator.negate();
	}
}

void Rational::_simplify()
{
	value_type _gcd = gcd(_sign() ? -_numerator : _numerator,
							_denominator);
	_numerator /= _gcd;
	_denominator /= _gcd;
}

Rational::Rational(int numerator, int denominator)
	: _numerator(numerator)
	, _denominator(denominator)
{
	if (!denominator) {
		throw "Denominator cannot be zero";
	}
	else if (denominator < 0) {
		_numerator.negate();
		_denominator.negate();
	}
	_simplify();
}

Rational::Rational(const value_type& numerator, const value_type& denominator)
	: _numerator(numerator)
	, _denominator(denominator)
{
	if (!denominator) {
		throw "Denominator cannot be zero";
	}
	else if (denominator < 0) {
		_numerator.negate();
		_denominator.negate();
	}
	_simplify();
}

Rational& Rational::operator+=(const Rational& rhs)
{
	if (_denominator == rhs._denominator) {
		_numerator += rhs._numerator;
	}
	else {
		_numerator = rhs._denominator * _numerator + _denominator * rhs._numerator;
		_denominator *= rhs._denominator;
	}
	_simplify();
	return *this;
}

Rational& Rational::operator-=(const Rational& rhs)
{
	if (_denominator == rhs._denominator) {
		_numerator -= rhs._numerator;
	}
	else {
		_numerator = rhs._denominator * _numerator - _denominator * rhs._numerator;
		_denominator *= rhs._denominator;
	}
	_simplify();
	return *this;
}

Rational& Rational::operator*=(const Rational& rhs)
{
	_numerator *= rhs._numerator;
	_denominator *= rhs._denominator;
	_simplify();
	return *this;
}

Rational& Rational::operator/=(const Rational& rhs)
{
	if (!rhs._numerator) {
		throw "Division by zero";
	}
	_numerator *= rhs._denominator;
	_denominator *= rhs._numerator;
	_correct_sign();
	_simplify();
	return *this;
}

Rational Rational::operator+() const
{
	return *this;
}

Rational Rational::operator-() const
{
	return Rational(-_numerator, _denominator);
}

Rational& Rational::operator++()
{
	return *this += 1;
}

Rational Rational::operator++(int)
{
	Rational ret(*this);
	*this += 1;
	return ret;
}

Rational& Rational::operator--()
{
	return *this -= 1;
}

Rational Rational::operator--(int)
{
	Rational ret(*this);
	*this -= 1;
	return ret;
}

std::strong_ordering Rational::operator<=>(const Rational& rhs) const
{
	value_type lhs_numerator = _numerator;
	value_type rhs_numerator = rhs._numerator;
	if (_denominator != rhs._denominator) {
		lhs_numerator *= rhs._denominator;
		rhs_numerator *= _denominator;
	}

	if (lhs_numerator < rhs_numerator) {
		return std::strong_ordering::less;
	}
	else if (lhs_numerator > rhs_numerator) {
		return std::strong_ordering::greater;
	}
	else {
		return std::strong_ordering::equivalent;
	}
}

std::string Rational::to_string() const
{
	std::string ret = _numerator.to_string();
	if (_is_not_integer()) {
		ret += '/' + _denominator.to_string();
	}
	return ret;
}

std::string Rational::as_decimal(size_t precision) const
{
	std::string ret;
	value_type integer_part = _numerator / _denominator;
	if (!integer_part and _sign()) {
		ret = '-';
	}

	value_type float_part = _numerator;
	if (_sign()) {
		float_part.negate();
	}

	if (precision) {
		ret += (integer_part).to_string() + '.';

		for (size_t i = 0; (float_part %= _denominator) != 0 and i < precision; ++i) {
			float_part *= 10;
			ret += (float_part / _denominator).to_string();
		}

		float_part *= 10;
		int rounding = static_cast<int>(float_part / _denominator);
		if (rounding >= 5) {
			_rounding(ret);
		}

		while (ret.back() == '0') {
			ret.pop_back();
		}

		if (ret.back() == '.') {
			ret.pop_back();
		}
	}
	else {
		float_part %= _denominator;
		float_part *= 10;
		if (float_part / _denominator >= 5) {
			ret += (integer_part + (_sign() and integer_part ? -1 : 1)).to_string();
		}
		else {
			ret += (integer_part).to_string();
		}
	}
	return ret;
}

Rational::value_type Rational::numerator() const
{
	return _numerator;
}

Rational::value_type Rational::denominator() const
{
	return _denominator;
}

Rational::operator double() const
{
	return std::stod(as_decimal(std::numeric_limits<double>::digits10));
}

Rational operator+(const Rational& lhs, const Rational& rhs)
{
	Rational ret(lhs);
	ret += rhs;
	return ret;
}

Rational operator-(const Rational& lhs, const Rational& rhs)
{
	Rational ret(lhs);
	ret -= rhs;
	return ret;
}

Rational operator*(const Rational& lhs, const Rational& rhs)
{
	Rational ret(lhs);
	ret *= rhs;
	return ret;
}

Rational operator/(const Rational& lhs, const Rational& rhs)
{
	Rational ret(lhs);
	ret /= rhs;
	return ret;
}
