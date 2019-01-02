#include "stdafx.h"

#include "CP_Triangle.h"

void CP_TriagleMesh::mb_clear()
{
	time1 = time2 = time3 = time4 = time5 = time6 = 0;

	for (int i = 0; i < m_vertexArray.size(); i++)
	{
		m_vertexArray[i]->m_triagles.clear();
	}
	for (int i = 0; i < m_triagleArray.size(); i++)
	{
		m_triagleArray[i]->del();
	}
	for (int i = 0; i < m_finalTriagleArray.size(); i++)
	{
		m_finalTriagleArray[i]->del();
	}
	for (int i = 0; i < m_constrainedEdgeSet.size(); i++)
	{
		m_constrainedEdgeSet[i]->del();
	}
	m_vertexArray.clear();
	m_triagleArray.clear();
	//m_edgeArray = VT_MeshEdgePointerArray();
	m_finalTriagleArray.clear();
	m_constrainedEdgeSet.clear();
	m_insideVertexPool.clear();
	m_polygon = NULL;

	// TRACE("cleared\n");
}

void gb_initTriagleMesh(CP_TriagleMesh *mesh, CP_Polygon* polygon)
{
	exactinit();

	mesh->mb_clear();

	mesh->m_polygon = polygon;
	mesh->m_vertexArray.reserve(mesh->m_polygon->m_pointArray.size());
	mesh->m_constrainedEdgeSet.reserve(10000);

	int index_count = 0;
	for (auto &region : mesh->m_polygon->m_regionArray)
	{
		for (int i = 0; i < region.m_loopArray.size(); i++)
		{
			CP_Loop &loop = region.m_loopArray[(i + 1) % region.m_loopArray.size()];
			int size = loop.m_pointIDArray.size();
			for (int j = 0; j < size; j++)
			{
				int pindex1 = loop.m_pointIDArray[j];
				int pindex2 = loop.m_pointIDArray[(j + 1) % size];
				double x1 = mesh->m_polygon->m_pointArray[pindex1].m_x;
				double y1 = mesh->m_polygon->m_pointArray[pindex1].m_y;
				double x2 = mesh->m_polygon->m_pointArray[pindex2].m_x;
				double y2 = mesh->m_polygon->m_pointArray[pindex2].m_y;
				int vindex1, vindex2;
				if (mesh->m_insideVertexPool.find(pair<double, double>(x1, y1)) == mesh->m_insideVertexPool.end())
				{
					mesh->m_vertexArray.push_back(make_shared<CP_MeshVertex>(x1, y1, index_count));
					vindex1 = index_count++;
					mesh->m_insideVertexPool[pair<double, double>(x1, y1)] = vindex1;
				}
				else
				{
					vindex1 = mesh->m_insideVertexPool[pair<double, double>(x1, y1)];
				}
				if (mesh->m_insideVertexPool.find(pair<double, double>(x2, y2)) == mesh->m_insideVertexPool.end())
				{
					mesh->m_vertexArray.push_back(make_shared<CP_MeshVertex>(x2, y2, index_count));
					vindex2 = index_count++;
					mesh->m_insideVertexPool[pair<double, double>(x2, y2)] = vindex2;
				}
				else
				{
					vindex2 = mesh->m_insideVertexPool[pair<double, double>(x2, y2)];
				}
				mesh->m_constrainedEdgeSet.push_back(make_shared<CP_MeshEdge>(mesh->m_vertexArray[vindex1], mesh->m_vertexArray[vindex2]));
			}
		}
	}

//#pragma omp parallel
//	for (int i = 0; i < mesh->m_vertexArray.size(); i++)
//	{
//		mesh->m_vertexArray.push_back(make_shared<CP_MeshVertex>(mesh->m_polygon->m_pointArray[i].m_x, mesh->m_polygon->m_pointArray[i].m_y, i));
//	}
//	for (auto &region : mesh->m_polygon->m_regionArray)
//	{
//		for (auto &loop : region.m_loopArray)
//		{
//			for (int i = 0; i < loop.m_pointIDArray.size(); i++)
//			{
//				int ai = loop.m_pointIDArray[i];
//				int bi = loop.m_pointIDArray[(i + 1) % loop.m_pointIDArray.size()];
//				mesh->m_constrainedEdgeSet.push_back(make_shared<CP_MeshEdge>(mesh->m_vertexArray[ai], mesh->m_vertexArray[bi]));
//			}
//		}
//	}
}

template <class T>
void gb_replaceArrayByValue(T *list, int size, const T &from, const T &to)
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
void gb_eraseArrayByValue(vector<T> &list, const T &val)
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

template <class T>
void gb_insertArrayByValue(vector<T> &list, const T& val)
{
	for (vector<T>::iterator iter = list.begin(); iter != list.end(); iter++)
	{
		if (*iter == val)
		{
			return;
		}
	}
	list.push_back(val);
}

