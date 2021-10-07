#pragma once


// CPlayerDetailsDialog dialog

#include "Player.h"

class CPlayerDetailsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPlayerDetailsDialog)

	BOOL isPlayerNameAlreadyExists(const CString& name) const;

public:
	CPlayerDetailsDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPlayerDetailsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_PLAYER_DETAILS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CTypedPtrArray<CObArray, Player*>* PlayersPtr;
	BOOL LoadedCustomIcon;
	BOOL IsEdit;
	CString ModifiedPlayerName;
	CString PlayerName;
	CString IconFileName;
	afx_msg void OnBnClickedLoadIconBtn();
	afx_msg void OnEnChangePlayerNameInputField();
	CEdit PlayerNameEditControl;
};
