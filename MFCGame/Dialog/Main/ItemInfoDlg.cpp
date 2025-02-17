// ItemInfoDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "ItemInfoDlg.h"


// CItemInfoDlg 대화 상자

IMPLEMENT_DYNAMIC(CItemInfoDlg, CDialogEx)

CItemInfoDlg::CItemInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ITEM_INFO, pParent)
{
	WNDCLASS wc = {};
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("ItemInfo_Window");
	AfxRegisterClass(&wc);
}

CItemInfoDlg::~CItemInfoDlg()
{
}

void CItemInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CItemInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CItemInfoDlg 메시지 처리기



BOOL CItemInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 아이템 정보 박스 생성
	for (int i = 0; i < 3; i++) {
		m_vInfoBox.push_back(std::make_shared<CInfoBoxDlg>());
		m_vInfoBox[i]->Create(IDD_INFO_BOX, this);
	}

	CRect thisRC, childRC;
	m_vInfoBox.front()->GetWindowRect(&childRC);
	this->GetClientRect(&thisRC);

	// 아이템 정보 박스 위치 조정
	this->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, childRC.Width() * 3, childRC.Height(), NULL);

	for (int i = 0; i < 3; i++) {
		m_vInfoBox[i]->MoveWindow(
			thisRC.left + childRC.Width() * i,
			thisRC.top,
			childRC.Width(), 
			childRC.Height()
		);
		m_vInfoBox[i]->ShowWindow(SW_SHOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CItemInfoDlg::SetItemInfo(CPoint point, std::vector<std::string> vItemInfo)
{
	// 아이템 정보 작성 및 출력
	CRect rc;
	for (int i = 0; i < vItemInfo.size(); i++) {

		// 아이템 정보 출력 위치 구하기
		m_vInfoBox[i]->GetWindowRect(&rc);
		CRect cRc(0, 12, rc.Width(), rc.Height());

		m_vInfoBox[i]->GetDC()->DrawText(vItemInfo[i].c_str(), &cRc, DT_EXPANDTABS | DT_CENTER | DT_NOPREFIX | DT_WORD_ELLIPSIS | DT_WORDBREAK);
	}

	this->SetWindowPos(&CWnd::wndNoTopMost, point.x + 20, point.y + 20, rc.Width() * vItemInfo.size(), rc.Height(), NULL);
}

void CItemInfoDlg::DrawItemRankEffect(int rank)
{
	COLORREF color = GetItemRankColor(rank);

	// CDC 및 Rect 가져오기
	CRect rc;
	this->GetClientRect(&rc);
	CDC* dc = this->GetDC();

	// 효과의 외곽선을 칠 할 CPen 설정
	CPen pen;
	pen.CreatePen(PS_SOLID, 8, color);
	dc->SelectObject(&pen);

	// 효과의 내부를 칠 할 CBrush 설정 (투명하도록 설정)
	CBrush brush;
	brush.CreateStockObject(NULL_BRUSH);
	dc->SelectObject(&brush);

	// 아이템 등급 효과 출력
	dc->Rectangle(rc);
}