double gb_distanceVertex2Vertex(const CP_MeshVertexPtr &v1, const CP_MeshVertexPtr &v2)
{
	double deltaX = v1->m_x - v2->m_x;
	double deltaY = v1->m_y - v2->m_y;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

void gb_findClosestPoints(CP_TriagleMesh *mesh, const CP_MeshVertexPtr &vertex, vector<pair<CP_MeshVertexPtr, double> > &distances)
{
	typedef pair<CP_MeshVertexPtr, double> pairs;
	int vsize = mesh->m_vertexArray.size();
	// distances.resize(vsize);
	for (int i = 0; i < vsize; i++)
	{
		if (mesh->m_vertexArray[i] != vertex)
		{
			double distance = gb_distanceVertex2Vertex(vertex, mesh->m_vertexArray[i]);
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

void gb_getBoundingBox(CP_TriagleMesh *mesh, double &xmin, double &xmax, double &ymin, double &ymax)
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

void gb_getTriagleCoord(const CP_MeshFacePtr &tri, const CP_MeshVertexPtr &vertex, double &l1, double &l2, double &l3)
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

bool gb_testVertexInTriagle(const CP_MeshFacePtr &tri, const CP_MeshVertexPtr &vertex)
{
	//double l1, l2, l3;
	//gb_getTriagleCoord(tri, vertex, l1, l2, l3);

	//return (l1 >= -1 * DBL_EPSILON) && (l2 >= -1 * DBL_EPSILON) && (l3 >= -1 * DBL_EPSILON);

	double x1 = tri->m_vertex[0]->m_x;
	double y1 = tri->m_vertex[0]->m_y;
	double x2 = tri->m_vertex[1]->m_x;
	double y2 = tri->m_vertex[1]->m_y;
	double x3 = tri->m_vertex[2]->m_x;
	double y3 = tri->m_vertex[2]->m_y;
	double x = vertex->m_x;
	double y = vertex->m_y;

	double xMin = min(x1, min(x2, x3)) - DBL_EPSILON;
	double xMax = max(x1, max(x2, x3)) + DBL_EPSILON;
	double yMin = min(y1, min(y2, y3)) - DBL_EPSILON;
	double yMax = max(y1, max(y2, y3)) + DBL_EPSILON;

	if (x < xMin || xMax < x || y < yMin || yMax < y)
	{
		return false;
	}

	bool checkSide1 = gb_orientationVertexEdge(x, y, x1, y1, x2, y2) >= 0;
	bool checkSide2 = gb_orientationVertexEdge(x, y, x2, y2, x3, y3) >= 0;
	bool checkSide3 = gb_orientationVertexEdge(x, y, x3, y3, x1, y1) >= 0;

	return checkSide1 && checkSide2 && checkSide3;
}

CP_MeshFacePtr& gb_oppositeTriagle(const CP_MeshFacePtr &tri, const CP_MeshVertexPtr &vertex)
{
	for (int i = 0; i < 3; i++)
	{
		if (tri->m_vertex[i] == vertex)
		{
			return tri->m_neighbors[(i + 1) % 3];
		}
	}
}

CP_MeshVertexPtr& gb_oppositeVertex(const CP_MeshFacePtr &tri1, const CP_MeshFacePtr &tri2)
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
	TRACE("wrong\n");
}

int gb_oppositeVertexIndex(const CP_MeshFacePtr &tri1, const CP_MeshFacePtr &tri2)
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
	TRACE("wrong\n");
}

CP_MeshFacePtr gb_flipTriagle(CP_MeshFacePtr tri1, CP_MeshFacePtr tri2)
{
	int index1 = gb_oppositeVertexIndex(tri1, tri2);
	int index2 = gb_oppositeVertexIndex(tri2, tri1);
	CP_MeshVertexPtr v0 = tri1->m_vertex[index1];
	CP_MeshVertexPtr v1 = tri1->m_vertex[(index1 + 1) % 3];
	CP_MeshVertexPtr v2 = tri2->m_vertex[index2];
	CP_MeshVertexPtr v3 = tri2->m_vertex[(index2 + 1) % 3];
	CP_MeshFacePtr n0 = tri1->m_neighbors[index1];
	CP_MeshFacePtr n1 = tri2->m_neighbors[(index2 + 2) % 3];
	CP_MeshFacePtr n2 = tri2->m_neighbors[index2];
	CP_MeshFacePtr n3 = tri1->m_neighbors[(index1 + 2) % 3];
	// 改变2个三角形的顶点
	tri1->m_vertex[0] = v0;
	tri1->m_vertex[1] = v2;
	tri1->m_vertex[2] = v3;
	tri2->m_vertex[0] = v0;
	tri2->m_vertex[1] = v1;
	tri2->m_vertex[2] = v2;
	// 改变顶点的三角形
	v0->m_triagles.push_back(tri2);
	gb_eraseArrayByValue(v1->m_triagles, tri1);
	v2->m_triagles.push_back(tri1);
	gb_eraseArrayByValue(v3->m_triagles, tri2);
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
		gb_replaceArrayByValue(n0->m_neighbors, 3, tri1, tri2);
	}
	if (n2 != NULL)
	{
		gb_replaceArrayByValue(n2->m_neighbors, 3, tri2, tri1);
	}

	return tri2;
}

int gb_orientationVertexEdge(double vx0, double vy0, double ex1, double ey1, double ex2, double ey2)
{
	double pa[2] = { ex1, ey1 };
	double pb[2] = { ex2, ey2 };
	double pc[2] = { vx0, vy0 };

	double result = orient2dfast(pa, pb, pc);

	if (result == 0)
	{
		return 0;
	}
	else if (result > 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

bool gb_testEmptyCircumcircleTriagleVertex(const CP_MeshFacePtr &tri, const CP_MeshVertexPtr &D)
{
	// https://www.newcastle.edu.au/__data/assets/pdf_file/0017/22508/13_A-fast-algorithm-for-constructing-Delaunay-triangulations-in-the-plane.pdf
	CP_MeshVertexPtr A = tri->m_vertex[0];
	CP_MeshVertexPtr B = tri->m_vertex[1];
	CP_MeshVertexPtr C = tri->m_vertex[2];

	double pa[2] = { A->m_x, A->m_y };
	double pb[2] = { B->m_x, B->m_y };
	double pc[2] = { C->m_x, C->m_y };
	double pd[2] = { D->m_x, D->m_y };
	double result = incircle(pa, pb, pc, pd);

	//TRACE("%.15f\n", result);

	//if (count++ == 10000)
	//{
	//	TRACE("%f\n", result);
	//	system("pause");
	//}

	return result <= 0;
}

void gb_testArtificialTriagle(CP_TriagleMesh *mesh, const CP_MeshFacePtr &tri1, const CP_MeshFacePtr &tri2, bool &is_artificial, bool &is_flip)
{
	int index1 = gb_oppositeVertexIndex(tri1, tri2);
	int index2 = gb_oppositeVertexIndex(tri2, tri1);
	CP_MeshVertexPtr v0 = tri1->m_vertex[index1];
	CP_MeshVertexPtr v1 = tri1->m_vertex[(index1 + 1) % 3];
	CP_MeshVertexPtr v2 = tri2->m_vertex[index2];
	CP_MeshVertexPtr v3 = tri2->m_vertex[(index2 + 1) % 3];
	int vsize = mesh->m_vertexArray.size();
	is_artificial = false;
	is_flip = false;

	if (mesh->m_constrainedEdgeSet.end() != std::find_if(mesh->m_constrainedEdgeSet.begin(), mesh->m_constrainedEdgeSet.end(),
		[&](const CP_MeshEdgePtr &e) -> bool
	{
		return (e->m_a->index == v1->index && e->m_b->index == v3->index)
			|| (e->m_a->index == v3->index && e->m_b->index == v1->index);
	}))
	{
		// 如果 v1 v3 是限制边，则不需要进行翻转
		is_artificial = true;
	}
	else if (v0->index >= vsize - 3 || v2->index >= vsize - 3)
	{
		// 如果v0 v2有一个是假节点，则不需要进行翻转
		is_artificial = true;
	}
	else if (v1->index >= vsize - 3)
	{
		// 如果v1 v3有一个是假节点，则需要进行额外的检查
		//     如果v0 v2 在 v1v3 的同侧，则需要进行翻转
		is_artificial = true;
		if (gb_orientationVertexEdge(v1->m_x, v1->m_y, v0->m_x, v0->m_y, v3->m_x, v3->m_y)
			== gb_orientationVertexEdge(v2->m_x, v2->m_y, v0->m_x, v0->m_y, v3->m_x, v3->m_y))
		{
			is_flip = true;
		}
	}
	else if (v3->index >= vsize - 3)
	{
		is_artificial = true;
		if (gb_orientationVertexEdge(v3->m_x, v3->m_y, v0->m_x, v0->m_y, v1->m_x, v1->m_y)
			== gb_orientationVertexEdge(v2->m_x, v2->m_y, v0->m_x, v0->m_y, v1->m_x, v1->m_y))
		{
			is_flip = true;
		}
	}
}

void gb_testEmptyCircumcircle(CP_TriagleMesh *mesh, VT_MeshTriaglePointerArray &tris)
{
	CP_MeshFacePtr tri = tris.back();
	if (tri->exits())
	{
		for (int outer_loop = 0; outer_loop < 3; outer_loop++)
		{
			if (tri->m_neighbors[outer_loop])
			{
				bool is_artificial = false;
				bool is_flip = false;
				gb_testArtificialTriagle(mesh, tri, tri->m_neighbors[outer_loop], is_artificial, is_flip);
				if (!is_artificial)
				{
					CP_MeshVertexPtr D = gb_oppositeVertex(tri->m_neighbors[outer_loop], tri);
					is_flip = !gb_testEmptyCircumcircleTriagleVertex(tri, D);
				}
				if (is_flip)
				{
					CP_MeshFacePtr neighbor = gb_flipTriagle(tri, tri->m_neighbors[outer_loop]);
					gb_insertArrayByValue(tris, neighbor);
					return;
				}
			}
		}
	}
	tris.pop_back();
}

bool gb_onEdgeInsertVertex(CP_TriagleMesh *mesh, CP_Plane *plane, CP_MeshFacePtr &tri0, CP_MeshVertexPtr &vertex)
{
	for (int i = 0; i < 3; i++)
	{
		CP_MeshVertexPtr v1 = tri0->m_vertex[i];
		CP_MeshVertexPtr v2 = tri0->m_vertex[(i + 1) % 3];
		CP_MeshVertexPtr v3 = tri0->m_vertex[(i + 2) % 3];
		if (gb_orientationVertexEdge(vertex->m_x, vertex->m_y, v1->m_x, v1->m_y, v2->m_x, v2->m_y) == 0)
		{
			TRACE("split to 4\n");
			CP_MeshFacePtr tri1 = gb_oppositeTriagle(tri0, v3);
			int index0 = gb_oppositeVertexIndex(tri0, tri1);
			int index1 = gb_oppositeVertexIndex(tri1, tri0);
			CP_MeshVertexPtr v4 = tri1->m_vertex[index1];
			CP_MeshFacePtr tri2 = tri0->m_neighbors[index0];
			CP_MeshFacePtr tri3 = tri1->m_neighbors[(index1 + 2) % 3];
			CP_MeshFacePtr tri4 = tri1->m_neighbors[index1];
			CP_MeshFacePtr tri5 = tri0->m_neighbors[(index0 + 2) % 3];
			// 插入4个新的三角形
			mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(vertex, v1, v4));
			CP_MeshFacePtr tri6 = mesh->m_triagleArray.back();
			mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(vertex, v4, v2));
			CP_MeshFacePtr tri7 = mesh->m_triagleArray.back();
			mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(vertex, v2, v3));
			CP_MeshFacePtr tri8 = mesh->m_triagleArray.back();
			mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(vertex, v3, v1));
			CP_MeshFacePtr tri9 = mesh->m_triagleArray.back();
			// 改变顶点的三角形
			vertex->m_triagles.push_back(tri6);
			vertex->m_triagles.push_back(tri7);
			vertex->m_triagles.push_back(tri8);
			vertex->m_triagles.push_back(tri9);
			gb_replaceArrayByValue(v1->m_triagles.data(), v1->m_triagles.size(), tri0, tri9);
			gb_replaceArrayByValue(v1->m_triagles.data(), v1->m_triagles.size(), tri1, tri6);
			gb_replaceArrayByValue(v2->m_triagles.data(), v2->m_triagles.size(), tri0, tri7);
			gb_replaceArrayByValue(v2->m_triagles.data(), v2->m_triagles.size(), tri1, tri8);
			gb_replaceArrayByValue(v3->m_triagles.data(), v3->m_triagles.size(), tri0, tri8);
			v3->m_triagles.push_back(tri9);
			gb_replaceArrayByValue(v4->m_triagles.data(), v4->m_triagles.size(), tri1, tri6);
			v4->m_triagles.push_back(tri7);
			// 改变新三角形的邻居
			tri6->m_neighbors[0] = tri9;
			tri6->m_neighbors[1] = tri3;
			tri6->m_neighbors[2] = tri7;
			tri7->m_neighbors[0] = tri6;
			tri7->m_neighbors[1] = tri4;
			tri7->m_neighbors[2] = tri8;
			tri8->m_neighbors[0] = tri7;
			tri8->m_neighbors[1] = tri5;
			tri8->m_neighbors[2] = tri9;
			tri9->m_neighbors[0] = tri8;
			tri9->m_neighbors[1] = tri2;
			tri9->m_neighbors[2] = tri6;
			// 改变邻居的三角形
			if (tri2 != NULL)
			{
				gb_replaceArrayByValue(tri2->m_neighbors, 3, tri0, tri9);
			}
			if (tri3 != NULL)
			{
				gb_replaceArrayByValue(tri3->m_neighbors, 3, tri1, tri6);
			}
			if (tri4 != NULL)
			{
				gb_replaceArrayByValue(tri4->m_neighbors, 3, tri1, tri7);
			}
			if (tri5 != NULL)
			{
				gb_replaceArrayByValue(tri5->m_neighbors, 3, tri0, tri8);
			}
			// 删除原两个三角形
			tri0->del();
			tri1->del();
			//
			insertPlane(*plane, CP_PlanePoint(vertex->index, vertex->m_x, vertex->m_y));
			//
			//for (auto iter : mesh->m_vertexArray)
			//{
			//	for (auto iter1 : iter->m_triagles)
			//	{
			//		if (iter1 && !iter1->exits())
			//		{
			//			TRACE("debug\n");
			//		}
			//	}
			//}
			//CP_MeshFacePtr arr[8] = { tri2 , tri3, tri4 , tri5 , tri6 , tri7 , tri8, tri9 };
			//for (int i = 0; i < 8; i++)
			//{
			//	if (arr[i])
			//	{
			//		for (int j = 0; j < 3; j++)
			//		{
			//			if (arr[i]->m_neighbors[j])
			//			{
			//				gb_oppositeVertexIndex(arr[i], arr[i]->m_neighbors[j]);
			//			}
			//		}
			//	}
			//}
			//
			return true;
		}
	}
	return false;
}

