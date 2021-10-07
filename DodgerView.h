
// DodgerView.h : interface of the CDodgerView class
//

#pragma once

#include <chrono>

#include "Game.h"

class CDodgerView : public CView
{
	Game game;
	std::chrono::high_resolution_clock::time_point lastGameUpdate;

	void initGame();


protected: // create from serialization only
	CDodgerView() noexcept;
	DECLARE_DYNCREATE(CDodgerView)

// Attributes
public:
	CDodgerDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDodgerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnInitialUpdate();

	void Update(const double& timeElapsed);

	void ResetGame();
};

#ifndef _DEBUG  // debug version in DodgerView.cpp
inline CDodgerDoc* CDodgerView::GetDocument() const
   { return reinterpret_cast<CDodgerDoc*>(m_pDocument); }
#endif

