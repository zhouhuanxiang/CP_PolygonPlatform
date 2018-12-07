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

class CP_MeshTriagle;
class CP_MeshVertex;

class CP_MeshTriagle
{
public:
	CP_MeshVertex* m_vertex[3];
	CP_MeshTriagle* m_neighbors[3];
	bool deleted;
public:
	CP_MeshTriagle() 
		:deleted(false) 
	{ 
		m_neighbors[0] = NULL; m_neighbors[1] = NULL; m_neighbors[2] = NULL; 
	}
	CP_MeshTriagle(CP_MeshVertex* p0, CP_MeshVertex* p1, CP_MeshVertex* p2) 
		:deleted(false)
	{ 
		m_vertex[0] = p0; m_vertex[1] = p1; m_vertex[2] = p2; 
		m_neighbors[0] = NULL; m_neighbors[1] = NULL; m_neighbors[2] = NULL;
	}
	~CP_MeshTriagle()
	{
		del();
	}
	void del()
	{
		m_vertex[0] = m_vertex[1] = m_vertex[2] = NULL;
		m_neighbors[0] = m_neighbors[1] = m_neighbors[2] = NULL;
		deleted = true;
	}
	bool exits()
	{
		return !deleted;
	}
}; // 类CP_Triagle定义结束

typedef vector<CP_MeshTriagle> VT_MeshTriagleArray;
typedef vector<CP_MeshTriagle*> VT_MeshTriaglePointerArray;

class CP_MeshVertex
{
public:
	int index;
	double m_x, m_y;
	VT_MeshTriaglePointerArray m_triagles;
public:
	CP_MeshVertex() :m_x(0.0), m_y(0.0), index(-1) 
	{ 
		m_triagles.reserve(5); 
	};
	CP_MeshVertex(double x, double y, int ind) 
		:m_x(x), m_y(y), index(ind) 
	{
		m_triagles.reserve(5);
	}
	~CP_MeshVertex()
	{
		for (int i = 0; i < m_triagles.size(); i++)
		{
			m_triagles[i] = NULL;
		}
		m_triagles.clear();
	}
};

typedef vector<CP_MeshVertex> VT_MeshVertexArray;
typedef vector<CP_MeshVertex*> VT_MeshVertexPointerArray;

class CP_TriagleMesh
{
public:
	VT_MeshTriaglePointerArray m_triagleArray;
	VT_MeshVertexPointerArray m_vertexArray;
	CP_Polygon* m_polygon;
public:
	CP_TriagleMesh() : m_polygon(NULL) { }
	void mb_clear();
}; // 类CP_TriagleMesh定义结束

extern void initTriagleMesh(CP_TriagleMesh *mesh, CP_Polygon* polygon);

template <class T>
extern void replaceByValue(T *list, int size, T from, T to);

template <class T>
extern void eraseByValue(vector<T> &list, T val);

extern double distanceVertex2Vertex(CP_MeshVertex *v1, CP_MeshVertex *v2);

extern void findClosestPoints(CP_TriagleMesh *mesh, CP_MeshVertex *vertex, vector<pair<CP_MeshVertex*, double> > &distances);

extern void getBoundingBox(CP_TriagleMesh *mesh, double &xmin, double &xmax, double &ymin, double &ymax);

extern void getTriagleCoord(CP_MeshTriagle *tri, CP_MeshVertex *vertex, double &l1, double &l2, double &l3);

extern bool testVertexInTriagle(CP_MeshTriagle *tri, CP_MeshVertex *vertex);

extern CP_MeshTriagle* oppositeTriagle(CP_MeshTriagle *tri, CP_MeshVertex *vertex);

extern CP_MeshVertex* oppositeVertex(CP_MeshTriagle *tri1, CP_MeshTriagle *tri2);

extern int oppositeVertexIndex(CP_MeshTriagle *tri1, CP_MeshTriagle *tri2);

extern CP_MeshTriagle* flipTriagle(CP_MeshTriagle *tri1, CP_MeshTriagle *tri2);

extern void testEmptyCircumcircle(VT_MeshTriaglePointerArray &tris);

extern void insertVertex(CP_TriagleMesh *mesh, CP_MeshTriagle *tri, CP_MeshVertex *vertex);

extern void initialization(CP_TriagleMesh *mesh);

extern void triangulation(CP_TriagleMesh *mesh);

extern void finalisation(CP_TriagleMesh *mesh);

extern void printVertex(CP_MeshVertex *vertex);

extern void printTriagle(CP_MeshTriagle *tri);

#endif
