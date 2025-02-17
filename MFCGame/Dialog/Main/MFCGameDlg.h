
// MFCApplicationDlg.h: 헤더 파일
#include "InventoryDlg.h"
#include "EquipmentDlg.h"
#include "GachaControllerDlg.h"
#include "Child/InfoBoxDlg.h"
#include "StatusDlg.h"
#include "UserBarDlg.h"
#include "ItemInfoDlg.h"

#include "../Login/LoginDlg.h"
#include "../Login/CharacterSelectDlg.h"

#include "../../Character/UserCharacter.h"
//

#pragma once

// CMFCApplicationDlg 대화 상자
class CMFCGameDlg : public CDialogEx
{
	// 생성입니다.
public:
	CMFCGameDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CMFCGameDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	HICON m_hIcon;

	BOOL QuitProgram();

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	void ResizeStatusDialog();

	std::shared_ptr<CUserCharacter> m_pUserCharacter;

	std::shared_ptr<CItemInfoDlg> m_pItemInfoDlg;

	std::vector<std::shared_ptr<CInventoryDlg>> m_vInventoryDlg;
	std::shared_ptr<CEquipmentDlg> m_pEquipmentDlg;
	std::shared_ptr<CGachaControllerDlg> m_pGachaControllerDlg;

	std::shared_ptr<CStatusDlg> m_pBasicStatusDlg;
	std::shared_ptr<CStatusDlg> m_pAdvancedStatusDlg;
	std::shared_ptr<CUserBarDlg> m_pUserBarDlg;

	std::shared_ptr<CLoginDlg> m_pLoginDlg;
	std::shared_ptr<CCharacterSelectDlg> m_pCharacterSelectDlg;

public:
	afx_msg void OnBnClickedRecoverHp();
	afx_msg void OnBnClickedReduceHp();
	afx_msg void OnBnClickedRecoverMp();
	afx_msg void OnBnClickedReduceMp();
	afx_msg void OnBnClickedGetExp();
};
