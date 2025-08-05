#include "stdafx.h"
#include "GCGuildInfo.h"
//
//
//
//
#include "../MyD3D.h"
#include "../Procedure.h"
//

IMPLEMENT_CLASSNAME( KGCGuildInfo );
IMPLEMENT_WND_FACTORY( KGCGuildInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildInfo, "gc_guild_info" );

KGCGuildInfo::KGCGuildInfo( void )
{
    m_pkGuildName       = NULL;
    m_pkGuildPoint      = NULL;
    m_pkGuildMaster     = NULL;
    m_pkGuildCommander  = NULL;
    m_pkGuildMemberInfo = NULL;

    LINK_CONTROL( "guild_name_static",          m_pkGuildName );
    LINK_CONTROL( "guild_point_static",         m_pkGuildPoint );
    LINK_CONTROL( "guild_master_static",        m_pkGuildMaster );
    LINK_CONTROL( "guild_commander_list_box",   m_pkGuildCommander );
    LINK_CONTROL( "guild_member_info_static",   m_pkGuildMemberInfo );
}

KGCGuildInfo::~KGCGuildInfo( void )
{
    // empty
}

void KGCGuildInfo::OnCreate( void )
{
    SetSelfInputCheck( true );

    m_pkGuildName->SetSelfInputCheck( false );
    m_pkGuildPoint->SetSelfInputCheck( false );
    m_pkGuildMaster->SetSelfInputCheck( false );
    m_pkGuildCommander->SetSelfInputCheck( false );
    m_pkGuildMemberInfo->SetSelfInputCheck( false );
}

void KGCGuildInfo::FrameMoveInEnabledState( void )
{
    if ( g_bGuildInfoUpdate )
    {		
        SetGuildMark( g_kGuildInfo.m_iMark );
        SetGuildName( g_kGuildInfo.m_strName );
        SetGuildPoint( g_kGuildInfo.m_iPoint );
        SetGuildMaster( g_kGuildInfo.m_strLoad );
        ClearGuildCommander();
        for ( int i = 0; i < NUM_MAX_COMMANDER && i < (int)g_kGuildInfo.m_vecGeneral.size(); ++i )
        {
            AddGuildCommander( g_kGuildInfo.m_vecGeneral[i] );
        }
        SetGuildMemberInfo( g_kGuildInfo.m_iConnMembers, g_kGuildInfo.m_iNumMembers );
        g_bGuildInfoUpdate = false;
    }
}

void KGCGuildInfo::PostDraw( void )
{
}

void KGCGuildInfo::SetGuildMark( int iMark )
{
    m_iMark = iMark;
}

void KGCGuildInfo::SetGuildName( const std::wstring& strName )
{
    m_pkGuildName->SetText( strName );
}

void KGCGuildInfo::SetGuildPoint( int iPoint )
{
    std::wostringstream strmPoint;

    strmPoint << iPoint << g_pkStrLoader->GetString( STR_ID_GUILD_POINT );

    m_pkGuildPoint->SetText( strmPoint.str() );
}

void KGCGuildInfo::SetGuildMaster( const std::wstring& strMaster )
{
    m_pkGuildMaster->SetText( strMaster );
}

void KGCGuildInfo::ClearGuildCommander( void )
{
    m_pkGuildCommander->Clear();
}

void KGCGuildInfo::AddGuildCommander( const std::wstring& strCommander )
{
    m_pkGuildCommander->AddString( strCommander );
}

void KGCGuildInfo::SetGuildMemberInfo( int iCurNum, int iMaxNum )
{
    std::wostringstream strmMemberInfo;

    strmMemberInfo << iMaxNum << g_pkStrLoader->GetString( STR_ID_GUILD_TOTAL_MEMBER )
                   << iCurNum << g_pkStrLoader->GetString( STR_ID_GUILD_CONNECT_MEMBER );

    m_pkGuildMemberInfo->SetText( strmMemberInfo.str() );
}