#include "stdafx.h"

#include "CP_Triangle.h"

void CP_TriagleMesh::mb_buildTriagleMesh(CP_Polygon& pn)
{
	m_polygon = &pn;
	//int n = pn.m_pointArray.size();
	//if (n<3)
	//	return;
	//m_triagleIDArray.resize(1);
	//m_triagleIDArray[0].m_vertices[0] = 0;
	//m_triagleIDArray[0].m_vertices[0] = 1;
	//m_triagleIDArray[0].m_vertices[0] = 2;
} // 类CP_TriagleMesh的成员函数mb_buildTriagleMesh结束


  // 读取点集并随机化
void gb_getDelaunayVertices(CP_Plane &plane, CP_TriagleMesh &mesh, CP_Polygon& pn)
{
	mesh.mb_buildTriagleMesh(pn);
	plane.m_points = pn.m_pointArray;
}

void gb_randomizeDelaunayVertices(CP_Plane &plane)
{
	random_shuffle(plane.m_points.begin(), plane.m_points.end());
}

// 查找网格坐标
Pair_Int gb_findCoord1l(CP_Plane &plane, int idPoint)
{
	double x = plane.m_points[idPoint].m_x;
	double y = plane.m_points[idPoint].m_y;
	int posX = (int)((x - plane.m_boxXMin) / plane.m_xSize);
	int posY = (int)((x - plane.m_boxYMin) / plane.m_ySize);
	Pair_Int coord1 = Pair_Int(posX, posY);

	return coord1;
}

Pair_Int gb_findCoord2l(CP_Plane &plane, int idPoint, Pair_Int coord1)
{
	int xSub = 4;
	int ySub = 4;
	double x = plane.m_points[idPoint].m_x;
	double y = plane.m_points[idPoint].m_y;
	double xSubSize = plane.m_xSize / xSub;
	double ySubSize = plane.m_ySize / ySub;
	int subPosX = (x - plane.m_boxXMin - coord1.first * plane.m_xSize) / xSubSize;
	int subPosY = (y - plane.m_boxYMin - coord1.second * plane.m_ySize) / ySubSize;
	subPosX = min(xSub, max(0, subPosX));
	subPosY = min(ySub, max(0, subPosY));
	Pair_Int coord2 = Pair_Int(subPosX, subPosY);

	return coord2;
}

// 初始化第一阶段划分
void gb_initSubdivision(CP_Plane &plane)
{
	int i;
	int n = plane.m_points.size();
	// 参数的初始化
	plane.m_boxXMax = -1 * DBL_MAX;
	plane.m_boxXMin = DBL_MAX;
	plane.m_boxYMax = -1 * DBL_MAX;
	plane.m_boxYMin = DBL_MAX;
	for (int i = 0; i < n; i++)
	{
		plane.m_boxXMax = max(plane.m_boxXMax, plane.m_points[i].m_x);
		plane.m_boxXMin = min(plane.m_boxXMin, plane.m_points[i].m_x);
		plane.m_boxYMax = max(plane.m_boxYMax, plane.m_points[i].m_y);
		plane.m_boxYMin = min(plane.m_boxYMin, plane.m_points[i].m_y);
	}
	double ratioXY = (plane.m_boxXMax - plane.m_boxXMin) / (plane.m_boxYMax - plane.m_boxYMin);
	plane.m_noOfCellsX1 = (int)(ratioXY * sqrt(n) / 2) + 1;
	plane.m_noOfCellsY1 = (int)(sqrt(n) / ratioXY / 2) + 1;
	plane.m_xSize = (plane.m_boxXMax - plane.m_boxXMin) / plane.m_noOfCellsX1;
	plane.m_ySize = (plane.m_boxYMax - plane.m_boxYMin) / plane.m_noOfCellsY1;
	plane.m_threshold = 4 * n / (plane.m_noOfCellsX1 * plane.m_noOfCellsY1);
}

