// MFCMassFileDlg.h : header file
//

#pragma once

#include "MassFileLib/GCMassFile.h"
#include "afxwin.h"
#include "KMyListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMFCMassFileDlg dialog

class CMFCMassFileDlg : public CDialog
{
// Construction
public:
	CMFCMassFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMFCMassFileDlg)
	enum { IDD = IDD_MFCMASSFILE_DIALOG };
	
	CString	m_KomFilePath;
	BOOL	m_bZip;
	//}}AFX_DATA
	KMyListCtrl	m_ListFileName;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCMassFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
private:	
	void UpdateTotalCount();
	void AddFile(const char* strFileName, DWORD dwFileSize );
	void CompressFile( CFileDialog &dlg, std::vector<std::string>& vecFileName);
	CString ExecuteConsoleProgram(CString strExec);
	void CheckAddDirectory(const char* strFileName);
	CString Decompress( CString& csComFile );
    void Load(const char* strFileName);
	void DeleteDirectoryAbsolutely( const char* strDirectory_ );
	void DeleteFileAbsolutely( const char* szFileName ) const;
	void AppendBackSlash( std::string& str ) const;

	GCMassFile m_MassFile;	


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMFCMassFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	virtual void OnOK();
	afx_msg void OnRemove();
	afx_msg void OnDecompress();
	afx_msg void OnOpenkom();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDeleteall();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnBegindragFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void CMFCMassFileDlg::OnEnddragFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult);
	
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedFilename(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickFilename(NMHDR *pNMHDR, LRESULT *pResult);

	static bool s_bSortReverse;
	afx_msg void OnDestroy();
    BOOL m_bUseMKOM;
    afx_msg void OnBnClickedOk();
    BOOL m_bUseCRC;
	CButton btnDecompress;
};

extern CMFCMassFileDlg* g_pMassFileDlg;