// ItemBox.cpp: 구현 파일
//

#include "pch.h"
#include "../../../MFCGame.h"
#include "afxdialogex.h"
#include "ItemBoxDlg.h"

#include "../InventoryDlg.h"

// CNSW_ItemBox 대화 상자

IMPLEMENT_DYNAMIC(CItemBoxDlg, CDialogEx)

CItemBoxDlg::CItemBoxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ITEM_BOX, pParent)
{
	m_nIndex = U_NONE;
	m_cRankEffectColor = COMMON_COLOR;
}

CItemBoxDlg::~CItemBoxDlg()
{

}

void CItemBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CItemBoxDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CItemBoxDlg::SetItemSlotIndex(int idx)
{
	this->m_nIndex = idx;
}

int CItemBoxDlg::GetItemSlotIndex()
{
	return this->m_nIndex;
}

void CItemBoxDlg::SetItemImage(CString file_path) 
{
	m_cImage.Destroy();
	m_cImage.Load(file_path);
}

void CItemBoxDlg::DrawItemImage()
{
	CClientDC dc(this);
	CRect rc;
	this->GetClientRect(&rc);

	dc.FillSolidRect(rc, dc.GetBkColor());

	if (!m_cImage.IsNull()) {
		m_cImage.AlphaBlend(
			dc.GetSafeHdc(),
			rc.CenterPoint().x - m_cImage.GetWidth() / 2,
			rc.CenterPoint().y - m_cImage.GetHeight() / 2
		);
	}
}

void CItemBoxDlg::SetItemRankEffect(int item_rank)
{
	m_cRankEffectColor = GetItemRankColor(item_rank);
}

void CItemBoxDlg::DrawItemRankEffect()
{
	// CDC 및 Rect 가져오기
	CRect rc;
	this->GetClientRect(&rc);
	CDC* dc = this->GetDC();

	// 효과의 외곽선을 칠 할 CPen 설정
	CPen pen;
	pen.CreatePen(PS_SOLID, 8, m_cRankEffectColor);
	dc->SelectObject(&pen);

	// 효과의 내부를 칠 할 CBrush 설정 (투명하도록 설정)
	CBrush brush;
	brush.CreateStockObject(NULL_BRUSH);
	dc->SelectObject(&brush);

	// 아이템 등급 효과 출력
	dc->Rectangle(rc);
}

// CNSW_ItemBox 메시지 처리기

BOOL CItemBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CItemBoxDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CWnd* pWnd = this->GetParent();

	if (pWnd->GetSafeHwnd() == ::FindWindow(_T("Gacha_Window"), NULL)) {
		SetItemImage(GACHA_BOX_IMAGE);
		DrawItemImage();
	}
	else {
		DrawItemImage();
		DrawItemRankEffect();
	}

	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}