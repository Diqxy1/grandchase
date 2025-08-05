#include "stdafx.h"
#define DIRECTINPUT_VERSION 0x0800

//

#include "KDInput.h"

#if defined(USE_JOYSTICK)
bool                    IsUseJoystic    = false;
#endif
BYTE                    diks[256]       = { 0, };
#if defined(USE_JOYSTICK)
bool					GamePad[17]		= { false, };
#endif
BYTE                    Old_diks[256]   = { 0, };
#if defined(USE_JOYSTICK)
LPDIRECTINPUTDEVICE8    g_pJoystick     = NULL;
#endif

static LPDIRECTINPUT8       g_pDI       = NULL;
static LPDIRECTINPUTDEVICE8 g_pKeyboard = NULL;
static DIJOYSTATE2          js;

#if defined(USE_JOYSTICK)
//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, void* pContext )
{
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}

static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext )
{
    HWND hDlg = (HWND)pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 

        // Set the range for the axis
        if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
            return DIENUM_STOP;
   }
   return DIENUM_CONTINUE;
}
#endif

//-----------------------------------------------------------------------------
// Name: OnCreateDevice()
// Desc: Setups a the keyboard device using the flags from the dialog.
//-----------------------------------------------------------------------------
HRESULT OnCreateDevice( HWND hDlg )
{
    HRESULT hr;
    FreeDirectInput();
	
	// Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (void**)&g_pDI, NULL ) ) ) return hr;
    
    
	// Obtain an interface to the system keyboard device.
    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL ) ) )  return hr;
    if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) ) return hr;
        
    //DISCL_EXCLUSIVE|DISCL_FOREGROUND|DISCL_NOWINKEY;
	if( FAILED ( hr = g_pKeyboard->SetCooperativeLevel( hDlg, DISCL_NONEXCLUSIVE|DISCL_NOWINKEY|DISCL_FOREGROUND))) return hr;

    g_pKeyboard->Acquire();

#if defined(USE_JOYSTICK)
	// 조이스틱
	// Look for a simple joystick we can use for this sample program.
    g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY );
        
    if(g_pJoystick)
    {
		//Joystic이 사용가능합니다
		g_pJoystick->SetDataFormat( &c_dfDIJoystick2 );	
		g_pJoystick->SetCooperativeLevel( hDlg, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
		g_pJoystick->EnumObjects( EnumObjectsCallback, (VOID*)hDlg, DIDFT_ALL);
		g_pJoystick->Acquire();
		IsUseJoystic = true;
		// IsUseJoystic값은 옵션 로딩에서 결정되어있음...
	}
    else
    {
		//Joystic이 사용 불가능합니다
		IsUseJoystic = false;
	}
#endif
    
    return S_OK;
}
#if defined(USE_JOYSTICK)
HRESULT ReadImmediateJoystickData( void )
{
    HRESULT hr;

    // Poll the device to read the current state
    if(g_pJoystick && IsUseJoystic)
    {
        g_pJoystick->Poll(); 
        hr = g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js );

        if( FAILED(hr) )
        {
            hr = g_pJoystick->Acquire();
            if( FAILED(hr) )
                while( hr == DIERR_INPUTLOST ) hr = g_pJoystick->Acquire(); 
        }

        float Length=D3DXVec2Length(&D3DXVECTOR2((float)js.lX, (float)js.lY));
        float Angle=acosf((float)js.lX/Length);
        if(js.lY<0) 
            Angle=6.2832f-Angle;

        if(Length>400.0f)
        {
            if(Angle>3.6652f && Angle<5.7596f) 
                GamePad[0] = true;
            if(Angle>.5236f && Angle<2.618f) 
                GamePad[1] = true;
            if(Angle>2.0944f && Angle<4.1888f) 
                GamePad[2] = true;
            if(Angle<1.0472f || Angle>5.236f) 
                GamePad[3] = true;
        }

        for(int i = 0 ; i < 17 -4; ++i){
            if(js.rgbButtons[i]){
                GamePad[i+4] = true;
            }
        }
    }
    return hr;
}
#endif


//-----------------------------------------------------------------------------
// Name: ReadImmediateData()
// Desc: Read the input device's state when in immediate mode and display it.
//-----------------------------------------------------------------------------
HRESULT ReadImmediateData( void )
{
    HRESULT hr;

	// Keboard data loading
	if( NULL == g_pKeyboard ) return S_OK;
    
    // Get the input's device state, and put the state in dims
    memcpy(Old_diks,diks,256);
	ZeroMemory( &diks, sizeof(diks) );
    hr = g_pKeyboard->GetDeviceState( sizeof(diks), &diks );
    	
	if( FAILED(hr) )
    {
		hr = g_pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
			hr = g_pKeyboard->Acquire();        
    }

#if defined(USE_JOYSTICK)
    ReadImmediateJoystickData();
#endif
	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
void FreeDirectInput( void )
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pKeyboard )
        g_pKeyboard->Unacquire();

#if defined(USE_JOYSTICK)
    if( g_pJoystick )
        g_pJoystick->Unacquire();
#endif

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pKeyboard );
#if defined(USE_JOYSTICK)
    SAFE_RELEASE( g_pJoystick );
#endif
	SAFE_RELEASE( g_pDI );
}
