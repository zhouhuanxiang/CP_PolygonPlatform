#ifndef ACCURATE_POINT_IN_TRIANGLE_H_
#define ACCURATE_POINT_IN_TRIANGLE_H_

#include <algorithm>
using namespace std;

extern "C"
{
#include "predicates.h"
}

extern bool accuratePointInTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y);

#endif