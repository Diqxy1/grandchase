// MFCMassFile.h : main header file for the MFCMASSFILE application
//

#pragma once
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMFCMassFileApp:
// See MFCMassFile.cpp for the implementation of this class
//



class CMFCMassFileApp : public CWinApp
{
public:
    CMFCMassFileApp();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMFCMassFileApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

    // Implementation

    //{{AFX_MSG(CMFCMassFileApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


////////////////////////////////////////////////////////////////////////////