// 插入点操作：1）首先找到要插入的网格；2）再在该网格进行二阶段插入
void gb_insertToSubdivision1l(CP_Plane &plane, int idPoint)
{
	Pair_Int coord1 = gb_findCoord1l(plane, idPoint);
	// 查找要插入的网格：1）如果网格不存在，先创建该网格；2）在网格中插入
	if (plane.m_gridMap.find(coord1) == plane.m_gridMap.end())
	{
		plane.m_gridMap.insert(pair<Pair_Int, CP_PlaneGrid>(coord1, CP_PlaneGrid()));
	}
	gb_insertToSubdivision2l(plane, idPoint);
}

void gb_insertToSubdivision2l(CP_Plane &plane, int idPoint)
{
	Pair_Int coord1 = gb_findCoord1l(plane, idPoint);

	int xSub = 4;
	int ySub = 4;
	int i;
	CP_PlaneGrid &grid = plane.m_gridMap[coord1];

	if (grid.mb_divided())
	{
		Pair_Int coord2 = gb_findCoord2l(plane, idPoint, coord1);
		grid.m_points2l[coord2.first][coord2.second].push_back(idPoint);
	}
	else
	{
		grid.m_points1l.push_back(idPoint);
		if (grid.m_points1l.size() > xSub * ySub)
		{
			grid.m_points2l.resize(xSub, vector<list<int> >(ySub, list<int>()));
			for (int i = 0; i < grid.m_points1l.size(); i++)
			{
				int id = grid.m_points1l[i];
				Pair_Int coord2 = gb_findCoord2l(plane, id, coord1);
				grid.m_points2l[coord2.first][coord2.second].push_back(id);
			}
			grid.m_points1l.clear();
		}
	}
}

// 查找最近点
void gb_querySubdivision(CP_Plane &plane, int idPoint)
{
	int bruteLimit = 100;
	int i;

	// 每次搜索之前，清空搜索记录
	plane.m_distance = DBL_MAX;
	plane.m_queryResult = -1;
	plane.m_queryGrid1l.clear();
	plane.m_queryGrid2l.clear();

	if (idPoint >= bruteLimit)
	{
		Pair_Int coord1 = gb_findCoord1l(plane, idPoint);
		gb_floodFillSubdivision1l(plane, coord1, idPoint);
	}
	else
	{
		// 在点数较少时采用暴力搜索
		CP_Point idp = plane.m_points[idPoint];
		for (i = 0; i < idPoint; i++)
		{
			double d = gb_distancePointPoint(idp, plane.m_points[i]);
			if (plane.m_distance > d)
			{
				plane.m_distance = d;
				plane.m_queryResult = i;
			}
		}
	}
}

void gb_floodFillSubdivision1l(CP_Plane &plane, Pair_Int coord1, int idPoint)
{
	// 标记为已访问
	plane.m_queryGrid1l.insert(coord1);
	// 如果网格非空，先在网格内查找
	if (plane.m_gridMap.find(coord1) != plane.m_gridMap.end())
	{
		CP_PlaneGrid &grid = plane.m_gridMap[coord1];
		if (grid.mb_divided())
		{
			Pair_Int coord2 = gb_findCoord2l(plane, idPoint, coord1);
			gb_floodFillSubdivision2l(plane, coord1, coord2, idPoint);
		}
		else
		{
			//for (auto iter : grid.m_points1l)
			for(int i = 0; i < grid.m_points1l.size(); i++)
			{
				double d = gb_distancePointPoint(plane.m_points[idPoint], plane.m_points[grid.m_points1l[i]]);
				if (plane.m_distance > d)
				{
					plane.m_distance = d;
					plane.m_queryResult = grid.m_points1l[i];
				}
			}
		}
	}
	// 四近邻查找
	Pair_Int neighbors[4] = {
		Pair_Int(coord1.first - 1, coord1.second),
		Pair_Int(coord1.first + 1, coord1.second),
		Pair_Int(coord1.first, coord1.second - 1),
		Pair_Int(coord1.first, coord1.second + 1),
	};
	for (int i = 0; i < 4; i++)
	{
		if (neighbors[i].first >= 0 && neighbors[i].first < plane.m_noOfCellsX1
			&& neighbors[i].second >= 0 && neighbors[i].second < plane.m_noOfCellsY1
			&& plane.m_queryGrid1l.find(neighbors[i]) == plane.m_queryGrid1l.end()
			&& gb_distancePointCell(plane, neighbors[i], idPoint) < plane.m_distance)
		{
			gb_floodFillSubdivision1l(plane, neighbors[i], idPoint);
		}
	}
}

