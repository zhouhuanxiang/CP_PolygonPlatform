// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CP_PolygonPlatformView.cpp : CCP_PolygonPlatformView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CP_PolygonPlatform.h"
#endif

#include "MainFrm.h"

#include "CP_PolygonPlatformDoc.h"
#include "CP_PolygonPlatformView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCP_PolygonPlatformView

IMPLEMENT_DYNCREATE(CCP_PolygonPlatformView, CView)

BEGIN_MESSAGE_MAP(CCP_PolygonPlatformView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCP_PolygonPlatformView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_COMBO_AorB, &CCP_PolygonPlatformView::OnUpdateComboAorb)
	ON_COMMAND(ID_COMBO_AorB, &CCP_PolygonPlatformView::OnComboAorb)
	ON_UPDATE_COMMAND_UI(ID_SELECT_POINT, &CCP_PolygonPlatformView::OnUpdateSelectPoint)
	ON_COMMAND(ID_SELECT_POINT, &CCP_PolygonPlatformView::OnSelectPoint)
	ON_UPDATE_COMMAND_UI(ID_SELECT_LOOP, &CCP_PolygonPlatformView::OnUpdateSelectLoop)
	ON_COMMAND(ID_SELECT_LOOP, &CCP_PolygonPlatformView::OnSelectLoop)
	ON_UPDATE_COMMAND_UI(ID_SELECT_REGION, &CCP_PolygonPlatformView::OnUpdateSelectRegion)
	ON_COMMAND(ID_SELECT_REGION, &CCP_PolygonPlatformView::OnSelectRegion)
	ON_UPDATE_COMMAND_UI(ID_SELECT_POLYGON, &CCP_PolygonPlatformView::OnUpdateSelectPolygon)
	ON_COMMAND(ID_SELECT_POLYGON, &CCP_PolygonPlatformView::OnSelectPolygon)
	ON_UPDATE_COMMAND_UI(ID_SELECT_TRIANGLE, &CCP_PolygonPlatformView::OnUpdateSelectTriangle)
	ON_COMMAND(ID_SELECT_TRIANGLE, &CCP_PolygonPlatformView::OnSelectTriangle)
	ON_UPDATE_COMMAND_UI(ID_SELECT_ONLY, &CCP_PolygonPlatformView::OnUpdateSelectOnly)
	ON_COMMAND(ID_SELECT_ONLY, &CCP_PolygonPlatformView::OnSelectOnly)
	ON_UPDATE_COMMAND_UI(ID_MOVE_SAME, &CCP_PolygonPlatformView::OnUpdateMoveSame)
	ON_COMMAND(ID_MOVE_SAME, &CCP_PolygonPlatformView::OnMoveSame)
	ON_UPDATE_COMMAND_UI(ID_VIEW_A, &CCP_PolygonPlatformView::OnUpdateViewA)
	ON_COMMAND(ID_VIEW_A, &CCP_PolygonPlatformView::OnViewA)
	ON_UPDATE_COMMAND_UI(ID_VIEW_B, &CCP_PolygonPlatformView::OnUpdateViewB)
	ON_COMMAND(ID_VIEW_B, &CCP_PolygonPlatformView::OnViewB)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POINT_ID, &CCP_PolygonPlatformView::OnUpdateViewPointId)
	ON_COMMAND(ID_VIEW_POINT_ID, &CCP_PolygonPlatformView::OnViewPointId)
	ON_UPDATE_COMMAND_UI(ID_VIEW_T_FACE, &CCP_PolygonPlatformView::OnUpdateViewTFace)
	ON_COMMAND(ID_VIEW_T_FACE, &CCP_PolygonPlatformView::OnViewTFace)
	ON_COMMAND(ID_EDGE_NUMBER, &CCP_PolygonPlatformView::OnEdgeNumber)
	ON_COMMAND(ID_TOLERANCE, &CCP_PolygonPlatformView::OnTolerance)
	ON_COMMAND(ID_NEW_RIGHT_OUTLOOP, &CCP_PolygonPlatformView::OnNewRightOutloop)
	ON_COMMAND(ID_NEW_RIGHT_INLOOP, &CCP_PolygonPlatformView::OnNewRightInloop)
	ON_COMMAND(ID_VIEW_FIT, &CCP_PolygonPlatformView::OnViewFit)
	ON_COMMAND(ID_VIEW_STANDARD, &CCP_PolygonPlatformView::OnViewStandard)
	ON_COMMAND(ID_ADD_OUTLOOP, &CCP_PolygonPlatformView::OnAddOutloop)
	ON_COMMAND(ID_ADD_INLOOP, &CCP_PolygonPlatformView::OnAddInloop)
	ON_COMMAND(ID_ADD_POINT, &CCP_PolygonPlatformView::OnAddPoint)
	ON_COMMAND(ID_DELETE, &CCP_PolygonPlatformView::OnDelete)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_CHECK, &CCP_PolygonPlatformView::OnCheck)
END_MESSAGE_MAP()

// CCP_PolygonPlatformView 构造/析构

CCP_PolygonPlatformView::CCP_PolygonPlatformView()
{
	// TODO: 在此处添加构造代码

}

CCP_PolygonPlatformView::~CCP_PolygonPlatformView()
{
}

BOOL CCP_PolygonPlatformView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCP_PolygonPlatformView 绘制

//void CCP_PolygonPlatformView::OnDraw(CDC* /*pDC*/)
//{
//	CCP_PolygonPlatformDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//
//	// TODO: 在此处为本机数据添加绘制代码
//}


// CCP_PolygonPlatformView 打印


void CCP_PolygonPlatformView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCP_PolygonPlatformView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCP_PolygonPlatformView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCP_PolygonPlatformView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

//void CCP_PolygonPlatformView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}

void CCP_PolygonPlatformView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCP_PolygonPlatformView 诊断

#ifdef _DEBUG
void CCP_PolygonPlatformView::AssertValid() const
{
	CView::AssertValid();
}

void CCP_PolygonPlatformView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCP_PolygonPlatformDoc* CCP_PolygonPlatformView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCP_PolygonPlatformDoc)));
	return (CCP_PolygonPlatformDoc*)m_pDocument;
}
#endif //_DEBUG

void  CCP_PolygonPlatformView::mb_statusSetText(char* s1, char* s2)
{
	CRect r;
	// View中获取MainFrame指针
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	//获取状态栏指针
	CMFCRibbonStatusBar *statusBar =
		(CMFCRibbonStatusBar *)pMainFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	//获取RibbonStatusBar上的元素
	CMFCRibbonLabel *locLabel;
	if (s1 != NULL)
	{
		locLabel = (CMFCRibbonLabel *)statusBar->GetElement(0);
		if (locLabel != NULL)
		{
			r = locLabel->GetRect();
			if (r.right - r.left<300)
				r.right += 300;
			locLabel->SetRect(&r);
			locLabel->SetText(s1);
		} // if结束
	} // 外if结束
	if (s2 != NULL)
	{
		locLabel = (CMFCRibbonLabel *)statusBar->GetExElement(0);
		if (locLabel != NULL)
		{
			r = locLabel->GetRect();
			if (r.right - r.left<300)
				r.left -= 300;
			locLabel->SetRect(&r);
			locLabel->SetText(s2);
		} // if结束
	} // 外if结束
	statusBar->Invalidate();
} // 类CCP_PolygonPlatformView的成员函数mb_statusSetText结束

  // CCP_PolygonPlatformView 绘制
