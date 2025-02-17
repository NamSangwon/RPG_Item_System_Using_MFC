// Equipment.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "EquipmentDlg.h"

#include "../../Character/UserCharacter.h"

// CNSW_Equipment 대화 상자

IMPLEMENT_DYNAMIC(CEquipmentDlg, CDialogEx)

CEquipmentDlg::CEquipmentDlg(std::shared_ptr<CItemInfoDlg>& item_info_dlg, std::shared_ptr<CEquipment>& user_equipment, std::shared_ptr<CUserCharacter>& user_character, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EQUIPMENT, pParent)
{
	// resource와 동일한 Class Name을 지정합니다.
	WNDCLASS wc = {};
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("Equipment_Window");
	AfxRegisterClass(&wc);

	for (int i = 0; i < EQUIPMENT_PARTS_NUM; i++) {
		m_vItemBox.emplace_back(std::make_shared<CItemBoxDlg>());
		m_vItemBox[i]->SetItemSlotIndex(i);
	}

	m_pItemInfoDlg = item_info_dlg;
	m_pReferenceUserEquipment = user_equipment;
	m_pUserCharacter = user_character;

	m_nSource = U_NONE;
	m_bDragging = false;
}

CEquipmentDlg::~CEquipmentDlg()
{
	for (int i = 0; i < m_vItemBox.size(); i++) {
		m_vItemBox[i]->DestroyWindow();
	}
}

void CEquipmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CEquipmentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CNSW_Equipment 메시지 처리기


