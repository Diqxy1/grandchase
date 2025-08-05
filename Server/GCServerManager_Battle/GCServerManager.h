// GCServerManager.h : main header file for the GCSERVERMANAGER application
//

#if !defined(AFX_GCSERVERMANAGER_H__476865C0_48CC_4A90_8816_1C3CDD8C7FE8__INCLUDED_)
#define AFX_GCSERVERMANAGER_H__476865C0_48CC_4A90_8816_1C3CDD8C7FE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGCServerManagerApp:
// See GCServerManager.cpp for the implementation of this class
//

class CGCServerManagerApp : public CWinApp
{
public:
	void OpenServer( void );
	CGCServerManagerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCServerManagerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGCServerManagerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCSERVERMANAGER_H__476865C0_48CC_4A90_8816_1C3CDD8C7FE8__INCLUDED_)
