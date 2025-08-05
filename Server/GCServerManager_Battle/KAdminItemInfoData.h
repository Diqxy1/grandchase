#if !defined(AFX_KADMINITEMINFODATA_H__4889734A_F646_4E5A_B05E_969B9D2E497C__INCLUDED_)
#define AFX_KADMINITEMINFODATA_H__4889734A_F646_4E5A_B05E_969B9D2E497C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KAdminItemInfoData.h : header file
//

#include "iostream"
using namespace std;
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// KAdminItemInfoData dialog

struct KAdminUseItemInfo;

class KAdminItemInfoData : public CDialog
{
// Construction
public:
	KAdminItemInfoData(CWnd* pParent = NULL);   // standard constructor
    void SetAdminItemInfoData( std::vector<KAdminUseItemInfo> vecAdminItemInfoData_ );

// Dialog Data
	//{{AFX_DATA(KAdminItemInfoData)
	enum { IDD = IDD_ADMIN_ITEM_IFNO_DATA };
	CListCtrl	m_crlItemInfoList;
	int		m_iGoodsID;
	CString	m_strGoodsName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KAdminItemInfoData)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    std::vector<KAdminUseItemInfo> m_vecAdminItemInfoData;

	// Generated message map functions
	//{{AFX_MSG(KAdminItemInfoData)
	afx_msg void OnSelectAdminItemInfoData();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADMINITEMINFODATA_H__4889734A_F646_4E5A_B05E_969B9D2E497C__INCLUDED_)
