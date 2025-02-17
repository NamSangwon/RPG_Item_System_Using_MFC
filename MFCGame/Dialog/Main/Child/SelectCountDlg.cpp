// SelectCountDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../../MFCGame.h"
#include "afxdialogex.h"
#include "SelectCountDlg.h"


// CSelectCountDlg 대화 상자

IMPLEMENT_DYNAMIC(CSelectCountDlg, CDialogEx)

CSelectCountDlg::CSelectCountDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECT_NUM, pParent)
{
    this->m_nSelected = U_NONE;
    this->m_nMax = U_NONE;

}

CSelectCountDlg::~CSelectCountDlg()
{
}

void CSelectCountDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_INPUT, m_cInput);
    DDX_Control(pDX, IDC_SLIDER, m_cSlider);
}


BEGIN_MESSAGE_MAP(CSelectCountDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CSelectCountDlg::OnBnClickedOk)
    ON_EN_CHANGE(IDC_INPUT, &CSelectCountDlg::OnEnChangeInput)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSelectCountDlg 메시지 처리기

int CSelectCountDlg::GetSelectedCnt() const
{
    return this->m_nSelected;
}

void CSelectCountDlg::SetMax(int max)
{
    this->m_nMax = max;
}

BOOL CSelectCountDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    
    // 입력 칸 설정
    m_cInput.SetDlgItemInt(IDC_INPUT, 1);

    m_cSlider.SetRange(1, m_nMax);
    m_cSlider.SetPos(1);
    m_cSlider.SetTicFreq(1);				// 눈금 간격 설정, 속성의 눈금과 자동 눈금이 True 설정 시
    m_cSlider.SetLineSize(1);				// 키보드 방향키로 움직였을 때의 간격 설정

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSelectCountDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    // 입력 받기
    m_nSelected = GetDlgItemInt(IDC_INPUT);

    CDialogEx::OnOK();
}


void CSelectCountDlg::OnEnChangeInput()
{
    // TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
    // CDialogEx::OnInitDialog() 함수를 재지정 
    //하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
    // 이 알림 메시지를 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
    
    // 입력 창에 값을 입력하면 해당 지점으로 스크롤 바 이동
    m_cSlider.SetPos(GetDlgItemInt(IDC_INPUT));
}


void CSelectCountDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;

    // 슬라이드할 때 마다 값 변경
    SetDlgItemInt(IDC_INPUT, pSlider->GetPos());
    UpdateData(FALSE);

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