void gb_insertVertex(CP_TriagleMesh *mesh, CP_Plane *plane, CP_MeshFacePtr &tri, CP_MeshVertexPtr &vertex)
{
	if (gb_onEdgeInsertVertex(mesh, plane, tri, vertex))
	{
		return;
	}
	CP_MeshVertexPtr v1 = tri->m_vertex[0];
	CP_MeshVertexPtr v2 = tri->m_vertex[1];
	CP_MeshVertexPtr v3 = tri->m_vertex[2];
	// 生成3个新的三角形
	mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(vertex, v1, v2));
	CP_MeshFacePtr t1 = mesh->m_triagleArray.back();
	mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(vertex, v2, v3));
	CP_MeshFacePtr t2 = mesh->m_triagleArray.back();
	mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(vertex, v3, v1));
	CP_MeshFacePtr t3 = mesh->m_triagleArray.back();
	// 跟新顶点的三角形
	vertex->m_triagles.push_back(t1);
	vertex->m_triagles.push_back(t2);
	vertex->m_triagles.push_back(t3);
	gb_replaceArrayByValue(v1->m_triagles.data(), v1->m_triagles.size(), tri, t1);
	v1->m_triagles.push_back(t3);
	gb_replaceArrayByValue(v2->m_triagles.data(), v2->m_triagles.size(), tri, t2);
	v2->m_triagles.push_back(t1);
	gb_replaceArrayByValue(v3->m_triagles.data(), v3->m_triagles.size(), tri, t3);
	v3->m_triagles.push_back(t2);
	// 更新三角形的邻居
	t1->m_neighbors[0] = t3;
	t1->m_neighbors[1] = tri->m_neighbors[0];
	t1->m_neighbors[2] = t2;
	if (tri->m_neighbors[0])
	{
		gb_replaceArrayByValue(tri->m_neighbors[0]->m_neighbors, 3, tri, t1);
	}
	//
	t2->m_neighbors[0] = t1;
	t2->m_neighbors[1] = tri->m_neighbors[1];
	t2->m_neighbors[2] = t3;
	if (tri->m_neighbors[1])
	{
		gb_replaceArrayByValue(tri->m_neighbors[1]->m_neighbors, 3, tri, t2);
	}
	//
	t3->m_neighbors[0] = t2;
	t3->m_neighbors[1] = tri->m_neighbors[2];
	t3->m_neighbors[2] = t1;
	if (tri->m_neighbors[2])
	{
		gb_replaceArrayByValue(tri->m_neighbors[2]->m_neighbors, 3, tri, t3);
	}
	//
	tri->del();
	//
	insertPlane(*plane, CP_PlanePoint(vertex->index, vertex->m_x, vertex->m_y));
}

