#include "stdafx.h"
// KInput.cpp: implementation of the KInput class.
//
//////////////////////////////////////////////////////////////////////

#include "KInput.h"


KInput* g_pkInput = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KInput::KInput()
: m_iSenarioCount( 0 )
, m_bVirtualMode( false )
{
//{{ Appended by parkch: 2004-06-10 오전 9:36:18
    g_pkInput   = this;
//}} Appended by parkch: 2004-06-10 오전 9:36:18

    m_pDI       = NULL;
    m_pKeyboard = NULL;
    m_pMouse    = NULL;

	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	m_bIdle = false;
	m_mapIgnoreKey.clear();
}

KInput::~KInput()
{
    FreeDirectInput();
//{{ Appended by parkch: 2004-06-11 오전 10:37:32
    g_pkInput = NULL;
//}} Appended by parkch: 2004-06-11 오전 10:37:32
}

HRESULT KInput::InitDevice(HWND hWnd, BOOL bCursor )
{
    m_hWnd = hWnd;

    HRESULT hr;
    
    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
        IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) ) return hr;
    
    // Obtain an interface to the system keyboard device.
    if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )  return hr;
    if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) ) return hr;
    
    //DISCL_EXCLUSIVE|DISCL_FOREGROUND|DISCL_NOWINKEY(노 윈도우 키);
    if( FAILED( hr = m_pKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE|DISCL_NOWINKEY|DISCL_FOREGROUND )))
    return hr;

    m_pKeyboard->Acquire();

    // 마우스 초기화
    // Obtain an interface to the system mouse device.
    if( FAILED( hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL ) ) ) return hr;
    if( FAILED( hr = m_pMouse->SetDataFormat( &c_dfDIMouse ) ) ) return hr;
    
    DWORD dwCoopLevel = ( bCursor ) ? DISCL_NONEXCLUSIVE : DISCL_EXCLUSIVE;
    m_pMouse->SetCooperativeLevel( hWnd, dwCoopLevel|DISCL_FOREGROUND );
    // Acquire the newly created device
    m_pMouse->Acquire();
    
    return S_OK;
}

HRESULT KInput::FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( m_pKeyboard ) m_pKeyboard->Unacquire();    
    if( m_pMouse ) m_pMouse->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( m_pKeyboard );
    SAFE_RELEASE( m_pMouse );
    SAFE_RELEASE( m_pDI );

    return S_OK;
}

HRESULT KInput::UpdateInput()
{
	HRESULT hr;

	// CurInputData to OldInputData
	m_sOldInput = m_sCurInput;
	m_sUserOldInput = m_sUserInput;

	// CurInputData Clear
	m_sCurInput.Clear();
	m_sUserInput.Clear();
    
    // Keboard data loading
    if( NULL == m_pKeyboard ) return S_OK;
    
    // Get the input's device state, and put the state in dims
    hr = m_pKeyboard->GetDeviceState( sizeof( m_sUserInput.m_key ), &m_sUserInput.m_key );
    
    if( FAILED(hr) )
    {
        hr = m_pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) hr = m_pKeyboard->Acquire();   
        m_sUserInput.ClearKey();
        
    }

    // Mouse data loading
    if( NULL == m_pMouse ) return S_OK;
    
    // Get the input's device state, and put the state in dims
    hr = m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &m_sUserInput.m_ms );
    
    if( FAILED(hr) ) 
    {
        hr = m_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) hr = m_pMouse->Acquire();
        m_sUserInput.ClearMouse();
    }

//{{ Appended by parkch: 2004-06-10 오전 9:40:05
    ::GetCursorPos( &m_sUserInput.m_ptMouse );
    ::ScreenToClient( m_hWnd, &m_sUserInput.m_ptMouse );
//}} Appended by parkch: 2004-06-10 오전 9:40:05


	bool bChk = false;

	if (!g_MyD3D->m_bHasFocus || (m_sUserInput.m_ptMouse.x == m_sUserOldInput.m_ptMouse.x 
		&& m_sUserInput.m_ptMouse.y == m_sUserOldInput.m_ptMouse.y ))
	{
		bChk = true;
		for (int i=0;i<256;i++)
		{
			if (IS_PRESS(m_sUserInput.m_key[i])==true)
			{
				bChk=false;
				break;
			}
		}
	}		

	if (bChk==true)
	{
		if (m_dwStopTime == 0)
		{
			m_dwStopTime = ::timeGetTime(); //시간 기록한다.
		}
		else
		{
			// 1분이상 잠수면...자리비이무
			if (m_bIdle == false && m_dwStopTime + TIME_FOR_IDLE < ::timeGetTime())
			{
				if (KP2P::GetInstance()!=NULL)
				{
					KP2P::GetInstance()->Send_SetMyIdleState(true);
					m_bIdle=true;		
				}
			}
		}
	}
	else
	{
		if (m_bIdle==true)
		{
			KP2P::GetInstance()->Send_SetMyIdleState(false);
			m_bIdle=false;
		}

		m_dwStopTime = 0;
	}

	if( m_bVirtualMode || !m_vecSenario.empty() )
	{
		return UpdateVirtualInput();
	}
	else
	{
		m_sCurInput = m_sUserInput;
		for(std::map<int,int>::iterator pos = m_mapIgnoreKey.begin(); pos != m_mapIgnoreKey.end();) 
		{
			m_sCurInput.m_key[pos->first]=1;
			pos->second--;
			if(pos->second == 0) {
				m_mapIgnoreKey.erase(pos++); 
			}
			else {
				++pos;
			}
		}
		
	}

    return hr;
}

