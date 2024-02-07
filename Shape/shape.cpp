#include "shape.h"

namespace Geometry
{
	/*++++++++++++++++++++++Support functions+++++++++++++++++++++++++++++*/
	/// Defines the direction of rotation in each three consecutive vertices
	double _cross_product(const Point& a, const Point& b, const Point& c)
	{
		Point ab =
		{
			b.x - a.x,
			b.y - a.y
		};
		Point bc =
		{
			c.x - b.x,
			c.y - b.y
		};
		return ab.x * bc.y - ab.y * bc.x;
	}

	/// Compares two double for equality with precision Point::PRECISION
	bool _double_equal(double _lhs, double _rhs, double precision = Point::PRECISION)
	{
		return std::abs(_lhs - _rhs) < precision;
	}
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	/*++++++++++++++++++++++++___class Point___+++++++++++++++++++++++++++*/
	Point::Point(double x, double y) : x(x), y(y) {}

	bool Point::operator==(const Point& other) const
	{
		return _double_equal(x, other.x) and
			_double_equal(y, other.y);
	}

	double Point::distance(const Point& pt1, const Point& pt2)
	{
		return std::sqrt((pt2.x - pt1.x) * (pt2.x - pt1.x) +
			(pt2.y - pt1.y) * (pt2.y - pt1.y));
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	/*++++++++++++++++++++++++___class Line___++++++++++++++++++++++++++++*/
	Line::Line() : Line(Point(0.0, 0.0), Point(0.0, 0.0)) {}

	// The equation of a line is ax + c = by;

	Line::Line(const Point& start, const Point& end)
		: _a(end.y - start.y)
		, _b(end.x - start.x)
		, _c(end.x* start.y - start.x * end.y) {}

	Line::Line(double slope, double offset)
		: _a(slope)
		, _b(_double_equal(std::atan(slope), 1.57, 1e-2) ? 0.0 : 1.0) // PI / 2 = 1.57
		, _c(offset) {}

	Line::Line(double slope, const Point& point)
		: _a(slope)
		, _b(_double_equal(std::atan(slope), 1.57, 1e-2) ? 0.0 : 1.0)
		, _c(point.y - slope * point.x) {}

	bool Line::operator==(const Line& other) const
	{
		return _double_equal(slope(), other.slope()) and
			_double_equal(offset(), other.offset());
	}

	double Line::slope() const
	{
		return _double_equal(_b, 0.0) ? _a : _a / _b;
	}

	double Line::offset() const
	{
		return _double_equal(_b, 0.0) ? _c : _c / _b;
	}

	double Line::angle_between(const Line& other) const
	{
		double k1 = slope();
		double k2 = other.slope();
		return std::atan2((k1 - k2), 1 + k1 * k2);
	}

	Point Line::common_point(const Line& other) const
	{
		// Kramer's method
		// a1x + c1 = b1y => a1x - b1y = -c1
		// a2x + c2 = b2y => a2x - b2y = -c2

		double det = -_a * other._b + _b * other._a;
		double det_x = _c * other._b - _b * other._c;
		double det_y = -_a * other._c + _c * other._a;
		return Point(det_x / det, det_y / det);
	}

	Line Line::perpendicular_line(const Point& point) const
	{
		double new_slope = _double_equal(slope(), 0.0) ? std::tan(std::numbers::pi / 2.0) : -1.0 / slope();
		double new_offset = point.y - point.x * new_slope;
		return Line(new_slope, new_offset);
	}

	std::pair<double, double> Line::normal_vector() const
	{
		return std::make_pair(_a, -_b);
	}
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	/*++++++++++++++++++++++++___class Polygon___+++++++++++++++++++++++++*/
	bool Polygon::_is_point_in_vertices(const Point& _pt, const std::vector<Point>& _vertices)
	{
		for (const Point& pt : _vertices) {
			if (_pt == pt) {
				return true;
			}
		}
		return false;
	}

	Polygon::Polygon(const std::vector<Point>& vertices) : _vertex(vertices) {}

	Polygon::Polygon(std::initializer_list<Point> init_list) : _vertex(init_list) {}

	bool operator==(const Polygon& lhs, const Polygon& rhs)
	{
		if (lhs._vertex.size() != rhs._vertex.size()) {
			return false;
		}

		for (const Point& pt : rhs._vertex) {
			if (!Polygon::_is_point_in_vertices(pt, lhs._vertex)) {
				return false;
			}
		}
		return true;
	}

	bool Polygon::operator==(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Polygon* pPoly = dynamic_cast<const Polygon*>(&other); pPoly) {
			return *this == *pPoly;
		}
		return false;
	}

