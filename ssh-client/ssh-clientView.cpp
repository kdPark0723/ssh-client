
// ssh-clientView.cpp: CsshclientView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ssh-client.h"
#endif
#include "MainFrm.h"
#include "ssh-clientDoc.h"
#include "ssh-clientView.h"
#include "DialogInsert.h"
#include "CDialogTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsshclientView

IMPLEMENT_DYNCREATE(CsshclientView, CFormView)

BEGIN_MESSAGE_MAP(CsshclientView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_KEY, &CsshclientView::OnClickedButtonKey)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SSH_TAB, &CsshclientView::OnSelchangeSshTab)
END_MESSAGE_MAP()

// CsshclientView 생성/소멸

CsshclientView::CsshclientView() noexcept
	: CFormView(IDD_SSHCLIENT_FORM),
    m_tab_sshInfos{},
    m_tab_contents{}
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CsshclientView::~CsshclientView()
{
}

void CsshclientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SSH_TAB, m_ssh_tab);
	DDX_Control(pDX, IDC_SSH_INFO_LIST, m_ssh_info_list);
}

BOOL CsshclientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CsshclientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	SetDlgItemText(IDC_STATIC,_T("aa"));
	//m_ssh_info_list.DeleteAllItems();
	//m_ssh_info_list.InsertColumn(0, _T("사용자 이름"), LVCFMT_CENTER);
	//m_ssh_info_list.SetExtendedStyle(m_ssh_info_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
}


// CsshclientView 진단

#ifdef _DEBUG
void CsshclientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CsshclientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

int CsshclientView::AddSshTab(const SshInfo &info, char *contents)
{
    std::stringstream stream;
    stream << info.ip << ":" << info.port;

    auto tab_name{ stream.str() };
    auto tab_name_c{ tab_name.c_str() };
    auto tab_id = m_ssh_tab.GetItemCount();

	
    m_ssh_tab.InsertItem(tab_id, CString{ tab_name_c });

    m_tab_sshInfos.push_back(info);
    m_tab_contents.push_back(std::stringstream{});

    return tab_id;
}

int CsshclientView::ChangeSshTab(int tab_id)
{
    return 0;
}

CsshclientDoc* CsshclientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CsshclientDoc)));
	return (CsshclientDoc*)m_pDocument;
}
#endif //_DEBUG


// CsshclientView 메시지 처리기


//void CsshclientView::OnClickedButtonInsert()
//{
//	// TODO: Add your control notification handler code here
//	DialogInsert dlg;
//	dlg.DoModal();
//
//}


void CsshclientView::OnClickedButtonKey()
{
	// TODO: Add your control notification handler code here
	DialogInsert dlg;
	dlg.DoModal();

}


void CsshclientView::UpdateButtons()
{
	// TODO: Add your implementation code here.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CsshclientDoc* pDoc = (CsshclientDoc*)pFrame ->GetActiveDocument();
	UpdateData(TRUE);
	m_ssh_info_list.DeleteAllItems();
	auto nCount{ m_ssh_info_list.GetItemCount() };

	for (auto info : pDoc->m_ssh_infos)
	{
		m_ssh_info_list.InsertItem(nCount, info.name);
	}

	m_ssh_tab.DeleteAllItems();
	int nTapCount{m_ssh_tab.GetItemCount()};	
	for (auto info : pDoc->m_ssh_infos)
	{
		m_ssh_tab.InsertItem(nTapCount, info.name);
	}

	UpdateData(FALSE);
}


void CsshclientView::OnSelchangeSshTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nSelection = m_ssh_tab.GetCurSel();
	//내용 바꾸기
	
	
	
	*pResult = 0;
}
