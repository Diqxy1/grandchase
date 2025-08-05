#include "stdafx.h"
//

#include "HackShield.h"
#include "HShield.h"
//

//

#include "MyD3D.h"
//



//#pragma comment( lib, "Version.lib" )
//#pragma comment( lib, "HShield.lib" )
//#pragma comment( lib, "HSUpChk.lib" )

// 060403. jseop. HackShield update.
/*
BOOL HS_HSUpdate()
{
#ifdef NO_HACKSHIELD
    return TRUE;
#else
    CHAR szHackShieldPath[MAX_PATH];
    std::string strHackShieldPath;
    CorrectProgramPath( strHackShieldPath );

    ::lstrcpyA( szHackShieldPath, strHackShieldPath.c_str() );
    ::lstrcatA( szHackShieldPath, HACKSHIELD_FOLDER_NAME_A );

    DWORD dwRet = _AhnHS_HSUpdate( szHackShieldPath, ( DWORD )HACKSHIELD_UPDATE_TIME_OUT );

    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];

    if( dwRet == HACKSHIELD_ERROR_SUCESS )
    {
        return TRUE;
    }
    else
    {
        switch( dwRet )
        {
            case HSERROR_ENVFILE_NOTREAD:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR01 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                break;
            }
            case HSERROR_ENVFILE_NOTWRITE:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR02 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                break;
            }
            case HSERROR_NETWORK_CONNECT_FAIL:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR03 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                break;
            }
            case HSERROR_LIB_NOTEDIT_REG:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR04 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                break;
            }
            case HSERROR_NOTFINDFILE:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR05 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                break;
            }
            case HSERROR_PROTECT_LISTLOAD_FAIL:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR06 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                break;
            }
            case HSERROR_HSUPDATE_TIMEOUT:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR07 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                break;
            }
            default:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR08 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                break;
            }
        }

        ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )dwRet, szMsg1, szMsg2 );

        return FALSE;
    }
#endif // NO_HACKSHIELD
}
*/

// 060403. jseop. HackShield initialization.
BOOL HS_Initialize()
{
#ifdef NO_HACKSHIELD
    return TRUE;
#else

    CHAR szHackShieldPath[MAX_PATH];
    CHAR szEhSvcPath[MAX_PATH];
    std::string strHackShieldPath;

    CorrectProgramPath( strHackShieldPath );
    ::lstrcpyA( szHackShieldPath, strHackShieldPath.c_str() );
    ::lstrcatA( szHackShieldPath, HACKSHIELD_FOLDER_NAME_A );
    ::lstrcpyA( szEhSvcPath, szHackShieldPath );
    ::lstrcatA( szEhSvcPath, HACKSHIELD_EHSVC_DLL_NAME_A );

    unsigned int dwOption =
                     AHNHS_CHKOPT_SPEEDHACK |
#ifndef _DEBUG       // debug 모드일 때는 끈다.
                     AHNHS_CHKOPT_READWRITEPROCESSMEMORY |
                     AHNHS_CHKOPT_KDTRACER |
                     AHNHS_CHKOPT_OPENPROCESS |
#endif // _DEBUG
                     AHNHS_CHKOPT_AUTOMOUSE |
#ifndef _DEBUG       // debug 모드일 때는 끈다.
                     AHNHS_CHKOPT_MESSAGEHOOK |
#endif // _DEBUG
                     AHNHS_CHKOPT_PROCESSSCAN |
                     // AHNHS_CHKOPT_ALL |
                     AHNHS_USE_LOG_FILE |
					 //AHNHS_CHKOPT_SELF_DESTRUCTION |
					 AHNHS_CHKOPT_PROTECT_D3DX |
					 AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION |
                     AHNHS_ALLOW_SVCHOST_OPENPROCESS |
                     AHNHS_ALLOW_LSASS_OPENPROCESS |
                     AHNHS_ALLOW_CSRSS_OPENPROCESS |
//					 AHNHS_DISPLAY_HACKSHIELD_LOGO |
					 AHNHS_DONOT_TERMINATE_PROCESS;
					 
	int iRet = _AhnHS_Initialize( GCUTIL_STR::GCStrCharToWide(szEhSvcPath), HS_Callback, HACKSHIELD_GAME_CODE, HACKSHIELD_LICENSE_KEY, dwOption, AHNHS_SPEEDHACK_SENSING_RATIO_HIGHEST );

    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];

    if( iRet == HS_ERR_OK )
    {
        return TRUE;
    }
    else
    {
        switch( iRet )
        {
            case HS_ERR_ANOTHER_SERVICE_RUNNING:
                {
                    //HackShield 3.1 부터는 발생하지 않는다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR09 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_COMPATIBILITY_MODE_RUNNING:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR10 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
            case HS_ERR_NEED_ADMIN_RIGHTS:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR11 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
            case HS_ERR_ALREADY_INITIALIZED:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR12 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_INVALID_PARAM:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR13 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_INVALID_LICENSE:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR14 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_INVALID_FILES:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR15 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                    break;
                }
            case HS_ERR_INIT_DRV_FAILED:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR16 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
            case HS_ERR_DEBUGGER_DETECT:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR17 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
            case HS_ERR_NEED_UPDATE:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR18 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                    break;
                }
            case HS_ERR_UNKNOWN:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR19 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
            default:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR19 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
        }

        ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )iRet, szMsg1, szMsg2 );

        return FALSE;
    }
