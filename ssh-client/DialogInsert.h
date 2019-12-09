#pragma once


// DialogInsert dialog

class DialogInsert : public CDialogEx
{
	DECLARE_DYNAMIC(DialogInsert)

public:
	DialogInsert(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DialogInsert();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INSERT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonInsert();
};
