#include <gtest/gtest.h>
//#include <gmock/gmock.h>

#include "String.h"
#include "String.cpp"
#include <random>
#include <string>

TEST(Constructors, Default) {
  String s;
  ASSERT_EQ(s.size(), 0);
  ASSERT_TRUE(s.empty());
}

TEST(Constructors, FromNumberAndLetter) {
  const size_t size = 100;
  String s(size, 'a');
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(s[i], 'a');
  }
}

TEST(Constructors, FromCString) {
  const char *cstr = "aaa";
  String s(cstr);
  for (size_t i = 0; i < 3; ++i) {
    ASSERT_EQ(s[i], 'a');
  }
}

TEST(Constructors, CopyConstructor) {
  const size_t size = 100;
  String s(size, 'a');
  String s1(s);
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(s[i], s1[i]);
  }
  ASSERT_NE(s.data(), s1.data());
}

TEST(Assignment, Simple) {
  const size_t size = 100;
  String s(size, 'a');
  String s1 = s;
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(s[i], s1[i]);
  }
  ASSERT_NE(s.data(), s1.data());
}

TEST(Assignment, Self) {
  const size_t size = 100;
  String s(size, 'a');
  s = s;
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(s[i], s[i]);
  }
  ASSERT_EQ(s.data(), s.data());
}

TEST(Clear, Clear) {
  const size_t size = 100;
  String s(size, 'a');
  s.clear();
  ASSERT_EQ(s.size(), 0);
  ASSERT_TRUE(s.empty());
}

TEST(PushBack, ToExisting) {
  String s = "aba";
  s.push_back('c');
  ASSERT_EQ(s.size(), 4);
}

TEST(PushBack, ToDefault) {
  String s;
  s.push_back('c');
  ASSERT_EQ(s.size(), 1);
}

TEST(PushBack, FromNonEmpty) {
  String s = "ab";
  s.pop_back();
  ASSERT_EQ(s.size(), 1);
  s.pop_back();
  ASSERT_EQ(s.size(), 0);
}

TEST(Swap, Two) {
  String s = "aboba";
  String t = "abiba";
  s.swap(t);
  ASSERT_EQ(s.size(), t.size());
  ASSERT_EQ(s[2], 'i');
  ASSERT_EQ(t[2], 'o');
}

TEST(Swap, One) {
  String s = "aboba";
  s.swap(s);
  ASSERT_EQ(s.size(), s.size());
  ASSERT_EQ(s[2], 'o');
}

TEST(SquareBrackets, NonConst) {
  String s = "aboba";
  const bool is_ref = std::is_reference<decltype(s[0])>();
  EXPECT_TRUE(is_ref);
  s[0] = 'c';
  const bool is_char =
      std::is_same<std::remove_reference_t<decltype(s[0])>, char>();
  EXPECT_TRUE(is_char);
}

TEST(SquareBrackets, Const) {
  const String s = "aboba";
  const bool is_const_ref =
      std::is_reference<decltype(s[0])>() &&
      std::is_const<std::remove_reference_t<decltype(s[0])>>();
  const bool is_not_ref = !std::is_reference<decltype(s[0])>();
  EXPECT_TRUE(is_const_ref || is_not_ref);
}

TEST(FrontBack, Const) {
  const String s = "abob";
  bool is_const_ref =
      std::is_reference<decltype(s.front())>() &&
      std::is_const<std::remove_reference_t<decltype(s.front())>>();
  bool is_not_ref = !std::is_reference<decltype(s.front())>();
  EXPECT_TRUE(is_const_ref || is_not_ref);
  EXPECT_EQ(s.front(), 'a');
  is_const_ref = std::is_reference<decltype(s.back())>() &&
                 std::is_const<std::remove_reference_t<decltype(s.back())>>();
  is_not_ref = !std::is_reference<decltype(s.back())>();
  EXPECT_TRUE(is_const_ref || is_not_ref);
  EXPECT_EQ(s.back(), 'b');
}

TEST(FrontBack, NonConst) {
  String s = "abob";
  bool is_ref = std::is_reference<decltype(s.front())>();
  bool is_char =
      std::is_same<std::remove_reference_t<decltype(s.front())>, char>();
  EXPECT_TRUE(is_ref && is_char);
  s.front() = 'c';
  EXPECT_EQ(s[0], 'c');

  is_ref = std::is_reference<decltype(s.back())>();
  is_char = std::is_same<std::remove_reference_t<decltype(s.back())>, char>();
  EXPECT_TRUE(is_ref && is_char);
  s.back() = 'c';
  EXPECT_EQ(s[s.size() - 1], 'c');
}

