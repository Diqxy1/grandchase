#ifndef _HACKSHIELD_H_
#define _HACKSHIELD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <string>
//#include <Windows.h>

#define HACKSHIELD_FOLDER_NAME_A "HShield\\"
#define HACKSHIELD_FOLDER_NAME_W L"HShield\\"
#define HACKSHIELD_EHSVC_DLL_NAME_A "EhSvc.dll"
#define HACKSHIELD_EHSVC_DLL_NAME_W L"EhSvc.dll"

#ifdef NATION_THAILAND
    #define HACKSHIELD_GAME_CODE 5602
#define HACKSHIELD_LICENSE_KEY L"69C54AD2D1AB74B9FAE38963"
#endif

#define HACKSHIELD_UPDATE_TIME_OUT 60000

// 060403. jseop. HackShield update.
//BOOL HS_HSUpdate();

// 060403. jseop. HackShield initialization.
BOOL HS_Initialize();

// 060405. jseop. HackShield start service.
BOOL HS_StartService();

// 060405. jseop. HackShield stop service.
BOOL HS_StopService();

// 060405. jseop. HackShield uninitialization.
BOOL HS_Uninitialize();

// 060405. jseop. HackShield pause service.
BOOL HS_PauseService();

// 060405. jseop. HackShield resume service.
BOOL HS_ResumeService();

// 060411. jseop. HackShield save function address.
BOOL HS_SaveFuncAddress();

// 060413. jseop. HackShield make guid ack msg.
BOOL HS_MakeGuidAckMsg( BYTE* abyteGuidReq, BYTE* abyteGuidAck );

// 060413. jseop. HackShield make ack msg.
BOOL HS_MakeAckMsg( BYTE* abyteReqMsg, BYTE* abyteAckMsg );

// 060405. jseop. HackShield callback function.
int CALLBACK HS_Callback( long lCode, long lParamSize, void* pParam );

// 060407. jseop. Message output function.
void ExitWithNPHSErrorMsg( LPCWSTR szTitle, long lErrorCode, LPCWSTR szMsg1, LPCWSTR szMsg2, BOOL bShowErrorCode = TRUE );

#ifdef NATION_CHINA
void ExitWithLauncher( LPCWSTR szTitle, long lErrorCode, LPCWSTR szMsg1, LPCWSTR szMsg2, BOOL bShowErrorCode = TRUE );
#endif

// 060403. jseop. Extract program path.
extern HRESULT CorrectProgramPath( std::string& strOutProgramPath );

#endif // _HACKSHIELD_H_