HRESULT KInput::ShowCursor( BOOL bShow )
{
	if( m_pMouse == NULL )
		return E_FAIL;
    m_pMouse->Unacquire();
    DWORD dwScl = ( bShow ) ? DISCL_NONEXCLUSIVE : DISCL_EXCLUSIVE;
    m_pMouse->SetCooperativeLevel( m_hWnd, dwScl|DISCL_FOREGROUND );
    
    return m_pMouse->Acquire();
}

HRESULT KInput::UpdateVirtualInput()
{
	//Set Input state;
	if( m_vecSenario.empty() )
	{
		m_sCurInput = m_sVirtualDefault;
		return S_OK;
	}

	if( m_iSenarioCount >= (int)m_vecSenario.size() )
	{
		m_vecSenario.clear();
		UpdateVirtualInput();
		m_iSenarioCount = 0;
		return S_OK;
	}

	m_sCurInput = m_vecSenario[m_iSenarioCount];
	++m_iSenarioCount;

	return S_OK;
}

void KInput::VirtualClick( POINT pt )
{
	SInputData vi;
	vi.m_ms.rgbButtons[MBLEFT] = 0x80;
	vi.m_ptMouse = pt;
	m_vecSenario.push_back(vi);
	ZeroMemory( &vi.m_ms, sizeof( DIMOUSESTATE ) );
	vi.m_ms.rgbButtons[MBLEFT] = 0;
	m_vecSenario.push_back(vi);
}

void KInput::VirtualMouseMove( POINT _ptBefore, POINT _ptAfter, int _ispeed )
{

}

void KInput::VirtualDrag( POINT startpt , POINT endpt, int delay)
{
     POINT pt;

    if(delay < 1) delay = 1;

    SInputData vi;

    ZeroMemory( &vi.m_ms, sizeof( DIMOUSESTATE ) );

    vi.m_ms.rgbButtons[MBLEFT] = 0;
    vi.m_ptMouse = startpt;
    m_vecSenario.push_back(vi);

    for(int i = 0; i < delay ; i++)
    {
        pt.x = (((endpt.x - startpt.x)*10000 / delay) * i)/10000 + startpt.x;
        pt.y = (((endpt.y - startpt.y)*10000 / delay) * i)/10000 + startpt.y;

        ZeroMemory( &vi.m_ms, sizeof( DIMOUSESTATE ) );
        vi.m_ms.rgbButtons[MBLEFT] = 0x80;
        vi.m_ptMouse = pt;
        m_vecSenario.push_back(vi);
    }

    ZeroMemory( &vi.m_ms, sizeof( DIMOUSESTATE ) );

    vi.m_ms.rgbButtons[MBLEFT] = 0;
    vi.m_ptMouse = endpt;
    m_vecSenario.push_back(vi);
}

void KInput::VirtualKey( std::vector<std::pair<int,int>>& vecKey)
{

    SInputData vi;

    for(std::vector<std::pair<int,int>>::iterator vIt = vecKey.begin(); vIt !=  vecKey.end() ; vIt++)
    {
        for(int i = 0; i < vIt->second ; i++)
        {
            ZeroMemory( &vi.m_ms, sizeof( DIMOUSESTATE ) );
            vi.m_key[vIt->first] = 0x80;
            m_vecSenario.push_back(vi);
        }

    }
}




void KInput::SetDefaultMousePos( POINT ptPos )
{
	m_sVirtualDefault.m_ptMouse = ptPos;
}

std::vector<BYTE> KInput::GetKeyList( bool (KInput::*func)(int) )
{
    std::vector<BYTE> vecKeys;
    vecKeys.reserve( 256 );

    for( int i = 0 ; i < 256 ; ++i ) {
        if( (this->*func)(i) == true ) {
            vecKeys.push_back( i );
        }
    }

    return vecKeys;
}