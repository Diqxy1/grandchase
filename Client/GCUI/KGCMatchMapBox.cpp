#include "stdafx.h"
//
#include "KGCMatchMapBox.h"
#include "GCChangeBox.h"
//
#include "KGCRoomManager.h"
#include "MyD3D.h"
#include "KGCUIHelper.h"

//
#include "GCStateGame.h"
#include "KGCDeathMatch.h"
#include "GCStateMachine.h"
#include "gcui/KGCSingleImage.h"

IMPLEMENT_CLASSNAME( KGCMatchMapBox );
IMPLEMENT_WND_FACTORY( KGCMatchMapBox );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchMapBox, "gc_matchmap_select" );

KGCMatchMapBox::KGCMatchMapBox( void )
: m_pkGoalStatic(NULL)
, m_pkTimeStatic(NULL)
, m_pkStatBalanceStatic(NULL)
, m_pkMapSelectBox(NULL)
, m_pkRandomMapStage(NULL)
, m_pkMapImage(NULL)
{
    //LINK_CONTROL( "team_right_btn",  m_pkCanabanBtn );
    LINK_CONTROL( "goal_static" , m_pkGoalStatic );    
    LINK_CONTROL( "time_static" , m_pkTimeStatic);
    LINK_CONTROL( "stat_bal_static" , m_pkStatBalanceStatic);
    LINK_CONTROL( "map_select_box",  m_pkMapSelectBox );
    LINK_CONTROL( "map_stage_random", m_pkRandomMapStage );
    LINK_CONTROL( "map_image", m_pkMapImage );

    CreateMapStringContainer();
}

KGCMatchMapBox::~KGCMatchMapBox( void )
{
    // empty
}

void KGCMatchMapBox::OnCreate( void )
{
    m_pkMapSelectBox->SetSelfInputCheck( false );
    m_pkMapSelectBox->AddActionListener( this );
    m_pkMapImage->InitState( false, true, this );

    m_pkGoalStatic->SetAlign( DT_LEFT );
    m_pkTimeStatic->SetAlign( DT_LEFT );
    m_pkStatBalanceStatic->SetAlign( DT_LEFT );
    m_pkGoalStatic->SetFontOutline( true );
    m_pkTimeStatic->SetFontOutline( true );
    m_pkStatBalanceStatic->SetFontOutline( true );
}

void KGCMatchMapBox::OnInitialize( void )
{
    UpdateGameData();
}

void KGCMatchMapBox::ActionPerformed( const KActionEvent& event )
{
    if( g_kGlobalValue.m_kUserInfo.bHost ||
        g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN ) // 방장인 경우만 조작 가능
    {
#if !defined( NATION_USA ) && !defined( NATION_EU ) && !defined( NATION_KOREA )
        if( g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_BALANCE) )
        {
            // 방장인 내가 아닌 옵저버가 있다면 방장 기능을 상실한다.
            if( SiKGCRoomManager()->GetRoomInfo().IsExistObserver() && !g_MyD3D->IsLocalPlayerObserver() )
                return;
        }
#endif
        bool bSendRoomInfo = false;
        if( event.m_pWnd == m_pkMapSelectBox ) // 맵 바꾸기
        {
            if( g_pkUIHelper->GetUILockTime( LOCK_ROOM ) == 0 )
            {
                switch ( event.m_dwCode )
                {
                case KGCChangeBox::RIGHT_BUTTON: // 오른쪽 버튼을 클릭했을 때
                    {
                        ChangeMap( true );
                        bSendRoomInfo = true;
                        g_KDSound.Play( "31" );
                        break;
                    }
                case KGCChangeBox::LEFT_BUTTON: // 왼쪽 버튼을 클릭했을 때
                    {
                        ChangeMap( false );
                        bSendRoomInfo = true;
                        g_KDSound.Play( "31" );
                        break;
                    }
                }
                g_pkUIHelper->SetUILockTime( LOCK_ROOM , BUTTON_LOCK_FRAME );
            }
        }

        if ( bSendRoomInfo )
        {
            SendRoomInfo();
            UpdateGameData();
            g_pkUIScene->RoomButtonSetStartDelay( 219 );
        }

    }
}

