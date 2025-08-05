#include "stdafx.h"
//
#include "KncP2PLib/KncP2P.h"
#include "GCUI/KGCServerBar.h"

//
//
#include "MyD3D.h"

//
#include "KSingleton.h"
#include "GCNetwork.h"
#include "VersionDef.h"
#include "Procedure.h"
#include "GCNickNameBox.h"
#include "GCNickNameCampaignBox.h"
#include "KGCChannelManager.h"
#include "GCNateOn.h"
#include "GCUI/GCNewNecklaceEnchantDlg.h"
#include "GCUI/GCFatigueInfoBox.h"

IMPLEMENT_CLASSNAME( KGCServerBar );
IMPLEMENT_WND_FACTORY( KGCServerBar );
IMPLEMENT_WND_FACTORY_NAME( KGCServerBar, "gc_server_bar" );

KGCServerBar::KGCServerBar(void)
{
    m_pkServerName = NULL;
    LINK_CONTROL( "server_name",    m_pkServerName );

    char strWndName[MAX_PATH] = {0,};
    for( int i = 0; i < NUM_SERVER_STATUS; ++i )
    {
		m_apkStatus[i] = NULL;
        sprintf( strWndName, "status_%d", i );
        LINK_CONTROL( strWndName, m_apkStatus[i] );
    }

    m_pkBalanceMark = NULL;
    LINK_CONTROL( "balance_mark", m_pkBalanceMark );

    m_pkMatchServerEdge = NULL;
    LINK_CONTROL( "match_server_edge", m_pkMatchServerEdge );

	m_iServerState = 0;

	m_bSendHackCheckPacket = false;
}

void KGCServerBar::OnCreate( void )
{

    m_pkServerName->InitState( true );
	m_pkServerName->SetFontColor( D3DCOLOR_ARGB(255, 255, 249, 217) );
	//m_pkServerName->SetFontOutline( true, D3DCOLOR_ARGB(255, 0, 0, 0) );
	

    for ( int i = 0; i < NUM_SERVER_STATUS; ++i )
        m_apkStatus[i]->InitState( true );

    if ( m_pkBalanceMark ) {
		m_pkBalanceMark->InitState( false );
    }

    if( m_pkMatchServerEdge ) { 
        m_pkMatchServerEdge->InitState( false );
    }

    InitState( false, true, this );

#if !defined( NATION_KOREA )
	std::wstring tooltip = g_pkStrLoader->GetString(6071) + L"\n" + g_pkStrLoader->GetString(6072) + L"\n" + g_pkStrLoader->GetString(6073) + L"\n" + g_pkStrLoader->GetString(6074) + L"\n";
	SetToolTip( tooltip );

	RenderToolTip( false );
#endif
}

KGCServerBar::~KGCServerBar(void)
{
}

void KGCServerBar::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
        case KD3DWnd::D3DWE_BUTTON_DOWN:  g_KDSound.Play( "31" ); return;
    }

    GCWND_MSG_MAP( this, KD3DWnd::D3DWE_BUTTON_CLICK, OnSelectServer )
}

void KGCServerBar::OnCreateComplete( void )
{
}