void gb_floodFillSubdivision2l(CP_Plane &plane, Pair_Int coord1, Pair_Int coord2, int idPoint)
{
	// 标记为已访问
	plane.m_queryGrid2l.insert(pair<Pair_Int, Pair_Int>(coord1, coord2));
	// 在网格内查找
	CP_PlaneGrid &grid = plane.m_gridMap[coord1];
	list<int> &l = grid.m_points2l[coord2.first][coord2.second];
	for (auto iter : l)
	{
		double d = gb_distancePointPoint(plane.m_points[idPoint], plane.m_points[iter]);
		if (plane.m_distance > d)
		{
			plane.m_distance = d;
			plane.m_queryResult = iter;
		}
	}
	// 四近邻查找
	Pair_Int neighbors[4] = {
		Pair_Int(coord2.first - 1, coord2.second),
		Pair_Int(coord2.first + 1, coord2.second),
		Pair_Int(coord2.first, coord2.second - 1),
		Pair_Int(coord2.first, coord2.second + 1),
	};
	int xSub = 4;
	int ySub = 4;
	for (int i = 0; i < 4; i++)
	{
		if (neighbors[i].first >= 0 && neighbors[i].first < xSub
			&& neighbors[i].second >= 0 && neighbors[i].second < xSub
			&& plane.m_queryGrid2l.find(pair<Pair_Int, Pair_Int>(coord1, neighbors[i])) == plane.m_queryGrid2l.end()
			&& gb_distancePointCell(plane, coord1, coord2, idPoint) < plane.m_distance)
		{
			gb_floodFillSubdivision2l(plane, coord1, neighbors[i], idPoint);
		}
	}
}

double gb_distancePointCell(CP_Plane &plane, Pair_Int coord1, int idPoint)
{
	double xmin = plane.m_boxXMin + coord1.first * plane.m_xSize;
	double xmax = plane.m_boxXMin + (coord1.first + 1) * plane.m_xSize;
	double ymin = plane.m_boxYMin + coord1.second * plane.m_ySize;
	double ymax = plane.m_boxYMin + (coord1.second + 1) * plane.m_ySize;

	double d = DBL_MAX;
	CP_Point idp = plane.m_points[idPoint];
	d = min(d, gb_distancePointPoint(idp, CP_Point(xmin, ymin)));
	d = min(d, gb_distancePointPoint(idp, CP_Point(xmin, ymax)));
	d = min(d, gb_distancePointPoint(idp, CP_Point(xmax, ymin)));
	d = min(d, gb_distancePointPoint(idp, CP_Point(xmax, ymax)));
	double maskX = (idp.m_x - xmin) * (idp.m_x - xmax);
	double maskY = (idp.m_y - ymin) * (idp.m_y - ymax);
	if (maskX >= 0 && maskY <= 0)
	{
		d = min(d, abs(idp.m_x - xmin));
		d = min(d, abs(idp.m_x - xmax));
	}
	if (maskX <= 0 && maskY >= 0)
	{
		d = min(d, abs(idp.m_y - ymin));
		d = min(d, abs(idp.m_y - ymax));
	}
	return d;
}

double gb_distancePointCell(CP_Plane &plane, Pair_Int coord1, Pair_Int coord2, int idPoint)
{
	int xSub = 4;
	int ySub = 4;
	double xmin = plane.m_boxXMin + (coord1.first + coord2.first / xSub) * plane.m_xSize;
	double xmax = plane.m_boxXMin + (coord1.first + (coord2.first + 1) / xSub) * plane.m_xSize;
	double ymin = plane.m_boxYMin + (coord1.second + coord2.second / ySub) * plane.m_ySize;
	double ymax = plane.m_boxYMin + (coord1.second + (coord2.second + 1) / ySub) * plane.m_ySize;

	double d = DBL_MAX;
	CP_Point idp = plane.m_points[idPoint];
	d = min(d, gb_distancePointPoint(idp, CP_Point(xmin, ymin)));
	d = min(d, gb_distancePointPoint(idp, CP_Point(xmin, ymax)));
	d = min(d, gb_distancePointPoint(idp, CP_Point(xmax, ymin)));
	d = min(d, gb_distancePointPoint(idp, CP_Point(xmax, ymax)));
	double maskX = (idp.m_x - xmin) * (idp.m_x - xmax);
	double maskY = (idp.m_y - ymin) * (idp.m_y - ymax);
	if (maskX >= 0 && maskY <= 0)
	{
		d = min(d, abs(idp.m_x - xmin));
		d = min(d, abs(idp.m_x - xmax));
	}
	if (maskX <= 0 && maskY >= 0)
	{
		d = min(d, abs(idp.m_y - ymin));
		d = min(d, abs(idp.m_y - ymax));
	}
	return d;
}

