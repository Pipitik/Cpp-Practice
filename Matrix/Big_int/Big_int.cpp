#include "Big_int.h"

//++++++++++++++++++++Support functions+++++++++++++++++++++++++++
//--------------------Static functions----------------------------
void Big_int::_delete_leading_zeros(container_type& _data)
{
	while (!_data.empty() and _data.back() == 0) {
		_data.pop_back();
	}
}

void Big_int::_reverse(container_type& _data)
{
	for (size_type i = 0; i < _data.size() / 2; ++i) {
		size_type reverse_i = _data.size() - i - 1;
		size_type temp = _data[i];
		_data[i] = _data[reverse_i];
		_data[reverse_i] = temp;
	}
}

int Big_int::_count_digits(base_type _number)
{
	int ret = 1;
	while ((_number /= 10) != 0) {
		++ret;
	}
	return ret;
}

int Big_int::_veccmp(const container_type& _lhs_data, const container_type& _rhs_data)
{
	if (_lhs_data.size() > _rhs_data.size()) {
		return 1;
	}
	else if (_lhs_data.size() < _rhs_data.size()) {
		return -1;
	}
	for (size_type i = _lhs_data.size(); i != 0; --i) {
		size_type reverse_i = i - 1;
		if (_lhs_data[reverse_i] > _rhs_data[reverse_i]) {
			return 1;
		}
		else if (_lhs_data[reverse_i] < _rhs_data[reverse_i]) {
			return -1;
		}
	}
	return 0;
}

Big_int::container_type
Big_int::_multiply_data(const container_type& _lhs_data,
						const container_type& _rhs_data)
{
	container_type result(_lhs_data.size() + _rhs_data.size(), 0);
	for (size_type i = 0; i < _lhs_data.size(); ++i) {
		double_base_type carry = 0;
		for (size_type j = 0; j < _rhs_data.size(); ++j) {
			double_base_type mul =
				static_cast<double_base_type>(_lhs_data[i]) * _rhs_data[j] + carry;
			carry = (result[i + j] + mul) / _BASE;
			result[i + j] = static_cast<base_type>((result[i + j] + mul) % _BASE);
		}
		if (carry > 0) {
			result[i + _rhs_data.size()] = carry;
		}
	}
	_delete_leading_zeros(result);
	return result;
}

Big_int::container_type
Big_int::_divide_data(	const container_type& _lhs_data,
						const container_type& _rhs_data)
{
	container_type ret;
	Big_int cur_val;
	for (size_type i = _lhs_data.size(); i != 0; --i) {
		size_type reverse_i = i - 1;
		cur_val = cur_val * _BASE + _lhs_data[reverse_i];
		base_type quotient = 0;
		base_type l = 0;
		base_type r = _BASE;
		while (l <= r) {
			base_type mid = (l + r) / 2;
			container_type temp = _multiply_data(_rhs_data, Big_int(mid)._data);
			int cmp = _veccmp(cur_val._data, temp);
			if (cmp == 1 or cmp == 0) {
				quotient = mid;
				l = mid + 1;
			}
			else {
				r = mid - 1;
			}
		}
		ret.push_back(quotient);
		Big_int incomplete_divisible;
		incomplete_divisible._data = _multiply_data(_rhs_data, Big_int(quotient)._data);
		cur_val -= incomplete_divisible;
	}
	_reverse(ret);
	_delete_leading_zeros(ret);
	return ret;
}

Big_int::container_type
Big_int::_take_remainder_data(	const container_type& _lhs_data,
								const container_type& _rhs_data)
{
	Big_int ret;
	for (size_type i = _lhs_data.size(); i != 0; --i) {
		size_type reverse_i = i - 1;
		ret = ret * _BASE + _lhs_data[reverse_i];
		base_type quotient = 0;
		base_type l = 0;
		base_type r = _BASE;
		while (l <= r) {
			base_type mid = (l + r) / 2;
			container_type temp = _multiply_data(_rhs_data, Big_int(mid)._data);
			int cmp = _veccmp(ret._data, temp);
			if (cmp == 1 or cmp == 0) {
				quotient = mid;
				l = mid + 1;
			}
			else {
				r = mid - 1;
			}
		}
		Big_int incomplete_divisible;
		incomplete_divisible._data = _multiply_data(_rhs_data, Big_int(quotient)._data);
		ret -= incomplete_divisible;
	}
	return ret._data;
}
//----------------------------------------------------------------

//--------------------Non-static functions------------------------
void Big_int::_assign_number(size_type _pos, base_type _number)
{
	if (_pos < _data.size()) {
		_data[_pos] = _number;
	}
	else {
		_data.push_back(_number);
	}
}

