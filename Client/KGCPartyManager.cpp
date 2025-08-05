#include "KGCPartyManager.h"
#include "gcui/GCAgitOverlayUI.h"

ImplementSingleton( KGCPartyManager );

KGCPartyManager::KGCPartyManager(void)
: m_bIsJoinParty(false)
, m_dwPartyMasterUID(0)
, m_bIsInviteState(true)
{
    m_wsRoomPass = L"";
}

KGCPartyManager::~KGCPartyManager(void)
{
}

void KGCPartyManager::SetPartyMemberList( std::vector<KPartyMember>& vecPartyPeople )
{
    if(vecPartyPeople.empty())
    {
        EndParty();
        return;
    }
    m_bIsJoinParty = true;
    m_vecPartyPeople = vecPartyPeople;
    for(std::vector<KPartyMember>::iterator vIter = vecPartyPeople.begin();vIter != vecPartyPeople.end();vIter++)
    {
        if(vIter->m_IsHost)
        {
            m_dwPartyMasterUID = vIter->m_dwUID;
        }

		m_setInviteUserList.erase(vIter->m_dwUID);
    }
	

}

void KGCPartyManager::EndParty( void )
{
    m_bIsJoinParty = false;
    m_vecPartyPeople.clear();
    m_dwPartyMasterUID = 0;
	m_setInviteUserList.clear();
}

void KGCPartyManager::OutPartyMember( DWORD dwUserUID )
{
    KP2P::GetInstance()->Send_BanPartyUserReq( dwUserUID );
}


void KGCPartyManager::LeaveParty( void )
{
    KP2P::GetInstance()->Send_LeavePartyUserReq( g_kGlobalValue.m_kUserInfo.dwUID );
    SetInviteState( true );
}

void KGCPartyManager::SendInviteUser( DWORD dwUserUID, int iPartyType )
{
	if ( iPartyType == KPartyData::PT_MATCH ) {
		if ( static_cast<int>( m_setInviteUserList.size() + m_vecPartyPeople.size() ) > KPM_MATCH_PARTY_MAX_INVITE_USER ) {
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR5)
				,L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
			return;
		}

		if ( m_vecPartyPeople.empty() && m_setInviteUserList.size() >= KPM_MATCH_PARTY_MAX_INVITE_USER ) {
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR5)
			,L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
			return;
		}

		for ( std::vector<KPartyMember>::iterator vit = m_vecPartyPeople.begin() ; vit != m_vecPartyPeople.end() ; ++vit ) {
			if ( dwUserUID == vit->m_dwUID ) {
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR4)
					,L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
				return;
			}
		}

		if ( m_setInviteUserList.insert(dwUserUID).second == false ) {
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_INVITE_FRIEND_LIST_ERR3)
				,L"",KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
			return;
		}
	}

	if ( m_bIsJoinParty == false && (static_cast<int>(m_setInviteUserList.size()) == 1 || iPartyType == KPartyData::PT_NORMAL)) {
        KP2P::GetInstance()->Send_CreatePartyReq( dwUserUID, iPartyType );
	}
	else if ( m_bIsJoinParty == false )
	{
		m_setWaitInviteUserList.insert(dwUserUID);
	}
	else {
        KP2P::GetInstance()->Send_InvitePartyReq( dwUserUID, iPartyType );
	}
}

void KGCPartyManager::SendWaitInviteUser( int iPartyType )
{
	std::set<DWORD> setTemp = m_setWaitInviteUserList;

	for ( std::set<DWORD>::iterator sit = setTemp.begin() ; sit != setTemp.end() ; ++sit ) {
		if ( m_bIsJoinParty == false && static_cast<int>(m_setInviteUserList.size()) == 1 ) {
			KP2P::GetInstance()->Send_CreatePartyReq( *sit, iPartyType );
			m_setWaitInviteUserList.erase(*sit);
		}
		else if ( m_bIsJoinParty == false )
		{
			m_setWaitInviteUserList.insert(*sit);
		}
		else {
			KP2P::GetInstance()->Send_InvitePartyReq( *sit, iPartyType );
			m_setWaitInviteUserList.erase(*sit);
		}
	}

}

