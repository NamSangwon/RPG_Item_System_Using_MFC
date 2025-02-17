// InfoBoxDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../../MFCGame.h"
#include "afxdialogex.h"
#include "InfoBoxDlg.h"

// CInfoBoxDlg 대화 상자

IMPLEMENT_DYNAMIC(CInfoBoxDlg, CDialogEx)

CInfoBoxDlg::CInfoBoxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INFO_BOX, pParent)
{

}

CInfoBoxDlg::~CInfoBoxDlg()
{
}

void CInfoBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInfoBoxDlg, CDialogEx)
END_MESSAGE_MAP()

// CInfoBoxDlg 메시지 처리기

BOOL CInfoBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	this->GetWindowRect(&m_cItemInfoRC);

	SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
