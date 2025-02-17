
// MFCApplicationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../framework.h"
#include "../../MFCGame.h"
#include "MFCGameDlg.h"
#include "afxdialogex.h"

#include "../../Manager/DatabaseManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMFCApplicationDlg 대화 상자

CMFCGameDlg::CMFCGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION_DIALOG, pParent)
{
	// resource와 동일한 Class Name을 지정합니다.
	WNDCLASS wc = {};
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
	wc.lpszClassName = _T("Main_Window");
	AfxRegisterClass(&wc);

	// icon
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMFCGameDlg::~CMFCGameDlg()
{

}

void CMFCGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RECOVER_HP, &CMFCGameDlg::OnBnClickedRecoverHp)
	ON_BN_CLICKED(IDC_REDUCE_HP, &CMFCGameDlg::OnBnClickedReduceHp)
	ON_BN_CLICKED(IDC_RECOVER_MP, &CMFCGameDlg::OnBnClickedRecoverMp)
	ON_BN_CLICKED(IDC_REDUCE_MP, &CMFCGameDlg::OnBnClickedReduceMp)
	ON_BN_CLICKED(IDC_GET_EXP, &CMFCGameDlg::OnBnClickedGetExp)
END_MESSAGE_MAP()


// CMFCApplicationDlg 메시지 처리기

