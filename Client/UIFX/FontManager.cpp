#include "stdafx.h"
#include "FontManager.h"

#if defined( _THAI_LANG )	//태국은 Tahoma체를 사용합니다.
GCDeviceFontTH* g_pkFontMgr               = NULL;
GCDeviceFontTH* g_pkFontMgrSmall		  = NULL;
GCDeviceFontTH* g_pkFontMgr14             = NULL;
GCDeviceFontTH* g_pkFontMgr16             = NULL;
GCDeviceFontTH* g_pkFontMgrTitle          = NULL;
GCDeviceFontTH* g_pkFontMgrOutline        = NULL;
GCDeviceFontTH* g_pkFontMgrOutlineSmall		  = NULL;
GCDeviceFontTH* g_pkFontMgrOutline14      = NULL;
GCDeviceFontTH* g_pkFontMgrOutline16      = NULL;
GCDeviceFontTH* g_pkFontMgrOutlineTitle   = NULL;
GCDeviceFontTH* g_pkFontMgr_original				 = NULL;
GCDeviceFontTH* g_pkFontMgrSmall_original				 = NULL;
GCDeviceFontTH* g_pkFontMgr14_original             = NULL;
GCDeviceFontTH* g_pkFontMgr16_original             = NULL;
GCDeviceFontTH* g_pkFontMgrTitle_original          = NULL;
GCDeviceFontTH* g_pkFontMgrOutline_original        = NULL;
GCDeviceFontTH* g_pkFontMgrOutlineSmall_original        = NULL;
GCDeviceFontTH* g_pkFontMgrOutline14_original      = NULL;
GCDeviceFontTH* g_pkFontMgrOutline16_original      = NULL;
GCDeviceFontTH* g_pkFontMgrOutlineTitle_original   = NULL;

//여긴 게임에 사용되는 중간 Scale
GCDeviceFontTH* g_pkFontMgr_mid_scale				= NULL;
GCDeviceFontTH* g_pkFontMgrSmall_mid_scale				= NULL;
GCDeviceFontTH* g_pkFontMgr14_mid_scale             = NULL;
GCDeviceFontTH* g_pkFontMgr16_mid_scale             = NULL;
GCDeviceFontTH* g_pkFontMgrTitle_mid_scale          = NULL;
GCDeviceFontTH* g_pkFontMgrOutline_mid_scale        = NULL;
GCDeviceFontTH* g_pkFontMgrOutlineSmall_mid_scale				= NULL;
GCDeviceFontTH* g_pkFontMgrOutline14_mid_scale      = NULL;
GCDeviceFontTH* g_pkFontMgrOutline16_mid_scale      = NULL;
GCDeviceFontTH* g_pkFontMgrOutlineTitle_mid_scale   = NULL;
#else
GCDeviceFont* g_pkFontMgr               = NULL;
GCDeviceFont* g_pkFontMgrSmall               = NULL;
GCDeviceFont* g_pkFontMgr14             = NULL;
GCDeviceFont* g_pkFontMgr16             = NULL;
GCDeviceFont* g_pkFontMgrTitle          = NULL;
GCDeviceFont* g_pkFontMgrOutline        = NULL;
GCDeviceFont* g_pkFontMgrOutlineSmall               = NULL;
GCDeviceFont* g_pkFontMgrOutline14      = NULL;
GCDeviceFont* g_pkFontMgrOutline16      = NULL;
GCDeviceFont* g_pkFontMgrOutlineTitle   = NULL;
GCDeviceFont* g_pkFontMgr_original				 = NULL;
GCDeviceFont* g_pkFontMgrSmall_original               = NULL;
GCDeviceFont* g_pkFontMgr14_original             = NULL;
GCDeviceFont* g_pkFontMgr16_original             = NULL;
GCDeviceFont* g_pkFontMgrTitle_original          = NULL;
GCDeviceFont* g_pkFontMgrOutline_original        = NULL;
GCDeviceFont* g_pkFontMgrOutlineSmall_original               = NULL;
GCDeviceFont* g_pkFontMgrOutline14_original      = NULL;
GCDeviceFont* g_pkFontMgrOutline16_original      = NULL;
GCDeviceFont* g_pkFontMgrOutlineTitle_original   = NULL;
GCDeviceFont* g_pkFontMgr_mid_scale				 = NULL;
GCDeviceFont* g_pkFontMgrSmall_mid_scale               = NULL;
GCDeviceFont* g_pkFontMgr14_mid_scale             = NULL;
GCDeviceFont* g_pkFontMgr16_mid_scale             = NULL;
GCDeviceFont* g_pkFontMgrTitle_mid_scale          = NULL;
GCDeviceFont* g_pkFontMgrOutline_mid_scale        = NULL;
GCDeviceFont* g_pkFontMgrOutlineSmall_mid_scale               = NULL;
GCDeviceFont* g_pkFontMgrOutline14_mid_scale      = NULL;
GCDeviceFont* g_pkFontMgrOutline16_mid_scale      = NULL;
GCDeviceFont* g_pkFontMgrOutlineTitle_mid_scale   = NULL;
#endif
