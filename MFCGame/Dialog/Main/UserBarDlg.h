#pragma once
#include "afxdialogex.h"

#include "../../Interface/Observer.h"
#include "../../Model/UserStatus.h"

// CUserBarDlg 대화 상자

class CUserBarDlg : public CDialogEx, public IObserver
{
	DECLARE_DYNAMIC(CUserBarDlg)

public:
	CUserBarDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUserBarDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_BAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void UpdateItem(int location_idx, int slot_idx) override {};
	void UpdateBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK) override;
	void UpdateAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage) override {}

	CProgressCtrl m_cHealthBar;
	CProgressCtrl m_cManaBar;
public:
	virtual BOOL OnInitDialog();
};
