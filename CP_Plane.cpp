#include "stdafx.h"

#include "CP_Plane.h"

CP_GridLevelOne::CP_GridLevelOne(CP_Plane *plane)
	:m_subdivided(false), m_plane(plane), m_xcells(4), m_ycells(4)
{
	m_xsize = m_plane->m_xsize / m_xcells;
	m_ysize = m_plane->m_ysize / m_ycells;
}

double distancePoint2Point(CP_PlanePoint &p1, CP_PlanePoint &p2)
{
	double dx = p1.m_x - p2.m_x;
	double dy = p1.m_y - p2.m_y;

	return sqrt(dx * dx + dy * dy);
}

void initializationPlane(CP_Plane &plane, int n, double xmin, double xmax, double ymin, double ymax)
{
	double ratioXY = (xmax - xmin) / (ymax - ymin);
	plane.m_xmin = xmin;
	plane.m_ymin = ymin;
	plane.m_xcells = ratioXY * sqrt(n + 1.0) / 2.0;
	plane.m_ycells = sqrt(n + 1.0) * ratioXY / 2.0;
	plane.m_xsize = (xmax - xmin) / plane.m_xcells;
	plane.m_ysize = (ymax - ymin) / plane.m_ycells;
	plane.threshold = 4.0 * n / (plane.m_xcells * plane.m_ycells);

	plane.m_grids.resize(plane.m_xcells * plane.m_ycells, CP_GridLevelOne(&plane));
	for (int i = 0; i < plane.m_xcells; i++)
	{
		for (int j = 0; j < plane.m_ycells; j++)
		{
			CP_GridLevelOne& grid1 = plane.grid(i, j);
			grid1.m_coord = CP_Coord(i, j);
			grid1.m_xmin = plane.m_xmin + i * plane.m_xsize;
			grid1.m_ymin = plane.m_ymin + j * plane.m_ysize;
		}
	}
}

CP_Coord querypointPlane(CP_Plane &plane, CP_PlanePoint &point)
{
	int xpos = (point.m_x - plane.m_xmin) / plane.m_xsize;
	int ypos = (point.m_y - plane.m_ymin) / plane.m_ysize;

	return CP_Coord(xpos, ypos);
}

void insertPlane(CP_Plane &plane, CP_PlanePoint &point)
{
	if (plane.m_data.size() < 500)
	{
		plane.m_data.push_back(point);
	}
	else if (plane.m_data.size() > 500)
	{
		CP_Coord coord1 = querypointPlane(plane, point);
		CP_GridLevelOne& grid1 = plane.grid(coord1);

		insertGridLevelOne(grid1, point);
	}
	else
	{
		plane.m_data.push_back(point);
		for (CP_PlanePoint &p : plane.m_data)
		{
			CP_Coord coord1 = querypointPlane(plane, p);
			CP_GridLevelOne& grid1 = plane.grid(coord1);

			insertGridLevelOne(grid1, p);
		}
	}
}

CP_Coord querypointGridLevelOne(CP_GridLevelOne &grid1, CP_PlanePoint &point)
{
	int xpos = (point.m_x - grid1.m_xmin) / grid1.m_xsize;
	int ypos = (point.m_y - grid1.m_ymin) / grid1.m_ysize;

	return CP_Coord(xpos, ypos);
}

void insertGridLevelOne(CP_GridLevelOne &grid1, CP_PlanePoint &point)
{
	if (grid1.subdivided())
	{
		CP_Coord coord2 = querypointGridLevelOne(grid1, point);
		CP_GridLevelTwo& grid2 = grid1.grid(coord2);

		insertGridLevelTwo(grid2, point);
	}
	else
	{
		grid1.m_data.push_back(point);
		if (grid1.m_data.size() >= 16)
		{
			subdivideGridLevelOne(grid1);
		}
	}
}

void insertGridLevelTwo(CP_GridLevelTwo &grid2, CP_PlanePoint &point)
{
	grid2.m_data.push_back(point);
}

void subdivideGridLevelOne(CP_GridLevelOne &grid1)
{
	grid1.m_subdivided = true;
	grid1.m_grids.resize(grid1.m_xcells * grid1.m_ycells, CP_GridLevelTwo(grid1.m_plane, &grid1));
	for (CP_PlanePoint &p : grid1.m_data)
	{
		insertGridLevelOne(grid1, p);
	}
	//grid1.m_data.clear();
}

void serachNearestVertex(CP_Plane &plane, CP_PlanePoint &point)
{
	CP_Coord coord1 = querypointPlane(plane, point);
	CP_GridLevelOne& grid1 = plane.grid(coord1);
	double distance = distancePoint2Point(point, plane.m_data[0]);
	VT_PlanePointArray points;

	floodFill(point, grid1, points, distance);
}

template <class T>
void floodFill(const CP_PlanePoint &point, const T &grid, VT_PlanePointArray &points, double &distance)
{
	if (grid.subdivided())
	{
		CP_Coord coord2 = querypointGridLevelOne(grid, point);
		CP_GridLevelTwo& grid2 = grid.grid(coord2);
		floodFill(point, grid2, points, distance)
	}


}