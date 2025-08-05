#include "stdafx.h"
#include "KGCMatchInfoBox.h"
#include "../MyD3D.h"

#include "KGCUIHelper.h"
#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "KGCRoomSceneS6.h"
#include "KGCSingleImage.h"

IMPLEMENT_CLASSNAME( KGCMatchInfoBox );
IMPLEMENT_WND_FACTORY( KGCMatchInfoBox );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchInfoBox, "gc_match_info" );

KGCMatchInfoBox::KGCMatchInfoBox( void )
: m_pkSubModeNoItem(NULL)
, m_pkSubModeItem(NULL)
, m_pkTeamMode(NULL)
, m_pkSurvivalMode(NULL)
, m_pkModeImage(NULL)
, m_pDeathMatchOption(NULL)
, m_pkOptionBtn(NULL)
, m_pkModeInfoBtn(NULL)
, m_pGuildBattleInfo(NULL)
, m_pkSubModeCheck(NULL)
, m_pkTeamSurvCheck(NULL)
{
    m_eUIGameModeCategory   = GC_GMC_MATCH;
    m_nUISubGameMode        = GC_SGM_NO_ITEM;

    LINK_CONTROL( "game_mode_change_box",       m_pkGameModeBox );
    LINK_CONTROL( "submode_no_item",   m_pkSubModeNoItem );
    LINK_CONTROL( "submode_item",    m_pkSubModeItem );
    LINK_CONTROL( "team_mode",   m_pkTeamMode );
    LINK_CONTROL( "survival_mode",    m_pkSurvivalMode );
    LINK_CONTROL( "mode_image", m_pkModeImage );
    LINK_CONTROL( "deathmatch_option", m_pDeathMatchOption );
    LINK_CONTROL( "option_btn", m_pkOptionBtn );
    LINK_CONTROL( "guildbattle_info",m_pGuildBattleInfo);
    LINK_CONTROL( "mode_info_btn", m_pkModeInfoBtn );
    LINK_CONTROL( "submode_check", m_pkSubModeCheck );
    LINK_CONTROL( "team_surv_check", m_pkTeamSurvCheck );

    for( int i = 0 ; i < (int)m_pGuildInfo.size() ; ++i )
    {
        m_pGuildInfo[i] = NULL;
    }

    CreateModeCategoryContainer();
}

KGCMatchInfoBox::~KGCMatchInfoBox( void )
{
}

void KGCMatchInfoBox::OnCreate( void )
{
    m_pkGameModeBox->SetSelfInputCheck( false );
    m_pkGameModeBox->AddActionListener( this );
    m_pkSubModeNoItem->InitState( true, true, this );
    m_pkSubModeItem->InitState( true, true, this );
    m_pkTeamMode->InitState( true, true, this );
    m_pkSurvivalMode->InitState( true, true, this );
    m_pkModeImage->InitState( true, false, this );
    m_pDeathMatchOption->InitState(false, true, this);
    m_pkOptionBtn->InitState(true, true, this);
    m_pGuildBattleInfo->InitState(false, true, this);
    m_pkModeInfoBtn->InitState(true, true, this);
    m_pkSubModeCheck->InitState(true);
    m_pkTeamSurvCheck->InitState(true);

    float fStartPosX = m_pGuildBattleInfo->GetFixedWindowLocalPos().x;
    float fStartPosY = m_pGuildBattleInfo->GetFixedWindowLocalPos().y;

    //길드 정보
    std::vector<KD3DWnd*> vecSlotList 
        = g_pkUIMgr->CreateTemplateUIListByFindName( this, "ui_room_sceneS6.stg", "", 
                                                    "room_scene_s6\\match_panel\\room_match_info\\guildbattle_info", true, GUILD_BATTLE_INFO_PAIR );

    if (!vecSlotList.empty())
    {
        std::vector<KD3DWnd*>::iterator iterSlot = vecSlotList.begin();
        DWORD dwWidth = (*iterSlot)->GetWidth();
        for ( int i = 0; iterSlot != vecSlotList.end(); ++iterSlot, ++i )
        {
            m_pGuildInfo[i] = static_cast<KGCGuildBattleInfo*>(*iterSlot);
            m_pGuildInfo[i]->InitState( false, true, this );

            D3DXVECTOR2 vPos;
            vPos.x = fStartPosX+ static_cast<float>( (i)*(dwWidth+GUILD_INFO_GAP) );
            vPos.y = fStartPosY;
            m_pGuildInfo[i]->SetFixedWindowLocalPos(vPos);
        }
    }
}

