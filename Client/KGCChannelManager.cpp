#include "stdafx.h"
#include "KGCChannelManager.h"

ImplementSingleton( KGCChannelManager )

KGCChannelManager::KGCChannelManager(void)
{
    m_dwCurrentChannelUID = 0xFFFFFFFF;
    m_eCurrentChannelType = CT_MAX;
}

KGCChannelManager::~KGCChannelManager(void)
{
}

void KGCChannelManager::ClearChannelList()
{
    m_dwDungeonChannelUID = 0xFFFFFFFF; // 이상한값으로 초기화하자..
    m_dwGetLastMatchChannelUID = 0xFFFFFFFF;
    m_vecChannelList.clear();
    m_vecMatchChannelList.clear();
    m_ChannelUIDHash.clear();
}

KChannelInfo* KGCChannelManager::GetChannel( DWORD dwChannelUID_ ) //해당 UID를 가진 채널을 찾아서 반환합니다.
{
    if( m_ChannelUIDHash.find( dwChannelUID_ ) != m_ChannelUIDHash.end() )
    {
        return &((*m_ChannelUIDHash[dwChannelUID_].first)[m_ChannelUIDHash[dwChannelUID_].second]);
    }

    return NULL;
}

std::wstring KGCChannelManager::GetCurrentChannelName()
{
    KChannelInfo* kTempChannel;
    if( m_dwCurrentChannelUID == 0xFFFFFFFF )
    {
        return L"";
    }
    else
    {
        kTempChannel = GetChannel( m_dwCurrentChannelUID );
        if( kTempChannel == NULL )
        {
            return L"";
        }
        else
        {
            return kTempChannel->m_strChannelName;
        }
    }
}

void KGCChannelManager::SetCurrentChannel( DWORD UID_ )
{
    //그럴일은 없겠지만 셋팅 하려는 UID가 없는 UID 일지도 모른다.
    if( GetChannel( UID_ ) == NULL )
    {
        m_dwCurrentChannelUID = 0xFFFFFFFF;
        m_dwGetLastMatchChannelUID = 0xFFFFFFFF;
        m_eCurrentChannelType = CT_MAX;
        return;
    }

    m_dwCurrentChannelUID = UID_;
    m_eCurrentChannelType = (EChannelType)(*m_ChannelUIDHash[m_dwCurrentChannelUID].first)[ m_ChannelUIDHash[m_dwCurrentChannelUID].second].m_cType;

    if( IsMatchChannel() == true )
    {
        m_dwGetLastMatchChannelUID = UID_;
    }
}

void KGCChannelManager::UpdateChannelList( std::vector<KChannelInfo>& kChannelInfo )
{
    //int iUserGrade = (int)g_kGlobalValue.m_kUserInfo.GetUserGrade();

    // 이전의 채널 정보를 클리어 하고..
    ClearChannelList();

    std::vector<KChannelInfo>::iterator vit;
    for( vit = kChannelInfo.begin(); vit != kChannelInfo.end(); ++vit )
    {
        if( (EChannelType)vit->m_cType == CT_DUNGEON )
            m_dwDungeonChannelUID = vit->m_dwChannelUID;
        else
            m_vecMatchChannelList.push_back( *vit );

        m_vecChannelList.push_back( *vit );
        m_ChannelUIDHash[vit->m_dwChannelUID] = std::pair<std::vector<KChannelInfo>*,int>( &m_vecChannelList, m_vecChannelList.size() - 1 );
    }
}

bool KGCChannelManager::IsChannelBonus()
{
    return ( m_eCurrentChannelType == CT_BEGINNER ||
             m_eCurrentChannelType == CT_ROOKIE ||
             m_eCurrentChannelType == CT_EXPERT ||
             m_eCurrentChannelType == CT_MASTER //||
             //m_eCurrentChannelType == CT_GUILD_UNLIMITED ||
             //m_eCurrentChannelType == CT_GUILD_BEGINNER ||
             //m_eCurrentChannelType == CT_GUILD_ROOKIE ||
             //m_eCurrentChannelType == CT_GUILD_EXPERT ||
             //m_eCurrentChannelType == CT_GUILD_MASTER 
             );
             
}

bool KGCChannelManager::IsMatchChannel()
{
    switch( m_eCurrentChannelType )
    {
    case CT_INDIGO:
    case CT_GUILD:
    case CT_DUNGEON:
    case CT_SQUARE:
    case CT_AGIT:
        return false;
    case CT_UNLIMITED:
    case CT_BEGINNER:
    case CT_ROOKIE:
    case CT_EXPERT:
    case CT_MASTER:
    case CT_GUILD_UNLIMITED:
    case CT_GUILD_BEGINNER:
    case CT_GUILD_ROOKIE:
    case CT_GUILD_EXPERT:
    case CT_GUILD_MASTER:
	case CT_AUTOMATCH :
        return true;
    }
    return false;
}

