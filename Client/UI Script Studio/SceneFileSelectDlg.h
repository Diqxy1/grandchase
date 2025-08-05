#pragma once
#include "afxwin.h"
//#include <vector>
#include "KWndManager.h"
#include "afxcmn.h"

extern KWndManager				*g_WndMgr;

// CSceneFileSelectDlg 대화 상자입니다.

class CSceneFileSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CSceneFileSelectDlg)

public:
	CSceneFileSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSceneFileSelectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SCENE_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

    virtual BOOL OnInitDialog();

    void OnOK();

public:
    
    CListCtrl m_listCtrlSceneFile;

    std::vector<tagUIScriptFile> m_vecUIScriptFile;
    std::vector<tagUIScriptFile> m_vecOrgUIScriptFile;  // 검색시 원본보존을 위해
    CString m_strFilePath;
    CString m_strSelectedFile;
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedSceneFind();

public:
    //void AddFileList( std::vector<std::string> &vecFileName );
    void AddFileNameToListControl( std::vector<tagUIScriptFile> &vecFileName );       // 2011. 09. 01
    
    afx_msg void OnNMClickListControlSceneSelect(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedSceneFindType();
    afx_msg void OnEnChangeSceneEdit();
    afx_msg void OnNMReturnListControlSceneSelect(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnKeydownListControlSceneSelect(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMSetfocusListControlSceneSelect(NMHDR *pNMHDR, LRESULT *pResult);
};