BOOL CMFCGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	std::shared_ptr<CInventory> user_inventory = std::make_shared<CInventory>();
	std::shared_ptr<CEquipment> user_equipment = std::make_shared<CEquipment>();
	std::shared_ptr<CUserStatus> user_status = std::make_shared<CUserStatus>();

	m_pUserCharacter = std::make_shared<CUserCharacter>(user_inventory, user_equipment, user_status);
	IObserver* pObserver = nullptr;

	// 인벤토리 창, 장비창, 아이템 생성 창에 사용될 아이템 정보 창 생성
	m_pItemInfoDlg = std::make_shared<CItemInfoDlg>();
	m_pItemInfoDlg->Create(IDD_ITEM_INFO, this);
	m_pItemInfoDlg->SetWindowText(_T("Item_Info"));

	// 유저 상태 바 생성
	m_pUserBarDlg = std::make_shared<CUserBarDlg>();
	m_pUserBarDlg->Create(IDD_USER_BAR, this);
	m_pUserBarDlg->SetWindowText(_T("User_Status_Bar"));
	pObserver = dynamic_cast<IObserver*>(m_pUserBarDlg.get());
	if (pObserver != nullptr)
		m_pUserCharacter->RegistStatus(pObserver);

	// 유저 기본 스테이터스 창 생성
	m_pBasicStatusDlg = std::make_shared<CStatusDlg>(ID_BASIC_STATUS);
	m_pBasicStatusDlg->Create(IDD_STATUS, this);
	m_pBasicStatusDlg->SetWindowText(_T("Basic_Status"));
	pObserver = dynamic_cast<IObserver*>(m_pBasicStatusDlg.get());
	if (pObserver != nullptr)
		m_pUserCharacter->RegistStatus(pObserver);
	
	// 유저 어드밴스드 스테이터스 창 생성
	m_pAdvancedStatusDlg = std::make_shared<CStatusDlg>(ID_ADVANCED_STATUS);
	m_pAdvancedStatusDlg->Create(IDD_STATUS, this);
	m_pAdvancedStatusDlg->SetWindowText(_T("Advanced_Status"));
	pObserver = dynamic_cast<IObserver*>(m_pAdvancedStatusDlg.get());
	if (pObserver != nullptr)
		m_pUserCharacter->RegistStatus(pObserver);

	// 스테이터스 창 위치 및 크기 조정
	ResizeStatusDialog();

	// 아이템 가챠 윈도우 생성
	m_pGachaControllerDlg = std::make_shared<CGachaControllerDlg>(m_pItemInfoDlg, m_pUserCharacter);

	// 장비창 생성
	m_pEquipmentDlg = std::make_shared<CEquipmentDlg>(m_pItemInfoDlg, user_equipment, m_pUserCharacter);
	m_pEquipmentDlg->Create(IDD_EQUIPMENT, this);
	m_pEquipmentDlg->SetWindowText(_T("Equipment"));
	pObserver = dynamic_cast<IObserver*>(m_pEquipmentDlg.get());
	if (pObserver != nullptr)
		m_pUserCharacter->RegistEquipment(pObserver);

	// 인벤토리 창 생성
	for (int i = 0; i < INVENTORY_WINDOW_NUM; i++) {
		m_vInventoryDlg.push_back(std::make_shared<CInventoryDlg>(m_pItemInfoDlg, user_inventory, m_pUserCharacter));
		m_vInventoryDlg[i]->Create(IDD_INVENTORY, this);
		pObserver = dynamic_cast<IObserver*>(m_vInventoryDlg[i].get());
		if (pObserver != nullptr)
			m_pUserCharacter->RegistInventory(pObserver);

		CString tmp;
		tmp.Format(_T("Inventory_%d"), i + 1);
		m_vInventoryDlg[i]->SetWindowText(tmp);
		m_vInventoryDlg[i]->SetInventoryIdx(i);
	}

	// 로그인 창 생성
	m_pLoginDlg = std::make_shared<CLoginDlg>(m_pUserCharacter);
	if (m_pLoginDlg->DoModal() != IDOK) {
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE) {
		QuitProgram();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// https://saack.tistory.com/69 (DoMadal vs Create)
BOOL CMFCGameDlg::PreTranslateMessage(MSG* pMsg) {
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		if ((pMsg->wParam >= '1' && pMsg->wParam <= '5')) { // Inventory 1 ~ 5
			TCHAR ch = pMsg->wParam;
			int inventory_idx = ch - '1';

			m_vInventoryDlg[inventory_idx]->ShowWindow(SW_SHOW);

			return TRUE;
		}
		else if (pMsg->wParam == 'E') { // Equipment
			m_pEquipmentDlg->ShowWindow(SW_SHOW);

			return TRUE;
		}
		else if (pMsg->wParam == 'G') { // Gacha
			m_pGachaControllerDlg->DoModal();

			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
			return QuitProgram();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CMFCGameDlg::QuitProgram() {
	if (MessageBox(_T("프로그램을 종료하시겠습니까?"), _T("Software Exit"), MB_YESNO) == IDYES) {

		// DB에 유저 스테이터스 및 유저가 소유한 아이템 저장
		if (CDatabaseManager::GetInstance().StoreCharacterData(m_pUserCharacter) == EXIT_SUCCESS);
			// 메인 창 종료
			AfxGetMainWnd()->PostMessage(WM_QUIT); 
	}

	return TRUE;
}

void CMFCGameDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pBasicStatusDlg->GetSafeHwnd() != nullptr && m_pAdvancedStatusDlg->GetSafeHwnd() != nullptr) {
		ResizeStatusDialog();
	}
}

void CMFCGameDlg::ResizeStatusDialog()
{
	CRect BasicStatusRC, AdvancedStatusRC, UserBarRC, MainRC;
	m_pBasicStatusDlg->GetWindowRect(&BasicStatusRC);
	m_pAdvancedStatusDlg->GetWindowRect(&AdvancedStatusRC);
	m_pUserBarDlg->GetWindowRect(&UserBarRC);
	this->GetClientRect(&MainRC);

	m_pBasicStatusDlg->MoveWindow(
		MainRC.left,
		MainRC.bottom - BasicStatusRC.Height(),
		BasicStatusRC.Width(),
		BasicStatusRC.Height()
	);

	m_pAdvancedStatusDlg->MoveWindow(
		MainRC.right - AdvancedStatusRC.Width(),
		MainRC.bottom - AdvancedStatusRC.Height(),
		AdvancedStatusRC.Width(),
		AdvancedStatusRC.Height()
	);

	m_pUserBarDlg->MoveWindow(
		MainRC.CenterPoint().x - UserBarRC.Width()/2,
		MainRC.bottom - UserBarRC.Height(),
		UserBarRC.Width(),
		UserBarRC.Height()
	);
}


void CMFCGameDlg::OnBnClickedRecoverHp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pUserCharacter->AddBasicStatusValue(1000, 0, 0, 0, 0, 0, 0, 0);
}


void CMFCGameDlg::OnBnClickedReduceHp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pUserCharacter->AddBasicStatusValue(-1000, 0, 0, 0, 0, 0, 0, 0);
}


void CMFCGameDlg::OnBnClickedRecoverMp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pUserCharacter->AddBasicStatusValue(0, 1000, 0, 0, 0, 0, 0, 0);
}


void CMFCGameDlg::OnBnClickedReduceMp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pUserCharacter->AddBasicStatusValue(0, -1000, 0, 0, 0, 0, 0, 0);
}


void CMFCGameDlg::OnBnClickedGetExp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}