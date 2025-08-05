#if !defined(AFX_GCHTMLVIEW_H__F88D639C_2C62_44CF_A12F_D8C417E1E30F__INCLUDED_)
#define AFX_GCHTMLVIEW_H__F88D639C_2C62_44CF_A12F_D8C417E1E30F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GCHtmlView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGCHtmlView html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CGCHtmlView : public CHtmlView
{
//protected:
public:
	CGCHtmlView();           // protected constructor used by dynamic creation
	virtual ~CGCHtmlView();

// html Data
public:
	//{{AFX_DATA(CGCHtmlView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	//Wnd의 Control자리에 정확하게 붙습니다
    CRect m_kRect;
    int  m_iWidth;
    int  m_iHeight;
    CDC* m_pDC;
	void AttachToDialog(CDialog* pParentWnd, DWORD dwTargetControlID, CString strUrl = "", bool bGrid = false );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCHtmlView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGCHtmlView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCHTMLVIEW_H__F88D639C_2C62_44CF_A12F_D8C417E1E30F__INCLUDED_)
