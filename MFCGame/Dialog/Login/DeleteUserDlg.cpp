// DeleteUserDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "DeleteUserDlg.h"

#include "../../Manager/DatabaseManager.h"

// CDeleteUserDlg 대화 상자

IMPLEMENT_DYNAMIC(CDeleteUserDlg, CDialogEx)

CDeleteUserDlg::CDeleteUserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELETE_USER, pParent)
{

}

CDeleteUserDlg::~CDeleteUserDlg()
{
}

void CDeleteUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELETE_USER_STATIC, m_cDeleteStatic);
	DDX_Control(pDX, IDC_DELETE_ID, m_cInputID);
	DDX_Control(pDX, IDC_DELETE_PW, m_cInputPW);
}


BEGIN_MESSAGE_MAP(CDeleteUserDlg, CDialogEx)
	ON_BN_CLICKED(IDOK2, &CDeleteUserDlg::OnBnClickedDeleteBtn)
END_MESSAGE_MAP()


// CDeleteUserDlg 메시지 처리기


BOOL CDeleteUserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString cText;
	m_cDeleteStatic.GetDlgItemText(IDC_REGISTER_STATIC, cText);

	CFont cFont;
	cFont.CreatePointFont(20, cText);

	m_cDeleteStatic.SetFont(&cFont);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDeleteUserDlg::OnBnClickedDeleteBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 회원 탈퇴를 위한 입력 값 받아 오기
	CString inputID, inputPW;

	GetDlgItemText(IDC_DELETE_ID, inputID);
	GetDlgItemText(IDC_DELETE_PW, inputPW);

	if (inputID.IsEmpty() || inputPW.IsEmpty()) return;

	// DB 연결을 통한 입력 값 기반 회원 탈퇴
	if (CDatabaseManager::GetInstance().DeleteUser(std::string(CT2CA(inputID)), std::string(CT2CA(inputPW))) == EXIT_SUCCESS)
		CDialogEx::OnOK();
}
