#pragma once

//#include <vector>

// CDlgFileList 대화 상자입니다.

class CDlgFileList : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileList)

public:
	CDlgFileList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgFileList();

	enum { FL_MIN_WINDOW_SIZE_X = 600, FL_MIN_WINDOW_SIZE_Y = 250 };

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FILE_LIST };

	bool AddList(CString strFileName);
	void AddFolderFileList(CString strFolder);
	void InsertList(int iIndex, CString strFileName);
	void DeleteList(int iIndex);
	void ClearList(void);
	void RefreshFileList(void);
	void SwapFileList(int iFirst, int iSecond);
	std::pair<CString, COleDateTime> GetItem(int iIndex);
	int GetItemCount(void) { return (int)m_vtFileList.size(); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl       m_lcFileList;
	CButton         m_btFolderAdd;
	CButton         m_btFileAdd;
	CButton         m_btDelete;
	CButton         m_btClearList;
	CButton         m_btLoad;
	CButton         m_btSave;
	CButton         m_btClose;
	CButton         m_btServerScript;
	afx_msg void OnBnClickedButtonFolderAdd();
	afx_msg void OnBnClickedButtonFileAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClearList();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSave();

private:
	std::vector< std::pair<CString, COleDateTime> > m_vtFileList;        // <FileName, Time>
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnNMDblclkListFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSaveToSscript();
};
