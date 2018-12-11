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
	friend bool operator<(const CP_Coord& l, const CP_Coord& r)
	{
		return l.m_x < r.m_x || (l.m_x == r.m_x && l.m_y < r.m_y);
	}
};

class CP_GridLevelTwo
{
public:
	double m_xmin;
	double m_ymin;
	CP_Coord m_coord;
	VT_PlanePointArray m_data;
	CP_GridLevelOne *m_gridLevelOne;
	CP_Plane *m_plane;
	bool m_visited;
public:
	CP_GridLevelTwo()
	{}
	CP_GridLevelTwo(CP_Plane *plane, CP_GridLevelOne *gridLevelOne)
		:m_gridLevelOne(gridLevelOne), m_plane(plane)
	{}
	bool subdivided() const
	{
		return false;
	}
	bool empty() const
	{
		return m_data.size() == 0;
	}
	CP_GridLevelOne* parent()
	{
		return m_gridLevelOne;
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
	bool m_visited;
public:
	CP_GridLevelOne()
		:m_subdivided(false), m_xcells(4), m_ycells(4)
	{}
	CP_GridLevelOne(CP_Plane *plane);
	bool validCoord(const CP_Coord &coord) const
	{
		return coord.m_x >= 0 && coord.m_x < m_xcells
			&& coord.m_y >= 0 && coord.m_y < m_ycells;
	}
	CP_GridLevelTwo& grid(const CP_Coord &coord)
	{
		return m_grids[coord.m_y * m_xcells + coord.m_x];
	}
	bool subdivided() const
	{
		return m_subdivided;
	}
	bool empty() const
	{
		return m_data.size() == 0;
	}
	CP_Plane* parent()
	{
		return m_plane;
	}
	void setUnVisited()
	{
		m_visited = false;
		if (subdivided())
		{
			for (CP_GridLevelTwo &grid : m_grids)
			{
				grid.m_visited = false;
			}
		}
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
	void mb_clear()
	{
		m_data.clear();
		m_grids.clear();
	}
	bool validCoord(const CP_Coord &coord)
	{
		return coord.m_x >= 0 && coord.m_x < m_xcells
			&& coord.m_y >= 0 && coord.m_y < m_ycells;
	}
	CP_GridLevelOne& grid(const CP_Coord &coord)
	{
		return m_grids[coord.m_y * m_xcells + coord.m_x];
	}
	void setUnVisited()
	{
		for (CP_GridLevelOne &grid : m_grids)
		{
			grid.setUnVisited();
		}
	}
};

extern double distancePoint2Point(CP_PlanePoint &p1, CP_PlanePoint &p2);

template<class T>
extern double distancePoint2Grid(CP_PlanePoint &p, T &grid);

extern void initializationPlane(CP_Plane &plane, int n, double xmin, double xmax, double ymin, double ymax);

extern CP_Coord querypointPlane(CP_Plane &plane, CP_PlanePoint &point);

extern void insertPlane(CP_Plane &plane, CP_PlanePoint &point);

extern CP_Coord querypointGridLevelOne(const CP_GridLevelOne &grid1, const CP_PlanePoint &point);

extern void insertGridLevelOne(CP_GridLevelOne &grid1, CP_PlanePoint &point);

extern void insertGridLevelTwo(CP_GridLevelTwo &grid2, CP_PlanePoint &point);

extern void subdivideGridLevelOne(CP_GridLevelOne &grid1);

extern void serachNearestVertex(CP_Plane &plane, CP_PlanePoint &point, vector<pair<int, double> > &points);

extern void floodFill(CP_PlanePoint &point, CP_GridLevelOne &grid, vector<pair<int, double> > &points, double &distance);

extern void floodFill(CP_PlanePoint &point, CP_GridLevelTwo &grid, vector<pair<int, double> > &points, double &distance);

template <class T>
T& findNeighbor(T &grid, CP_PlanePoint &p, double distance);

#endif
