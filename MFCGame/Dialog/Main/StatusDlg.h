#pragma once
#include "afxdialogex.h"
#include "../../Interface/Observer.h"
#include "../../Model/Status/BasicStatus.h"
#include "Child/StatusElementDlg.h"


// CStatusDlg 대화 상자

class CStatusDlg : public CDialogEx, public IObserver
{
	DECLARE_DYNAMIC(CStatusDlg)

public:
	CStatusDlg(int type, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStatusDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS };
#endif

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void UpdateItem(int location_idx, int slot_idx) override {}
	void UpdateBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK) override;
	void UpdateAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage) override;

	DECLARE_MESSAGE_MAP()

private:
	int m_iStatusType;
	std::vector<std::shared_ptr<CStatusElementDlg>> m_vStatusElements;
};
