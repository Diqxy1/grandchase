#ifndef _NPROTECT_H_
#define _NPROTECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <Windows.h>

#define NPGG_CHECK_INTERVAL ( 5.0f )

// 060206. jseop. nProtect GameGuard Initialization.
BOOL NPGameGuardInit();

// 060207. jseop. nProtect GameGuard Set Window Handle.
void NPGameGuardSetHWnd( HWND hWindow );

// 060227. jseop. nProtect GameGuard Send.
void NPGameGuardSend( LPCTSTR szUserID );

// 060208. jseop. nProtect GameGuard Check.
BOOL NPGameGuardCheck();

// 060208. jseop. nProtect GameGuard Callback.
extern BOOL CALLBACK NPGameMonCallBack( DWORD dwMsg, DWORD dwArg );

// 060407. jseop. Message output function.
extern void ExitWithNPHSErrorMsg( LPCWSTR szTitle, long lErrorCode, LPCWSTR szMsg1, LPCWSTR szMsg2, BOOL bShowErrorCode );

#endif // _NPROTECT_H_