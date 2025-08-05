// GCFileDiff.h : main header file for the GCFILEDIFF application
//

#if !defined(AFX_GCFILEDIFF_H__20C3D599_9C43_4480_BEA1_EF2F132EF3DB__INCLUDED_)
#define AFX_GCFILEDIFF_H__20C3D599_9C43_4480_BEA1_EF2F132EF3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffApp:
// See GCFileDiff.cpp for the implementation of this class
//

class CGCFileDiffApp : public CWinApp
{
public:
	CGCFileDiffApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCFileDiffApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGCFileDiffApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCFILEDIFF_H__20C3D599_9C43_4480_BEA1_EF2F132EF3DB__INCLUDED_)
