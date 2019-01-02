#include "stdafx.h"

#include "accurate_point_in_triangle.h"

// http://totologic.blogspot.com/2014/01/accurate-point-in-triangle-test.html

double side(double x1, double y1, double x2, double y2, double x, double y)
{
	double pa[2] = { x1, y1 };
	double pb[2] = { x2, y2 };
	double pc[2] = { x, y };

	double result = orient2d(pa, pb, pc);

	if (result == 0)
	{
		return 0;
	}
	else if (result > 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

bool naivePointInTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y)
{
	bool checkSide1 = side(x1, y1, x2, y2, x, y) >= 0;
	bool checkSide2 = side(x2, y2, x3, y3, x, y) >= 0;
	bool checkSide3 = side(x3, y3, x1, y1, x, y) >= 0;

	return checkSide1 && checkSide2 && checkSide3;
}

bool pointInTriangleBoundingBox(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y)
{
	double xMin = min(x1, min(x2, x3)) - DBL_EPSILON;
	double xMax = max(x1, max(x2, x3)) + DBL_EPSILON;
	double yMin = min(y1, min(y2, y3)) - DBL_EPSILON;
	double yMax = max(y1, max(y2, y3)) + DBL_EPSILON;

	if (x < xMin || xMax < x || y < yMin || yMax < y)
		return false;
	else
		return true;
}

double distanceSquarePointToSegment(double x1, double y1, double x2, double y2, double x, double y)
{
	double p1_p2_squareLength = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
	double dotProduct = ((x - x1)*(x2 - x1) + (y - y1)*(y2 - y1)) / p1_p2_squareLength;
	if (dotProduct < 0)
	{
		return (x - x1)*(x - x1) + (y - y1)*(y - y1);
	}
	else if (dotProduct <= 1)
	{
		double p_p1_squareLength = (x1 - x)*(x1 - x) + (y1 - y)*(y1 - y);
		return p_p1_squareLength - dotProduct * dotProduct * p1_p2_squareLength;
	}
	else
	{
		return (x - x2)*(x - x2) + (y - y2)*(y - y2);
	}
}

bool accuratePointInTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y)
{
	if (!pointInTriangleBoundingBox(x1, y1, x2, y2, x3, y3, x, y))
		return false;

	if (naivePointInTriangle(x1, y1, x2, y2, x3, y3, x, y))
		return true;

	//if (distanceSquarePointToSegment(x1, y1, x2, y2, x, y) <= DBL_EPSILON)
	//	return true;
	//if (distanceSquarePointToSegment(x2, y2, x3, y3, x, y) <= DBL_EPSILON)
	//	return true;
	//if (distanceSquarePointToSegment(x3, y3, x1, y1, x, y) <= DBL_EPSILON)
	//	return true;

	return false;
}