#endif // NO_HACKSHIELD
}

// 060405. jseop. HackShield start service.
BOOL HS_StartService()
{
#ifdef NO_HACKSHIELD
    return TRUE;
#else
    int iRet = _AhnHS_StartService();

    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];

    if( iRet == HS_ERR_OK )
    {
        return TRUE;
    }
    else
    {
        switch( iRet )
        {
            case HS_ERR_NOT_INITIALIZED:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR20 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_ALREADY_SERVICE_RUNNING:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR21 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_START_ENGINE_FAILED:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR22 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                    break;
                }
            case HS_ERR_DRV_FILE_CREATE_FAILED:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR23 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                    break;
                }
            case HS_ERR_REG_DRV_FILE_FAILED:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR24 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_RESTART_REINSTALL ).c_str() );
                    break;
                }
            case HS_ERR_START_DRV_FAILED:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR25 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_REBOOT ).c_str() );
                    break;
                }
            default:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR26 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
        }

        ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )iRet, szMsg1, szMsg2 );

        return FALSE;
    }
#endif // NO_HACKSHIELD
}

// 060405. jseop. HackShield stop service.
BOOL HS_StopService()
{
#ifdef NO_HACKSHIELD
    return TRUE;
#else
    int iRet = _AhnHS_StopService();

    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];

    if( iRet == HS_ERR_OK )
    {
        return TRUE;
    }
    else
    {
        switch( iRet )
        {
            case HS_ERR_NOT_INITIALIZED:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR20 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_SERVICE_NOT_RUNNING:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR27 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            default:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR28 ).c_str() );
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
        }

        ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )iRet, szMsg1, szMsg2 );

        return FALSE;
    }
#endif // NO_HACKSHIELD
}

// 060405. jseop. HackShield uninitialization.
BOOL HS_Uninitialize()
{
#ifdef NO_HACKSHIELD
    return TRUE;
#else
    int iRet = _AhnHS_Uninitialize();

    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];

    if( iRet == HS_ERR_OK )
    {
        return TRUE;
    }
    else
    {
        switch( iRet )
        {
            case HS_ERR_SERVICE_STILL_RUNNING:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR29 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_NOT_INITIALIZED:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR20 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            default:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR30 ).c_str() );
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
        }

        ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )iRet, szMsg1, szMsg2 );

        return FALSE;
    }
#endif // NO_HACKSHIELD
}

// 060405. jseop. HackShield pause service.
BOOL HS_PauseService()
{
#ifdef NO_HACKSHIELD
    return TRUE;
#else
    int iRet = _AhnHS_PauseService( AHNHS_CHKOPT_MESSAGEHOOK );

    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];

    if( iRet == HS_ERR_OK )
    {
        return TRUE;
    }
    else
    {
        switch( iRet )
        {
            case HS_ERR_NOT_INITIALIZED:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR20 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_SERVICE_NOT_RUNNING:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR27 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_INVALID_PARAM:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR31 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            default:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR32 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
        }

        ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )iRet, szMsg1, szMsg2 );

        return FALSE;
    }
#endif // NO_HACKSHIELD
}

// 060405. jseop. HackShield resume service.
BOOL HS_ResumeService()
{
#ifdef NO_HACKSHIELD
    return TRUE;
#else
    int iRet = _AhnHS_ResumeService( AHNHS_CHKOPT_MESSAGEHOOK );

    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];

    if( iRet == HS_ERR_OK )
    {
        return TRUE;
    }
    else
    {
        switch( iRet )
        {
            case HS_ERR_NOT_INITIALIZED:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR20 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_SERVICE_NOT_RUNNING:
                {
                    // 별도의 처리가 필요하지 않다.
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR27 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            case HS_ERR_INVALID_PARAM:
                {
                    // 별도의 처리가 필요하지 않을 듯(?)
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR33 ).c_str() );
                    ::lstrcpyW( szMsg2, L"" );
                    break;
                    //return TRUE;
                }
            default:
                {
                    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR34 ).c_str() );
                    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                    break;
                }
        }

        ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )iRet, szMsg1, szMsg2 );

        return FALSE;
    }