void CCP_PolygonPlatformView::OnDraw(CDC* pDC)
{
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
	CRect r;
	GetClientRect(&r);
	if (pDoc->m_flagShowTriangleFace)
	{
		for (const CP_MeshEdgePtr &e : pDoc->m_triagleMesh.m_edgeArray)
		{
			VT_PointArray pa(2);
			pa[0] = CP_Point(e->m_a->m_x, e->m_a->m_y);
			pa[1] = CP_Point(e->m_b->m_x, e->m_b->m_y);
			gb_drawPointArrayLine(pDC, pa,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				200, 0, 255,
				3);
		}
		// TODO
		//for (auto &iter : pDoc->m_triagleMesh.m_triagleArray)
		//{
		//	if (iter->exits())
		//	{
		//		VT_PointArray pa(3);

		//		for (int i = 0; i < 3; i++)
		//		{
		//			CP_MeshVertexPtr vertex = iter->m_vertex[i];
		//			pa[i] = CP_Point(vertex->m_x, vertex->m_y);
		//		}
		//		gb_drawPointArrayLoop(pDC, pa,
		//			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
		//			200, 0, 255,
		//			3);
		//	}
		//}
	} // if (pDoc->m_flagShowTriangleFace)结束
	if (!pDoc->m_flagShowSelect)
	{
		if (pDoc->m_flagShowA)
		{
			gb_drawPolygonLoop(pDC, pDoc->m_a,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				255, 0, 0,
				0, 255, 0,
				5);
			gb_drawPolygonPoint(pDC, pDoc->m_a,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				0, 0, 0,
				5);
			if (pDoc->m_flagShowPointID)
				gb_drawPolygonPointID(pDC, pDoc->m_a,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					0, 0, 0);
		} // if结束
		if (pDoc->m_flagShowB)
		{
			gb_drawPolygonLoop(pDC, pDoc->m_b,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				255, 0, 255,
				0, 0, 255,
				5);
			gb_drawPolygonPoint(pDC, pDoc->m_b,
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				0, 0, 0,
				5);
			if (pDoc->m_flagShowPointID)
				gb_drawPolygonPointID(pDC, pDoc->m_b,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					0, 0, 255);
		} // if结束
	} // if(!pDoc->m_flagShowSelect)结束: 显示所有的基本内容
	if (pDoc->m_flagSelect) // 显示选择集
	{
		switch (pDoc->m_flagSelectType)
		{
		case 1: // 点。
			if (pDoc->m_flagSelectPolygon == 0)
				gb_drawPointGlobal(pDC, pDoc->m_a.m_pointArray[pDoc->m_flagSelectID],
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					50, 50, 50,
					6); // A
			else gb_drawPointGlobal(pDC, pDoc->m_b.m_pointArray[pDoc->m_flagSelectID],
				pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
				50, 50, 50,
				6); // B
			break;
		case 2: // 环。
			if (pDoc->m_flagSelectPolygon == 0)
			{
				if (pDoc->m_flagSelectID == 0)
					gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						255, 100, 100,
						5); // 外环
				else
					gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						100, 255, 50,
						5); // 内环
			} // A
			else
			{
				if (pDoc->m_flagSelectID == 0)
					gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						200, 100, 200,
						5); // 外环
				else
					gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[pDoc->m_flagSelectID],
						pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
						100, 50, 255,
						5); // 内环
			} // B
			break;
		case 3: // 区域。
			int i, n;
			if (pDoc->m_flagSelectPolygon == 0)
			{
				n = pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray.size();
				for (i = 0; i<n; i++)
					if (i == 0)
						gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
							pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
							255, 100, 100,
							5); // 外环
					else
						gb_drawLoop(pDC, pDoc->m_a.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
							pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
							100, 255, 50,
							5); // 内环
			} // A
			else
			{
				n = pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray.size();
				for (i = 0; i<n; i++)
					if (i == 0)
						gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
							pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
							200, 100, 200,
							5); // 外环
					else
						gb_drawLoop(pDC, pDoc->m_b.m_regionArray[pDoc->m_flagSelectRegion].m_loopArray[i],
							pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
							100, 50, 255,
							5); // 内环
			} // B
			break;
		case 4: // 多边形。
			if (pDoc->m_flagSelectPolygon == 0)
			{
				gb_drawPolygonLoop(pDC, pDoc->m_a,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					255, 100, 100,
					100, 255, 50,
					5);
				gb_drawPolygonPoint(pDC, pDoc->m_a,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					50, 50, 50,
					6);
			} // A
			else
			{
				gb_drawPolygonLoop(pDC, pDoc->m_b,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					200, 100, 200,
					100, 50, 255,
					5);
				gb_drawPolygonPoint(pDC, pDoc->m_b,
					pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
					50, 50, 50,
					6);
			} // B
			break;
		case 5: // 剖分三角形。
			break;
		} // switch结束
	} // if(pDoc->m_flagSelect)结束
	if (pDoc->m_flagAdd == 1) // 外环
	{
		gb_drawPointArrayLine(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			200, 0, 255,
			1);
		gb_drawPointArrayPoint(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 0, 0,
			1);
	} // if (pDoc->m_flagAdd == 1)结束
	if (pDoc->m_flagAdd == 2) // 内环
	{
		gb_drawPointArrayLine(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 200, 255,
			1);
		gb_drawPointArrayPoint(pDC, pDoc->m_flagAddPointArray,
			pDoc->m_scale, pDoc->m_translation, r.right, r.bottom,
			0, 0, 0,
			1);
	} // if (pDoc->m_flagAdd == 2)结束
}

void gb_drawLoop(CDC* pDC, CP_Loop& p,
	double scale, CP_Point translation, int screenX, int screenY,
	int r, int g, int b, int size)
{
	int n = p.m_pointIDArray.size();
	if (n <= 0)
		return;
	CPen pen(0, size, RGB(r, g, b));
	CPen * penOld = (CPen *)pDC->SelectObject(&pen);
	int i, k;
	i = p.m_pointIDArray[n - 1];
	CP_Point pg = p.m_polygon->m_pointArray[i];
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
	pDC->MoveTo((int)(ps.m_x + 0.5), (int)(ps.m_y + 0.5));
	for (k = 0; k<n; k++)
	{
		i = p.m_pointIDArray[k];
		pg = p.m_polygon->m_pointArray[i];
		gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
		pDC->LineTo((int)(ps.m_x + 0.5), (int)(ps.m_y + 0.5));
	} //for结束
	pDC->SelectObject(penOld);
} // 函数gb_drawPolygonLoop结束

void gb_drawPointArrayLine(CDC* pDC, VT_PointArray& pa,
	double scale, CP_Point translation, int screenX, int screenY,
	int r, int g, int b, int size)
{
	int n = pa.size();
	if (n <= 0)
		return;
	CPen pen(0, size, RGB(r, g, b));
	CPen * penOld = (CPen *)pDC->SelectObject(&pen);
	int i;
	CP_Point pg = pa[0];
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
	pDC->MoveTo((int)(ps.m_x + 0.5), (int)(ps.m_y + 0.5));
	for (i = 0; i<n; i++)
	{
		pg = pa[i];
		gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
		pDC->LineTo((int)(ps.m_x + 0.5), (int)(ps.m_y + 0.5));
	} //for结束
	pDC->SelectObject(penOld);
} // 函数gb_drawPointArrayLine结束

void gb_drawPointArrayLoop(CDC* pDC, VT_PointArray& pa,
	double scale, CP_Point translation, int screenX, int screenY,
	int r, int g, int b, int size)
{
	int n = pa.size();
	if (n <= 0)
		return;
	CPen pen(0, size, RGB(r, g, b));
	CPen * penOld = (CPen *)pDC->SelectObject(&pen);
	int i;
	CP_Point pg = pa[0];
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
	pDC->MoveTo((int)(ps.m_x + 0.5), (int)(ps.m_y + 0.5));
	for (i = 0; i<=n; i++)
	{
		pg = pa[i % n];
		gb_pointConvertFromGlobalToScreen(ps, pg, scale, translation, screenX, screenY);
		pDC->LineTo((int)(ps.m_x + 0.5), (int)(ps.m_y + 0.5));
	} //for结束
	pDC->SelectObject(penOld);
} // 函数gb_drawPointArrayLoop结束

