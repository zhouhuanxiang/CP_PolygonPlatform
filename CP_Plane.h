#ifndef CP_Plane_H
#define CP_Plane_H
#include <iostream>
using namespace std;
#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
#include <iostream>
#include <memory>

#include "CP_Polygon.h"

class CP_PlanePoint;
class CP_Plane;
class CP_GridLevelOne;
class CP_GridLevelTwo;

typedef vector<CP_PlanePoint> VT_PlanePointArray;
typedef vector<CP_GridLevelOne> VT_GridLevelOneArray;
typedef vector<CP_GridLevelTwo> VT_GridLevelTwoArray;

class CP_PlanePoint
{
public:
	double m_x, m_y;
	int m_index;
public:
	CP_PlanePoint(void) :m_x(0.0), m_y(0.0), m_index(-1) {}
	CP_PlanePoint(int index, double x, double y) :m_index(index), m_x(x), m_y(y) {};
}; 

class CP_Coord
{
public:
	int m_x;
	int m_y;
public:
	CP_Coord()
		:m_x(-1), m_y(-1)
	{}
	CP_Coord(int x, int y)
		:m_x(x), m_y(y)
	{}
};

class CP_GridLevelTwo
{
public:
	CP_Coord m_coord;
	VT_PlanePointArray m_data;
	CP_GridLevelOne *m_gridLevelTwo;
	CP_Plane *m_plane;
public:
	CP_GridLevelTwo()
	{}
	CP_GridLevelTwo(CP_Plane *plane, CP_GridLevelOne *gridLevelTwo)
		:m_gridLevelTwo(gridLevelTwo), m_plane(plane)
	{}
	bool subdivided()
	{
		return false;
	}
};

class CP_GridLevelOne
{
public:
	double m_xmin;
	double m_ymin;
	int m_xcells;
	int m_ycells;
	double m_xsize;
	double m_ysize;
	CP_Coord m_coord;
	VT_GridLevelTwoArray m_grids;
	VT_PlanePointArray m_data;
	bool m_subdivided;
	CP_Plane *m_plane;
public:
	CP_GridLevelOne()
		:m_subdivided(false), m_xcells(4), m_ycells(4)
	{}
	CP_GridLevelOne(CP_Plane *plane);
	CP_GridLevelTwo& grid(CP_Coord &coord)
	{
		return m_grids[coord.m_y * m_xcells + coord.m_x];
	}
	CP_GridLevelTwo& grid(int x, int y)
	{
		return m_grids[y * m_xcells + x];
	}
	bool subdivided()
	{
		return m_subdivided;
	}
};

class CP_Plane
{
public:
	double m_xmin;
	double m_ymin;
	int m_xcells;
	int m_ycells;
	double m_xsize;
	double m_ysize;
	double threshold;
	VT_PlanePointArray m_data;
	VT_GridLevelOneArray m_grids;
public:
	CP_Plane()
	{}
	CP_GridLevelOne& grid(CP_Coord &coord)
	{
		return m_grids[coord.m_y * m_xcells + coord.m_x];
	}
	CP_GridLevelOne& grid(int x, int y)
	{
		return m_grids[y * m_xcells + x];
	}
};

extern double distancePoint2Point(CP_PlanePoint &p1, CP_PlanePoint &p2);

extern void initializationPlane(CP_Plane &plane, int n, double xmin, double xmax, double ymin, double ymax);

extern CP_Coord querypointPlane(CP_Plane &plane, CP_PlanePoint &point);

extern void insertPlane(CP_Plane &plane, CP_PlanePoint &point);

extern CP_Coord querypointGridLevelOne(CP_GridLevelOne &grid1, CP_PlanePoint &point);

extern void insertGridLevelOne(CP_GridLevelOne &grid1, CP_PlanePoint &point);

extern void insertGridLevelTwo(CP_GridLevelTwo &grid2, CP_PlanePoint &point);

extern void subdivideGridLevelOne(CP_GridLevelOne &grid1);

#endif
