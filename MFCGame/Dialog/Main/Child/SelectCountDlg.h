#pragma once
#include "afxdialogex.h"

// CSelectCountDlg 대화 상자

class CSelectCountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectCountDlg)

public:
	CSelectCountDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSelectCountDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOW_MANY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	int m_nSelected;
	int m_nMax;

	CEdit m_cInput;
	CSliderCtrl m_cSlider;

	DECLARE_MESSAGE_MAP()
public:
	int GetSelectedCnt() const;
	void SetMax(int max);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeInput();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
