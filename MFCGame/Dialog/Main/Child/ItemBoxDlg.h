#pragma once
#include "afxdialogex.h"

#include "../../../Model/Item/NSW_Item.h"
#include "../ItemInfoDlg.h"

// CNSW_ItemBox 대화 상자

class CItemBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CItemBoxDlg)

public:
	CItemBoxDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CItemBoxDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITEM_BOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void SetItemSlotIndex(int idx);
	int GetItemSlotIndex();

	void SetItemImage(CString file_path);
	void DrawItemImage();

	void SetItemRankEffect(int item_rank);
	void DrawItemRankEffect();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

private:
	CImage m_cImage;
	COLORREF m_cRankEffectColor;

	int m_nIndex;
};