void gb_drawPointArrayPoint(CDC* pDC, VT_PointArray& pa,
	double scale, CP_Point translation, int screenX, int screenY,
	int r, int g, int b, int size)
{
	int n = pa.size();
	int i;
	for (i = 0; i<n; i++)
	{
		gb_drawPointGlobal(pDC, pa[i], scale, translation, screenX, screenY, r, g, b, size);
	} // for结束
} // 函数gb_drawPointArrayPoint结束

void gb_drawPointGlobal(CDC* pDC, CP_Point pointGlobal, double scale, CP_Point translation, int screenX, int screenY, int r, int g, int b, int size)
{
	CP_Point ps;
	gb_pointConvertFromGlobalToScreen(ps, pointGlobal, scale, translation, screenX, screenY);
	gb_drawPointScreen(pDC, (int)(ps.m_x + 0.5), (int)(ps.m_y + 0.5), r, g, b, size);
} // 函数gb_drawPointScreen结束

void gb_drawPointScreen(CDC* pDC, int x, int y, int r, int g, int b, int size)
{
	CBrush brush(RGB(r, g, b));
	CBrush* brushOld = (CBrush*)pDC->SelectObject(&brush);;
	// 绘制格子
	CRect rect(x - size, y - size, x + size, y + size);
	pDC->Ellipse(&rect);
	pDC->SelectObject(brushOld);
} // 函数gb_drawPointScreen结束

void gb_drawPolygonLoop(CDC* pDC, CP_Polygon& p,
	double scale, CP_Point translation, int screenX, int screenY,
	int outR, int outG, int outB,
	int inR, int inG, int inB,
	int size)
{
	unsigned int i, k;
	for (i = 0; i<p.m_regionArray.size(); i++)
	{
		for (k = 0; k<p.m_regionArray[i].m_loopArray.size(); k++)
		{
			if (k == 0)
				gb_drawLoop(pDC, p.m_regionArray[i].m_loopArray[k],
					scale, translation, screenX, screenY,
					outR, outG, outB, size);
			else
				gb_drawLoop(pDC, p.m_regionArray[i].m_loopArray[k],
					scale, translation, screenX, screenY,
					inR, inG, inB, size);
		} // 内部for结束
	} // 外部for结束
} // 函数gb_drawPolygonLoop结束

void gb_drawPolygonPoint(CDC* pDC, CP_Polygon& p, double scale, CP_Point translation, int screenX, int screenY, int r, int g, int b, int size)
{
	int n = p.m_pointArray.size();
	int i;
	for (i = 0; i<n; i++)
	{
		gb_drawPointGlobal(pDC, p.m_pointArray[i], scale, translation, screenX, screenY, r, g, b, size);
	} // for结束
} // 函数gb_drawPolygonPoint结束

void gb_drawPolygonPointID(CDC* pDC, CP_Polygon& p,
	double scale, CP_Point translation, int screenX, int screenY,
	int r, int g, int b)
{
	COLORREF cOld = pDC->SetTextColor(RGB(r, g, b));
	int nr = p.m_regionArray.size();
	int nL, nLv, ir, iL, iLv, v;
	CP_Point ps;
	char buffer[100];
	for (ir = 0; ir<nr; ir++)
	{
		nL = p.m_regionArray[ir].m_loopArray.size();
		for (iL = 0; iL<nL; iL++)
		{
			nLv = p.m_regionArray[ir].m_loopArray[iL].m_pointIDArray.size();
			for (iLv = 0; iLv<nLv; iLv++)
			{
				v = p.m_regionArray[ir].m_loopArray[iL].m_pointIDArray[iLv];
				gb_pointConvertFromGlobalToScreen(ps, p.m_pointArray[v],
					scale, translation, screenX, screenY);
				// TODO
				sprintf_s(buffer, 100, "[%1d]R%1dL%1dV%1d", v, ir, iL, iLv);
				//sprintf_s(buffer, 100, "P%1d", v);
				pDC->TextOutA((int)(ps.m_x + 0.5), (int)(ps.m_y + 0.5), buffer);
			} // for(iLv)结束
		} // for(iL)结束
	} // for(ir)结束
	pDC->SetTextColor(cOld);
} // 函数gb_drawPolygonPointID结束

void CCP_PolygonPlatformView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	//	OnContextMenu(this, point);
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagAdd == 3)
	{
		pDoc->m_flagAdd = 0;
		mb_statusSetText("点添加操作结束。", "请继续其他操作");
		return;
	} // if结束
	if ((pDoc->m_flagAdd == 1) || (pDoc->m_flagAdd == 2)) // 环
	{
		int n = pDoc->m_flagAddPointArray.size();
		if (n <= 0)
		{
			pDoc->m_flagAdd = 0;
			mb_statusSetText("环添加操作结束。", "请继续其他操作");
			return;
		} // if结束
		if (n<3)
		{
			if (MessageBox("构成环的点不能少于3个，是否继续输入? 选是则继续，否则放弃前面输入的点。"
				, "操作确认"
				, MB_ICONQUESTION | MB_YESNO) == IDYES)
				return;
			if (pDoc->m_flagAdd == 1)
				mb_statusSetText("外环点添加操作结束。", "请继续其他操作");
			else mb_statusSetText("内环点添加操作结束。", "请继续其他操作");
			pDoc->m_flagAdd = 0;
			pDoc->m_flagAddPointArray.clear();
			return;
		} // if结束
		int i, k;
		CP_Polygon* pn;
		if (pDoc->m_flagAddIDPolygon == 0)
			pn = &(pDoc->m_a);
		else pn = &(pDoc->m_b);
		int nv = pn->m_pointArray.size();
		pn->m_pointArray.resize(nv + n);
		for (i = 0, k = nv; i<n; i++, k++)
		{
			pn->m_pointArray[k].m_x = pDoc->m_flagAddPointArray[i].m_x;
			pn->m_pointArray[k].m_y = pDoc->m_flagAddPointArray[i].m_y;
		} // for结束
		if (pDoc->m_flagAdd == 1)
		{
			pDoc->m_flagAddIDLoop = 0;
			pDoc->m_flagAddIDRegion = pn->m_regionArray.size();
			pn->m_regionArray.resize(pDoc->m_flagAddIDRegion + 1);
			pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.resize(1);
			pn->m_regionArray[pDoc->m_flagAddIDRegion].m_polygon = pn;
			pn->m_regionArray[pDoc->m_flagAddIDRegion].m_regionIDinPolygon = pDoc->m_flagAddIDRegion;
		}
		else
		{
			pDoc->m_flagAddIDLoop = pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size();
			pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.resize(pDoc->m_flagAddIDLoop + 1);
		} // if/else结束
		pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_polygon = pn;

		pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_regionIDinPolygon = pDoc->m_flagAddIDRegion;

		pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_loopIDinRegion = pDoc->m_flagAddIDLoop;

		pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.resize(n);

		for (i = 0, k = nv; i<n; i++, k++)
		{
			pn->m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray[i] = k;
		} // for结束
		if (pDoc->m_flagAdd == 1)
			mb_statusSetText("外环点添加操作结束。", "请继续其他操作");
		else mb_statusSetText("内环点添加操作结束。", "请继续其他操作");
		pDoc->m_flagAdd = 0;
		pDoc->m_flagAddPointArray.clear();
		Invalidate();
		return;
	} // 外if结束
}

// CCP_PolygonPlatformView 消息处理程序
void CCP_PolygonPlatformView::OnUpdateComboAorb(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
	if (robbon_bar == NULL)
		return;
	CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB); // 获取编辑框句柄
	if (pbox == NULL)
		return;
	pbox->AddItem("多边形A");
	pbox->AddItem("多边形B");
}

void CCP_PolygonPlatformView::OnComboAorb()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
	if (robbon_bar == NULL)
		return;
	CMFCRibbonComboBox* pbox = (CMFCRibbonComboBox*)robbon_bar->FindByID(ID_COMBO_AorB); // 获取编辑框句柄
	if (pbox == NULL)
		return;
	pbox->AddItem("多边形A");
	pbox->AddItem("多边形B");
	int i = pbox->GetCurSel();
	pbox->SelectItem(i);
	if (i == 0)
		pDoc->m_flagBuildA = true;
	else pDoc->m_flagBuildA = false;
	Invalidate();
}

