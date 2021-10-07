// CPlayersSettingsDialog.cpp : implementation file
//

#include "pch.h"
#include "Dodger.h"
#include "CPlayersSettingsDialog.h"
#include "afxdialogex.h"

#include "CPlayerDetailsDialog.h"


// CPlayersSettingsDialog dialog

IMPLEMENT_DYNAMIC(CPlayersSettingsDialog, CDialogEx)

CPlayersSettingsDialog::CPlayersSettingsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_PLAYERS_LIST_DIALOG, pParent)
	, InitialSelectedPlayerIndex(-1), SelectedPlayerIndex(-1), PlayersPtr(nullptr)
{}

CPlayersSettingsDialog::~CPlayersSettingsDialog() {}

BOOL CPlayersSettingsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	this->initListGrid();

	this->syncListWithPlayers();

	this->forcePlayerSelection();

	BOOL setFocusOnDialog = this->InitialSelectedPlayerIndex == -1;// no focus on list is needed

	return setFocusOnDialog;
}

void CPlayersSettingsDialog::initListGrid() {
	// build basic grid style
	PlayersListControl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 150);
	PlayersListControl.InsertColumn(1, _T("Icon"), LVCFMT_LEFT, 100);
	PlayersListControl.InsertColumn(2, _T("Highest Score"), LVCFMT_LEFT, 100);
	PlayersListControl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

void CPlayersSettingsDialog::syncListWithPlayers() {
	// add players by input
	for (int i = 0; i < this->PlayersPtr->GetSize(); i++) {
		this->addPlayerToList(*this->PlayersPtr->ElementAt(i));
	}

	if (-1 < this->InitialSelectedPlayerIndex) {
		PlayersListControl.SetFocus();
		PlayersListControl.SetItemState(
			this->InitialSelectedPlayerIndex,
			LVIS_FOCUSED | LVIS_SELECTED,
			LVIS_FOCUSED | LVIS_SELECTED
		);
	}
}

CString CPlayersSettingsDialog::getIconStatusText(const Player& player) const {
	CString iconStatus = _T("Default");
	if (player.IsCustomIcon()) {
		iconStatus = _T("Custom");
	}
	return iconStatus;
}

void CPlayersSettingsDialog::addPlayerToList(const Player& player) {
	CString score;
	score.Format(_T("%llu"), player.GetHighestScore());

	int count = PlayersListControl.GetItemCount();
	PlayersListControl.InsertItem(count, player.GetName());
	PlayersListControl.SetItemText(count, 0, player.GetName());
	PlayersListControl.SetItemText(count, 1, this->getIconStatusText(player));
	PlayersListControl.SetItemText(count, 2, score);
}

void CPlayersSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_PLAYERS_LIST, PlayersListControl);
}


BEGIN_MESSAGE_MAP(CPlayersSettingsDialog, CDialogEx)
	ON_BN_CLICKED(ID_ADD_PLAYER_BTN, &CPlayersSettingsDialog::OnBnClickedAddPlayerBtn)
	ON_BN_CLICKED(ID_REMOVE_PLAYER_FROM_LIST_BTN, &CPlayersSettingsDialog::OnBnClickedRemovePlayerFromListBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, ID_PLAYERS_LIST, &CPlayersSettingsDialog::OnLvnItemchangedPlayersList)
	ON_BN_CLICKED(ID_EDIT_PLAYER_BTN, &CPlayersSettingsDialog::OnBnClickedEditPlayerBtn)
END_MESSAGE_MAP()


// CPlayersSettingsDialog message handlers

void CPlayersSettingsDialog::OnBnClickedAddPlayerBtn() {
	CPlayerDetailsDialog formDialog;

	formDialog.PlayersPtr = this->PlayersPtr;

	if (formDialog.DoModal() == IDOK) {
		// create player and add to collection
		Player* player;
		if (formDialog.LoadedCustomIcon) {
			player = new Player(formDialog.PlayerName, formDialog.IconFileName);
		}
		else {
			player = new Player(formDialog.PlayerName);
		}
		this->PlayersPtr->Add(player);
		this->addPlayerToList(*player);
	}
	else {
		// do nothing atm
	}
}

void CPlayersSettingsDialog::forcePlayerSelection() {
	PlayersListControl.SetFocus();

	CWnd* okBtn = GetDlgItem(IDOK);
	if (!okBtn) return;
	CWnd* editBtn = GetDlgItem(ID_EDIT_PLAYER_BTN);
	if (!editBtn) return;
	CWnd* removeBtn = GetDlgItem(ID_REMOVE_PLAYER_FROM_LIST_BTN);
	if (!removeBtn) return;

	BOOL playerSelected = getSelectedItemIndex() != -1;
	// disable if no selection
	BOOL EnableButtons = playerSelected;

	okBtn->EnableWindow(EnableButtons);
	editBtn->EnableWindow(EnableButtons);
	removeBtn->EnableWindow(EnableButtons);
}

int CPlayersSettingsDialog::getSelectedItemIndex() const {
	int count = PlayersListControl.GetItemCount();

	for (int i = 0; i <= count; i++) {
		if (PlayersListControl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) {
			return i;
		}
	}
	return -1;
}

void CPlayersSettingsDialog::OnBnClickedRemovePlayerFromListBtn() {
	int selectedIndex = getSelectedItemIndex();
	if (selectedIndex == -1) return;

	this->PlayersListControl.DeleteItem(selectedIndex);
	// handle actual data
	Player* player = this->PlayersPtr->ElementAt(selectedIndex);
	this->PlayersPtr->RemoveAt(selectedIndex);

	delete player;
}

void CPlayersSettingsDialog::OnLvnItemchangedPlayersList(NMHDR* pNMHDR, LRESULT* pResult) {
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	this->forcePlayerSelection();
	this->SelectedPlayerIndex = getSelectedItemIndex();
}

void CPlayersSettingsDialog::OnBnClickedEditPlayerBtn() {
	CPlayerDetailsDialog formDialog;

	formDialog.PlayersPtr = this->PlayersPtr;

	int selectedIndex = getSelectedItemIndex();
	if (selectedIndex == -1) return;
	Player* player = this->PlayersPtr->ElementAt(selectedIndex);

	formDialog.PlayerName = player->GetName();
	formDialog.ModifiedPlayerName = player->GetName();
	formDialog.IconFileName = player->GetIconFileName();

	formDialog.IsEdit = TRUE;

	if (formDialog.DoModal() == IDOK) {
		player->SetName(formDialog.PlayerName);
		if (formDialog.LoadedCustomIcon) {
			player->SetIconFileName(formDialog.IconFileName);
		}

		PlayersListControl.SetItemText(selectedIndex, 0, player->GetName());
		PlayersListControl.SetItemText(selectedIndex, 1, this->getIconStatusText(*player));
	}
	else {
		// do nothing atm
	}
}
