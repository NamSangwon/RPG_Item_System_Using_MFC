// RegisterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "RegisterDlg.h"

#include "../../Manager/DatabaseManager.h"

// CRegisterDlg 대화 상자

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTER, pParent)
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REGISTER_STATIC, m_cRegisterStatic);
	DDX_Control(pDX, IDC_REGISTER_ID, m_cInputID);
	DDX_Control(pDX, IDC_REGISTER_PW, m_cInputPW);
	DDX_Control(pDX, IDC_REGISTER_NAME, m_cInputName);
	DDX_Control(pDX, IDC_REGISTER_GENDER, m_cGenderComboBox);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegisterDlg::OnBnClickedRegisterBtn)
END_MESSAGE_MAP()


// CRegisterDlg 메시지 처리기


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 해당 다이얼로그 타이틀
	CString cText;
	m_cRegisterStatic.GetDlgItemText(IDC_REGISTER_STATIC, cText);
	
	CFont cFont;
	cFont.CreatePointFont(20, cText);

	m_cRegisterStatic.SetFont(&cFont);

	// 성별 선택 콤보 박스
	m_cGenderComboBox.AddString(_T("Male"));
	m_cGenderComboBox.AddString(_T("Female"));

	if (m_cGenderComboBox.GetCount() > 0)
		m_cGenderComboBox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRegisterDlg::OnBnClickedRegisterBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 회원 가입을 위한 입력 값 받아 오기
	CString inputID, inputPW, inputName;

	GetDlgItemText(IDC_REGISTER_ID, inputID);
	GetDlgItemText(IDC_REGISTER_PW, inputPW);
	GetDlgItemText(IDC_REGISTER_NAME, inputName);

	int inputGender = m_cGenderComboBox.GetCurSel();

	if (inputID.IsEmpty() || inputPW.IsEmpty() || inputName.IsEmpty() || inputGender == CB_ERR) return;

	// DB 연결을 통한 입력 값 기반 회원 가입
	if (CDatabaseManager::GetInstance().Register(std::string(CT2CA(inputID)), std::string(CT2CA(inputPW)), std::string(CT2CA(inputName)), inputGender) == EXIT_SUCCESS)
		CDialogEx::OnOK();
}
