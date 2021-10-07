
// DodgerView.cpp : implementation of the CDodgerView class
//

#include "pch.h"
#include "framework.h"
#include "FlickerFreeDC.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Dodger.h"
#endif

#include "DodgerDoc.h"
#include "DodgerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDodgerView

IMPLEMENT_DYNCREATE(CDodgerView, CView)

BEGIN_MESSAGE_MAP(CDodgerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

UINT_PTR GAME_TIMER = 1;

// CDodgerView construction/destruction

CDodgerView::CDodgerView() noexcept
{
	// TODO: add construction code here

}

CDodgerView::~CDodgerView()
{
}

BOOL CDodgerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs



	return CView::PreCreateWindow(cs);
}

// CDodgerView drawing

void CDodgerView::OnDraw(CDC* pDC)
{
	FlickerFreeDC fDC{ pDC };

	// TODO: add draw code for native data here
	game.Draw(fDC);
}

// CDodgerView printing

BOOL CDodgerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDodgerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDodgerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CDodgerView diagnostics

#ifdef _DEBUG
void CDodgerView::AssertValid() const
{
	CView::AssertValid();
}

void CDodgerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDodgerDoc* CDodgerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDodgerDoc)));
	return (CDodgerDoc*)m_pDocument;
}
#endif //_DEBUG

// CDodgerView message handlers

BOOL CDodgerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return FALSE;
}


void CDodgerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == GAME_TIMER) {
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

		std::chrono::milliseconds int_ms =
			std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastGameUpdate);

		double timeElapsed = int_ms.count() / 1000.0;

		lastGameUpdate = currentTime;

		this->Update(timeElapsed);
	}

	CView::OnTimer(nIDEvent);
}

void CDodgerView::Update(const double& timeElapsed) {
	CDodgerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	game.SetPlayer(pDoc->SelectedPlayer);
	game.SetScoreToBeat(pDoc->GetHighestPlayersScore());

	game.Update(timeElapsed);

	Invalidate();

	UpdateWindow();
}

void CDodgerView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	game.OnKeyUp(nChar);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CDodgerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	game.OnKeyDown(nChar);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDodgerView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

	initGame();
}

void CDodgerView::initGame() {
	lastGameUpdate = std::chrono::high_resolution_clock::now();
	SetTimer(GAME_TIMER, 10, nullptr);
}

void CDodgerView::OnInitialUpdate() {
	CView::OnInitialUpdate();

	// TODO: Add your message handler code here
	CDodgerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect window;
	this->GetWindowRect(&window);
	game.SetPlaygroundSize(window);
	game.AddSubscriber(pDoc);	
}

void CDodgerView::ResetGame() {
	game.FactoryReset();
}