void KGCServerBar::SetServerInfo( KServerInfo* pServerInfo )
{
    m_kServerInfo = *pServerInfo;
    SetServerName( m_kServerInfo.m_strName );


//     if( inet_addr( m_kServerInfo.m_strConnIP.c_str() ) == g_kGlobalValue.ServerInfo.dwServerIP &&
//         m_kServerInfo.m_usPort == g_kGlobalValue.ServerInfo.usServerPort )
//         m_pkServerName->SetFontColor( 0xFFFFFF00 );
//     else
//         m_pkServerName->SetFontColor( 0xFFFFFFFF );

    int iUserNum = m_kServerInfo.m_iUserNum;
    
    if( g_kGlobalValue.m_fFakeCCU != -1 )
        iUserNum = static_cast<int>(static_cast<float>(iUserNum) * g_kGlobalValue.m_fFakeCCU );        

    SetGauge( iUserNum * 100 / m_kServerInfo.m_iMaxUserNum );

#if !defined( NATION_KOREA )
    if( m_kServerInfo.m_strBenefitMsg != L"" )
    {
        SetToolTip(m_kServerInfo.m_strBenefitMsg);
        RenderToolTip(true);
    }
    else
    {
        RenderToolTip(false);
    }
#endif

/*
// 정정당당 서버 천칭 이미지 처리 코드인데 게임 서버에 접속하지 않은 시점에서 서버 타입을 알 수 없어서
// 서버 UID로 구분하는 방법을 썼어요 아트하지 않아서 죄송합니다
// 천칭 이미지 표시 ON/OFF 플래그와 서버 UID 지정 부분을 스크립트로 빼면 좋을 것 같습니다
    if ( m_pkBalanceMark != NULL ) {
#if defined ( NATION_USA )
#if defined( __PATH__ ) // 본섭이면
        if ( m_kServerInfo.m_dwUID == 1 || m_kServerInfo.m_dwUID == 3 ) {
            m_pkBalanceMark->ToggleRender( true );
            m_pkBalanceMark->SetToolTip( g_pkStrLoader->GetString( STR_ID_PVP_BALANCE_TOOLTIP ), -50.0f );
        }
        else {
            m_pkBalanceMark->ToggleRender( false );
        }
#else // 테섭이면
        if ( m_kServerInfo.m_dwUID == 1 || m_kServerInfo.m_dwUID == 8 ) {
            m_pkBalanceMark->ToggleRender( true );
            m_pkBalanceMark->SetToolTip( g_pkStrLoader->GetString( STR_ID_PVP_BALANCE_TOOLTIP ), -50.0f );
        }
        else {
            m_pkBalanceMark->ToggleRender( false );
        }
#endif
#endif
    }
*/

// #if defined(NATION_IDN) //Temp code
//     if( m_kServerInfo.m_strName == L"Tournament" )
//     {
//         SetToolTip(L"Harus Memiliki Olimpiade 2011 Signboard");
//         RenderToolTip(true);
//     }
//    else { 
//        m_pkBalanceMark->ToggleRender( false );
//    }
//#else 
//    if( m_kServerInfo.m_dwUID == 79 ) {
//        m_pkBalanceMark->ToggleRender( true );
//        m_pkBalanceMark->SetToolTip( g_pkStrLoader->GetString( STR_ID_PVP_BALANCE_TOOLTIP ), -50.0f );
//    }
//    else { 
//        m_pkBalanceMark->ToggleRender( false );
//    }
//#endif

//	}
#if defined( USE_MATCH_SERVER_EGDE ) 
    if( m_pkMatchServerEdge ) { 
        bool bRenderMatchServerEdge = false;
#if defined( __PATH__ ) 
        if( m_kServerInfo.m_dwUID == 26 || m_kServerInfo.m_dwUID == 22 || m_kServerInfo.m_dwUID == 38 ) { //9430 
            bRenderMatchServerEdge = true;
        }
#else
        if( m_kServerInfo.m_dwUID == 47 ) { //9430 
            bRenderMatchServerEdge = true;
        }
#endif

        m_pkMatchServerEdge->ToggleRender( bRenderMatchServerEdge );
    }
#else
        if( m_pkMatchServerEdge ) { 
            m_pkMatchServerEdge->ToggleRender( false );
        }
#endif

#if defined( USE_BALANCE_SERVER_MARK ) 
        if( m_pkMatchServerEdge && m_pkBalanceMark ) { 
            if( m_pkMatchServerEdge->IsRenderOn() ) { 
                m_pkBalanceMark->ToggleRender( true );
                m_pkBalanceMark->SetToolTip( g_pkStrLoader->GetString( STR_ID_PVP_BALANCE_TOOLTIP ), -30.0f );
            }
            else { 
                m_pkBalanceMark->ToggleRender( false );
            }
        }
#else 
        if( m_pkBalanceMark ) { 
            m_pkBalanceMark->ToggleRender( false );
        }
#endif
}

