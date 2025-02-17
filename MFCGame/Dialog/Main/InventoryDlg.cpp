// TestInventory.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "InventoryDlg.h"

// CTestInventory 대화 상자

IMPLEMENT_DYNAMIC(CInventoryDlg, CDialogEx)

CInventoryDlg::CInventoryDlg(std::shared_ptr<CItemInfoDlg>& item_info_dlg, std::shared_ptr<CInventory>& user_inventory, std::shared_ptr<CUserCharacter>& user_character, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INVENTORY, pParent)
{
	// resource와 동일한 Class Name을 지정합니다.
	WNDCLASS wc = {};
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("Inventory_Window");
	AfxRegisterClass(&wc);

	// 아이템 슬롯 초기화
	for (int i = 0; i < INVENTORY_COL_NUM * INVENTORY_ROW_NUM; i++) {
		m_vItemBox.emplace_back(std::make_shared<CItemBoxDlg>());
		m_vItemBox[i]->SetItemSlotIndex(i);
	}
	
	m_pItemInfoDlg = item_info_dlg;
	m_pReferenceUserInventory = user_inventory;
	m_pUserCharacter = user_character;

	// 드레그용 변수 초기화
	m_nSource = U_NONE;

	// 인벤토리 다이얼로그 인덱스
	m_iInven = U_NONE;

	// 드래깅 유무 변수 초기화
	m_bDragging = false;
}

CInventoryDlg::~CInventoryDlg()
{
	for (int i = 0; i < m_vItemBox.size(); i++) {
		m_vItemBox[i]->DestroyWindow();
	}
}

void CInventoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInventoryDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


void CInventoryDlg::SetInventoryIdx(int idx)
{
	this->m_iInven = idx;
}

int CInventoryDlg::GetInventoryIdx() const
{
	return this->m_iInven;
}

int CInventoryDlg::GetSelectedNum(int max_range)
{
	CSelectCountDlg cMsgBox;

	cMsgBox.SetMax(max_range);
	if (cMsgBox.DoModal() == IDOK)
		return cMsgBox.GetSelectedCnt();
	else
		return U_NONE;
}

int CInventoryDlg::GetItemSlotIndex(CWnd* pWnd)
{
	CItemBoxDlg* pItemBox = dynamic_cast<CItemBoxDlg*>(pWnd);

	if (pItemBox != nullptr)
		return pItemBox->GetItemSlotIndex();

	return U_NONE;
}


// CTestInventory 메시지 처리기



BOOL CInventoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	IObserver* pObserver = nullptr;
	for (int i = 0; i < m_vItemBox.size(); i++) {
		m_vItemBox[i]->Create(IDD_ITEM_BOX, this);
		m_vItemBox[i]->SetItemSlotIndex(i);
	}

	CRect ClientRC, WindowRC, ItemBoxRC;
	this->GetClientRect(&ClientRC);
	this->GetWindowRect(&WindowRC);
	m_vItemBox.front()->GetWindowRect(&ItemBoxRC);

	int item_slot_size = ItemBoxRC.Width();
	int dialog_frame_width = WindowRC.Width() - ClientRC.Width();
	int dialog_frame_height = WindowRC.Height() - ClientRC.Height();

	// 인벤토리 창 설정 (아이템 박스가 (ROW_NUM * COL_NUM) 개 들어갈 수 있는 크기로 설정)
	this->MoveWindow(WindowRC.left, WindowRC.top,
		item_slot_size * INVENTORY_COL_NUM + dialog_frame_width,
		item_slot_size * INVENTORY_ROW_NUM + dialog_frame_height);

	// 인벤토리 내의 아이템 칸 설정
	int row = -1, col = -1;
	for (int i = 0; i < m_vItemBox.size(); i++) {
		col = i % INVENTORY_COL_NUM;
		if (col == 0) row++;

		m_vItemBox[i]->MoveWindow(item_slot_size * col, item_slot_size * row, item_slot_size, item_slot_size);
		m_vItemBox[i]->ShowWindow(SW_SHOW);
	}

	// 배경색 설정
	SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CInventoryDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



void CInventoryDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CWnd* pChildWnd = CWnd::ChildWindowFromPoint(point);

	if (pChildWnd != nullptr) {
		// 클릭한 인벤토리 칸의 인덱스 구하기
		m_nSource = GetItemSlotIndex(pChildWnd);

		if (m_nSource >= 0 && m_nSource < INVENTORY_COL_NUM * INVENTORY_ROW_NUM && m_pReferenceUserInventory->IsExist(m_nSource)) {
			// CBitmap으로 이미지 로드 
			CImage item_image;
			item_image.Load(m_pReferenceUserInventory->GetItem(m_nSource)->GetFilePath().c_str());

			CBitmap cBitMap;
			cBitMap.Attach(item_image.Detach());

			// 이미지 리스트 초기화 
			m_bDragging = true;
			m_pImageDrag.Create(64, 64, ILC_COLOR32 | ILC_MASK, 5, 0);

			// 드래그용 이미지 추가
			m_pImageDrag.Add(&cBitMap, RGB(0,0,0));

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

		// Hide Item Info
		m_pItemInfoDlg->ShowWindow(SW_HIDE);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CInventoryDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDragging) // 드래그하고 있다면 (== 드래그를 위한 이미지 리스트에 값이 존재하면)
	{
		ClientToScreen(&point); // 스크린 좌표 기반 마우스 좌표 반환

		// 모든 창에 드래그 상태를 유지해라.
		m_pImageDrag.DragEnter(NULL, point);
		m_pImageDrag.DragMove(point);
	}

	// 아이템 슬롯을 호버링하지 않는다면 
	CWnd* pChildWnd = CWnd::ChildWindowFromPoint(point);
	if (pChildWnd != nullptr) {
		ClientToScreen(&point);

		int iHoveredSlot = GetItemSlotIndex(pChildWnd);

		if (iHoveredSlot >= 0 && iHoveredSlot < INVENTORY_COL_NUM * INVENTORY_ROW_NUM) {
			// 아이템 슬롯 범위 설정
			CRect rc;
			m_vItemBox[iHoveredSlot]->GetWindowRect(&rc);
			rc.SetRect(rc.left + ITEM_SLOT_IN_PADDING, rc.top + ITEM_SLOT_IN_PADDING, rc.right - ITEM_SLOT_IN_PADDING, rc.bottom - ITEM_SLOT_IN_PADDING);

			// 아이템 슬롯 범위 내에 마우스를 호버링하고 있고, 해당 아이템 슬롯에 아이템이 존재하면,
			int index = m_iInven * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + iHoveredSlot;
			if (rc.PtInRect(point) && m_pReferenceUserInventory->IsExist(index)) {
				// Show Item Info
				// 아이템 정보 구하기
				std::shared_ptr<CNSW_Item> pHoveredItem = m_pReferenceUserInventory->GetItem(index);
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


void CInventoryDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDragging)// m_pImageDrag != NULL) // 드래그하고 있다면 (== 드래그를 위한 이미지 리스트에 값이 존재하면)
	{
		// 스크린 좌표 기반 마우스 좌표 반환 후 드래그 끝내기
		ClientToScreen(&point);
		m_pImageDrag.DragLeave(NULL);
		m_pImageDrag.EndDrag();
		ReleaseCapture();

		// 드래그를 마친 창의 리스트에 아이템추가하기
		CWnd* pWnd = CWnd::WindowFromPoint(point); // 드래그를 마친 창 (메인 윈도우 or 장비창)	

		pWnd->ScreenToClient(&point);
		CWnd* pChildWnd = pWnd->ChildWindowFromPoint(point); // 드래그를 마친 창 (인벤토리 내의 아이템 칸 확인 용)

		if (m_nSource >= 0 && pWnd != nullptr) {
			int index = m_iInven * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + m_nSource;
			if (m_pReferenceUserInventory->IsExist(index)) {
				// 메인 창에 드랍 시
				if (pWnd->GetSafeHwnd() == ::FindWindow(_T("Main_Window"), NULL)) {
					if (MessageBox(_T("해당 아이템을 버리시겠습니까?"), NULL, MB_OKCANCEL) == IDOK) {
						m_pUserCharacter->DiscardItem(m_iInven, m_nSource);
					}
				}
				// 장비 창의 장비 칸에 드랍 시 
				else if (pWnd->GetSafeHwnd() == ::FindWindow(_T("Equipment_Window"), NULL) && pChildWnd != nullptr) {
					int equipment_slot = GetItemSlotIndex(pChildWnd);

					CItemBoxDlg* pItemBox = dynamic_cast<CItemBoxDlg*>(pChildWnd);
					if (pItemBox != nullptr) {
						m_pUserCharacter->EquipItem(m_iInven, m_nSource, equipment_slot); // 아이템 장착 (인벤토리 -> 장비창)
					}
				}
				// 인벤토리 창에 드랍 시
				else {
					// 5개의 인벤토리 중 해당하는 인벤토리 찾기
					for (int i = ID_INVENTORY; i < INVENTORY_WINDOW_NUM; i++) {
						CString WndText;
						WndText.Format(_T("Inventory_%d"), i + 1);

						if (pWnd->GetSafeHwnd() == ::FindWindow(_T("Inventory_Window"), WndText) && pChildWnd != nullptr) {
							// Type Casting
							CInventoryDlg* pInventoryDlg = dynamic_cast<CInventoryDlg*>(pWnd);

							if (pInventoryDlg != nullptr) {
								// 드랍한 인벤토리 칸의 인덱스 구하기
								int iDroppedItemSlot = GetItemSlotIndex(pChildWnd);
								int dest_inven = i;

								CItemBoxDlg* pItemBox = dynamic_cast<CItemBoxDlg*>(pChildWnd);
								if (pItemBox != nullptr) {
									m_pUserCharacter->SwapItem(m_iInven, m_nSource, dest_inven, iDroppedItemSlot); // 아이템 이동 (인벤토리 -> 인벤토리)
								}

								break;
							}
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

void CInventoryDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// Hide Item Info
	m_pItemInfoDlg->ShowWindow(SW_HIDE);

	CDialogEx::OnClose();
}

void CInventoryDlg::UpdateItem(int location_idx, int slot_idx)
{
	if (location_idx == m_iInven) {
		int index = location_idx * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + slot_idx;
		std::shared_ptr<CNSW_Item> pItem = m_pReferenceUserInventory->GetItem(index);
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