// 生成三角剖分
void gb_generateTriagleMesh(CP_Plane &plane, CP_TriagleMesh &mesh, CP_Polygon& pn)
{
	// 点初始化
	gb_getDelaunayVertices(plane, mesh, pn);
	// TODO
	//gb_randomizeDelaunayVertices(plane);
	gb_initSubdivision(plane);
	// 构造外围多边形顶点
	double M = -1;
	M = max(M, abs(plane.m_boxXMin));
	M = max(M, abs(plane.m_boxXMax));
	M = max(M, abs(plane.m_boxYMin));
	M = max(M, abs(plane.m_boxYMax));
	plane.m_points.push_back(CP_Point(3 * M, 0));
	plane.m_points.push_back(CP_Point(-3 * M, -3 * M));
	plane.m_points.push_back(CP_Point(0, 3 * M));
	// 初始化mesh并添加第1个顶点
	int vnum = plane.m_points.size();
	mesh.m_vertexArray.resize(vnum);
	mesh.m_triagleIDArray.push_back(CP_Triagle(0, vnum - 3, vnum - 2));
	mesh.m_triagleIDArray.push_back(CP_Triagle(0, vnum - 2, vnum - 1));
	mesh.m_triagleIDArray.push_back(CP_Triagle(0, vnum - 1, vnum - 3));
	mesh.m_triagleFlag.push_back(true);
	mesh.m_triagleFlag.push_back(true);
	mesh.m_triagleFlag.push_back(true);
	mesh.m_triagleIDArray[0].m_neighbors[0] = 2;
	mesh.m_triagleIDArray[0].m_neighbors[1] = -1;
	mesh.m_triagleIDArray[0].m_neighbors[2] = 1;
	mesh.m_triagleIDArray[1].m_neighbors[0] = 0;
	mesh.m_triagleIDArray[1].m_neighbors[1] = -1;
	mesh.m_triagleIDArray[1].m_neighbors[2] = 2;
	mesh.m_triagleIDArray[2].m_neighbors[0] = 1;
	mesh.m_triagleIDArray[2].m_neighbors[1] = -1;
	mesh.m_triagleIDArray[2].m_neighbors[2] = 0;
	mesh.m_vertexArray[0].m_triagles.push_back(0);
	mesh.m_vertexArray[0].m_triagles.push_back(1);
	mesh.m_vertexArray[0].m_triagles.push_back(2);
	mesh.m_vertexArray[vnum - 3].m_triagles.push_back(0);
	mesh.m_vertexArray[vnum - 3].m_triagles.push_back(2);
	mesh.m_vertexArray[vnum - 2].m_triagles.push_back(0);
	mesh.m_vertexArray[vnum - 2].m_triagles.push_back(1);
	mesh.m_vertexArray[vnum - 1].m_triagles.push_back(1);
	mesh.m_vertexArray[vnum - 1].m_triagles.push_back(2);

	Pair_Int triagles;
	for (int idPoint = 1; idPoint < vnum - 3; idPoint++)
	{
		TRACE("\n\n#########################\n%d now\n", idPoint);
		gb_querySubdivision(plane, idPoint);
		triagles = gb_findTriagleContainingPoint(plane, mesh, idPoint, plane.m_queryResult);
		VT_IntArray newTris;
		// TODO
		//if (triagles.second < 0)
		if (true)
		{
			// 创建3个新三角形
			mesh.m_triagleIDArray.push_back(CP_Triagle(idPoint, mesh.tri(triagles.first).m_points[0], mesh.tri(triagles.first).m_points[1]));
			mesh.m_triagleIDArray.push_back(CP_Triagle(idPoint, mesh.tri(triagles.first).m_points[1], mesh.tri(triagles.first).m_points[2]));
			mesh.m_triagleIDArray.push_back(CP_Triagle(idPoint, mesh.tri(triagles.first).m_points[2], mesh.tri(triagles.first).m_points[0]));
			mesh.m_triagleFlag.push_back(true);
			mesh.m_triagleFlag.push_back(true);
			mesh.m_triagleFlag.push_back(true);
			int triNum = mesh.m_triagleIDArray.size();
			newTris.resize(3);
			newTris[0] = triNum - 3;
			newTris[1] = triNum - 2;
			newTris[2] = triNum - 1;
			// 跟新三角形之间的相邻关系
			for (int i = 0; i < 3; i++)
			{
				mesh.tri(newTris[i]).m_neighbors[0] = newTris[(i + 2) % 3];
				mesh.tri(newTris[i]).m_neighbors[1] = mesh.tri(triagles.first).m_neighbors[i];
				mesh.tri(newTris[i]).m_neighbors[2] = newTris[(i + 1) % 3];
				if (mesh.tri(triagles.first).m_neighbors[i] >= 0)
				{
					for (int j = 0; j < 3; j++)
					{
						if (mesh.tri(mesh.tri(triagles.first).m_neighbors[i]).m_neighbors[j] == triagles.first)
						{
							mesh.tri(mesh.tri(triagles.first).m_neighbors[i]).m_neighbors[j] = newTris[i];
							break;
						}
					}
				}
			}
			// 跟新顶点与三角形的相邻关系
			for (int i = 0; i < 3; i++)
			{
				mesh.m_vertexArray[idPoint].m_triagles.push_back(newTris[i]);
			}
			for (int i = 0; i < 3; i++)
			{
				for (auto iter = mesh.m_vertexArray[mesh.tri(triagles.first).m_points[i]].m_triagles.begin();
					iter != mesh.m_vertexArray[mesh.tri(triagles.first).m_points[i]].m_triagles.end();
					iter++)
				{
					if (*iter == triagles.first)
					{
						mesh.m_vertexArray[mesh.tri(triagles.first).m_points[i]].m_triagles.erase(iter);
						break;
					}
				}
				mesh.m_vertexArray[mesh.tri(triagles.first).m_points[i]].m_triagles.push_back(newTris[(i + 2) % 3]);
				mesh.m_vertexArray[mesh.tri(triagles.first).m_points[i]].m_triagles.push_back(newTris[i]);
			}
			// TODO：删除原三角形？
			mesh.m_triagleFlag[triagles.first] = false;
		}
		else
		{
		}

		TRACE("tris: %d %d\n", triagles.first, triagles.second);
		gb_debugMesh(mesh);
		gb_legalizeTriagleMesh(plane, mesh, newTris);
		gb_insertToSubdivision1l(plane, idPoint);

		gb_debugMesh(mesh);
	}

	gb_finalizeTriagleMesh();
}