void CCP_PolygonPlatformView::OnEdgeNumber()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CString string;
	CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); //获取Ribbon bar 句柄
	if (robbon_bar == NULL)
		return;
	CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_EDGE_NUMBER); // 获取编辑框句柄
	if (slider == NULL)
		return;
	string = slider->GetEditText(); // 获取数字
	int i = atoi(string);
	if (i<3)
		i = 3;
	if (i>10000)
		i = 10000;
	pDoc->m_edgeNumber = i;
	string.Format("%d", i);
	slider->SetEditText(string);
	Invalidate();
}

void CCP_PolygonPlatformView::OnTolerance()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CString string;
	CMFCRibbonBar* robbon_bar = ((CFrameWndEx*)AfxGetMainWnd())->GetRibbonBar(); //获取Ribbon bar 句柄
	if (robbon_bar == NULL)
		return;
	CMFCRibbonEdit* slider = (CMFCRibbonEdit*)robbon_bar->FindByID(ID_TOLERANCE); // 获取编辑框句柄
	if (slider == NULL)
		return;
	string = slider->GetEditText(); // 获取数字
	double d = atof(string);
	if (d <= 0.0)
		d = 1e-6;
	pDoc->m_tolerance = d;
	string.Format("%g", pDoc->m_tolerance);
	slider->SetEditText(string);
	Invalidate();
}

void CCP_PolygonPlatformView::OnNewRightOutloop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect r;
	GetClientRect(&r);
	double dr = (r.right<r.bottom ? r.right : r.bottom);
	dr /= 3;
	if (pDoc->m_flagBuildA)
		gb_polygonNewOutLoopRegular(pDoc->m_a, pDoc->m_edgeNumber, dr, 0.0, 0.0);
	else gb_polygonNewOutLoopRegular(pDoc->m_b, pDoc->m_edgeNumber, dr, 0.0, 0.0);
	Invalidate();
	char s[100];
	sprintf_s(s, 100, "新外环是正%1d边形", pDoc->m_edgeNumber);
	if (pDoc->m_flagBuildA)
		mb_statusSetText("在多边形A中创建了新外环。", s);
	else mb_statusSetText("在多边形B中创建了新外环。", s);
}

void CCP_PolygonPlatformView::OnViewStandard()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->m_scale = 1.0; // 缩放比例
	pDoc->m_translation.m_x = 0.0; // 坐标平移量
	pDoc->m_translation.m_y = 0.0; // 坐标平移量
	Invalidate();
	mb_statusSetText("标准化坐标系。", "不平移，也不缩放。");
}

void CCP_PolygonPlatformView::OnViewFit()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	double dxMin, dyMin, dxMax, dyMax, ra, s1, s2;
	int na = pDoc->m_a.m_pointArray.size();
	int nb = pDoc->m_b.m_pointArray.size();
	if ((na == 0) && (nb == 0))
		return;
	if (na != 0)
	{
		dxMin = pDoc->m_a.m_pointArray[0].m_x;
		dxMax = dxMin;
		dyMin = pDoc->m_a.m_pointArray[0].m_y;
		dyMax = dyMin;
	}
	else if (nb != 0)
	{
		dxMin = pDoc->m_b.m_pointArray[0].m_x;
		dxMax = dxMin;
		dyMin = pDoc->m_b.m_pointArray[0].m_y;
		dyMax = dyMin;
	}
	int i;
	for (i = 0; i<na; i++)
	{
		if (dxMin>pDoc->m_a.m_pointArray[i].m_x)
			dxMin = pDoc->m_a.m_pointArray[i].m_x;
		if (dxMax<pDoc->m_a.m_pointArray[i].m_x)
			dxMax = pDoc->m_a.m_pointArray[i].m_x;
		if (dyMin>pDoc->m_a.m_pointArray[i].m_y)
			dyMin = pDoc->m_a.m_pointArray[i].m_y;
		if (dyMax<pDoc->m_a.m_pointArray[i].m_y)
			dyMax = pDoc->m_a.m_pointArray[i].m_y;
	} // for结束
	for (i = 0; i<nb; i++)
	{
		if (dxMin>pDoc->m_b.m_pointArray[i].m_x)
			dxMin = pDoc->m_b.m_pointArray[i].m_x;
		if (dxMax<pDoc->m_b.m_pointArray[i].m_x)
			dxMax = pDoc->m_b.m_pointArray[i].m_x;
		if (dyMin>pDoc->m_b.m_pointArray[i].m_y)
			dyMin = pDoc->m_b.m_pointArray[i].m_y;
		if (dyMax<pDoc->m_b.m_pointArray[i].m_y)
			dyMax = pDoc->m_b.m_pointArray[i].m_y;
	} // for结束
	CRect r;
	GetClientRect(&r);
	r.bottom -= 40;
	r.right -= 40;
	pDoc->m_translation.m_x = (dxMin + dxMax) / 2;
	pDoc->m_translation.m_y = (dyMin + dyMax) / 2;
	ra = (double)(dxMax - dxMin);
	if (ra<10e-8)
		ra = 1;
	s1 = (double)(r.right - r.left) / ra;
	ra = (double)(dyMax - dyMin);
	if (ra<10e-8)
		ra = 1;
	s2 = (double)(r.bottom - r.top) / ra;
	pDoc->m_scale = (s1<s2 ? s1 : s2);
	Invalidate();
	mb_statusSetText("自适应显示!", "尽量充满屏幕!");
}

void CCP_PolygonPlatformView::OnUpdateSelectPoint(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 1)
		pCmdUI->SetCheck(true);
	else pCmdUI->SetCheck(false);
}

void CCP_PolygonPlatformView::OnSelectPoint()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 1)
		pDoc->m_flagSelectType = 0;
	else pDoc->m_flagSelectType = 1;
	pDoc->m_flagSelect = false;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateSelectLoop(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 2)
		pCmdUI->SetCheck(true);
	else pCmdUI->SetCheck(false);
}

void CCP_PolygonPlatformView::OnSelectLoop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 2)
		pDoc->m_flagSelectType = 0;
	else pDoc->m_flagSelectType = 2;
	pDoc->m_flagSelect = false;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateSelectRegion(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 3)
		pCmdUI->SetCheck(true);
	else pCmdUI->SetCheck(false);
}

void CCP_PolygonPlatformView::OnSelectRegion()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 3)
		pDoc->m_flagSelectType = 0;
	else pDoc->m_flagSelectType = 3;
	pDoc->m_flagSelect = false;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateSelectPolygon(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 4)
		pCmdUI->SetCheck(true);
	else pCmdUI->SetCheck(false);
}

void CCP_PolygonPlatformView::OnSelectPolygon()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 4)
		pDoc->m_flagSelectType = 0;
	else pDoc->m_flagSelectType = 4;
	pDoc->m_flagSelect = false;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateSelectTriangle(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 5)
		pCmdUI->SetCheck(true);
	else pCmdUI->SetCheck(false);
}

void CCP_PolygonPlatformView::OnSelectTriangle()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagSelectType == 5)
		pDoc->m_flagSelectType = 0;
	else pDoc->m_flagSelectType = 5;
	pDoc->m_flagSelect = false;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateSelectOnly(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pCmdUI->SetCheck(pDoc->m_flagShowSelect);
}