void KGCMatchInfoBox::OnInitialize( void )
{
    UpdateGameData();
}

void KGCMatchInfoBox::ActionPerformed( const KActionEvent& event )
{
    bool bSendRoomInfo = false;

    if( g_pkUIHelper->GetUILockTime( LOCK_ROOM ) == 0 )
    {
        if ( g_kGlobalValue.m_kUserInfo.bHost ||	// 방장만 모드 바꾸기가 가능하다.
            g_MyD3D->IsLocalPlayerObserver()) //운영자도 가능하다.
        {
#if !defined( NATION_USA )	&& !defined( NATION_EU )  && !defined( NATION_KOREA )// 미국은 몰래 관찰만 하겠다. 한국도..
            // 방장인 내가 아닌 옵저버가 있다면 방장 기능을 상실한다.
            if(GC_GMC_DUNGEON != SiKGCRoomManager()->GetGameModeCategory())
            {
                if( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() && !g_MyD3D->IsLocalPlayerObserver() )
                    return;
            }
#endif
            if ( event.m_pWnd == m_pkGameModeBox )
            {
                switch ( event.m_dwCode )
                {
                case KGCChangeBox::LEFT_BUTTON:
                    {
                        ChangeGameModeCategory( false );
                        break;
                    }
                case KGCChangeBox::RIGHT_BUTTON:
                    {
                        ChangeGameModeCategory( true );
                        break;
                    }
                }
            }

            if ( event.m_pWnd == m_pkOptionBtn )
            {
                if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
                     SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
                {               
                    switch ( event.m_dwCode )
                    {
                    case KGCChangeBox::D3DWE_BUTTON_CLICK:
                        {
                            OnClickOptionBtn();
                            break;
                        }
                    }

                }
            }

            //아이템 노템 선택
            if(  ( event.m_pWnd == m_pkSubModeNoItem || event.m_pWnd == m_pkSubModeItem ) && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DOTA )
            {
                switch ( event.m_dwCode )
                {
                case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); break;
                case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play( "31" ); break;

                case KD3DWnd::D3DWE_BUTTON_CLICK:
                    {
                        if( event.m_pWnd == m_pkSubModeNoItem )
                        {
                            SiKGCRoomManager()->SetSubGameMode( GC_SGM_NO_ITEM );

                            ChangeUIGameMode();
                            SendRoomInfo();
                            g_KDSound.Play( "31" );
                        }

                        else if( event.m_pWnd == m_pkSubModeItem && !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
                        {
                            SiKGCRoomManager()->SetSubGameMode( GC_SGM_NORMAL );

                            ChangeUIGameMode();
                            SendRoomInfo();
                            g_KDSound.Play( "31" );
                        }
                    }
                    break;
                }
            }

            //팀 개인 선택
            if( event.m_pWnd == m_pkTeamMode || event.m_pWnd == m_pkSurvivalMode )
            {
                switch ( event.m_dwCode )
                {
                case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); break;
                case KD3DWnd::D3DWE_BUTTON_DOWN: g_KDSound.Play( "31" ); break;

                case KD3DWnd::D3DWE_BUTTON_CLICK:
                    {
                        if( event.m_pWnd == m_pkTeamMode )
                        {
                            //팀모드로 바뀌면 모든 슬롯을 연다.
                            std::vector<std::pair<char,bool> > vecChangeSlot;
                            for ( char c = 0 ; c < MAX_PLAYER_NUM; c++ )
                            {
                                if ( !g_MyD3D->MyPlayer[c]->m_kUserInfo.bLive )
                                {
                                    vecChangeSlot.push_back( std::make_pair( c, true ) );
                                }
                            }
                            bool bModeChanged = false;

                            if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH )
                            {                            
                                if( SiKGCRoomManager()->GetGameMode() != GC_GM_TEAM_MODE )
                                {
                                    SiKGCRoomManager()->SetGameMode( GC_GM_TEAM_MODE );
                                    bModeChanged = true;
                                }

                            }
                            else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH )
                            {   
                                if( SiKGCRoomManager()->GetGameMode() != GC_GM_DEATH_TEAM )
                                {
                                    SiKGCRoomManager()->SetGameMode( GC_GM_DEATH_TEAM );
                                    bModeChanged = true;
                                }
                            }
                            else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
                            {   
                                if( SiKGCRoomManager()->GetGameMode() != GC_GM_FATAL_DEATH_TEAM )
                                {
                                    SiKGCRoomManager()->SetGameMode( GC_GM_FATAL_DEATH_TEAM );
                                    bModeChanged = true;
                                }
                            }

                            if( bModeChanged ) {
                                KP2P::GetInstance()->Send_ChangeRoomInfoReq( vecChangeSlot );
                            }

                            //bSendRoomInfo = true;
                            //break;                        
                        }

                        else if( event.m_pWnd == m_pkSurvivalMode && !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
                        {
                            if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH )
                            {                            
                                SiKGCRoomManager()->SetGameMode( GC_GM_SURVIVAL_MODE );
                            }
                            else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH )
                            {                            
                                SiKGCRoomManager()->SetGameMode( GC_GM_DEATH_SURVIVAL );
                            }
                            else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
                            {                            
                                SiKGCRoomManager()->SetGameMode( GC_GM_FATAL_DEATH_SURVIVAL );
                            }
                            //bSendRoomInfo = true;
                            //break;
                        }
                        // UI 갱신
                        bSendRoomInfo = true;
                        break;
                    }
                }
            }


            if ( bSendRoomInfo )
            {
                SendRoomInfo();
                UpdateGameData();
                g_pkUIScene->RoomButtonSetStartDelay( 219 );
            }

        }
        if ( event.m_pWnd == m_pkModeInfoBtn && g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) == true ) {
            switch ( event.m_dwCode )
            {
            case KGCChangeBox::D3DWE_BUTTON_CLICK:
                {
                    OnClickModeInfoBtn();
                    break;
                }
            }
        }

    }
}