	bool Polygon::is_congruent_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Polygon* pPoly = dynamic_cast<const Polygon*>(&other);
			pPoly and _vertex.size() == pPoly->_vertex.size()) {

			std::vector<double> lengths_lhs(_vertex.size());
			std::vector<double> lengths_rhs(pPoly->_vertex.size());
			for (size_type i = 0, j = _vertex.size() - 1; i < _vertex.size(); j = i++) {
				lengths_lhs[i] = Point::distance(_vertex[j], _vertex[i]);
				lengths_rhs[i] = Point::distance(pPoly->_vertex[j], pPoly->_vertex[i]);
			}

			std::sort(lengths_lhs.begin(), lengths_lhs.end());
			std::sort(lengths_rhs.begin(), lengths_rhs.end());
			for (std::vector<double>::size_type i = 0; i < lengths_lhs.size(); ++i) {
				if (!_double_equal(lengths_lhs[i], lengths_rhs[i])) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool Polygon::is_similar_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Polygon* pPoly = dynamic_cast<const Polygon*>(&other);
			pPoly and _vertex.size() == pPoly->_vertex.size()) {

			std::vector<double> lengths_lhs(_vertex.size());
			std::vector<double> lengths_rhs(pPoly->_vertex.size());
			for (size_type i = 0, j = _vertex.size() - 1; i < _vertex.size(); j = i++) {
				lengths_lhs[i] = Point::distance(_vertex[j], _vertex[i]);
				lengths_rhs[i] = Point::distance(pPoly->_vertex[j], pPoly->_vertex[i]);
			}

			std::sort(lengths_lhs.begin(), lengths_lhs.end());
			std::sort(lengths_rhs.begin(), lengths_rhs.end());
			for (std::vector<double>::size_type i = 0, j = lengths_lhs.size() - 1; i < lengths_lhs.size(); j = i++) {
				if (!_double_equal(lengths_lhs[i] * lengths_rhs[j], lengths_rhs[i] * lengths_lhs[j])) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	double Polygon::perimeter() const
	{
		double ret = 0.0;
		for (size_type i = 0, j = _vertex.size() - 1; i < _vertex.size(); j = i++) {
			ret += Point::distance(_vertex[j], _vertex[i]);
		}
		return ret;
	}

	double Polygon::area() const
	{
		double ret = 0.0;
		if (_vertex.size() < 3) {
			return ret;
		}

		/*for (size_type i = 0, j = _vertex.size() - 1; i < _vertex.size(); j = i++) {
			ret += _vertex[j].x * _vertex[i].y;
			ret -= _vertex[j].y * _vertex[i].x;
		}*/

		// Since C++20
		for (Point start = _vertex.back(); const Point& end : _vertex) {
			ret += start.x * end.y;
			ret -= start.y * end.x;
			start = end;
		}
		ret = std::abs(ret) / 2.0;
		return ret;
	}

	bool Polygon::contains_point(const Point& point) const
	{
		bool ret = false;
		if (_vertex.size() < 3) {
			return ret;
		}
		/*for (size_type i = 0, j = _vertex.size() - 1; i < _vertex.size(); j = i++) {
			if (((_vertex[i].y > point.y) != (_vertex[j].y > point.y)) and
				(_vertex[i].x + (point.y - _vertex[i].y) * (_vertex[j].x - _vertex[i].x) / (_vertex[j].y - _vertex[i].y) < point.x)) {
				ret = !ret;
			}
		}*/

		// Since C++20
		for (Point start = _vertex.back(); const Point & end : _vertex) {
			if (((end.y > point.y) != (start.y > point.y)) and
				(end.x + (point.y - end.y) * (start.x - end.x) / (start.y - end.y) < point.x)) {
				ret = !ret;
			}
			start = end;
		}
		return ret;
	}

	void Polygon::rotate(double angle, const Point& center)
	{
		for (Point& pt : _vertex) {
			double x = pt.x - center.x;
			double y = pt.y - center.y;
			pt.x = x * std::cos(angle) - y * std::sin(angle) + center.x;
			pt.y = x * std::sin(angle) + y * std::cos(angle) + center.y;
		}
	}

	void Polygon::reflex(const Point& center)
	{
		for (Point& pt : _vertex) {
			pt.x = 2.0 * center.x - pt.x;
			pt.y = 2.0 * center.y - pt.y;
		}
	}

	void Polygon::reflex(const Line& axis)
	{
		double slope = axis.slope();
		double angle = axis.angle_between(Line({ 0.0, 0.0 }, { 1.0, 0.0 })); // the angle between the x axis
		Point center = _double_equal(angle, 1.57, 1e-2) ?
			Point{ -axis.offset() / slope, 0.0 } :
			Point{ 0.0, axis.offset() };
		angle *= 2.0;
		for (Point& pt : _vertex) {
			double x = pt.x - center.x;
			double y = pt.y - center.y;
			pt.x = x * std::cos(angle) + y * std::sin(angle) + center.x;
			pt.y = x * std::sin(angle) - y * std::cos(angle) + center.y;
		}
	}

	void Polygon::scale(double coefficient, const Point& center)
	{
		for (Point& pt : _vertex) {
			double x = pt.x - center.x;
			double y = pt.y - center.y;
			pt.x = coefficient * x + center.x;
			pt.y = coefficient * y + center.y;
		}
	}

	Polygon::size_type Polygon::vertices_count() const
	{
		return _vertex.size();
	}

	bool Polygon::is_convex() const
	{
		if (_vertex.size() < 3) {
			return true;
		}

		size_type i = 0;
		size_type j = _vertex.size() - 1;
		size_type k = j - 1;
		bool sign_product = _cross_product(_vertex[k], _vertex[j], _vertex[i]) < 0.0;
		bool prev_sign = sign_product;
		k = j;
		j = i++;
		for (; i < _vertex.size(); k = j, j = i++) {
			sign_product = _cross_product(_vertex[k], _vertex[j], _vertex[i]) < 0.0;
			if (sign_product != prev_sign) {
				return false;
			}
		}
		return true;
	}
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	/*++++++++++++++++++++++++___class Ellipse___+++++++++++++++++++++++++*/
	double Ellipse::_focal_distance() const
	{
		return Point::distance(_focus1, _focus2);
	}

	double Ellipse::_length_semi_major_axis() const
	{
		return _sum_distances / 2.0;
	}

	double Ellipse::_length_semi_minor_axis() const
	{
		double a = _length_semi_major_axis();
		double c = _focal_distance() / 2.0;
		return std::sqrt(a * a - c * c);
	}

	Ellipse::Ellipse(const Point& focus1, const Point& focus2, double sum_distances)
		: _focus1(focus1)
		, _focus2(focus2)
		, _sum_distances(sum_distances) {}

	bool operator==(const Ellipse& lhs, const Ellipse& rhs)
	{
		if (lhs._focus1 == rhs._focus1 and lhs._focus2 == rhs._focus2 and
			_double_equal(lhs._sum_distances, rhs._sum_distances)) {
			return true;
		}
		return false;
	}

	bool Ellipse::operator==(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Ellipse* pEllipse = dynamic_cast<const Ellipse*>(&other); pEllipse) {
			return *this == *pEllipse;
		}
		return false;
	}

	bool Ellipse::is_congruent_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Ellipse* pEllipse = dynamic_cast<const Ellipse*>(&other);
			pEllipse and
			_double_equal(_focal_distance(), pEllipse->_focal_distance()) and
			_double_equal(_sum_distances, pEllipse->_sum_distances)) {
				return true;
		}
		return false;
	}

	bool Ellipse::is_similar_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Ellipse* pEllipse = dynamic_cast<const Ellipse*>(&other); pEllipse) {
			double k1 = _sum_distances * pEllipse->_focal_distance();
			double k2 = _focal_distance() * pEllipse->_sum_distances;
			if (_double_equal(k1, k2)) {
				return true;
			}
		}
		return false;
	}

	double Ellipse::perimeter() const
	{
		double a = _length_semi_major_axis();
		double b = _length_semi_minor_axis();
		return 2.0 * std::numbers::pi * std::sqrt((a * a + b * b) / 2.0);
	}

	double Ellipse::area() const
	{
		double a = _length_semi_major_axis();
		double b = _length_semi_minor_axis();
		return std::numbers::pi * a * b;
	}

	bool Ellipse::contains_point(const Point& point) const
	{
		double a = _length_semi_major_axis();
		double b = _length_semi_minor_axis();
		return (point.x * point.x) / (a * a) + (point.y * point.y) / (b * b) < 1.0;
	}

	void Ellipse::rotate(double angle, const Point& center)
	{
		for (Point* pt : { &_focus1, &_focus2 }) {
			double x = pt->x - center.x;
			double y = pt->y - center.y;
			pt->x = x * std::cos(angle) - y * std::sin(angle) + center.x;
			pt->y = x * std::sin(angle) + y * std::cos(angle) + center.y;
		}
	}

	void Ellipse::reflex(const Point& center)
	{
		for (Point* pt : { &_focus1, &_focus2 }) {
			pt->x = 2.0 * center.x - pt->x;
			pt->y = 2.0 * center.y - pt->y;
		}
	}

	/// Reflection relative to the line
	void Ellipse::reflex(const Line& line)
	{
		double slope = line.slope();
		double angle = line.angle_between(Line(Point(0.0, 0.0), Point(1.0, 0.0))); // the angle between the x line
		Point center = _double_equal(angle, 1.57, 1e-2) ?
			Point{ -line.offset() / slope, 0.0 } :
			Point{ 0.0, line.offset() };
		angle *= 2.0;
		for (Point* pt : { &_focus1, &_focus2 }) {
			double x = pt->x - center.x;
			double y = pt->y - center.y;
			pt->x = x * std::cos(angle) + y * std::sin(angle) + center.x;
			pt->y = x * std::sin(angle) - y * std::cos(angle) + center.y;
		}
	}

	void Ellipse::scale(double coefficient, const Point& center)
	{
		for (Point* pt : { &_focus1, &_focus2 }) {
			double x = pt->x - center.x;
			double y = pt->y - center.y;
			pt->x = coefficient * x + center.x;
			pt->y = coefficient * y + center.y;
		}
		_sum_distances *= coefficient;
	}

	std::pair<Point, Point> Ellipse::focuses() const
	{
		return std::make_pair(_focus1, _focus2);
	}

	std::pair<Line, Line> Ellipse::directrices() const
	{
		double c2 = _focal_distance();
		if (_double_equal(c2, 0.0)) {
			return std::make_pair(Line(), Line());
		}
		double distance_from_semi_minor =
			_sum_distances * _sum_distances / (2.0 * c2);

		Point ctr = center();
		std::pair<double, double> normal = Line(ctr, _focus2).normal_vector();
		Point additional(ctr.x + normal.first, ctr.y + normal.second);

		double angle = Line(_focus1, _focus2).angle_between(Line({ 0.0, 0.0 }, { 1.0, 0.0 })); // the angle between the x axis
		Line left_ditectrices
		(
			Point(ctr.x - distance_from_semi_minor * std::cos(angle), ctr.y - distance_from_semi_minor * std::sin(angle)),
			Point(additional.x - distance_from_semi_minor * std::cos(angle), additional.y - distance_from_semi_minor * std::sin(angle))
		);

		Line right_directrices
		(
			Point(ctr.x + distance_from_semi_minor * std::cos(angle), ctr.y + distance_from_semi_minor * std::sin(angle)),
			Point(additional.x + distance_from_semi_minor * std::cos(angle), additional.y + distance_from_semi_minor * std::sin(angle))
		);
		return std::make_pair(left_ditectrices, right_directrices);
	}

	double Ellipse::eccentricity() const
	{
		return _focal_distance() / _sum_distances;
	}

	Point Ellipse::center() const
	{
		return { (_focus1.x + _focus2.x) / 2.0, (_focus1.y + _focus2.y) / 2.0 };
	}
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	/*++++++++++++++++++++++++___class Circle___++++++++++++++++++++++++++*/
	Circle::Circle(const Point& center, double radius) : Ellipse(center, center, 2.0 * radius) {}

	bool Circle::is_congruent_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Circle* pCircle = dynamic_cast<const Circle*>(&other); pCircle) {
			if (_double_equal(_sum_distances, pCircle->_sum_distances)) {
				return true;
			}
		}
		return Ellipse::is_congruent_to(other);
	}

	bool Circle::is_similar_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Circle* pCircle = dynamic_cast<const Circle*>(&other); pCircle) {
			return true;
		}
		return Ellipse::is_congruent_to(other);
	}

	double Circle::perimeter() const
	{
		return std::numbers::pi * _sum_distances;
	}

	double Circle::area() const
	{
		double radius = _sum_distances / 2.0;
		return std::numbers::pi * radius * radius;
	}

	bool Circle::contains_point(const Point& point) const
	{
		double radius = _sum_distances / 2.0;
		return (point.x - _focus1.x) * (point.x - _focus1.x) +
			(point.y - _focus1.y) * (point.y - _focus1.y) < radius * radius;
	}

	Point Circle::center() const
	{
		return _focus1;
	}

	double Circle::radius() const
	{
		return _sum_distances / 2.0;
	}
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	/*++++++++++++++++++++++++___class Rectangle___+++++++++++++++++++++++*/
	Rectangle::Rectangle(const Point& left_top, const Point& right_bottom)
		: Polygon({ left_top, Point(right_bottom.x, left_top.y),
				right_bottom, Point(left_top.x, right_bottom.y) }) {}

	bool Rectangle::is_congruent_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Square* pSquare = dynamic_cast<const Square*>(&other); pSquare) {
			double length = Point::distance(_vertex[0], _vertex[1]);
			double width = Point::distance(_vertex[1], _vertex[2]);
			double rhs_length = Point::distance(pSquare->_vertex[0], pSquare->_vertex[1]);
			if (_double_equal(length, width) and
				_double_equal(length, rhs_length)) {
				return true;
			}
		}
		else if (const Rectangle* pRect = dynamic_cast<const Rectangle*>(&other); pRect) {
			double lhs_length = Point::distance(_vertex[0], _vertex[1]);
			double rhs_length = Point::distance(pRect->_vertex[0], pRect->_vertex[1]);
			double lhs_width = Point::distance(_vertex[1], _vertex[2]);
			double rhs_width = Point::distance(pRect->_vertex[1], pRect->_vertex[2]);

			bool are_lengths_equal = _double_equal(lhs_length, rhs_length) or
				_double_equal(lhs_length, rhs_width); // If the rectangle is inverted
			bool are_widths_equal = _double_equal(lhs_width, rhs_width) or
				_double_equal(lhs_width, rhs_length); // If the rectangle is inverted
			if (are_lengths_equal and are_widths_equal) {
				return true;
			}
		}
		return Polygon::is_congruent_to(other);
	}

	bool Rectangle::is_similar_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Square* pSquare = dynamic_cast<const Square*>(&other); pSquare) {
			double length = Point::distance(_vertex[0], _vertex[1]);
			double width = Point::distance(_vertex[1], _vertex[2]);
			if (_double_equal(length, width)) {
				return true;
			}
		}
		else if (const Rectangle* pRect = dynamic_cast<const Rectangle*>(&other); pRect) {
			double lhs_length = Point::distance(_vertex[0], _vertex[1]);
			double rhs_length = Point::distance(pRect->_vertex[0], pRect->_vertex[1]);
			double lhs_width = Point::distance(_vertex[1], _vertex[2]);
			double rhs_width = Point::distance(pRect->_vertex[1], pRect->_vertex[2]);
			if (lhs_length / rhs_length == lhs_width / rhs_width or
				lhs_length / rhs_width == lhs_width / rhs_length) { // If the rectangle is inverted
				return true;
			}
		}
		return Polygon::is_similar_to(other);
	}

	double Rectangle::perimeter() const
	{
		return 2.0 * (Point::distance(_vertex[0], _vertex[1]) + Point::distance(_vertex[1], _vertex[2]));
	}

	double Rectangle::area() const
	{
		return Point::distance(_vertex[0], _vertex[1]) * Point::distance(_vertex[1], _vertex[2]);
	}

	bool Rectangle::contains_point(const Point& point) const
	{
		/// Formula d = (x2 - x1) * (yp - y2) - (xp - x2) * (y2 - y1)
		/// Clockwise bypass
		/// If for all of d are less than 0 then the point inside the rectangle

		for (Point start = _vertex.back(); const Point & end : _vertex) {
			if (_cross_product(start, end, point) > 0.0) {
				return false;
			}
			start = end;
		}
		return true;
	}

	bool Rectangle::is_convex() const
	{
		return true;
	}

	Point Rectangle::center() const
	{
		return { (_vertex[0].x + _vertex[2].x) / 2.0, (_vertex[0].y + _vertex[2].y) / 2.0 };
	}

	std::pair<Line, Line> Rectangle::diagonals() const
	{
		return { Line(_vertex[0], _vertex[2]), Line(_vertex[3], _vertex[1]) };
	}

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	/*++++++++++++++++++++++++___class Square___++++++++++++++++++++++++++*/
	Square::Square(const Point& left_top, double length)
		: Rectangle(left_top, Point{ left_top.x + length, left_top.y + length }) {}

	bool Square::is_congruent_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Square* pSquare = dynamic_cast<const Square*>(&other); pSquare) {
			double lhs_length = Point::distance(_vertex[0], _vertex[1]);
			double rhs_length = Point::distance(pSquare->_vertex[0], pSquare->_vertex[1]);
			if (_double_equal(lhs_length, rhs_length)) {
				return true;
			}
		}
		return Rectangle::is_congruent_to(other);
	}

	bool Square::is_similar_to(const Shape& other) const
	{
		if (this == &other) {
			return true;
		}
		if (const Square* pSquare = dynamic_cast<const Square*>(&other); pSquare) {
			return true;
		}
		return Rectangle::is_similar_to(other);
	}

	Circle Square::circumscribed_circle() const
	{
		double length = Point::distance(_vertex[0], _vertex[1]);
		return Circle(center(), length / std::sqrt(2.0));
	}

	Circle Square::inscribed_circle() const
	{
		double length = Point::distance(_vertex[0], _vertex[1]);
		return Circle(center(), length / 2.0);
	}
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

	/*++++++++++++++++++++++++___class Triangle___++++++++++++++++++++++++*/
	Triangle::Triangle(const Point& pt1, const Point& pt2, const Point& pt3)
		: Polygon({ pt1, pt2, pt3 }) {}

	bool Triangle::is_congruent_to(const Shape& other) const
	{
		return Polygon::is_congruent_to(other);
	}

	bool Triangle::is_similar_to(const Shape& other) const
	{
		return Polygon::is_similar_to(other);
	}

	double Triangle::perimeter() const
	{
		return Polygon::perimeter();
	}

	double Triangle::area() const
	{
		double a = Point::distance(_vertex[0], _vertex[1]);
		double b = Point::distance(_vertex[1], _vertex[2]);
		double c = Point::distance(_vertex[2], _vertex[0]);
		double p = (a + b + c) / 2.0;
		return std::sqrt(p * (p - a) * (p - b) * (p - c)); // Heron's formula
	}

	bool Triangle::contains_point(const Point& point) const
	{
		// https://www.cyberforum.ru/algorithms/thread144722.html
		// (x1 - x0) * (y2 - y1) - (x2 - x1) * (y1 - y0)
		// (x2 - x0) * (y3 - y2) - (x3 - x2) * (y2 - y0)
		// (x3 - x0) * (y1 - y3) - (x1 - x3) * (y3 - y0)
		// 1, 2, 3 - the vertices of the triangle, 0 - the point
		// If they are of the same sign, then the point is inside the Triangle
		bool sign_a = _cross_product(_vertex[0], _vertex[1], point);
		bool sign_b = _cross_product(_vertex[1], _vertex[2], point);
		bool sign_c = _cross_product(_vertex[2], _vertex[0], point);
		return sign_a and sign_b and sign_c or
			!sign_a and !sign_b and !sign_c;
	}

	bool Triangle::is_convex() const
	{
		return true;
	}

	Circle Triangle::circumscribed_circle() const
	{
		// https://ru.wikipedia.org/wiki/Описанная_окружность#Уравнения_окружности
		double a = Point::distance(_vertex[0], _vertex[1]);
		double b = Point::distance(_vertex[1], _vertex[2]);
		double c = Point::distance(_vertex[2], _vertex[0]);
		double radius = (a * b * c) / (4 * area());

		double xa = _vertex[0].x;
		double xb = _vertex[1].x;
		double xc = _vertex[2].x;
		double ya = _vertex[0].y;
		double yb = _vertex[1].y;
		double yc = _vertex[2].y;

		Point center =
		{
			-(ya * (xb * xb + yb * yb - xc * xc - yc * yc) + yb * (xc * xc + yc * yc - xa * xa - ya * ya) + yc * (xa * xa + ya * ya - xb * xb - yb * yb)) /
			(2.0 * (xa * (yb - yc) + xb * (yc - ya) + xc * (ya - yb))),

			(xa * (xb * xb + yb * yb - xc * xc - yc * yc) + xb * (xc * xc + yc * yc - xa * xa - ya * ya) + xc * (xa * xa + ya * ya - xb * xb - yb * yb)) /
			(2.0 * (xa * (yb - yc) + xb * (yc - ya) + xc * (ya - yb)))
		};
		return Circle(center, radius);
	}

	Circle Triangle::inscribed_circle() const
	{
		/*
					 b
				   /  \
				  /    \
				 /      \
				/        \
			   /          \
			  a------------c
		*/
		// Bisector between ab and ac
		std::pair<double, double> ab =
		{
			_vertex[1].x - _vertex[0].x,
			_vertex[1].y - _vertex[0].y
		};

		std::pair<double, double> ac =
		{
			_vertex[2].x - _vertex[0].x,
			_vertex[2].y - _vertex[0].y
		};

		double len_ab = std::sqrt(ab.first * ab.first + ab.second * ab.second);
		double len_ac = std::sqrt(ac.first * ac.first + ac.second * ac.second);
		std::pair<double, double> vec_bisector =
		{
			ab.first / len_ab + ac.first / len_ac,
			ab.second / len_ab + ac.second / len_ac
		};
		Point a1(vec_bisector.first + _vertex[0].x,
			vec_bisector.second + _vertex[0].y);
		Line bisectorA(_vertex[0], a1);

		// Bisector between ba and bc
		std::pair<double, double> ba =
		{
			_vertex[0].x - _vertex[1].x,
			_vertex[0].y - _vertex[1].y
		};

		std::pair<double, double> bc =
		{
			_vertex[2].x - _vertex[1].x,
			_vertex[2].y - _vertex[1].y
		};

		double len_ba = std::sqrt(ba.first * ba.first + ba.second * ba.second);
		double len_bc = std::sqrt(bc.first * bc.first + bc.second * bc.second);
		vec_bisector =
		{
			ba.first / len_ba + bc.first / len_bc,
			ba.second / len_ba + bc.second / len_bc
		};
		Point b1(vec_bisector.first + _vertex[1].x,
			vec_bisector.second + _vertex[1].y);
		Line bisectorB(_vertex[1], b1);

		Point center = bisectorA.common_point(bisectorB);
		double radius = 2.0 * area() / perimeter();
		return Circle(center, radius);
	}

	Point Triangle::centroid() const
	{
		return Point((_vertex[0].x + _vertex[1].x + _vertex[2].x) / 3.0,
			(_vertex[0].y + _vertex[1].y + _vertex[2].y) / 3.0);
	}

	Point Triangle::orthocenter() const
	{
		Line ab(_vertex[0], _vertex[1]);
		Line ac(_vertex[0], _vertex[2]);
		Line height_to_ab = ab.perpendicular_line(_vertex[2]);
		Line height_to_ac = ac.perpendicular_line(_vertex[1]);
		return height_to_ab.common_point(height_to_ac);
	}

	Line Triangle::euler_line() const
	{
		return Line(centroid(), orthocenter());
	}

	Circle Triangle::nine_points_circle() const
	{
		Circle circum = circumscribed_circle();
		Point orthto = orthocenter();
		Point center((circum.center().x + orthto.x) / 2.0, (circum.center().y + orthto.y) / 2.0);
		return Circle(center, circum.radius() / 2.0);
	}
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
}
