#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include "d3dUtil.h"
#include <crtdbg.h>
#include <string>

class Shape
{
public:
	Shape();

	struct Point
	{
		Point()	{};
		Point(double a_x, double a_y)	{ x = a_x; y = a_y;};
		double x;
		double y;
	};

	struct AABB
	{
		AABB() {left = 99999; top = 999999; right = 0; bottom = 0;};
		AABB(double l, double t, double r, double b) { left = l; top = t; right = r; b = bottom;};

		double left;
		double top;
		double right;
		double bottom;
	};

	void addPoint(Point point);
	AABB getAABB(void);
	RECT getRect(void);
	void scale(direction side, double dwidth, double dheight);

	AABB aabb;
	Point origin;
	int sides;
	std::vector<Point> pointList;
	//Point center;
};

#endif