#include "stdafx.h"
#ifndef STRICT
#define STRICT
#endif
#include <windowsx.h>
#include "MyD3D.h"
#include "./KncP2PLib/KncP2P.h"
#include "KDInput.h"
#include "KSingleton.h"
#include "Controls.h"
#include "dxutil.h"
#include "Square/GCSquare.h"
#include <chrono>

static CD3DApplication* g_pD3DApp = NULL;
static const float      FPS = GC_ELAPSED_TIME;

//-----------------------------------------------------------------------------
// Name: CD3DApplication()
// Desc: Constructor
//-----------------------------------------------------------------------------
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPDIRECT3D9       g_pD3D = NULL;
GCDeviceShader* g_pRenderState = NULL;


CD3DApplication::CD3DApplication()
{
	g_pD3DApp = this;
	m_dwNumAdapters = 0;
	m_dwAdapter = 0L;
	m_pD3D = NULL;
	m_pd3dDevice = NULL;
	m_hWnd = NULL;
	m_hWndFocus = NULL;
	m_hMenu = NULL;
	m_bActive = FALSE;
	m_bReady = FALSE;
	m_bHasFocus = FALSE;
	m_bWebBrowser = FALSE;
	m_dwCreateFlags = 0L;

	IsPrimaryBuffer = true;

	m_bKeepFPS60 = TRUE;
	m_bFrameMoving = TRUE;
	m_bSingleStep = FALSE;
	m_FPS = 0;
	m_strDeviceStats[0] = _T('\0');
	m_strFrameStats[0] = _T('\0');
	m_fTime = 0;
	m_fElapsedTime = 0;
	m_IsFirstStart = true;
	m_strWindowTitle = _T("D3D9 Application");
	m_bUseDepthBuffer = FALSE;
	m_dwMinDepthBits = 16;
	m_dwMinStencilBits = 0;
	m_uiPacketPerSecond = 0;
	m_bShowCursorWhenFullscreen = FALSE;
	timeBeginPeriod(1);

	DXUtil_Timer(TIMER_RESET);

	m_bClipCursorWhenFullscreen = FALSE;
	m_bFrameCheck = false;

	int iLocalWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int iLocalHeight = ::GetSystemMetrics(SM_CYSCREEN);

	//  1024, 768보다 작을때 작게 그린다.
	if (iLocalWidth < 1024 || iLocalHeight < 768)
	{
		m_dwCreationWidth = GC_OLD_SCREEN_WIDTH;
		m_dwCreationHeight = GC_OLD_SCREEN_HEIGHT;
	}
	else
	{
		m_dwCreationWidth = GC_SCREEN_WIDTH;
		m_dwCreationHeight = GC_SCREEN_HEIGHT;
	}
}

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_pD3DApp)
		return g_pD3DApp->MsgProc(hWnd, uMsg, wParam, lParam);
	else
		return E_FAIL;
}




//-----------------------------------------------------------------------------
// Name: Create()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Create(HINSTANCE hInstance)
{
	//Pause(TRUE);
	HRESULT hr;
	m_hInstance = hInstance;

	// Create the Direct3D object
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DX_RUNTIME_LOAD1).c_str(),
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DX_RUNTIME_LOAD2).c_str(),
			KMsgBox::MBOX_OK);
		return D3DAPPERR_NODIRECT3D;
	}
	g_pD3D = m_pD3D;

	// 옵션 로딩
	g_MyD3D->m_KGCOption.Load_Option();

	// Build a list of Direct3D adapters, modes and devices. The
	// ConfirmDevice() callback is used to confirm that only devices that
	// meet the app's requirements are considered.
	if (FAILED(hr = BuildDeviceList()))
	{
		switch (hr)
		{
		case D3DAPPERR_NOCOMPATIBLEDEVICES:
		{
			KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_NOCOMPATIBLEDEV1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_NOCOMPATIBLEDEV2).c_str(),
				KMsgBox::MBOX_OK);
			break;
		}
		case D3DAPPERR_HALNOTCOMPATIBLE:
		{
			D3DADAPTER_IDENTIFIER9 identifier = { 0, };
			m_pD3D->GetAdapterIdentifier(m_dwAdapter, 0, &identifier);
			KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_HALNOTCOMPATIBLE).c_str(),
				GCUTIL_STR::GCStrCharToWide(identifier.Description),
				KMsgBox::MBOX_OK);
			break;
		}
		}
		SAFE_RELEASE(m_pD3D);
		return hr;
	}

	g_MyD3D->m_KGCOption.InitResolutionOption();

	// 사내 테스트용일 때는 중복 실행을 체크하지 않는다.
#if !defined(_DISABLE_ONLY_ONE_EXECUTE_) 
	{
		hDuplicateWnd = ::FindWindowW(L"D3D Window", m_strWindowTitle);

		if (hDuplicateWnd != NULL)
		{
			KMsgBox::Close();
			KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_ALREADY_EXECUTE1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_ALREADY_EXECUTE2).c_str(),
				KMsgBox::MBOX_OK);
			return E_FAIL;
		}
	}
#endif // !_DISABLE_ONLY_ONE_EXECUTE_

	// Unless a substitute hWnd has been specified, create a window to
	// render into
	if (m_hWnd == NULL)
	{
		// Register the windows class
		WNDCLASS wndClass = { 0, WndProc, 0, 0, hInstance,
							  LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON)),
							  LoadCursor(hInstance, IDC_ARROW),
							  (HBRUSH)GetStockObject(WHITE_BRUSH),
							  NULL, _T("D3D Window") };

		RegisterClass(&wndClass);

		// Set the window's initial style
		m_dwWindowStyle = WS_POPUP | WS_CLIPCHILDREN;//|WS_THICKFRAME; // 크기 조정 불가능하게 함.

		// Set the window's initial width
		RECT rc;
		SetRect(&rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight);
		AdjustWindowRect(&rc, m_dwWindowStyle, FALSE);


		int iScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int iScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

		// Create the render window
		m_hWnd = CreateWindowEx(WS_EX_TOPMOST, L"D3D Window", m_strWindowTitle, m_dwWindowStyle,
			(iScreenWidth - (rc.right - rc.left)) / 2, (iScreenHeight - (rc.bottom - rc.top)) / 2,
			(rc.right - rc.left), (rc.bottom - rc.top), 0L,
			NULL,
			hInstance, 0L);

	}



	KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_INFO_WAIT_CONNECT1).c_str(),
		g_pkStrLoader->GetString(STR_ID_APPMSG_INFO_WAIT_CONNECT2).c_str(),
		KMsgBox::MBOX_DEFAULT);

	// device window.  If not, use the device window as the focus window.
	if (m_hWndFocus == NULL) m_hWndFocus = m_hWnd;

	// Save window properties
	m_dwWindowStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	GetWindowRect(m_hWnd, &m_rcWindowBounds);
	GetClientRect(m_hWnd, &m_rcWindowClient);

	if (IsWindowMode())
	{
		POINT pt = { m_rcWindowClient.left, m_rcWindowClient.top, };
		::ClientToScreen(m_hWnd, &pt);
		W_X = pt.x;
		W_Y = pt.y;
	}
	else
	{
		W_X = 0;
		W_Y = 0;
	}

	// Initialize the app's custom scene stuff
	if (S_OK != (hr = OneTimeSceneInit()))
	{
		SAFE_RELEASE(m_pD3D);
		KMsgBox::Close();
		if (hr == E_FAIL)
		{
			KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_CONNECT_FAIL1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_CONNECT_FAIL2).c_str(),
				KMsgBox::MBOX_OK);
		}
		else if (hr == E_ABORT)
		{
			KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_WEB_BROWSER_INIT1).c_str(),
				g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_WEB_BROWSER_INIT2).c_str(),
				KMsgBox::MBOX_OK);
		}
		return E_FAIL;//DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
	}
	KMsgBox::Close();

	OnCreateDevice(m_hWnd);

#if defined(USE_JOYSTICK)
	if (!g_pJoystick)
		g_MyD3D->m_KGCOption.SetJoystic(false);
#endif

	// Initialize the 3D environment for the app
	if (FAILED(hr = Initialize3DEnvironment()))
	{
		SAFE_RELEASE(m_pD3D);
		KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DISPLAY_SETTING1).c_str(),
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_DISPLAY_SETTING2).c_str(),
			KMsgBox::MBOX_OK);
		return E_FAIL;
		//return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
	}

	DXUtil_Timer(TIMER_START);

	// The app is ready to go
	m_bReady = TRUE;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SortModesCallback()
