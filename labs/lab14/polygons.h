#ifndef _polygons_h_
#define _polygons_h_

#include "utilities.h"
#include <string>
#include <vector>
#include <cassert>

class Polygon {
public:
	Polygon(const std::string& n, const std::vector<Point>& p) : name(n), points(p) {
		for (int i = 0; i < points.size(); i++) {
			sides.push_back(DistanceBetween(p[i],p[(i+1)%points.size()]));
		}
		for (int i = 0; i < points.size(); i++) {
			angles.push_back(Angle(p[i],p[(i+1)%points.size()], p[(i+2)%points.size()]));
		}
	}
	virtual ~Polygon() = default;
	std::string getName() const { return name; }
	bool HasAllEqualSides() { 
		for (int i = 0; i < sides.size()-1; i++) {
			if (!EqualSides(sides[i], sides[i+1])) {
				return false;
			}
		}
		return EqualSides(sides[0], sides[sides.size()-1]);
	}
	bool HasAllEqualAngles() {
		for (int i = 0; i < angles.size()-1; i++) {
			if (!EqualAngles(angles[i], angles[i+1])) {
				return false;
			}
		}
		return EqualAngles(angles[0], angles[angles.size()-1]);
	}
	bool HasARightAngle() {
		for (int i = 0; i < angles.size(); i++) {
			if (RightAngle(angles[i])) {
				return true;
			}
		}
		return false;
	}

protected:
	std::string name;
	std::vector<Point> points;
	std::vector<double> sides;
	std::vector<double> angles;

};

class Quadrilateral: public Polygon {
public:
	Quadrilateral(const std::string& n, const std::vector<Point>& p) : Polygon(n, p) {
		if (p.size() != 4) {
			throw(-1);
		}
	}

};

class Rectangle: public Quadrilateral {
public:
	Rectangle(const std::string& n, const std::vector<Point>& p) : Quadrilateral(n, p) {
		double angle1 = Angle(p[0],p[1],p[2]);
		double angle2 = Angle(p[1],p[2],p[3]);
		double angle3 = Angle(p[2],p[3],p[0]);
		//if there are 3 right angles, it must be a rectangle
		if (!RightAngle(angle1) || !RightAngle(angle2) || !RightAngle(angle3)) {
			throw(-1);
		}
	}

};

class Square: public Rectangle {
public:
	Square(const std::string& n, const std::vector<Point>& p) : Rectangle(n, p) {
		double side1 = DistanceBetween(p[0],p[1]);
		double side2 = DistanceBetween(p[1],p[2]);
		//if two adjacent sides are equal, it must be a square
		if (!EqualSides(side1, side2)) {
			throw(-1);
		}
	}
};

class Triangle: public Polygon {
public:
	Triangle(const std::string& n, const std::vector<Point>& p) : Polygon(n, p) {
		if (p.size() != 3) {
			throw(-1);
		}
	}

};

class IsoscelesTriangle: virtual public Triangle {
public:
	IsoscelesTriangle(const std::string& n, const std::vector<Point>& p) : Triangle(n, p) {
		//two angles must be the same
		if (!(EqualAngles(angles[0], angles[1]) || EqualAngles(angles[1], angles[2]) ||
			EqualAngles(angles[2], angles[0]))) {
			throw(-1);
		}
	}

};

class EquilateralTriangle: public IsoscelesTriangle {
public:
	EquilateralTriangle(const std::string& n, const std::vector<Point>& p) : IsoscelesTriangle(n, p) {
		//all angles must be the same
		if (!EqualAngles(angles[0], angles[1]) || !EqualAngles(angles[1], angles[2]) ||
			!EqualAngles(angles[2], angles[0])) {
			throw(-1);
		}
	}
};

class RightTriangle: virtual public Triangle {
public:
	RightTriangle(const std::string& n, const std::vector<Point>& p) : Triangle(n, p) {
		//one angle must be right
		if (!RightAngle(angles[0]) && !RightAngle(angles[1]) && !RightAngle(angles[2])) {
			throw(-1);
		}
	}
	bool HasARightAngle() { return true; }
};

class RightIsoscelesTriangle: public RightTriangle, public IsoscelesTriangle {
public:
	RightTriangle(const std::string& n, const std::vector<Point>& p) : Triangle(n, p), RightTriangle(n, p), 
	IsoscelesTriangle(n, p) {}
};

#endif