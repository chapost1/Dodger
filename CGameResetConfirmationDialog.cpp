// CGameResetConfirmationDialog.cpp : implementation file
//

#include "pch.h"
#include "Dodger.h"
#include "CGameResetConfirmationDialog.h"
#include "afxdialogex.h"


// CGameResetConfirmationDialog dialog

IMPLEMENT_DYNAMIC(CGameResetConfirmationDialog, CDialogEx)

CGameResetConfirmationDialog::CGameResetConfirmationDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_RESET_GAME_CONFIRMATION_DIALOG, pParent)
{

}

CGameResetConfirmationDialog::~CGameResetConfirmationDialog()
{
}

void CGameResetConfirmationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameResetConfirmationDialog, CDialogEx)
END_MESSAGE_MAP()


// CGameResetConfirmationDialog message handlers