// Desc: Callback function for sorting display modes (used by BuildDeviceList).
//-----------------------------------------------------------------------------
static int SortModesCallback(const VOID* arg1, const VOID* arg2)
{
	D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
	D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

	if (p1->Format > p2->Format)   return -1;
	if (p1->Format < p2->Format)   return +1;
	if (p1->Width < p2->Width)    return -1;
	if (p1->Width > p2->Width)    return +1;
	if (p1->Height < p2->Height)   return -1;
	if (p1->Height > p2->Height)   return +1;

	return 0;
}




//-----------------------------------------------------------------------------
// Name: BuildDeviceList()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::BuildDeviceList()
{
	const DWORD dwNumDeviceTypes = 3;                                   // 디바이스 타입은 2가지인다.
	const TCHAR* strDeviceDescs[] = { _T("HAL"), _T("REF"), _T("SW") };           // 하드웨어와 소프트웨어 2가지...
	const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF, D3DDEVTYPE_SW };

	BOOL bHALExists = FALSE;                                            // 가속기가 존재하지 않음
	BOOL bHALIsWindowedCompatible = FALSE;                              // 윈도우 모드를 지원하지 않음
	BOOL bHALIsDesktopCompatible = FALSE;
	BOOL bHALIsSampleCompatible = FALSE;

	// Loop through all the adapters on the system (usually, there's just one
	// unless more than one graphics card is present).
	// 시스템의 모든 adapter를 찾아본다.(보통 그래픽 카드가 1개라면 1개만 존재한다)
	//{{ Changed by parkch: 2003-08-26 오후 5:15:49
	for (UINT iAdapter = 0; iAdapter < /*m_pD3D->GetAdapterCount()*/1; iAdapter++)
		//}} Changed by parkch: 2003-08-26 오후 5:15:49
	{
		// Fill in adapter info
		// adapter info에 채워넣기
		D3DAdapterInfo* pAdapter = &m_Adapters[m_dwNumAdapters];
		m_pD3D->GetAdapterIdentifier(iAdapter, D3DENUM_WHQL_LEVEL, &pAdapter->d3dAdapterIdentifier);
		m_pD3D->GetAdapterDisplayMode(iAdapter, &pAdapter->d3ddmDesktop);
		pAdapter->dwNumDevices = 0;
		pAdapter->dwCurrentDevice = 0;

		// Enumerate all display modes on this adapter
		D3DDISPLAYMODE modes[100];
		D3DFORMAT      formats[20];
		DWORD dwNumFormats = 0;
		DWORD dwNumModes = 0;
		// Add the adapter's current desktop format to the list of formats
		formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

		for (int i = 0; i < 6; i++)
		{
			DWORD dwNumAdapterModes = m_pD3D->GetAdapterModeCount(iAdapter, (D3DFORMAT)(20 + i)); // D3DFMT_R8G8B8 부터 D3DFMT_A1R5G5B5 까지 테스트;;
			for (UINT iMode = 0; iMode < dwNumAdapterModes; iMode++)
			{
				// Get the display mode attributes
				D3DDISPLAYMODE DisplayMode;
				m_pD3D->EnumAdapterModes(iAdapter, (D3DFORMAT)(20 + i), iMode, &DisplayMode);

				// 해상도 확장을 위해서 해상도 모드에서 제외된 것들을 추가해 줍니다.				
				if ((DisplayMode.Width < 800))
					continue;

				if ((DisplayMode.Height < 600))
					continue;

				// 가로 세로 비율이 안맞는 녀석들 제외
				//if (DisplayMode.Width*100/DisplayMode.Height != 800*100/600)
				//	continue;
#ifdef ENABLE_RESOLUTION
				if (g_MyD3D->m_KGCOption.IsEnableResolution(DisplayMode.Width, DisplayMode.Height) == false)
					continue;
#endif

				// Check if the mode already exists (to filter out refresh rates)
				DWORD m;
				for (m = 0L; m < dwNumModes; m++)
				{
					if ((modes[m].Width == DisplayMode.Width) &&
						(modes[m].Height == DisplayMode.Height))
					{
						if (KD3DDeviceState::GetAlphaBits(modes[m].Format) < KD3DDeviceState::GetAlphaBits(DisplayMode.Format))
						{
							modes[m].Width = DisplayMode.Width;
							modes[m].Height = DisplayMode.Height;
							modes[m].Format = DisplayMode.Format;
							modes[m].RefreshRate = DisplayMode.RefreshRate;
						}
						break;
					}
				}

				// If we found a new mode, add it to the list of modes
				if (m == dwNumModes)
				{
					modes[dwNumModes].Width = DisplayMode.Width;
					modes[dwNumModes].Height = DisplayMode.Height;
					modes[dwNumModes].Format = DisplayMode.Format;
					modes[dwNumModes].RefreshRate = DisplayMode.RefreshRate;
					dwNumModes++;

					// Check if the mode's format already exists
					DWORD f;
					for (f = 0; f < dwNumFormats; f++)
					{
						if (DisplayMode.Format == formats[f])
							break;
					}

					// If the format is new, add it to the list
					if (f == dwNumFormats)
						formats[dwNumFormats++] = DisplayMode.Format;
				}
			}
		}

		// Sort the list of display modes (by format, then width, then height)
		qsort(modes, dwNumModes, sizeof(D3DDISPLAYMODE), SortModesCallback);

		// Add devices to adapter
		for (UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++)
		{
			// Fill in device info
			D3DDeviceInfo* pDevice;
			pDevice = &pAdapter->devices[pAdapter->dwNumDevices];
			pDevice->DeviceType = DeviceTypes[iDevice];
			m_pD3D->GetDeviceCaps(iAdapter, DeviceTypes[iDevice], &pDevice->d3dCaps);
			pDevice->strDesc = strDeviceDescs[iDevice];
			pDevice->dwNumModes = 0;
			pDevice->dwCurrentMode = 0;
			pDevice->bCanDoWindowed = FALSE;
			pDevice->bWindowed = FALSE;
			pDevice->MultiSampleTypeFullscreen = D3DMULTISAMPLE_NONE;
			pDevice->MultiSampleTypeWindowed = D3DMULTISAMPLE_NONE;

			// Examine each format supported by the adapter to see if it will
			// work with this device and meets the needs of the application.
			BOOL  bFormatConfirmed[20];
			DWORD dwBehavior[20];
			D3DFORMAT fmtDepthStencil[20];

			for (DWORD f = 0; f < dwNumFormats; f++)
			{
				bFormatConfirmed[f] = FALSE;
				fmtDepthStencil[f] = D3DFMT_UNKNOWN;

				// Skip formats that cannot be used as render targets on this device
				if (FAILED(m_pD3D->CheckDeviceType(iAdapter, pDevice->DeviceType,
					formats[f], formats[f], FALSE)))
					continue;

				if (pDevice->DeviceType == D3DDEVTYPE_HAL)
				{
					// This system has a HAL device
					bHALExists = TRUE;

					//if( pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED )
					{
						// HAL can run in a window for some mode
						bHALIsWindowedCompatible = TRUE;

						if (f == 0)
						{
							// HAL can run in a window for the current desktop mode
							bHALIsDesktopCompatible = TRUE;
						}
					}
				}

				// Confirm the device/format for HW vertex processing
				if (pDevice->d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
				{
					if (pDevice->d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
					{
						dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
							D3DCREATE_PUREDEVICE;

						if (SUCCEEDED(ConfirmDevice(&pDevice->d3dCaps, dwBehavior[f],
							formats[f])))
							bFormatConfirmed[f] = TRUE;
					}

					if (FALSE == bFormatConfirmed[f])
					{
						dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

						if (SUCCEEDED(ConfirmDevice(&pDevice->d3dCaps, dwBehavior[f],
							formats[f])))
							bFormatConfirmed[f] = TRUE;
					}

					if (FALSE == bFormatConfirmed[f])
					{
						dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;

						if (SUCCEEDED(ConfirmDevice(&pDevice->d3dCaps, dwBehavior[f],
							formats[f])))
							bFormatConfirmed[f] = TRUE;
					}
				}

				// Confirm the device/format for SW vertex processing
				if (FALSE == bFormatConfirmed[f])
				{
					dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

					if (SUCCEEDED(ConfirmDevice(&pDevice->d3dCaps, dwBehavior[f],
						formats[f])))
						bFormatConfirmed[f] = TRUE;
				}

				// Find a suitable depth/stencil buffer format for this device/format
				if (bFormatConfirmed[f] && m_bUseDepthBuffer)
				{
					if (!FindDepthStencilFormat(iAdapter, pDevice->DeviceType,
						formats[f], &fmtDepthStencil[f]))
					{
						bFormatConfirmed[f] = FALSE;
					}
				}
			}

			// Add all enumerated display modes with confirmed formats to the
			// device's list of valid modes
			for (DWORD m = 0L; m < dwNumModes; m++)
			{
				for (DWORD f = 0; f < dwNumFormats; f++)
				{
					if (modes[m].Format == formats[f])
					{
						if (bFormatConfirmed[f] == TRUE)
						{
							// Add this mode to the device's list of valid modes
							pDevice->modes[pDevice->dwNumModes].Width = modes[m].Width;
							pDevice->modes[pDevice->dwNumModes].Height = modes[m].Height;
							pDevice->modes[pDevice->dwNumModes].Format = modes[m].Format;
							pDevice->modes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
							pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
							pDevice->dwNumModes++;

							if (pDevice->DeviceType == D3DDEVTYPE_HAL)
								bHALIsSampleCompatible = TRUE;
						}
					}
				}
			}

			// Select any 640x480 mode for default (but prefer a 16-bit mode)
			for (DWORD m = 0; m < pDevice->dwNumModes; m++)
			{
				if (pDevice->modes[m].Width == 800 && pDevice->modes[m].Height == 600) // 800 x 600 고정
				{
					pDevice->dwCurrentMode = m;
					pDevice->dwBasicMode = m;
					pDevice->dwDefaultMode = m;

					break;
				}

			}

			for (DWORD m = 0; m < pDevice->dwNumModes; m++)
			{

				if (pDevice->modes[m].Width == GC_SCREEN_WIDTH && pDevice->modes[m].Height == GC_SCREEN_HEIGHT)
					//}} Changed by parkch: 2004-06-09 오후 12:02:59
				{
					pDevice->dwDefaultMode = m;
					break;
				}
			}

			// Check if the device is compatible with the desktop display mode
			// (which was added initially as formats[0])
			//d3d9 에서는 모든 device 가 Windowmode 를 지원한다네요. -lychnis
			//if( bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) )
			{
				pDevice->bCanDoWindowed = TRUE;
				pDevice->bWindowed = TRUE;
			}

			// If valid modes were found, keep this device
			if (pDevice->dwNumModes > 0)
				pAdapter->dwNumDevices++;
		}

		// If valid devices were found, keep this adapter
		if (pAdapter->dwNumDevices > 0)
			m_dwNumAdapters++;
	}

	// Return an error if no compatible devices were found
	if (0L == m_dwNumAdapters)
		return D3DAPPERR_NOCOMPATIBLEDEVICES;

	if (FALSE == bHALExists) return D3DAPPERR_HALNOTCOMPATIBLE;

	for (DWORD a = 0; a < m_dwNumAdapters; a++)
	{
		for (DWORD d = 0; d < m_Adapters[a].dwNumDevices; d++)
		{
			//{{ Changed by parkch: 2003-08-26 오후 5:15:49
			// HAL이 지원되지 않으면 게임을 못하게 한다.
			if (m_Adapters[a].devices[d].DeviceType == D3DDEVTYPE_HAL)
			{
				if (m_Adapters[a].devices[d].bWindowed)
				{
					m_Adapters[a].dwCurrentDevice = d;
					m_dwAdapter = a;
					SetWindowMode(TRUE);
					Windowable = true;
					return S_OK;
				}
			}
			else break;
			//}} Changed by parkch: 2003-08-26 오후 5:15:49
		}
	}
	/**/
	//{{ Appended by parkch: 2003-08-26 오후 5:16:03
	// 창모드 렌더링을 지원하는 HAL Device를 찾지 못했다면
	// 창모드를 off한 상태로 Primary adapter를 사용한다.
	m_Adapters[D3DADAPTER_DEFAULT].dwCurrentDevice = 0; // HAL device
	m_dwAdapter = D3DADAPTER_DEFAULT;                   // Primary adapter
	SetWindowMode(FALSE);
	Windowable = false;                                // 창모드 off
	//}} Appended by parkch: 2003-08-26 오후 5:16:03

	//{{ Changed by parkch: 2003-08-26 오후 5:16:06
	return S_OK;
	//return D3DAPPERR_NOWINDOWABLEDEVICES;
	//}} Changed by parkch: 2003-08-26 오후 5:16:06    
	/**/
}




//-----------------------------------------------------------------------------
// Name: FindDepthStencilFormat()
// Desc: Finds a depth/stencil format for the given device that is compatible
//       with the render target format and meets the needs of the app.
//-----------------------------------------------------------------------------
BOOL CD3DApplication::FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType,
	D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat)
{
	if (m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D16)))
			{
				*pDepthStencilFormat = D3DFMT_D16;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D15S1)))
			{
				*pDepthStencilFormat = D3DFMT_D15S1;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24X8)))
			{
				*pDepthStencilFormat = D3DFMT_D24X8;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24S8)))
			{
				*pDepthStencilFormat = D3DFMT_D24S8;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24X4S4)))
			{
				*pDepthStencilFormat = D3DFMT_D24X4S4;
				return TRUE;
			}
		}
	}

	if (m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)))
		{
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D32)))
			{
				*pDepthStencilFormat = D3DFMT_D32;
				return TRUE;
			}
		}
	}

	return FALSE;
}








