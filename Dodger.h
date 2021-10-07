
// Dodger.h : main header file for the Dodger application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDodgerApp:
// See Dodger.cpp for the implementation of this class
//

class CDodgerApp : public CWinApp
{
public:
	CDodgerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDodgerApp theApp;
