#include "stdafx.h"
#include "KGCStringID.h"
#include "KMsgBox.h"
#include "NProtect.h"
#include "Serializer/SerBuffer.h"

#include "NPGameLib.h"
#include "GlobalVariables.h" 

#include "KMailSender.h"

#pragma comment( lib, "NPGameLib.lib" )

// 060206. jseop. nProtect GameGuard Initialization.
BOOL NPGameGuardInit()
{
#ifdef NO_GAMEGUARD
    return TRUE;
#else
    DWORD dwResult;

    dwResult = g_CNPGameLib.Init();

    if( dwResult == NPGAMEMON_SUCCESS )
    {
        return TRUE;
    }
    else
    {
        WCHAR szMsg1[MAX_PATH];
        WCHAR szMsg2[MAX_PATH];
		std::wstring wstrTitle;
		std::wstring wstrErr;
		KMailSender kMail;
        std::string strErr;
	
		wstrTitle = _itow(dwResult,szMsg2,10);
		wstrTitle += L" ";

        switch( dwResult )
        {
	        case NPGAMEMON_ERROR_EXIST:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR01 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REBOOT );
                ::lstrcpyW( szMsg1, wstrErr.c_str() );
                ::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_EXIST";
                break;
            }
            case NPGAMEMON_ERROR_GAME_EXIST:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR02 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_RESTART );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_GAME_EXIST";	

                break;
            }
            case NPGAMEMON_ERROR_NPSCAN:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR03 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_NO_MEM_VIRUS );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_NPSCAN";	

                break;
            }
            case NPGAMEMON_ERROR_INIT:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR04 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REBOOT );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_INIT";	
                break;
            }
            case NPGAMEMON_ERROR_NFOUND_GG:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR05 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REINSTALL );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_NFOUND_GG";	

                break;
            }
            case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR05 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REINSTALL );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_AUTH_GAMEGUARD";	

                break;
            }
            case NPGAMEMON_ERROR_AUTH_INI:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR06 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REINSTALL );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_AUTH_INI";	

                break;
            }
            case NPGAMEMON_ERROR_CORRUPT_INI:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR06 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REINSTALL );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_CORRUPT_INI";	

                break;
            }
            case NPGAMEMON_ERROR_CORRUPT_INI2:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR06 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REINSTALL );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_CORRUPT_INI2";	

                break;
            }
            case NPGAMEMON_ERROR_NFOUND_INI:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR06 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REINSTALL );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_NFOUND_INI";	

                break;
            }
            case NPGAMEMON_ERROR_CRYPTOAPI:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR07 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_VIRUS_TEST );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_CRYPTOAPI";	

                break;
            }
            case NPGAMEMON_ERROR_EXECUTE:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR08 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_REINSTALL );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_EXECUTE";	

                break;
            }
            case NPGAMEMON_ERROR_ILLEGAL_PRG:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR09 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_END_INVALID_PROGRAM );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGAMEMON_ERROR_ILLEGAL_PRG";	

                break;
            }
            case NPGMUP_ERROR_CONNECT:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR10 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_RECONNECT );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGMUP_ERROR_CONNECT";	

                break;
            }
            case NPGMUP_ERROR_DOWNCFG:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR11 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_CHECK_NETWORK );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGMUP_ERROR_DOWNCFG";	

                break;
            }
            case NPGMUP_ERROR_ABORT:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR12 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_RETRY );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGMUP_ERROR_ABORT";	

                break;
            }
            case NPGMUP_ERROR_AUTH:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR13 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_STOP_VACCINE );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "NPGMUP_ERROR_AUTH";	

                break;
            }
            default:
            {
				wstrErr = g_pkStrLoader->GetString( STR_ID_NPGG_INIT_ERROR14 ).c_str();
				wstrErr += L"\n";
				wstrErr += g_pkStrLoader->GetString( STR_ID_NPGG_SEND_MAIL );
				::lstrcpyW( szMsg1, wstrErr.c_str() );
				::lstrcpyW( szMsg2, wstrTitle.c_str() );
				strErr = "Unknown_Init_Error";	

                break;
            }
        }

        char cTemp[MAX_PATH];
        sprintf( cTemp, "%d", dwResult );
        g_kGlobalValue.RunLogProgram("19", cTemp);

        MessageBox(NULL,szMsg1,szMsg2,MB_OK|MB_TOPMOST);
        kMail.SendCurrentGameGuardInfo( strErr.c_str() );
        exit(0);
        return FALSE;
    }
