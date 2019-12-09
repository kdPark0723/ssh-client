#pragma once


// CDialogTab dialog

class CDialogTab : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTab)

public:
	CDialogTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