void CCP_PolygonPlatformView::OnSelectOnly()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->m_flagShowSelect ^= true;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CP_Point ps, pg;
	ps.m_x = point.x;
	ps.m_y = point.y;
	CRect r;
	GetClientRect(&r);
	gb_pointConvertFromScreenToGlobal(pg, ps,
		pDoc->m_scale, pDoc->m_translation, r.right, r.bottom);
	pDoc->m_basePoint = pg;
	bool flagSuceess = false;
	CP_Polygon* pn0;
	if (pDoc->m_flagAddIDPolygon == 0)
		pn0 = &(pDoc->m_a);
	else pn0 = &(pDoc->m_b);
	if (pDoc->m_flagAdd == 3)
	{
		gb_insertPointInPolygon(*pn0, pDoc->m_flagAddIDRegion, pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop, pg);
		mb_statusSetText("点添加操作成功。", "用鼠标右键结束点添加操作");
		Invalidate();
		CView::OnLButtonDown(nFlags, point);
		return;
	} // 外if结束
	if ((pDoc->m_flagAdd == 1) || (pDoc->m_flagAdd == 2)) // 环
	{
		pDoc->m_flagAddPointArray.push_back(pg);
		if (pDoc->m_flagAdd == 1)
			mb_statusSetText("外环点添加操作成功。", "用鼠标右键结束外环添加操作");
		else mb_statusSetText("内环点添加操作成功。", "用鼠标右键结束内环添加操作");
		Invalidate();
		CView::OnLButtonDown(nFlags, point);
		return;
	} // 外if结束

	if ((!(pDoc->m_flagShowA)) && (!(pDoc->m_flagShowB)))
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	} // if结束
	double da, db;
	int ida, idb, ira, irb;
	CP_Polygon* pn1;
	VT_IntArray* pSet0;
	VT_IntArray* pSet1;
	CP_Point p0, p1;
	switch (pDoc->m_flagSelectType)
	{
	case 1: // 点。
		if (pDoc->m_flagShowA)
			gb_distanceMinPointPolygon(da, ida, pg, pDoc->m_a);
		else ida = -1;
		if (pDoc->m_flagShowB)
			gb_distanceMinPointPolygon(db, idb, pg, pDoc->m_b);
		else idb = -1;
		if (ida >= 0)
		{
			if (idb >= 0)
			{
				if (da <= db)
				{
					pDoc->m_flagSelect = true;
					pDoc->m_flagSelectPolygon = 0;
					pDoc->m_flagSelectID = ida;
				}
				else
				{
					pDoc->m_flagSelect = true;
					pDoc->m_flagSelectPolygon = 1;
					pDoc->m_flagSelectID = idb;
				}
			}
			else
			{
				pDoc->m_flagSelect = true;
				pDoc->m_flagSelectPolygon = 0;
				pDoc->m_flagSelectID = ida;
			}
		}
		else
		{
			if (idb >= 0)
			{
				pDoc->m_flagSelect = true;
				pDoc->m_flagSelectPolygon = 1;
				pDoc->m_flagSelectID = idb;
			}
			else pDoc->m_flagSelect = false;
		}
		if (pDoc->m_flagSelect)
		{
			if (pDoc->m_flagMoveSame)
			{
				if (pDoc->m_flagSelectPolygon == 0)
				{
					pn0 = &(pDoc->m_a);
					pn1 = &(pDoc->m_b);
					pSet0 = &(pDoc->m_flagSelectIDSetInA);
					pSet1 = &(pDoc->m_flagSelectIDSetInB);
				}
				else
				{
					pn0 = &(pDoc->m_b);
					pn1 = &(pDoc->m_a);
					pSet0 = &(pDoc->m_flagSelectIDSetInB);
					pSet1 = &(pDoc->m_flagSelectIDSetInA);
				} // if/else结束
				gb_intArrayInitPoint(*pSet0, *pn0, pDoc->m_flagSelectID, pDoc->m_tolerance);
				gb_intArrayInitPolygonSamePoint(*pSet1, *pn1, *pSet0, *pn0, pDoc->m_tolerance);
			} // if结束
		} // if结束
		break;
	case 2: // 环。
	case 3: // 区域。
	case 4: // 多边形。
		if (pDoc->m_flagShowA)
			gb_distanceMinPointLoop(da, ira, ida, pg, pDoc->m_a);
		else ida = -1;
		if (pDoc->m_flagShowB)
			gb_distanceMinPointLoop(db, irb, idb, pg, pDoc->m_b);
		else idb = -1;
		if (ida >= 0)
		{
			if (idb >= 0)
			{
				if (da <= db)
				{
					pDoc->m_flagSelect = true;
					pDoc->m_flagSelectPolygon = 0;
					pDoc->m_flagSelectRegion = ira;
					pDoc->m_flagSelectID = ida;
				}
				else
				{
					pDoc->m_flagSelect = true;
					pDoc->m_flagSelectPolygon = 1;
					pDoc->m_flagSelectRegion = irb;
					pDoc->m_flagSelectID = idb;
				}
			}
			else
			{
				pDoc->m_flagSelect = true;
				pDoc->m_flagSelectPolygon = 0;
				pDoc->m_flagSelectRegion = ira;
				pDoc->m_flagSelectID = ida;
			}
		}
		else
		{
			if (idb >= 0)
			{
				pDoc->m_flagSelect = true;
				pDoc->m_flagSelectPolygon = 1;
				pDoc->m_flagSelectRegion = irb;
				pDoc->m_flagSelectID = idb;
			}
			else pDoc->m_flagSelect = false;
		}
		if (pDoc->m_flagSelect)
		{
			if (pDoc->m_flagMoveSame)
			{
				if (pDoc->m_flagSelectPolygon == 0)
				{
					pn0 = &(pDoc->m_a);
					pn1 = &(pDoc->m_b);
					pSet0 = &(pDoc->m_flagSelectIDSetInA);
					pSet1 = &(pDoc->m_flagSelectIDSetInB);
				}
				else
				{
					pn0 = &(pDoc->m_b);
					pn1 = &(pDoc->m_a);
					pSet0 = &(pDoc->m_flagSelectIDSetInB);
					pSet1 = &(pDoc->m_flagSelectIDSetInA);
				} // if/else结束
				if (pDoc->m_flagSelectType == 4)
					gb_intArrayInitPolygon(*pSet0, *pn0);
				else if (pDoc->m_flagSelectType == 3)
					gb_intArrayInitRegion(*pSet0, *pn0, pDoc->m_flagSelectRegion, pDoc->m_tolerance);
				else if (pDoc->m_flagSelectType == 2)
					gb_intArrayInitLoop(*pSet0, *pn0, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, pDoc->m_tolerance);
				gb_intArrayInitPolygonSamePoint(*pSet1, *pn1, *pSet0, *pn0, pDoc->m_tolerance);
			} // if结束
		} // if结束
		break;
	case 5: // 剖分三角形。
		break;
	} // switch结束

	CView::OnLButtonDown(nFlags, point);
	Invalidate();
	if (pDoc->m_flagSelect)
		pDoc->m_flagMouseDown = true;
}

