#include <cstring>

#include "String.h"

size_t find_strrstr(const char* str, const char* substr, size_t pos)
{
	for (const char* pc = str + pos - strlen(substr); pc >= str; --pc) {
		if (strcmp(str, substr) == 0) {
			return pc - str;
		}
	}
   return String::npos;	
}

String::String(size_t count)
		: m_data(new char[count + 1])
		, m_size(count)
		, m_capacity(count + 1)
{
	m_data[m_size] = '\0';
}

String::String() : String(static_cast<size_t>(0)) {}

String::String(const String& other) : String(other.size())
{
	memcpy(m_data, other.m_data, m_size);
}

String::String(const char* str) : String(strlen(str))
{
	memcpy(m_data, str, m_size);
}

String::String(size_t count, char c) : String(count)
{
	memset(m_data, c, m_size);
}

String::~String() {	delete[] m_data; }

bool String::operator==(const String& str) const
{
	if (m_size != str.m_size) return false;
	
	for (size_t i = 0; i != m_size; ++i) {
		if ((*this)[i] != str[i]) return false;
	}
	return true;
}

std::weak_ordering String::operator<=>(const String& str) const
{
	int cmp = strcmp(m_data, str.m_data);

	if (cmp == -1) {
		return std::weak_ordering::less;
	}
	else if (cmp == 1) {
		return std::weak_ordering::greater;
	}
	else {
		return std::weak_ordering::equivalent;
	}
}


bool String::operator==(const char* str) const
{
	if (m_size != std::strlen(str)) return false;
	
	for (size_t i = 0; i != m_size; ++i) {
		if ((*this)[i] != str[i]) return false;
	}
	return true;
}

std::weak_ordering String::operator<=>(const char* str) const
{
	int cmp = strcmp(m_data, str);

	if (cmp == -1) {
		return std::weak_ordering::less;
	}
	else if (cmp == 1) {
		return std::weak_ordering::greater;
	}
	else {
		return std::weak_ordering::equivalent;
	}
}

String& String::operator=(String other)
{
	swap(other);
	return *this;
}

String& String::operator+=(const String& other)
{
	if (m_capacity < other.m_capacity) {
		m_capacity = other.m_capacity;
		rebuild();
	}

	for (size_t i = 0; i != other.m_size; ++i) {
		add_char(other[i]);
	}
	m_data[m_size] = '\0';

	return *this;	
}

String& String::operator+=(const char* str)
{
	size_t length = strlen(str);
	for (size_t i = 0; i != length; ++i) {
		add_char(str[i]);
	}
	m_data[m_size] = '\0';
	return *this;
}

String& String::operator+=(char c)
{
	push_back(c);
	return *this;
}

char& String::operator[](size_t index) { return m_data[index]; }

const char& String::operator[](size_t index) const { return m_data[index]; }

void String::swap(String& other)
{
	std::swap(m_data, other.m_data);
	std::swap(m_size, other.m_size);
	std::swap(m_capacity, other.m_capacity);
}

const char* String::data() const { return m_data; }

char* String::data() { return m_data; }

size_t String::size() const { return m_size; }

size_t String::length() const {	return m_size; }

bool String::empty() const { return m_size == 0; }

char& String::front() {	return m_data[0]; }

const char& String::front() const {	return m_data[0]; }

char& String::back() { return m_data[m_size - 1]; }

const char& String::back() const { return m_data[m_size - 1]; }

void String::push_back(char c) 
{
	add_char(c);
	m_data[m_size] = '\0';
}

void String::pop_back()
{
	--m_size;
	m_data[m_size] = '\0';
}

void String::clear() { m_size = 0; }

size_t String::find(const String& str, size_t pos) const
{
	char* pOccurrence = strstr(m_data + pos, str.m_data); 
	return pOccurrence == nullptr ? npos : pOccurrence - m_data;
}

size_t String::find(const char* str, size_t pos) const
{
	char* pOccurrence = strstr(m_data + pos, str); 
	return pOccurrence == nullptr ? npos : pOccurrence - m_data;
}

size_t String::find(char c, size_t pos) const
{
	char* pOccurrence = strchr(m_data + pos, c); 
	return pOccurrence == nullptr ? npos : pOccurrence - m_data;
}

size_t String::rfind(const String& str, size_t pos) const { return find_strrstr(m_data, str.m_data, (pos > m_size) ? pos : m_size);  }

size_t String::rfind(const char* str, size_t pos) const { return find_strrstr(m_data + pos, str, (pos > m_size) ? pos : m_size);  }

size_t String::rfind(char c, size_t pos) const
{
	for (size_t i = m_size - pos; i != 0; --i) {
		if (m_data[i - 1] == c) {
			return i - 1;
		}
	}
	return npos;
}

String String::substr(size_t pos, size_t count) const
{
	String substring(count > m_size - pos ? m_size - pos : count);
	memcpy(substring.m_data, m_data + pos, substring.m_size);
	return substring;
}

void String::rebuild()
{
	char* temp_data = new char[m_capacity];
	memcpy(temp_data, m_data, m_size);
	std::swap(m_data, temp_data);
	delete[] temp_data;
}

void String::add_char(char c)
{
	if (m_size + 1 == m_capacity) {
		m_capacity *= 2;
		rebuild();
	}
	m_data[m_size] = c;
	++m_size;
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
	for (size_t i = 0; i != str.size(); ++i) {
		os << str[i];
	}
	return os;
}

std::istream& operator>>(std::istream& is, String& str)
{
	str.clear();
	char c;
	while (is >> c && !is.eof() && !std::isspace(c)) {
		str.push_back(c);
	}
	return is;
}

String operator+(const String& lhs, const String& rhs)
{
	String result(lhs);
	result += rhs;
	return result;
}

String operator+(const String& lhs, const char* rhs)
{
	String result(lhs);
	result += rhs;
	return result;
}

String operator+(const char* lhs, const String& rhs)
{
	String result(lhs);
	result += rhs;
	return result;
}

String operator+(const String& lhs, char rhs)
{
	String result(lhs);
	result += rhs;
	return result;
}

String operator+(char lhs, const String& rhs)
{
	String result(1, lhs);
	result += rhs;
	return result;
}

/*bool operator!=(const String& lhs, const String& rhs)
{
	return !(lhs == rhs);
}

bool operator<(const String& lhs, const String& rhs)
{
	const char* begin_lhs = lhs.data();
	const char*	begin_rhs = rhs.data();
	const char* end_lhs   = lhs.data() + lhs.size();
	const char* end_rhs   = rhs.data() + rhs.size();

	for ( ; (begin_lhs != end_lhs) && (begin_rhs != end_rhs); ++begin_lhs, ++begin_rhs) {
		if (*begin_lhs < *begin_rhs) return true;
		
		if (*begin_lhs > *begin_rhs) return false;	
	}
	return lhs.size() < rhs.size(); 
}

bool operator>(const String& lhs, const String& rhs)
{
	return rhs < lhs;
}

bool operator<=(const String& lhs, const String& rhs)
{
	return lhs < rhs || lhs == rhs;
}

bool operator>=(const String& lhs, const String& rhs)
{
	return lhs > rhs || lhs == rhs;
}*/
