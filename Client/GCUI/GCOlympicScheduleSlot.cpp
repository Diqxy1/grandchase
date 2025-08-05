#include "stdafx.h"
#include "GCOlympicScheduleSlot.h"

IMPLEMENT_CLASSNAME( KGCOlympicScheduleSlot );
IMPLEMENT_WND_FACTORY( KGCOlympicScheduleSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicScheduleSlot, "gc_olympic_schedule_slot" );

KGCOlympicScheduleSlot::KGCOlympicScheduleSlot( void )
: m_iNation( -1 )
{
    m_pkStaticGameDay = NULL;
    m_pkStaticGameName = NULL;
    m_pkStaticWin = NULL;
    m_pkStaticWinNick = NULL;
    LINK_CONTROL( "static_game_day",    m_pkStaticGameDay );
    LINK_CONTROL( "static_game_name",   m_pkStaticGameName );
    LINK_CONTROL( "static_win",         m_pkStaticWin );
    LINK_CONTROL( "static_win_nickname", m_pkStaticWinNick );

    for( int i=0 ; i<NUM_NATION ; ++i ) { 
        char szTemp[ 128 ];
        
        m_apkFlag[ i ] = NULL;
        sprintf_s( szTemp, 127, "flag%d", i );
        LINK_CONTROL( szTemp, m_apkFlag[ i ] );
    }
}

KGCOlympicScheduleSlot::~KGCOlympicScheduleSlot( void )
{
}

void KGCOlympicScheduleSlot::OnCreate( void )
{
    m_pkStaticGameDay->InitState( false );
    m_pkStaticGameName->InitState( false );
    m_pkStaticWin->InitState( false );
    m_pkStaticWinNick->InitState( false );

    m_pkStaticGameDay->SetAlign( DT_LEFT );
    m_pkStaticGameName->SetAlign( DT_LEFT );
    m_pkStaticWin->SetAlign( DT_LEFT );
    m_pkStaticWinNick->SetAlign( DT_LEFT );


    for( int i=0 ; i<NUM_NATION ; ++i ) { 
        m_apkFlag[ i ]->InitState( false );
    }

    m_iNation = -1;
}

void KGCOlympicScheduleSlot::ActionPerformed( const KActionEvent& event ) {}

void KGCOlympicScheduleSlot::FrameMoveInEnabledState( void ) {}

void KGCOlympicScheduleSlot::ClearScheduleSlot( void )
{
    m_pkStaticGameDay->InitState( false );
    m_pkStaticGameName->InitState( false );
    m_pkStaticWin->InitState( false );
    m_pkStaticWinNick->InitState( false );

    for( int i=0 ; i<NUM_NATION ; ++i ) { 
        m_apkFlag[ i ]->InitState( false );
    }

    m_iNation = -1;
}

void KGCOlympicScheduleSlot::SetScheduleSlot( const std::wstring& strGameDay_, const std::wstring& strGameName_, const std::wstring& strWinNick_, const int iNation_, const bool bToday_ )
{
    ClearScheduleSlot();

    m_iNation = iNation_ - 1;

    m_pkStaticGameDay->ToggleRender( true );
    m_pkStaticGameName->ToggleRender( true );
    m_pkStaticGameName->SetShadow( true );

    // 오늘 게임이면 색 다르게 
    if( bToday_ ) { 
        std::wostringstream stmDay, stmName;
        stmDay << L"#cFFC100" << strGameDay_ << L"#cX";
        stmName << L"#cFFC100" << strGameName_ << L"#cX";
        m_pkStaticGameDay->SetText( stmDay.str() );
        m_pkStaticGameName->SetText( stmName.str() );
    }
    else { 
        m_pkStaticGameDay->SetText( strGameDay_ );
        m_pkStaticGameName->SetText( strGameName_ );
    }

    // 랭크정보 있을때만 
    if(  iNation_ <= NUM_NATION && m_iNation >= 0 ) { 
        m_pkStaticWin->ToggleRender( true );
        m_pkStaticWinNick->ToggleRender( true );

        m_pkStaticWin->SetText( g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_WIN ) );

        std::wostringstream stm;
        stm << L"#cD4D4D6" << strWinNick_ << L"#cX";
        m_pkStaticWinNick->SetText( stm.str() );

        m_apkFlag[ m_iNation ]->ToggleRender( true );

    }
}