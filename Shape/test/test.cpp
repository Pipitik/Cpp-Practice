#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "../shape.h"
#include "../shape.cpp"

#include "gtest/gtest.h"

using namespace Geometry;

TEST(PointsDistance, TestOne) {
    Point a(3, 0);
    Point b(0, 4);

    EXPECT_NEAR(Point::distance(a, b), 5.0, 6);
}

TEST(Polygon, EqualOperator) {
    Point a(1, 1);
    Point b(-1, 1);
    Point c(-1, -1);
    Point d(1, -1);

    std::vector<Point> vec = {a, b, c, d};
    Polygon actual(vec);
    vec = {d, a, b, c};
    Polygon expected(vec);
    EXPECT_TRUE(actual == expected);
}

TEST(Polygon, PolygonConvexity) {
    Point a(0, 1);
    Point b(-1, 0);
    Point c(0, -1);
    Point d(1, 0);

    std::vector<Point> vec = {a, b, c, d};
    Polygon poly(vec);
    EXPECT_TRUE(poly.is_convex());
}

TEST(Similarity, Test1) {
    std::vector<Point> vec;
    {
        Point a(0, 1);
        Point b(-1, 0);
        Point c(0, -1);
        Point d(1, 0);
        vec = {a, b, c, d};
    }

    Polygon actual(vec);

    {
        Point a(20, 20);
        Point b(-20, 20);
        Point c(-20, -20);
        Point d(20, -20);
        vec = {a, b, c, d};
    }

    Polygon expected(vec);

    EXPECT_TRUE(actual.is_similar_to(expected));
}

TEST(Similarity, Test2) {
    std::vector<Point> vec;
    {
        Point a(0, 1);
        Point b(-1, 0);
        Point c(0, -1);
        Point d(1, 0);
        vec = {a, b, c, d};
    }

    Polygon actual(vec);

    {
        Point a(11, 11);
        Point b(-11, -11);
        Point c(0, -100);
        vec = {a, b, c};
    }

    Polygon expected(vec);

    EXPECT_FALSE(actual.is_similar_to(expected));
}

TEST(Congruent, Congruent) {
    std::vector<Point> vec;
    {
        Point a(0, 1);
        Point b(-1, 0);
        Point c(0, -1);
        Point d(1, 0);
        vec = {a, b, c, d};
    }

    Polygon actual(vec);

    actual.rotate(std::numbers::pi / 4, { 0, 0 });
    actual.scale(sqrt(2), { 0, 0 });
    {
        Point a(1, 1);
        Point b(-1, 1);
        Point c(-1, -1);
        Point d(1, -1);
        vec = {a, b, c, d};
    }


    Polygon expected(vec);

    EXPECT_TRUE(actual.is_congruent_to(expected));
}

TEST(Area, TrianglePolygons) {
    Point a(1.0, 0.0);
    Point b(0.0, 1.0);
    Point c(2.0, 1.0);
    Point d(1, 1.5);

    Triangle triangle1(a, b, d);
    Triangle triangle2(a, c, d);

    std::vector<Point> vec = {a, b, d, c};
    Polygon poly(vec);

    EXPECT_NEAR(poly.area(), triangle1.area() + triangle2.area(), Point::PRECISION);
}

TEST(Transformations, Test1) {
    std::vector<Point> vec = 
    {
        Point(1, 1),
        Point(-1, 1),
        Point(-1, -1),
        Point(1, -1)
    };

    Polygon actual(vec);
    actual.rotate(std::numbers::pi / 4);
    actual.scale(20);
    actual.scale(1 / sqrt(2));

	vec =
    {
        Point(0, 20),
        Point(-20, 0),
        Point(0, -20),
        Point(20, 0)
    };

    Polygon expected(vec);

    EXPECT_TRUE(actual == expected);
    EXPECT_NEAR(actual.perimeter(), expected.perimeter(), Point::PRECISION);
    EXPECT_NEAR(actual.area(), expected.area(), Point::PRECISION);
}

TEST(Transformations, Test2) {
    Point a(0, 0);
    Point b(-1, -1);
    Point c(1, -1);
    Point d(-1, 1);
    Point e(1, 1);

    Triangle actual(a, b, c);
    Triangle expected(a, d, e);
    actual.rotate(std::numbers::pi / 4, a);
    actual.scale(20, a);
    actual.rotate(std::numbers::pi / 4, a);
    Line line(a, {1, 1});
    actual.reflex(line);
    actual.scale(0.05, a);
    EXPECT_TRUE(actual == expected);
}

TEST(Rectangle, Test1) {
    Point a(1.0, 0.0);
    Point b(2.5, 3.0);
    Point c(1.0, 3.0);

    Rectangle rectangle(a, b);
    Triangle triangle(a, b, c);
    EXPECT_NEAR(rectangle.area(), 2 * triangle.area(), Point::PRECISION);
}

TEST(ContainsPoint, Test1) {
    Point a(1.0, 0.0);
    Point b(2.5, 3.0);
    Point c(1.0, 3.0);
    Point d(2.5, 0.0);

    std::vector<Point> vec = { a, b, c, d };
    Polygon poly(vec);

    EXPECT_TRUE(poly.contains_point(Point(1.6, 1)));
}

TEST(ContainsPoint, Test2) {
    Point a(-1.0, 0.0);
    Point b(1.0, 0.0);

    Ellipse ellipse(a, b, 4);
    EXPECT_TRUE(ellipse.contains_point(Point(0, 0)));
}

TEST(Ellipse, Eccentricity) {
    Point a(-1.0, 0.0);
    Point b(1.0, 0.0);
    Ellipse ellipse(a, b, 4);

    double eccentricity = 0.5;
    EXPECT_NEAR(ellipse.eccentricity(), eccentricity, Point::PRECISION);
}

TEST(Ellipse, Perimeter) {
    double a = 5;
    double b = 4;
    double c = 3;

    Point p1(-c, 0.0);
    Point p2(c, 0.0);
    Ellipse ellipse(p1, p2, 2 * a);

    double perimeter = 2 * std::numbers::pi * std::sqrt((a * a + b * b) / 2);
    EXPECT_NEAR(ellipse.perimeter(), perimeter, Point::PRECISION);
}

TEST(Ellipse, Area) {
    double a = 5;
    double b = 4;
    double c = 3;

    Point p1(-c, 0.0);
    Point p2(c, 0.0);
    Ellipse ellipse(p1, p2, 2 * a);

    double area = std::numbers::pi * a * b;
    EXPECT_NEAR(ellipse.area(), area, Point::PRECISION);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
