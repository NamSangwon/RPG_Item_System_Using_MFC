// LoginDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "LoginDlg.h"

#include "../../Manager/DatabaseManager.h"

// CLoginDlg 대화 상자

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(std::shared_ptr<CUserCharacter>& user_character, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

	m_pUserCharacter = user_character;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGIN_ID, m_cInputID);
	DDX_Control(pDX, IDC_LOGIN_PW, m_cInputPW);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CLoginDlg::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CLoginDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CLoginDlg 메시지 처리기


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//m_cInputID.ShowBalloonTip(_T("ID"), _T("User Identifier"), TTI_INFO);
	//m_cInputID.ShowBalloonTip(_T("PW"), _T("User Password"), TTI_INFO);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLoginDlg::OnBnClickedButtonLogin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString inputID, inputPW;

	GetDlgItemText(IDC_LOGIN_ID, inputID);
	GetDlgItemText(IDC_LOGIN_PW, inputPW);

	if (inputID.IsEmpty() || inputPW.IsEmpty()) return;

	// 유저의 캐릭터들의 이름을 저장
	std::vector<std::string> vCharacters;

	// DB 연결을 통한 아이디 및 비밀번호 조회로 로그인
	if ((CDatabaseManager::GetInstance().Login(std::string(CT2CA(inputID)), std::string(CT2CA(inputPW)), vCharacters) == EXIT_SUCCESS)) {

		// 캐릭터 선택 창 출력
		m_pCharacterSelectDlg = std::make_shared<CCharacterSelectDlg>(inputID, m_pUserCharacter, vCharacters);
		
		if (m_pCharacterSelectDlg->DoModal() == IDOK)
			CDialogEx::OnOK();
		else
			CDialogEx::OnCancel();
	}
}


void CLoginDlg::OnBnClickedButtonRegister()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRegisterDlg m_cRegisterDlg;
	m_cRegisterDlg.DoModal();
}


void CLoginDlg::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDeleteUserDlg m_cDeleteUserDlg;
	m_cDeleteUserDlg.DoModal();
}