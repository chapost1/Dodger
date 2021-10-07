// CPlayerDetailsDialog.cpp : implementation file
//

#include "pch.h"
#include "Dodger.h"
#include "CPlayerDetailsDialog.h"
#include "afxdialogex.h"

#include <windows.h>
#include <chrono>
#include <shlwapi.h>

// CPlayerDetailsDialog dialog

IMPLEMENT_DYNAMIC(CPlayerDetailsDialog, CDialogEx)

CPlayerDetailsDialog::CPlayerDetailsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_PLAYER_DETAILS_DIALOG, pParent)
	, PlayerName(_T("")), IconFileName(_T("")), LoadedCustomIcon(FALSE), PlayersPtr(nullptr)
	, IsEdit(FALSE), ModifiedPlayerName(_T(""))
{
}

CPlayerDetailsDialog::~CPlayerDetailsDialog() {}

BOOL CPlayerDetailsDialog::OnInitDialog() {
	CDialogEx::OnInitDialog();

	CWnd* applyBtn = GetDlgItem(IDOK);
	if (applyBtn && this->PlayerName.GetLength() < 1) {
		applyBtn->EnableWindow(FALSE);
	}

	return TRUE;
}

void CPlayerDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ID_PLAYER_NAME_INPUT_FIELD, PlayerName);
	DDX_Control(pDX, ID_PLAYER_NAME_INPUT_FIELD, PlayerNameEditControl);
}


BEGIN_MESSAGE_MAP(CPlayerDetailsDialog, CDialogEx)
	ON_BN_CLICKED(ID_LOAD_ICON_BTN, &CPlayerDetailsDialog::OnBnClickedLoadIconBtn)
	ON_EN_CHANGE(ID_PLAYER_NAME_INPUT_FIELD, &CPlayerDetailsDialog::OnEnChangePlayerNameInputField)
END_MESSAGE_MAP()


// CPlayerDetailsDialog message handlers

void copyFile(CString sourcePath, CString destPath) {
	CopyFile(sourcePath, destPath, true);
}

const long long getNow() {
	const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

void CPlayerDetailsDialog::OnBnClickedLoadIconBtn() {
	CFileDialog dlg(TRUE, _T(".png"), NULL, 0, _T("Images (*.png)|*.*||"));
	dlg.m_ofn.lpstrInitialDir = _T("./assets/example icons");
	CString fileName;
	if (dlg.DoModal() == IDOK) {
		fileName = dlg.GetPathName();

		CString fileExtension = PathFindExtension(fileName);

		if (fileExtension != _T(".png")) {
			AfxMessageBox(_T("Invalid File Extension!\nOnly .png Files Are Allowed"));
			return;
		}

		const long long unix = getNow();

		CString newFileName;
		newFileName.Format(_T("%lld.png"), unix);

		CString newFilePath = _T("./assets/icons/") + newFileName;

		copyFile(fileName, newFilePath);

		this->IconFileName = newFileName;
		this->LoadedCustomIcon = TRUE;
	}
}

void CPlayerDetailsDialog::OnEnChangePlayerNameInputField() {
	this->PlayerNameEditControl.GetWindowTextW(this->PlayerName);

	CWnd* applyBtn = GetDlgItem(IDOK);
	if (!applyBtn) return;
	CWnd* label = GetDlgItem(ID_PLAYER_NAME_INPUT_ERROR_MESSAGE);
	if (!label) return;

	// validate input and indicate user on screen
	if (this->PlayerName.GetLength() < 1) {
		applyBtn->EnableWindow(FALSE);
	}
	else if (15 < this->PlayerName.GetLength()) {
		label->SetWindowText(_T("Name Max Length Is 15"));
		applyBtn->EnableWindow(FALSE);
	} else if (this->isPlayerNameAlreadyExists(this->PlayerName)) {
		label->SetWindowText(_T("Name Is Already Taken"));
		applyBtn->EnableWindow(FALSE);
	}
	else {
		label->SetWindowText(_T(""));
		applyBtn->EnableWindow(TRUE);
	}
}

BOOL CPlayerDetailsDialog::isPlayerNameAlreadyExists(const CString& name) const {
	for (int i = 0; i < this->PlayersPtr->GetSize(); i++) {
		if (
			this->PlayersPtr->ElementAt(i)->GetName() == name && 
			!(this->IsEdit && this->ModifiedPlayerName == name)
		) {
			return TRUE;
		}
	}
	return FALSE;
}