void Big_int::_add(const container_type& _rhs_data)
{
	base_type carry = 0;
	size_type max_size =
		_data.size() < _rhs_data.size() ?
		_data.size() :
		_rhs_data.size();
	for (size_type i = 0; i < max_size; ++i) {
		base_type sum = 0;
		if (i < _data.size()) {
			sum += _data[i];
		}
		if (i < _rhs_data.size()) {
			sum += _rhs_data[i];
		}
		sum += carry;

		_assign_number(i, sum % _BASE);
		carry = sum / _BASE;
	}
	if (carry > 0) {
		_data.push_back(carry);
	}
}

void Big_int::_difference(const container_type& _lhs_data, const container_type& _rhs_data)
{
	base_type borrowed = 0;
	for (size_type i = 0; i < _lhs_data.size(); ++i) {
		base_type diff = _lhs_data[i];
		base_type rhs_num = i < _rhs_data.size() ? _rhs_data[i] : 0;
		if (diff > rhs_num) {
			diff -= rhs_num + borrowed;
			borrowed = 0;
		}
		else if (diff < rhs_num) {
			diff += _BASE - (rhs_num + borrowed);
			borrowed = 1;
		}
		else {
			if (borrowed) {
				diff = _BASE;
			}
			diff -= rhs_num + borrowed;
		}
		_assign_number(i, diff);
	}
	_delete_leading_zeros(_data);
}

void Big_int::_zeroing()
{
	_sign = false;
	_data.clear();
}
//----------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

std::ostream& operator<<(std::ostream& os, const Big_int& bi)
{
	/*using size_type = Big_int::size_type;
	if (bi._sign) {
		os << '-';
	}

	if (bi) {
		os << bi._data.back();
		for (size_type i = bi._data.size() - 1; i != 0; --i) {
			size_t count_zeros = Big_int::_COUNT_ZEROS - Big_int::_count_digits(bi._data[i - 1]);
			for (size_t i = 0; i < count_zeros; ++i) { os << 0; }
			os << bi._data[i - 1];
		}
	}
	else {
		std::cout << 0;
	}*/
	os << bi.to_string();
	return os;
}

std::istream& operator>>(std::istream& is, Big_int& bi)
{
	std::string str_number;
	std::cin >> str_number;
	bi = Big_int(str_number);
	return is;
}

Big_int::Big_int()
	: _sign(false)
	, _data() {}

Big_int::Big_int(long long number) : _sign(number < 0 ? true : false)
{
	if (number != 0) {

		/*if (_sign) {
			number = -number;
		}*/ //Don't work because if (number = LLONG_MIN) then (-number == LLONG_MIN)

		do {
			base_type temp = (number % _BASE) * (_sign ? -1 : 1);
			_data.push_back(temp);
		} while ((number /= _BASE) != 0);
	}
}

Big_int::Big_int(const std::string& str) : _sign(str.front() == '-' ? true : false)
{
	using str_size = std::string::size_type;
	str_size shift = _sign or str.front() == '+';
	while (str[shift] == '0') {
		++shift;
	}

	if (shift == str.size()) {
		_sign = false; // Zero cannot be negative
	}
	else {
		str_size len_past_num = (str.size() - shift) % _COUNT_ZEROS;
		str_size len_part_str = (str.size() - shift) / _COUNT_ZEROS;
		for (str_size i = 0; i < len_part_str; ++i) {
			str_size reverse_i = str.size() - (i + 1) * _COUNT_ZEROS;
			_data.push_back(std::stoul(str.substr(reverse_i, _COUNT_ZEROS)));
		}
		if (len_past_num != 0) {
			_data.push_back(std::stoul(str.substr(shift, len_past_num)));
		}
	}
}

Big_int& Big_int::operator+=(const Big_int& rhs)
{
	if (!rhs) {
		return *this;
	}
	else if (!(*this)) {
		*this = rhs;
	}
	else if (_sign != rhs._sign) {
		_sign = !_sign;
		*this -= rhs;
		_sign = _data.empty() ? _sign : !_sign; // If _data == rhs._data, else return to the original state _sign
	}
	else {
		_add(rhs._data);
	}
	return *this;
}

Big_int& Big_int::operator-=(const Big_int& rhs)
{
	if (!rhs) {
		return *this;
	}
	else if (!(*this)) {
		*this = rhs;
		_sign = !_sign;
	}
	else if (_sign != rhs._sign) {
		_sign = !_sign;
		*this += rhs;
		_sign = !_sign;
	}
	else {
		int cmp = _veccmp(_data, rhs._data);
		if (cmp == -1) {
			_difference(rhs._data, _data);
			_sign = !_sign;
		}
		else if (cmp == 1) {
			_difference(_data, rhs._data);
		}
		else {
			_zeroing();
		}
	}
	return *this;
}

Big_int& Big_int::operator*=(const Big_int& rhs)
{
	if (!rhs) {
		_zeroing();
	}
	else if (*this) {
		_sign = _sign != rhs._sign;
		_data = _multiply_data(_data, rhs._data);
	}
	return *this;
}

