#ifndef _KDINPUT_H_
#define _KDINPUT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>

#if defined(USE_JOYSTICK)
extern bool                 IsUseJoystic;
#endif
extern BYTE                 diks[256];
#if defined(USE_JOYSTICK)
extern bool					GamePad[17];
#endif
extern BYTE                 Old_diks[256];
#if defined(USE_JOYSTICK)
extern LPDIRECTINPUTDEVICE8 g_pJoystick;
#endif

extern HRESULT  OnCreateDevice( HWND hDlg );
extern HRESULT  ReadImmediateData( void );
extern void     FreeDirectInput( void );
extern HRESULT ReadImmediateJoystickData( void );

#endif // _KDINPUT_H_