void KGCServerBar::SetServerName( std::wstring& strServerName )
{
    size_t idx = strServerName.find( '(' );
    if( idx != std::wstring::npos )
        strServerName = strServerName.substr( 0, idx );

    m_pkServerName->SetText( strServerName );
}

void KGCServerBar::SetGauge( int iPercent )
{
    if( iPercent > 100 )
        iPercent = 100;
    else if( iPercent < MIN_PERCENT ) // 사람이 아무도 없어도 찔끔 보여준다
        iPercent = MIN_PERCENT;

    float fPercent = ( (float)iPercent ) / 100.0f;

    for ( int i = 0; i < NUM_SERVER_STATUS; ++i )
        m_apkStatus[i]->ToggleRender( false );

	m_iServerState = (int)( iPercent / 25 ) ;
    // 0%  ~ 24% : 쾌적
    // 25% ~ 49% : 원활
    // 50% ~ 74% : 보통
    // 75% ~ 99% : 혼잡
    // 100%      : FULL
    m_apkStatus[ m_iServerState ]->ToggleRender( true );
}

void KGCServerBar::OnSelectServer()
{
// #ifdef NICKNAME_CAMPAIGN
//     if( 0 == g_pkUIHelper->GetUILockTime( LOCK_SERVERLIST ) ) {
//         g_pkUIHelper->SetUILockTime( LOCK_SERVERLIST , BUTTON_LOCK_FRAME );
//     }
//     else {
//         return;
//     }
// #endif

    // 체험계정이면 용사의 섬 입장이 불가능 하다.        
    if( m_kServerInfo.m_strName == L"용사의섬" && g_kGlobalValue.m_bExpAccount) //exp_id
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_EXPID_NOT_ENTER_BRAVE_ISLAND ),
                                g_pkStrLoader->GetString( STR_ID_EXPID_IMPOSSIBLE ));
        return;
    }

    // 선택한 서버가 내가 이미 접속해 있는 서버라면..
    if( RemainServer())
    {
		//kom파일 변조한 적이 있었다면 채널로 이동시키 않겠다.
		if( g_kGlobalValue.m_bCheckChangedKOM )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CLIENT_VERSION1 ),
				g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CLIENT_VERSION2 ) );			
		}
		else
        {
            if ( g_MyD3D->m_pStateMachine->GetPrevGameState() == GS_CHAR_SELECT ) 
            {
                g_MyD3D->m_pStateMachine->GoCharSelect();
            }
            else
            {
                g_MyD3D->GoChannel();
            }
        }
        return;
    }

	if( KP2P::GetInstance()->IsConnectedToGameServer() )
	{
		Result_DepotUpdateItemReq = INT_MAX;
		KP2P::GetInstance()->Send_DepotDBUpdateReq();
		g_MyD3D->WaitForServerAck( Result_DepotUpdateItemReq, INT_MAX, 5000, TIME_OUT_VALUE );
	}

    // 서버에 사람이 많아서 들어갈 수가 없다
    if( m_kServerInfo.m_iMaxUserNum <= m_kServerInfo.m_iUserNum && (!g_kGlobalValue.m_kLoginInfo.bNewUser) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_SERVER_FULL ) );
        return;
    }

	g_MyD3D->GetMyShortCutSlot()->Clear();	// 단축슬롯을 클리어


	// 레인보우 변수들 초기화 
	g_kGlobalValue.m_bIsRainbowEvent = false;
	g_kGlobalValue.m_bJackPot = false;
	g_kGlobalValue.m_iRainbowEventID = 0;
	KSingleton<KEnchantStrong>::GetInstance()->SetSecondaryRatio( 1.0f );
	

	g_pkUIScene->m_pkNewNecklaceEnchantDlg->SetSecondaryRatio( 1.0f );		

    bool bResult = ConnectGameServer();

	g_kGlobalValue.RunLogProgram("11");

    if( bResult )
    {
		g_pNateOn->SetNickName();
		g_pNateOn->SetGrade();

        g_kGlobalValue.ServerInfo.dwServerIP = inet_addr( m_kServerInfo.m_strConnIP.c_str() );
        g_kGlobalValue.ServerInfo.usServerPort = m_kServerInfo.m_usPort;

        KSingleton<KncP2P>::GetInstance()->Close();
        if( !Init_TCPIP() )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_CONNECT_FAIL1 ) );
            return;
        }


		if( KP2P::GetInstance()->m_kMsgServer != NULL )
		{
			// 만약 메시지 서버에 접속되어있지 않다면 메시지 서버에 접속시도
			if( KP2P::GetInstance()->m_kMsgServer->IsConnectedToMsgServer() == false )
				KP2P::GetInstance()->m_kMsgServer->ConnectToMsgServer();

			if( KP2P::GetInstance()->m_kMsgServer->IsConnectedToMsgServer() )
				KP2P::GetInstance()->m_kMsgServer->Send_ChangeGameServer();
		}

        {
            //g_MyD3D->GoChannelBeforeInit();

            // 오전 11:32 2008-04-02 by jemitgge - 새로운 서버로 접속할때는 이전 채널 정보 없애버리쟈!!
            SiKGCChannelManager()->SetCurrentChannel( 0xFFFFFFFF );
            SiKGCChannelManager()->InitPrevMatchChannel();

            //g_MyD3D->GoChannel();
			g_MyD3D->m_pStateMachine->GoCharSelect();
		}

        g_kGlobalValue.RunLogProgram("14");
    }
	else
	{
		g_kGlobalValue.ServerInfo.dwServerIP = 0;
		g_kGlobalValue.ServerInfo.usServerPort = 0;
	}


	KP2P::GetInstance()->Send_CostRateForGambleBuyReq();

}