void KGCPartyManager::GoRoom( DWORD dwUserUID  , std::wstring wsPass)
{
    m_wsRoomPass = wsPass;

    if(dwUserUID == -1) // 방을 만드는 파티장임
    {
        //SiKGCRoomManager()->CreateRoomInfo();
    }
    else // 떨거지들 따라가기
    {
        KP2P::GetInstance()->Send_UserServerRoomReq( dwUserUID );
    }
}

bool KGCPartyManager::IsLocalPartyMasterUser( void )
{
    return g_kGlobalValue.m_kUserInfo.dwUID == m_dwPartyMasterUID; 
}

bool KGCPartyManager::IsPartyMasterUser( DWORD dwUserUID )
{
    return dwUserUID == m_dwPartyMasterUID; 
}

bool KGCPartyManager::IsLocalPartyMember( DWORD dwUserUID )
{
    return g_kGlobalValue.m_kUserInfo.dwUID == dwUserUID ; 
}

void KGCPartyManager::SetPartyInfo(const KPartyData& kPartyData )
{
    m_kPartyData = kPartyData;
}

KPartyData KGCPartyManager::GetPartyInfo( void )
{
    return m_kPartyData;
}

void KGCPartyManager::ChangePartyMasterUser( DWORD dwUserUID )
{
    KP2P::GetInstance()->Send_ChangePartyHostReq( dwUserUID );
}
void KGCPartyManager::JoinParty( bool bIsJoin )
{
    KP2P::GetInstance()->Send_JoinPartyReq( m_kPartyData , bIsJoin );
}
void KGCPartyManager::JoinPartyToInvite( bool bIsInvite )
{
    KP2P::GetInstance()->Send_InvitePartyAnswer( m_kPartyData , bIsInvite );
}

bool KGCPartyManager::IsParty( void )
{
    return m_bIsJoinParty;
}

bool KGCPartyManager::IsDungeonParty( void )
{
    if ( static_cast<int> ( m_vecPartyPeople.size())   > static_cast<int> (OLD_MAX_DUNGEON_PLAYER_NUM) )
        return false;

    return true;
}

void KGCPartyManager::SetInviteeUserInfo( bool bInviteUser)
{
    m_bIsInvitee = bInviteUser;
}

bool KGCPartyManager::GetInviteeUserInfo( void )
{
    return m_bIsInvitee;
}

std::wstring KGCPartyManager::GetRoomPass ( void )
{
    return m_wsRoomPass;
}

bool KGCPartyManager::IsMaxSize()
{
    if (static_cast<int>( m_vecPartyPeople.size()) >= MAX_PARTY_SIZE)
        return true;

    return false;
}

bool KGCPartyManager::SetInviteState( bool bInvite )
{
    return m_bIsInviteState = bInvite;
}

bool KGCPartyManager::IsInviteState()
{
    return m_bIsInviteState;
}

void KGCPartyManager::EraseInvitePartyUser( DWORD dwUserUID )
{
	m_setInviteUserList.erase(dwUserUID);
}

bool KGCPartyManager::IsMatchReady( bool bAll )
{
	bool bReady = true;

	for(std::vector<KPartyMember>::iterator vIter = m_vecPartyPeople.begin();vIter != m_vecPartyPeople.end();vIter++)
	{
		if ( vIter->m_dwUID == g_kGlobalValue.m_kUserInfo.dwUID && bAll == false) {
			return vIter->m_dwState == KPartyMember::US_READY;
		}

		if ( bReady && vIter->m_dwState != KPartyMember::US_READY) {
			bReady = false;
		}
	}

	if ( m_vecPartyPeople.empty() )
		bReady = false;

	return bReady;
}