void gb_removeTriagle(CP_MeshFacePtr &tri)
{
	for (CP_MeshFacePtr &t : tri->m_neighbors)
	{
		if (t != NULL)
		{
			gb_replaceArrayByValue(t->m_neighbors, 3, tri, (CP_MeshFacePtr)NULL);
		}
	}
	for (CP_MeshVertexPtr &vertex : tri->m_vertex)
	{
		gb_eraseArrayByValue(vertex->m_triagles, tri);
	}
	tri->del();
}

void gb_initialization(CP_TriagleMesh *mesh, CP_Plane *plane)
{
	// 获得包围盒
	double xmin, xmax, ymin, ymax;
	gb_getBoundingBox(mesh, xmin, xmax, ymin, ymax);
	mesh->xmin = xmin;
	mesh->ymin = ymin;
	initializationPlane(*plane, mesh->m_vertexArray.size(), xmin, xmax, ymin, ymax);
	xmax = max(abs(xmin), abs(xmax));
	ymax = max(abs(ymin), abs(ymax));
	// 生成外围三角形
	int vsize = mesh->m_vertexArray.size();
	mesh->m_vertexArray.push_back(make_shared<CP_MeshVertex>(5 * xmax, 0, vsize));
	CP_MeshVertexPtr v1 = mesh->m_vertexArray.back();
	mesh->m_vertexArray.push_back(make_shared<CP_MeshVertex>(0, 5 * ymax, vsize + 1));
	CP_MeshVertexPtr v2 = mesh->m_vertexArray.back();
	mesh->m_vertexArray.push_back(make_shared<CP_MeshVertex>(-5 * xmax, -5 * ymax, vsize + 2));
	CP_MeshVertexPtr v3 = mesh->m_vertexArray.back();
	mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(v1, v2, v3));
	CP_MeshFacePtr tri = mesh->m_triagleArray[0];
	v1->m_triagles.push_back(tri);
	v2->m_triagles.push_back(tri);
	v3->m_triagles.push_back(tri);
	// 插入第一个顶点
	gb_insertVertex(mesh, plane, tri, mesh->m_vertexArray[0]);
}

