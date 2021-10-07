#pragma once


// CGameResetConfirmationDialog dialog

class CGameResetConfirmationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CGameResetConfirmationDialog)

public:
	CGameResetConfirmationDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CGameResetConfirmationDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_RESET_GAME_CONFIRMATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