void KGCMatchMapBox::UpdateGameData()
{
    m_pkGoalStatic->ToggleRender( false );
    m_pkTimeStatic->ToggleRender( false );
    m_pkStatBalanceStatic->ToggleRender( false );


    // 방장이라면 맵을 바꿀 수 있도록 한다
    if( g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_BALANCE) ){
        // 대회서버라면
        if( g_kGlobalValue.ServerInfo.CheckServerType(ST_ENTER_LIMIT) || g_kGlobalValue.ServerInfo.CheckServerType(ST_CREATE_LIMIT) )
            m_pkMapSelectBox->EnableChangeBox( g_MyD3D->IsLocalPlayerObserver() );
        else
            m_pkMapSelectBox->EnableChangeBox( g_kGlobalValue.m_kUserInfo.bHost );
    }
    else
        m_pkMapSelectBox->EnableChangeBox( g_kGlobalValue.m_kUserInfo.bHost );

    switch( SiKGCRoomManager()->GetGameModeCategory() )
    {
    case GC_GMC_DEATHMATCH: // 대전
    case GC_GMC_FATAL_DEATHMATCH:
        {
            m_pkGoalStatic->ToggleRender( true );
            m_pkTimeStatic->ToggleRender( true );

            bool bBalance = ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH );
            m_pkStatBalanceStatic->ToggleRender( bBalance );

            wstringstream stmTime;
            wstringstream stmGoal;
            wstringstream stmJoin;
            wstringstream stmStatBal;
            KGCStateGame* pState;
            pState = (KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME );
            KGCDeathMatch* pGame = (KGCDeathMatch*)pState->GetCurGameModePtr();

            stmGoal << g_pkStrLoader->GetString( STR_ID_DEATHMATCH_MAX_KILL ).c_str() << pGame->m_iGoal;
            stmTime << g_pkStrLoader->GetString( STR_ID_DEATHMATCH_TIME_LIMIT ).c_str() << (int)pGame->m_fTime;
            stmStatBal << g_pkStrLoader->GetString( STR_ID_DEATHMATCH_STAT_BALANCE_SHOW ).c_str();

            if ( SiKGCRoomManager()->GetRoomInfo().bDeathMatchBalancing )
            {
                stmStatBal << g_pkStrLoader->GetString( STR_ID_DEATHMATCH_AGREE ).c_str();
            }
            else
            {
                stmStatBal << g_pkStrLoader->GetString( STR_ID_DEATHMATCH_DISAGREE ).c_str();
            }

            m_pkTimeStatic->SetText( stmTime.str() );
            m_pkGoalStatic->SetText( stmGoal.str() );
            m_pkStatBalanceStatic->SetText( stmStatBal.str() );

        }
    }

    m_pkRandomMapStage->ToggleRender( false );

    // 대전이랑 태그매치만 맵이 보인다.. 길드전도 보여주면 안돼??
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH || 
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_GUILD_BATTLE ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN )
    {
        if( SiKGCRoomManager()->GetRoomInfo().bRandomMapMode )
        {
            m_pkRandomMapStage->ToggleRender( true );
            m_pkMapImage->ToggleRender( false );
            m_pkMapSelectBox->SetText( g_pkStrLoader->GetString( STR_ID_MAP_BOX_TEXT0 ) );
        }
        else
        {
            EGCGameStage eGameStage = SiKGCRoomManager()->GetGameModeStage();
            m_pkMapSelectBox->SetText( g_pkStrLoader->GetString( m_mapMapNameID[ eGameStage ].first) );
            UpdateMapImage( eGameStage );
        }
    }
    else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
    {
        if( g_kGlobalValue.IsDotaMapID( g_kGlobalValue.m_iCurDotaMapID ) )
        {
            m_pkMapSelectBox->SetText( g_pkStrLoader->GetString( m_mapMapNameID[ EGCGameStage(NUM_TOTAL_STAGE + g_kGlobalValue.m_iCurDotaMapID) ].first ) );
            UpdateMapImage( EGCGameStage(NUM_TOTAL_STAGE + g_kGlobalValue.m_iCurDotaMapID) );
        }
    }

    
}

void KGCMatchMapBox::FrameMoveInEnabledState( void )
{

}

void KGCMatchMapBox::ChangeMap( bool bRightMove )
{
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH || 
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH || 
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_GUILD_BATTLE ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_ANGELS_EGG ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_CAPTAIN )
        SiKGCRoomManager()->ChangeStage( bRightMove );

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
        SiKGCRoomManager()->ChangeDotaStage( bRightMove );
}

void KGCMatchMapBox::SendRoomInfo()
{
    if ( !( SiKGCRoomManager()->GetPrevRoomInfo() == SiKGCRoomManager()->GetRoomInfo() ) )
        KP2P::GetInstance()->Send_ChangeRoomInfoReq( &SiKGCRoomManager()->GetRoomInfo() );
}

