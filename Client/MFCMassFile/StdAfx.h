// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#define FBOOL(value) (value==0 ? false:true)
#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxole.h>
#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <string>

extern UINT g_uCustomClipbrdFormat;
extern bool g_bNT;

#include <CdErr.h>
#include <shlwapi.h>

#include "../stdafx_stl.h"
#include <Windows.h>
