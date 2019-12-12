
// ssh-clientView.cpp: CsshclientView 클래스의 구현
//

#include <cerrno>
#include <cstring>
#include <sstream>

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
#include "InputPassword.h"
#include "SSHException.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CsshclientView

IMPLEMENT_DYNCREATE(CsshclientView, CFormView)

BEGIN_MESSAGE_MAP(CsshclientView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_KEY, &CsshclientView::OnClickedInsertInfoButton)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SSH_TAB, &CsshclientView::OnSelchangeSshTab)
    ON_BN_CLICKED(IDC_CONNECT_SSH_BUTTON, &CsshclientView::OnBnClickedConnectSshButton)
    ON_BN_CLICKED(IDC_SSH_INPUT_BUTTON, &CsshclientView::OnBnClickedSshInputButton)
END_MESSAGE_MAP()

// CsshclientView 생성/소멸

CsshclientView::CsshclientView() noexcept
	: CFormView(IDD_SSHCLIENT_FORM),
    m_ssh_console_in(_T("")),
    m_ssh_console_out(_T("")),
    m_tab_sshInfos{},
    m_ssh_session{nullptr}
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CsshclientView::~CsshclientView()
{
    delete m_ssh_session;
}

void CsshclientView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SSH_TAB, m_ssh_tab);
    DDX_Control(pDX, IDC_SSH_INFO_LIST, m_ssh_info_list);
    DDX_Text(pDX, IDC_SSH_CONSOLE_OUTPUT, m_ssh_console_out);
    DDX_Text(pDX, IDC_SSH_CONSOLE_INPUT, m_ssh_console_in);
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

int CsshclientView::AddSshTab(const SSHInfo &info, const char *contents)
{
    std::stringstream stream;
    stream << info.user << "@" << info.host;

    auto tab_name{ stream.str() };
    auto tab_name_c{ tab_name.c_str() };
    auto tab_id = m_ssh_tab.GetItemCount();
	
    m_ssh_tab.InsertItem(tab_id, CString{ tab_name_c });

    m_tab_sshInfos.push_back(info);

    ChangeSshTab(tab_id);

    return tab_id;
}

int CsshclientView::ChangeSshTab(int tab_id)
{
    m_ssh_tab.SetCurSel(tab_id);

    try {
        if (!InitSshSecction())
            MessageBox(_T("접속 실패"), _T("실패 경고"), MB_ICONERROR);
    }
    catch (const SSHException &err) {
        MessageBox(CString{ err.what() }, _T("실패 경고"), MB_ICONERROR);
    }

    return 0;
}

void CsshclientView::ClearSshConsole()
{
    UpdateData(TRUE);

    m_ssh_console_out = _T("");
    m_ssh_console_in = _T("");

    UpdateData(FALSE);
}

bool CsshclientView::InitSshSecction()
{
    auto id{ m_ssh_tab.GetCurSel() };

    delete m_ssh_session;

    m_ssh_session = new SSHSession{ m_tab_sshInfos[id] };

	m_ssh_session->setSSHDirOption(".");

    m_ssh_session->connect();
    m_ssh_session->verifyKnownhost();

    InputPassword dlg;
    dlg.DoModal();
    auto password{ convertCstringToString(dlg.m_password) };

    m_ssh_session->userauthPassword(password);

    return true;
}

CsshclientDoc* CsshclientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CsshclientDoc)));
	return (CsshclientDoc*)m_pDocument;
}
#endif //_DEBUG


// CsshclientView 메시지 처리기


void CsshclientView::OnClickedInsertInfoButton()
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
        m_ssh_info_list.InsertItem(nCount, CString{ info.user.c_str() });

	UpdateData(FALSE);

    ClearSshConsole();
}

void CsshclientView::OnSelchangeSshTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int id = m_ssh_tab.GetCurSel();
	//내용 바꾸기
	
    try {
        ChangeSshTab(id);
    }
    catch (const SSHException &err) {
        MessageBox(CString{ err.what() }, _T("실패 경고"), MB_ICONERROR);
    }

	*pResult = 0;
}

void CsshclientView::OnBnClickedConnectSshButton()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CsshclientDoc* pDoc = (CsshclientDoc*)pFrame->GetActiveDocument();

    auto nCount{ m_ssh_info_list.GetItemCount() };

    for (auto i = 0; i < nCount; ++i)
        if (m_ssh_info_list.GetItemState(i, LVIS_SELECTED) != 0)
            AddSshTab(pDoc->m_ssh_infos[nCount - (i + 1)]);
}


void CsshclientView::OnBnClickedSshInputButton()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdateData(TRUE);

    try {
        SSHChanner channer{ *m_ssh_session };
        channer.open();

        m_ssh_console_out = CString{ channer.requestAndGetResult(convertCstringToString(m_ssh_console_in)).c_str() };
        m_ssh_console_in = _T("");
    }
    catch (const SSHException &err) {
        MessageBox(CString{ err.what() }, _T("실패 경고"), MB_ICONERROR);
    }

    UpdateData(FALSE);
}
