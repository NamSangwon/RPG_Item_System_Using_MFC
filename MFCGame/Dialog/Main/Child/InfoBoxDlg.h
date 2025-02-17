#pragma once
#include "afxdialogex.h"

// CInfoBoxDlg 대화 상자

class CInfoBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoBoxDlg)

public:
	CInfoBoxDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CInfoBoxDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFO_BOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog();

private:
	CRect m_cItemInfoRC;
};