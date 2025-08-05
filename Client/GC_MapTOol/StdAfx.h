#pragma once
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AEC0E673_144E_4429_869E_8B11B334F5A1__INCLUDED_)
#define AFX_STDAFX_H__AEC0E673_144E_4429_869E_8B11B334F5A1__INCLUDED_

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../stdafx_stl.h"
#include "../stdafx_boost.h"

//#include <windowsx.h>
#include <basetsd.h>
#include <cmath>
//#include <stdio.h>
#include <D3DX9.h>
#include <DXErr.h>
#include <tchar.h>
#include <dinput.h>
#include <d3d9types.h>
#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DIUtil.h"
#include "DMUtil.h"
#include "DXUtil.h"
#include "resource.h"
#include "D3DRes.h"
#include <fstream>
#include <dbg/dbg.hpp>
//#include "../Define.h"
#include "../ProtectedType_new.h"

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// lua
#include "../GCLuaManager/KLuabinder.h"
#include "../GCLuaManager/KLuaManager.h"
#include "./KGCLuaBinder_Map.h"

#include "../GCUtil/GCUtil.h"
#include "../GCDeviceLib/GCDevice.h"
#include "../GCDeviceLib/GCBaseMesh.h"
#include "../GCDeviceLib/GCDeviceVertexBuffer.h"
#include "../GCDeviceLib/GCDeviceIndexBuffer.h"
#include "../Vertex.h"
#include "../GCDeviceLib/GCDeviceManager.h"
#include "../GCDeviceLib/GCDeviceShader.h"
#include "../GCDeviceLib/GCDeviceFont.h"
#include "../GCDeviceLib/GCDeviceTexture.h"
#include "../Kom/manager.h"
//#include "../MassFileLib/GCMassFileManager.h"

//#include <algorithm>

//#include <afxdhtml.h>

/*---------------------------------------------------------------------------------------
Object의 Z 거리를 적어둡니다.
---------------------------------------------------------------------------------------*/

// Projections Near & Far
//#define WOGZ_FAR				(1000.0f)
//#define WOGZ_NEAR				(0.1f)
#define WOG_WIDTH				(800.0f)
#define WOG_HEIGHT				(600.0f)

#define TO_TOOL_COORDINATE_RATE   (400.0f)
#define TO_GC_COORDINATE_RATE   (0.0025f)
//
//
//#define WOGZ_SHADOW				(900.0f)
//#define WOGZ_MONSTER			(500.0f)
//#define WOGZ_PLAYER				(300.0f)

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AEC0E673_144E_4429_869E_8B11B334F5A1__INCLUDED_)
