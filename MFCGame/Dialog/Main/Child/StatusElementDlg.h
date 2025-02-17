#pragma once
#include "afxdialogex.h"


// CStatusElementDlg 대화 상자

class CStatusElementDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusElementDlg)

public:
	CStatusElementDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStatusElementDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS_ELEMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CStatic m_cStatusName;
	CStatic m_cStatusValue;
};
