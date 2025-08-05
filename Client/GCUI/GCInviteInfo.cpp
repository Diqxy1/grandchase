#include "stdafx.h"
#include ".\GCInviteInfo.h"
//
//
#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "KGCWorldMapManager.h"

IMPLEMENT_CLASSNAME( KGCInviteInfo );
IMPLEMENT_WND_FACTORY( KGCInviteInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteInfo, "gc_invite_info" );

KGCInviteInfo::KGCInviteInfo(void)
: m_pkChannel(NULL)
, m_pkRoomNum(NULL)
, m_pkMode(NULL)
, m_pkRoomTitle(NULL)
{
    LINK_CONTROL( "static_channel",     m_pkChannel );
    LINK_CONTROL( "static_room_num",    m_pkRoomNum );
    LINK_CONTROL( "static_mode",        m_pkMode );
    LINK_CONTROL( "static_room_title",  m_pkRoomTitle );
}

KGCInviteInfo::~KGCInviteInfo(void)
{

}

void KGCInviteInfo::SetInviteInfo( const KInviteInfo& kInfo )
{
    std::wstring strChannelName;
    std::wstring strMode;
    KChannelInfo* pChannelInfo = SiKGCChannelManager()->GetChannel( kInfo.m_dwChannelUID );
    if( pChannelInfo == NULL )
    {
        ASSERT( !"Invalid ChannelUID" );
        strChannelName = L"Invalid Channel";
        strMode = L"Invalid Mode";
    }
    else
    {
        strChannelName = pChannelInfo->m_strChannelName;
        if( CT_DUNGEON == pChannelInfo->m_cType )
        {
            strMode = SiKGCWorldMapManager()->GetCurrentDungeonName( (EGCGameMode)kInfo.m_nGameMode );
        }
        else
        {
            strMode = SiKGCRoomManager()->GetGameModeName( (EGCGameMode)kInfo.m_nGameMode );
        }
    }

    std::wstringstream stm;
    stm << g_pkStrLoader->GetString( STR_ID_INVITE_INFO_CHANNEL ) << strChannelName;
    m_pkChannel->SetText( stm.str() );

    stm.str(L"");
    stm << g_pkStrLoader->GetString( STR_ID_INVITE_INFO_ROOM_NUM ) << (kInfo.m_usRoomID + 1);
    m_pkRoomNum->SetText( stm.str() );

    stm.str(L"");
    stm << g_pkStrLoader->GetString( STR_ID_INVITE_INFO_MODE ) << strMode;
#if defined(NATION_LATINAMERICA)
    m_pkMode->SetForceWordWrapText(stm.str(), true);
#else
	m_pkMode->SetText(stm.str());
#endif

    stm.str(L"");
    stm << g_pkStrLoader->GetString( STR_ID_INVITE_INFO_TITLE ) << kInfo.m_strRoomName;
#if defined(NATION_LATINAMERICA)
    m_pkRoomTitle->SetForceWordWrapText(stm.str(), true);
#else
    m_pkRoomTitle->SetText(stm.str());
#endif

    m_kInviteInfo = kInfo;
}

void KGCInviteInfo::OnCreate()
{
    m_pkChannel->InitState( true );
    m_pkRoomNum->InitState( true );
    m_pkMode->InitState( true );    
    m_pkRoomTitle->InitState( true );
}
