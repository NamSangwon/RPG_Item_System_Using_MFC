#pragma once
#include "afxdialogex.h"


// CRegisterDlg 대화 상자

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRegisterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CStatic m_cRegisterStatic;
	CEdit m_cInputID;
	CEdit m_cInputPW;
	CEdit m_cInputName;
	CComboBox m_cGenderComboBox;
public:
	afx_msg void OnBnClickedRegisterBtn();
};
