#pragma once


// InputPassword 대화 상자

class InputPassword : public CDialogEx
{
	DECLARE_DYNAMIC(InputPassword)

public:
	InputPassword(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~InputPassword();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT_PASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CString m_password;
};