BOOL CEquipmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 장비 아이템의 장착 여부를 보여주는 아이템 슬롯 생성
	// 아이템 슬롯 보이기
	IObserver* pObserver = nullptr;
	for (int i = 0; i < m_vItemBox.size(); i++) {
		m_vItemBox[i]->Create(IDD_ITEM_BOX, this);
		m_vItemBox[i]->SetItemSlotIndex(i);
	}

	// 아이템 슬롯 위치 및 크기 변경
	MoveItemSlot();

	SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CEquipmentDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE){
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CEquipmentDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CWnd* pChildWnd = CWnd::ChildWindowFromPoint(point); 

	if (pChildWnd != nullptr) {
		// 클릭한 장비 칸의 인덱스 구하기
		m_nSource = GetItemSlotIndex(pChildWnd);
	
		if (m_nSource >= 0 && m_nSource < EQUIPMENT_PARTS_NUM && m_pReferenceUserEquipment->IsExist(m_nSource)) {
			// CBitmap으로 이미지 로드 
			CImage item_image;
			item_image.Load(m_pReferenceUserEquipment->GetItem(m_nSource)->GetFilePath().c_str());
			CBitmap cBitMap;
			cBitMap.Attach(item_image.Detach());

			// 이미지 리스트 초기화 
			m_bDragging = true;
			m_pImageDrag.Create(32, 32, ILC_COLOR32 | ILC_MASK, 5, 0);

			// 드래그용 이미지 추가
			m_pImageDrag.Add(&cBitMap, RGB(0, 0, 0));

			// 드래그용 이미지 지정
			ClientToScreen(&point); // 스크린 좌표로 변경
			m_pImageDrag.SetDragCursorImage(0, point);
			m_pImageDrag.SetBkColor(RGB(255, 255, 255));

			SetCapture(); //마우스 캡쳐, 다른창에 넘어가도 문제없게 하기위해서 사용

			CRect rc;
			m_vItemBox[m_nSource]->GetWindowRect(&rc);

			m_pImageDrag.BeginDrag(0, CPoint(rc.Width() / 2, rc.Height() / 2));
			m_pImageDrag.DragEnter(NULL, point);  // 드래그 상태로 변경하며, 반투명한 드래그 이미지를 출력.

			cBitMap.Detach();
		}
	}

	// Hide Item Info
	m_pItemInfoDlg->ShowWindow(SW_HIDE);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CEquipmentDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDragging) // 드래그하고 있다면 (== 드래그를 위한 이미지 리스트에 값이 존재하면)
	{
		ClientToScreen(&point); // 스크린 좌표 기반 마우스 좌표 반환

		// 모든 창에 드래그 상태를 유지해라.
		m_pImageDrag.DragEnter(NULL, point);
		m_pImageDrag.DragMove(point);
	}

	// Hover Item Info
	CWnd* pChildWnd = CWnd::ChildWindowFromPoint(point);
	int iHoveredSlot = U_NONE;
	bool bHovering = false;

	if (pChildWnd != nullptr) {
		ClientToScreen(&point);

		// 마우스를 호버링 중인 아이템 슬롯 찾기		
		iHoveredSlot = GetItemSlotIndex(pChildWnd);

		if (iHoveredSlot >= 0 && iHoveredSlot < EQUIPMENT_PARTS_NUM) {
			// 아이템 슬롯 범위 설정
			CRect rc;
			m_vItemBox[iHoveredSlot]->GetWindowRect(&rc);
			rc.SetRect(rc.left + 10, rc.top + 10, rc.right - 10, rc.bottom - 10);

			// 아이템 슬롯 범위 내에 마우스를 호버링하고 있고, 해당 아이템 슬롯에 아이템이 존재하면,
			if (rc.PtInRect(point) && m_pReferenceUserEquipment->IsExist(iHoveredSlot)) {
				// Show Item Info
				// 아이템 정보 구하기
				std::shared_ptr<CNSW_Item> pHoveredItem = m_pReferenceUserEquipment->GetItem(iHoveredSlot);
				std::vector<std::string> vItemInfo;

				// 아이템 기본 정보 추가
				vItemInfo.push_back(pHoveredItem->ShowInfo());

				if (pHoveredItem->IsEquipment()) {
					std::shared_ptr<CNSW_Equipment> pEquipment = std::dynamic_pointer_cast<CNSW_Equipment>(pHoveredItem);

					// 추가 능력치 정보 추가
					if (pEquipment->GetAdditionalData() != nullptr)
						vItemInfo.push_back(pEquipment->GetAdditionalData()->ShowInfo());

					// 특수 능력치 정보 추가
					if (pEquipment->GetSpecialData() != nullptr)
						vItemInfo.push_back(pEquipment->GetSpecialData()->ShowInfo());
				}

				// Show Item Info
				m_pItemInfoDlg->SetItemInfo(point, vItemInfo);
				m_pItemInfoDlg->DrawItemRankEffect(pHoveredItem->GetItemRank());
				m_pItemInfoDlg->ShowWindow(SW_SHOW);
			}
			else {
				// Hide Item Info
				m_pItemInfoDlg->ShowWindow(SW_HIDE);
			}
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CEquipmentDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDragging) // 드래그하고 있다면 (== 드래그를 위한 이미지 리스트에 값이 존재하면)
	{
		// 스크린 좌표 기반 마우스 좌표 반환 후 드래그 끝내기
		ClientToScreen(&point);
		m_pImageDrag.DragLeave(NULL);
		m_pImageDrag.EndDrag();
		ReleaseCapture();

		// 드래그를 마친 창의 리스트에 아이템추가하기
		CWnd* pWnd = CWnd::WindowFromPoint(point); // 드래그를 마친 창

		if (m_nSource >= 0) {
			if (m_pReferenceUserEquipment->IsExist(m_nSource)) {
				// 메인 창에 드랍 시
				if (pWnd->GetSafeHwnd() == ::FindWindow(_T("Main_Window"), NULL)) {
					if (MessageBox(_T("해당 아이템을 버리시겠습니까?"), NULL, MB_OKCANCEL) == IDOK) {
						m_pUserCharacter->DiscardItem(m_nSource);
					}
				}
				// 인벤토리 창에 드랍 시
				else {
					// 5개의 인벤토리 중 해당하는 인벤토리 찾기
					for (int i = ID_INVENTORY; i < INVENTORY_WINDOW_NUM; i++) {
						CString WndText;
						WndText.Format(_T("Inventory_%d"), i + 1);

						if (pWnd->GetSafeHwnd() == ::FindWindow(_T("Inventory_Window"), WndText)) {
							pWnd->ScreenToClient(&point);
							CWnd* pChildWnd = pWnd->ChildWindowFromPoint(point);

							// 인벤토리 내에 아이템을 드랍한 칸에 아이템 전송
							if (pChildWnd != nullptr) {
								int location_idx = i;
								int inven_slot = GetItemSlotIndex(pChildWnd);

								CItemBoxDlg* pItemBox = dynamic_cast<CItemBoxDlg*>(pChildWnd);
								if (pItemBox != nullptr) {
									m_pUserCharacter->UnequipItem(location_idx, inven_slot, m_nSource);
								}
							}

							break;
						}
					}
				}
			}
		}
		
		m_pImageDrag.DeleteImageList();
		m_bDragging = false;
		m_nSource = U_NONE;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CEquipmentDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// Hide Item Info
	m_pItemInfoDlg->ShowWindow(SW_HIDE);

	CDialogEx::OnClose();
}

void CEquipmentDlg::UpdateItem(int location_idx, int slot_idx)
{
	if (location_idx == ID_EQUIPMENT) {
		std::shared_ptr<CNSW_Item> pItem = m_pReferenceUserEquipment->GetItem(slot_idx);

		CString file_path;
		int item_rank = U_NONE;

		if (pItem != nullptr) {
			file_path = pItem->GetFilePath().c_str();
			item_rank = pItem->GetItemRank();
		}

		m_vItemBox[slot_idx]->SetItemImage(file_path);
		m_vItemBox[slot_idx]->DrawItemImage();

		m_vItemBox[slot_idx]->SetItemRankEffect(item_rank);
		m_vItemBox[slot_idx]->DrawItemRankEffect();
	}
}

void CEquipmentDlg::MoveItemSlot()
{
	enum EquipmentType { Head, Chest, Pants, Gloves, Boots, Weapon };

	CRect thisRC, itemBoxRC;
	this->GetClientRect(&thisRC);
	m_vItemBox.front()->GetWindowRect(&itemBoxRC);

	int item_slot_size = itemBoxRC.Width();

	m_vItemBox[EquipmentType::Head]->MoveWindow(
		(thisRC.CenterPoint().x - item_slot_size / 2),
		(thisRC.CenterPoint().y - item_slot_size / 2) - EQUIPMENT_PARTS_OUT_PADDING * 3,
		item_slot_size,
		item_slot_size
	);

	m_vItemBox[EquipmentType::Chest]->MoveWindow(
		(thisRC.CenterPoint().x - item_slot_size / 2),
		(thisRC.CenterPoint().y - item_slot_size / 2) - EQUIPMENT_PARTS_OUT_PADDING,
		item_slot_size,
		item_slot_size
	);

	m_vItemBox[EquipmentType::Pants]->MoveWindow(
		(thisRC.CenterPoint().x - item_slot_size / 2),
		(thisRC.CenterPoint().y - item_slot_size / 2) + EQUIPMENT_PARTS_OUT_PADDING,
		item_slot_size,
		item_slot_size
	);

	m_vItemBox[EquipmentType::Gloves]->MoveWindow(
		(thisRC.CenterPoint().x - item_slot_size / 2) + EQUIPMENT_PARTS_OUT_PADDING * 2,
		(thisRC.CenterPoint().y - item_slot_size / 2),
		item_slot_size,
		item_slot_size
	);

	m_vItemBox[EquipmentType::Boots]->MoveWindow(
		(thisRC.CenterPoint().x - item_slot_size / 2),
		(thisRC.CenterPoint().y - item_slot_size / 2) + EQUIPMENT_PARTS_OUT_PADDING * 3,
		item_slot_size,
		item_slot_size
	);

	m_vItemBox[EquipmentType::Weapon]->MoveWindow(
		(thisRC.CenterPoint().x - item_slot_size / 2) - EQUIPMENT_PARTS_OUT_PADDING * 2,
		(thisRC.CenterPoint().y - item_slot_size / 2),
		item_slot_size,
		item_slot_size
	);
}

int CEquipmentDlg::GetItemSlotIndex(CWnd* pWnd)
{
	CItemBoxDlg* pItemBox = dynamic_cast<CItemBoxDlg*>(pWnd);

	if (pItemBox != nullptr)
		return pItemBox->GetItemSlotIndex();

	return U_NONE;
}