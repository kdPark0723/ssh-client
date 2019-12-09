// InputPassword.cpp: 구현 파일
//

#include "stdafx.h"
#include "ssh-client.h"
#include "InputPassword.h"
#include "afxdialogex.h"


// InputPassword 대화 상자

IMPLEMENT_DYNAMIC(InputPassword, CDialogEx)

InputPassword::InputPassword(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT_PASSWORD, pParent)
    , m_password(_T(""))
{

}

InputPassword::~InputPassword()
{
}

void InputPassword::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_PASSWORD_INPUT, m_password);
}


BEGIN_MESSAGE_MAP(InputPassword, CDialogEx)
    ON_BN_CLICKED(IDOK, &InputPassword::OnBnClickedOk)
END_MESSAGE_MAP()


// InputPassword 메시지 처리기


void InputPassword::OnBnClickedOk()
{
    UpdateData(TRUE);

    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CDialogEx::OnOK();
}