void KGCMatchMapBox::CreateMapStringContainer()
{
    m_mapMapNameID[GC_GS_FOREST_OF_ELF_REVERS]  = std::pair<int, int> ( STR_ID_GS_FOREST_OF_ELF_REVERS ,1);
    m_mapMapNameID[GC_GS_VALLEY_OF_OATH_REVERS] = std::pair<int, int> (STR_ID_GS_VALLEY_OF_OATH_REVERS, 2);
    m_mapMapNameID[GC_GS_FOREST_OF_ELF]         = std::pair<int, int> (STR_ID_GS_FOREST_OF_ELF, 3);
    m_mapMapNameID[GC_GS_SWAMP_OF_OBLIVION]     = std::pair<int, int> (STR_ID_GS_SWAMP_OF_OBLIVION, 4);
    m_mapMapNameID[GC_GS_FLYING_SHIP]           = std::pair<int, int> (STR_ID_GS_FLYING_SHIP, 5);
    m_mapMapNameID[GC_GS_VALLEY_OF_OATH]        = std::pair<int, int> (STR_ID_GS_VALLEY_OF_OATH, 6);
    m_mapMapNameID[GC_GS_FOGOTTEN_CITY]         = std::pair<int, int> (STR_ID_GS_FOGOTTEN_CITY, 7);
    m_mapMapNameID[GC_GS_TEMPLE_OF_FIRE]        = std::pair<int, int> (STR_ID_GS_TEMPLE_OF_FIRE, 8);
    m_mapMapNameID[GC_GS_SHOOTING_FIELD]        = std::pair<int, int> (STR_ID_GS_SHOOTING_FIELD, 9);
    m_mapMapNameID[GC_GS_BABEL_OF_X_MAS]        = std::pair<int, int> (STR_ID_GS_BABEL_OF_X_MAS, 10);
    m_mapMapNameID[GC_GS_HELL_BRIDGE]           = std::pair<int, int> (STR_ID_GS_HELL_BRIDGE, 11);
    m_mapMapNameID[GC_GS_TEMPLE_OF_ORC]         = std::pair<int, int> (STR_ID_GS_TEMPLE_OF_ORC, 12);
    m_mapMapNameID[GC_GS_GUILD]                 = std::pair<int, int> (STR_ID_GS_GUILD, 13);
    m_mapMapNameID[GC_GS_CARRIBEACH]            = std::pair<int, int> (STR_ID_GS_CARRIBEACH, 14);
    m_mapMapNameID[GC_GS_FOREST_OF_TRIAL]       = std::pair<int, int> (STR_ID_GS_TRIAL, 15);
    m_mapMapNameID[GC_GS_GRAVEYARD_OF_DECEASED] = std::pair<int, int> (STR_ID_GS_GRAVEYARD_OF_DECEASED, 16);
    m_mapMapNameID[GC_GS_BAHIA]                 = std::pair<int, int> (STR_ID_GS_BAHIA, 17);
    m_mapMapNameID[GC_GS_AMAZON_FOREST]         = std::pair<int, int> (STR_ID_GS_AMAZON_FOREST, 18);
    m_mapMapNameID[GC_GS_MARKET_WATER]          = std::pair<int, int> (STR_ID_GS_MARKET_WATER, 19);
    m_mapMapNameID[GC_GS_RIZAL_PARK]            = std::pair<int, int> (STR_ID_GS_RIZAL_PARK, 20);
    m_mapMapNameID[GC_GS_CASTLE_OF_RULE]        = std::pair<int, int> (STR_ID_GC_GS_CASTLE_OF_RULE, 21);
    //점령전
    m_mapMapNameID[EGCGameStage(NUM_TOTAL_STAGE+1)]= std::pair<int, int> (STR_ID_GS_DOTA_PARK, 13);
}

void KGCMatchMapBox::UpdateMapImage( EGCGameStage eGameStage )
{
    //텍스쳐 이미지 사이즈 설정
    D3DXVECTOR2 vTexSize,vSecondTexStartAxis;
    vTexSize = D3DXVECTOR2( 254.0f, 103.0f );
    vSecondTexStartAxis = D3DXVECTOR2( 257.0f, 105.0f );

    int iFileNum = 0;
    int iIndex = 0;
    bool bOK = GetTextureFileNum( eGameStage , iFileNum, iIndex );

    if ( !bOK )
        return;

    char cBuf[256];
    memset(cBuf,'\0',256);
    sprintf(cBuf, "pvp_map_%d.dds", iFileNum );

    std::string strTemp = cBuf;
    m_pkMapImage->SetInfomation( vTexSize, vSecondTexStartAxis, strTemp , iIndex , 4 );
    m_pkMapImage->ToggleRender( true );
}

bool KGCMatchMapBox::GetTextureFileNum(IN EGCGameStage eGameStage, OUT int& iFileNum, OUT int& iIndex)
{
    std::map<EGCGameStage, std::pair<int, int>>::iterator mit = m_mapMapNameID.find( eGameStage);

    if (mit == m_mapMapNameID.end())
        return false;

    int iNum = m_mapMapNameID[eGameStage].second;
    
    if ( iNum <= 0 ) {
        iFileNum = 0;
    }
    else {
        iFileNum = iNum / 8;
        iIndex = iNum % 8;
    }

    iFileNum += 1;

    return true;
}