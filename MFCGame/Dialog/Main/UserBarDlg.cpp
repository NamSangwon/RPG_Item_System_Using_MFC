// UserBarDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "UserBarDlg.h"


// CUserBarDlg 대화 상자

IMPLEMENT_DYNAMIC(CUserBarDlg, CDialogEx)

CUserBarDlg::CUserBarDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USER_BAR, pParent)
{

}

CUserBarDlg::~CUserBarDlg()
{
}

void CUserBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEALTH_BAR, m_cHealthBar);
	DDX_Control(pDX, IDC_MANA_BAR, m_cManaBar);
}

void CUserBarDlg::UpdateBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK)
{
	m_cHealthBar.SetRange(0, MaxHP);
	m_cManaBar.SetRange(0, MaxMP);

	m_cHealthBar.SetPos(HP);
	m_cManaBar.SetPos(MP);
}


BEGIN_MESSAGE_MAP(CUserBarDlg, CDialogEx)
END_MESSAGE_MAP()


// CUserBarDlg 메시지 처리기


BOOL CUserBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	this->SetBackgroundColor(RGB(200, 200, 200));

	m_cHealthBar.SetBarColor(RGB(255, 0, 0));
	m_cManaBar.SetBarColor(RGB(0, 0, 255));

	this->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
