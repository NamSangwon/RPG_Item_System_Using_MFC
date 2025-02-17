// CharacterSelectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "../../MFCGame.h"
#include "afxdialogex.h"
#include "CharacterSelectDlg.h"

#include "../../Manager/DatabaseManager.h"


// CCharacterSelectDlg 대화 상자

IMPLEMENT_DYNAMIC(CCharacterSelectDlg, CDialogEx)

CCharacterSelectDlg::CCharacterSelectDlg(CString user_id, std::shared_ptr<CUserCharacter>& user_character, std::vector<std::string>& vCharacters, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARACTER_SELECT, pParent)
{
	m_nSelection = U_NONE;

	m_strUserID = user_id;
	m_pUserCharacter = user_character;
	m_vCharacters = vCharacters;
}

CCharacterSelectDlg::~CCharacterSelectDlg()
{
}

void CCharacterSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARACTER_LIST, m_cCharacterList);
	DDX_Control(pDX, IDC_INPUT_GENDER, m_cInputGender);
}


BEGIN_MESSAGE_MAP(CCharacterSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHARACTER_CREATE, &CCharacterSelectDlg::OnBnClickedCharacterCreate)
	ON_BN_CLICKED(IDC_CHARACTER_SELECT, &CCharacterSelectDlg::OnBnClickedCharacterSelect)
	ON_BN_CLICKED(IDC_CHARACTER_DELETE, &CCharacterSelectDlg::OnBnClickedCharacterDelete)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_CLICK, IDC_CHARACTER_LIST, &CCharacterSelectDlg::OnNMClickCharacterList)
END_MESSAGE_MAP()




BOOL CCharacterSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 캐릭터 리스트
	m_cCharacterList.ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0);
	m_cCharacterList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CFont cFont;
	cFont.CreatePointFont(200, "Arail");
	m_cCharacterList.SetFont(&cFont);

	CRect rc;
	m_cCharacterList.GetClientRect(&rc);
	m_cCharacterList.InsertColumn(0, _T("CHARACTER"), 0, rc.Width(), -1);

	for (int i = 0; i < m_vCharacters.size(); i++) {
		m_cCharacterList.InsertItem(i, m_vCharacters[i].c_str());
	}

	// 성별 선택 콤보 박스
	m_cInputGender.AddString(_T("Male"));
	m_cInputGender.AddString(_T("Female"));

	if (m_cInputGender.GetCount() > 0)
		m_cInputGender.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CCharacterSelectDlg::OnBnClickedCharacterCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCharacterName = m_cCharacterList.GetItemText(m_nSelection, 0);

	CString inputName;
	GetDlgItemText(IDC_INPUT_NAME, inputName);
	int inputGender = m_cInputGender.GetCurSel();

	if (!inputName.IsEmpty() && inputGender != CB_ERR) {
		if (CDatabaseManager::GetInstance().CreateCharacter(
			m_strUserID.GetString(),
			inputName.GetString(),
			static_cast<int>(CharacterClass::Citizen),
			inputGender
		) == EXIT_SUCCESS) {
			m_cCharacterList.InsertItem(m_vCharacters.size(), inputName);
			m_vCharacters.push_back(inputName.GetString());
		}
	}
}


void CCharacterSelectDlg::OnBnClickedCharacterSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCharacterName = m_cCharacterList.GetItemText(m_nSelection, 0);

	// 캐릭터 데이터 불러오기
	std::string input_name = strCharacterName.GetString();
	if (CDatabaseManager::GetInstance().LoadCharacterData(input_name, m_pUserCharacter) == EXIT_SUCCESS)
		CDialogEx::OnOK();
	else
		CDialogEx::OnCancel();
}


void CCharacterSelectDlg::OnBnClickedCharacterDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCharacterName = m_cCharacterList.GetItemText(m_nSelection, 0);

	if (CDatabaseManager::GetInstance().DeleteCharacter(strCharacterName.GetString()) == EXIT_SUCCESS) {
		m_cCharacterList.DeleteItem(m_nSelection);
		m_vCharacters.erase(std::remove(m_vCharacters.begin(), m_vCharacters.end(), m_vCharacters[m_nSelection]), m_vCharacters.end());
	}
}

void CCharacterSelectDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDCtl == IDC_CHARACTER_LIST)
	{
		lpMeasureItemStruct->itemHeight += 20;
	}

	CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CCharacterSelectDlg::OnNMClickCharacterList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nSelection = pNMIA->iItem;

	*pResult = 0;
}
