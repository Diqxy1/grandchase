#include "StdAfx.h"
#include "GCCharacterUI.h"
#include "../mission/GCMissionManager.h"
#include "GCDungeonMissionList.h"

IMPLEMENT_CLASSNAME( KGCDungeonMissionList );
IMPLEMENT_WND_FACTORY( KGCDungeonMissionList );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonMissionList, "gc_kgcdungeonmissionlist" );

KGCDungeonMissionList::KGCDungeonMissionList(void)
: m_iListNum( 0 )
{
    m_pkTitle = NULL;
    m_pkBackGround = NULL;
    LINK_CONTROL( "static_title", m_pkTitle );
    LINK_CONTROL( "background", m_pkBackGround );

    for( int i = 0 ; i < DML_LIST_SIZE ; ++i )
    {
        char strControlName[MAX_PATH] = {0,};
        m_apkMissionName[i] = NULL;
        sprintf( strControlName, "static_title%d", i );
        LINK_CONTROL( strControlName, m_apkMissionName[i] );
        m_apkCharacter[i] = NULL;
        sprintf( strControlName, "char_ball%d", i );
        LINK_CONTROL( strControlName, m_apkCharacter[i] );
        m_apkMissionBG[i] = NULL;
        sprintf( strControlName, "bgline%d", i );
        LINK_CONTROL( strControlName, m_apkMissionBG[i] );
    }
}

KGCDungeonMissionList::~KGCDungeonMissionList(void)
{
}

void KGCDungeonMissionList::OnCreate( void )
{
    m_pkTitle->InitState( true );
    m_pkTitle->SetAlign( DT_CENTER );
    m_pkTitle->SetText( g_pkStrLoader->GetString( STR_ID_DUNGEON_MISSION_LIST_TITLE ) );

    m_pkBackGround->InitState( true );

    ClearMissionInfo();

    RefreshBackgroundSize();
}

bool KGCDungeonMissionList::AddMissionInfo( DWORD dwMissionID )
{
    if( m_iListNum >= DML_LIST_SIZE )
        return false;

    const SMissionInfo* pMission = SiKGCMissionManager()->GetMissionInfo( dwMissionID );

    if( pMission == NULL )
        return false;

    m_apkCharacter[m_iListNum]->ToggleRender( true );
    m_apkCharacter[m_iListNum]->SetChar( pMission->m_iCharacter );

    m_apkMissionName[m_iListNum]->ToggleRender( true );
    m_apkMissionName[m_iListNum]->SetText( SiKGCMissionManager()->GetString( pMission->m_iTitleID) );

    m_apkMissionBG[m_iListNum]->ToggleRender( true );

    ++m_iListNum;

    RefreshBackgroundSize();

    return true;
}

void KGCDungeonMissionList::ClearMissionInfo()
{
    for( int i = 0 ; i < DML_LIST_SIZE ; ++i )
    {
        m_apkMissionName[i]->InitState( false );
        //m_apkMissionName[i]->SetAlign( DT_CENTER );
        m_apkMissionName[i]->SetText();

        m_apkCharacter[i]->InitState( false, true );
        m_apkMissionBG[i]->InitState( false );
    }

    m_iListNum = 0;
}

void KGCDungeonMissionList::RefreshBackgroundSize()
{
    DWORD dwHeight = static_cast<DWORD>( 33.f + 27.f * m_iListNum );
    m_pkBackGround->SetSize( m_pkBackGround->GetWidth(), dwHeight );
    SetWidthDirect( m_pkBackGround->GetWidth() );
    SetWidthDirect( dwHeight );
}