bool KGCChannelManager::MoveChannel(DWORD dwChannelUID)
{
    KChannelInfo* pChennelInfo = GetChannel(dwChannelUID);
    if(pChennelInfo == NULL)
        return false;

    if( true == CheckChannel(static_cast<EChannelType>(pChennelInfo->m_cType)))
        return true;
    
    int iUserGrade = 0;
    if(g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) == true)
        iUserGrade = static_cast<int>(g_kGlobalValue.m_kGuildUserInfo.m_ChannelGrade);
    else
        iUserGrade = static_cast<int>(g_kGlobalValue.m_kUserInfo.GetUserGrade());
    EChannelType eChannelType = g_MyD3D->GradeToChannelType( iUserGrade );


    //CT_DUNGEON      = 0, // 던젼 플레이
    //    CT_GUILD        = 1, // 길드 채널
    //    CT_INDIGO       = 2, // 용사의 섬 채널
    //    CT_UNLIMITED    = 3, // 자유 대전 채널
    //    CT_BEGINNER     = 4, // 동 떠돌이기사단  ~ 은 떠돌이기사단
    //    CT_ROOKIE       = 5, // 금 떠돌이기사단  ~ 은 용병기사단
    //    CT_EXPERT       = 6, // 금 용병기사단    ~ 동 왕립기사단
    //    CT_MASTER       = 7, // 은 왕립기사단    ~ 금 신의기사단
    //    CT_SQUARE       = 8, // 광장~
    g_MyD3D->GoChannel();

    switch(static_cast<EChannelType>(pChennelInfo->m_cType))
    {
    case CT_DUNGEON:
        if( false == g_MyD3D->m_pStateMachine->GoWorldMap() )
            return false;
        break;
    case CT_INDIGO:
        return false;
    case CT_SQUARE:
        return false;
    case CT_UNLIMITED:
    case CT_BEGINNER:
    case CT_ROOKIE:
    case CT_EXPERT:
    case CT_MASTER:
	case CT_AUTOMATCH:
        if( static_cast<EChannelType>(pChennelInfo->m_cType) != eChannelType && // 자기 계급이거나..
            static_cast<EChannelType>(pChennelInfo->m_cType) != CT_UNLIMITED &&
			static_cast<EChannelType>(pChennelInfo->m_cType) != CT_AUTOMATCH){  // 자유 채널이면..
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENTER_CHANNEL_ERROR9 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return false;
        }
        g_MyD3D->OnMatchButton();
        if( false == g_MyD3D->m_pStateMachine->GoMatch(dwChannelUID) )
            return false;
        break;
    case CT_GUILD_UNLIMITED:
    case CT_GUILD_MASTER:
    case CT_GUILD_EXPERT:
    case CT_GUILD_ROOKIE:
    case CT_GUILD_BEGINNER:
        if( static_cast<EChannelType>(pChennelInfo->m_cType) != eChannelType && // 자기 계급이거나..
            static_cast<EChannelType>(pChennelInfo->m_cType) != CT_GUILD_UNLIMITED ){  // 자유 채널이면..
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ENTER_CHANNEL_ERROR9 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
                return false;
        }
        g_MyD3D->OnMatchButton();
        if( false == g_MyD3D->m_pStateMachine->GoMatch(dwChannelUID) )
            return false;
        break;

    default:
        return false;
    }


    return true;
}

bool KGCChannelManager::MoveChannelType( int eChannelType )
{
    std::vector<KChannelInfo>   m_vecMatchChannelList;

    for(std::vector<KChannelInfo>::iterator vIt = m_vecMatchChannelList.begin(); vIt != m_vecMatchChannelList.end(); vIt++)
    {
        if(vIt->m_cType == static_cast<EChannelType>(eChannelType))
        {
            MoveChannel(vIt->m_dwChannelUID);
            return true;
        }
    }
    return false;
}

bool KGCChannelManager::GoAutoMatch()
{
	for ( std::vector<KChannelInfo>::iterator vit = m_vecMatchChannelList.begin() ; vit != m_vecMatchChannelList.end() ; ++vit ) {
		if ( vit->m_cType == CT_AUTOMATCH ) {
			return g_MyD3D->m_pStateMachine->GoMatch(vit->m_dwChannelUID);
		}
	}

	return false;
}

bool KGCChannelManager::GoCustomMatch()
{
	for ( std::vector<KChannelInfo>::iterator vit = m_vecMatchChannelList.begin() ; vit != m_vecMatchChannelList.end() ; ++vit ) {
		if ( vit->m_cType == CT_UNLIMITED ) {
			return g_MyD3D->m_pStateMachine->GoMatch(vit->m_dwChannelUID);
		}
	}

	return false;
}

bool KGCChannelManager::IsAutoMatchChannel()
{
    if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP ) &&  SiKGCChannelManager()->CheckChannel( CT_AUTOMATCH ) )
        return true;

    return false;
}

bool KGCChannelManager::IsPVPBallanceServer()
{
    if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH )
        return true;

    return false;
}

bool KGCChannelManager::IsCreateLimitServer()
{
    if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_CREATE_LIMIT ) && SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH )
        return true;
    return false;
}
