#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <string>

#include "gtest/gtest.h"

#include "../Big_int.h"
#include "../Big_int.cpp"
#include "../rational.h"
#include "../rational.cpp"

// class Big_int

TEST(BigintegerTest, from_int)
{
	std::vector<int> values =
	{
		0, 999'999'999, 1'000'000'000, 1'000'010'090, 2'000'000'000, 2'000'010'090,
		-1, -999'999'999,  -1'000'000'000, -1'000'010'090, -2'000'000'000, -2'000'010'090
	};

	for (auto val : values)
	{
		EXPECT_EQ(std::to_string(val), Big_int(val).to_string());
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int64_t> dist(std::numeric_limits<int64_t>::min());

	for (size_t i = 0; i < 1'000'000; ++i)
	{
		int64_t val = dist(gen);
		EXPECT_EQ(std::to_string(val), Big_int(val).to_string());
	}
}

TEST(BigintegerTest, two_plus_two)
{
	EXPECT_EQ(Big_int(4), Big_int(2) + Big_int(2));
	EXPECT_EQ(4, Big_int(2) + 2); // implicit conversion from int must work
	EXPECT_EQ(4, 2 + Big_int(2));
}

TEST(BigintegerTest, default_ctor)
{
	Big_int a;
	Big_int b = 0;
	EXPECT_EQ(0, a);
	EXPECT_EQ(b, a);
}

TEST(BigintegerTest, ctor_limits)
{
	Big_int a = std::numeric_limits<int>::min();
	Big_int b = std::numeric_limits<int>::max();
	EXPECT_EQ(-1, a + b);
}

TEST(BigintegerTest, copy_ctor)
{
	Big_int a = 3;
	Big_int b = a;
	EXPECT_EQ(b, a);
	EXPECT_EQ(3, b);
}

TEST(BigintegerTest, copy_ctor_real_copy)
{
	Big_int a = 3;
	Big_int b = a;
	EXPECT_EQ(3, b);
}

TEST(BigintegerTest, copy_ctor_real_copy2)
{
	Big_int a = 3;
	Big_int b = a;
	b = 5;

	EXPECT_EQ(3, a);
}

TEST(BigintegerTest, assignment_operator)
{
	Big_int a = 4;
	Big_int b = 7;
	b = a;

	EXPECT_TRUE(a == b);
}

TEST(BigintegerTest, self_assignment)
{
	Big_int a = 5;
	a = a;
	Big_int& b = a;

	EXPECT_TRUE(a == 5);
	EXPECT_TRUE(a == a);
	EXPECT_TRUE(a == b);
	EXPECT_TRUE(b == a);
}

TEST(BigintegerTest, assignment_return_value)
{
	Big_int a = 4;
	Big_int b = 7;
	(a = b) = a;

	EXPECT_TRUE(a == 7);
	EXPECT_TRUE(b == 7);
}

TEST(BigintegerTest, comparisons)
{
	Big_int a("1000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000");
	Big_int b("1000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000");
	Big_int c("1000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000010000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000");
	Big_int d("-1000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000"
		"000000000000000000000000000000000000000000000000000000000000000");

	EXPECT_TRUE(a == b);
	EXPECT_TRUE(a != c);
	EXPECT_TRUE(a < c);
	EXPECT_TRUE(c > a);
	EXPECT_TRUE(a <= a);
	EXPECT_TRUE(a <= b);
	EXPECT_TRUE(a <= c);
	EXPECT_TRUE(c >= a);
	EXPECT_TRUE(d != 0);
}

TEST(BigintegerTest, compare_zero_and_minus_zero)
{
	Big_int a;
	Big_int b = -a;

	EXPECT_TRUE(a == b);
}

TEST(BigintegerTest, add)
{
	Big_int a = 5;
	Big_int b = 20;

	EXPECT_TRUE(a + b == 25);

	a += b;
	EXPECT_TRUE(a == 25);
}

TEST(BigintegerTest, add_signed)
{
	Big_int a = 5;
	Big_int b = -20;
	EXPECT_TRUE(a + b == -15);

	a += b;
	EXPECT_TRUE(a == -15);
}

TEST(BigintegerTest, add_return_value)
{
	Big_int a = 5;
	Big_int b = 1;

	(a += b) += b;
	EXPECT_EQ(7, a);
}

TEST(BigintegerTest, sub)
{
	Big_int a = 20;
	Big_int b = 5;

	EXPECT_TRUE(a - b == 15);

	a -= b;
	EXPECT_TRUE(a == 15);
}

TEST(BigintegerTest, sub_signed)
{
	Big_int a = 5;
	Big_int b = 20;

	EXPECT_TRUE(a - b == -15);

	a -= b;
	EXPECT_TRUE(a == -15);

	a -= -100;
	EXPECT_TRUE(a == 85);
}

TEST(BigintegerTest, sub_return_value)
{
	Big_int a = 5;
	Big_int b = 1;

	(a -= b) -= b;
	EXPECT_EQ(3, a);
}

TEST(BigintegerTest, mul)
{
	Big_int a = 5;
	Big_int b = 20;
	EXPECT_TRUE(a * b == 100);

	a *= b;
	EXPECT_TRUE(a == 100);
}

TEST(BigintegerTest, mul_signed)
{
	Big_int a = -5;
	Big_int b = 20;

	EXPECT_TRUE(a * b == -100);

	a *= b;
	EXPECT_TRUE(a == -100);
}

TEST(BigintegerTest, mul_return_value)
{
	Big_int a = 5;
	Big_int b = 2;

	(a *= b) *= b;
	EXPECT_EQ(20, a);
}

TEST(BigintegerTest, div_)
{
	Big_int a = 20;
	Big_int b = 5;
	Big_int c = 20;
	EXPECT_EQ(0, b / c);
	EXPECT_TRUE(a / b == 4);
	EXPECT_TRUE(a % b == 0);

	a /= b;
	EXPECT_TRUE(a == 4);

	c %= b;
	EXPECT_TRUE(c == 0);
}

TEST(BigintegerTest, div_int_min)
{
	Big_int a = std::numeric_limits<int>::min();
	EXPECT_TRUE((a / a) == (a / std::numeric_limits<int>::min()));
}

TEST(BigintegerTest, div_int_min_2)
{
	Big_int a = std::numeric_limits<int>::min();
	Big_int b = -1;
	Big_int c = a / b;
	EXPECT_TRUE(c == (a / -1));
	EXPECT_TRUE((c - std::numeric_limits<int>::max()) == 1);
}

TEST(BigintegerTest, div_signed)
{
	Big_int a = -20;
	Big_int b = 5;

	EXPECT_TRUE(a / b == -4);
	EXPECT_TRUE(a % b == 0);
}

TEST(BigintegerTest, div_rounding)
{
	Big_int a = 23;
	Big_int b = 5;

	EXPECT_TRUE(a / b == 4);
	EXPECT_TRUE(a % b == 3);
}

TEST(BigintegerTest, div_rounding_negative)
{
	Big_int a = 23;
	Big_int b = -5;
	Big_int c = -23;
	Big_int d = 5;

	EXPECT_TRUE(a / b == -4);
	EXPECT_TRUE(c / d == -4);
	EXPECT_TRUE(a % b == 3);
	EXPECT_TRUE(c % d == -3);
}

TEST(BigintegerTest, div_return_value)
{
	Big_int a = 100;
	Big_int b = 2;

	(a /= b) /= b;
	EXPECT_EQ(25, a);
}

TEST(BigintegerTest, unary_plus)
{
	Big_int a = 123;
	Big_int b = +a;

	EXPECT_TRUE(a == b);

	// this code should not compile:
	// &+a;
}

TEST(BigintegerTest, negation)
{
	Big_int a = 666;
	Big_int b = -a;

	EXPECT_TRUE(b == -666);
}

TEST(BigintegerTest, negation_int_min)
{
	Big_int a = std::numeric_limits<int>::min();
	Big_int b = -a;

	EXPECT_EQ(b - 1, std::numeric_limits<int>::max());
}

TEST(BigintegerTest, increment)
{
	Big_int a = 42;
	Big_int pre = ++a;
	Big_int post = a++;

	EXPECT_EQ(43, pre);
	EXPECT_EQ(43, post);
}

TEST(BigintegerTest, decrement)
{
	Big_int a = 42;
	Big_int pre = --a;
	Big_int post = a--;

	EXPECT_EQ(41, pre);
	EXPECT_EQ(41, post);
}

TEST(BigintegerTest, add_long)
{
	Big_int a("10000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000");
	Big_int b("100000000000000000000000000000000000000");
	Big_int c("10000000000000000000000000000000000000000000000000000100000000"
			"000000000000000000000000000000");

	EXPECT_EQ(c, a + b);
}

TEST(BigintegerTest, add_long_signed)
{
	Big_int a("-1000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000");
	Big_int b("10000000000000000000000000000000000000000000000000000000000000"
			"00000000000000000000000000000");

	EXPECT_EQ(0, a + b);
}

TEST(BigintegerTest, add_long_signed2)
{
	Big_int a("-1000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000");
	Big_int b("100000000000000000000000000000000000000");
	Big_int c("-9999999999999999999999999999999999999999999999999999000000000"
			"00000000000000000000000000000");

	EXPECT_EQ(c, a + b);
}

TEST(BigintegerTest, add_long_pow2)
{
	Big_int a("18446744073709551616");
	Big_int b("-18446744073709551616");
	Big_int c("36893488147419103232");

	EXPECT_EQ(c, a + a);
	EXPECT_EQ(a, b + c);
	EXPECT_EQ(a, c + b);
}

TEST(BigintegerTest, sub_long)
{
	Big_int a("10000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000");
	Big_int b("100000000000000000000000000000000000000");
	Big_int c("99999999999999999999999999999999999999999999999999999000000000"
			"00000000000000000000000000000");

	EXPECT_EQ(c, a - b);
}

TEST(BigintegerTest, sub_long_pow2)
{
	Big_int a("36893488147419103232");
	Big_int b("36893488147419103231");

	EXPECT_EQ(1, a - b);
}

TEST(BigintegerTest, mul_long)
{
	Big_int a("10000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000");
	Big_int b("100000000000000000000000000000000000000");
	Big_int c("10000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000"
			"00000000000000000000000000000000000000");

	EXPECT_EQ(c, a * b);
}

TEST(BigintegerTest, mul_long_signed)
{
	Big_int a("-1000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000");
	Big_int b("100000000000000000000000000000000000000");
	Big_int c("-1000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000"
			"00000000000000000000000000000000000000");

	EXPECT_EQ(c, a * b);
}

TEST(BigintegerTest, mul_long_signed2)
{
	Big_int a("-100000000000000000000000000");
	Big_int c("100000000000000000000000000"
			"00000000000000000000000000");

	EXPECT_EQ(c, a * a);
}

TEST(BigintegerTest, mul_long_pow2)
{
	Big_int a("18446744073709551616");
	Big_int b("340282366920938463463374607431768211456");
	Big_int c("11579208923731619542357098500868790785326998466564056403945758"
			"4007913129639936");

	EXPECT_EQ(b, a * a);
	EXPECT_EQ(c, b * b);
}

TEST(BigintegerTest, div_0_long)
{
	Big_int a;
	Big_int b("100000000000000000000000000000000000000000000000000000000000");
	EXPECT_EQ(a, a / b);
}

TEST(BigintegerTest, div_long)
{
	Big_int a("10000000000000000000000000000000000000000000000000000000000000"
			"000000000000000000000000000000");
	Big_int b("100000000000000000000000000000000000000");
	Big_int c("100000000000000000000000000000000000000000000000000000");

	EXPECT_EQ(c, a / b);
}

TEST(BigintegerTest, div_long_signed)
{
	Big_int a("-1000000000000000000000000000000000000000000000000000000000000"
			"0000000000000000000000000000000");
	Big_int b("100000000000000000000000000000000000000");
	Big_int c("-100000000000000000000000000000000000000000000000000000");

	EXPECT_EQ(c, a / b);
}

TEST(BigintegerTest, div_long_signed2)
{
	Big_int a("-1000000000000000000000000000000000000000000000000000000000000"
				"0000000000000000000000000000000");
	Big_int b("-100000000000000000000000000000000000000");
	Big_int c("100000000000000000000000000000000000000000000000000000");

	EXPECT_EQ(c, a / b);
}

TEST(BigintegerTest, negation_long)
{
	Big_int a("10000000000000000000000000000000000000000000000000000");
	Big_int c("-10000000000000000000000000000000000000000000000000000");

	EXPECT_EQ(-a, c);
	EXPECT_EQ(a, -c);
	EXPECT_EQ((-a).to_string(), "-10000000000000000000000000000000000000000000000000000");
	EXPECT_EQ((-c).to_string(), "10000000000000000000000000000000000000000000000000000");
}

TEST(BigintegerTest, string_conv)
{
	EXPECT_EQ("100", Big_int("100").to_string());
	EXPECT_EQ("100", Big_int("0100").to_string());
	EXPECT_EQ("0", Big_int("0").to_string());
	EXPECT_EQ("-1000000000000000", Big_int("-1000000000000000").to_string());

	Big_int lim = std::numeric_limits<int32_t>::max();
	EXPECT_EQ("2147483647", lim.to_string());
	lim++;
	EXPECT_EQ("2147483648", lim.to_string());
	lim = std::numeric_limits<int32_t>::min();
	EXPECT_EQ("-2147483648", lim.to_string());
	lim--;
	EXPECT_EQ("-2147483649", lim.to_string());
}

template <typename T>
void test_converting_ctor(T value)
{
	using std::to_string;

	Big_int bi = value;
	EXPECT_EQ(to_string(value), bi.to_string());
}

TEST(BigintegerTest, converting_ctor)
{
	using std::numeric_limits;

	test_converting_ctor(numeric_limits<short>::min());
	test_converting_ctor(numeric_limits<short>::max());
	test_converting_ctor(numeric_limits<unsigned short>::min());
	test_converting_ctor(numeric_limits<unsigned short>::max());

	test_converting_ctor(numeric_limits<int>::min());
	test_converting_ctor(numeric_limits<int>::max());
	test_converting_ctor(numeric_limits<unsigned>::min());
	test_converting_ctor(numeric_limits<unsigned>::max());

	test_converting_ctor(numeric_limits<long>::min());
	test_converting_ctor(numeric_limits<long>::max());
   // test_converting_ctor(numeric_limits<unsigned long>::min());
   // test_converting_ctor(numeric_limits<unsigned long>::max());

	test_converting_ctor(numeric_limits<long long>::min());
	test_converting_ctor(numeric_limits<long long>::max());
	//test_converting_ctor(numeric_limits<unsigned long long>::min());
	//test_converting_ctor(numeric_limits<unsigned long long>::max());
}

TEST(BigintegerTest, converting_ctor2)
{
	Big_int a(1);
	Big_int b(1U);
	Big_int c(1L);
	Big_int d(1UL);
	Big_int e(1LL);
	Big_int f(1ULL);

	EXPECT_TRUE(a == b);
	EXPECT_TRUE(a == c);
	EXPECT_TRUE(a == d);
	EXPECT_TRUE(a == e);
	EXPECT_TRUE(a == f);
}

TEST(BigintegerTest, converting_ctor3)
{
	Big_int a(-1);
	Big_int b(-1L);
	Big_int c(-1LL);

	EXPECT_TRUE(a == b);
	EXPECT_TRUE(a == c);
}

TEST(BigintegerTest, converting_ctor4)
{
	long long num = static_cast<long long>(-1U) << 32;
	Big_int bignum(num);

	EXPECT_EQ(bignum.to_string(), std::to_string(num));
}

TEST(BigintegerTest, literals)
{
	EXPECT_EQ("0"_bi, 0_bi);
	EXPECT_EQ("-0"_bi, -0_bi);
	EXPECT_EQ("+00000000000000000000000000000000000000000000"_bi, 0_bi);
	EXPECT_EQ("2147483647"_bi, 2147483647_bi);
	EXPECT_EQ("-2147483647"_bi, -2147483647_bi);
	EXPECT_TRUE("1234567891234567891234567891234567891234567890"_bi > 123456789_bi);
	EXPECT_TRUE("-2147483647"_bi + 2147483647_bi == 0);
}

TEST(BigintegerTest, static_cast_int)
{
	Big_int a = std::numeric_limits<int>::min();
	Big_int b = std::numeric_limits<int>::max();
	Big_int c = a / 2;
	Big_int d = b / 2;

	int a1 = std::numeric_limits<int>::min();
	int b1 = std::numeric_limits<int>::max();
	int c1 = a1 / 2;
	int d1 = b1 / 2;

	EXPECT_EQ(static_cast<int>(a), a1);
	EXPECT_EQ(static_cast<int>(b), b1);
	EXPECT_EQ(static_cast<int>(c), c1);
	EXPECT_EQ(static_cast<int>(d), d1);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//class Rational

void test_creating_only_numerator(int i, const Rational& r)
{
	EXPECT_EQ(std::to_string(i), r.to_string());
	EXPECT_EQ(std::to_string(i), r.as_decimal());
	EXPECT_EQ(i, r.numerator());
	EXPECT_EQ(1, r.denominator());
}

TEST(RationalTest, creating_only_numerator)
{
	Rational a;
	Rational b(std::numeric_limits<int>::max());
	Rational c(std::numeric_limits<int>::min());
	Rational b1(std::numeric_limits<int>::max() / 2);
	Rational c1(std::numeric_limits<int>::min() / 2);

	test_creating_only_numerator(0, a);
	test_creating_only_numerator(std::numeric_limits<int>::max(), b);
	test_creating_only_numerator(std::numeric_limits<int>::min(), c);
	test_creating_only_numerator(std::numeric_limits<int>::max() / 2, b1);
	test_creating_only_numerator(std::numeric_limits<int>::min() / 2, c1);
}

TEST(RationalTest, denominator)
{
	std::vector<int> num =   { 353522978, 1, 17, 5, -47};
	std::vector<int> denom = { 398554817, 3, 13, 7, 121};
	for (std::vector<int>::size_type i = 0; i < num.size(); ++i) {
		std::string temp = std::to_string(num[i]) + "/" + std::to_string(denom[i]);
		EXPECT_EQ(temp, Rational(num[i], denom[i]).to_string());
	}
}

/*TEST(RationalTest, minus_zero_creating)
{
	for (int i = -1; i > -1'000 ; --i) {
		EXPECT_EQ("-0", Rational(0, i).to_string());
		EXPECT_EQ("-0", Rational(0, i).as_decimal());
		EXPECT_EQ("-0", Rational(0, i).as_decimal(-i));
	}
}

TEST(RationalTest, minus_zero_addition)
{
	Rational a(0, -1);
	Rational b(0, -123);
	EXPECT_EQ("-0", (a + b).to_string());
	EXPECT_EQ("-0", (b + a).to_string());
	EXPECT_EQ("-0", (a + b).as_decimal());
	EXPECT_EQ("-0", (b + a).as_decimal());

	Rational c;
	EXPECT_EQ("0", (a + c).to_string());
	EXPECT_EQ("0", (c + a).to_string());
	EXPECT_EQ("0", (a + c).as_decimal());
	EXPECT_EQ("0", (c + a).as_decimal());

	Rational d(1);
	EXPECT_EQ("1", (a + d).to_string());
	EXPECT_EQ("1", (d + a).to_string());
	EXPECT_EQ("1", (a + d).as_decimal());
	EXPECT_EQ("1", (d + a).as_decimal());

	Rational f(-1);
	EXPECT_EQ("-1", (a + f).to_string());
	EXPECT_EQ("-1", (f + a).to_string());
	EXPECT_EQ("-1", (a + f).as_decimal());
	EXPECT_EQ("-1", (f + a).as_decimal());
}

TEST(RationalTest, minus_zero_difference)
{
	Rational a(0, -1);
	Rational b(0, -123);
	EXPECT_EQ("0", (a - b).to_string());
	EXPECT_EQ("0", (b - a).to_string());
	EXPECT_EQ("0", (a - b).as_decimal());
	EXPECT_EQ("0", (b - a).as_decimal());

	Rational c;
	EXPECT_EQ("-0", (a - c).to_string());
	EXPECT_EQ("0", (c - a).to_string());
	EXPECT_EQ("-0", (a - c).as_decimal());
	EXPECT_EQ("0", (c - a).as_decimal());

	Rational d(1);
	EXPECT_EQ("-1", (a - d).to_string());
	EXPECT_EQ("1", (d - a).to_string());
	EXPECT_EQ("-1", (a - d).as_decimal());
	EXPECT_EQ("1", (d - a).as_decimal());

	Rational f(-1);
	EXPECT_EQ("1", (a - f).to_string());
	EXPECT_EQ("-1", (f - a).to_string());
	EXPECT_EQ("1", (a - f).as_decimal());
	EXPECT_EQ("-1", (f - a).as_decimal());
}

TEST(RationalTest, minus_zero_mul)
{
	Rational a(0, -1);
	Rational b(0, -123);
	EXPECT_EQ("0", (a * b).to_string());
	EXPECT_EQ("0", (b * a).to_string());
	EXPECT_EQ("0", (a * b).as_decimal());
	EXPECT_EQ("0", (b * a).as_decimal());

	Rational c;
	EXPECT_EQ("-0", (a * c).to_string());
	EXPECT_EQ("-0", (c * a).to_string());
	EXPECT_EQ("-0", (a * c).as_decimal());
	EXPECT_EQ("-0", (c * a).as_decimal());

	Rational d(1);
	EXPECT_EQ("-0", (a * d).to_string());
	EXPECT_EQ("-0", (d * a).to_string());
	EXPECT_EQ("-0", (a * d).as_decimal());
	EXPECT_EQ("-0", (d * a).as_decimal());

	Rational f(-1);
	EXPECT_EQ("0", (a * f).to_string());
	EXPECT_EQ("0", (f * a).to_string());
	EXPECT_EQ("0", (a * f).as_decimal());
	EXPECT_EQ("0", (f * a).as_decimal());
}

TEST(RationalTest, minus_zero_div)
{
	Rational a(0, -1);
	Rational d(1);
	EXPECT_EQ("-0", (a / d).to_string());
	EXPECT_EQ("-0", (a / d).as_decimal());

	Rational f(-1);
	EXPECT_EQ("0", (a / f).to_string());
	EXPECT_EQ("0", (a / f).as_decimal());
}*/ // Not supported

TEST(RationalTest, comparisons)
{
	Rational a(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
	Rational b(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
	Rational c(123456789, 12345678);
	Rational d(1,-1);
	Rational f(-1);

	EXPECT_TRUE(a == b);
	EXPECT_TRUE(a == 1);
	EXPECT_TRUE(b == 1);
	EXPECT_TRUE(1 == a);
	EXPECT_TRUE(1 == b);
	EXPECT_TRUE(a != c);
	EXPECT_TRUE(a < c);
	EXPECT_TRUE(c > a);
	EXPECT_TRUE(a <= a);
	EXPECT_TRUE(a <= b);
	EXPECT_TRUE(a <= c);
	EXPECT_TRUE(c >= a);

	EXPECT_TRUE(d != 0);
	EXPECT_TRUE(0 != d);
	EXPECT_TRUE(a > d);
	EXPECT_TRUE(d < a);
	EXPECT_TRUE(b > d);
	EXPECT_TRUE(d < b);
	EXPECT_TRUE(c > d);
	EXPECT_TRUE(d < c);
	EXPECT_TRUE(a >= d);
	EXPECT_TRUE(d <= a);
	EXPECT_TRUE(b >= d);
	EXPECT_TRUE(d <= b);
	EXPECT_TRUE(c >= d);
	EXPECT_TRUE(d <= c);

	EXPECT_TRUE(f != 0);
	EXPECT_TRUE(0 != f);
	EXPECT_TRUE(a > f);
	EXPECT_TRUE(f < a);
	EXPECT_TRUE(b > f);
	EXPECT_TRUE(f < b);
	EXPECT_TRUE(c > f);
	EXPECT_TRUE(f < c);
	EXPECT_TRUE(a >= f);
	EXPECT_TRUE(f <= a);
	EXPECT_TRUE(b >= f);
	EXPECT_TRUE(f <= b);
	EXPECT_TRUE(c >= f);
	EXPECT_TRUE(f <= c);

	EXPECT_TRUE(d == f);
	EXPECT_TRUE(d <= f);
	EXPECT_TRUE(d >= f);
	EXPECT_TRUE(f == d);
	EXPECT_TRUE(f <= d);
	EXPECT_TRUE(f >= d);
}

TEST(RationalTest, operator_minus)
{
	Rational a(1, -1);
	Rational b(-1);
	Rational c(0, 123456789);
	Rational d(11, 7);

	EXPECT_TRUE(-a == -b);
	EXPECT_TRUE(-a == 1);
	EXPECT_TRUE(1 == -a);
	EXPECT_TRUE(-b == 1);
	EXPECT_TRUE(1 == -b);
	EXPECT_TRUE(-c == 0);
	EXPECT_TRUE(0 == -c);

	EXPECT_TRUE(-d < -a);
	EXPECT_TRUE(-d < 0);
	EXPECT_TRUE(d > -a);
	EXPECT_TRUE(d > 0);
	EXPECT_TRUE(-a > -d);
	EXPECT_TRUE(0 > -d);
	EXPECT_TRUE(-a < d);
	EXPECT_TRUE(0 < d);
}

TEST(RationalTest, as_decimal)
{
	EXPECT_EQ("0.33333333333333333333333333333333333333333333333333", Rational(1, 3).as_decimal(50));
	EXPECT_EQ("0.66666666666666666667", Rational(2, 3).as_decimal(20));
	EXPECT_EQ("0.285714285714286", Rational(2, 7).as_decimal(15));
	EXPECT_EQ("4", Rational(7, 2).as_decimal());
	EXPECT_EQ("0.1", Rational(1, 11).as_decimal(1));
	EXPECT_EQ("0.09", Rational(1, 11).as_decimal(2));
	EXPECT_EQ("0.091", Rational(1, 11).as_decimal(3));
	EXPECT_EQ("1", Rational(5, 7).as_decimal());
	EXPECT_EQ("1", Rational(99, 100).as_decimal());
	EXPECT_EQ("1", Rational(99, 100).as_decimal(1));
	EXPECT_EQ("0.99", Rational(99, 100).as_decimal(2));

	EXPECT_EQ("-0.33333333333333333333333333333333333333333333333333", Rational(1, -3).as_decimal(50));
	EXPECT_EQ("-0.66666666666666666667", Rational(2, -3).as_decimal(20));
	EXPECT_EQ("-0.285714285714286", Rational(2, -7).as_decimal(15));
	EXPECT_EQ("-4", Rational(7, -2).as_decimal());
	EXPECT_EQ("-0.1", Rational(1, -11).as_decimal(1));
	EXPECT_EQ("-0.09", Rational(1, -11).as_decimal(2));
	EXPECT_EQ("-0.091", Rational(1, -11).as_decimal(3));
	EXPECT_EQ("-1", Rational(5, -7).as_decimal());
	EXPECT_EQ("-1", Rational(99, -100).as_decimal());
	EXPECT_EQ("-1", Rational(99, -100).as_decimal(1));
	EXPECT_EQ("-0.99", Rational(99, -100).as_decimal(2));

	EXPECT_EQ("-0.33333333333333333333333333333333333333333333333333", Rational(-1, 3).as_decimal(50));
	EXPECT_EQ("-0.66666666666666666667", Rational(-2, 3).as_decimal(20));
	EXPECT_EQ("-0.285714285714286", Rational(-2, 7).as_decimal(15));
	EXPECT_EQ("-4", Rational(-7, 2).as_decimal());
	EXPECT_EQ("-0.1", Rational(-1, 11).as_decimal(1));
	EXPECT_EQ("-0.09", Rational(-1, 11).as_decimal(2));
	EXPECT_EQ("-0.091", Rational(-1, 11).as_decimal(3));
	EXPECT_EQ("-1", Rational(-5, 7).as_decimal());
	EXPECT_EQ("-1", Rational(-99, 100).as_decimal());
	EXPECT_EQ("-1", Rational(-99, 100).as_decimal(1));
	EXPECT_EQ("-0.99", Rational(-99, 100).as_decimal(2));

	EXPECT_EQ("0.33333333333333333333333333333333333333333333333333", Rational(-1, -3).as_decimal(50));
	EXPECT_EQ("0.66666666666666666667", Rational(-2, -3).as_decimal(20));
	EXPECT_EQ("0.285714285714286", Rational(-2, -7).as_decimal(15));
	EXPECT_EQ("4", Rational(-7, -2).as_decimal());
	EXPECT_EQ("0.1", Rational(-1, -11).as_decimal(1));
	EXPECT_EQ("0.09", Rational(-1, -11).as_decimal(2));
	EXPECT_EQ("0.091", Rational(-1, -11).as_decimal(3));
	EXPECT_EQ("1", Rational(-5, -7).as_decimal());
	EXPECT_EQ("1", Rational(-99, -100).as_decimal());
	EXPECT_EQ("1", Rational(-99, -100).as_decimal(1));
	EXPECT_EQ("0.99", Rational(-99, -100).as_decimal(2));
}

TEST(RationalTest, static_cast_double)
{
	EXPECT_EQ(std::to_string(1.0 / 3), std::to_string(static_cast<double>(Rational(1, 3))));
	EXPECT_EQ(std::to_string(2.0 / 3), std::to_string(static_cast<double>(Rational(2, 3))));
	EXPECT_EQ(std::to_string(2.0 / 7), std::to_string(static_cast<double>(Rational(2, 7))));
	EXPECT_EQ(std::to_string(7.0 / 2), std::to_string(static_cast<double>(Rational(7, 2))));
	EXPECT_EQ(std::to_string(1.0 / 11), std::to_string(static_cast<double>(Rational(1, 11))));
	EXPECT_EQ(std::to_string(1.0 / 1234), std::to_string(static_cast<double>(Rational(1, 1234))));
	EXPECT_EQ(std::to_string(5.0 / 7), std::to_string(static_cast<double>(Rational(5, 7))));
	EXPECT_EQ(std::to_string(99.0 / 100), std::to_string(static_cast<double>(Rational(99, 100))));
	EXPECT_EQ(std::to_string(54.0 / 69), std::to_string(static_cast<double>(Rational(54, 69))));
	EXPECT_EQ(std::to_string(666.0 / 6), std::to_string(static_cast<double>(Rational(666, 6))));
	EXPECT_EQ(std::to_string(731946285.0 / 28731946), std::to_string(static_cast<double>(Rational(731946285, 28731946))));

	EXPECT_EQ(std::to_string(1.0 / -3), std::to_string(static_cast<double>(Rational(1, -3))));
	EXPECT_EQ(std::to_string(2.0 / -3), std::to_string(static_cast<double>(Rational(2, -3))));
	EXPECT_EQ(std::to_string(2.0 / -7), std::to_string(static_cast<double>(Rational(2, -7))));
	EXPECT_EQ(std::to_string(7.0 / -2), std::to_string(static_cast<double>(Rational(7, -2))));
	EXPECT_EQ(std::to_string(1.0 / -11), std::to_string(static_cast<double>(Rational(1, -11))));
	EXPECT_EQ(std::to_string(1.0 / -1234), std::to_string(static_cast<double>(Rational(1, -1234))));
	EXPECT_EQ(std::to_string(5.0 / -7), std::to_string(static_cast<double>(Rational(5, -7))));
	EXPECT_EQ(std::to_string(99.0 / -100), std::to_string(static_cast<double>(Rational(99, -100))));
	EXPECT_EQ(std::to_string(54.0 / -69), std::to_string(static_cast<double>(Rational(54, -69))));
	EXPECT_EQ(std::to_string(666.0 / -6), std::to_string(static_cast<double>(Rational(666, -6))));
	EXPECT_EQ(std::to_string(731946285.0 / -28731946), std::to_string(static_cast<double>(Rational(731946285, -28731946))));

	EXPECT_EQ(std::to_string(-1.0 / 3), std::to_string(static_cast<double>(Rational(-1, 3))));
	EXPECT_EQ(std::to_string(-2.0 / 3), std::to_string(static_cast<double>(Rational(-2, 3))));
	EXPECT_EQ(std::to_string(-2.0 / 7), std::to_string(static_cast<double>(Rational(-2, 7))));
	EXPECT_EQ(std::to_string(-7.0 / 2), std::to_string(static_cast<double>(Rational(-7, 2))));
	EXPECT_EQ(std::to_string(-1.0 / 11), std::to_string(static_cast<double>(Rational(-1, 11))));
	EXPECT_EQ(std::to_string(-1.0 / 1234), std::to_string(static_cast<double>(Rational(-1, 1234))));
	EXPECT_EQ(std::to_string(-5.0 / 7), std::to_string(static_cast<double>(Rational(-5, 7))));
	EXPECT_EQ(std::to_string(-99.0 / 100), std::to_string(static_cast<double>(Rational(-99, 100))));
	EXPECT_EQ(std::to_string(-54.0 / 69), std::to_string(static_cast<double>(Rational(-54, 69))));
	EXPECT_EQ(std::to_string(-666.0 / 6), std::to_string(static_cast<double>(Rational(-666, 6))));
	EXPECT_EQ(std::to_string(-731946285.0 / 28731946), std::to_string(static_cast<double>(Rational(-731946285, 28731946))));

	EXPECT_EQ(std::to_string(-1.0 / -3), std::to_string(static_cast<double>(Rational(-1, -3))));
	EXPECT_EQ(std::to_string(-2.0 / -3), std::to_string(static_cast<double>(Rational(-2, -3))));
	EXPECT_EQ(std::to_string(-2.0 / -7), std::to_string(static_cast<double>(Rational(-2, -7))));
	EXPECT_EQ(std::to_string(-7.0 / -2), std::to_string(static_cast<double>(Rational(-7, -2))));
	EXPECT_EQ(std::to_string(-1.0 / -11), std::to_string(static_cast<double>(Rational(-1, -11))));
	EXPECT_EQ(std::to_string(-1.0 / -1234), std::to_string(static_cast<double>(Rational(-1, -1234))));
	EXPECT_EQ(std::to_string(-5.0 / -7), std::to_string(static_cast<double>(Rational(-5, -7))));
	EXPECT_EQ(std::to_string(-99.0 / -100), std::to_string(static_cast<double>(Rational(-99, -100))));
	EXPECT_EQ(std::to_string(-54.0 / -69), std::to_string(static_cast<double>(Rational(-54, -69))));
	EXPECT_EQ(std::to_string(-666.0 / -6), std::to_string(static_cast<double>(Rational(-666, -6))));
	EXPECT_EQ(std::to_string(-731946285.0 / -28731946), std::to_string(static_cast<double>(Rational(-731946285, -28731946))));
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