void gb_triangulation(CP_TriagleMesh *mesh, CP_Plane *plane)
{
	int vsize = mesh->m_vertexArray.size() - 3;
	vector<int> shuffled_index(vsize - 1);
	for (int i = 0; i < vsize - 1; i++)
	{
		shuffled_index[i] = i + 1;
	}
	random_shuffle(shuffled_index.begin(), shuffled_index.end());
	//for (int outer_loop = 1; outer_loop < vsize; outer_loop++)
	for (int i = 0; i < vsize - 1; i++)
	{
		std::chrono::steady_clock::time_point tp0 = std::chrono::steady_clock::now();

		int outer_loop = shuffled_index[i];
		// TRACE("outer loop #%d\n", outer_loop);
		CP_MeshVertexPtr &vertex = mesh->m_vertexArray[outer_loop];
		CP_MeshFacePtr tri = NULL;
		int count1 = 0;
		int count2 = 0;
		vector<pair<int, double> > points;
		serachNearestVertex(*plane, CP_PlanePoint(vertex->index, vertex->m_x, vertex->m_y), points);

		std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();

		for (int i = 0; i < points.size() && tri == NULL; i++)
		{
			CP_MeshVertexPtr &neighbor = mesh->m_vertexArray[points[i].first];
			for (int j = 0; j < neighbor->m_triagles.size() && tri == NULL; j++)
			{
				count1++;
				if (gb_testVertexInTriagle(neighbor->m_triagles[j], vertex))
				{
					//TRACE("####%d\n", points[i].first);
					tri = neighbor->m_triagles[j];
					break;
				}
			}
		}

		std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();

		if (tri == NULL)
		{
			for (auto iter = mesh->m_vertexArray.end() - 3; iter != mesh->m_vertexArray.end() && tri == NULL; iter++)
			{
				for (int i = (*iter)->m_triagles.size() - 1; i >= 0 && tri == NULL; i--)
				{
					count2++;
					if (gb_testVertexInTriagle((*iter)->m_triagles[i], vertex))
					{
						tri = (*iter)->m_triagles[i];
						break;
					}
				}
			}
		}

		std::chrono::steady_clock::time_point tp3 = std::chrono::steady_clock::now();

		TRACE("search count %d %d\n", count1, count2);
		if (tri == NULL)
		{
			TRACE("%d bad\n", i);
#pragma omp parallel
			for (auto iter : mesh->m_triagleArray)
			{
				if (iter->exits() && tri == NULL)
				{
					if (gb_testVertexInTriagle(iter, vertex))
					{
						tri = iter;
					}
				}
			}
		}

		if (tri == NULL)
		{
			TRACE("debug\n");
		}

		std::chrono::steady_clock::time_point tp4 = std::chrono::steady_clock::now();

		int fsize = mesh->m_triagleArray.size();
		gb_insertVertex(mesh, plane, tri, vertex);

		VT_MeshTriaglePointerArray tris;
		for (auto iter = mesh->m_triagleArray.begin() + fsize; iter != mesh->m_triagleArray.end(); iter++)
		{
			tris.push_back(*iter);
		}
		std::chrono::steady_clock::time_point tp5 = std::chrono::steady_clock::now();
		while (!tris.empty())
		{
			 //TRACE("outer loop #%d test #%d\n", outer_loop, tris.size());
			gb_testEmptyCircumcircle(mesh, tris);
		}

		std::chrono::steady_clock::time_point tp6 = std::chrono::steady_clock::now();

		long long time6 = std::chrono::duration_cast<std::chrono::microseconds>(tp1 - tp0).count();
		long long time1 = std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count();
		long long time2 = std::chrono::duration_cast<std::chrono::microseconds>(tp3 - tp2).count();
		long long time3 = std::chrono::duration_cast<std::chrono::microseconds>(tp4 - tp3).count();
		long long time4 = std::chrono::duration_cast<std::chrono::microseconds>(tp5 - tp4).count();
		long long time5 = std::chrono::duration_cast<std::chrono::microseconds>(tp6 - tp5).count();

		mesh->time6 += time6;
		mesh->time1 += time1;
		mesh->time2 += time2;
		mesh->time3 += time3;
		mesh->time4 += time4;
		mesh->time5 += time5;

	}

	TRACE("%d %d %d %d %d %d\n", mesh->time6, mesh->time1, mesh->time2, mesh->time3, mesh->time4, mesh->time5);
}

