
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


std::string convertCstringToString(const CString &cstring) {
    CT2CA pszConvertedAnsiString{ cstring };

    return std::string{ pszConvertedAnsiString };
}

int verify_knownhost(ssh_session session)
{
    enum ssh_known_hosts_e state;
    unsigned char *hash = NULL;
    ssh_key srv_pubkey = NULL;
    size_t hlen;
    char buf[10];
    char *hexa;
    char *p;
    int cmp;
    int rc;
    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if (rc < 0) {
        return -1;
    }
    rc = ssh_get_publickey_hash(srv_pubkey,
        SSH_PUBLICKEY_HASH_SHA1,
        &hash,
        &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0) {
        return -1;
    }
    state = ssh_session_is_known_server(session);
    switch (state) {
    case SSH_KNOWN_HOSTS_OK:
        /* OK */
        break;
    case SSH_KNOWN_HOSTS_CHANGED:
        ssh_clean_pubkey_hash(&hash);
        return -1;
    case SSH_KNOWN_HOSTS_OTHER:
        ssh_clean_pubkey_hash(&hash);
        return -1;
    case SSH_KNOWN_HOSTS_NOT_FOUND:
    case SSH_KNOWN_HOSTS_UNKNOWN:
        hexa = ssh_get_hexa(hash, hlen);
        ssh_string_free_char(hexa);
        ssh_clean_pubkey_hash(&hash);
        p = fgets(buf, sizeof(buf), stdin);
        if (p == NULL) {
            return -1;
        }
        cmp = std::strncmp(buf, "yes", 3);
        if (cmp != 0) {
            return -1;
        }
        rc = ssh_session_update_known_hosts(session);
        if (rc < 0) {
            return -1;
        }
        break;
    case SSH_KNOWN_HOSTS_ERROR:
        ssh_clean_pubkey_hash(&hash);
        return -1;
    }
    ssh_clean_pubkey_hash(&hash);
    return 0;
}

CString RunCommand(ssh_session session, const CString &command)
{
    ssh_channel channel;
    int rc;
    char buffer[256];
    int nbytes;
    channel = ssh_channel_new(session);
    if (channel == NULL)
        throw _T("channel == NULL");

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK)
    {
        ssh_channel_free(channel);
        throw _T("rc != SSH_OK");
    }
    rc = ssh_channel_request_exec(channel, CT2CA{ command });
    if (rc != SSH_OK)
    {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        throw _T("rc != SSH_OK");
    }

    std::stringstream ss{};
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0)
    {
        for (auto i = 0; i < nbytes; ++i) 
            ss << buffer[i];
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }
    if (nbytes < 0)
    {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        throw _T("nbytes < 0");
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return CString{ ss.str().c_str() };
}

// CsshclientView 생성/소멸

CsshclientView::CsshclientView() noexcept
	: CFormView(IDD_SSHCLIENT_FORM),
    m_ssh_console_in(_T("")),
    m_ssh_console_out(_T("")),
    m_tab_sshInfos{},
    m_ssh_session{}
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CsshclientView::~CsshclientView()
{
    if (m_ssh_session != NULL)
        ssh_free(m_ssh_session);
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

int CsshclientView::AddSshTab(const SshInfo &info, const char *contents)
{
    std::stringstream stream;
    stream << convertCstringToString(info.name) << "@" << convertCstringToString(info.ip);

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
    catch (const CString &err) {
        MessageBox(err, _T("실패 경고"), MB_ICONERROR);
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

    if (m_ssh_session != NULL) {
        ssh_disconnect(m_ssh_session);
        ssh_free(m_ssh_session);
    }
  
    ssh_session m_ssh_session = ssh_new();
    if (m_ssh_session == NULL)
        return false;

    ssh_options_set(m_ssh_session, SSH_OPTIONS_HOST, CT2CA{ m_tab_sshInfos[id].ip });

    if (!m_tab_sshInfos[id].name.IsEmpty())
        ssh_options_set(m_ssh_session, SSH_OPTIONS_USER, CT2CA{ m_tab_sshInfos[id].name });

    int rc{ ssh_connect(m_ssh_session) };
    if (rc != SSH_OK) {
        auto err{ CString(ssh_get_error(m_ssh_session)) };

        ssh_disconnect(m_ssh_session);
        ssh_free(m_ssh_session);
        m_ssh_session = NULL;

        throw err;
    }

    if (verify_knownhost(m_ssh_session) < 0)
    {
        ssh_disconnect(m_ssh_session);
        ssh_free(m_ssh_session);
        
        return false;
    }

    InputPassword dlg;
    dlg.DoModal();

    rc = ssh_userauth_password(m_ssh_session, NULL, CT2CA{ dlg.m_password });
    if (rc != SSH_AUTH_SUCCESS)
    {
        auto err{ CString(ssh_get_error(m_ssh_session)) };

        ssh_disconnect(m_ssh_session);
        ssh_free(m_ssh_session);
        m_ssh_session = NULL;

        throw err;
    }

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
		m_ssh_info_list.InsertItem(nCount, info.name);

	UpdateData(FALSE);

    ClearSshConsole();
}


void CsshclientView::OnSelchangeSshTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nSelection = m_ssh_tab.GetCurSel();
	//내용 바꾸기
	
    ClearSshConsole();

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

    m_ssh_console_out = RunCommand(m_ssh_session, m_ssh_console_in);
    m_ssh_console_in = _T("");

    UpdateData(FALSE);
}