void gb_debugMesh(CP_TriagleMesh &mesh)
{
	TRACE("****************\n");
	TRACE("mesh debug info\n");
	TRACE("****************\n");
	for (int i = 0; i < mesh.m_triagleIDArray.size(); i++)
	{
		if (mesh.m_triagleFlag[i])
		{
			TRACE("triangle #%d \n\t neighbor %d %d %d \n\t point %d %d %d\n", 
				i,
				mesh.m_triagleIDArray[i].m_neighbors[0],
				mesh.m_triagleIDArray[i].m_neighbors[1],
				mesh.m_triagleIDArray[i].m_neighbors[2],
				mesh.m_triagleIDArray[i].m_points[0],
				mesh.m_triagleIDArray[i].m_points[1],
				mesh.m_triagleIDArray[i].m_points[2]);
		}
	}
	for (int i = 0; i < mesh.m_vertexArray.size(); i++)
	{
		if (mesh.m_vertexArray[i].m_triagles.size())
		{
			TRACE("vertex #%d \n", i);
			for (auto iter : mesh.m_vertexArray[i].m_triagles)
			{
				TRACE("%d \n", iter);
			}
		}
	}
}

Pair_Int gb_findTriagleContainingPoint(CP_Plane &plane, CP_TriagleMesh &mesh, int idPoint, int idVertex)
{
	CP_MeshVertex vertex = mesh.m_vertexArray[idVertex];
	CP_Point p = plane.m_points[idPoint];
	Pair_Int result = Pair_Int(-1, -1);
	for (int i = 0; i < vertex.m_triagles.size(); i++)
	{
		// 采用重心坐标判断点在三角形内外
		// https://en.wikipedia.org/wiki/Barycentric_coordinate_system
		CP_Point p1 = plane.m_points[mesh.tri(vertex.m_triagles[i]).m_points[0]];
		CP_Point p2 = plane.m_points[mesh.tri(vertex.m_triagles[i]).m_points[1]];
		CP_Point p3 = plane.m_points[mesh.tri(vertex.m_triagles[i]).m_points[2]];
		double det = (p2.m_y - p3.m_y) * (p1.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (p1.m_y - p3.m_y);
		double lambda1 = ((p2.m_y - p3.m_y) * (p.m_x - p3.m_x) + (p3.m_x - p2.m_x) * (p.m_y - p3.m_y)) / det;
		double lambda2 = ((p3.m_y - p1.m_y) * (p.m_x - p3.m_x) + (p1.m_x - p3.m_x) * (p.m_y - p3.m_y)) / det;
		double lambda3 = 1 - lambda1 - lambda2;
		if (lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0)
		{
			if (result.first < 0)
			{
				result.first = vertex.m_triagles[i];
			}
			else if (result.second < 0)
			{
				result.second = vertex.m_triagles[i];
			}
			else
			{
				TRACE("重心坐标计算错误!\n");
			}
		}
	}

	if (result.first < 0)
	{
		TRACE("未找到所属的三角形!\n");
	}
	return result;
}

// 合法化三角剖分
void gb_legalizeTriagleMesh(CP_Plane &plane, CP_TriagleMesh &mesh, VT_IntArray newTris)
{
	for (int i = 0; i < newTris.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			gb_emptyCircleTestAndAdjust(plane, mesh, newTris[i], j);
		}
	}
}

