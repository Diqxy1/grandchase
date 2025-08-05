#include "stdafx.h"
//
#include "GCChatManager.h"
//
#include "MyD3D.h"

#include "GCUI/GCChatBox.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/KGCSquareOverlayUI.h"
//
//
#include "SlangFilter.h"
//

#include "KStats.h"
#include "GCStateMachine.h"
#include "gcui/KGCGuildLobbyDlg.h"


#if defined (USE_AGIT_SYSTEM )
#include "GCUI/GCAgitOverlayUI.h"
#endif

#if defined( USE_SOCIAL_MOTION )
#include "KGCSocialMotionManager.h"
#endif

#include "KGCAnnounceMgr.h"
#include "GCAchieveManager.h"
#include "Monster.h"

KGCChatManager* g_pkChatManager = NULL;

KGCChatManager::KGCChatManager( void )
{
    ASSERT( g_pkChatManager == NULL && "KGCChatManager instance is only one!" );

    g_pkChatManager = this;
    m_bWhisperMode  = false;
    m_iWhisperIdx   = 0;
    m_bWhisperFilter = false;
    //for test
    //m_bWhisperFilter = true;

    m_vecWhisperID.clear();
    m_cCurrentUserGrade = 0;
    m_RestrictData.first= 0;
    m_RestrictData.second = 0;
	m_eCmd = CHAT_CMD_NORMAL;
    BindToLua();
}

KGCChatManager::~KGCChatManager( void )
{
    g_pkChatManager = NULL;
}

// [8/2/2007 breadceo] Log Type °áÁ¤
GCPairPlaySystem::PPS_CHAT_TYPE KGCChatManager::GetChatLogType()
{
    switch ( g_MyD3D->m_pStateMachine->GetState() )
    {
    case GS_GAME:
        return GCPairPlaySystem::PPSCT_GAME;
        break;
    case GS_ROOM:
    case GS_MY_INFO_FROM_ROOM:
    case GS_CHAR_SELECT_FROM_ROOM:
        return GCPairPlaySystem::PPSCT_ROOM;
        break;
    case GS_GUILD:
        return GCPairPlaySystem::PPSCT_GUILD;
        break;
    case GS_SHOP_CASH:
#ifndef DISABLE_MAP_SHOP // Kawan>
    case GS_SHOP_GP:
#endif
    case GS_MY_INFO:
        if( g_MyD3D->m_pStateMachine->GetPRevChattingState() == GS_GUILD )
            return GCPairPlaySystem::PPSCT_GUILD;
        else
            return GCPairPlaySystem::PPSCT_LOBBY;
        break;
    default:
        return GCPairPlaySystem::PPSCT_LOBBY;
        break;
    }
}

