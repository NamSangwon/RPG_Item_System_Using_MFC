#pragma once
#include "afxdialogex.h"

#include "Child/SelectCountDlg.h"
#include "Child/ItemBoxDlg.h"
#include "ItemInfoDlg.h"

// CTestInventory 대화 상자

class CInventoryDlg : public CDialogEx, public IObserver
{
	DECLARE_DYNAMIC(CInventoryDlg)

public:
	CInventoryDlg(std::shared_ptr<CItemInfoDlg>& item_info_dlg, std::shared_ptr<CInventory>& user_inventory, std::shared_ptr<CUserCharacter>& user_character, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CInventoryDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INVENTORY};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:	
	void SetInventoryIdx(int idx);
	int GetInventoryIdx() const;
	int GetSelectedNum(int max_range);
	int GetItemSlotIndex(CWnd* pWnd);

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();

private:
	void UpdateItem(int location_idx, int slot_idx) override;
	void UpdateBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK) override {}
	void UpdateAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage) override {}

	std::shared_ptr<CUserCharacter> m_pUserCharacter;
	std::shared_ptr<CInventory> m_pReferenceUserInventory;

	std::vector<std::shared_ptr<CItemBoxDlg>> m_vItemBox;
	std::shared_ptr<CItemInfoDlg> m_pItemInfoDlg;

	CImageList m_pImageDrag;
	int m_nSource;
	int m_iInven;
	bool m_bDragging;
};
