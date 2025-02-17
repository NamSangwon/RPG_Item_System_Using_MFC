#pragma once
#include "afxdialogex.h"

#include "../../Character/UserCharacter.h"

// CCharacterSelectDlg 대화 상자

class CCharacterSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCharacterSelectDlg)

public:
	CCharacterSelectDlg(CString user_id, std::shared_ptr<CUserCharacter>& user_character, std::vector<std::string>& vCharacters, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCharacterSelectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARACTER_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_cCharacterList;
	CComboBox m_cInputGender;

	std::shared_ptr<CUserCharacter> m_pUserCharacter;
	std::vector<std::string> m_vCharacters;
	CString m_strUserID;

	int m_nSelection;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCharacterCreate();
	afx_msg void OnBnClickedCharacterSelect();
	afx_msg void OnBnClickedCharacterDelete();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnNMClickCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
};