void KGCChatManager::ChatMsgProc(const std::wstring& strNickColor, const std::wstring& strNick, std::wstring& strMsg, char cChatType, DWORD dwColor /*= 0xffffffff */)
{
    bool bIsColorEnabled = false;
    KItem* chatPaintItem = g_pItemMgr->GetInventoryItemFromID(ITEM_VIP_TEMPORARY_ITEM);
    if (chatPaintItem != NULL)
        bIsColorEnabled = true;

    std::wstringstream strTemp;
    std::wstring strNickColorTemp;
    bool bClickNick=false;

    if (strNickColor.size() < 6)
        strNickColorTemp = L"FFFFFF";
    else
        strNickColorTemp = strNickColor;

    DWORD dwStrColor = 0xffffffff;	

    int iChatType = CHAT_TYPE_NORMAL;
    GCPairPlaySystem::PPS_CHAT_TYPE eType = GetChatLogType();

#ifdef __ORIGIN_DEV__
	if( g_MyD3D->m_bLuaInputMode )
	{
        std::string strMssage = GCUTIL_STR::GCStrWideToChar( strMsg.c_str() );
		if( FAILED(KGCLuabinder::getInstance().DoString( strMssage.c_str(), strlen(strMssage.c_str()) ) ) )
		{
			strMsg = L"Lua Error!!";
			std::cout<<"Lua Error "<<std::endl;
		}
	}
#endif
    if(g_pkChatManager->CheckChatBlockUser() )
        return;

	switch( cChatType )
	{
	case KChatData::MSG_COMMON:
		{	
            // [8/2/2007 breadceo] Log°¡ ÇÊ¿äÇÑ ºÎºÐ¸¸ ³²±è
            SiGCPairPlaySystem()->AddChatLog( eType, strNick, strMsg );
            //¸Þ½ÃÁö¸¦ ÇÑ¹ø ÇÊÅÍ¸µ ÇØÁØ´Ù. (¿å¼³°ú #C ¿É¼ÇÀ» ¾ø¾ÖÁÜ )
            MessageFilter( strMsg );
            strTemp << L"#c" + strNickColorTemp + strNick + L"#cX" << L":" << strMsg;
            bClickNick=true;

            dwStrColor = CHAT_TYPE_NORMAL;

			break;
		}		
	case KChatData::MSG_COLOR_CHAT:
		{			
            SiGCPairPlaySystem()->AddChatLog( eType, strNick, strMsg );
            MessageFilter(strMsg);
            strTemp << L"#c" + strNickColorTemp + strNick + L"#cX" << L":" << L"#c" << SiGCTextColorManager()->ColorToString(dwColor) << strMsg << L"#cX";
            bClickNick=true;
            iChatType=CHAT_TYPE_COLORCHAT;
			break;
		}
	case KChatData::MSG_WHISPER:
		{
            SiGCPairPlaySystem()->AddChatLog( eType, strNick, strMsg );

            //¸Þ½ÃÁö¸¦ ÇÑ¹ø ÇÊÅÍ¸µ ÇØÁØ´Ù. (¿å¼³°ú #C ¿É¼ÇÀ» ¾ø¾ÖÁÜ )
            MessageFilter( strMsg );

			std::wstring input;
			// ±Ó¼Ó¸» Â÷´ÜÀÌ ¼³Á¤µÇ¾î ÀÖÀ¸¸é ¹ÞÁö ¾ÊÀ½
			if( m_bWhisperFilter )
			{
				strTemp<<g_pkStrLoader->GetReplacedString( STR_ID_CAN_NOT_RECV_WHISPER, "L", strNick );
				iChatType = CHAT_TYPE_ALERT;	
				g_KDSound.Play( "72" );
				break;
			}
			else 
			{
                strTemp << L">>[" << L"#c" + strNickColorTemp + strNick + L"#cX" << "]:" << strMsg;
				iChatType = CHAT_TYPE_WHISPER;	

				// ±Ó¸»À» º¸³½ »ç¶÷ÀÇ ID¸¦ ÀúÀåÇÕ´Ï´Ù.
				// ¹Ù·Î ´äº¯ÇÒ ¼ö ÀÖ°Ô..
                bClickNick=true;
				AddWhisperID( strNick );
			}

			g_KDSound.Play( "72" );
			break;      
		}
	case KChatData::MSG_WHISPER_EX:    // ÇÊÅÍ¸µ ÇÒ ¼ö ¾ø´Â ±Ó¸» : ex ¿î¿µÀÚ ±Ó¸»
		{
            strTemp<<L">>["<< L"#c" + strNickColorTemp + strNick + L"#cX" <<"]:"<<strMsg;
			iChatType = CHAT_TYPE_ADMIN;	
			AddWhisperID( strNick );
			g_KDSound.Play( "72" );
			break;
		}
	case KChatData::MSG_NOTIFY:
		{
			strTemp<<strMsg;
			iChatType = CHAT_TYPE_ADMIN;
//#ifndef __PATH__
			KSignBoardData kRecv;
			kRecv.m_dwColor  = 0xffffffff;
			kRecv.m_dwSenderUID = 21312321;
			kRecv.m_eType = KSignBoardData::SBT_SERVER_ADMIN;
			kRecv.m_strMsg = strTemp.str().c_str();
			kRecv.m_strSenderNick = L"¿î¿µÀÚ´Ù";
			SiKGCAnnounceMgr()->ReserveAnnounce( kRecv );
//#endif

			break;
		}
	case KChatData::MSG_ADMIN_COMMON:
		{
			// [8/2/2007 breadceo] Log°¡ ÇÊ¿äÇÑ ºÎºÐ¸¸ ³²±è
			SiGCPairPlaySystem()->AddChatLog( eType, strNick, strMsg );
            strTemp << L"#c" + strNickColorTemp + strNick + L"#cX" << L":" << strMsg;
			bClickNick=true;
			iChatType = CHAT_TYPE_COLORCHAT;
			
			break;
		}
	case KChatData::MSG_TEAM_CHAT: //zstaiji - ÆÀÃ¤ÆÃ
	case KChatData::MSG_TEAM_COLOR: //zstaiji - ÆÀÃ¤ÆÃ + gcÅ¬·´
		{
            MessageFilter( strMsg );
            strTemp << L"#c" + strNickColorTemp + strNick + L"#cX" << L":" << strMsg;
			iChatType = CHAT_TYPE_TEAM;
			break;
		}
    case KChatData::MSG_GUILD:
        {
            MessageFilter( strMsg );
            strTemp << L"#c" + strNickColorTemp + strNick + L"#cX" << L":" << strMsg;
            bClickNick=true;
            iChatType = CHAT_TYPE_GUILD;
            break;
        }
	}

    //ÀÚ±â ÀÚ½ÅÀÎ °æ¿ì, Å¬¸¯ ¾ÈµÇ°Ô ¼öÁ¤
    if (strNick==g_kGlobalValue.m_kUserInfo.strNickName)
    {
        bClickNick=false;
    }

    if (bClickNick==false)		
    {
        AddChatMsg( strTemp.str(), iChatType, dwColor );
    }
    else
    { // Å¬¸¯ µÇ´Â °æ¿ì
        AddChatMsg( strTemp.str(), iChatType, dwColor, false, strNick );
    }
}

void KGCChatManager::AddChatMsg( const std::wstring& strMsg, std::wstring strReturnMsg) const
{
    AddChatMsg( strMsg, KGCChatManager::CHAT_TYPE_NORMAL, 0xffffffff, false, strReturnMsg );
}

void KGCChatManager::AddChatMsg( const std::wstring& strMsg, int iChatType/* = CHAT_TYPE_NORMAL*/,
                                DWORD dwColor/* = 0xffffffff*/, bool bNoSound/* = false*/, std::wstring  strReturnMsg/* = L""*/ ) const
{
    if( strMsg == g_pkStrLoader->GetString( STR_ID_TOO_MANY_CHAT ) ) {
        SiKGCAchieveManager()->OccurAction( SAchieve::CHAT_TOO_MANY, GC_CHAR_INVALID );
    }
    AddChatMsgTo( g_MyD3D->m_pStateMachine->GetState(), strMsg, iChatType, bNoSound, strReturnMsg, dwColor );
}

void KGCChatManager::AddSystemMsg( int iIndex, const std::wstring& strMsg, int iChatType /* = CHAT_TYPE_RESULT */) const
{
    if ( 0 <= iIndex && iIndex < MAX_PLAYER_NUM )
    {
        std::wostringstream strmSystem;
        strmSystem << L"#c" + g_MyD3D->MyPlayer[iIndex]->GetStrUserColor() + g_MyD3D->MyPlayer[iIndex]->GetStrUserName() + L"#cx" << strMsg;
        AddChatMsg( strmSystem.str(), iChatType );
    }
    else
    {
        AddChatMsg( strMsg, iChatType );
    }

    g_KDSound.Play( "72" );
}

