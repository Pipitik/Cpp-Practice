#ifndef STRING_H
#define STRING_H

#include <algorithm>
#include <compare>
#include <iostream>

class String
{
public:
	const static size_t npos = -1;

	String();
	String(const String& other);
	String(const char* str);
	String(size_t count, char c);

	~String();

	bool operator==(const String& str) const;
	std::weak_ordering operator<=>(const String& str) const;

	bool operator==(const char* str) const;
	std::weak_ordering operator<=>(const char* str) const;

	String& operator=(String other);
	String& operator+=(const String& other);
	String& operator+=(const char* str);
	String& operator+=(char c);

	char& operator[](size_t index);
	const char& operator[](size_t index) const;

	void swap(String& other);

	const char* data() const;
	char* data();

	size_t size() const;
	size_t length() const;

	bool empty() const;

	char& front();
	const char& front() const;

	char& back();
	const char& back() const;

	void push_back(char c);
	void pop_back();

	void clear();

	size_t find(const String& str, size_t pos = 0) const;
	size_t find(const char* str, size_t pos = 0) const;
	size_t find(char c, size_t pos = 0) const;

	size_t rfind(const String& str, size_t pos = npos) const;
	size_t rfind(const char* str, size_t pos = npos) const;
	size_t rfind(char c, size_t pos = npos) const;

	String substr(size_t pos = 0, size_t count = npos) const;

private:
	char*  m_data;
	size_t m_size;
	size_t m_capacity;

	String(size_t count);

	void rebuild();
	void add_char(char c);
};

std::istream& operator>>(std::istream& is, String& str);
std::ostream& operator<<(std::ostream& os, const String& str);
String operator+(const String& lhs, const String& rhs);
String operator+(const String& lhs, const char* rhs);
String operator+(const char* lhs, const String& rhs);
String operator+(const String& lhs, char rhs);
String operator+(char lhs, const String& rhs);
/*bool operator==(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
bool operator<=(const String& lhs, const String& rhs);
bool operator>=(const String& lhs, const String& rhs);*/

#endif
