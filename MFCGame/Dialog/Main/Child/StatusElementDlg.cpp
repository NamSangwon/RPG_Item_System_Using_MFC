// StatusElementDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../../MFCGame.h"
#include "afxdialogex.h"
#include "StatusElementDlg.h"


// CStatusElementDlg 대화 상자

IMPLEMENT_DYNAMIC(CStatusElementDlg, CDialogEx)

CStatusElementDlg::CStatusElementDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STATUS_ELEMENT, pParent)
{

}

CStatusElementDlg::~CStatusElementDlg()
{
}

void CStatusElementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_STATUS_NAME, m_cStatusName);
	DDX_Control(pDX, IDC_STATIC_STATUS_VALUE, m_cStatusValue);
}


BEGIN_MESSAGE_MAP(CStatusElementDlg, CDialogEx)
END_MESSAGE_MAP()


// CStatusElementDlg 메시지 처리기