#endif // NO_HACKSHIELD
}

// 060508. jseop. SaveFuncAddress 에서 필요한 함수.
#ifndef NO_HACKSHIELD
extern DWORD GetFileCheckSum(char* szFileName = NULL);
#endif // NO_HACKSHIELD

// 060411. jseop. HackShield save function address.
BOOL HS_SaveFuncAddress()
{
	return TRUE; //서버 연동은 사용하지 않는다. 확장 서버 연동만 사용함.

#ifdef NO_HACKSHIELD
    return TRUE;
#else
    //int iRet = _AhnHS_SaveFuncAddress( 3, HS_Initialize, HS_Callback, GetFileCheckSum );

    //if( iRet == HS_ERR_OK )
    //{
    //    return TRUE;
    //}
    //else
    //{
    //    return FALSE;
    //}
#endif // NO_HACKSHIELD
}

// 060413. jseop. HackShield make guid ack msg.
BOOL HS_MakeGuidAckMsg( BYTE* abyteGuidReq, BYTE* abyteGuidAck )
{
    return TRUE;
#ifdef NO_HACKSHIELD
    return TRUE;
#else
    //DWORD dwRet = _AhnHS_MakeGuidAckMsg( abyteGuidReq, abyteGuidAck );

    //WCHAR szMsg1[MAX_PATH];
    //WCHAR szMsg2[MAX_PATH];

    //if( dwRet == ERROR_SUCCESS )
    //{
    //    return TRUE;
    //}
    //else
    //{
    //    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR35 ).c_str() );
    //    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );

    //    ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )dwRet, szMsg1, szMsg2 );

    //    return FALSE;
    //}
#endif // NO_HACKSHIELD
}

// 060413. jseop. HackShield make ack msg.
BOOL HS_MakeAckMsg( BYTE* abyteReqMsg, BYTE* abyteAckMsg )
{
    return TRUE;
#ifdef NO_HACKSHIELD
    return TRUE;
#else
    //DWORD dwRet = _AhnHS_MakeAckMsg( abyteReqMsg, abyteAckMsg );

    //WCHAR szMsg1[MAX_PATH];
    //WCHAR szMsg2[MAX_PATH];

    //if( dwRet == HS_ERR_OK )
    //{
    //    return TRUE;
    //}
    //else
    //{
    //    ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_FUNCTION_ERROR36 ).c_str() );
    //    ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );

    //    ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), ( long )dwRet, szMsg1, szMsg2 );

    //    return FALSE;
    //}
#endif // NO_HACKSHIELD
}

