// stdafx.h : 잘 변경되지 않고 자주 사용하는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows 헤더에서 거의 사용되지 않는 내용을 제외시킵니다.
#endif

// 아래 지정된 플랫폼보다 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0603		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT 0x0603		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0603 // Windows Me 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 4.0 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0603	// IE 5.0 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소
#include <afxext.h>         // MFC 익스텐션
#include <afxdisp.h>        // MFC 자동화 클래스

#include <afxdtctl.h>		// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>


#include "../stdafx_stl.h"
#include "../stdafx_boost.h"
#include <assert.h>


#include <windows.h>
#include <windowsx.h>

#include <DXErr.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>
#include <d3dx9math.h>
#include "d3denumeration.h"
#include "D3DApp.h"
#include "D3DFont.h"
//#include "D3DUtil.h"

#include "DIUtil.h"
#include "DMUtil.h"
#include "DXUtil.h"
#include "D3DRes.h"
#include <tchar.h>
#include <cmath>
#include <stdio.h>
#include <strsafe.h>

// lua
#include "../GCLuaManager/KLuabinder.h"
#include "../GCLuaManager/KLuaManager.h"


#include "../ProtectedType_new.h"
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
#include "../kom/manager.h"


#include <KNCSingleton.h>



//#include <windowsx.h>
//#include <basetsd.h>
//#include <cmath>
//#include <stdio.h>
//#include <D3DX9.h>
//#include <DXErr9.h>
//#include <tchar.h>
//#include <dinput.h>
//#include <d3d9types.h>
//#include "D3DApp.h"
//#include "D3DFont.h"
//#include "D3DUtil.h"
//#include "DIUtil.h"
//#include "DMUtil.h"
//#include "DXUtil.h"
//#include "resource.h"
//#include "D3DRes.h"
//#include <algorithm>

#define QS_FPS_LIMIT        (55)
#define QS_ELAPSED_TIME     (1.0f/QS_FPS_LIMIT)
#define WM_CLICK_TOOL_BUTTON    WM_USER+10001
#define WM_MONSTER_ADD_WINDOW   WM_USER+10002
#define WM_CHANGESEQUENCE       WM_USER+10003
#define WM_MENU_ENABLE          WM_USER+10004
#define WM_CHANG_DIALOG         WM_USER+10005
#define WM_SHIFT_SELECT         WM_USER+10006
#define WM_ADDCHILD_ENTER       WM_USER+10007
#define WM_ADDSIBLING_SPACEBAR  WM_USER+10008
#define WM_ADDCHILDQ            WM_USER+10009
#define WM_ADDSIBLINGQ          WM_USER+10010
#define WM_DELETESUBTREE        WM_USER+10011
#define WM_MOVESIBLING          WM_USER+10012
#define WM_ACTIVE_INFOSTRUCT    WM_USER+10013
#define WM_SAVE_TO_FILE         WM_USER+10014
#define WM_SAVE_TO_FILE_FULL    WM_USER+10015
#define WM_LOAD_FROM_FILE       WM_USER+10016
#define WM_LOAD_FROM_FILE_FULL  WM_USER+10017
#define WM_NEW_TREE             WM_USER+10018
#define WM_CUT_TREE             WM_USER+10019
#define WM_COPY_TREE            WM_USER+10020
#define WM_PASTE_TREE           WM_USER+10021
#define WM_LOAD_TOOL_ICON       WM_USER+10022
#define WM_CHAGNE_TREE_FOCUS    WM_USER+10023
#define WM_SELECT_TRIGGER       WM_USER+10024
#define WM_CHANGED_TREE_LABEL   WM_USER+10025
#define WM_TREE_ITEM_DCLICK     WM_USER+10026
#define WM_TREE_NMCLICK         WM_USER+10027
#define WM_TREE_SELCHANGE       WM_USER+10028
#define WM_CLICK_TOOL_TYPE      WM_USER+10029
#define WM_PLAYER_ADD_WINDOW    WM_USER+10030
#define WM_MONSTER_SEL_WINDOW   WM_USER+10031
#define WM_AREA_SEL_WINDOW      WM_USER+10032
#define WM_OBJECT_ADD_WINDOW    WM_USER+10033
#define WM_TREE_CUSTOMMDRAW     WM_USER+10034
#define WM_TREE_REDRAW_TEXT_TO_SELITEM   WM_USER+10035
#define WM_ADD_EVENT            WM_USER+10036
#define WM_ADD_CONDITION        WM_USER+10037
#define WM_ADD_ACTION           WM_USER+10038
#define WM_MODIFY_EVENT         WM_USER+10039
#define WM_MODIFY_CONDITION     WM_USER+10040
#define WM_MODIFY_ACTION        WM_USER+10041
#define WM_MONSEL_FUNCTION      WM_USER+10042
#define WM_AREASEL_FUNCTION     WM_USER+10043
#define WM_GATEOBJECT_ADD_WINDOW    WM_USER+10044
#define WM_FOCUS_WINDOW         WM_USER+10045
#define WM_LOAD_FILE_FROM_FILE_LIST WM_USER+10046
#define WM_SAVE_SERVER_SCRIPT_FROM_FILE_LIST WM_USER+10047
#define WM_POSASEL_FUNCTION     WM_USER+10048
#define WM_POS_SEL_WINDOW       WM_USER+10049
#define WM_DAMAGEOBJECT_ADD_WINDOW  WM_USER+10050

#define STR_QUEST           L"Quest"
#define STR_STAGE           L"Stage"
#define STR_SEQUENCE        L"Sequence"
#define STR_STAGELV         L"StageLv"
#define STR_STAGEMON        L"Stage_mon"

#define MAX_SLOT_MONSTER          15
#define MAX_SLOT_OBJECT           15
#define MAX_SLOT_GATEOBJECT       15
#define MAX_SLOT_PLAYER           6

#define FOOTHOLDSIZE_ON_MAP       (20.0f)

#define NUM_OF_LAYER    14
#define LAST_DEPH       -50.0f

void SetWindowTextToEdit(CEdit *pEdit, int value);
void SetWindowTextToEdit(CEdit *pEdit, float value);
void GetWindowTextToEdit(CEdit *pEdit, int *value);
void GetWindowTextToEdit(CEdit *pEdit, float *value);
void HeapChk(CString strText, int iIndex);
void strncpy_lastNull(char *pDest, char *pSource, int iSize);

POINT Convert_3DTo2D_Point( LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3 v );

extern char g_Path[MAX_PATH];


