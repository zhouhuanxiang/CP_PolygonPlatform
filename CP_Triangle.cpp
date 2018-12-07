#include "stdafx.h"

#include "CP_Triangle.h"

void CP_TriagleMesh::mb_clear()
{
	for (int i = 0; i < m_triagleArray.size(); i++)
	{
		delete m_triagleArray[i];
		m_triagleArray[i] = NULL;
	}
	m_triagleArray.clear();

	for (int i = 0; i < m_vertexArray.size(); i++)
	{
		delete m_vertexArray[i];
		m_vertexArray[i] = NULL;
	}
	m_vertexArray.clear();

	m_polygon = NULL;
}

void initTriagleMesh(CP_TriagleMesh *mesh, CP_Polygon* polygon)
{
	mesh->mb_clear();

	mesh->m_polygon = polygon;
	mesh->m_vertexArray.resize(mesh->m_polygon->m_pointArray.size());
	for (int i = 0; i < mesh->m_vertexArray.size(); i++)
	{
		mesh->m_vertexArray[i] = new CP_MeshVertex(mesh->m_polygon->m_pointArray[i].m_x, mesh->m_polygon->m_pointArray[i].m_y, i);
	}
}

template <class T>
void replaceByValue(T *list, int size, T from, T to)
{
	for (int i = 0; i < size; i++)
	{
		if (*(list + i) == from)
		{
			*(list + i) = to;
			break;
		}
	}
}

template <class T>
void eraseByValue(vector<T> &list, T val)
{
	for (vector<T>::iterator iter = list.begin(); iter != list.end(); iter++)
	{
		if (*iter == val)
		{
			list.erase(iter);
			return;
		}
	}
}