void KGCMatchInfoBox::UpdateGameData()
{
    // UI에 표시되는 게임 모드와 다르게 갱신되었다면 바꾼다.
    if ( SiKGCRoomManager()->GetGameModeCategory() != m_eUIGameModeCategory ||
        SiKGCRoomManager()->GetSubGameMode() != m_nUISubGameMode )
    {
        ChangeUIGameMode();
    }

    if( SiKGCChannelManager()->IsCreateLimitServer() )    //밸런스채널에 옵저버 상태면 모드박스 활성화
#if defined (PVP_SERVER_TEST)
        EnableModeBox( g_kGlobalValue.m_kUserInfo.bHost );
#else
        EnableModeBox( g_MyD3D->IsLocalPlayerObserver() );
#endif
    else if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) )
    {
        EnableModeBox( false);
    }
    else 
    {
        EnableModeBox( g_kGlobalValue.m_kUserInfo.bHost );
    }

    m_pkSubModeNoItem->InitState( true );
    m_pkSubModeItem->InitState( true );
    m_pkSurvivalMode->InitState( true );
    m_pkTeamMode->InitState( true );
    m_pkOptionBtn->ToggleRender(false);

    EGCGameMode eGameMode = SiKGCRoomManager()->GetGameMode();

    bool bTeamMode = false;
    bool bSurvivalMode = false;
    if ( eGameMode == GC_GM_TEAM_MODE || eGameMode == GC_GM_DEATH_TEAM || eGameMode == GC_GM_FATAL_DEATH_TEAM )
        bTeamMode = true;

    if ( eGameMode == GC_GM_SURVIVAL_MODE || eGameMode == GC_GM_DEATH_SURVIVAL || eGameMode == GC_GM_FATAL_DEATH_SURVIVAL )
        bSurvivalMode = true;

    m_pkTeamMode->Activate( bTeamMode );
    m_pkSurvivalMode->Activate( bSurvivalMode );
    m_pkSubModeNoItem->Activate( m_nUISubGameMode == GC_SGM_NO_ITEM );
    m_pkSubModeItem->Activate( m_nUISubGameMode == GC_SGM_NORMAL);
    ModeCheck();

        // 방장이면 버튼을 누를 수 있게..
    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
    //#if !defined( NATION_USA ) && !defined( NATION_EU ) && !defined( NATION_KOREA )
    //    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) )
    //    {
    //        if( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() && !g_MyD3D->IsLocalPlayerObserver() )
    //            break;
    //    }
    //#endif
        
        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH || 
             SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
        {
            m_pDeathMatchOption->InitData();
            m_pkOptionBtn->ToggleRender(true);
        }
        else {
            m_pDeathMatchOption->ToggleRender( false );
        }

        m_pkSurvivalMode->InitState( true, true, this );
        m_pkTeamMode->InitState( true, true, this );
        m_pkSubModeNoItem->InitState( true, true, this );
        m_pkSubModeItem->InitState( true, true, this );
    }
    else
    {
        m_pkOptionBtn->ToggleRender(false);
        m_pDeathMatchOption->ToggleRender( false );
        
        m_pGuildInfo[0]->ToggleRender(false);
        m_pGuildInfo[1]->ToggleRender(false);
    }

    if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {
        SetGuildBattleInfo();
        m_pkModeInfoBtn->ToggleRender( true );
    }
    else {
        m_pkModeInfoBtn->ToggleRender(false);
    }
}

