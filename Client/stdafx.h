#pragma once



#define DIRECTX_VERSIONUP_2009_AUG
#define _USE_MATH_DEFINES
#undef _WIN32_IE
#undef WINVER
#undef _WIN32_WINDOWS
#undef _WIN32_WINNT
#define WINVER 0x0601
#define _WIN32_WINDOWS 0x0601
#define _WIN32_WINNT 0x0601

#include <vector>
#include <string>
#include <WinSock2.h>
#include "stdafx_stl.h"
#include "stdafx_boost.h"

#include "define.h"
#include "PrimitiveTypedef.h"

#include "GCEnum.h"
#include "P2P.h"
#include "UserPacket.h"
#include "GCItem.h"
#include "KDSound.h"

#include <math.h>
#include <strsafe.h>
// assert
#include "KAssert.h"
#include <assert.h>

// lua
#include "KLuabinder.h"
#include "KLuaManager.h"
#include "KGCLuabinder.h"

#include "KGCStringLoader.h"

#include <InitGuid.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9math.h>
#include <windows.h>

#include <iostream>
#include <stdexcept>
#include <stdio.h>


//#include "./MassFileLib/GCMassFileManager.h"
#include "./kom/manager.h"
#include "./GCUtil/GCUtil.h"
#include "Vertex.h"
#include "./GCDeviceLib/GCDeviceManager.h"
#include "./GCDeviceLib/GCDynamicVBManager.h"
#include "./GCUtil/MD5/MD5Checksum.h"

// uifx
#include "stdafx_ui.h"

#include "KBrowserWrapper.h"

// global's
#include "GCGlobal.h"
#include "GCGlobalFunction.h"

// GCUI
#include "GCUI/GCUIScene.h"
#include "GCDeviceManager2.h"
#include "./uifx/D3DUIManager.h"

//Loja Heroica EP2
#include "GCUI/KGCHeroDungeonInfoListS6.h"
#include "GCUI/KGCHeroDungeonInfoS6.h"
#include "GCUI/KGCHeroDungeonOpentimeBoxS6.h"
#include "GCUI/KGCHeroDungeonUpgradePanel.h"
#include "GCUI/KGCHeroDungeonUpgradeSidePanel.h"
#include "GCUI/KGCHeroDungeonUpgradeSlot.h"
#include "GCUI/KGCHeroDungeonRewardDlg.h"
#include "GCUI/KGCHeroDungeonRewardItemS6.h"

//Shop
#include "GCUI/KGCHeroDungeonShopItemSlot.h"
#include "GCUI/KGCHeroDungeonShopS6Dlg.h"
#include "GCUI/KGCHeroDungeonShopPanel.h"
#include "GCUI/KGCHeroDungeonShopSidePanel.h"
#include "GCUI/KGCHeroDungeonUpgradePanel.h"

//PERFORMANCE
#include "KGCPerformanceCounter.h"

// CrashReport
#include "BugTrap.h"
#include "BTTrace.h"
#include "KncCrashRpt.h"
#include "KGCMultipleLanguages.h"


//ATL
#include "atlconv.h"
#include "atlcore.h"
#include "StlFunctions.h"

#define SAFE_REMOVE(obj) if(obj) g_RenderManager->RemoveObject( obj );
#define SAFE_REMOVE_DELETE(obj) if (obj) { g_RenderManager->RemoveObject( obj ); SAFE_DELETE(obj); obj = NULL; }



#include "UnitTest.h"
#include "profile.h"

#if !defined( _LOADING_TIMECHECK_ )
#define MAKE_LOAD_PROFILE
#endif
