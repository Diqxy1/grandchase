#pragma once
#include "afxwin.h"
#include "LoadSettingWindow.h"

#define TEXTURE_DIR "Texture Directory Path "
#define SCRIPT_DIR  "Script Directory Path "
#define TEXTURE_CNT "Texture Count"
#define SCRIPT_CNT  "Script Count"
#define EXTENSION   ".ini"
#define SAVE_DIR    "C:\\Program Files\\UIScript Setting\\"

// CSettingListDlg 대화 상자입니다.

class CSettingListDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingListDlg)

public:
	CSettingListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSettingListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETTINGLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    void SetLoadSettingInstance(CLoadSettingWindow* pLoadFileInfoWindow ) { m_LoadFileInfoWindow = pLoadFileInfoWindow; }    
    void GetIniFile(const char *parmPath);
    void SaveSetting();
    void LoadSetting( CString lAppName );    
    void ResetContent();

public:
    int m_TextureListCnt;
    int m_ScriptListCnt;    
    CListBox m_list;
    CString  m_strInput;
    CLoadSettingWindow* m_LoadFileInfoWindow; 
        
    afx_msg void OnLbnSelchangeList1();
    afx_msg void OnBnClickedAddButton();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedLoadButton();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedDeleteButton();    
};
