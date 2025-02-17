#pragma once
#include "afxdialogex.h"

#include "ItemInfoDlg.h"
#include "Child/ItemBoxDlg.h"

// CGachaControllerDlg 대화 상자

class CGachaControllerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGachaControllerDlg)

public:
	CGachaControllerDlg(std::shared_ptr<CItemInfoDlg>& item_info_dlg, std::shared_ptr<CUserCharacter>& user_character, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	~CGachaControllerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GACHA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedGetItem();
	afx_msg void OnBnClickedDiscardItem();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClose();

private:
	void UpdateItemSlot();

	std::shared_ptr<CUserCharacter> m_pUserCharacter;

	std::shared_ptr<CItemBoxDlg> m_pItemBox;
	std::shared_ptr<CItemInfoDlg> m_pItemInfoDlg;

	std::shared_ptr<CNSW_Item> m_pGeneratedItem;
};
