// DialogInsert.cpp : implementation file
//

#include "stdafx.h"
#include "ssh-client.h"
#include "DialogInsert.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ssh-clientDoc.h"
#include "ssh-clientView.h"


// DialogInsert dialog

IMPLEMENT_DYNAMIC(DialogInsert, CDialogEx)

DialogInsert::DialogInsert(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INSERT, pParent)
{

}

DialogInsert::~DialogInsert()
{
}

void DialogInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogInsert, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &DialogInsert::OnClickedButtonInsert)
END_MESSAGE_MAP()


// DialogInsert message handlers


void DialogInsert::OnClickedButtonInsert()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CsshclientDoc* pDoc = (CsshclientDoc*)pFrame ->GetActiveDocument();
	CString cUsername,cIp,cKey;
	int iPort =0;
	SetDlgItemText(IDC_EDIT_KEY,cKey);
	SetDlgItemText(IDC_EDIT_USERNAME,cUsername);
	SetDlgItemText(IDC_IPADDRESS,cIp);
	SetDlgItemInt(IDC_EDIT_PORT,iPort);
	pDoc->m_IPort[count]=iPort;
	pDoc->m_strIp[count] = cIp;
	pDoc->m_strKey[count] = cKey;
	pDoc->m_strUsername[count] = cUsername;
	count++;
	SendMessage(WM_CLOSE,0,0);


}
