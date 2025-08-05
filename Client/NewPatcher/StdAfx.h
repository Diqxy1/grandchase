// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__80B671AC_2BFE_419D_BAF9_BA4D9B120166__INCLUDED_)
#define AFX_STDAFX_H__80B671AC_2BFE_419D_BAF9_BA4D9B120166__INCLUDED_

#pragma once


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#ifdef _UNICODE
#undef _UNICODE
#endif //_UNICODE
#include <afxwin.h>         // MFC core and standard components

#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          if (p) { delete (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    if (p) { delete[] (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE_ARRAY


#include "wingdi.h"

#pragma comment( lib, "Wininet.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "urlmon.lib" )

#pragma warning ( disable:4819 )
#pragma warning ( disable:4996 )
#include "../stdafx_stl.h"


#include <curl/curl.h>
#include <tchar.h>
#include <d3d9.h>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

#include <fstream>

#include <assert.h>

#include "bugtrapheader_fordll.h"
#include <dbg/dbg.hpp>
#include <Thread/Locker.h>
#include <DSetup.h>
#include "define.h"
#include "jpeg/header/ximage.h"

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/pool/pool.hpp>
#include <boost/scoped_array.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <curl/curl.h>	

#include <io.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "KCSLockType.h"


//#ifdef _DEBUG
//    #pragma comment( lib, "./dbg/KncDebug71MDd" )
//#else
//    #pragma comment( lib, "./dbg/KncDebug71MD" )
//#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

struct SGridDownFileInfo
{
public:
	SGridDownFileInfo()
		:szURL(""),
		szDIR(""),
		szFileName(""),
		ldFileSize(0)
	{}

	SGridDownFileInfo(const char* a_szURL, const char* a_szDIR, const char* a_szFileName, __int64 a_ldFileSize)
	{
		szURL = a_szURL;
		szDIR = a_szDIR;
		szFileName = a_szFileName;
		ldFileSize = a_ldFileSize;
	}

	SGridDownFileInfo(const SGridDownFileInfo& r){
		szURL = r.szURL;
		szDIR = r.szDIR;
		szFileName = r.szFileName;
		ldFileSize = r.ldFileSize;
	}
	SGridDownFileInfo& operator=(SGridDownFileInfo& r) {
		szURL = r.szURL;
		szDIR = r.szDIR;
		szFileName = r.szFileName;
		ldFileSize = r.ldFileSize;
		return *this; 
	}

	const char* szURL;
	const char* szDIR;
	const char* szFileName;
	__int64		ldFileSize;
};

#endif // !defined(AFX_STDAFX_H__80B671AC_2BFE_419D_BAF9_BA4D9B120166__INCLUDED_)

