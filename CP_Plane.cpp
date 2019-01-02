#include "stdafx.h"

#include "CP_Plane.h"

#define BRUTE_LIMIT 20
#define SUBDIVIDE_LIMIT 16

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

template<class T>
double distancePoint2Grid(CP_PlanePoint &p, T &grid)
{
	double xmin = grid.m_xmin;
	double xmax = grid.m_xmin + grid.parent()->m_xsize;
	double ymin = grid.m_ymin;
	double ymax = grid.m_ymin + grid.parent()->m_ysize;

	double dx = 0;
	double dy = 0;

	if (p.m_x >= xmin && p.m_x <= xmax)
	{
		return min(abs(p.m_y - ymin), abs(p.m_y - ymax));
	}
	else if (p.m_y >= ymin && p.m_y <= ymax)
	{
		return min(abs(p.m_x - xmin), abs(p.m_x - xmax));
	}
	else if (p.m_x <= xmin && p.m_y <= ymin)
	{
		dx = p.m_x - xmin;
		dy = p.m_y - ymin;
	}
	else if (p.m_x <= xmin && p.m_y >= ymax)
	{
		dx = p.m_x - xmin;
		dy = p.m_y - ymax;
	}
	else if (p.m_x >= xmax && p.m_y <= ymin)
	{
		dx = p.m_x - xmax;
		dy = p.m_y - ymin;
	}
	else if (p.m_x >= xmax && p.m_y >= ymax)
	{
		dx = p.m_x - xmax;
		dy = p.m_y - ymax;
	}
	return sqrt(dx * dx + dy * dy);
}