bool KGCServerBar::ConnectGameServer()
{
    KEVENT_VERIFY_ACCOUNT_REQ kReq, tempReq;
    kReq.m_strLogin     = g_kGlobalValue.m_kLoginInfo.strID;
    kReq.m_strPasswd    = g_kGlobalValue.m_kLoginInfo.strPass;
	kReq.m_wstrCCode	= g_kGlobalValue.m_kUserInfo.strNation;
    kReq.m_bMale        = g_kGlobalValue.m_kLoginInfo.bMale;
    kReq.m_iAge         = g_kGlobalValue.m_kUserInfo.iAge;
    kReq.m_iVersion     = SERVER_VERSION;
    kReq.m_iP2PVersion  = CLIENT_P2P_VER;
    kReq.m_dwAuthType   = g_kGlobalValue.m_dwAuthType;
    kReq.m_dwAuthTick   = g_kGlobalValue.m_dwAuthTick;
    kReq.m_bExpAccount  = g_kGlobalValue.m_bExpAccount; //exp_id
	kReq.m_nFunBoxBonus = g_kGlobalValue.m_kLoginInfo.nFunBoxBonus;
    kReq.m_nLinBonus    = g_kGlobalValue.m_kLoginInfo.nLinBonus;
#ifdef NATION_CHINA
	kReq.m_nAccOnlineTime = SiKGCFatigueManager()->GetAccOnlineTime();
	kReq.m_strBillingAccount = SiKGCFatigueManager()->GetBilingAccount( );
#endif

    #if defined( _DEBUG ) || !defined( __PATH__ ) || defined( NETMARBLE_BUY_TEST )
    {
        kReq.m_dwChecksum = 0;
    }
    #else
    {
        kReq.m_dwChecksum = GetFileCheckSum(); 
    }
    #endif

	

    Result_Connect_GameServer = INT_MAX;

    bool bFirstLogin = KP2P::GetInstance()->IsConnectedToGameServer();


    // 게임 서버 접속
    //tempReq = kReq;
#if defined( __PATH__ )
    KP2P::GetInstance()->ConnectToGameServer( m_kServerInfo, kReq, 60000 );
    g_MyD3D->WaitForServerAck( Result_Connect_GameServer, INT_MAX, 60000, TIME_OUT_VALUE );
#else 
    KP2P::GetInstance()->ConnectToGameServer( m_kServerInfo, kReq, 100000 );
    g_MyD3D->WaitForServerAck( Result_Connect_GameServer, INT_MAX, 100000, TIME_OUT_VALUE );
#endif

    // 0 ( 인증 성공 )
    // 1 ( 잘못된 패스워드 )
    // 2 ( 이중 접속 )
    // 3 ( 길드정보 잘못됨 )
    // 4 ( 프로토콜 버전 다름 )
    // 5 ( 불량 유저 )
    // 7 ( login 크기가 0이거나 인증 실패 )
    // 8 ( 신규 유저 추가 실패 )
    // 9 ( esports 예선 신청자가 아님. )
    // 10 ( 실행파일 checksum이 무효함 )
    // 11 ( 알 수 없는 오류. 대만 인증 실패)
    // 12 ( 존재하지 않는 login )
    // 13 ( 인증 tick Count가 다르다. )
    switch( Result_Connect_GameServer )
    {
        case 0: // 접속 성공
		{
			if ( g_kGlobalValue.IsSuccessRecvInventoryPacket() ) {
				g_kGlobalValue.SetInventoryPacketMaxCnt(-1);
			} 
			else {
				Result_InventoryRecvData = INT_MAX;
				g_MyD3D->WaitForServerAck( Result_InventoryRecvData, INT_MAX, 30000, TIME_OUT_VALUE );
				if ( Result_InventoryRecvData != 0 )
                {
					break;
                }
				else {
					g_kGlobalValue.SetInventoryPacketMaxCnt(-1);
				}
			}

            g_kGlobalValue.SendLoadingTime();
            g_kGlobalValue.SendLoginServerConTime();
            g_kGlobalValue.SetCurrentServerUID( m_kServerInfo.m_dwUID );

            // 최초 접속이면 클라이언트 사양 정보 보낸다.
            if ( !bFirstLogin ) 
            {
                KEVENT_STAT_CLIENT_INFO     kStatClientInfo;
                g_MyD3D->GetClientHarwareInfo( kStatClientInfo );
                KP2P::GetInstance()->SendClientHardwareInfo( kStatClientInfo );
            }

            return true;
        }
        case 1: // 존재하지 않는 아이디 또는 패스워드가 틀림
        case 7: // login 크기가 0이거나 인증 실패
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_ID_OR_PW2 ) );
            break;
        }
        case 2: // 이중접속 시도
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DOUBLE_CONNECT1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_DOUBLE_CONNECT2 ) );
            break;
        }
        case 3: // 잘못된 길드
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_GUILD_INFO1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_GUILD_INFO2 ) );
            break;
        }
        case 4: // 패치가 되지 않았음 ( 클라이언트 체크섬이 디비에 등록되어 있지 않다 )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CLIENT_VERSION1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CLIENT_VERSION2 ) );

			if( g_kGlobalValue.m_bCheckChangedKOM && !m_bSendHackCheckPacket )
			{
				KP2P::GetInstance()->Send_ClientHackingUserNot(KEVENT_DUNGEON_HACKING_USER_NOT::MONSTER_TEMPLATE);
				m_bSendHackCheckPacket = true;
			}
            break;
        }
        case 5: // 계정 중지
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ACCOUNT_BLOCK1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_ACCOUNT_BLOCK2 ) );
            break;
        }
        case 9: // 대회서버인데 대회참가자가 아님
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_CONTEST1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_CONTEST2 ) );
            break;
        }
        case 10: // 실행파일 체크섬이 유효하지 않음.
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_FILE1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_INVALID_FILE2 ) );
            break;
        }
        case 11: // Gash 오류
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_GASH ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_NOMATCH_SERVER ) );
            break;
        }
        case 12: // 체험계정이라면 
        case 13: // 12세 미만의 사용자 제한.
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_TIMEOUT1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_SVR_CONNECT_FAIL1 ) );
            break;
        }
		case 14:
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
									g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_CONTEST1),
									g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_CONTEST2 ) );
			break;
		}
        case 15:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ERR_GAME_START_TIME), L"" );
            break;
        }
        case 16:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_GUILD_USER) );
            break;
        }
        case 17:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_FORMAL_GUILD_ENTER_ERROR_MSG ) );
            break;
        }
		case 20:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_APPMSG_SERVER_FULL ) );
            break;
        }
        case 21:
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                g_pkStrLoader->GetString(STR_ID_VERIFY_ACCOUNT_ERROR_23), L"");
            break;
        }
        case 22:
        {
            std::wstring str = g_pkStrLoader->GetString( STR_ID_SHUTDOWN_MSG_3 );
            str += g_pkStrLoader->GetString( STR_ID_SHUTDOWN_MSG_4 );

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );

            break;    
        }
        case 8:
        case TIME_OUT_VALUE:
        default:        // 타임아웃 또는 알 수 없는 에러
        {
            std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", 
                KUserEvent::EVENT_VERIFY_ACCOUNT_ACK, Result_Connect_GameServer );	

            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_TIMEOUT1 ),
                                     g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CONNECT_TIMEOUT2 ) + strTemp );
            break;
        }
    }

    FILE *fo2 = NULL;
    fo2 = fopen("FailedGameServerConnection.txt","a");
    fwprintf(fo2,L"%d",Result_Connect_GameServer);
    fclose(fo2);

    return false;
}