void CCP_PolygonPlatformView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (!pDoc->m_flagSelect)
		return;
	if (!pDoc->m_flagMouseDown)
		return;
	CP_Point ps, pg;
	ps.m_x = point.x;
	ps.m_y = point.y;
	CRect r;
	GetClientRect(&r);
	gb_pointConvertFromScreenToGlobal(pg, ps,
		pDoc->m_scale, pDoc->m_translation, r.right, r.bottom);
	double vx = pg.m_x - pDoc->m_basePoint.m_x;
	double vy = pg.m_y - pDoc->m_basePoint.m_y;
	if (pDoc->m_flagMoveSame)
	{
		gb_movePointIntArray(pDoc->m_a, pDoc->m_flagSelectIDSetInA, vx, vy);
		gb_movePointIntArray(pDoc->m_b, pDoc->m_flagSelectIDSetInB, vx, vy);
		pDoc->m_basePoint = pg;
		Invalidate();
		pDoc->m_flagMouseDown = false;
		return;
	} // if结束
	switch (pDoc->m_flagSelectType)
	{
	case 1: // 点。
		if (pDoc->m_flagSelectPolygon == 0) // A
			gb_movePoint(pDoc->m_a, pDoc->m_flagSelectID, vx, vy);
		else gb_movePoint(pDoc->m_b, pDoc->m_flagSelectID, vx, vy);
		break;
	case 2: // 环。
		if (pDoc->m_flagSelectPolygon == 0) // A
			gb_moveLoop(pDoc->m_a, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
		else gb_moveLoop(pDoc->m_b, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
		break;
	case 3: // 区域。
		if (pDoc->m_flagSelectPolygon == 0) // A
			gb_moveRegion(pDoc->m_a, pDoc->m_flagSelectRegion, vx, vy);
		else gb_moveRegion(pDoc->m_b, pDoc->m_flagSelectRegion, vx, vy);
		break;
	case 4: // 多边形。
		if (pDoc->m_flagSelectPolygon == 0) // A
			gb_movePolygon(pDoc->m_a, vx, vy);
		else gb_movePolygon(pDoc->m_b, vx, vy);
		break;
	case 5: // 剖分三角形。
		break;
	} // switch结束

	pDoc->m_basePoint = pg;
	Invalidate();
	pDoc->m_flagMouseDown = false;
}

void CCP_PolygonPlatformView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (!pDoc->m_flagSelect)
		return;
	if (!pDoc->m_flagMouseDown)
		return;
	CP_Point ps, pg;
	ps.m_x = point.x;
	ps.m_y = point.y;
	CRect r;
	GetClientRect(&r);
	gb_pointConvertFromScreenToGlobal(pg, ps,
		pDoc->m_scale, pDoc->m_translation, r.right, r.bottom);
	double vx = pg.m_x - pDoc->m_basePoint.m_x;
	double vy = pg.m_y - pDoc->m_basePoint.m_y;

	if (pDoc->m_flagMoveSame)
	{
		gb_movePointIntArray(pDoc->m_a, pDoc->m_flagSelectIDSetInA, vx, vy);
		gb_movePointIntArray(pDoc->m_b, pDoc->m_flagSelectIDSetInB, vx, vy);
		pDoc->m_basePoint = pg;
		Invalidate();
		return;
	} // if结束

	switch (pDoc->m_flagSelectType)
	{
	case 1: // 点。
		if (pDoc->m_flagSelectPolygon == 0) // A
			gb_movePoint(pDoc->m_a, pDoc->m_flagSelectID, vx, vy);
		else gb_movePoint(pDoc->m_b, pDoc->m_flagSelectID, vx, vy);
		break;
	case 2: // 环。
		if (pDoc->m_flagSelectPolygon == 0) // A
			gb_moveLoop(pDoc->m_a, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
		else gb_moveLoop(pDoc->m_b, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID, vx, vy);
		break;
	case 3: // 区域。
		if (pDoc->m_flagSelectPolygon == 0) // A
			gb_moveRegion(pDoc->m_a, pDoc->m_flagSelectRegion, vx, vy);
		else gb_moveRegion(pDoc->m_b, pDoc->m_flagSelectRegion, vx, vy);
		break;
	case 4: // 多边形。
		if (pDoc->m_flagSelectPolygon == 0) // A
			gb_movePolygon(pDoc->m_a, vx, vy);
		else gb_movePolygon(pDoc->m_b, vx, vy);
		break;
	case 5: // 剖分三角形。
		break;
	} // switch结束

	pDoc->m_basePoint = pg;
	Invalidate();
}

void CCP_PolygonPlatformView::OnNewRightInloop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	bool flagSuccess = false;
	bool flagA = true;
	CRect r;
	GetClientRect(&r);
	double dr = (r.right<r.bottom ? r.right : r.bottom);
	dr /= 4;
	int ir = pDoc->m_flagSelectRegion;
	if (!pDoc->m_flagSelect)
	{
		if (pDoc->m_flagBuildA)
			flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_a, pDoc->m_a.m_regionArray.size() - 1,
				pDoc->m_edgeNumber, dr, 0.0, 0.0);
		else
		{
			flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_b, pDoc->m_b.m_regionArray.size() - 1,
				pDoc->m_edgeNumber, dr, 0.0, 0.0);
			flagA = false;
		} // if/else结束
	}
	else
	{
		switch (pDoc->m_flagSelectType)
		{
		case 1: // 点。
		case 4: // 多边形。
			if (pDoc->m_flagSelectPolygon == 0)
				flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_a, pDoc->m_a.m_regionArray.size() - 1,
					pDoc->m_edgeNumber, dr, 0.0, 0.0);
			else
			{
				flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_b, pDoc->m_b.m_regionArray.size() - 1,
					pDoc->m_edgeNumber, dr, 0.0, 0.0);
				flagA = false;
			} // if/else结束
			break;
		case 2: // 环。
		case 3: // 区域。
			if (pDoc->m_flagSelectPolygon == 0)
				flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_a, pDoc->m_flagSelectRegion,
					pDoc->m_edgeNumber, dr, 0.0, 0.0);
			else
			{
				flagSuccess = gb_polygonNewInLoopRegular(pDoc->m_b, pDoc->m_flagSelectRegion,
					pDoc->m_edgeNumber, dr, 0.0, 0.0);
				flagA = false;
			} // if/else结束
			break;
		} // switch结束
	} // if/else结束
	Invalidate();

	char s[100];
	sprintf_s(s, 100, "新内环位于多边形%c中。", (flagA ? 'A' : 'B'));
	if (flagSuccess)
		mb_statusSetText("新内环创建成功。", s);
	else mb_statusSetText("没有创建新内环。", s);
}

void CCP_PolygonPlatformView::OnAddOutloop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagAdd != 0)
	{
		MessageBox("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。", "操作错误");
	} // if结束
	if (pDoc->m_flagSelect)
	{
		if (pDoc->m_flagSelectPolygon == 0)
			pDoc->m_flagAddIDPolygon = 0;
		else pDoc->m_flagAddIDPolygon = 1;
	}
	else
	{
		if (pDoc->m_flagBuildA)
			pDoc->m_flagAddIDPolygon = 0;
		else pDoc->m_flagAddIDPolygon = 1;
	} // if/else结束
	pDoc->m_flagAddPointArray.clear();
	pDoc->m_flagAdd = 1;
	mb_statusSetText("请按鼠标左键在工作区中添加新外环的点", "请用鼠标右键结束新外环添加操作");
}

void CCP_PolygonPlatformView::OnAddInloop()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagAdd != 0)
	{
		MessageBox("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。", "操作错误");
	} // if结束
	bool flagSuceess = false;
	CP_Polygon* pn = NULL;
	if (pDoc->m_flagSelect)
	{
		if (pDoc->m_flagSelectPolygon == 0)
		{
			pDoc->m_flagAddIDPolygon = 0;
			pn = &(pDoc->m_a);
		}
		else
		{
			pDoc->m_flagAddIDPolygon = 1;
			pn = &(pDoc->m_b);
		} // if/else结束
		switch (pDoc->m_flagSelectType)
		{
		case 1: // 点。
			flagSuceess = gb_findPointInLoop(*pn, pDoc->m_flagAddIDRegion,
				pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop,
				pDoc->m_flagSelectID);
			break;
		case 2: // 环。
			pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
			pDoc->m_flagAddIDLoop = pDoc->m_flagSelectID;
			flagSuceess = true;
			break;
		case 3: // 区域。
			pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
			pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size() - 1;
			flagSuceess = true;
			break;
		case 4: // 多边形。
			pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size() - 1;
			if (pDoc->m_flagAddIDRegion<0)
				break;
			pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size() - 1;
			if (pDoc->m_flagAddIDLoop<0)
				break;
			flagSuceess = true;
			break;
		} // switch结束
	}
	else
	{
		if (pDoc->m_flagBuildA)
		{
			pDoc->m_flagAddIDPolygon = 0;
			pn = &(pDoc->m_a);
		}
		else
		{
			pDoc->m_flagAddIDPolygon = 1;
			pn = &(pDoc->m_b);
		} // if/else结束

		pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size() - 1;
		if (pDoc->m_flagAddIDRegion >= 0)
		{
			pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size() - 1;
			if (pDoc->m_flagAddIDLoop >= 0)
			{
				flagSuceess = true;
			} // if(idL)结束
		} // if(idr)结束
	} // if/else结束
	if (flagSuceess)
	{
		pDoc->m_flagAddPointArray.clear();
		pDoc->m_flagAdd = 2;
		mb_statusSetText("请按鼠标左键在工作区中添加新内环的点", "请用鼠标右键结束新内环添加操作");
	}
	else
	{
		pDoc->m_flagAdd = 0;
		if (pDoc->m_flagAddIDPolygon == 0)
			mb_statusSetText("内环添加操作失败。", "多边形A还没有任何外环。");
		else
			mb_statusSetText("内环添加操作失败。", "请给多边形B增加外环，再执行本操作。");
	} // if/else结束
}

