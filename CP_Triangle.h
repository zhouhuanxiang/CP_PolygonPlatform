#ifndef CP_TRIAGLE_H
#define CP_TRIAGLE_H
#include <iostream>
using namespace std;
#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
#include <iostream>

#include "CP_Polygon.h"

class CP_Triagle
{
public:
	int m_points[3];
	int m_neighbors[3];
public:
	CP_Triagle() { m_points[0] = 0; m_points[1] = 0; m_points[2] = 0; }
	CP_Triagle(int p0, int p1, int p2) { m_points[0] = p0; m_points[1] = p1; m_points[2] = p2; }
}; // 类CP_Triagle定义结束

typedef vector<CP_Triagle> VT_TriagleArray;
typedef vector<CP_Triagle*> VT_TriaglePointerArray;

class CP_MeshVertex
{
public:
	VT_IntArray m_triagles;
public:
	CP_MeshVertex() { }
};

typedef vector<CP_MeshVertex> VT_MeshVertexArray;
typedef vector<vector<CP_MeshVertex> > VT_MeshVertexArrayArray;

class CP_TriagleMesh
{
public:
	VT_TriagleArray m_triagleIDArray;
	VT_MeshVertexArray m_vertexArray;
	CP_Polygon* m_polygon;
public:
	CP_TriagleMesh() : m_polygon(NULL) { }
	void mb_clear() { m_polygon = NULL; m_triagleIDArray.clear(); m_vertexArray.clear(); }
	void mb_buildTriagleMesh(CP_Polygon& pn);
	CP_Triagle& tri(int i) { return m_triagleIDArray[i]; }
}; // 类CP_TriagleMesh定义结束

class CP_PlaneGrid
{
public:
	VT_IntArray m_points1l;
	vector<vector<list<int> > > m_points2l;
public:
	CP_PlaneGrid() { }
	bool mb_empty() { return m_points1l.size() == 0 && m_points2l.size() == 0; }
	bool mb_divided() { return m_points2l.size() > 0; }
};

typedef pair<int, int> Pair_Int;
typedef map<Pair_Int, CP_PlaneGrid> VT_PlaneGridMap;

class CP_Plane
{
public:
	// 整个平面的包围盒
	double m_boxXMax;
	double m_boxXMin;
	double m_boxYMax;
	double m_boxYMin;
	// 第一层网格划分参数
	int m_noOfCellsX1;
	int m_noOfCellsY1;
	double m_xSize;
	double m_ySize;
	// 第二层网格划分参数
	double m_threshold;
	// 网格数据
	VT_PlaneGridMap m_gridMap;
	// 临时数据
	VT_PointArray m_points;
	set<Pair_Int> m_queryGrid1l;
	set<pair<Pair_Int, Pair_Int> > m_queryGrid2l;
	int m_queryResult;
	double m_distance;
public:
	CP_Plane() { }
}; // 类CP_Plane定义结束

// 读取点集并随机化
extern void gb_getDelaunayVertices(CP_Plane &plane);
extern void gb_randomizeDelaunayVertices(CP_Plane &plane);
// 查找网格坐标
extern Pair_Int gb_findCoord1l(CP_Plane &plane, int idPoint);
extern Pair_Int gb_findCoord2l(CP_Plane &plane, int idPoint, Pair_Int coord1);
// 初始化第一阶段划分
extern void gb_initSubdivision(CP_Plane &plane);
// 插入点操作：1）首先找到要插入的网格；2）再在该网格进行二阶段插入
extern void gb_insertToSubdivision1l(CP_Plane &plane, int idPoint);
extern void gb_insertToSubdivision2l(CP_Plane &plane, int idPoint);
// 查找最近点
extern void gb_querySubdivision(CP_Plane &plane, int idPoint);
extern void gb_floodFillSubdivision1l(CP_Plane &plane, Pair_Int coord1, int idPoint);
extern void gb_floodFillSubdivision2l(CP_Plane &plane, Pair_Int coord1, Pair_Int coord2, int idPoint);
extern double gb_distancePointCell(CP_Plane &plane, Pair_Int coord1, int idPoint);
extern double gb_distancePointCell(CP_Plane &plane, Pair_Int coord1, Pair_Int coord2, int idPoint);
// 生成三角剖分
extern void gb_generateTriagleMesh(CP_Plane &plane, CP_TriagleMesh &mesh);
extern Pair_Int gb_findTriagleContainingPoint(CP_Plane &plane, CP_TriagleMesh &mesh, int idPoint, int idVertex);
// 合法化三角剖分
extern void gb_legalizeTriagleMesh();
// 生成三角网格
extern void gb_finalizeTriagleMesh();

#endif
