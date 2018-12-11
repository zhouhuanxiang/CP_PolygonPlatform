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
#include <memory>

#include "CP_Polygon.h"
#include "CP_Plane.h"

class CP_MeshTriagle;
class CP_MeshVertex;
class CP_MeshEdge;

typedef shared_ptr<CP_MeshTriagle> CP_MeshTriaglePtr;
typedef shared_ptr<CP_MeshVertex> CP_MeshVertexPtr;
typedef shared_ptr<CP_MeshEdge> CP_MeshEdgePtr;

typedef vector<CP_MeshTriaglePtr> VT_MeshTriaglePointerArray;
typedef vector<CP_MeshVertexPtr> VT_MeshVertexPointerArray;
typedef vector<CP_MeshEdgePtr> VT_MeshEdgePointerArray;

typedef set<CP_MeshEdgePtr> SET_MeshEdgePointerArray;

class CP_MeshTriagle
{
public:
	CP_MeshVertexPtr m_vertex[3];
	CP_MeshTriaglePtr m_neighbors[3];
	bool deleted;
public:
	CP_MeshTriagle()
		:deleted(false)
	{
		m_neighbors[0] = NULL; m_neighbors[1] = NULL; m_neighbors[2] = NULL;
	}
	CP_MeshTriagle(CP_MeshVertexPtr p0, CP_MeshVertexPtr p1, CP_MeshVertexPtr p2)
		:deleted(false)
	{
		m_vertex[0] = p0; m_vertex[1] = p1; m_vertex[2] = p2;
		m_neighbors[0] = NULL; m_neighbors[1] = NULL; m_neighbors[2] = NULL;
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
};

class CP_MeshEdge
{
public:
	CP_MeshVertexPtr m_a;
	CP_MeshVertexPtr m_b;
public:
	CP_MeshEdge()
	{}
	CP_MeshEdge(CP_MeshVertexPtr a, CP_MeshVertexPtr b)
		:m_a(a), m_b(b)
	{}
	void del()
	{
		m_a = NULL;
		m_b = NULL;
	}
};

class CP_TriagleMesh
{
public:
	VT_MeshTriaglePointerArray m_triagleArray;
	VT_MeshVertexPointerArray m_vertexArray;
	SET_MeshEdgePointerArray m_edgeArray;
	CP_Polygon* m_polygon;
public:
	CP_TriagleMesh() : m_polygon(NULL) { }
	void mb_clear();
	~CP_TriagleMesh()
	{
		mb_clear();
	}
}; // 类CP_TriagleMesh定义结束

extern void initTriagleMesh(CP_TriagleMesh *mesh, CP_Polygon* polygon);

template <class T>
extern void replaceByValue(T *list, int size, const T &from, const T &to);

template <class T>
extern void eraseByValue(vector<T> &list, const T &val);

template <class T>
extern void insertByValue(vector<T> &list, const T &val);

extern double distanceVertex2Vertex(const CP_MeshVertexPtr &v1, const CP_MeshVertexPtr &v2);

extern void findClosestPoints(CP_TriagleMesh *mesh, const CP_MeshVertexPtr &vertex, vector<pair<CP_MeshVertexPtr, double> > &distances);

extern void getBoundingBox(CP_TriagleMesh *mesh, double &xmin, double &xmax, double &ymin, double &ymax);

extern void getTriagleCoord(const CP_MeshTriaglePtr &tri, const CP_MeshVertexPtr &vertex, double &l1, double &l2, double &l3);

extern bool testVertexInTriagle(const CP_MeshTriaglePtr &tri, const CP_MeshVertexPtr &vertex);

extern CP_MeshTriaglePtr& oppositeTriagle(const CP_MeshTriaglePtr &tri, const CP_MeshVertexPtr &vertex);

extern CP_MeshVertexPtr& oppositeVertex(const CP_MeshTriaglePtr &tri1, const CP_MeshTriaglePtr &tri2);

extern int oppositeVertexIndex(const CP_MeshTriaglePtr& tri1, const CP_MeshTriaglePtr &tri2);

extern CP_MeshTriaglePtr flipTriagle(CP_MeshTriaglePtr tri1, CP_MeshTriaglePtr tri2);

extern bool testEmptyCircumcircleTriagleVertex(const CP_MeshTriaglePtr &tri, const CP_MeshVertexPtr &D);

extern void testArtificialTriagle(CP_TriagleMesh *mesh, const CP_MeshTriaglePtr &tri1, const CP_MeshTriaglePtr &tri2, bool &is_artificial, bool &is_flip);

extern void testEmptyCircumcircle(CP_TriagleMesh *mesh, VT_MeshTriaglePointerArray &tris);

extern void insertVertex(CP_TriagleMesh *mesh, CP_Plane *plane, CP_MeshTriaglePtr &tri, CP_MeshVertexPtr &vertex);

extern void removeTriagle(CP_MeshTriaglePtr &tri);

extern void initialization(CP_TriagleMesh *mesh, CP_Plane *plane);

extern void triangulation(CP_TriagleMesh *mesh, CP_Plane *plane);

extern void finalisation(CP_TriagleMesh *mesh);

extern bool intersectEdgeEdge(const CP_MeshEdgePtr &edge1, const CP_MeshEdgePtr &edge2);

extern bool intersectEdgeTriagle(const CP_MeshEdgePtr &edge, const CP_MeshTriaglePtr &tri);

extern bool orientationVertexEdge(const CP_MeshEdgePtr &edge, const CP_MeshVertexPtr &vertex);

extern void insertEdgeCDT(CP_TriagleMesh *mesh);

extern void triangulatePseudoPolygon(CP_TriagleMesh *mesh, VT_MeshVertexPointerArray &polygon, CP_MeshEdgePtr &edge);

#endif