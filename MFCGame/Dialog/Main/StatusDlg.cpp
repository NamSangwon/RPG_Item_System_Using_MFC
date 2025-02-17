// UserStatusDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "StatusDlg.h"


// CStatusDlg 대화 상자

IMPLEMENT_DYNAMIC(CStatusDlg, CDialogEx)

CStatusDlg::CStatusDlg(int type, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STATUS, pParent)
{
	m_iStatusType = type;

	WNDCLASS wc = {};
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("Status_Window");
	AfxRegisterClass(&wc);
}

CStatusDlg::~CStatusDlg()
{
}

void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CStatusDlg::UpdateBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK)
{
	// Basic Status Update
	if (m_iStatusType == ID_BASIC_STATUS) {
		std::vector<std::string> basic_status_info = {
			std::format("{} / {}", HP, MaxHP),
			std::format("{} / {}", MP, MaxMP),
			std::to_string(STR),
			std::to_string(DEX),
			std::to_string(INT),
			std::to_string(LUK)
		};

		for (int i = 0; i < basic_status_info.size(); i++) 
			m_vStatusElements[i]->SetDlgItemText(IDC_STATIC_STATUS_VALUE, basic_status_info[i].c_str());
	}
}

void CStatusDlg::UpdateAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage)
{
	// Basic Status Update
	if (m_iStatusType == ID_ADVANCED_STATUS) {
		std::vector<std::string> advanced_status_info = {
			std::to_string(RegenerateHP),
			std::to_string(RegenerateMP),
			std::to_string(AttackPower),
			std::to_string(MagicPower),
			std::to_string(Defence),
			std::to_string(MoveSpeed),
			std::to_string(AttackSpeed),
			std::to_string(CriticalRate),
			std::to_string(CriticalDamage),
		};

		for (int i = 0; i < advanced_status_info.size(); i++)
			m_vStatusElements[i]->SetDlgItemText(IDC_STATIC_STATUS_VALUE, advanced_status_info[i].c_str());
	}
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialogEx)
END_MESSAGE_MAP()


// CStatusDlg 메시지 처리기



BOOL CStatusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	std::vector<CString> vStatusName;
	if (m_iStatusType == ID_BASIC_STATUS)
		vStatusName = {
			_T("HP"),
			_T("MP"),
			_T("STR"),
			_T("DEX"),
			_T("INT"),
			_T("LUK")
	};
	else if (m_iStatusType == ID_ADVANCED_STATUS)
		vStatusName = {
			_T("RegenerateHP"),
			_T("RegenerateMP"),
			_T("AttackPower"),
			_T("MagicPower"),
			_T("Defence"),
			_T("MoveSpeed"),
			_T("AttackSpeed"),
			_T("CriticalRate"),
			_T("CriticalDamage"),
	};


	// 스테이터스 타입에 맞춰 원소들을 생성
	for (int i = 0; i < vStatusName.size(); i++) {
		m_vStatusElements.push_back(std::make_shared<CStatusElementDlg>());
		m_vStatusElements[i]->Create(IDD_STATUS_ELEMENT, this);
	}

	CRect childWindowRC, childClientRC, thisRC;
	m_vStatusElements.front()->GetWindowRect(&childWindowRC);
	m_vStatusElements.front()->GetClientRect(&childClientRC);
	this->GetClientRect(&thisRC);

	int child_height_pad = childWindowRC.Width() - childClientRC.Width();

	this->MoveWindow(
		0,
		0,
		childWindowRC.Width(),
		childWindowRC.Height() * m_vStatusElements.size() + child_height_pad
	);

	for (int i = 0; i < m_vStatusElements.size(); i++) {
		m_vStatusElements[i]->MoveWindow(
			thisRC.left, 
			thisRC.top + i * childWindowRC.Height(),
			childWindowRC.Width(),
			childWindowRC.Height());

		m_vStatusElements[i]->ShowWindow(SW_SHOW);
		m_vStatusElements[i]->SetDlgItemText(IDC_STATIC_STATUS_NAME, vStatusName[i]);
	}

	this->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