void initializationPlane(CP_Plane &plane, int n, double xmin, double xmax, double ymin, double ymax)
{
	double ratioXY = (xmax - xmin) / (ymax - ymin);
	plane.m_xmin = xmin;
	plane.m_ymin = ymin;
	plane.m_xcells = ratioXY * sqrt(n) / 2.0 + 1;
	plane.m_ycells = sqrt(n) * ratioXY / 2.0 + 1;
	plane.m_xsize = 1.1 * (xmax - xmin) / plane.m_xcells;
	plane.m_ysize = 1.1 * (ymax - ymin) / plane.m_ycells;
	plane.threshold = 4.0 * n / (plane.m_xcells * plane.m_ycells);

	plane.m_grids.resize(plane.m_xcells * plane.m_ycells, CP_GridLevelOne(&plane));
	for (int i = 0; i < plane.m_xcells; i++)
	{
		for (int j = 0; j < plane.m_ycells; j++)
		{
			CP_GridLevelOne& grid1 = plane.grid(CP_Coord(i, j));
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
	//if (point.m_index == 48)
	//{
	//	TRACE("debug\n");
	//}
	if (plane.m_data.size() < BRUTE_LIMIT)
	{
		plane.m_data.push_back(point);
	}
	else if (plane.m_data.size() > BRUTE_LIMIT)
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

CP_Coord querypointGridLevelOne(const CP_GridLevelOne &grid1, const CP_PlanePoint &point)
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
		if (grid1.m_data.size() >= SUBDIVIDE_LIMIT)
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
	for (int i = 0; i < grid1.m_xcells; i++)
	{
		for (int j = 0; j < grid1.m_ycells; j++)
		{
			CP_GridLevelTwo& grid2 = grid1.grid(CP_Coord(i, j));
			grid2.m_coord = CP_Coord(i, j);
			grid2.m_xmin = grid1.m_xmin + i * grid1.m_xsize;
			grid2.m_ymin = grid1.m_ymin + j * grid1.m_ysize;
		}
	}
	for (CP_PlanePoint &p : grid1.m_data)
	{
		insertGridLevelOne(grid1, p);
	}
	//grid1.m_data.clear();
}

void serachNearestVertex(CP_Plane &plane, CP_PlanePoint &point, vector<pair<int, double> > &points)
{
	points.clear();
	points.reserve(BRUTE_LIMIT);
	if (plane.m_data.size() <= BRUTE_LIMIT)
	{
		// 如果平面上的点少于 BRUTE_LIMIT，暴力搜索
		for (CP_PlanePoint &p : plane.m_data)
		{
			points.push_back(pair<int, double>(p.m_index, distancePoint2Point(p, point)));
		}
	}
	else
	{
		CP_Coord coord1 = querypointPlane(plane, point);
		CP_GridLevelOne& grid1 = plane.grid(coord1);
		double distance = DBL_MAX;
		int index = -1;
		// 如果平面上的点多于 BRUTE_LIMIT，先粗略搜索，减小后面的搜索范围
		for (int i = 0; i < plane.m_data.size(); i += 50)
		{
			double d = distancePoint2Point(point, plane.m_data[0]);
			if (distance > d)
			{
				distance = d;
				index = plane.m_data[0].m_index;
			}
		}
		points.push_back(pair<int, double>(index, distance));
		// 再进行floodfill搜索
		plane.setUnVisited();
		//
		floodFill(point, grid1, points, distance);
	}

	// 搜索结果升序排列
	sort(points.begin(), points.end(),
		[](const pair<int, double> &a, const pair<int, double> &b) -> bool
	{
		return a.second < b.second;
	});

	cout << points[0].first << "\n";
}

void floodFill(CP_PlanePoint &point, CP_GridLevelOne &grid, vector<pair<int, double> > &points, double &distance)
{
	cout << "one " << grid.m_coord.m_x << " " << grid.m_coord.m_y << "\n";

	if (grid.m_visited)
		return;
	grid.m_visited = true;

	if (grid.subdivided())
	{
		// 如果已经被细分了，子格中搜索
		CP_Coord coord2 = querypointGridLevelOne(grid, point);
		if (grid.validCoord(coord2))
		{
			CP_GridLevelTwo& grid2 = grid.grid(coord2);
			floodFill(point, grid2, points, distance);
		}
		else
		{
			for (CP_GridLevelTwo& grid2 : grid.m_grids)
			{
				floodFill(point, grid2, points, distance);
			}
		}
	}
	else
	{
		// 未被细分，直接搜索
		for (CP_PlanePoint &p : grid.m_data)
		{
			double d = distancePoint2Point(point, p);
			distance = min(distance, d);
			points.push_back(pair<int, double>(p.m_index, d));
		}
	}
	// 在邻居中搜索
	CP_GridLevelOne& neighbor = findNeighbor(grid, point, distance);
	if (neighbor.m_coord.m_x != grid.m_coord.m_x
		|| neighbor.m_coord.m_y != grid.m_coord.m_y)
	{
		floodFill(point, neighbor, points, distance);
	}
}

void floodFill(CP_PlanePoint &point, CP_GridLevelTwo &grid, vector<pair<int, double> > &points, double &distance)
{
	cout << "two " << grid.m_coord.m_x << " " << grid.m_coord.m_y << "\n";

	if (grid.m_visited)
		return;
	grid.m_visited = true;

	// 直接搜索
	for (CP_PlanePoint &p : grid.m_data)
	{
		double d = distancePoint2Point(point, p);
		distance = min(distance, d);
		points.push_back(pair<int, double>(p.m_index, d));
	}
	// 在邻居中搜索
	CP_GridLevelTwo &neighbor = findNeighbor(grid, point, distance);
	if (neighbor.m_coord.m_x != grid.m_coord.m_x
		|| neighbor.m_coord.m_y != grid.m_coord.m_y)
	{
		floodFill(point, neighbor, points, distance);
	}
}

template <class T>
T& findNeighbor(T &grid, CP_PlanePoint &p, double distance)
{
	int cell_distance = 0;
	while (true)
	{
		cell_distance++;
		bool found = false;
		// 查询L1距离为cell_distance的网格，存在集合中
		set<CP_Coord> coords;
		for (int x_cell_distance = -1 * cell_distance; x_cell_distance <= cell_distance; x_cell_distance++)
		{
			int y_cell_distance = cell_distance - abs(x_cell_distance);
			coords.insert(CP_Coord(grid.m_coord.m_x + x_cell_distance, grid.m_coord.m_y + y_cell_distance));
			coords.insert(CP_Coord(grid.m_coord.m_x + x_cell_distance, grid.m_coord.m_y - y_cell_distance));
		}
		// 查找距离 < distance，而且未被访问的网格
		for (const CP_Coord &coord : coords)
		{
			if (grid.parent()->validCoord(coord))
			{
				T& tmp = grid.parent()->grid(coord);
				if (distancePoint2Grid(p, tmp) < distance)
				{
					found = true;
					if (!tmp.m_visited && tmp.m_data.size() > 0)
					{
						return tmp;
					}
				}
			}
		}
		// 如果所有网格距离大于 > distance，停止查找
		if (!found)
		{
			return grid;
		}
	}
}