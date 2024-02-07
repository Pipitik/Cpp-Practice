#ifndef SHAPE_H
#define SHAPE_H

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <numbers>
#include <numeric>
#include <vector>

namespace Geometry
{
	struct Point final
	{
		static constexpr double PRECISION = 1e-6;
		double x;
		double y;
		Point() = default;
		Point(double x, double y);
		bool operator==(const Point& other) const;
		static double distance(const Point& pt1, const Point& pt2);
	};

	class Line final
	{
	public:
		Line();
		Line(const Point& start, const Point& end);
		Line(double slope, double offset);
		Line(double slope, const Point& point);
		Line(const Line& other) = default;
		Line& operator=(const Line& other) = default;
		bool operator==(const Line& other) const;
		double slope() const;
		double offset() const;
		double angle_between(const Line& other) const;
		Point common_point(const Line& other) const;
		Line perpendicular_line(const Point& point) const;
		std::pair<double, double> normal_vector() const;

	private:
		double _a;
		double _b;
		double _c;
	};

	class Shape
	{
	public:
		virtual ~Shape() = default;
		virtual bool operator==(const Shape& other) const = 0;
		virtual bool is_congruent_to(const Shape& other) const = 0;
		virtual bool is_similar_to(const Shape& other) const = 0;
		virtual double perimeter() const = 0;
		virtual double area() const = 0;
		virtual bool contains_point(const Point& point) const = 0;
		virtual void rotate(double angle, const Point& center = { 0.0, 0.0 }) = 0;
		virtual void reflex(const Point& center) = 0;
		virtual void reflex(const Line& axis) = 0;
		virtual void scale(double coefficient, const Point& center = { 0.0, 0.0 }) = 0;
	};

	class Rectangle;
	class Square;
	class Triangle;
	class Ellipse;
	class Circle;

	class Polygon : public Shape
	{
		friend bool operator==(const Polygon& lhs, const Polygon& rhs);

	public:
		using size_type = std::vector<Point>::size_type;

		Polygon(const std::vector<Point>& vertices = std::vector<Point>());
		Polygon(std::initializer_list<Point> init_list);
		bool operator==(const Shape& other) const final;
		bool operator==(const Ellipse&) const = delete;
		bool operator==(const Circle&) const = delete;
		bool is_congruent_to(const Shape& other) const override;
		bool is_similar_to(const Shape& other) const override;
		double perimeter() const override;
		double area() const override;
		bool contains_point(const Point& point) const override;
		void rotate(double angle, const Point& center = { 0.0, 0.0 }) final;
		void reflex(const Point& center) final;
		void reflex(const Line& axis) final;
		void scale(double coefficient, const Point& center = { 0.0, 0.0 }) final;
		size_type vertices_count() const;
		virtual bool is_convex() const;

	protected:
		std::vector<Point> _vertex;

		static bool _is_point_in_vertices(const Point& _pt, const std::vector<Point>& _vertices);
	};

	bool operator==(const Polygon& lhs, const Polygon& rhs);

	class Ellipse : public Shape
	{
		friend bool operator==(const Ellipse& lhs, const Ellipse& rhs);

	public:
		Ellipse(const Point& focus1, const Point& focus2, double sum_distances);
		bool operator==(const Shape& other) const final;
		bool operator==(const Polygon&) const = delete;
		bool operator==(const Rectangle&) const = delete;
		bool operator==(const Square&) const = delete;
		bool operator==(const Triangle&) const = delete;
		bool is_congruent_to(const Shape& other) const override;
		bool is_similar_to(const Shape& other) const override;
		double perimeter() const override;
		double area() const override;
		bool contains_point(const Point& point) const override;
		void rotate(double angle, const Point& center = { 0.0, 0.0 }) final;
		void reflex(const Point& center) final;
		void reflex(const Line& axis) final;
		void scale(double coefficient, const Point& center = { 0.0, 0.0 }) final;
		std::pair<Point, Point> focuses() const;
		std::pair<Line, Line> directrices() const;
		double eccentricity() const;
		virtual Point center() const;

	protected:
		Point _focus1;
		Point _focus2;
		double _sum_distances;

	private:
		double _focal_distance() const;
		double _length_semi_major_axis() const;
		double _length_semi_minor_axis() const;
	};

	bool operator==(const Ellipse& lhs, const Ellipse& rhs);

	class Circle final : public Ellipse
	{
	public:
		Circle(const Point& center, double radius);
		bool is_congruent_to(const Shape& other) const final;
		bool is_similar_to(const Shape& other) const final;
		double perimeter() const final;
		double area() const final;
		bool contains_point(const Point& point) const final;
		Point center() const final;
		double radius() const;
	};

	class Rectangle : public Polygon
	{
	public:
		Rectangle(const Point& left_top, const Point& right_bottom);
		bool operator==(const Triangle&) const = delete;
		bool is_congruent_to(const Shape& other) const override;
		bool is_similar_to(const Shape& other) const override;
		double perimeter() const final;
		double area() const final;
		bool contains_point(const Point& point) const final;
		bool is_convex() const final;
		Point center() const;
		std::pair<Line, Line> diagonals() const;
	};

	class Square final : public Rectangle
	{
	public:
		Square(const Point& left_top, double length);
		bool is_congruent_to(const Shape& other) const final;
		bool is_similar_to(const Shape& other) const final;
		Circle circumscribed_circle() const;
		Circle inscribed_circle() const;
	};

	class Triangle final : public Polygon
	{
	public:
		Triangle(const Point& pt1, const Point& pt2, const Point& pt3);
		bool operator==(const Rectangle&) const = delete;
		bool operator==(const Square&) const = delete;
		bool is_congruent_to(const Shape& other) const final;
		bool is_similar_to(const Shape& other) const final;
		double perimeter() const final;
		double area() const final;
		bool contains_point(const Point& point) const final;
		bool is_convex() const final;
		Circle circumscribed_circle() const;
		Circle inscribed_circle() const;
		Point centroid() const;
		Point orthocenter() const;
		Line euler_line() const;
		Circle nine_points_circle() const;
	};
}

#endif