// 060405. jseop. HackShield callback function.
int CALLBACK HS_Callback( long lCode, long lParamSize, void* pParam )
{
    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];
	//FILE *fhsk;
	//fhsk=fopen("HackShieldTest.txt","w");
	//fprintf(fhsk,"%d \n",lCode);
	//fclose(fhsk);

    switch( lCode )
    {
        case AHNHS_ENGINE_DETECT_GAME_HACK:
            {
                WCHAR szHackingToolName[MAX_PATH];
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR01 ).c_str() );
                ::mbstowcs( szHackingToolName, ( CHAR* )pParam, ( size_t )( lParamSize + 1 ) );
                ::lstrcatW( szMsg1, L"\n" );
                ::lstrcatW( szMsg1, szHackingToolName );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                break;
            }
        case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
            {
                // 별도의 처리가 필요하지 않다.
                ACTAPCPARAM_DETECT_HOOKFUNCTION sHookFunctionName;
                WCHAR szModuleName[MAX_PATH];
                WCHAR szFunctionName[MAX_PATH];

                ::memcpy( &sHookFunctionName, pParam, ( size_t )lParamSize );
                ::mbstowcs( szModuleName, sHookFunctionName.szModuleName, lstrlenA( sHookFunctionName.szModuleName ) + 1 );
                ::mbstowcs( szFunctionName, sHookFunctionName.szFunctionName, lstrlenA( sHookFunctionName.szFunctionName ) + 1 );
                ::wsprintfW( szMsg1, L"%s\n%s -> %s", g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR02 ).c_str(), szModuleName, szFunctionName );
                return 1;
            }
        case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
            {
                ACTAPCPARAM_DETECT_HOOKFUNCTION sHookFunctionName;
                WCHAR szModuleName[MAX_PATH];
                WCHAR szFunctionName[MAX_PATH];

                ::memcpy( &sHookFunctionName, pParam, ( size_t )lParamSize );
                ::mbstowcs( szModuleName, sHookFunctionName.szModuleName, lstrlenA( sHookFunctionName.szModuleName ) + 1 );
                ::mbstowcs( szFunctionName, sHookFunctionName.szFunctionName, lstrlenA( sHookFunctionName.szFunctionName ) + 1 );
                ::wsprintfW( szMsg1, L"%s\n%s -> %s", g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR03 ).c_str(), szModuleName, szFunctionName );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                break;
            }
        case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
            {
                ACTAPCPARAM_DETECT_AUTOMOUSE sAutoMouseInfo;
 
                ::memcpy( &sAutoMouseInfo, pParam, ( size_t ) lParamSize );
                ::wsprintfW( szMsg1, L"%s(%d)", g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR04 ).c_str(), sAutoMouseInfo.byDetectType );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                return 1;
            }
        case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR05 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_REBOOT ).c_str() );
                break;
            }
        case AHNHS_ACTAPC_DETECT_SPEEDHACK:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR06 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                return 1;
                break;
            }

        case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR08 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                break;
            }
        case AHNHS_ACTAPC_DETECT_KDTRACE:
        case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR09 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                break;
            }
        case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
            {
                ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR10 ).c_str() );
                ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM ).c_str() );
                break;
            }
		case AHNHS_ACTAPC_DETECT_SPEEDHACK_RATIO:
			{
				return 1;
			}
		case AHNHS_ACTAPC_DETECT_AUTOMACRO:
			{
				return 1;
			}
		case AHNHS_ACTAPC_DETECT_LMP_FAILED:
		case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:
		case AHNHS_ACTAPC_DETECT_ENGINEFAILED:
		case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
		case AHNHS_ACTAPC_DETECT_ANTIFREESERVER: 
		case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
			{
				//핵쉴드 안써..
				::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_AHNHS_CALLBACK_ERROR01).c_str() );				
				::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_AHNHS_TERMINATE_PROGRAM).c_str() );
				break;
			}
		default:
			{
				return 1;
			}
	}

    ExitWithNPHSErrorMsg( g_pkStrLoader->GetString( STR_ID_ERROR_MESSAGE_TITLE ).c_str(), lCode, szMsg1, szMsg2 );
    ::PostMessage( g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0 );

    return 1;
}

// 060407. jseop. Message output function.
void ExitWithNPHSErrorMsg( LPCWSTR szTitle, long lErrorCode, LPCWSTR szMsg1, LPCWSTR szMsg2, BOOL bShowErrorCode/* = TRUE*/ )
{
    // 060628. jongwon. 일본은 error code 10진수로
    WCHAR szErrCode[MAX_PATH];
    ::wsprintfW( szErrCode, L"%x", lErrorCode );

    std::wstring strTitle;
    strTitle = szTitle;
    if( bShowErrorCode )
    {
        strTitle += L" ";
        strTitle += szErrCode;
    }

    if( g_MyD3D != NULL && g_MyD3D->Get_hWnd() != NULL )
    {
        StringCchCopyW( g_MyD3D->LastMessageCode, MAX_PATH, strTitle.c_str() );
        StringCchCopyW( g_MyD3D->LastMessage1, MAX_PATH, szMsg1 );
        StringCchCopyW( g_MyD3D->LastMessage2, MAX_PATH, szMsg2 );

#if !defined(NO_GAMEGUARD) || defined( USE_XTRAP )
		g_MyD3D->SendCurrentGameGuardInfo(GCUTIL_STR::GCStrWideToChar(szErrCode));
#endif

        ::PostMessage( g_MyD3D->Get_hWnd(), WM_CLOSE, 0, 0 );
    }
    else
    {
        std::wstring strMsg = L"";

#if !defined( NATION_TAIWAN ) && !defined( NATION_HONGKONG )
        strMsg = strTitle;
        strMsg += L"\n";
#endif
        strMsg += szMsg1;
        strMsg += L"\n";
        strMsg += szMsg2;

        ::MessageBoxW( NULL, strMsg.c_str(), szTitle, MB_OK | MB_TOPMOST );
    }
}