void KGCChatManager::AddMacroCommunityMsg( DWORD dwUID, const std::wstring& strMsg, int iSoundID, DWORD dwColor ) const
{
    PLAYER *pPlayer = g_MyD3D->GetPlayerByUID( dwUID );
    if( pPlayer )
    {
        std::wostringstream strmSystem;
        strmSystem << L"#c" + pPlayer->GetStrUserColor() + pPlayer->GetStrUserName() + L"#cx" << L":" << strMsg;
        AddChatMsg( strmSystem.str(), KGCChatManager::CHAT_TYPE_MACRO_COMMUNITY, dwColor, true );
    }
    else
    {
        AddChatMsg( strMsg, KGCChatManager::CHAT_TYPE_MACRO_COMMUNITY, dwColor, true );
    }

    if( -1 != iSoundID )
        g_KDSound.Play( iSoundID, 0, LATENCY_BUFFER_SIZE );
}

const std::wstring& KGCChatManager::OnWhisperInputStart( void )
{
    if ( m_vecWhisperID.size() > 0 )
    {
        m_strCommand = L">"+ m_vecWhisperID.back() + L" ";
    }
    else
    {
        m_strCommand.erase();
    }

    m_bWhisperMode = true;
    m_iWhisperIdx = (int)m_vecWhisperID.size() - 1;

    return m_strCommand;
}

const std::wstring& KGCChatManager::OnWhisperScrollUp( void )
{
    if ( m_vecWhisperID.size() > 0 )
    {
        if ( --m_iWhisperIdx < 0 ) m_iWhisperIdx = 0;

        m_strCommand = L">"+ m_vecWhisperID[m_iWhisperIdx] + L" ";
    }
    else
    {
        m_strCommand.erase();
    }

    return m_strCommand;
}

const std::wstring& KGCChatManager::OnWhisperScrollDown( void )
{
    if ( m_vecWhisperID.size() > 0 )
    {
        if ( ++m_iWhisperIdx >= static_cast<int>( m_vecWhisperID.size() ) )
        {
            m_iWhisperIdx = (int)m_vecWhisperID.size() - 1;
        }

        m_strCommand = L">" + m_vecWhisperID[m_iWhisperIdx] + L" ";
    }
    else
    {
        m_strCommand.erase();
    }

    return m_strCommand;
}

D3DCOLOR KGCChatManager::GetColorFromTable( int iChatType, DWORD dwColor ) const
{

    switch ( iChatType )
    {
        case CHAT_TYPE_RESULT:  
			return D3DCOLOR_XRGB( 255, 186, 0 );  // °á°ú¾Ë¸²
        case CHAT_TYPE_ALERT:   
			return D3DCOLOR_XRGB( 255, 186, 0 );  // °æ°í¸»/¾Ë¸²
        case CHAT_TYPE_ADMIN:   
#if defined( NATION_PHILIPPINE )
			return D3DCOLOR_XRGB( 252, 255, 0 );    // ¿î¿µÃ¤ÆÃ
#else
            return D3DCOLOR_XRGB( 255, 186, 0 );    // ¿î¿µÃ¤ÆÃ
#endif
        case CHAT_TYPE_NORMAL:  
			return D3DCOLOR_XRGB( 254, 254, 254 );  // ÀÏ¹ÝÃ¤ÆÃ
        case CHAT_TYPE_INVITE:  
        case CHAT_TYPE_SCREEN_SHOT:
			return D3DCOLOR_XRGB( 255, 186, 0 );  // ÃÊ´ëÃ¤ÆÃ
        case CHAT_TYPE_WHISPER: 
			return D3DCOLOR_XRGB( 0, 186 , 255);   // ±Ó¼Ó¸»/±æµåÀü°á°ú
        case CHAT_TYPE_SIX:     
			return D3DCOLOR_XRGB( 255, 186, 0 );
        case CHAT_TYPE_KILLINFO:
            return D3DCOLOR_XRGB( 255, 186, 0 );
		case CHAT_TYPE_AUTO:     
			return D3DCOLOR_XRGB( 255, 186, 0 );
        case CHAT_TYPE_GUILD:        
            return D3DCOLOR_XRGB( 204, 0, 255 );
        case CHAT_TYPE_COLORCHAT:
		case CHAT_TYPE_TEAM:
            return D3DCOLOR_XRGB( 255, 255, 255 );
    }

    return D3DCOLOR_XRGB( 255, 255, 255 );
}

