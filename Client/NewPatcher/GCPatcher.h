// GCPatcher.h : main header file for the GCPATCHER application
//

#if !defined(AFX_GCPATCHER_H__1A5EB954_ADD5_4A65_8F4B_2E400D455B60__INCLUDED_)
#define AFX_GCPATCHER_H__1A5EB954_ADD5_4A65_8F4B_2E400D455B60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "globalfunction.h"
#include "resource.h"		// main symbols
#include "GCPatcherDlg.h"
#ifdef GRID_DOWN
#include "GCGridPatcherDlg.h"
#endif
#include "DownLoadDlg.h"
#include "patch.h"
/////////////////////////////////////////////////////////////////////////////
// CGCPatcherApp:
// See GCPatcher.cpp for the implementation of this class
//

class CGCPatcherApp : public CWinApp
{
public:
	CGCPatcherApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCPatcherApp)
	public:
	virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGCPatcherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    void GetArguments(std::vector<std::string>& tokens); 
    bool GetPatchTimeCheck( boost::shared_ptr<KPatchLib> patchlib );
    bool GetPatchPathFile( boost::shared_ptr<KPatchLib> patchlib );
    void GetPatchMode( boost::shared_ptr<KPatchLib> patchlib );
    bool CheckArguments();
    bool CheckSrcPath();
	bool CheckValidFolder( boost::shared_ptr<KPatchLib> patchlib, std::string reporturl, std::string service, std::string version );
	bool RunPatcher(KPatchLib & patchlib);
	bool RunClient(KPatchLib & patchlib);
	void SaveLastUpdateDate();
    bool RunLogProgram(std::string strFile,std::string arg, std::string mt = "1");
	bool RunExecute( std::string filename, std::string param );
	std::vector<std::string> arguments;	

	std::string serverurl;
	std::string patchpath;
	std::string patchmode;
	std::string patchtimecheckpath;		
	std::string manualpatchpath;

	bool istest; // 사내 테스트 여부 
	bool skippatchtimecheck;// 패치타임체크 여부 
	bool patchtimecheck;
    bool isrunwithoutpach;
    bool skippatcherpatch;
	bool ismanual;

	std::string id;
	std::string pw;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCPATCHER_H__1A5EB954_ADD5_4A65_8F4B_2E400D455B60__INCLUDED_)