void gb_finalisation(CP_TriagleMesh *mesh)
{
	for (auto iter = mesh->m_vertexArray.end() - 3; iter != mesh->m_vertexArray.end(); iter++)
	{
		// 这里一定要从后往前删！！！
		// 因为顶点的三角形列表在不断减少
		for (int i = (*iter)->m_triagles.size() - 1; i >= 0; i--)
		{
			gb_removeTriagle((*iter)->m_triagles[i]);
		}
	}
	// TRACE("finalized\n");
}

bool gb_intersectSegmentSegment(double x1, double x2, double x3, double x4, double y1, double y2, double y3, double y4)
{
	double det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (det != 0)
	{
		double x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / det;
		double y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / det;
		return (x - x1) * (x - x2) <= DBL_EPSILON && (y - y1) * (y - y2) <= DBL_EPSILON
			&& (x - x3) * (x - x4) <= DBL_EPSILON && (y - y3) * (y - y4) <= DBL_EPSILON;
	}
	else
	{
		double det1 = (x3 - x1) * (y3 - y2) - (x3 - x2) * (y3 - y1);
		if (det1 == 0 
			&& ((x3 - x1) * (x3 - x2) <= DBL_EPSILON && (y3 - y1) * (y3 - y2) <= DBL_EPSILON || (x4 - x1) * (x4 - x2) <= DBL_EPSILON && (y4 - y1) * (y4 - y2) <= DBL_EPSILON))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool gb_intersectEdgeEdge(const CP_MeshEdgePtr &edge1, const CP_MeshEdgePtr &edge2)
{
	// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
	if (edge1->m_a->index == edge2->m_a->index || edge1->m_a->index == edge2->m_b->index ||
		edge1->m_b->index == edge2->m_a->index || edge1->m_b->index == edge2->m_b->index)
	{
		return false;
	}

	double x1 = edge1->m_a->m_x;
	double y1 = edge1->m_a->m_y;
	double x2 = edge1->m_b->m_x;
	double y2 = edge1->m_b->m_y;
	double x3 = edge2->m_a->m_x;
	double y3 = edge2->m_a->m_y;
	double x4 = edge2->m_b->m_x;
	double y4 = edge2->m_b->m_y;

	return gb_intersectSegmentSegment(x1, x2, x3, x4, y1, y2, y3, y4);
}

bool gb_intersectEdgeTriagle(const CP_MeshEdgePtr &edge, const CP_MeshFacePtr &tri)
{
	for (int i = 0; i < 3; i++)
	{
		CP_MeshEdgePtr tmp = make_shared<CP_MeshEdge>(tri->m_vertex[i], tri->m_vertex[(i + 1) % 3]);
		if (gb_intersectEdgeEdge(edge, tmp))
		{
			return true;
		}
	}
	return false;
}

//bool gb_orientationVertexEdge(const CP_MeshEdgePtr &edge, const CP_MeshVertexPtr &vertex)
//{
//	// http://www.cs.cmu.edu/~quake/robust.html
//	double ax = edge->m_a->m_x;
//	double ay = edge->m_a->m_y;
//	double bx = edge->m_b->m_x;
//	double by = edge->m_b->m_y;
//	double cx = vertex->m_x;
//	double cy = vertex->m_y;
//
//	return ((ax - cx) * (by - cy) - (ay - cy) * (bx - cx)) > 0;
//}

void gb_insertEdgeCDT(CP_TriagleMesh *mesh)
{
	int init_fsize = mesh->m_triagleArray.size();
	//for (CP_MeshEdgePtr edge : mesh->m_constrainedEdgeSet)
	for(int outer_loop = 0; outer_loop < mesh->m_constrainedEdgeSet.size(); outer_loop++)
	{
		CP_MeshEdgePtr &edge = mesh->m_constrainedEdgeSet[outer_loop];
		// 上下多边形
		VT_MeshVertexPointerArray upper_polygon;
		VT_MeshVertexPointerArray lower_polygon;
		// 查找第一个相交三角形
		CP_MeshFacePtr tri_seq = NULL;
		for (CP_MeshFacePtr &tri : edge->m_a->m_triagles)
		{
			if (gb_intersectEdgeTriagle(edge, tri))
			{
				tri_seq = tri;
				break;
			}
		}
		// 如果未找到相交三角形，检查下一条约束边
		if (tri_seq == NULL)
		{
			continue;
		}
		// 查找其余的相交三角形
		while (tri_seq != NULL)
		{
			CP_MeshFacePtr old_tri_seq = tri_seq;
			// 首先将三角形顶点加入多边形
			for (CP_MeshVertexPtr &vertex : old_tri_seq->m_vertex)
			{
				if (vertex->index != edge->m_a->index && vertex->index != edge->m_b->index)
				{
					int orientation = gb_orientationVertexEdge(vertex->m_x, vertex->m_y, edge->m_a->m_x, edge->m_a->m_y, edge->m_b->m_x, edge->m_b->m_y);
					if (orientation > 0)
					{
						gb_insertArrayByValue(upper_polygon, vertex);
					}
					else if(orientation < 0)
					{
						gb_insertArrayByValue(lower_polygon, vertex);
					}
					else
					{
						TRACE("on line!\n");
						mesh->m_constrainedEdgeSet.push_back(make_shared<CP_MeshEdge>(vertex, edge->m_b));
						edge->m_b = vertex;
						tri_seq = NULL;
					}
				}
			}
			if (tri_seq == NULL)
			{
				break;
			}
			if (tri_seq->m_vertex[0]->index == edge->m_a->index 
				|| tri_seq->m_vertex[1]->index == edge->m_a->index 
				|| tri_seq->m_vertex[2]->index == edge->m_a->index)
			{
				tri_seq = gb_oppositeTriagle(tri_seq, edge->m_a);
			}
			else if (tri_seq->m_vertex[0]->index == edge->m_b->index 
				|| tri_seq->m_vertex[1]->index == edge->m_b->index 
				|| tri_seq->m_vertex[2]->index == edge->m_b->index)
			{
				tri_seq = NULL;
			}
			else
			{
				tri_seq = NULL;
				for (CP_MeshFacePtr &tri : old_tri_seq->m_neighbors)
				{
					if (tri == NULL)
					{
						continue;
					}
					if (gb_intersectEdgeTriagle(edge, tri))
					{
						tri_seq = tri;
						break;
					}
				}
			}
			gb_removeTriagle(old_tri_seq);
		}

		int fsize = mesh->m_triagleArray.size();
		gb_triangulatePseudoPolygon(mesh, upper_polygon, edge);
		// 为了保证每个面为逆时针
		CP_MeshEdgePtr reverse_edge = make_shared<CP_MeshEdge>(edge->m_b, edge->m_a);
		std::reverse(lower_polygon.begin(), lower_polygon.end());
		gb_triangulatePseudoPolygon(mesh, lower_polygon, reverse_edge);
		// 更新面的邻居
		for (int f = fsize; f < mesh->m_triagleArray.size(); f++)
		{
			CP_MeshFacePtr tri1 = mesh->m_triagleArray[f];
			for (int v = 0; v < 3; v++)
			{
				if (tri1->m_neighbors[v] == NULL)
				{
					CP_MeshVertexPtr vertex = tri1->m_vertex[v];
					for (CP_MeshFacePtr &tri2 : vertex->m_triagles)
					{
						int vv;
						for (vv = 0; vv < 3; vv++)
						{
							if (tri2->m_vertex[vv] == vertex)
							{
								break;
							}
						}
						if (tri1->m_vertex[(v + 1) % 3] == tri2->m_vertex[(vv + 2) % 3])
						{
							tri1->m_neighbors[v] = tri2;
							tri2->m_neighbors[(vv + 2) % 3] = tri1;
						}
					}
				}
			}
		}

		VT_MeshTriaglePointerArray tris;
		tris.insert(tris.begin(), mesh->m_triagleArray.begin() + fsize, mesh->m_triagleArray.end());
		while (!tris.empty())
		{
			// TRACE("test #%d\n", tris.size());
			gb_testEmptyCircumcircle(mesh, tris);
		}
	}

	mesh->m_finalTriagleArray.reserve(mesh->m_triagleArray.size());
	for (CP_MeshFacePtr &iter1 : mesh->m_triagleArray)
	{
		if (iter1->exits())
		{
			const CP_MeshVertexPtr &v0 = iter1->m_vertex[0];
			const CP_MeshVertexPtr &v1 = iter1->m_vertex[1];
			const CP_MeshVertexPtr &v2 = iter1->m_vertex[2];
			CP_MeshVertexPtr center = make_shared<CP_MeshVertex>((v0->m_x + v1->m_x + v2->m_x) / 3, (v0->m_y + v1->m_y + v2->m_y) / 3, -1);
			CP_MeshVertexPtr infinite = make_shared<CP_MeshVertex>(mesh->xmin - 10, center->m_y + 10, -1);
			CP_MeshEdgePtr e = make_shared<CP_MeshEdge>(center, infinite);
			vector<int> intersect_pool(mesh->m_constrainedEdgeSet.size(), 0);
#pragma omp parallel
			for (int i = 0; i < mesh->m_constrainedEdgeSet.size(); i++)
			{
				intersect_pool[i] = gb_intersectEdgeEdge(mesh->m_constrainedEdgeSet[i], e);
			}
			int intersect = 0;
			for (auto iter2 : intersect_pool)
			{
				intersect += iter2;
			}
			//for (const CP_MeshEdgePtr &iter2 : mesh->m_constrainedEdgeSet)
			//{
			//	if (gb_intersectEdgeEdge(iter2, e))
			//	{
			//		intersect++;
			//	}
			//}
			if (intersect % 2 == 1)
			{
				mesh->m_finalTriagleArray.push_back(iter1);
			}
		}
	}
}

void gb_triangulatePseudoPolygon(CP_TriagleMesh *mesh, VT_MeshVertexPointerArray &polygon, CP_MeshEdgePtr &edge)
{
	CP_MeshVertexPtr c = NULL;
	if (polygon.size() > 1)
	{
		int index = 0;
		c = polygon[index];
		CP_MeshFacePtr tri = make_shared<CP_MeshFace>(edge->m_a, edge->m_b, c);
		for (int i = 1; i < polygon.size(); i++)
		{
			if (!gb_testEmptyCircumcircleTriagleVertex(tri, polygon[i]))
			{
				index = i;
				c = polygon[i];
				tri = make_shared<CP_MeshFace>(edge->m_a, edge->m_b, c);
			}
		}
		CP_MeshEdgePtr edge_ac = make_shared<CP_MeshEdge>(edge->m_a, c);
		CP_MeshEdgePtr edge_cb = make_shared<CP_MeshEdge>(c, edge->m_b);
		VT_MeshVertexPointerArray polygon_ac, polygon_cb;
		polygon_ac.insert(polygon_ac.begin(), polygon.begin(), polygon.begin() + index);
		polygon_cb.insert(polygon_cb.begin(), polygon.begin() + index + 1, polygon.end());
		gb_triangulatePseudoPolygon(mesh, polygon_ac, edge_ac);
		gb_triangulatePseudoPolygon(mesh, polygon_cb, edge_cb);
	}
	if (polygon.size())
	{
		if (c == NULL)
		{
			c = polygon[0];
		}
		mesh->m_triagleArray.push_back(make_shared<CP_MeshFace>(edge->m_a, edge->m_b, c));
		edge->m_a->m_triagles.push_back(mesh->m_triagleArray.back());
		edge->m_b->m_triagles.push_back(mesh->m_triagleArray.back());
		c->m_triagles.push_back(mesh->m_triagleArray.back());

		 //TRACE("add triagle %d %d %d!\n",
			//mesh->m_triagleArray.back()->m_vertex[0]->index,
			//mesh->m_triagleArray.back()->m_vertex[1]->index,
			//mesh->m_triagleArray.back()->m_vertex[2]->index);
	}
}