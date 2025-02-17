// GachaControllerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "GachaControllerDlg.h"

#include "../../Character/UserCharacter.h"
#include "../../Manager/ItemGenerator.h"

// CGachaControllerDlg 대화 상자

IMPLEMENT_DYNAMIC(CGachaControllerDlg, CDialogEx)

CGachaControllerDlg::CGachaControllerDlg(std::shared_ptr<CItemInfoDlg>& item_info_dlg, std::shared_ptr<CUserCharacter>& user_character, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GACHA, pParent)
{
	// resource와 동일한 Class Name을 지정합니다.
	WNDCLASS wc = {};
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("Gacha_Window");
	AfxRegisterClass(&wc);

	m_pItemInfoDlg = item_info_dlg;
	m_pUserCharacter = user_character;
}

CGachaControllerDlg::~CGachaControllerDlg()
{
	
}

void CGachaControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGachaControllerDlg, CDialogEx)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_GET_ITEM, &CGachaControllerDlg::OnBnClickedGetItem)
	ON_BN_CLICKED(IDC_DISCARD_ITEM, &CGachaControllerDlg::OnBnClickedDiscardItem)
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGachaControllerDlg 메시지 처리기


BOOL CGachaControllerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 
	// 해당 다이얼로그 Rect 가져오기

	// 아이템 이미지 박스 생성
	IObserver* pObserver = nullptr;
	m_pItemBox = std::make_shared<CItemBoxDlg>();
	m_pItemBox->Create(IDD_ITEM_BOX, this);

	CRect rc, ItemBoxRC;
	this->GetClientRect(&rc);
	m_pItemBox->GetWindowRect(&ItemBoxRC);
	int item_slot_size = ItemBoxRC.Width();

	m_pItemBox->MoveWindow(
		(rc.CenterPoint().x - item_slot_size / 2),
		(rc.CenterPoint().y - item_slot_size / 2 - 15), // 10 == padding
		item_slot_size,
		item_slot_size
	);
	m_pItemBox->SetItemSlotIndex(0);
	m_pItemBox->ShowWindow(SW_SHOW);

	// 배경색 흰 색으로 초기화
	this->SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CGachaControllerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 가챠 아이템 칸을 더블 클릭 시, 
	// 아이템 가챠 이벤트 실행
	CWnd* pChildWnd = CWnd::ChildWindowFromPoint(point);
	if (pChildWnd->GetSafeHwnd() == m_pItemBox->GetSafeHwnd() && m_pGeneratedItem == nullptr) { // 뽑은 아이템을 처리(Get or Discard)를 한 후인지 확인 여부
		m_pGeneratedItem = CItemGenerator::GetInstance().GenerateRandomItem();

		UpdateItemSlot();
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CGachaControllerDlg::OnBnClickedGetItem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pGeneratedItem != nullptr) {
		m_pUserCharacter->AddItem(m_pGeneratedItem);
		m_pGeneratedItem = nullptr;

		UpdateItemSlot();
	}
}


void CGachaControllerDlg::OnBnClickedDiscardItem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pGeneratedItem != nullptr) {
		m_pGeneratedItem.reset();
		m_pGeneratedItem = nullptr;

		UpdateItemSlot();
	}
}

void CGachaControllerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// Hover Item Info
	CWnd* pChildWnd = CWnd::ChildWindowFromPoint(point);

	if (pChildWnd != nullptr) {
		ClientToScreen(&point);

		// 아이템 슬롯 범위 설정
		CRect rc;
		m_pItemBox->GetWindowRect(&rc);
		rc.SetRect(rc.left + ITEM_SLOT_IN_PADDING, rc.top + ITEM_SLOT_IN_PADDING, rc.right - ITEM_SLOT_IN_PADDING, rc.bottom - ITEM_SLOT_IN_PADDING);

		// 아이템 슬롯 범위 내에 마우스를 호버링
		if (rc.PtInRect(point) && m_pGeneratedItem != nullptr) {
			// Show Item Info
			// 아이템 정보 구하기
			std::vector<std::string> vItemInfo;

			// 아이템 기본 정보 추가
			vItemInfo.push_back(m_pGeneratedItem->ShowInfo());

			if (m_pGeneratedItem->IsEquipment()) {
				std::shared_ptr<CNSW_Equipment> pEquipment = std::dynamic_pointer_cast<CNSW_Equipment>(m_pGeneratedItem);

				// 추가 능력치 정보 추가
				if (pEquipment->GetAdditionalData() != nullptr)
					vItemInfo.push_back(pEquipment->GetAdditionalData()->ShowInfo());

				// 특수 능력치 정보 추가
				if (pEquipment->GetSpecialData() != nullptr)
					vItemInfo.push_back(pEquipment->GetSpecialData()->ShowInfo());
			}

			// Show Item Info
			m_pItemInfoDlg->SetItemInfo(point, vItemInfo);
			m_pItemInfoDlg->DrawItemRankEffect(m_pGeneratedItem->GetItemRank());
			m_pItemInfoDlg->ShowWindow(SW_SHOW);
		}
		else {
			// Hide Item Info
			m_pItemInfoDlg->ShowWindow(SW_HIDE);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CGachaControllerDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pGeneratedItem == nullptr) {
		// Hide Item Info
		m_pItemInfoDlg->ShowWindow(SW_HIDE);
		CDialogEx::OnClose();
	}
}

void CGachaControllerDlg::UpdateItemSlot()
{
	CString file_path = GACHA_BOX_IMAGE;
	int item_rank = U_NONE;

	if (m_pGeneratedItem != nullptr) {
		file_path = m_pGeneratedItem->GetFilePath().c_str();
		item_rank = m_pGeneratedItem->GetItemRank();
	}

	m_pItemBox->SetItemImage(file_path);
	m_pItemBox->DrawItemImage();

	m_pItemBox->SetItemRankEffect(item_rank);
	m_pItemBox->DrawItemRankEffect();
}