TEST(Comparison, SameLen) {
  String s = "aboba";
  String t = "abiba";
  EXPECT_TRUE(t < s);
  EXPECT_FALSE(t > s);
  EXPECT_FALSE(t >= s);
  EXPECT_TRUE(t <= s);
  EXPECT_FALSE(t == s);
  EXPECT_TRUE(t != s);
}

TEST(Comparison, Const) {
  const String s = "aboba";
  const String t = "abiba";
  EXPECT_TRUE(t < s);
  EXPECT_FALSE(t > s);
  EXPECT_FALSE(t >= s);
  EXPECT_TRUE(t <= s);
  EXPECT_FALSE(t == s);
  EXPECT_TRUE(t != s);
}

TEST(Comparison, DiffLen) {
  String s = "abobabiba";
  String t = "aboba";
  EXPECT_TRUE(t < s);
  EXPECT_FALSE(t > s);
  EXPECT_FALSE(t >= s);
  EXPECT_TRUE(t <= s);
  EXPECT_FALSE(t == s);
  EXPECT_TRUE(t != s);
}

TEST(Comparison, Empty) {
  String s = "aboba";
  String t = "";
  EXPECT_TRUE(t < s);
  EXPECT_FALSE(t > s);
  EXPECT_FALSE(t >= s);
  EXPECT_TRUE(t <= s);
  EXPECT_FALSE(t == s);
  EXPECT_TRUE(t != s);
}

TEST(Iostream, In) {
  std::stringstream is{"olololo"};
  String s;
  is >> s;
  ASSERT_EQ(s, String("olololo"));
}

TEST(Iostream, Out) {
  std::stringstream os;
  String s = "lol";
  os << s;
  ASSERT_EQ(os.str(), "lol");
}

TEST(Iostream, ConstOut) {
  std::stringstream os;
  const String s = "lol";
  os << s;
  ASSERT_EQ(os.str(), "lol");
}

TEST(Concat, EasyPlus) {
  String s = "aboba";
  String t = "biba";
  EXPECT_TRUE(s + t == "abobabiba");
}

TEST(Concat, ConstPlus) {
  const String s = "aboba";
  const String t = "biba";
  EXPECT_TRUE(s + t == "abobabiba");
}

TEST(Concat, StressPlus) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 25);
  std::string expected;
  String s;
  const size_t num_iterations = 1 << 10;
  for (size_t i = 0; i < num_iterations; ++i) {
    String to_add = "a";
    to_add[0] += distrib(gen);
    expected.append(to_add.data());
    s = s + to_add;
  }
EXPECT_TRUE(s == expected.data());
}

TEST(Concat, PlusAssignmentCombo) {
  String s = "(";
  String t = ")";
  s += t + s += s + t += t;
  EXPECT_TRUE((s += t) == "()(()))");
  (s += t) = "f";
EXPECT_TRUE(s == "f");
}

TEST(Concat, PlusAssignmentFibonacciStyle) {
  String s = "(";
  String t = ")";
  std::string s_s = "(";
  std::string t_s = ")";
  const size_t num_iterations = 18;
  for (size_t i = 0; i < num_iterations; ++i) {
    s += t;
    t += s;
    s_s += t_s;
    t_s += s_s;
  }
  EXPECT_TRUE(t == t_s.data());
  EXPECT_TRUE(s == s_s.data());
}

TEST(FindSubstr, Find) {
	String s = "This is a string";

	size_t n = s.find("is");
	EXPECT_TRUE(n == 2);

	n = s.find("is", 5);
	EXPECT_TRUE(n == 5);

	n = s.find('a');
	EXPECT_TRUE(n == 8);

	n = s.find("string", 11);
	EXPECT_TRUE(n == String::npos);

	n = s.find('q');
	EXPECT_TRUE(n == String::npos);
}

TEST(FindSubstr, rFind) {
	std::string const s = "This is a string";

    size_t n = s.rfind("is");
	EXPECT_TRUE(n == 5); 

    n = s.rfind("is", 4);
	EXPECT_TRUE(n == 2);
 
    n = s.rfind('s');
	EXPECT_TRUE(n == 10);
 
    n = s.rfind('q');
	EXPECT_TRUE(n == String::npos);
 
    n = s.rfind("This", 0);
	EXPECT_TRUE(n == 0);
}

TEST(Substr, CreateSubstr) {
	String a = "0123456789abcdefghij";
 
    String sub1 = a.substr(10);
	EXPECT_TRUE(sub1 == "abcdefghij");

    String sub2 = a.substr(5, 3);
	EXPECT_TRUE(sub2 == "567");
 
    String sub3 = a.substr(a.size() - 3, 50);
 	EXPECT_TRUE(sub3 == "hij");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