void KGCMatchInfoBox::FrameMoveInEnabledState( void )
{

}

void KGCMatchInfoBox::ChangeGameModeCategory( bool bRightMove )
{
    //g_MyD3D->ChangeGameModeCategory( bRightMove );
    if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {     
        SiKGCRoomManager()->ChangeGameModeCategory( bRightMove );

        ChangeUIGameMode();
        g_KDSound.Play( "31" );
        SendRoomInfo();
    }    
}

void KGCMatchInfoBox::ChangeSubGameMode( bool bRightMove )
{
    if( !g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {     
        SiKGCRoomManager()->ChangeSubGameMode( bRightMove );

        ChangeUIGameMode();
        g_KDSound.Play( "31" );
        SendRoomInfo();
    }
}

void KGCMatchInfoBox::ChangeUIGameMode( void )
{
    bool bChangetoTagmatch = ( m_eUIGameModeCategory != SiKGCRoomManager()->GetGameModeCategory() ) &&
        ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH );
    bool bChangetoAngelsEgg = ( m_eUIGameModeCategory != SiKGCRoomManager()->GetGameModeCategory() ) &&
        ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG );
    bool bChangetoCaptain = ( m_eUIGameModeCategory != SiKGCRoomManager()->GetGameModeCategory() ) &&
        ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN );
    bool bChangetoDota = ( m_eUIGameModeCategory != SiKGCRoomManager()->GetGameModeCategory() ) &&
        ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA );

    m_eUIGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();
    m_nUISubGameMode = SiKGCRoomManager()->GetSubGameMode();

    m_pkGameModeBox->SetText( g_pkStrLoader->GetString( m_mapGameModeCategoryNameID[m_eUIGameModeCategory].first) );
    UpdateModeImage(m_eUIGameModeCategory);

    if( true == bChangetoAngelsEgg )
    {
        g_MyD3D->AngelsEggHelpMessage();
        // 2007/3/29. iridology. 내가 호스트인 경우 강제로 레디를 풀도록 요청
        if( true == g_kGlobalValue.m_kUserInfo.bHost )
            KP2P::GetInstance()->Send_ForceChangeRoomUserInfo();
    }

    if( true == bChangetoCaptain )
    {
        g_MyD3D->CaptainHelpMessage();
        // 2007/3/29. iridology. 내가 호스트인 경우 강제로 레디를 풀도록 요청
        if( true == g_kGlobalValue.m_kUserInfo.bHost )
            KP2P::GetInstance()->Send_ForceChangeRoomUserInfo();
    }

    if( true == bChangetoDota )
    {
        g_pkChatManager->AddChatMsg( 
            g_pkStrLoader->GetString( STR_ID_DOTA_MODE_1 ) 
            , KGCChatManager::CHAT_TYPE_ADMIN
            );
        g_pkChatManager->AddChatMsg( 
            g_pkStrLoader->GetString( STR_ID_DOTA_MODE_2 ) 
            , KGCChatManager::CHAT_TYPE_ADMIN
            );
        g_pkChatManager->AddChatMsg( 
            g_pkStrLoader->GetString( STR_ID_DOTA_MODE_3 ) 
            , KGCChatManager::CHAT_TYPE_ADMIN
            );
    }
}

