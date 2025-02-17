#pragma once
#include "afxdialogex.h"
#include "Dialog/Main/Child/InfoBoxDlg.h"


// CItemInfoDlg 대화 상자

class CItemInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CItemInfoDlg)

public:
	CItemInfoDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CItemInfoDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITEM_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	std::vector<std::shared_ptr<CInfoBoxDlg>> m_vInfoBox;
public:
	virtual BOOL OnInitDialog();
};