double distanceVertex2Vertex(CP_MeshVertex *v1, CP_MeshVertex *v2)
{
	double deltaX = v1->m_x - v2->m_x;
	double deltaY = v1->m_y - v2->m_y;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

void findClosestPoints(CP_TriagleMesh *mesh, CP_MeshVertex *vertex, vector<pair<CP_MeshVertex*, double> > &distances)
{
	typedef pair<CP_MeshVertex*, double> pairs;
	int vsize = mesh->m_vertexArray.size();
	// distances.resize(vsize);
	for (int i = 0; i < vsize; i++)
	{
		if (mesh->m_vertexArray[i] != vertex)
		{
			double distance = distanceVertex2Vertex(vertex, mesh->m_vertexArray[i]);
			distances.push_back(pairs(mesh->m_vertexArray[i], distance));
		}
		else
			break;
	}

	sort(distances.begin(), distances.end(), 
		[](const pairs &a, const pairs &b) -> bool
	{
		return a.second < b.second;
	});
}

void getBoundingBox(CP_TriagleMesh *mesh, double &xmin, double &xmax, double &ymin, double &ymax)
{
	xmin = DBL_MAX;
	ymin = DBL_MAX;
	xmax = DBL_MAX * -1;
	ymax = DBL_MAX * -1;
	int vsize = mesh->m_vertexArray.size();
	for (int i = 0; i < vsize; i++)
	{
		xmin = min(mesh->m_vertexArray[i]->m_x, xmin);
		ymin = min(mesh->m_vertexArray[i]->m_y, ymin);
		xmax = max(mesh->m_vertexArray[i]->m_x, xmax);
		ymax = max(mesh->m_vertexArray[i]->m_y, ymax);
	}
}

void getTriagleCoord(CP_MeshTriagle *tri, CP_MeshVertex *vertex, double &l1, double &l2, double &l3)
{
	// https://en.wikipedia.org/wiki/Barycentric_coordinates_%28mathematics%29
	double x1 = tri->m_vertex[0]->m_x;
	double y1 = tri->m_vertex[0]->m_y;
	double x2 = tri->m_vertex[1]->m_x;
	double y2 = tri->m_vertex[1]->m_y;
	double x3 = tri->m_vertex[2]->m_x;
	double y3 = tri->m_vertex[2]->m_y;
	double x = vertex->m_x;
	double y = vertex->m_y;

	double detT = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
	l1 = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / detT;
	l2 = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / detT;
	l3 = 1 - l1 - l2;
}

bool testVertexInTriagle(CP_MeshTriagle *tri, CP_MeshVertex *vertex)
{
	double l1, l2, l3;
	getTriagleCoord(tri, vertex, l1, l2, l3);

	return (l1 >= 0) && (l2 >= 0) && (l3 >= 0);
}

CP_MeshTriagle* oppositeTriagle(CP_MeshTriagle *tri, CP_MeshVertex *vertex)
{
	for (int i = 0; i < 3; i++)
	{
		if (tri->m_vertex[i] == vertex)
		{
			return tri->m_neighbors[(i + 1) % 3];
		}
	}
}

CP_MeshVertex* oppositeVertex(CP_MeshTriagle *tri1, CP_MeshTriagle *tri2)
{
	for (int i = 0; i < 3; i++)
	{
		if (tri1->m_vertex[i] != tri2->m_vertex[0] &&
			tri1->m_vertex[i] != tri2->m_vertex[1] &&
			tri1->m_vertex[i] != tri2->m_vertex[2])
		{
			return tri1->m_vertex[i];
		}
	}
}

int oppositeVertexIndex(CP_MeshTriagle *tri1, CP_MeshTriagle *tri2)
{
	for (int i = 0; i < 3; i++)
	{
		if (tri1->m_vertex[i] != tri2->m_vertex[0] &&
			tri1->m_vertex[i] != tri2->m_vertex[1] &&
			tri1->m_vertex[i] != tri2->m_vertex[2])
		{
			return i;
		}
	}
}

CP_MeshTriagle* flipTriagle(CP_MeshTriagle *tri1, CP_MeshTriagle *tri2)
{
	int index1 = oppositeVertexIndex(tri1, tri2);
	int index2 = oppositeVertexIndex(tri2, tri1);
	CP_MeshVertex *v0 = tri1->m_vertex[index1];
	CP_MeshVertex *v1 = tri1->m_vertex[(index1 + 1) % 3];
	CP_MeshVertex *v2 = tri2->m_vertex[index2];
	CP_MeshVertex *v3 = tri2->m_vertex[(index2 + 1) % 3];
	CP_MeshTriagle *n0 = tri1->m_neighbors[index1];
	CP_MeshTriagle *n1 = tri2->m_neighbors[(index2 + 2) % 3];
	CP_MeshTriagle *n2 = tri2->m_neighbors[index2];
	CP_MeshTriagle *n3 = tri1->m_neighbors[(index1 + 2) % 3];
	// 改变2个三角形的顶点
	tri1->m_vertex[0] = v0;
	tri1->m_vertex[1] = v2;
	tri1->m_vertex[2] = v3;
	tri2->m_vertex[0] = v0;
	tri2->m_vertex[1] = v1;
	tri2->m_vertex[2] = v2;
	// 改变顶点的三角形
	v0->m_triagles.push_back(tri2);
	eraseByValue(v1->m_triagles, tri1);
	v2->m_triagles.push_back(tri1);
	eraseByValue(v3->m_triagles, tri2);
	// 改变两个三角形的邻居
	tri1->m_neighbors[0] = tri2;
	tri1->m_neighbors[1] = n2;
	tri1->m_neighbors[2] = n3;
	tri2->m_neighbors[0] = n0;
	tri2->m_neighbors[1] = n1;
	tri2->m_neighbors[2] = tri1;
	// 改变邻居的邻居
	if (n0 != NULL)
	{
		replaceByValue(n0->m_neighbors, 3, tri1, tri2);
	}
	if (n2 != NULL)
	{
		replaceByValue(n2->m_neighbors, 3, tri2, tri1);
	}
	return tri2;
}

void testEmptyCircumcircle(VT_MeshTriaglePointerArray &tris)
{
	CP_MeshTriagle *tri = tris.back();
	for (int outer_loop = 0; outer_loop < 3; outer_loop++)
	{
		if (tri->m_neighbors[outer_loop])
		{	
			CP_MeshVertex *A = tri->m_vertex[0];
			CP_MeshVertex *B = tri->m_vertex[1];
			CP_MeshVertex *C = tri->m_vertex[2];
			CP_MeshVertex *D = oppositeVertex(tri->m_neighbors[outer_loop], tri);
			double a = A->m_x - D->m_x;
			double b = A->m_y - D->m_y;
			double c = a * a + b * b;
			double d = B->m_x - D->m_x;
			double e = B->m_y - D->m_y;
			double f = d * d + e * e;
			double g = C->m_x - D->m_x;
			double h = C->m_y - D->m_y;
			double i = g * g + h * h;
			double det = a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h;
			if (det > 0.0001)
			{
				TRACE("det %f\n", det);
				CP_MeshTriagle* neighbor = flipTriagle(tri, tri->m_neighbors[outer_loop]);
				for (auto iter : tris)
				{
					if (iter == neighbor)
					{
						return;
					}
				} 
				tris.push_back(neighbor);
				return;
			}
		}
	}
	tris.pop_back();
}

void insertVertex(CP_TriagleMesh *mesh, CP_MeshTriagle *tri, CP_MeshVertex *vertex)
{
	CP_MeshVertex *v1 = tri->m_vertex[0];
	CP_MeshVertex *v2 = tri->m_vertex[1];
	CP_MeshVertex *v3 = tri->m_vertex[2];
	// 生成3个新的三角形
	mesh->m_triagleArray.push_back(new CP_MeshTriagle(vertex, v1, v2));
	CP_MeshTriagle *t1 = mesh->m_triagleArray.back();
	mesh->m_triagleArray.push_back(new CP_MeshTriagle(vertex, v2, v3));
	CP_MeshTriagle *t2 = mesh->m_triagleArray.back();
	mesh->m_triagleArray.push_back(new CP_MeshTriagle(vertex, v3, v1));
	CP_MeshTriagle *t3 = mesh->m_triagleArray.back();
	// 跟新顶点的三角形
	vertex->m_triagles.push_back(t1);
	vertex->m_triagles.push_back(t2);
	vertex->m_triagles.push_back(t3);
	replaceByValue(v1->m_triagles.data(), v1->m_triagles.size(), tri, t1);
	v1->m_triagles.push_back(t3);
	replaceByValue(v2->m_triagles.data(), v2->m_triagles.size(), tri, t2);
	v2->m_triagles.push_back(t1);
	replaceByValue(v3->m_triagles.data(), v3->m_triagles.size(), tri, t3);
	v3->m_triagles.push_back(t2);
	// 更新三角形的邻居
	t1->m_neighbors[0] = t3;
	t1->m_neighbors[1] = tri->m_neighbors[0];
	t1->m_neighbors[2] = t2;
	if (tri->m_neighbors[0])
	{
		replaceByValue(tri->m_neighbors[0]->m_neighbors, 3, tri, t1);
	}
	//
	t2->m_neighbors[0] = t1;
	t2->m_neighbors[1] = tri->m_neighbors[1];
	t2->m_neighbors[2] = t3;
	if (tri->m_neighbors[1])
	{
		replaceByValue(tri->m_neighbors[1]->m_neighbors, 3, tri, t2);
	}
	//
	t3->m_neighbors[0] = t2;
	t3->m_neighbors[1] = tri->m_neighbors[2];
	t3->m_neighbors[2] = t1;
	if (tri->m_neighbors[2])
	{
		replaceByValue(tri->m_neighbors[2]->m_neighbors, 3, tri, t3);
	}
	//
	tri->del();
}

void initialization(CP_TriagleMesh *mesh)
{
	// 获得包围盒
	double xmin, xmax, ymin, ymax;
	getBoundingBox(mesh, xmin, xmax, ymin, ymax);
	xmax = max(abs(xmin), abs(xmax));
	ymax = max(abs(ymin), abs(ymax));
	// 生成外围三角形
	int vsize = mesh->m_vertexArray.size();
	mesh->m_vertexArray.push_back(new CP_MeshVertex(3 * xmax, 0, vsize));
	CP_MeshVertex *v1 = mesh->m_vertexArray.back();
	mesh->m_vertexArray.push_back(new CP_MeshVertex(0, 3 * ymax, vsize + 1));
	CP_MeshVertex *v2 = mesh->m_vertexArray.back();
	mesh->m_vertexArray.push_back(new CP_MeshVertex(-3 * xmax, -3 * ymax, vsize + 2));
	CP_MeshVertex *v3 = mesh->m_vertexArray.back();
	mesh->m_triagleArray.push_back(new CP_MeshTriagle(v1, v2, v3));
	CP_MeshTriagle *tri = (mesh->m_triagleArray[0]);
	v1->m_triagles.push_back(tri);
	v2->m_triagles.push_back(tri);
	v3->m_triagles.push_back(tri);
	// 插入第一个顶点
	insertVertex(mesh, tri, mesh->m_vertexArray[0]);
}

void triangulation(CP_TriagleMesh *mesh)
{
	int vsize = mesh->m_vertexArray.size() - 3;
	for (int outer_loop = 1; outer_loop < vsize; outer_loop++)
	{
		TRACE("outer loop #%d\n", outer_loop);
		CP_MeshVertex *vertex = mesh->m_vertexArray[outer_loop];
		CP_MeshTriagle *tri = NULL;
		vector<pair<CP_MeshVertex*, double> > distances;
		findClosestPoints(mesh, vertex, distances);
		for (int i = 0; i < distances.size(); i++ && tri == NULL)
		{
			for (int j = 0; j < distances[i].first->m_triagles.size(); j++ && tri == NULL)
			{
				if (testVertexInTriagle(distances[i].first->m_triagles[j], vertex))
				{
					tri = distances[i].first->m_triagles[j];
					break;
				}
			}
		}
		insertVertex(mesh, tri, vertex);
		VT_MeshTriaglePointerArray tris;
		for (auto iter = mesh->m_triagleArray.end() - 3; iter != mesh->m_triagleArray.end(); iter++)
		{
			tris.push_back(*iter);
		}
		while (!tris.empty())
		{
			TRACE("outer loop #%d test #%d\n", outer_loop, tris.size());
			testEmptyCircumcircle(tris);
		}
	}
}

void finalisation(CP_TriagleMesh *mesh)
{
	for (auto iter = mesh->m_vertexArray.end() - 3; iter != mesh->m_vertexArray.end(); iter++)
	{
		for (int i = 0; i < (*iter)->m_triagles.size(); i++)
		{
			(*iter)->m_triagles[i]->del();
		}
	}
}

void printVertex(CP_MeshVertex *vertex)
{
	TRACE("%d %d\n", vertex->m_x, vertex->m_y);
}

void printTriagle(CP_MeshTriagle *tri)
{
	TRACE("%d %d %d %d %d %d\n",
		tri->m_vertex[0]->m_x, tri->m_vertex[0]->m_y,
		tri->m_vertex[1]->m_x, tri->m_vertex[1]->m_y,
		tri->m_vertex[2]->m_x, tri->m_vertex[2]->m_y);
}