void gb_emptyCircleTestAndAdjust(CP_Plane &plane, CP_TriagleMesh &mesh, int idTri0, int t1)
{
	// https://en.wikipedia.org/wiki/Delaunay_triangulation
	// https://en.wikipedia.org/wiki/Determinant
	CP_Triagle& tri0 = mesh.tri(idTri0);
	int idTri1 = tri0.m_neighbors[t1];
	if (idTri1 < 0)
		return;
	CP_Triagle& tri1 = mesh.tri(idTri1);

	// 如果是第neighbor个邻居，那么 tri 三角形的第 (neighbor + 2) % 3 个顶点为不与 tri1 相邻的顶点
	int idPoint0 = (t1 + 2) % 3;
	if (tri0.m_points[idPoint0] == tri1.m_points[0]
		|| tri0.m_points[idPoint0] == tri1.m_points[1]
		|| tri0.m_points[idPoint0] == tri1.m_points[2])
	{
		TRACE("本三角形的非邻接点错误!\n");
		return;
	}
	CP_Point A = plane.m_points[tri1.m_points[0]];
	CP_Point B = plane.m_points[tri1.m_points[1]];
	CP_Point C = plane.m_points[tri1.m_points[2]];
	CP_Point D = plane.m_points[tri0.m_points[idPoint0]];
	double a, b, c, d, e, f, g, h, i;
	a = A.m_x - D.m_x;
	b = A.m_y - D.m_y;
	c = (A.m_x - D.m_x) * (A.m_x - D.m_x) + (A.m_y - D.m_y) * (A.m_y - D.m_y);
	d = B.m_x - D.m_x;
	e = B.m_y - D.m_y;
	f = (B.m_x - D.m_x) * (B.m_x - D.m_x) + (B.m_y - D.m_y) * (B.m_y - D.m_y);
	g = C.m_x - D.m_x;
	h = C.m_y - D.m_y;
	i = (C.m_x - D.m_x) * (C.m_x - D.m_x) + (C.m_y - D.m_y) * (C.m_y - D.m_y);
	double det = a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h;
	// 这里由于是顺时针存储三角形的顶点，所以与wiki不一样
	if (det < 0)
	{
		TRACE("开始进行翻转\n");
		// 进行边翻转，这里不能采取新建三角形的方式，因为会破坏 gb_legalizeTriagleMesh 的最外层循环
		// 确定tri1的非邻接定点
		int idPoint1;
		for (int iter = 0; iter < 3; iter++)
		{
			if (tri1.m_points[iter] != tri0.m_points[t1]
				&& tri1.m_points[iter] != tri0.m_points[(t1 + 1) % 3])
			{
				idPoint1 = iter;
				break;
			}
		}
		if (tri1.m_points[idPoint1] == tri0.m_points[0]
			|| tri1.m_points[idPoint1] == tri0.m_points[1]
			|| tri1.m_points[idPoint1] == tri0.m_points[2])
		{
			TRACE("对三角形的非邻接点错误!\n");
			return;
		}
		// 对tri和tri1的临界三角形进行修改
		/*nb2-				-nb0			nb2-			-nb0
			   tir1---tri0		=====>             tri0
		  nb3-				-nb1				     |
											       tri1
										    nb3-			-nb1
		*/
		int nb0 = tri0.m_neighbors[(idPoint0 + 2) % 3];
		int nb1 = tri0.m_neighbors[(idPoint0 + 1) % 3];
		int nb2 = tri1.m_neighbors[idPoint1];
		int nb3 = tri1.m_neighbors[(idPoint1 + 2) % 3];
		if (tri1.m_neighbors[(idPoint1 + 1) % 3] != idTri0)
		{
			TRACE("对三角形的对三角形必须为本三角形!\n");
			return;
		}
		tri0.m_neighbors[0] = nb0;
		tri0.m_neighbors[1] = idTri1;
		tri0.m_neighbors[2] = nb2;
		tri1.m_neighbors[0] = nb3;
		tri1.m_neighbors[1] = idTri0;
		tri1.m_neighbors[2] = nb1;
		if (nb1 >= 0)
		{
			for (int i = 0; i < 3; i++)
			{
				if (mesh.tri(nb1).m_neighbors[i] == idTri0)
				{
					mesh.tri(nb1).m_neighbors[i] = idTri1;
					break;
				}
			}
		}
		if (nb2 >= 0)
		{
			for (int i = 0; i < 3; i++)
			{
				if (mesh.tri(nb2).m_neighbors[i] == idTri1)
				{
					mesh.tri(nb2).m_neighbors[i] = idTri0;
					break;
				}
			}
		}

		// 对定点进行修改
		// 对tri和tri1进行修改，两个三角形的第一个定点都是非邻接点
		/* 
		    p2                 0
		   / | \             /    \
		  p3 | p0  ==>       2  - 1
	       \ | /             1  - 2
	        p1               \    /
	                           0
		*/
		int p0 = tri0.m_points[idPoint0];
		int p1 = tri0.m_points[(idPoint0 + 1) % 3];
		int p2 = tri0.m_points[(idPoint0 + 2) % 3];
		int p3 = tri1.m_points[idPoint1];
		tri0.m_points[0] = p2;
		tri0.m_points[1] = p0;
		tri0.m_points[2] = p3;
		tri1.m_points[0] = p1;
		tri1.m_points[1] = p3;
		tri1.m_points[2] = p0;
		mesh.pt(p0).m_triagles.push_back(idTri1);
		mesh.pt(p3).m_triagles.push_back(idTri0);
		for (auto iter = mesh.pt(p1).m_triagles.begin(); iter != mesh.pt(p1).m_triagles.end(); iter++)
		{
			if (*iter == idTri0)
			{
				mesh.pt(p1).m_triagles.erase(iter);
				break;
			}
		}
		for (auto iter = mesh.pt(p2).m_triagles.begin(); iter != mesh.pt(p2).m_triagles.end(); iter++)
		{
			if (*iter == idTri1)
			{
				mesh.pt(p2).m_triagles.erase(iter);
				break;
			}
		}
	}
}

// 生成三角网格
void gb_finalizeTriagleMesh()
{

}
