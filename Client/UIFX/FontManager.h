#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#if _MSC_VER > 1000
#   pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

//#include "../KAssert.h"
#include <d3dx9.h>
//#include <map>
//#include "GCDeviceFont.h"
class GCDeviceFont;

#if defined( _THAI_LANG )	//태국은 Tahoma체를 사용합니다.
extern GCDeviceFontTH* g_pkFontMgr;
extern GCDeviceFontTH* g_pkFontMgrSmall;
extern GCDeviceFontTH* g_pkFontMgr14;
extern GCDeviceFontTH* g_pkFontMgr16;
extern GCDeviceFontTH* g_pkFontMgrTitle;
extern GCDeviceFontTH* g_pkFontMgrOutline;
extern GCDeviceFontTH* g_pkFontMgrOutlineSmall;
extern GCDeviceFontTH* g_pkFontMgrOutline14;
extern GCDeviceFontTH* g_pkFontMgrOutline16;
extern GCDeviceFontTH* g_pkFontMgrOutlineTitle;

extern GCDeviceFontTH* g_pkFontMgr_original;
extern GCDeviceFontTH* g_pkFontMgrSmall_original;
extern GCDeviceFontTH* g_pkFontMgr14_original;
extern GCDeviceFontTH* g_pkFontMgr16_original;
extern GCDeviceFontTH* g_pkFontMgrTitle_original;
extern GCDeviceFontTH* g_pkFontMgrOutlineSmall_original;
extern GCDeviceFontTH* g_pkFontMgrOutline_original;
extern GCDeviceFontTH* g_pkFontMgrOutline14_original;
extern GCDeviceFontTH* g_pkFontMgrOutline16_original;
extern GCDeviceFontTH* g_pkFontMgrOutlineTitle_original;

extern GCDeviceFontTH* g_pkFontMgr_mid_scale;
extern GCDeviceFontTH* g_pkFontMgrSmall_mid_scale;
extern GCDeviceFontTH* g_pkFontMgr14_mid_scale;
extern GCDeviceFontTH* g_pkFontMgr16_mid_scale;
extern GCDeviceFontTH* g_pkFontMgrTitle_mid_scale;
extern GCDeviceFontTH* g_pkFontMgrOutline_mid_scale;
extern GCDeviceFontTH* g_pkFontMgrOutlineSmall_mid_scale;
extern GCDeviceFontTH* g_pkFontMgrOutline14_mid_scale;
extern GCDeviceFontTH* g_pkFontMgrOutline16_mid_scale;
extern GCDeviceFontTH* g_pkFontMgrOutlineTitle_mid_scale;
#else
extern GCDeviceFont* g_pkFontMgr;
extern GCDeviceFont* g_pkFontMgrSmall;
extern GCDeviceFont* g_pkFontMgr14;
extern GCDeviceFont* g_pkFontMgr16;
extern GCDeviceFont* g_pkFontMgrTitle;
extern GCDeviceFont* g_pkFontMgrOutline;
extern GCDeviceFont* g_pkFontMgrOutlineSmall;
extern GCDeviceFont* g_pkFontMgrOutline14;
extern GCDeviceFont* g_pkFontMgrOutline16;
extern GCDeviceFont* g_pkFontMgrOutlineTitle;

extern GCDeviceFont* g_pkFontMgr_original;
extern GCDeviceFont* g_pkFontMgrSmall_original;
extern GCDeviceFont* g_pkFontMgr14_original;
extern GCDeviceFont* g_pkFontMgr16_original;
extern GCDeviceFont* g_pkFontMgrTitle_original;
extern GCDeviceFont* g_pkFontMgrOutline_original;
extern GCDeviceFont* g_pkFontMgrOutlineSmall_original;
extern GCDeviceFont* g_pkFontMgrOutline14_original;
extern GCDeviceFont* g_pkFontMgrOutline16_original;
extern GCDeviceFont* g_pkFontMgrOutlineTitle_original;

extern GCDeviceFont* g_pkFontMgr_mid_scale;
extern GCDeviceFont* g_pkFontMgrSmall_mid_scale;
extern GCDeviceFont* g_pkFontMgr14_mid_scale;
extern GCDeviceFont* g_pkFontMgr16_mid_scale;
extern GCDeviceFont* g_pkFontMgrTitle_mid_scale;
extern GCDeviceFont* g_pkFontMgrOutline_mid_scale;
extern GCDeviceFont* g_pkFontMgrOutlineSmall_mid_scale;
extern GCDeviceFont* g_pkFontMgrOutline14_mid_scale;
extern GCDeviceFont* g_pkFontMgrOutline16_mid_scale;
extern GCDeviceFont* g_pkFontMgrOutlineTitle_mid_scale;
#endif
#endif // _FONTMANAGER_H_