void KGCMatchInfoBox::SendRoomInfo()
{
    if( !( SiKGCRoomManager()->GetPrevRoomInfo() == SiKGCRoomManager()->GetRoomInfo() ) )
    {
        std::vector<std::pair<char,bool> > vecChangeSlot;
        for ( int i = 0 ; i < MAX_PLAYER_NUM; i++ )
        {
            if ( !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            {
                vecChangeSlot.push_back( std::make_pair( i, true ) );
            }
        }

        KP2P::GetInstance()->Send_ChangeRoomInfoReq( &SiKGCRoomManager()->GetRoomInfo() );
        KP2P::GetInstance()->Send_ChangeRoomInfoReq( vecChangeSlot );
    }
}

void KGCMatchInfoBox::EnableModeBox( bool bEnable )
{
    m_pkGameModeBox->EnableChangeBox( bEnable );
    m_pkSubModeNoItem->Lock( !bEnable );
    m_pkSubModeItem->Lock( !bEnable );
    ChangeUIGameMode();
}

void KGCMatchInfoBox::OnClickOptionBtn()
{
    m_pDeathMatchOption->ToggleRender( !m_pDeathMatchOption->IsRenderOn() );
    if ( m_pDeathMatchOption->IsRenderOn() )
        m_pDeathMatchOption->InitData();
}

void KGCMatchInfoBox::OnClickModeInfoBtn()
{
    m_pGuildInfo[0]->ToggleRender( !m_pGuildInfo[0]->IsRenderOn());
    m_pGuildInfo[1]->ToggleRender( !m_pGuildInfo[1]->IsRenderOn());
}

void KGCMatchInfoBox::CreateModeCategoryContainer()
{
    m_mapGameModeCategoryNameID[GC_GMC_MATCH]                = std::pair<int, int> ( STR_ID_GMC_MATCH, 0 );
    m_mapGameModeCategoryNameID[GC_GMC_DEATHMATCH]           = std::pair<int, int> (STR_ID_GMC_DEATHMATCH ,1);
    m_mapGameModeCategoryNameID[GC_GMC_GUILD_BATTLE]         = std::pair<int, int> ( STR_ID_GMC_GUILD_BATTLE, 2);
    m_mapGameModeCategoryNameID[GC_GMC_CAPTAIN]              = std::pair<int, int> ( STR_ID_GMC_CAPTAIN , 3);
    m_mapGameModeCategoryNameID[GC_GMC_DOTA]                 = std::pair<int, int> (STR_ID_GMC_DOTA ,4 );
    m_mapGameModeCategoryNameID[GC_GMC_DUNGEON]              = std::pair<int, int> (STR_ID_GMC_QUEST , 5 );
    m_mapGameModeCategoryNameID[GC_GMC_INDIGO]               = std::pair<int, int> (STR_ID_GMC_INDIGO ,6 );
    m_mapGameModeCategoryNameID[GC_GMC_TAG_MATCH]            = std::pair<int, int> (STR_ID_GMC_TAG_MATCH, 7 );
    m_mapGameModeCategoryNameID[GC_GMC_MONSTER_CRUSADER]     = std::pair<int, int> (STR_ID_GMC_MONSTER_CRUSADERS , 8);
    m_mapGameModeCategoryNameID[GC_GMC_MONSTER_HUNT]         = std::pair<int, int> (STR_ID_GMC_MONSTER_HUNT , 9);
    //m_mapGameModeCategoryNameID[GC_GMC_ANGELS_EGG]         = std::pair<int, int> (STR_ID_GMC_ANGELS_EGG , 10);
    m_mapGameModeCategoryNameID[GC_GMC_ANGELS_EGG]           = std::pair<int, int> (STR_ID_GMC_HALLOWEEN_PUMPKIN , 10);
    m_mapGameModeCategoryNameID[GC_GMC_FATAL_DEATHMATCH]     = std::pair<int, int> (STR_ID_GMC_FATAL_DEATHMATCH ,11);
}

void KGCMatchInfoBox::UpdateModeImage( EGCGameModeCategory eGameMode )
{
    //텍스쳐 이미지 사이즈 설정
    D3DXVECTOR2 vTexSize,vSecondTexStartAxis;
    vTexSize = D3DXVECTOR2( 253.0f, 124.0f );
    vSecondTexStartAxis = D3DXVECTOR2( 258.0f, 128.0f );

    int iFileNum = 0;
    int iIndex = 0;
    bool bOK = GetTextureFileNum( eGameMode , iFileNum , iIndex);

    if ( !bOK )
        return;

    char cBuf[256];
    memset(cBuf,'\0',256);
    sprintf(cBuf, "room_pvp_image_%d.dds", iFileNum );

    std::string strTemp = cBuf;
    m_pkModeImage->SetInfomation( vTexSize, vSecondTexStartAxis, strTemp , iIndex , 4 );
    m_pkModeImage->ToggleRender( true );
}

bool KGCMatchInfoBox::GetTextureFileNum(IN EGCGameModeCategory eGameMode, OUT int& iFileNum, OUT int& iIndex)
{
    std::map<EGCGameModeCategory, std::pair<int, int> >::iterator mit = m_mapGameModeCategoryNameID.find(eGameMode);

    if (mit == m_mapGameModeCategoryNameID.end() )
        return false;

    int iNum = m_mapGameModeCategoryNameID[eGameMode].second;

    if ( iNum <= 0 ) {
        iFileNum = 0;
    }
    else {
        iFileNum = iNum / 8;
        iIndex = iNum % 8;
    }

    return true;
}

void KGCMatchInfoBox::SetGuildBattleInfo()
{
    std::wstring wstrGuildMarkName1 = SiKGCRoomManager()->GetRoomInfo().m_pairGuildMarkName.first;
    std::wstring wstrGuildName1 = SiKGCRoomManager()->GetRoomInfo().m_pairGuildName.first;
    DWORD dwBP1 = SiKGCRoomManager()->GetRoomInfo().m_pairBattlePoint.first;

    std::wstring wstrGuildMarkName2 = SiKGCRoomManager()->GetRoomInfo().m_pairGuildMarkName.second;
    std::wstring wstrGuildName2 = SiKGCRoomManager()->GetRoomInfo().m_pairGuildName.second;
    DWORD dwBP2 = SiKGCRoomManager()->GetRoomInfo().m_pairBattlePoint.second;

    //왼쪽 길드 정보
    m_pGuildInfo[0]->SetInformation(wstrGuildMarkName1,wstrGuildName1, dwBP1);
    //오른쪽 길드 정보
    m_pGuildInfo[1]->SetInformation(wstrGuildMarkName2,wstrGuildName2, dwBP2);
}


void KGCMatchInfoBox::ModeCheck()
{
    if ( m_pkTeamMode->IsActive() ) {
        DrawCheckImg(m_pkTeamMode, SUBMODE_CHECK);
    }
    else if ( m_pkSurvivalMode->IsActive() ) {
        DrawCheckImg(m_pkSurvivalMode , SUBMODE_CHECK);
    }
    else {
        m_pkSubModeCheck->ToggleRender(false);
    }

    if ( m_pkSubModeNoItem->IsActive() ) {
        DrawCheckImg(m_pkSubModeNoItem, TEAM_OR_SURVIVAL_CHECK);
    }
    else if ( m_pkSubModeItem->IsActive() ) {
        DrawCheckImg(m_pkSubModeItem, TEAM_OR_SURVIVAL_CHECK);
    }
    else  {
        m_pkTeamSurvCheck->ToggleRender(false);
    }
}

void KGCMatchInfoBox::DrawCheckImg( KD3DWnd *pkBaseWnd , ECHECKWNDTYPE eCheckType ) 
{
    float xpos = pkBaseWnd->GetFixedWindowLocalPos().x + 2.f;

    if ( eCheckType == SUBMODE_CHECK) {
        xpos += static_cast<float>(pkBaseWnd->GetWidth()) / 2.f - static_cast<float>(m_pkSubModeCheck->GetWidth()) / 2.f;
        m_pkSubModeCheck->SetWindowPosXDirect(xpos);
        m_pkSubModeCheck->ToggleRender(true);
    }
    
    if ( eCheckType == TEAM_OR_SURVIVAL_CHECK)
    {
        xpos += static_cast<float>(pkBaseWnd->GetWidth()) / 2.f - static_cast<float>(m_pkTeamSurvCheck->GetWidth()) / 2.f;
        m_pkTeamSurvCheck->SetWindowPosXDirect(xpos);
        m_pkTeamSurvCheck->ToggleRender(true);
    }
}