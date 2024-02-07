#ifndef BIG_INT_H
#define BIG_INT_H

#include <iostream>
#include <string>
#include <vector>

class Big_int
{
	friend std::ostream& operator<<(std::ostream& os, const Big_int& bi);
	friend std::istream& operator>>(std::istream& is, Big_int& bi);
public:

	Big_int();
	Big_int(const Big_int& bi) = default;
	Big_int& operator=(const Big_int& bi) = default;
	Big_int(long long number);
	explicit Big_int(const std::string& str);

	Big_int& operator+=(const Big_int& rhs);
	Big_int& operator-=(const Big_int& rhs);
	Big_int& operator*=(const Big_int& rhs);
	Big_int& operator/=(const Big_int& rhs);
	Big_int& operator%=(const Big_int& rhs);

	[[nodiscard]] Big_int operator+() const;
	[[nodiscard]] Big_int operator-() const;

	Big_int& operator++();
	Big_int operator++(int);

	Big_int& operator--();
	Big_int operator--(int);

	bool operator==(const Big_int& rhs) const;
	std::weak_ordering operator<=>(const Big_int& rhs) const;

	[[nodiscard]] std::string to_string() const;
	void swap(Big_int& other);
	Big_int& negate();

	explicit operator bool() const;
	explicit operator int() const;

private:
	using base_type = unsigned int;
	using double_base_type = unsigned long long;
	using container_type = std::vector<base_type>;
	using size_type = container_type::size_type;
	static constexpr base_type _BASE = 1'000'000'000;
	static constexpr unsigned char _COUNT_ZEROS = 9;

	bool _sign;
	container_type _data;

	static void _delete_leading_zeros(container_type& _data);
	static void _reverse(container_type& _data);
	static int _count_digits(base_type _number);

	/// Similar to strcmp from C.
	/// Return value:
	/// Negative value if _lhs_data < _rhs_data
	/// Zero if _lhs_data == _rhs_data
	/// Positive value if _lhs_data > _rhs_data
	static int _veccmp(const container_type& _lhs_data, const container_type& _rhs_data);

	static container_type _multiply_data(const container_type& _lhs_data, const container_type& _rhs_data);
	static container_type _divide_data(const container_type& _lhs_data, const container_type& _rhs_data);
	static container_type _take_remainder_data(const container_type& _lhs_data, const container_type& _rhs_data);

	/// Assign _number to _data[_pos].
	/// If ( _pos < _data.size() ) then ( _data.push_back(_number) )
	void _assign_number(size_type _pos, base_type _number);

	/// Similar to (_data += _rhs_data)
	void _add(const container_type& _rhs_data);

	/// Similar to (_data = _lhs_data - _rhs_data)
	void _difference(const container_type& _lhs_data, const container_type& _rhs_data);

	void _zeroing();
};

Big_int operator""_bi(unsigned long long num);
Big_int operator""_bi(const char* str, size_t len);

Big_int operator+(const Big_int& lhs, const Big_int& rhs);
Big_int operator-(const Big_int& lhs, const Big_int& rhs);
Big_int operator*(const Big_int& lhs, const Big_int& rhs);
Big_int operator/(const Big_int& lhs, const Big_int& rhs);
Big_int operator%(const Big_int& lhs, const Big_int& rhs);

[[nodiscard]] Big_int gcd(Big_int m, Big_int n);

#endif
