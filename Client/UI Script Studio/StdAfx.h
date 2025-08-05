// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__65A87BCE_A35F_4393_914D_9A21309B0D14__INCLUDED_)
#define AFX_STDAFX_H__65A87BCE_A35F_4393_914D_9A21309B0D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../stdafx_stl.h"

#include "../GCUtil/GCUtil.h"

// lua
#include "KLuabinder.h"
#include "KLuaManager.h"

#define GC_SCREEN_WIDTH             (1024)
#define GC_SCREEN_HEIGHT            (768)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__65A87BCE_A35F_4393_914D_9A21309B0D14__INCLUDED_)
