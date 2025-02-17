#pragma once
#include "afxdialogex.h"


// CDeleteUserDlg 대화 상자

class CDeleteUserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteUserDlg)

public:
	CDeleteUserDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDeleteUserDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELETE_USER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_cDeleteStatic;
	CEdit m_cInputID;
	CEdit m_cInputPW;
public:
	afx_msg void OnBnClickedDeleteBtn();
	virtual BOOL OnInitDialog();
};
