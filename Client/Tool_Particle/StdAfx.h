// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B3245B75_42F9_4FEA_9AF2_8206EED4DDD8__INCLUDED_)
#define AFX_STDAFX_H__B3245B75_42F9_4FEA_9AF2_8206EED4DDD8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <Afx.h>
#include <windows.h>
#include <basetsd.h>
#include <cmath>
#include <stdio.h>
#include <D3DX9.h>

#include <tchar.h>
#include "D3DApp.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "resource.h"
#include "ImportKncDebug.h"

#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/multiplies.hpp>

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/random.hpp>
#include <boost/foreach.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/scope_exit.hpp>
#include <boost/signal.hpp>
#include <boost/signals2.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>	//	숫자를 문자열로, 문자열을 숫자로 바꿀때 편하다.
#include <boost/circular_buffer.hpp>    // 환형큐


//#include <vector>
#include "../stdafx_stl.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B3245B75_42F9_4FEA_9AF2_8206EED4DDD8__INCLUDED_)
