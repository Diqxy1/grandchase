#pragma once
#include "afxwin.h"
#include "KWndManager.h"
//#include <vector>

// CLoadSettingWindow 대화 상자입니다.
class CLoadSettingWindow : public CDialog
{
	DECLARE_DYNAMIC(CLoadSettingWindow)

public:
	CLoadSettingWindow(CWnd* pParent = NULL);   // 표준 생성자입니다.    

	virtual ~CLoadSettingWindow();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PRE_FILELIST };

    void OpenDirectoryDialog(CString *pstrResult, CString strTitle, CString strPath);
    bool WriteToRegister(void); 
    bool GetFromRegister(void);
    void AddFolderFileList(CString strFolder, bool bTexture);
    bool AddList(CString strFileName, bool bTexture);
    void DeleteList(int iIndex, bool bTexture);
    
    void FullPatthMode(bool bShow);

    int GetTextureListCnt(void) { return (int)m_vtTextureList.size(); }
    int GetScriptListCnt(void) { return (int)m_vtScriptList.size(); }    
    
    std::string GetTextureFromIndex(int iIndex) { if( m_vtTextureList.size() <= iIndex ) return ""; return m_vtTextureList[iIndex]; }
    std::string GetScriptFromIndex(int iIndex) { if( m_vtScriptList.size() <= iIndex ) return ""; return m_vtScriptList[iIndex]; }

private:
	void AddTexture( CString strFileName );
	void AddScript( CString strFileName );    



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public: 
    std::vector< std::string > m_vtTextureList;
    std::vector< std::string > m_vtScriptList;

    afx_msg void OnBnClickedButtonAddTexture();
    afx_msg void OnBnClickedButtonAddScript();
    CListBox m_lbTexture;
    CListBox m_lbScript;
    CButton m_raFullPath;
    CButton m_raOnlyName;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnDropFiles(HDROP hDropInfo);
    virtual BOOL OnInitDialog();
    afx_msg void OnShowType();
    afx_msg void OnLbnDblclkList2();
    afx_msg void OnLbnDblclkList3();
    afx_msg void OnBnClickedButtonTexDelete();
    afx_msg void OnBnClickedButtonTexScript();
    afx_msg void OnBnClickedSaveSetting();    

        BOOL CreateCopy();
        BOOL TranslateFormat();
        BOOL FindCaptionString( IN char* strScriptFile, IN std::vector<tagUIScriptFile>& vecData, OUT CString& strCaption );
};