void CCP_PolygonPlatformView::OnAddPoint()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_flagAdd != 0)
	{
		MessageBox("前一个添加操作还没有完成，请继续前一个操作或按鼠标右键结束前一个添加操作。", "操作错误");
		return;
	} // if结束

	bool flagSuceess = false;
	CP_Polygon* pn = NULL;
	if (pDoc->m_flagSelect)
	{
		if (pDoc->m_flagSelectPolygon == 0)
		{
			pDoc->m_flagAddIDPolygon = 0;
			pn = &(pDoc->m_a);
		}
		else
		{
			pDoc->m_flagAddIDPolygon = 1;
			pn = &(pDoc->m_b);
		} // if/else结束

		switch (pDoc->m_flagSelectType)
		{
		case 1: // 点。
			flagSuceess = gb_findPointInLoop(*pn, pDoc->m_flagAddIDRegion,
				pDoc->m_flagAddIDLoop, pDoc->m_flagAddIDPointInLoop,
				pDoc->m_flagSelectID);
			break;
		case 2: // 环。
			pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
			pDoc->m_flagAddIDLoop = pDoc->m_flagSelectID;
			pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size() - 1;
			if (pDoc->m_flagAddIDPointInLoop<0)
				break;
			flagSuceess = true;
			break;
		case 3: // 区域。
			pDoc->m_flagAddIDRegion = pDoc->m_flagSelectRegion;
			pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size() - 1;
			pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size() - 1;
			if (pDoc->m_flagAddIDPointInLoop<0)
				break;
			flagSuceess = true;
			break;
		case 4: // 多边形。
			pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size() - 1;
			if (pDoc->m_flagAddIDRegion<0)
				break;
			pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size() - 1;
			if (pDoc->m_flagAddIDLoop<0)
				break;
			pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size() - 1;
			if (pDoc->m_flagAddIDPointInLoop<0)
				break;
			flagSuceess = true;
			break;
		} // switch结束
	}
	else
	{
		if (pDoc->m_flagBuildA)
		{
			pDoc->m_flagAddIDPolygon = 0;
			pn = &(pDoc->m_a);
		}
		else
		{
			pDoc->m_flagAddIDPolygon = 1;
			pn = &(pDoc->m_b);
		} // if/else结束

		pDoc->m_flagAddIDRegion = (*pn).m_regionArray.size() - 1;
		if (pDoc->m_flagAddIDRegion >= 0)
		{
			pDoc->m_flagAddIDLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray.size() - 1;
			if (pDoc->m_flagAddIDLoop >= 0)
			{
				pDoc->m_flagAddIDPointInLoop = (*pn).m_regionArray[pDoc->m_flagAddIDRegion].m_loopArray[pDoc->m_flagAddIDLoop].m_pointIDArray.size() - 1;
				if (pDoc->m_flagAddIDPointInLoop >= 0)
				{
					flagSuceess = true;
				} // if(idLv)结束
			} // if(idL)结束
		} // if(idr)结束
	} // if/else结束
	if (flagSuceess)
	{
		pDoc->m_flagAdd = 3;
		mb_statusSetText("用鼠标左键在工作区中添加点", "用鼠标右键结束点添加操作");
	}
	else
	{
		pDoc->m_flagAdd = 0;
		if (pDoc->m_flagAddIDPolygon == 0)
			mb_statusSetText("点添加操作失败。", "多边形A还没有任何外环。");
		else
			mb_statusSetText("点添加操作失败。", "请给多边形B增加外环，再执行本操作。");
	} // if/else结束
}

void CCP_PolygonPlatformView::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (!pDoc->m_flagSelect)
	{
		MessageBox("还没有拾取图形，无法删除。", "无效操作");
		return;
	} // if结束
	if (pDoc->m_flagAdd != 0)
	{
		MessageBox("添加图形的操作还没有结束，无法删除。", "无效操作");
		return;
	} // if结束
	CP_Polygon* pn;
	if (pDoc->m_flagSelectPolygon == 0)
		pn = &(pDoc->m_a);
	else pn = &(pDoc->m_b);
	switch (pDoc->m_flagSelectType)
	{
	case 1: // 点。
		gb_removePoint(*pn, pDoc->m_flagSelectID);
		break;
	case 2: // 环。
		gb_removeLoop(*pn, pDoc->m_flagSelectRegion, pDoc->m_flagSelectID);
		break;
	case 3: // 区域。
		gb_removeRegion(*pn, pDoc->m_flagSelectRegion);
		break;
	case 4: // 多边形。
		pn->mb_clear();
		break;
	} // switch结束
	pDoc->m_flagSelect = false;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateMoveSame(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pCmdUI->SetCheck(pDoc->m_flagMoveSame);
}

void CCP_PolygonPlatformView::OnMoveSame()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->m_flagMoveSame ^= true;
	if (!(pDoc->m_flagMoveSame))
	{
		pDoc->m_flagSelectIDSetInA.clear();
		pDoc->m_flagSelectIDSetInB.clear();
	} // if结束
}

void CCP_PolygonPlatformView::OnUpdateViewA(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pCmdUI->SetCheck(pDoc->m_flagShowA);
}

void CCP_PolygonPlatformView::OnViewA()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->m_flagShowA ^= true;

	if (!(pDoc->m_flagShowA))
		if (pDoc->m_flagSelectPolygon == 0)
			pDoc->m_flagSelect = false;

	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateViewB(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pCmdUI->SetCheck(pDoc->m_flagShowB);
}

void CCP_PolygonPlatformView::OnViewB()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->m_flagShowB ^= true;
	if (!(pDoc->m_flagShowB))
		if (pDoc->m_flagSelectPolygon != 0)
			pDoc->m_flagSelect = false;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateViewPointId(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pCmdUI->SetCheck(pDoc->m_flagShowPointID);
}

void CCP_PolygonPlatformView::OnViewPointId()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->m_flagShowPointID ^= true;
	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnViewTFace()
{
	// TODO: 在此添加命令处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->m_flagShowTriangleFace ^= true;

	if (pDoc->m_flagShowTriangleFace)
	{
		CP_Polygon *polygon = &pDoc->m_a;
		if(pDoc->m_flagBuildA == false)
			polygon = &pDoc->m_b;
		if (polygon->m_pointArray.size())
		{
			initTriagleMesh(&pDoc->m_triagleMesh, polygon);
			pDoc->m_plane.mb_clear();
			initialization(&pDoc->m_triagleMesh, &pDoc->m_plane);
			triangulation(&pDoc->m_triagleMesh, &pDoc->m_plane);
			finalisation(&pDoc->m_triagleMesh);
			insertEdgeCDT(&pDoc->m_triagleMesh);
		}
		else
		{
			MessageBox("当前选择的多边形不符合三角剖分的条件。", "操作错误");
		}
		polygon = NULL;
	}

	Invalidate(); // 刷新
}

void CCP_PolygonPlatformView::OnUpdateViewTFace(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pCmdUI->SetCheck(pDoc->m_flagShowTriangleFace);
}



