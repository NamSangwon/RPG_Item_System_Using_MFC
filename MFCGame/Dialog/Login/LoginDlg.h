#pragma once
#include "afxdialogex.h"
#include "../../Character/UserCharacter.h"

#include "RegisterDlg.h"
#include "DeleteUserDlg.h"
#include "CharacterSelectDlg.h"

// CLoginDlg 대화 상자

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(std::shared_ptr<CUserCharacter>& user_character, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLoginDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_cInputID;
	CEdit m_cInputPW;

	std::shared_ptr<CUserCharacter> m_pUserCharacter;

	std::shared_ptr<CCharacterSelectDlg> m_pCharacterSelectDlg;

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonDelete();
};
