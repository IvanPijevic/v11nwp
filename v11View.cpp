#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "v11.h"
#endif

#include "v11Doc.h"
#include "v11View.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cv11View

IMPLEMENT_DYNCREATE(Cv11View, CView)

BEGIN_MESSAGE_MAP(Cv11View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cv11View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SHAPE, &Cv11View::OnShape)
	ON_COMMAND(ID_COLOR, &Cv11View::OnColor)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, &Cv11View::OnHighlightRibbonListItem)
END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() :
	color(RGB(0, 0, 0)),
	shape(0),
	color_copy(RGB(0, 0, 0)),
	shape_copy(0)
{
	//rc.SetRect(150, 150, 500, 500);
}

Cv11View::~Cv11View()
{
}

BOOL Cv11View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cv11View drawing

void Cv11View::OnDraw(CDC* pDC)
{
	CPen pen(PS_SOLID, 2, color);
	CPen* pOldPen = pDC->SelectObject(&pen);

	switch (shape) 
	{
	case 0:
		pDC->Rectangle(rc);
		break;

	case 1:
		pDC->Ellipse(rc);
		break;

	case 2:
		pDC->RoundRect(rc, CPoint(5, 5));
		break;
	}

	pDC->SelectObject(pOldPen);
}

// Cv11View printing
void Cv11View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cv11View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cv11View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cv11View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cv11View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cv11View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRectTracker crt;
	if (crt.TrackRubberBand(this, point, true)) 
	{
		rc = crt.m_rect;
		Invalidate();
	};
}

void Cv11View::OnShape()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arr);

	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)arr.GetAt(0);
	shape = shape_copy = pGallery->GetSelectedItem();

	Invalidate();
}

void Cv11View::OnColor()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arr);

	CMFCRibbonColorButton* ribbon_color = (CMFCRibbonColorButton*)arr.GetAt(0);
	color = color_copy = ribbon_color->GetColor();

	Invalidate();
}

void Cv11View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM wParam, LPARAM lParam) 
{
	int index = (int)wParam;
	CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*)lParam;
	UINT id = pElem->GetID();

	if (id == ID_SHAPE) 
	{
		if (index == -1)
		{
			shape = shape_copy;
		}
		else 
		{
			shape = index;
		}

		Invalidate();
	}
	else if (id == ID_COLOR)
	{
		if (index == -1)
		{
			color = color_copy;
		}
		else 
		{
			CMFCRibbonColorButton* highlight_color = static_cast<CMFCRibbonColorButton*>(pElem);
			color = highlight_color->GetHighlightedColor();
		}

		Invalidate();
	}
	return 0;
}

// Cv11View diagnostics

#ifdef _DEBUG
void Cv11View::AssertValid() const
{
	CView::AssertValid();
}

void Cv11View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cv11Doc* Cv11View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cv11Doc)));
	return (Cv11Doc*)m_pDocument;
}
#endif //_DEBUG


// Cv11View message handlers