Big_int& Big_int::operator/=(const Big_int& rhs)
{
	if (!rhs) {
		throw "Division by zero";
	}
	else if (_veccmp(_data, rhs._data) == -1) {
		_zeroing();
	}
	else if (*this) {
		_sign = _sign != rhs._sign;
		_data = _divide_data(_data, rhs._data);
	}
	return *this;
}

Big_int& Big_int::operator%=(const Big_int& rhs)
{
	if (!rhs) {
		throw "Division by zero";
	}
	else if (*this) {
		container_type remainder = _take_remainder_data(_data, rhs._data);
		if (remainder.empty()) {
			_zeroing();
		}
		else {
			_data = remainder;
		}
	}
	return *this;
}

Big_int Big_int::operator+() const
{
	return *this;
}

Big_int Big_int::operator-() const
{
	Big_int ret(*this);
	ret.negate();
	return ret;
}

Big_int& Big_int::operator++()
{
	return *this += 1;
}

Big_int Big_int::operator++(int)
{
	Big_int ret(*this);
	*this += 1;
	return ret;
}

Big_int& Big_int::operator--()
{
	return *this -= 1;
}

Big_int Big_int::operator--(int)
{
	Big_int ret(*this);
	*this -= 1;
	return ret;
}

bool Big_int::operator==(const Big_int& rhs) const
{
	return (_sign == rhs._sign) and
		_veccmp(_data, rhs._data) == 0;
}

std::weak_ordering Big_int::operator<=>(const Big_int& rhs) const
{
	bool lhs_is_negative = _sign > rhs._sign;
	bool rhs_is_negative = _sign < rhs._sign;
	bool are_positive = !_sign and !rhs._sign;
	bool are_negative = _sign and rhs._sign;

	bool lhs_is_less = lhs_is_negative or
		(are_positive and (_veccmp(_data, rhs._data) == -1)) or
		(are_negative and (_veccmp(_data, rhs._data) == 1));

	bool lhs_is_greater = rhs_is_negative or
		(are_positive and (_veccmp(_data, rhs._data) == 1)) or
		(are_negative and (_veccmp(_data, rhs._data) == -1));

	if (lhs_is_less) {
		return std::weak_ordering::less;
	}
	else if (lhs_is_greater) {
		return std::weak_ordering::greater;
	}
	else {
		return std::weak_ordering::equivalent;
	}
}

std::string Big_int::to_string() const
{
	std::string ret = _data.empty() ?
		"0" :
		(_sign ? "-" : "") + std::to_string(_data.back());

	for (size_type i = _data.empty() ? 0 : _data.size() - 1; i != 0; --i) {
		size_type reverse_i = i - 1;
		size_t count_zeros = _COUNT_ZEROS - _count_digits(_data[reverse_i]);
		if (count_zeros == 0) {
			ret += std::to_string(_data[reverse_i]);
		}
		else {
			ret += std::string(count_zeros, '0') + std::to_string(_data[reverse_i]);
		}
	}
	return ret;
}

void Big_int::swap(Big_int& other)
{
	bool temp = _sign;
	_sign = other._sign;
	other._sign = temp;
	std::swap(_data, other._data);
}

Big_int& Big_int::negate()
{
	if (*this) {
		_sign = !_sign;
	}
	return *this;
}

Big_int::operator bool() const
{
	return !_data.empty();
}

Big_int::operator int() const
{
	if (_data.empty()) {
		return 0;
	}
	int ret = _sign ? -static_cast<int>(_data.front()) : _data.front();
	for (size_type i = 1; i < _data.size(); ++i) {
		int num = _BASE;
		for (size_type j = 1; j < i; ++j) {
			num *= _BASE;
		}
		if (_sign) {
			ret -= num * _data[i];
		}
		else {
			ret += num * _data[i];
		}
	}
	return ret;
}

Big_int operator""_bi(unsigned long long num)
{
	return Big_int(static_cast<long long>(num));
}

Big_int operator""_bi(const char* str, size_t len)
{
	return Big_int(std::string(str, len));
}

Big_int operator+(const Big_int& lhs, const Big_int& rhs)
{
	Big_int ret(lhs);
	ret += rhs;
	return ret;
}

Big_int operator-(const Big_int& lhs, const Big_int& rhs)
{
	Big_int ret(lhs);
	ret -= rhs;
	return ret;
}

Big_int operator*(const Big_int& lhs, const Big_int& rhs)
{
	Big_int ret(lhs);
	ret *= rhs;
	return ret;
}

Big_int operator/(const Big_int& lhs, const Big_int& rhs)
{
	Big_int ret(lhs);
	ret /= rhs;
	return ret;
}

Big_int operator%(const Big_int& lhs, const Big_int& rhs)
{
	Big_int ret(lhs);
	ret %= rhs;
	return ret;
}

Big_int gcd(Big_int m, Big_int n)
{
	if (!m and !n) {
		return 0;
	}
	else if (!m) {
		return n;
	}
	else if (!n) {
		return m;
	}

	while (m and n) {
		if (m > n) {
			m %= n;
		}
		else {
			n %= m;
		}
	}
	return m + n;
}