//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Initialize3DEnvironment()
{
	HRESULT hr;

	D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
	D3DDeviceInfo* pDeviceInfo = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
	D3DModeInfo* pModeInfo = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

	AdjustWindowForChange();

	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.Windowed = Windowable ? pDeviceInfo->bWindowed : FALSE;
	pDeviceInfo->bWindowed = m_d3dpp.Windowed;

	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.MultiSampleType = pDeviceInfo->bWindowed ? pDeviceInfo->MultiSampleTypeWindowed : pDeviceInfo->MultiSampleTypeFullscreen;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.EnableAutoDepthStencil = m_bUseDepthBuffer;
	m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
	m_d3dpp.hDeviceWindow = m_hWnd;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (IsWindowMode())
	{
		m_d3dpp.BackBufferWidth = m_rcWindowClient.right - m_rcWindowClient.left;
		m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
		m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
	}
	else
	{
		m_d3dpp.BackBufferWidth = pModeInfo->Width;
		m_d3dpp.BackBufferHeight = pModeInfo->Height;
		m_d3dpp.BackBufferFormat = pModeInfo->Format;
	}

	hr = m_pD3D->CreateDevice(m_dwAdapter, pDeviceInfo->DeviceType,
		m_hWndFocus, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &m_d3dpp,
		&m_pd3dDevice);

	if (SUCCEEDED(hr))
	{
		g_pd3dDevice = m_pd3dDevice;
		if (IsWindowMode())
		{
			SetWindowPos(m_hWnd, HWND_NOTOPMOST,
				m_rcWindowBounds.left, m_rcWindowBounds.top,
				(m_rcWindowBounds.right - m_rcWindowBounds.left),
				(m_rcWindowBounds.bottom - m_rcWindowBounds.top), 0);
		}
		m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);
		m_dwCreateFlags = pModeInfo->dwBehavior;

		LPDIRECT3DSURFACE9 pBackBuffer;
		m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		pBackBuffer->GetDesc(&m_d3dsdBackBuffer);
		pBackBuffer->Release();

		if (m_bClipCursorWhenFullscreen)
		{
			if (!IsWindowMode())
			{
				RECT rcWindow;
				GetWindowRect(m_hWnd, &rcWindow);
				ClipCursor(&rcWindow);
			}
			else
			{
				ClipCursor(NULL);
			}
		}

		if (!Windowable)
		{
			m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 10, 25, 102), 1.0f, 0L);
			if (D3D_OK == m_pd3dDevice->Present(NULL, NULL, NULL, NULL))
				IsPrimaryBuffer = !IsPrimaryBuffer;
		}

		hr = InitDeviceObjects();
		if (SUCCEEDED(hr))
		{
			m_bActive = TRUE;
			return S_OK;
		}
		InvalidateDeviceObjects();
		DeleteDeviceObjects();
		SAFE_RELEASE(m_pd3dDevice);
	}
	return hr;
}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Resize3DEnvironment()
{
	HRESULT hr;

	// Release all vidmem objects
	if (FAILED(hr = InvalidateDeviceObjects()))
	{
		return hr;
	}

	// Reset the device
	if (FAILED(hr = m_pd3dDevice->Reset(&m_d3dpp)))
	{
		return hr;
	}
	// Calling Reset or CreateDevice, Set IsPrimaryBuffer=true;
	IsPrimaryBuffer = true;

	// Store render target surface desc
	LPDIRECT3DSURFACE9 pBackBuffer;
	m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	pBackBuffer->GetDesc(&m_d3dsdBackBuffer);
	pBackBuffer->Release();

	// Set up the fullscreen cursor
	if (m_bShowCursorWhenFullscreen && !IsWindowMode())
	{
		HCURSOR hCursor;
#ifdef _WIN64
		hCursor = (HCURSOR)GetClassLongPtr(m_hWnd, GCLP_HCURSOR);
#else
		hCursor = (HCURSOR)GetClassLong(m_hWnd, GCL_HCURSOR);
#endif
		//D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, TRUE );
		m_pd3dDevice->ShowCursor(TRUE);
	}

	// Confine cursor to fullscreen window
	if (m_bClipCursorWhenFullscreen)
	{
		if (!IsWindowMode())
		{
			RECT rcWindow;
			GetWindowRect(m_hWnd, &rcWindow);
			ClipCursor(&rcWindow);
		}
		else
		{
			ClipCursor(NULL);
		}
	}

	// Initialize the app's device-dependent objects
	hr = RestoreDeviceObjects();
	if (FAILED(hr))
	{
		return hr;
	}

	// If the app is paused, trigger the rendering of the current frame
	if (FALSE == m_bFrameMoving)
	{
		m_bSingleStep = TRUE;
		DXUtil_Timer(TIMER_START);
		DXUtil_Timer(TIMER_STOP);
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ToggleFullScreen()
// Desc: Called when user toggles between fullscreen mode and windowed mode
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ToggleFullscreen()
{
	// Get access to current adapter, device, and mode
	D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
	D3DDeviceInfo* pDeviceInfo = &pAdapterInfo->devices[pAdapterInfo->dwCurrentDevice];
	D3DModeInfo* pModeInfo = &pDeviceInfo->modes[pDeviceInfo->dwCurrentMode];

	// Need device change if going windowed and the current device
	// can only be fullscreen
	if (!IsWindowMode() && !pDeviceInfo->bCanDoWindowed)
		return ForceWindowed();

	m_bReady = FALSE;

	// Toggle the windowed state
	SetWindowMode(!IsWindowMode());
	pDeviceInfo->bWindowed = IsWindowMode();

	// Prepare window for windowed/fullscreen change
	AdjustWindowForChange();

	// Set up the presentation parameters
	m_d3dpp.Windowed = pDeviceInfo->bWindowed;
	if (IsWindowMode())
		m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleTypeWindowed;
	else
		m_d3dpp.MultiSampleType = pDeviceInfo->MultiSampleTypeFullscreen;
	m_d3dpp.AutoDepthStencilFormat = pModeInfo->DepthStencilFormat;
	m_d3dpp.hDeviceWindow = m_hWnd;
	if (IsWindowMode())
	{
		int iScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int iScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

		g_MyD3D->m_dwCreationWidth = static_cast<DWORD>(GC_SCREEN_WIDTH * g_pGCDeviceManager->GetWindowScaleX());
		g_MyD3D->m_dwCreationHeight = static_cast<DWORD>(GC_SCREEN_HEIGHT * g_pGCDeviceManager->GetWindowScaleY());

		RECT rc;
		::SetRect(&rc, 0, 0, g_MyD3D->m_dwCreationWidth, g_MyD3D->m_dwCreationHeight);

		::SetWindowPos(g_MyD3D->m_hWnd, HWND_NOTOPMOST,
			(iScreenWidth - (rc.right - rc.left)) / 2,
			(iScreenHeight - (rc.bottom - rc.top)) / 2,
			(rc.right - rc.left),
			(rc.bottom - rc.top),
			SWP_SHOWWINDOW);

		RECT rcClientOld;
		rcClientOld = m_rcWindowClient;

		// Update window properties
		GetWindowRect(m_hWnd, &m_rcWindowBounds);
		GetClientRect(m_hWnd, &m_rcWindowClient);

		POINT pt = { m_rcWindowClient.left, m_rcWindowClient.top, };
		::ClientToScreen(m_hWnd, &pt);
		W_X = pt.x;
		W_Y = pt.y;


		m_d3dpp.BackBufferWidth = m_rcWindowClient.right - m_rcWindowClient.left;
		m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
		m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
	}
	else
	{
		/*m_d3dpp.BackBufferWidth  = pModeInfo->Width;
		m_d3dpp.BackBufferHeight = pModeInfo->Height;*/
		m_d3dpp.BackBufferWidth = pModeInfo->Width;
		m_d3dpp.BackBufferHeight = pModeInfo->Height;
		m_d3dpp.BackBufferFormat = pModeInfo->Format;
	}

	// Resize the 3D device
	if (FAILED(Resize3DEnvironment()))
	{
		if (IsWindowMode())
			return ForceWindowed();
		else
			return E_FAIL;
	}

	// When moving from fullscreen to windowed mode, it is important to
	// adjust the window size after resetting the device rather than
	// beforehand to ensure that you get the window size you want.  For
	// example, when switching from 640x480 fullscreen to windowed with
	// a 1000x600 window on a 1024x768 desktop, it is impossible to set
	// the window size to 1000x600 until after the display mode has
	// changed to 1024x768, because windows cannot be larger than the
	// desktop.
	if (IsWindowMode())
	{
		//     	SetWindowPos( m_hWnd, HWND_NOTOPMOST,
		//                      m_rcWindowBounds.left, m_rcWindowBounds.top,
		//                      ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
		//                      ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
		//                      SWP_SHOWWINDOW );
		RECT rc;
		SetRect(&rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight);
		//{{ Changed by parkch: 2004-07-16 오전 10:29:04
		AdjustWindowRect(&rc, m_dwWindowStyle/*|WS_CAPTION*/, FALSE);
		//}} Changed by parkch: 2004-07-16 오전 10:29:04
		SetWindowPos(m_hWnd, HWND_NOTOPMOST,
			(GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2,
			(rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);

		W_Width = m_rcWindowClient.right;
		W_Height = m_rcWindowClient.bottom;

		POINT pt = { m_rcWindowClient.left, m_rcWindowClient.top, };
		::ClientToScreen(m_hWnd, &pt);
		W_X = pt.x;
		W_Y = pt.y;

	}
	else {
		W_X = 0;
		W_Y = 0;
		W_Width = m_d3dpp.BackBufferWidth;
		W_Height = m_d3dpp.BackBufferHeight;
	}
	m_bReady = TRUE;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ForceWindowed()
// Desc: Switch to a windowed mode, even if that means picking a new device
//       and/or adapter
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::ForceWindowed()
{
	HRESULT hr;
	D3DAdapterInfo* pAdapterInfoCur = &m_Adapters[m_dwAdapter];
	D3DDeviceInfo* pDeviceInfoCur = &pAdapterInfoCur->devices[pAdapterInfoCur->dwCurrentDevice];
	BOOL bFoundDevice = FALSE;

	if (pDeviceInfoCur->bCanDoWindowed)
	{
		bFoundDevice = TRUE;
	}
	else
	{
		// Look for a windowable device on any adapter
		D3DAdapterInfo* pAdapterInfo;
		DWORD dwAdapter;
		D3DDeviceInfo* pDeviceInfo;
		DWORD dwDevice;
		for (dwAdapter = 0; dwAdapter < m_dwNumAdapters; dwAdapter++)
		{
			pAdapterInfo = &m_Adapters[dwAdapter];
			for (dwDevice = 0; dwDevice < pAdapterInfo->dwNumDevices; dwDevice++)
			{
				pDeviceInfo = &pAdapterInfo->devices[dwDevice];
				if (pDeviceInfo->bCanDoWindowed)
				{
					m_dwAdapter = dwAdapter;
					pDeviceInfoCur = pDeviceInfo;
					pAdapterInfo->dwCurrentDevice = dwDevice;
					bFoundDevice = TRUE;
					break;
				}
			}
			if (bFoundDevice)
				break;
		}
	}

	if (!bFoundDevice)
		return E_FAIL;

	pDeviceInfoCur->bWindowed = TRUE;
	SetWindowMode(TRUE);

	// Now destroy the current 3D device objects, then reinitialize

	m_bReady = FALSE;

	// Release all scene objects that will be re-created for the new device
	InvalidateDeviceObjects();
	DeleteDeviceObjects();

	// Release display objects, so a new device can be created
	if (m_pd3dDevice->Release() > 0L) {
		return D3DAPPERR_NONZEROREFCOUNT;//, MSGERR_APPMUSTEXIT );
	}

	// Create the new device
	if (FAILED(hr = Initialize3DEnvironment()))
		return DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT);
	m_bReady = TRUE;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Prepare the window for a possible change between windowed mode and
//       fullscreen mode.  This function is virtual and thus can be overridden
//       to provide different behavior, such as switching to an entirely
//       different window for fullscreen mode (as in the MFC sample apps).
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::AdjustWindowForChange()
{
	if (IsWindowMode())
	{
		static bool First = true;
		if (First) {
			SetWindowLong(m_hWnd, GWL_STYLE, m_dwWindowStyle | WS_VISIBLE);
			First = false;
		}
		else {
			//{{ Changed by parkch: 2004-07-16 오전 10:29:38
			SetWindowLong(m_hWnd, GWL_STYLE, m_dwWindowStyle | WS_VISIBLE/*|WS_CAPTION*/);
			//}} Changed by parkch: 2004-07-16 오전 10:29:38
			//{{ Appended by parkch: 2004-06-15 오전 10:33:21
			RECT rc;
			SetRect(&rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight);
			//{{ Changed by parkch: 2004-07-16 오전 10:29:53
			AdjustWindowRect(&rc, m_dwWindowStyle | WS_VISIBLE/*|WS_CAPTION*/, FALSE);
			//}} Changed by parkch: 2004-07-16 오전 10:29:53
			SetWindowPos(m_hWnd, HWND_NOTOPMOST,
				(GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2,
				(GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2,
				(rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);
			//}} Appended by parkch: 2004-06-15 오전 10:33:21
		}
	}
	else
	{
		// Set fullscreen-mode style
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE | WS_CLIPCHILDREN);
		//SetWindowLong( m_hWnd, GWL_STYLE, WS_OVERLAPPED|WS_SYSMENU|WS_VISIBLE );
		//SetWindowLong( m_hWnd, GWL_EXSTYLE, WS_EX_TOPMOST );
	}
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: UserSelectNewDevice()
// Desc: Displays a dialog so the user can select a new adapter, device, or
//       display mode, and then recreates the 3D environment if needed
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::UserSelectNewDevice()
{
	HRESULT hr;
	/*
	// Can't display dialogs in fullscreen mode
	if( m_bWindowed == FALSE )
	{
		if( FAILED( ToggleFullscreen() ) )
		{
			DisplayErrorMsg( D3DAPPERR_RESIZEFAILED, MSGERR_APPMUSTEXIT );
			return E_FAIL;
		}
	}

	// Prompt the user to change the mode
	if( IDOK != DialogBoxParam( (HINSTANCE)GetModuleHandle(NULL),
								MAKEINTRESOURCE(IDD_SELECTDEVICE), m_hWnd,
								SelectDeviceProc, (LPARAM)this ) )
		return S_OK;
	*/
	// Get access to the newly selected adapter, device, and mode
	DWORD dwDevice;
	dwDevice = m_Adapters[m_dwAdapter].dwCurrentDevice;
	SetWindowMode(m_Adapters[m_dwAdapter].devices[dwDevice].bWindowed);

	// Release all scene objects that will be re-created for the new device
	InvalidateDeviceObjects();
	DeleteDeviceObjects();

	// Release display objects, so a new device can be created
	if (m_pd3dDevice->Release() > 0L)
		return DisplayErrorMsg(D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT);

	// Inform the display class of the change. It will internally
	// re-create valid surfaces, a d3ddevice, etc.
	if (FAILED(hr = Initialize3DEnvironment()))
		return DisplayErrorMsg(hr, MSGERR_APPMUSTEXIT);

	// If the app is paused, trigger the rendering of the current frame
	if (FALSE == m_bFrameMoving)
	{
		m_bSingleStep = TRUE;
		DXUtil_Timer(TIMER_START);
		DXUtil_Timer(TIMER_STOP);
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: SelectDeviceProc()
// Desc: Windows message handling function for the device select dialog
//-----------------------------------------------------------------------------
INT_PTR CALLBACK CD3DApplication::SelectDeviceProc(HWND hDlg, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	// Get access to the UI controls
	HWND hwndAdapterList = GetDlgItem(hDlg, IDC_ADAPTER_COMBO);
	HWND hwndDeviceList = GetDlgItem(hDlg, IDC_DEVICE_COMBO);
	HWND hwndFullscreenModeList = GetDlgItem(hDlg, IDC_FULLSCREENMODES_COMBO);
	HWND hwndWindowedRadio = GetDlgItem(hDlg, IDC_WINDOW);
	HWND hwndFullscreenRadio = GetDlgItem(hDlg, IDC_FULLSCREEN);
	HWND hwndMultiSampleList = GetDlgItem(hDlg, IDC_MULTISAMPLE_COMBO);
	BOOL bUpdateDlgControls = FALSE;

	// Static state for adapter/device/mode selection
	static CD3DApplication* pd3dApp;
	static DWORD  dwOldAdapter, dwNewAdapter;
	static DWORD  dwOldDevice, dwNewDevice;
	static DWORD  dwOldMode, dwNewMode;
	static BOOL   bOldWindowed, bNewWindowed;
	static D3DMULTISAMPLE_TYPE OldMultiSampleTypeWindowed, NewMultiSampleTypeWindowed;
	static D3DMULTISAMPLE_TYPE OldMultiSampleTypeFullscreen, NewMultiSampleTypeFullscreen;

	// Working variables
	D3DAdapterInfo* pAdapter;
	D3DDeviceInfo* pDevice;

	// Handle the initialization message
	if (WM_INITDIALOG == msg)
	{
		// Old state
		pd3dApp = (CD3DApplication*)lParam;
		dwOldAdapter = pd3dApp->m_dwAdapter;
		pAdapter = &pd3dApp->m_Adapters[dwOldAdapter];

		dwOldDevice = pAdapter->dwCurrentDevice;
		pDevice = &pAdapter->devices[dwOldDevice];

		dwOldMode = pDevice->dwCurrentMode;
		bOldWindowed = pDevice->bWindowed;
		OldMultiSampleTypeWindowed = pDevice->MultiSampleTypeWindowed;
		OldMultiSampleTypeFullscreen = pDevice->MultiSampleTypeFullscreen;

		// New state is initially the same as the old state
		dwNewAdapter = dwOldAdapter;
		dwNewDevice = dwOldDevice;
		dwNewMode = dwOldMode;
		bNewWindowed = bOldWindowed;
		NewMultiSampleTypeWindowed = OldMultiSampleTypeWindowed;
		NewMultiSampleTypeFullscreen = OldMultiSampleTypeFullscreen;

		// Set flag to update dialog controls below
		bUpdateDlgControls = TRUE;
	}

	if (WM_COMMAND == msg)
	{
		// Get current UI state
		bNewWindowed = Button_GetCheck(hwndWindowedRadio);

		if (IDOK == LOWORD(wParam))
		{
			// Handle the case when the user hits the OK button. Check if any
			// of the options were changed
			if (dwNewAdapter != dwOldAdapter || dwNewDevice != dwOldDevice ||
				dwNewMode != dwOldMode || bNewWindowed != bOldWindowed ||
				NewMultiSampleTypeWindowed != OldMultiSampleTypeWindowed ||
				NewMultiSampleTypeFullscreen != OldMultiSampleTypeFullscreen)
			{
				pd3dApp->m_dwAdapter = dwNewAdapter;

				pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
				pAdapter->dwCurrentDevice = dwNewDevice;

				pAdapter->devices[dwNewDevice].dwCurrentMode = dwNewMode;
				pAdapter->devices[dwNewDevice].bWindowed = bNewWindowed;
				pAdapter->devices[dwNewDevice].MultiSampleTypeWindowed = NewMultiSampleTypeWindowed;
				pAdapter->devices[dwNewDevice].MultiSampleTypeFullscreen = NewMultiSampleTypeFullscreen;

				EndDialog(hDlg, IDOK);
			}
			else
				EndDialog(hDlg, IDCANCEL);

			return TRUE;
		}
		else if (IDCANCEL == LOWORD(wParam))
		{
			// Handle the case when the user hits the Cancel button
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		else if (CBN_SELENDOK == HIWORD(wParam))
		{
			if (LOWORD(wParam) == IDC_ADAPTER_COMBO)
			{
				dwNewAdapter = ComboBox_GetCurSel(hwndAdapterList);
				pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];

				dwNewDevice = pAdapter->dwCurrentDevice;
				dwNewMode = pAdapter->devices[dwNewDevice].dwCurrentMode;
				bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
			}
			else if (LOWORD(wParam) == IDC_DEVICE_COMBO)
			{
				pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];

				dwNewDevice = ComboBox_GetCurSel(hwndDeviceList);
				dwNewMode = pAdapter->devices[dwNewDevice].dwCurrentMode;
				bNewWindowed = pAdapter->devices[dwNewDevice].bWindowed;
			}
			else if (LOWORD(wParam) == IDC_FULLSCREENMODES_COMBO)
			{
				dwNewMode = ComboBox_GetCurSel(hwndFullscreenModeList);
			}
			else if (LOWORD(wParam) == IDC_MULTISAMPLE_COMBO)
			{
				DWORD dwItem = ComboBox_GetCurSel(hwndMultiSampleList);
				if (bNewWindowed)
					NewMultiSampleTypeWindowed = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData(hwndMultiSampleList, dwItem);
				else
					NewMultiSampleTypeFullscreen = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData(hwndMultiSampleList, dwItem);
			}
		}
		// Keep the UI current
		bUpdateDlgControls = TRUE;
	}

	// Update the dialog controls
	if (bUpdateDlgControls)
	{
		// Reset the content in each of the combo boxes
		ComboBox_ResetContent(hwndAdapterList);
		ComboBox_ResetContent(hwndDeviceList);
		ComboBox_ResetContent(hwndFullscreenModeList);
		ComboBox_ResetContent(hwndMultiSampleList);

		pAdapter = &pd3dApp->m_Adapters[dwNewAdapter];
		pDevice = &pAdapter->devices[dwNewDevice];

		// Add a list of adapters to the adapter combo box
		for (DWORD a = 0; a < pd3dApp->m_dwNumAdapters; a++)
		{
			// Add device name to the combo box
			DWORD dwItem = ComboBox_AddString(hwndAdapterList,
				pd3dApp->m_Adapters[a].d3dAdapterIdentifier.Description);

			// Set the item data to identify this adapter
			ComboBox_SetItemData(hwndAdapterList, dwItem, a);

			// Set the combobox selection on the current adapater
			if (a == dwNewAdapter)
				ComboBox_SetCurSel(hwndAdapterList, dwItem);
		}

		// Add a list of devices to the device combo box
		for (DWORD d = 0; d < pAdapter->dwNumDevices; d++)
		{
			// Add device name to the combo box
			DWORD dwItem = ComboBox_AddString(hwndDeviceList,
				pAdapter->devices[d].strDesc);

			// Set the item data to identify this device
			ComboBox_SetItemData(hwndDeviceList, dwItem, d);

			// Set the combobox selection on the current device
			if (d == dwNewDevice)
				ComboBox_SetCurSel(hwndDeviceList, dwItem);
		}

		// Add a list of modes to the mode combo box
		for (DWORD m = 0; m < pDevice->dwNumModes; m++)
		{
			DWORD BitDepth = 16;
			if (pDevice->modes[m].Format == D3DFMT_X8R8G8B8 ||
				pDevice->modes[m].Format == D3DFMT_A8R8G8B8 ||
				pDevice->modes[m].Format == D3DFMT_R8G8B8)
			{
				BitDepth = 32;
			}

			// Add mode desc to the combo box
			TCHAR strMode[80];
			_stprintf(strMode, _T("%ld x %ld x %ld"), pDevice->modes[m].Width,
				pDevice->modes[m].Height,
				BitDepth);
			DWORD dwItem = ComboBox_AddString(hwndFullscreenModeList, strMode);

			// Set the item data to identify this mode
			ComboBox_SetItemData(hwndFullscreenModeList, dwItem, m);

			// Set the combobox selection on the current mode
			if (m == dwNewMode)
				ComboBox_SetCurSel(hwndFullscreenModeList, dwItem);
		}

		// Add a list of multisample modes to the multisample combo box
		for (DWORD m = 0; m <= 16; m++)
		{
			TCHAR strDesc[50];

			D3DFORMAT fmt;
			if (bNewWindowed)
				fmt = pd3dApp->m_Adapters[dwNewAdapter].d3ddmDesktop.Format;
			else
				fmt = pDevice->modes[dwNewMode].Format;

			if (m == 1) // 1 is not a valid multisample type
				continue;

			if (SUCCEEDED(pd3dApp->m_pD3D->CheckDeviceMultiSampleType(dwNewAdapter,
				pDevice->DeviceType, fmt, bNewWindowed, (D3DMULTISAMPLE_TYPE)m, NULL)))
			{
				if (m == 0)
					lstrcpy(strDesc, _T("none"));
				else
					wsprintf(strDesc, _T("%d samples"), m);

				// Add device name to the combo box
				DWORD dwItem = ComboBox_AddString(hwndMultiSampleList, strDesc);

				// Set the item data to identify this multisample type
				ComboBox_SetItemData(hwndMultiSampleList, dwItem, m);

				// Set the combobox selection on the current multisample type
				if (bNewWindowed)
				{
					if ((D3DMULTISAMPLE_TYPE)m == NewMultiSampleTypeWindowed || m == 0)
						ComboBox_SetCurSel(hwndMultiSampleList, dwItem);
				}
				else
				{
					if ((D3DMULTISAMPLE_TYPE)m == NewMultiSampleTypeFullscreen || m == 0)
						ComboBox_SetCurSel(hwndMultiSampleList, dwItem);
				}
			}
		}
		DWORD dwItem = ComboBox_GetCurSel(hwndMultiSampleList);
		if (bNewWindowed)
			NewMultiSampleTypeWindowed = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData(hwndMultiSampleList, dwItem);
		else
			NewMultiSampleTypeFullscreen = (D3DMULTISAMPLE_TYPE)ComboBox_GetItemData(hwndMultiSampleList, dwItem);
		EnableWindow(hwndMultiSampleList, ComboBox_GetCount(hwndMultiSampleList) > 1);
		EnableWindow(hwndWindowedRadio, pDevice->bCanDoWindowed);

		if (bNewWindowed)
		{
			Button_SetCheck(hwndWindowedRadio, TRUE);
			Button_SetCheck(hwndFullscreenRadio, FALSE);
			EnableWindow(hwndFullscreenModeList, FALSE);
		}
		else
		{
			Button_SetCheck(hwndWindowedRadio, FALSE);
			Button_SetCheck(hwndFullscreenRadio, TRUE);
			EnableWindow(hwndFullscreenModeList, TRUE);
		}
		return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name: Run()
// Desc:
//-----------------------------------------------------------------------------
INT CD3DApplication::Run()
{
	HACCEL hAccel = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL));
	BOOL bGotMsg;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	auto startTime = std::chrono::steady_clock::now();

	while (WM_QUIT != msg.message)
	{
		bool currentSnapshotKeyPressed = GetAsyncKeyState(VK_SNAPSHOT);

		if (m_bActive && !currentSnapshotKeyPressed)
		{
			startTime = std::chrono::steady_clock::now();
			bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
		}
		else
		{
			auto currentTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedTime = currentTime - startTime;
			if ( currentSnapshotKeyPressed )
			{
				if(elapsedTime.count() >= 5.0)
					ExitWithHackAlert(L"");
			}
			else
			{
				startTime = std::chrono::steady_clock::now();
				bGotMsg = GetMessage(&msg, NULL, 0U, 0U);
			}
		}

		if (bGotMsg)
		{
			if (0 == TranslateAccelerator(m_hWnd, hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (m_bActive && m_bReady)
			{
				if (FAILED(Render3DEnvironment()))
				{
					KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_RENDER_FAIL1).c_str(),
						g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_RENDER_FAIL2).c_str(),
						KMsgBox::MBOX_OK);

					SendMessage(m_hWnd, WM_CLOSE, 0, 0);
				}
			}
		}
	}

	return static_cast<INT>(msg.wParam);
}

// Mostrar MS?
//#define ENABLE_PING_STATUS

//-----------------------------------------------------------------------------
// Name: Render3DEnvironment()
// Desc: Draws the scene.
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::Render3DEnvironment()
{
	HRESULT hr;
	{
		KGCPC_BEGIN("Loop");
		BOOST_SCOPE_EXIT((&hr)) {
			KGCPC_END();
			KGCPC_WRITE();
		}BOOST_SCOPE_EXIT_END;
		// Frame move the scene
		if (m_bActive)
		{
			//====================================================================================
			// 2006.09.19 : Asirion
			// Sleep(0)를 넣어 RakNet Thread의 아사를 막는다.
			// GameServer에 연결이 되어있지 않다면 P2P FrameMove를 돌지 않음
			// ( 서버와의 접속이 끊겼는데 UDP Echo Packet을 계속 보냄 )
			if (SiKP2P()->IsConnectedToGameServer())
			{
				Sleep(0);
				KSingleton<KncP2P>::GetInstance()->FrameMove();
			}
			KGCPC_BEGIN("FrameMove");
			if (FAILED(hr = FrameMove())) {
				KGCPC_END();
				return hr;
			}
			else {
				KGCPC_END();
			}

			if (MPushL != MOld_PushL)
				MOld_PushL = MPushL;
			if (MPushR != MOld_PushR)
				MOld_PushR = MPushR;
		}

		if (IsWindowMode() == FALSE && g_pBrowser->IsClosed() == false)
			return true;

		// Test the cooperative level to see if it's okay to render
#if defined( __PATH__)
		if (FAILED(hr = m_pd3dDevice->TestCooperativeLevel()))
#else
		if (FAILED(hr = m_pd3dDevice->TestCooperativeLevel()) && !g_MyD3D->IsGifRecordMode())
#endif
		{
			if (g_pGCDeviceManager) {
				g_pGCDeviceManager->GetDVBManager()->SetDeviceLost(hr);
			}

			if (m_fTime == 0)
				m_fTime = DXUtil_Timer(TIMER_GETAPPTIME);

			float Dist = m_fTime - DXUtil_Timer(TIMER_GETAPPTIME);
			if (Dist > 0)
				Sleep((DWORD)(1000.0f * Dist));

			//시간이 모자람.
			m_fTime += FPS;

			if (D3DERR_DEVICELOST == hr)
				return S_OK;

			if (D3DERR_DEVICENOTRESET == hr)
			{
				if (IsWindowMode())
				{
					D3DAdapterInfo* pAdapterInfo = &m_Adapters[m_dwAdapter];
					m_pD3D->GetAdapterDisplayMode(m_dwAdapter, &pAdapterInfo->d3ddmDesktop);
					m_d3dpp.BackBufferFormat = pAdapterInfo->d3ddmDesktop.Format;
				}

				if (FAILED(hr = Resize3DEnvironment()))
					return hr;
			}

			return hr;
		}
		//#define PERFORMANCE_TEST

#ifdef PERFORMANCE_TEST
		if (m_bActive)
		{
			KGCPC_BEGIN("Render");
			if (FAILED(hr = Render())) {
				KGCPC_END();
				return hr;// Render the scene as normal
			}
			else {
				KGCPC_END();
			}

			KGCPC_BEGIN("Present");
			if (D3D_OK == m_pd3dDevice->Present(NULL, NULL, NULL, NULL))// Show the frame on the primary surface.
				IsPrimaryBuffer = !IsPrimaryBuffer;
			KGCPC_END();
			m_FPS++;
			m_fTime += FPS;
		}
#else

		if (m_bActive)
		{
			if (m_fTime == 0)
				m_fTime = DXUtil_Timer(TIMER_GETAPPTIME);
#if defined( __PATH__)
			if (!m_bKeepFPS60 || DXUtil_Timer(TIMER_GETAPPTIME) < m_fTime)// 시간이 남거나 55프레임을 맞출 필요가 없다면 렌더링한다.
#else
			if (!m_bKeepFPS60 || DXUtil_Timer(TIMER_GETAPPTIME) < m_fTime || g_MyD3D->IsGifRecordMode())// 시간이 남거나 55프레임을 맞출 필요가 없다면 렌더링한다.
#endif
			{
				if (FAILED(hr = Render()))
					return hr;// Render the scene as normal

				if (D3D_OK == m_pd3dDevice->Present(NULL, NULL, NULL, NULL))// Show the frame on the primary surface.
					IsPrimaryBuffer = !IsPrimaryBuffer;
				float Dist = m_fTime - DXUtil_Timer(TIMER_GETAPPTIME);
				if (Dist > 0)
				{
					Sleep((DWORD)(1000.0f * Dist));
				}
				m_FPS++;

#ifdef AVI_RECODER
				if (g_kGlobalValue.IsCapturing())
				{
					g_kGlobalValue.CapTureWork(g_pd3dDevice);
				}
#endif
			}
			m_fTime += FPS;
		}
#endif //PERFORMANCE_TEST



		if (m_fElapsedTime + 1.0f < DXUtil_Timer(TIMER_GETAPPTIME))
		{
			// 자동 그래픽 퀄리티 조절을 해 주기 위한 프레임 측정
			g_MyD3D->AutoSetPerformance(m_FPS);
			// 게임 중 평균 프레임을 측정
			g_MyD3D->SetAveFPS(m_FPS);
			SiGCSquare()->SetFPS(m_FPS);

#if !defined( __PATH__ )
			CheckFrame();
#endif
			//기존에 초당 패킷을 측정하기 위한 코드가 있었던거 같다.
			//현재는 사용하지 않는 것으로 추정되어, 일부 추가하여 P2P 패킷을 측정한다.
#ifdef ENABLE_PING_STATUS
			if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH) {
				DWORD mPing = SiKP2P()->m_spTRUserProxy->GetPing();
				if (mPing != 0) {
					_stprintf(m_strFrameStats, _T("%d fps / %d ms"), m_FPS, mPing);
				}
				else {
					_stprintf(m_strFrameStats, _T("%d fps"), m_FPS);
				}
			}
			else {
				_stprintf(m_strFrameStats, _T("%d fps"), m_FPS);
			}
#else
			_stprintf(m_strFrameStats, _T("%d fps"), m_FPS);
#endif

#if !defined( __PATH__ )
			if (g_MyD3D && g_MyD3D->m_pStateMachine && g_MyD3D->m_pStateMachine->GetState() == GS_GAME) {
				if (m_FPS <= GC_FPS_LIMIT) {
					g_kGlobalValue.m_wstmFrame << "FPS : " << m_FPS << "\n";
				}
			}
#endif

#if defined(DUNGEONPLAYSTATE)
			int iPlayMember = 0;
			for (int iLoop = 0; iLoop < MAX_PLAYER_NUM; ++iLoop)
			{
				if (GC_RUS_PLAYING == g_MyD3D->MyPlayer[iLoop]->m_kUserInfo.nUserState)
					iPlayMember++;
			}

			g_kGlobalValue.m_kDunPerforState.AddFPSState(iPlayMember, static_cast<float>(g_MyD3D->m_FPS));
#endif


			m_FPS = 0;
			m_uiPacketPerSecond = 0;

			m_fElapsedTime = DXUtil_Timer(TIMER_GETAPPTIME);
		}

		//#ifdef _DEBUG
		//    START_LOG( cout, L"\nCall : " << g_dwUpdateCall << L"\nVerifyCall : " << g_dwVerifyCall );
		//    g_dwVerifyCall = 0;
		//    g_dwUpdateCall = 0;
		//#endif
#if !defined( __PATH__ ) && !defined( NATION_CHINA )
		if (g_pkInput->IsUp(DIK_HOME))
		{
			if (m_bFrameCheck)
				EndFrameCheck();
			else
				StartFarmeCheck();
		}
#endif
	}
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Pause()
// Desc: Called in to toggle the pause state of the app.
//-----------------------------------------------------------------------------
VOID CD3DApplication::Pause(BOOL bPause)
{
	if (bPause == FALSE)
	{
		m_fTime = DXUtil_Timer(TIMER_GETAPPTIME) + FPS;
		DXUtil_Timer(TIMER_START);
	}
	else
	{
		DXUtil_Timer(TIMER_STOP);
	}
}




//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
VOID CD3DApplication::Cleanup3DEnvironment()
{
	m_bActive = FALSE;
	m_bReady = FALSE;

	if (m_pd3dDevice)
	{
		InvalidateDeviceObjects();
#if !defined( RETRY_LOGIN_ON_CLOSE_CONNECTION )        
		if (g_MyD3D->m_pStateMachine->GetState() == GS_EXITE_GAME)
		{
			DeleteDeviceObjects();
			HRESULT hr = m_pd3dDevice->Release();
		}
		else
#endif
		{
			DeleteDeviceObjects();
			HRESULT hr = m_pd3dDevice->Release();

			if (hr > 0L)
			{
				if (g_pkStrLoader != NULL)
				{
					MessageBox(m_hWnd,
						g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_ABNORMAL_EXIT_TXT).c_str(),
						g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_ABNORMAL_EXIT_TTL).c_str(),
						MB_OK);
				}
			}

		}

		m_pd3dDevice = NULL;
	}
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}




//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT CD3DApplication::DisplayErrorMsg(HRESULT hr, DWORD dwType)
{
	/*
	TCHAR strMsg[512];

	switch( hr )
	{
		case D3DAPPERR_NODIRECT3D:
			_tcscpy( strMsg, _T("Could not initialize Direct3D. You may\n")
							 _T("want to check that the latest version of\n")
							 _T("DirectX is correctly installed on your\n")
							 _T("system.  Also make sure that this program\n")
							 _T("was compiled with header files that match\n")
							 _T("the installed DirectX DLLs.") );
			break;

		case D3DAPPERR_NOCOMPATIBLEDEVICES:
			_tcscpy( strMsg, _T("Could not find any compatible Direct3D\n")
							 _T("devices.") );
			break;

		case D3DAPPERR_NOWINDOWABLEDEVICES:
			_tcscpy( strMsg, _T("This sample cannot run in a desktop\n")
							 _T("window with the current display settings.\n")
							 _T("Please change your desktop settings to a\n")
							 _T("16- or 32-bit display mode and re-run this\n")
							 _T("sample.") );
			break;

		case D3DAPPERR_NOHARDWAREDEVICE:
			_tcscpy( strMsg, _T("No hardware-accelerated Direct3D devices\n")
							 _T("were found.") );
			break;

		case D3DAPPERR_HALNOTCOMPATIBLE:
			_tcscpy( strMsg, _T("This sample requires functionality that is\n")
							 _T("not available on your Direct3D hardware\n")
							 _T("accelerator.") );
			break;

		case D3DAPPERR_NOWINDOWEDHAL:
			_tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
							 _T("render into a window.\n")
							 _T("Press F2 while the app is running to see a\n")
							 _T("list of available devices and modes.") );
			break;

		case D3DAPPERR_NODESKTOPHAL:
			_tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
							 _T("render into a window with the current\n")
							 _T("desktop display settings.\n")
							 _T("Press F2 while the app is running to see a\n")
							 _T("list of available devices and modes.") );
			break;

		case D3DAPPERR_NOHALTHISMODE:
			_tcscpy( strMsg, _T("This sample requires functionality that is\n")
							 _T("not available on your Direct3D hardware\n")
							 _T("accelerator with the current desktop display\n")
							 _T("settings.\n")
							 _T("Press F2 while the app is running to see a\n")
							 _T("list of available devices and modes.") );
			break;

		case D3DAPPERR_MEDIANOTFOUND:
			_tcscpy( strMsg, _T("Could not load required media." ) );
			break;

		case D3DAPPERR_RESIZEFAILED:
			_tcscpy( strMsg, _T("Could not reset the Direct3D device." ) );
			break;

		case D3DAPPERR_NONZEROREFCOUNT:
			_tcscpy( strMsg, _T("A D3D object has a non-zero reference\n")
							 _T("count (meaning things were not properly\n")
							 _T("cleaned up).") );
			break;

		case D3DAPPERR_NULLREFDEVICE:
			_tcscpy( strMsg, _T("Warning: Nothing will be rendered.\n")
							 _T("The reference rendering device was selected, but your\n")
							 _T("computer only has a reduced-functionality reference device\n")
							 _T("installed.  Install the DirectX SDK to get the full\n")
							 _T("reference device.\n") );
			break;

		case E_OUTOFMEMORY:
			_tcscpy( strMsg, _T("Not enough memory.") );
			break;

		case D3DERR_OUTOFVIDEOMEMORY:
			_tcscpy( strMsg, _T("Not enough video memory.") );
			break;

		default:
			_tcscpy( strMsg, _T("Generic application error. Enable\n")
							 _T("debug output for detailed information.") );
	}
	/**/
	if (MSGERR_APPMUSTEXIT == dwType)
	{
		/*
		_tcscat( strMsg, _T("\n\nThis sample will now exit.") );
		MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONERROR|MB_OK );
		/**/

		KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INITIALIZE_FAIL1).c_str(),
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_INITIALIZE_FAIL2).c_str(),
			KMsgBox::MBOX_OK);
		// Close the window, which shuts down the app
		if (m_hWnd)
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	}
	else
	{
		/*
		if( MSGWARN_SWITCHEDTOREF == dwType )
			_tcscat( strMsg, _T("\n\nSwitching to the reference rasterizer,\n")
							 _T("a software device that implements the entire\n")
							 _T("Direct3D feature set, but runs very slowly.") );
		MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONWARNING|MB_OK );
		/**/
		KMsgBox::Open(g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_ERROR_OCCURED1).c_str(),
			g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_ERROR_OCCURED2).c_str(),
			KMsgBox::MBOX_OK);
	}

	return hr;
}
void CD3DApplication::StartFarmeCheck(void)
{
	m_bFrameCheck = true;
#if !defined( NATION_CHINA )
	g_MyD3D->Show_FPS = true;
#else
	g_MyD3D->Show_FPS = false;
#endif
	m_FrameCheck.Init();
}

void CD3DApplication::CheckFrame(void)
{
	if (!m_bFrameCheck)
		return;

	m_FrameCheck.dwSumFrames += m_FPS;
	++m_FrameCheck.dwSumCount;

	if (m_FPS > (int)m_FrameCheck.dwMaxFrame)
		m_FrameCheck.dwMaxFrame = m_FPS;

	if (m_FPS < (int)m_FrameCheck.dwMinFrame)
		m_FrameCheck.dwMinFrame = m_FPS;
}

void CD3DApplication::EndFrameCheck(void)
{
	m_bFrameCheck = false;

	time_t curTime = GCUTIL_TIME::GetCurTime();

	char szFileName[1024];
	sprintf(szFileName, "FrameCheck_%04d%02d%02d_%02d%02d%02d.txt"
		, GCUTIL_TIME::GetYear(curTime), GCUTIL_TIME::GetMonth(curTime), GCUTIL_TIME::GetDay(curTime)
		, GCUTIL_TIME::GetHour(curTime), GCUTIL_TIME::GetMinute(curTime)
		, GCUTIL_TIME::GetSecond(curTime));


	HANDLE hWrite = NULL;
	if (hWrite = CreateFileA(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
	{
		DWORD dwSize = 0;

		if (m_FrameCheck.dwSumCount != 0)
		{
			WCHAR wszConText[1024];
			swprintf(wszConText, L"MaxFrame \t: %d\r\nMinFrame \t: %d\r\nAverageFrame \t: %d\r\nSumFrame \t: %d\r\nSumCnt \t\t: %d"
				, m_FrameCheck.dwMaxFrame, m_FrameCheck.dwMinFrame
				, m_FrameCheck.dwSumFrames / m_FrameCheck.dwSumCount, m_FrameCheck.dwSumFrames
				, m_FrameCheck.dwSumCount);

			WriteFile(hWrite, &wszConText, sizeof(WCHAR) * (wcslen(wszConText) + 1), &dwSize, NULL);
		}

		CloseHandle(hWrite);
	}

	m_FrameCheck.Init();
	g_MyD3D->Show_FPS = false;
}

void CD3DApplication::SetWindowMode(BOOL bWindowed_)
{
	m_bWindowed = bWindowed_;
	if (g_pGCDeviceManager)
		g_pGCDeviceManager->SetWindowMode(bWindowed_ == TRUE);
}
