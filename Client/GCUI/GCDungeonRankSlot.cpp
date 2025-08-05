#include "stdafx.h"
#include "GCDungeonRankSlot.h"

IMPLEMENT_CLASSNAME( KGCDungeonRankSlot );
IMPLEMENT_WND_FACTORY( KGCDungeonRankSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonRankSlot, "gc_dungeonrankslot" );

KGCDungeonRankSlot::KGCDungeonRankSlot( void )
{
    m_pkMyBack = NULL;
    m_pkChar = NULL;
    
    m_pkMyRankNum = NULL;
    m_pkRankNum = NULL;

    m_pkStaticName = NULL;
    m_pkStaticStage = NULL;
    m_pkStaticTime = NULL;

    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < EM_RANK_NUM; ++i ) 
    {
        m_pkTopRank[i] = NULL;
        m_pkMyTopRank[i] = NULL;

        sprintf( str, "rank_num%d", i+1 );  
        LINK_CONTROL( str, m_pkTopRank[i] );

        sprintf( str, "my_rank_num%d", i+1 );  
        LINK_CONTROL( str, m_pkMyTopRank[i] );
    }

    LINK_CONTROL( "my_back", m_pkMyBack );
    LINK_CONTROL( "char", m_pkChar );
    
    LINK_CONTROL( "number_rank", m_pkMyRankNum );
    LINK_CONTROL( "number_my_rank", m_pkRankNum );

    LINK_CONTROL( "static_name", m_pkStaticName );
    LINK_CONTROL( "static_stage", m_pkStaticStage );
    LINK_CONTROL( "static_time", m_pkStaticTime );
}

KGCDungeonRankSlot::~KGCDungeonRankSlot( void )
{
}

void KGCDungeonRankSlot::ActionPerformed( const KActionEvent& event )
{
}

void KGCDungeonRankSlot::OnCreate( void )
{
    m_pkMyBack->InitState( false );
    m_pkChar->InitState( false );

    m_pkMyRankNum->InitState( false );
    m_pkMyRankNum->SetStringType( "dungeon_my_rank" );
    m_pkMyRankNum->SetWriteAlign( ALIGN_CENTER_SIDE );
    m_pkMyRankNum->SetShadow( false );
    
    m_pkRankNum->InitState( true );
    m_pkRankNum->SetStringType( "dungeon_rank" );
    m_pkRankNum->SetWriteAlign( ALIGN_CENTER_SIDE );
    m_pkRankNum->SetShadow( false );

    m_pkStaticName->InitState( true );
    m_pkStaticStage->InitState( true );
    m_pkStaticStage->SetAlign( DT_CENTER );

    m_pkStaticTime->InitState( true );
    m_pkStaticTime->SetAlign( DT_RIGHT );

    m_pkStaticName->SetText( L"" );
    m_pkStaticStage->SetText( L"" );
    m_pkStaticTime->SetText( L"" );

    for ( int i = 0; i < EM_RANK_NUM; ++i ) 
    {
        m_pkTopRank[i]->InitState( false );
        m_pkMyTopRank[i]->InitState( false );
    }
}

void KGCDungeonRankSlot::InitUI()
{
    m_pkMyBack->ToggleRender( false );
    m_pkChar->ToggleRender( false );

    m_pkRankNum->ToggleRender( true );
    m_pkMyRankNum->ToggleRender( false );

    m_pkStaticName->SetText( L"" );
    m_pkStaticStage->SetText( L"" );
    m_pkStaticTime->SetText( L"" );

    for ( int i = 0; i < EM_RANK_NUM; ++i ) 
    {
        m_pkTopRank[i]->ToggleRender( false );
        m_pkMyTopRank[i]->ToggleRender( false );
    }
}

void KGCDungeonRankSlot::SetRank( int nRank )
{
    InitUI();

    if ( nRank > 0 && nRank <= EM_RANK_NUM )
    {
        m_pkTopRank[nRank-1]->ToggleRender( true );
        m_pkRankNum->ToggleRender( false );
    }
    else
    {
        m_pkRankNum->SetNumber( nRank );
        m_pkRankNum->ToggleRender( true );
    }
}

void KGCDungeonRankSlot::SetRankData( bool bMyData, KDungeonRankInfo& kRankInfo )
{
    bool bTopRank = ( kRankInfo.m_nRank > 0 && kRankInfo.m_nRank <= EM_RANK_NUM );

    m_pkMyBack->ToggleRender( bMyData );
    m_pkRankNum->ToggleRender( !bMyData && !bTopRank );
    m_pkMyRankNum->ToggleRender( bMyData && !bTopRank );
    m_pkChar->ToggleRender( true );

    for ( int i = 0; i < EM_RANK_NUM; ++i ) 
    {
        m_pkTopRank[i]->ToggleRender( !bMyData && (( i+1 ) == kRankInfo.m_nRank) );
        m_pkMyTopRank[i]->ToggleRender( bMyData && (( i+1 ) == kRankInfo.m_nRank) );
    }

    m_pkRankNum->SetNumber( kRankInfo.m_nRank );
    m_pkMyRankNum->SetNumber( kRankInfo.m_nRank );
    m_pkChar->SetChar( kRankInfo.m_cCharType );


    int nMin = ( kRankInfo.m_dwStageClearTime / 1000 ) / 60;
    int nSec = ( kRankInfo.m_dwStageClearTime / 1000 ) % 60;

    std::wstring wstrClearData = boost::str( boost::wformat( L"%02d : %02d" )%nMin%nSec );

    m_pkStaticName->SetText( kRankInfo.m_wstrNickName );
    m_pkStaticStage->SetText( g_pkStrLoader->GetReplacedString( STR_ID_DUNGEON_RANK_STAGE, "i", kRankInfo.m_nStageNum + 1 ));
    m_pkStaticTime->SetText( wstrClearData );

}