KGCChatManager::EChatCommand KGCChatManager::FindChatType( const std::wstring& strCommand )
{
	if ( strCommand.empty() ) return CHAT_CMD_NORMAL;

    EChatCommand eCmd = CHAT_CMD_ERROR;
    WCHAR Input_Text[MAX_CHAT_STR_BUF_SIZE] = { 0, };
    StringCchCopyW( Input_Text, MAX_CHAT_STR_BUF_SIZE, strCommand.c_str() );

    // Kawan> ?
    //if ( Input_Text[0]==L':' && Input_Text[1]==L':' && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) 
    //    eCmd = CHAT_CMD_ADMIN;

    if ( Input_Text[0]==L'!' && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) 
        eCmd = CHAT_CMD_BROADCAST;

    if ( Input_Text[0]==L'$' && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
        eCmd = CHAT_CMD_NOTICE;

    if ( Input_Text[0]==L'/' && (Input_Text[1]==L't' || Input_Text[1]==L'T'
		|| Input_Text[1] == L'¤µ'
		) && Input_Text[2]==L' ') //±Ó¼Ó¸»
        eCmd = CHAT_CMD_WHISPER;

    if ( Input_Text[0]==L'/' && (Input_Text[1]==L'g' || Input_Text[1]==L'G'
		|| Input_Text[1]==L'¤¾'
		) && Input_Text[2]==L' '
		&& g_kGlobalValue.m_kGuildInfo.m_dwUID !=0 &&g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_JOINER) //±æµå
        eCmd = CHAT_CMD_GUILD;

#if defined( ENABLE_TEAM_CHAT)
    if ( Input_Text[0]==L'%' ) //zstaiji - ÆÀÃ¤ÆÃ 
        eCmd = CHAT_CMD_TEAM;
#endif
	if ( Input_Text[0]==L'/' && (Input_Text[1]==L's' || Input_Text[1]==L'S'
		|| Input_Text[1]==L'¤¤'
		) && Input_Text[2]==L' ')
		eCmd = CHAT_CMD_NORMAL;
	
    return eCmd;
}

bool KGCChatManager::GetSplitExpCommand(int iIndex, std::wstring strCommand, OUT std::wstring& strNumber )
{
    bool bIsNumber = true;
    strNumber = L"";

    if(L'-' == strCommand[iIndex])
    {
        ++iIndex;
        strNumber += L'-';
    }

    for(int i = iIndex ; i < (int)strCommand.size() ; ++i )
    {
        if( !iswdigit(strCommand[i] ) )
        {
            bIsNumber = false;
            break;
        }
        strNumber += strCommand[i];
    }

    return bIsNumber ? true : false;
}


KGCChatManager::EChatCommand KGCChatManager::AnalyzeChatCommand( const std::wstring& strCommand )
{
    EChatCommand eCmd = CHAT_CMD_NORMAL;
    WCHAR Input_Text[MAX_CHAT_STR_BUF_SIZE] = { 0, };
    m_bWhisperMode = false;

    StringCchCopyW( Input_Text, MAX_CHAT_STR_BUF_SIZE, strCommand.c_str() );

    // Ã¤ÆÃ ÀÌº¥Æ® ( Æ¯Á¤ ´Ü¾î¸¦ ¹æ¿øµéÀÌ ÀÔ·Â½Ã ¹ßµ¿ )
    if( ( g_kGlobalValue.m_kUserInfo.nUserState == GC_RUS_READY || g_kGlobalValue.m_kUserInfo.bHost ) &&    // ·¹µðÇÑ »óÅÂÀÌ°Å³ª ¹æÀåÀÏ¶§
        g_kGlobalValue.m_kChattingEvent.CheckChatting( strCommand ) )                   // ´Ü¾î¸¦ ÀÔ·ÂÇß´Â°¡?
        KP2P::GetInstance()->Send_Chatting_Event_Correct_Not( g_kGlobalValue.m_kChattingEvent.GetUID() );

    eCmd=FindChatType(strCommand);
	if ( eCmd == CHAT_CMD_ERROR ) {
		eCmd = m_eCmd;
	}
   
///////////////////////////////////////////////////////////////////////////////

#ifdef __ORIGIN_DEV__
	if (-1 == strCommand.find(L'/', 1) && -1 != strCommand.find(L"/crash") ){
        throw "";
        return CHAT_CMD_DEVCONSOLE;
    }
    if( -1 == strCommand.find(L'/', 1) && -1 != strCommand.find(L"/logmemory") ) 
    {
        FILE* pFile = NULL;
        pFile = fopen( "MemoryLog.txt", "wb" );
        if ( pFile )
        {
            int nMemorySize = 0;
            nMemorySize = g_pkUIScene->GetMemorySize();
            fprintf( pFile, "UI Object : %dKB\n", (nMemorySize/(1024)) );

            nMemorySize = g_ParticleManager->GetMemorySize();
            fprintf( pFile, "Particle Object : %dKB\n", (nMemorySize/(1024)) );


            int nTotal = 0;
            int nLoad = 0;
            g_ParticleManager->GetLoadedTexture( nTotal, nLoad );
            fprintf( pFile, "Paritcle Texture : %d/%d\n", nLoad, nTotal );

            fclose( pFile );

            g_ParticleManager->LogUseParticleList();

            g_pkChatManager->AddChatMsg( L"MemeryLog Success( MemoryLog.txt )", KGCChatManager::CHAT_TYPE_ALERT );
        }   


        return CHAT_CMD_DEVCONSOLE;
    }
#if defined (NATION_KOREA)
    if( -1 == strCommand.find(L'/', 1) && ( -1 != strCommand.find(L"/hack") ) )
    {
        int nIndex = g_MyD3D->Get_MyPlayer();

        std::vector<std::wstring> vecString;
        GCUTIL_STR::Tokenizer( strCommand, L" ", vecString );

        if ( vecString.size() > 2 ) 
        {
            nIndex = boost::lexical_cast<DWORD>( vecString[1] );
        }

        KChangeRoomUserInfo kRoomUserInfo;
        g_MyD3D->MyPlayer[nIndex]->m_kUserInfo.SetChangeRoomUserInfo( &kRoomUserInfo, KChangeRoomUserInfo::RUI_CHAR );
        kRoomUserInfo.m_strLogin = g_MyD3D->MyPlayer[0]->m_kUserInfo.strLogin;
        kRoomUserInfo.m_nUserSlot = nIndex;

        KP2P::GetInstance()->Send_ChangeRoomUserInfoReq( kRoomUserInfo );
    }
#endif
#endif 
	if( -1 == strCommand.find(L'/', 1) && ( -1 != strCommand.find(L"/additem ") || -1 != strCommand.find(L"/i ") ) && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) 
    {
        std::vector<std::wstring> vecString;
        GCUTIL_STR::Tokenizer( strCommand, L" ", vecString );

        if ( vecString.size() < 2 ) 
        {
            g_pkChatManager->AddChatMsg( L"Faild AddItem", KGCChatManager::CHAT_TYPE_ALERT );
            return CHAT_CMD_DEVCONSOLE;
        }

        try
        {
            GCITEMID itemID = boost::lexical_cast<DWORD>( vecString[1] );
            
            int nPeriod = -1;
            int nDuration = -1;
            int nGrade = -1;
            int nItemLevel = -1;

            if ( vecString.size() >= 3 ) 
            {
                nPeriod = boost::lexical_cast<int>( vecString[2] );
            }

            if ( vecString.size() >= 4 ) 
            {
                nDuration = boost::lexical_cast<int>( vecString[3] );
            }

            if ( vecString.size() >= 5 ) 
            {
                nGrade = boost::lexical_cast<int>( vecString[4] );
            }

            if ( vecString.size() >= 6 ) 
            {
                nItemLevel = boost::lexical_cast<int>( vecString[5] );
            }

            std::wstring strTemp = boost::str(boost::wformat(L"%1% %2% %3% %4% %5% %6%")%vecString[0]%itemID%nPeriod%nDuration%nGrade%nItemLevel );
            g_pkChatManager->AddChatMsg( strTemp, KGCChatManager::CHAT_TYPE_ALERT );

            KP2P::GetInstance()->Send_AddItem( itemID, nPeriod, nDuration, nGrade, nItemLevel );       
        }
        catch( boost::bad_lexical_cast& ) 
        {
            g_pkChatManager->AddChatMsg( L"Faild AddItem", KGCChatManager::CHAT_TYPE_ALERT );
        }

        return CHAT_CMD_DEVCONSOLE;
    }

	if( -1 == strCommand.find(L'/', 1) && -1 != strCommand.find(L"/addexp ") 
#if defined(__PATH__)
		&& g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN
#endif
		)
	{
		bool bIsNumber = true;

		std::wstring strAddExpNumber = L"";
		int iIndex = 8;

		if(L'-' == strCommand[8])
		{
			++iIndex;
			strAddExpNumber += L'-';
		}

		for(int i = iIndex ; i < (int)strCommand.size() ; ++i )
		{
			if( !iswdigit(strCommand[i] ) )
			{
				bIsNumber = false;
				break;
			}
			strAddExpNumber += strCommand[i];
		}

		if(bIsNumber)
		{
			__int64 biAddExp = _wtoi64( strAddExpNumber.c_str() );
            biAddExp = std::min<__int64>( 1865394200, biAddExp );
            
			KP2P::GetInstance()->Send_AddExp(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, biAddExp);
		}

		return CHAT_CMD_DEVCONSOLE;
	}


	if( -1 == strCommand.find(L'/', 1) && -1 != strCommand.find(L"/setexp ") 
#if defined(__PATH__)
		&& g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN
#endif
		)
	{
        std::wstring strSetExpNumber;
        if( GetSplitExpCommand(8, strCommand, strSetExpNumber) )
		{
			__int64 biSetExp = _wtoi64( strSetExpNumber.c_str() );
			biSetExp = std::min<__int64>( 1865394200, biSetExp );

			KP2P::GetInstance()->Send_SetExp(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, biSetExp);
		}

		return CHAT_CMD_DEVCONSOLE;
    }
    //·¹º§ ¼³Á¤
    if( -1 == strCommand.find(L'/', 1) && -1 != strCommand.find(L"/lv ") 
#if defined(__PATH__)
        && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN
#endif
        )
    {
        std::wstring strSetLvNumber;
        if( GetSplitExpCommand(4, strCommand, strSetLvNumber) )
        {
            int iSetlv = _wtoi( strSetLvNumber.c_str() ) - 1;
            __int64 biSetExp = Level_2_Exp(iSetlv);
            biSetExp = std::min<__int64>( 1865394200, biSetExp );

            KP2P::GetInstance()->Send_SetExp(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, biSetExp);
        }

        return CHAT_CMD_DEVCONSOLE;
    }
    //·¹º§¾÷ Á÷Àü »óÅÂ ¼³Á¤
    if( -1 == strCommand.find(L'/', 1) && -1 != strCommand.find(L"/lvs ") 
#if defined(__PATH__)
        && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN
#endif
        )
    {
        std::wstring strSetLvNumber;
        if( GetSplitExpCommand(5, strCommand, strSetLvNumber) )
        {
            int iSetlv = _wtoi( strSetLvNumber.c_str() ) - 1;
            __int64 biSetExp = Level_2_Exp(iSetlv) - 1;
            biSetExp = std::min<__int64>( 1865394200, biSetExp );

            KP2P::GetInstance()->Send_SetExp(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, biSetExp);
        }

        return CHAT_CMD_DEVCONSOLE;
    }

    if( -1 == strCommand.find(L'/', 1) && -1 != strCommand.find(L"/replayopen") 
#if defined(__PATH__)
        && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN
#endif
        )
    {
        g_kGlobalValue.m_bUseOldReplay = !g_kGlobalValue.m_bUseOldReplay;
    }

#if defined( NATION_BRAZIL)
	if(L'/' == Input_Text[0])
	{
		//wstring wstrBlockString = L'/' + g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT9 ) + L' ';
		wstring wstrBlockString = L"/";
		wstrBlockString += L"bloquear";
		wstrBlockString += L" ";


		if(-1 != strCommand.find(wstrBlockString))
		{
			size_t BlockStringLength = wstrBlockString.size();
			wstring wstrCommandTemp = strCommand;
			wstring wstrCommandTemp2 = (wstrCommandTemp.substr(BlockStringLength, wstrCommandTemp.size() - BlockStringLength)).c_str();
			
			g_kGlobalValue.ADD_BLOCK_USER( wstrCommandTemp2);
		}

		wstring wstrUnBlockString = L"/";
		wstrUnBlockString += L"desbloquear";
		wstrUnBlockString += L" ";
		//wstring wstrUnBlockString = L'/' + g_pkStrLoader->GetString(STR_ID_FAIRPLAY_REPORT10) + L' ';

		if(-1 != strCommand.find(wstrUnBlockString))
		{
			size_t UnBlockStringLength = wstrUnBlockString.size();
			wstring wstrCommandTemp = strCommand;
			wstring wstrCommandTemp2 = (wstrCommandTemp.substr(UnBlockStringLength, wstrCommandTemp.size() - UnBlockStringLength )).c_str();

			g_kGlobalValue.DEL_BLOCK_USER( wstrCommandTemp2 );
		}
	}
#else
    //////////////////////////////////////////////////////////////////////////
    //¿ÀÈÄ 8:22 2008-02-28 jemitgge - Â÷´Ü±â´É Ãß°¡!
    if ( Input_Text[0]==L'/' && Input_Text[1]==L'Â÷' 
        && Input_Text[2]==L'´Ü' && Input_Text[3]==L' ' )
    {
        if ( NULL != Input_Text[4] && L' ' != Input_Text[4] )
        {
			WCHAR pMsg[MAX_CHAT_STR_BUF_SIZE] = {0,};
            wcscpy( pMsg, Input_Text );
            WCHAR seps[] = L" \0";
            WCHAR* token = NULL;
            token = wcstok( &pMsg[4], seps );
            if ( NULL != token)
            {
                std::wstring tmp = token;
                g_kGlobalValue.ADD_BLOCK_USER( tmp );
                return CHAT_CMD_DEVCONSOLE;
            }
        }
    }

    if ( Input_Text[0]==L'/' && Input_Text[1]==L'Â÷' 
        && Input_Text[2]==L'´Ü' && Input_Text[3]==L'ÇØ'
        && Input_Text[4]==L'Á¦' && Input_Text[5]==L' ')
    {
        if ( NULL != Input_Text[6] && L' ' != Input_Text[6] )
        {
			WCHAR pMsg[MAX_CHAT_STR_BUF_SIZE] = {0,};
            wcscpy( pMsg, Input_Text );
            WCHAR seps[] = L" \0";
            WCHAR* token = NULL;            
            token = wcstok( &pMsg[6], seps );
            if ( NULL != token)
            {
                std::wstring tmp = token;
                g_kGlobalValue.DEL_BLOCK_USER( tmp );
                return CHAT_CMD_DEVCONSOLE;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
#endif

#if defined( USE_SOCIAL_MOTION )
    std::wstring strSocialMotionCommand = Input_Text;
    if( true == SiKGCSocialMotionManager()->IsSocialMotionCommand( strSocialMotionCommand ) ) {
        return CHAT_CMD_SOCIAL_MOTION;
    }
#endif

	switch ( eCmd )
	{
        // Kawan> ?
        /*case CHAT_CMD_ADMIN:
        {
            bool Request_Server=true;
			
			WCHAR Text_Command[8] = {0,};
            
            for(int i=0;i<7;i++) 
				Text_Command[i]=Input_Text[i+2];

            Text_Command[7]=NULL;

            // ±× Àü¿¡ Å¬¶óÀÌ¾ðÆ® ¾È¿¡¼­ Ã³¸®ÇÒ ¼ö ÀÖ´Â ³»¿ëÀÎÁö º»´Ù.
			int iAdminLevel = g_kGlobalValue.m_kUserInfo.eGCUserLevel;
            if( iAdminLevel == USERLEVEL_ADMIN || iAdminLevel == USERLEVEL_HIGH )
            {
                    }
            break;
        }*/
		case CHAT_CMD_BROADCAST:
			{
				std::wostringstream strmChatMsg;
#ifdef NEW_GM_BROADCAST
                strmChatMsg << g_kGlobalValue.m_kUserInfo.strNickName << L': ' << &Input_Text[1];
#else
				strmChatMsg << g_pkStrLoader->GetString( STR_ID_ADMIN_MESSAGE_TAG ) << L': ' << &Input_Text[1];
#endif
				KP2P::GetInstance()->Send_NotifyMessageNot( L"Server", strmChatMsg.str());
				break;
			}
        case CHAT_CMD_WHISPER:
        {
			// ¿î¿µÀÚ¶ó¸é ¹Þ´Â°É Â÷´ÜÇØµµ º¸³»±â¸¦ ÇÒ ¼ö ÀÖÀ½.
			bool isAdmin = ( g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) ? true : false;
			
			// ±Ó¼Ó¸» Â÷´Ü±â´ÉÀÌ ¼³Á¤µÇ¾î ÀÖÀ¸¸é º¸³»Áö ¾ÊÀ½
			// ( ¹Þ´Â°É Â÷´ÜÇÏ°í ±Ó¼Ó¸» º¸³»´Â ÇàÀ§¸¦ ¹æÁö )
			if( m_bWhisperFilter && !isAdmin )
			{
				AddChatMsg( g_pkStrLoader->GetString(STR_ID_NOTIFY_WHISPER_FILTER_MODE), KGCChatManager::CHAT_TYPE_ALERT );
				break;
			}

            std::wstring strWhisper = Input_Text;
            std::wstring::size_type pos;

			if ( std::wstring::npos == ( pos = strWhisper.find( L' ' ) ) ) 
				return CHAT_CMD_NORMAL;

            std::wstring strID = strWhisper.substr( 1, pos - 1 );
            std::wstring strText = strWhisper.substr( pos+1 );

            // ¿ÀÈÄ 12:17 2008-03-21 by jemitgge
            // ¾Æ¹«¸»µµ ¾ÈÃÆÀ» °æ¿ì¿¡´Â ÆÐÅ¶ º¸³»Áö ¾Ê½À´Ï´Ù.
			if( strText.size() >= 1 )
				KP2P::GetInstance()->Send_WhisperReq( L"Server", strID.c_str(), strText.c_str() );

			AddWhisperID( strID );

            m_bWhisperMode = true;
            break;
        }
        case CHAT_CMD_NOTICE:
            {
                if( Input_Text[1] != L'1' && Input_Text[1] != L'2' && Input_Text[1] != L'3' && Input_Text[2] != L' ' )
                {
                    break;
                }

                int nType = KSignBoardData::SBT_SERVER_ADMIN;
                if( Input_Text[1] == L'1' )
                {
                    nType = KSignBoardData::SBT_SERVER_ADMIN;
                }
                else if( Input_Text[1] == L'2' )
                {
                    nType = KSignBoardData::SBT_SQUARE_ADMIN;
                }
				else if( Input_Text[1] == L'3' )
					nType = KSignBoardData::SBT_SERVER_ADMIN_EX_GAME;

                std::wostringstream strmChatMsg;
                strmChatMsg << g_pkStrLoader->GetString( STR_ID_ADMIN_MESSAGE_TAG )
                    << L':' << &Input_Text[3];

                KP2P::GetInstance()->Send_AdminSignBoardNot( nType, strmChatMsg.str() );
                break;
            }
    }
    return eCmd;
}

void KGCChatManager::AddWhisperID(const std::wstring& strID)
{
	std::vector<std::wstring>::iterator itor = m_vecWhisperID.begin();
	std::vector<std::wstring>::iterator end = m_vecWhisperID.end();
	while(itor != end)
	{
		if( 0 == wcsicmp((*itor).c_str(), strID.c_str() ) )
		{
			itor = m_vecWhisperID.erase( itor );
			break;
		}else
		{
			++itor;
		}
	}


	// º¤ÅÍ¿¡ Ãß°¡
	m_vecWhisperID.push_back( strID );
}

void KGCChatManager::RemoveSpaceChar( std::wstring& strMessage )
{
    std::wstring::size_type pos,  // where we are now
        found;    // where the found data is
    std::wstring str;

    //0000 - 0020 ±îÁö »èÁ¦
    for (int i=0;i<=32;i++)
    {
        pos = 0;
        str=(WCHAR)i;
        while ((found = strMessage.find(str, pos)) != string::npos)
        {
            strMessage.replace (found, str.size(), L" ");
            pos = found+str.size();
        }
    }  
    GCUTIL_STR::Trim(strMessage);
    return;
}

void KGCChatManager::MessageFilter( std::wstring& strMessage )
{
    std::wstring strFiltered = SiKSlangFilter()->Filter( strMessage, KSlangFilter::SLANG_FILTER_CHAT );

    if( strFiltered != strMessage ) {
        SiKGCAchieveManager()->OccurAction( SAchieve::CHAT_FILTER, GC_CHAR_INVALID );
    }

    RemoveColorKey( strFiltered );
    RemoveSpaceChar( strFiltered );

    strMessage = strFiltered;
}

void KGCChatManager::RemoveColorKey( std::wstring& strMessage )
{
    g_pkStrLoader->Replacer(strMessage, L"#c", L"");
    g_pkStrLoader->Replacer(strMessage, L"#C", L"");
}

void KGCChatManager::ClearAllChatMsg() //ÇØ»óµµ º¯°æÇÏ°í ³ª¸é ±ÛÂ¥Ã¢ ³¯·Á¹ö¸²..
{
	if (g_pkSquareChatBox != NULL)
		g_pkSquareChatBox->Clear();

	if (g_pkLobbyChatBox != NULL)
		g_pkLobbyChatBox->Clear();

	if (g_pkRoomChatBox != NULL)
		g_pkRoomChatBox->Clear();

	if (g_pkGuildChatBox != NULL)
		g_pkGuildChatBox->Clear();

	if (g_pkWorldmapChatBox != NULL)
		g_pkWorldmapChatBox->Clear();

#if defined( USE_AGIT_SYSTEM )
    if( g_pkAgitChatBox != NULL )
        g_pkAgitChatBox->Clear();
#endif
}


void KGCChatManager::AddChatMsgTo( int iStateID, const std::wstring& strMsg, int iMsgType /*= CHAT_TYPE_NORMAL*/, 
                                  bool bNoSound /*= false*/, std::wstring strReturnMsg /*= L"" */, DWORD dwColor/* = 0xffffffff*/ ) const
{
	if( g_MyD3D == NULL )
		return;
	if( g_MyD3D->m_pStateMachine == NULL )
		return;


    if( g_pkUIScene->m_pkGuildLobbyDlg->GetWndState() == KD3DWnd::D3DWS_ENABLED &&
        g_pkUIScene->m_pkGuildLobbyDlg->IsRenderOn() && NULL != g_pkGuildLobbyChatBox )
    {
        g_pkGuildLobbyChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
        if( !bNoSound )
            g_KDSound.Play( "72" );

        return;
    }

	switch ( iStateID )
	{
	case GS_GAME:
		{
#if defined( USE_AGIT_SYSTEM )
			if ( g_pkGameOverlayUI != NULL && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TUTORIAL )
			{
                if( iMsgType == CHAT_TYPE_MACRO_COMMUNITY )
                    g_pkGameOverlayUI->AddMacroCommunityChat( strMsg, iMsgType, dwColor );
                else
				    g_pkGameOverlayUI->AddChat( strMsg, iMsgType, dwColor );
				if( !bNoSound )
					g_KDSound.Play( "72" );
			}
            else if ( g_pkAgitOverlayUI != NULL && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
            {
                g_pkAgitChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
                if( !bNoSound )
                    g_KDSound.Play( "72" );
            }
            if ( g_pkGameOverlayUI != NULL && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL ) {
              
                if( iMsgType == CHAT_TYPE_WHISPER || iMsgType == CHAT_TYPE_GUILD ) { 
                    g_pkGameOverlayUI->AddChat( strMsg, iMsgType, dwColor );
                    if( !bNoSound )
                        g_KDSound.Play( "72" );
                }
            }
			break;
#else
			if ( g_pkGameOverlayUI != NULL )
			{
				g_pkGameOverlayUI->AddChat( strMsg, iMsgType, dwColor );
				if( !bNoSound )
					g_KDSound.Play( "72" );
			}
			break;
#endif
		}
	case GS_SQUARE:
		//±¤Àå ºÎºÐ
		if ( g_pkSquareOverlayUI != NULL )
		{
			g_pkSquareChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
			if( !bNoSound )
				g_KDSound.Play( "72" );
		}
		break;
	case GS_MATCH:
		{
			if ( g_pkLobbyChatBox != NULL )
			{
				g_pkLobbyChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
				if( !bNoSound )
					g_KDSound.Play( "72" );
			}
			break;
		}
	case GS_ROOM:
	case GS_MY_INFO_FROM_ROOM:
    case GS_CHAR_SELECT_FROM_ROOM:
		{
			if ( g_pkRoomChatBox != NULL )
			{
				g_pkRoomChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
				if( !bNoSound )
					g_KDSound.Play( "72" );
			}
			break;
		}
	case GS_GUILD:
		{
			if ( g_pkGuildChatBox != NULL )
			{
				g_pkGuildChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
				if( !bNoSound )
					g_KDSound.Play( "72" );
			}
			break;
		}
	case GS_SHOP_CASH:
#ifndef DISABLE_MAP_SHOP // Kawan>
    case GS_SHOP_GP:
#endif
	case GS_MY_INFO:
		{
			switch( g_MyD3D->m_pStateMachine->GetPRevChattingState() )
			{
			case GS_GUILD:
				{
					if ( g_pkGuildChatBox != NULL )
					{
						g_pkGuildChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
					}
					break;
				}
			case GS_MATCH:
				{
					if ( g_pkLobbyChatBox != NULL )
					{
						g_pkLobbyChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
					}
					break;
				}
			case GS_WORLDMAP:
				{
					if ( g_pkWorldmapChatBox != NULL )
					{
						g_pkWorldmapChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
					}
					break;
				}
			}
			break;
		}
	case GS_WORLDMAP:
		{
			if ( g_pkWorldmapChatBox != NULL )
			{
				g_pkWorldmapChatBox->AddChat( strMsg, iMsgType, strReturnMsg, dwColor );
				if( !bNoSound )
					g_KDSound.Play( "72" );
			}
			break;
		}
	}
}

void KGCChatManager::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)

    lua_tinker::class_add<KGCChatManager>( L, "KGCChatManager" );
    lua_tinker::class_def<KGCChatManager>( L,  "AddSystemMsg", &KGCChatManager::AddSystemMsg );

    lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "ChatMgr", this );
}

void KGCChatManager::SetChatUserState( KEVENT_BAD_USER_INFO_NOT& kEventInfo )
{
    m_cCurrentUserGrade = kEventInfo.m_cCurrentGrade;
    m_RestrictData.first= kEventInfo.m_nPeriodType;
    m_RestrictData.second = kEventInfo.m_tmLimit;
}

char KGCChatManager::GetCharUserGrade()
{
    return m_cCurrentUserGrade;
}

std::pair< int , time_t> KGCChatManager::GetRestrictData()
{
    return m_RestrictData;
};

bool KGCChatManager::CheckChatBlockUser()
{
    if ( m_cCurrentUserGrade == -4 ) {
        //¿µ±¸ Ã¤ÆÃ ºí·° À¯Àú¸é
        if (m_RestrictData.first == 1)
        {
            if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME )
                g_pkGameOverlayUI->AddChat(g_pkStrLoader->GetString( STR_CHATTING_RESTRICT_MSG1 ), KGCChatManager::CHAT_TYPE_ALERT ); 
            //¿µ±¸ ºí·° À¯Àú ÀÔ´Ï´Ù.
            else
                AddChatMsg( g_pkStrLoader->GetString( STR_CHATTING_RESTRICT_MSG1 ), KGCChatManager::CHAT_TYPE_ALERT );
            return true;
        }
        else
        {
            std::wstring strMsg;

            strMsg = g_pkStrLoader->GetReplacedString( STR_CHATTING_RESTRICT_MSG2, "iii", 
                GCUTIL_TIME::GetYear(m_RestrictData.second), GCUTIL_TIME::GetMonth(m_RestrictData.second), GCUTIL_TIME::GetDay(m_RestrictData.second) );
            
            if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME )
                g_pkGameOverlayUI->AddChat(strMsg, KGCChatManager::CHAT_TYPE_ALERT ); 
            //Ã¤ÆÃºí·° ±â°£ÀÌ ¸îÀÏ±îÁö ÀÔ´Ï´Ù. 
            else
                AddChatMsg( strMsg, KGCChatManager::CHAT_TYPE_ALERT );
            return true;
        }
    }
    return false;
}