#endif // NO_GAMEGUARD
}

// 060207. jseop. nProtect GameGuard Set Window Handle.
void NPGameGuardSetHWnd( HWND hWindow )
{
#ifdef NO_GAMEGUARD
    return;
#else
    g_CNPGameLib.SetHwnd( hWindow );
#endif // NO_GAMEGUARD
}

// 060227. jseop. nProtect GameGuard Send.
void NPGameGuardSend( LPCTSTR szUserID )
{
#ifdef NO_GAMEGUARD
	return;
#else
	g_CNPGameLib.Send( szUserID );
#endif // NO_GAMEGUARD
}

// 060208. jseop. nProtect GameGuard Check.
BOOL NPGameGuardCheck()
{
#ifdef NO_GAMEGUARD
    return TRUE;
#else
    DWORD dwResult;

    dwResult = g_CNPGameLib.Check();

    if( dwResult == NPGAMEMON_SUCCESS )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif // NO_GAMEGUARD
}

// 060208. jseop. nProtect GameGuard Callback.
BOOL CALLBACK NPGameMonCallback( DWORD dwMsg, DWORD dwArg )
{
#ifdef NO_GAMEGUARD
    return TRUE;
#else
    WCHAR szMsg1[MAX_PATH];
    WCHAR szMsg2[MAX_PATH];

    switch( dwMsg )
    {
		case NPGAMEMON_CHECK_CSAUTH3:
		{
			KSerBuffer buff;
			PCSAuth3Data pCSAuth3 = (PCSAuth3Data)dwArg;
			buff.Write( pCSAuth3->bPacket, pCSAuth3->dwPacketSize );
			SiKP2P()->m_pkUserProxy->SendPacket( KUserEvent::EVENT_NPGG_AUTH_ACK, buff );
			return TRUE;
		}
        case NPGAMEMON_CHECK_CSAUTH2:
        {
            KSerBuffer buff;
            buff.Write( ( void* )dwArg, sizeof( GG_AUTH_DATA ) );
            SiKP2P()->m_pkUserProxy->SendPacket( KUserEvent::EVENT_NPGG_AUTH_ACK, buff );
            return TRUE;
        }
        case NPGAMEMON_COMM_ERROR:
        case NPGAMEMON_COMM_CLOSE:
        {
            //lstrcpy( szMsg1, STR_ID_NPGG_CALLBACK_ERROR01 );
            //lstrcpy( szMsg2, L"" );
            return FALSE;
        }
        case NPGAMEMON_INIT_ERROR:
        {
            ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_NPGG_CALLBACK_ERROR02 ).c_str() );
            ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_NPGG_TERMINATE_PROGRAM ).c_str() );
            break;
        }
        case NPGAMEMON_SPEEDHACK:
        {
            ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_NPGG_CALLBACK_ERROR03 ).c_str() );
            ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_NPGG_TERMINATE_PROGRAM ).c_str() );
            break;
        }
        case NPGAMEMON_GAMEHACK_KILLED:
        {
            ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_NPGG_CALLBACK_ERROR04 ).c_str() );
            ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_NPGG_TERMINATE_PROGRAM ).c_str() );
            break;
        }
        case NPGAMEMON_GAMEHACK_DETECT:
        {
            ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_NPGG_CALLBACK_ERROR05 ).c_str() );
            ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_NPGG_TERMINATE_PROGRAM ).c_str() );
            break;
        }
        case NPGAMEMON_GAMEHACK_DOUBT:
        {
            ::lstrcpyW( szMsg1, g_pkStrLoader->GetString( STR_ID_NPGG_CALLBACK_ERROR07 ).c_str() );
            ::lstrcpyW( szMsg2, g_pkStrLoader->GetString( STR_ID_NPGG_TERMINATE_PROGRAM ).c_str() );
            break;
        }
        default:
        {
            return TRUE;
        }
    }

    char cTemp[MAX_PATH];
    sprintf( cTemp, "%d", dwMsg );
    g_kGlobalValue.RunLogProgram("19", cTemp);

	MessageBox(NULL,szMsg1,szMsg2,MB_OK|MB_TOPMOST);
 	ExitWithHackAlert( szMsg1 ); 
    return FALSE;
#endif // NO_GAMEGUARD
}
