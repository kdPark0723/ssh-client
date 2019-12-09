
// ssh-clientView.h: CsshclientView 클래스의 인터페이스
//

#pragma once

#include "SshInfo.h"

class CsshclientView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CsshclientView() noexcept;
	DECLARE_DYNCREATE(CsshclientView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_SSHCLIENT_FORM };
#endif

// 특성입니다.
public:
	CsshclientDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CsshclientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
    int AddSshTab(const SshInfo &info, char *contents = "");

    int ChangeSshTab(int tab_id);

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
    CTabCtrl m_ssh_tab;
};

#ifndef _DEBUG  // ssh-clientView.cpp의 디버그 버전
inline CsshclientDoc* CsshclientView::GetDocument() const
   { return reinterpret_cast<CsshclientDoc*>(m_pDocument); }
#endif

