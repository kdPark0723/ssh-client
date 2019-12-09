// CDialogTab.cpp : implementation file
//

#include "stdafx.h"
#include "ssh-client.h"
#include "CDialogTab.h"
#include "afxdialogex.h"


// CDialogTab dialog

IMPLEMENT_DYNAMIC(CDialogTab, CDialogEx)

CDialogTab::CDialogTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB, pParent)
{

}

CDialogTab::~CDialogTab()
{
}

void CDialogTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogTab, CDialogEx)
END_MESSAGE_MAP()


// CDialogTab message handlers
