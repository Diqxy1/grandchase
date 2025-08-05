// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__67FC60AD_C221_4A1B_8CA2_0DCAECD8E926__INCLUDED_)
#define AFX_STDAFX_H__67FC60AD_C221_4A1B_8CA2_0DCAECD8E926__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{ Appended by parkch: 2004-05-18 오전 11:35:28
#pragma warning(disable:4786)
//}} Appended by parkch: 2004-05-18 오전 11:35:28

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// 아래 지정된 플랫폼보다 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0501		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINDOWS		// Windows 98 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0501 // Windows Me 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 4.0 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0700	// IE 5.0 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{ Appended by parkch: 2003-11-24 오후 2:58:24
#ifdef TRY
#undef TRY
#endif // TRY
//}} Appended by parkch: 2003-11-24 오후 2:58:24

//{{ Appended by parkch: 2003-12-10 오후 3:50:51
#define USER_STOP           (-2)
#define USER_BAD            (-1)
#define USER_NORMAL         (0)
#define USER_CHALLENGER     (1)
#define USER_SUPERIOR_MC    (2)
#define USER_ADMIN          (3)
//}} Appended by parkch: 2003-12-10 오후 3:50:51

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          if (p) { delete (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    if (p) { delete[] (p); (p) = NULL; }
#endif // #ifndef SAFE_DELETE_ARRAY

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         if (p) { (p)->Release(); (p) = NULL; }
#endif // #ifndef SAFE_RELEASE

#include <map>

extern int g_nLevel;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__67FC60AD_C221_4A1B_8CA2_0DCAECD8E926__INCLUDED_)
