#pragma once

#include "Player.h"

// CPlayersSettingsDialog dialog

class CPlayersSettingsDialog : public CDialogEx {
	DECLARE_DYNAMIC(CPlayersSettingsDialog)

	void addPlayerToList(const Player& player);
	int getSelectedItemIndex() const;
	CString getIconStatusText(const Player& player) const;
	void forcePlayerSelection();

	void initListGrid();
	void syncListWithPlayers();

public:
	CTypedPtrArray<CObArray, Player*>* PlayersPtr;

	int InitialSelectedPlayerIndex;

	CPlayersSettingsDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPlayersSettingsDialog();

	virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_PLAYERS_LIST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl PlayersListControl;

	int SelectedPlayerIndex;

	afx_msg void OnBnClickedAddPlayerBtn();
	afx_msg void OnBnClickedRemovePlayerFromListBtn();
	afx_msg void OnLvnItemchangedPlayersList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedEditPlayerBtn();
};