void CCP_PolygonPlatformView::OnCheck()
{
	CCP_PolygonPlatformDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CP_Polygon *polygon = &pDoc->m_a;
	if (pDoc->m_flagBuildA == false)
		polygon = &pDoc->m_b;

	bool valid = true;
	for (int i = 0; i < polygon->m_regionArray.size() && valid; i++)
	{
		CP_Region &region = polygon->m_regionArray[i];
		// 检测自交
		for (int j = 0; j < region.m_loopArray.size() && valid; j++)
		{
			CP_Loop &loop = region.m_loopArray[j];
			valid = gb_testSelfIntersection(loop);
		}
		// 检测内环均在外环内部
		CP_Loop &loop0 = region.m_loopArray[0];
		for (int j = 1; j < region.m_loopArray.size() && valid; j++)
		{
			CP_Loop &loop1 = region.m_loopArray[j];
			valid = gb_testLoopInsideLoop(loop0, loop1);
			// 内环与内环不相交
			for (int k = j + 1; k < region.m_loopArray.size() && valid; k++)
			{
				CP_Loop &loop2 = region.m_loopArray[k];
				valid = gb_testLoopOutsideLoop(loop1, loop2);
			}
		}
		// 检测外环均在其他region的外环外部
		// 或者在其他region的内环内部
		for (int j = i + 1; j < polygon->m_regionArray.size() && valid; j++)
		{
			CP_Loop &loop1 = polygon->m_regionArray[j].m_loopArray[0];
			valid = gb_testLoopOutsideLoop(loop0, loop1);
			if (!valid)
			{
				for (int k = 1; k < region.m_loopArray.size() && !valid; k++)
				{
					CP_Loop &loop2 = region.m_loopArray[k];
					valid = gb_testLoopInsideLoop(loop2, loop1);
				}
			}
		}
		// 检测环的方向
		for (int j = 0; j < region.m_loopArray.size() && valid; j++)
		{
			CP_Loop &loop1 = region.m_loopArray[j];
			gb_adjustOrientation(loop1, j == 0);
		}
	}
	if (!valid)
	{
		MessageBox("当前多边形不合法。", "合法性检查错误");
	}
	else
	{
		mb_statusSetText("合法性检查正确。", "合法性检查正确");
	}
}

void gb_adjustOrientation(CP_Loop &loop, bool ccw)
{
	int size = loop.m_pointIDArray.size();
	int tmp = 0;
	for (int i = 0; i < size - 1; i++)
	{
		CP_Point &p1 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[i]];
		CP_Point &p2 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[(i + 1) % size]];
		tmp += -0.5 * (p2.m_y + p1.m_y) * (p2.m_x - p1.m_x);
	}
	bool valid;
	if (ccw = true)
	{
		valid = tmp > 0;
	}
	else
	{
		valid = tmp < 0;
	}
	if (!valid)
	{
		reverse(loop.m_pointIDArray.begin(), loop.m_pointIDArray.end());
	}
}

bool gb_testSelfIntersection(CP_Loop &loop)
{
	int size = loop.m_pointIDArray.size();
	for (int i = 0; i < size; i++)
	{
		CP_Point &p1 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[i]];
		for (int j = i + 1; j < size; j++)
		{
			CP_Point &p2 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[j]];
			if (p1.m_x == p2.m_x && p1.m_y == p2.m_y)
			{
				return false;
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		CP_Point &p1 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[i]];
		CP_Point &p2 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[(i + 1) % size]];
		CP_Point &p3 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[(i + 2) % size]];
		double det = (p2.m_x - p1.m_x) * (p3.m_y - p1.m_y) - (p2.m_y - p1.m_y) * (p3.m_x - p1.m_x);
		if (abs(det) < DBL_EPSILON)
		{
			if ((p2.m_x - p1.m_x) * (p3.m_x - p1.m_x) <= 0
				|| (p2.m_y - p1.m_y) * (p3.m_y - p1.m_y) <= 0)
			{
				return false;
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		CP_Point &p1 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[i]];
		CP_Point &p2 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[(i + 1) % size]];
		for (int j = i + 2; j < min(size, i + size - 1); j++)
		{
			CP_Point &p3 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[j]];
			CP_Point &p4 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[(j + 1) % size]];
			if (intersectSegmentSegment(p1.m_x, p2.m_x, p3.m_x, p4.m_x, p1.m_y, p2.m_y, p3.m_y, p4.m_y))
			{
				return false;
			}
		}
	}

	return true;
}

bool gb_testLoopOutsideLoop(CP_Loop &loop1, CP_Loop &loop2)
{
	int size1 = loop1.m_pointIDArray.size();
	int size2 = loop2.m_pointIDArray.size();
	vector<int> point_mark1(size1);
	vector<int> point_mark2(size2);
	//
	for (int i = 0; i < size1; i++)
	{
		CP_Point &point = loop1.m_polygon->m_pointArray[loop1.m_pointIDArray[i]];
		int tmp = gb_testPointInsideLoop(point, loop2);
		if (tmp > 0)
		{
			return false;
		}
		point_mark1[i] = tmp;
	}
	for (int i = 0; i < size1; i++)
	{
		if (point_mark1[i] + point_mark1[(i + 1) % size1] == 0)
		{
			return false;
		}
	}
	//
	for (int i = 0; i < size2; i++)
	{
		CP_Point &point = loop2.m_polygon->m_pointArray[loop2.m_pointIDArray[i]];
		int tmp = gb_testPointInsideLoop(point, loop1);
		if (tmp > 0)
		{
			return false;
		}
		point_mark2[i] = tmp;
	}
	for (int i = 0; i < size2; i++)
	{
		if (point_mark2[i] + point_mark2[(i + 1) % size2] == 0)
		{
			return false;
		}
	}

	return true;
}

bool gb_testLoopInsideLoop(CP_Loop &outside, CP_Loop &inside)
{
	int osize = outside.m_pointIDArray.size();
	int isize = inside.m_pointIDArray.size();
	vector<int> ipoint_mark(isize);
	vector<int> opoint_mark(osize);
	//
	for (int i = 0; i < isize; i++)
	{
		CP_Point &point = inside.m_polygon->m_pointArray[inside.m_pointIDArray[i]];
		int tmp = gb_testPointInsideLoop(point, outside);
		if (tmp < 0)
		{
			return false;
		}
		ipoint_mark[i] = tmp;
	}
	for (int i = 0; i < isize; i++)
	{
		if (ipoint_mark[i] + ipoint_mark[(i + 1) % isize] == 0)
		{
			return false;
		}
	}
	//
	for (int i = 0; i < osize; i++)
	{
		CP_Point &point = outside.m_polygon->m_pointArray[outside.m_pointIDArray[i]];
		int tmp = gb_testPointInsideLoop(point, inside);
		if (tmp > 0)
		{
			return false;
		}
		opoint_mark[i] = tmp;
	}
	for (int i = 0; i < osize; i++)
	{
		if (opoint_mark[i] + opoint_mark[(i + 1) % osize] == 0)
		{
			return false;
		}
	}

	return true;
}

int gb_testPointInsideLoop(CP_Point &point, CP_Loop &loop)
{
	int size = loop.m_pointIDArray.size();
	double x1 = point.m_x;
	double y1 = point.m_y;
	double x2 = x1 - 1000000;
	double y2 = y1 - 1000001;
	int intersect = 0;
	for (int i = 0; i < size; i++)
	{
		CP_Point &p1 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[i]];
		CP_Point &p2 = loop.m_polygon->m_pointArray[loop.m_pointIDArray[(i + 1) % size]];
		if (gb_testPointOnLine(x1, p1.m_x, p2.m_x, y1, p1.m_y, p2.m_y))
		{
			return 0;
		}
		if (intersectSegmentSegment(x1, x2, p1.m_x, p2.m_x, y1, y2, p1.m_y, p2.m_y))
		{
			intersect++;
		}
	}
	if (intersect % 2 == 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

bool gb_testPointOnLine(double x1, double x2, double x3, double y1, double y2, double y3)
{
	double det = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	if (abs(det) < DBL_EPSILON)
	{
		if ((x2 - x1) * (x3 - x1) <= 0
			|| (y1 - y1) * (y3 - y1) <= 0)
		{
			return true;
		}
	}
	return false;
}