void KGCServerBar::FrameMoveInEnabledState( void )
{
	if ( GetWndMode() == D3DWM_DEFAULT )
	{
		if( IsRenderToolTip() )
			SetWndMode( D3DWM_SELECT );
	}
	
}

bool KGCServerBar::IsServerUID(DWORD dwServerUID)
{
    if(m_kServerInfo.m_dwUID == dwServerUID)
        return true;
    return false;
}


bool KGCServerBar::RemainServer() 
{
    if( g_kGlobalValue.ServerInfo.dwServerIP == inet_addr( m_kServerInfo.m_strConnIP.c_str() ) &&
        g_kGlobalValue.ServerInfo.usServerPort == m_kServerInfo.m_usPort &&
        KP2P::GetInstance()->IsConnectedToGameServer())
        return true;
    return false;
}

void KGCServerBar::OptimizationPos()
{
	float fWidth = static_cast<float>(GetWidth());
	float fHeight = static_cast<float>(GetHeight());

	float fNamePosY = ( fHeight - ( m_pkServerName->GetHeight() + 4 ) ) / 2;
	float fStatusPosY = ( fHeight - ( m_apkStatus[0]->GetHeight() + 4 ) ) / 2;

	float fNamePosX = fWidth/15 - 4 ;

#if defined (NATION_PHILIPPINE)
	float fStatusPosX = fWidth/14 * 10 + 2;
#else
    float fStatusPosX = ( fWidth - ( fWidth * 0.16f ) ) - ( m_apkStatus[0]->GetWidth() / 2 );
#endif

    // +1.f 이유는 UI 디자이너의 요청
	m_pkServerName->SetFixedWindowLocalPos(D3DXVECTOR2(fNamePosX,fNamePosY + 1.f));
	for (int i=0 ; i<NUM_SERVER_STATUS ; i++)
	{
		m_apkStatus[i]->SetFixedWindowLocalPos(D3DXVECTOR2(fStatusPosX,fStatusPosY + 1.f));
	}

#if defined( USE_BALANCE_SERVER_MARK )
    float fMarkPosX = fStatusPosX - ( m_apkStatus[0]->GetWidth() + 4 );
    float fMarkPosY = fStatusPosY - ( m_apkStatus[0]->GetHeight() / 2 ) + 4;

    if( m_pkBalanceMark ) { 
        m_pkBalanceMark->SetFixedWindowLocalPos( D3DXVECTOR2( fMarkPosX, fMarkPosY ) );
    }
#endif

#if defined( USE_MATCH_SERVER_EGDE ) 
    if( m_pkMatchServerEdge ) { 
        m_pkMatchServerEdge->SetFixedWindowLocalPos( D3DXVECTOR2( -7.0f, -3.0f ) );
    }
#endif
}

void KGCServerBar::ReSetInfo()
{
	//구분을 UID 로 해버리자..
	m_kServerInfo.ReSetInfo();

}
