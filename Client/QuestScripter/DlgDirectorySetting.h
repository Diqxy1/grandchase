#pragma once
#include "stdafx.h"
#include "ReadOnlyEdit.h"
#include "KNCSingleton.h"
#include "afxwin.h"
// CDlgDirectorySetting 대화 상자입니다.

class CDlgDirectorySetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgDirectorySetting)

	DeclareSingleton(CDlgDirectorySetting);
public:
	CDlgDirectorySetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDirectorySetting();

	CString m_strMapPath;
	CString m_strMapSubFilePath;
	CString m_strSoundPath;
	CString m_strQuestScriptPath;

	bool WriteToRegister(void);
	bool GetFromRegister(void);
	void OpenDirectoryDialog(CString *pstrResult, CString strTitle, CString strPath);

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DIRECTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CReadOnlyEdit m_ebMap;
	CReadOnlyEdit m_ebMapSubFile;
	CReadOnlyEdit m_ebSound;
	CReadOnlyEdit m_ebQuestScript;
	afx_msg void OnBnClickedButtonMap();
	afx_msg void OnBnClickedButtonSound();
	afx_msg void OnBnClickedButtonQuestscript();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonMapsubfile();
	virtual BOOL OnInitDialog();
	CComboBox m_cbState;
	afx_msg void OnCbnSelendokComboStateSorting();
};

DefSingletonInline(CDlgDirectorySetting);