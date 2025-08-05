#include "stdafx.h"
#include "KGCMatchManager.h"
#include "KGCPartyManager.h"
#include "KGCRoomManager.h"
#include "KGCChannelManager.h"

ImplementSingleton( KGCMatchManager )


KGCMatchManager::KGCMatchManager( void )
: m_iCurrentState(MSTATE_NONE)
, m_iCurrentMode(MMATCHMODE_1VS1)
, m_tmWaitTime(0)
, m_iCurrentExceptMapID(-1)
, m_bStartOption(true)
{
	InitMatchMapName();
}

KGCMatchManager::~KGCMatchManager( void )
{

}

int KGCMatchManager::GetCurrentUserState( void )
{
	if ( SiKGCPartyManager()->IsParty() == false )
		return KGCMatchManager::MUSERSTATE_NONE;

	if ( SiKGCPartyManager()->IsLocalPartyMasterUser() == false )
		return KGCMatchManager::MUSERSTATE_PARTY_USER;

	return KGCMatchManager::MUSERSTATE_PARTY_MASTER;
}

bool KGCMatchManager::IsModePlayable( int iMode )
{
	if ( SiKGCPartyManager()->IsParty() == false ) {
		return true;
	}

	if ( SiKGCPartyManager()->IsLocalPartyMasterUser() == false ) {
		return false;
	}

	return SiKGCPartyManager()->GetCurrentPartyUserCnt() <= iMode;
}	

time_t KGCMatchManager::GetWaitMachingTime( void )
{
	time_t curTime = GCUTIL_TIME::GetCurTime();

	if ( m_tmWaitTime < curTime ) {
		m_tmWaitTime = curTime;		
	}
	if ( m_tmSendWaitTime < curTime ) {
		m_tmSendWaitTime = curTime + 60;
		if ( ( SiKGCPartyManager()->IsParty() && SiKGCPartyManager()->IsLocalPartyMasterUser() ) ||
			!SiKGCPartyManager()->IsParty() ) {
			KP2P::GetInstance()->Send_CurrentMatchModeCountReq();
		}

	}

	return m_tmWaitTime;
}

void KGCMatchManager::SetWaitMachingTime( void )
{
	if ( ( SiKGCPartyManager()->IsParty() && SiKGCPartyManager()->IsLocalPartyMasterUser() ) ||
		!SiKGCPartyManager()->IsParty() ) {
		KP2P::GetInstance()->Send_CurrentMatchModeCountReq();
	}
}

void KGCMatchManager::SetWaitMatchingTime( int iMatchUser, int iMinMatchUser, bool bSystemOn )
{
	time_t curTime = GCUTIL_TIME::GetCurTime();

	if ( bSystemOn == false ) {
		m_tmWaitTime = curTime + 30;
		m_tmSendWaitTime = curTime + 35;
		return;
	}

	int iWaitTime = ( iMinMatchUser * 2 ) - iMatchUser;
	if ( iWaitTime < 0 )
		iWaitTime = 0;

	iWaitTime = iWaitTime * iWaitTime * iWaitTime +30;

	m_tmSendWaitTime = curTime + 60;
	
	if ( (curTime + iWaitTime) < m_tmSendWaitTime )
		m_tmSendWaitTime = (curTime + iWaitTime) + 5;


	if ( iWaitTime + curTime > m_tmWaitTime && m_tmWaitTime > curTime + 60 )
		return;

	m_tmWaitTime = curTime + iWaitTime;
}

void KGCMatchManager::InitMatchMapName( void )
{
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript( luaMgr, "enum.stg" );
	GCFUNC::LoadLuaScript( luaMgr, "MatchMapInfo.lua" );

	int iMapNameStringNum = 0;
	int	iMapNum = 0;
	LUA_BEGIN_TABLE("MATCH_MAP_NAME_TABLE",return) {
		for ( int i = 1 ; ; ++i ) {
			LUA_BEGIN_TABLE(i,break ) {
				LUA_GET_VALUE_DEF( 1, iMapNum,0);
				LUA_GET_VALUE_DEF( 2, iMapNameStringNum,0);
				m_mapMatchMapName.insert(std::make_pair(iMapNum,iMapNameStringNum));
			} LUA_END_TABLE(break)
		}
	} LUA_END_TABLE()
}

int KGCMatchManager::GetMatchMapNameID( int iMapID )
{
	std::map<int, int>::iterator mit = m_mapMatchMapName.find(iMapID);

	if ( mit == m_mapMatchMapName.end() ) return 0;

	return mit->second;
}

int KGCMatchManager::GetCurrentMode( void )
{
	if ( GetCurrentUserState() != KGCMatchManager::MUSERSTATE_NONE && 
		SiKGCPartyManager()->GetCurrentPartyUserCnt() > ( m_iCurrentMode + 1 ) ) {
			m_iCurrentMode = KGCMatchManager::MMATCHMODE_2VS2;
	}

	return m_iCurrentMode;
}

void KGCMatchManager::SetCurrentExceptMapID( int iMapID )
{
	m_iCurrentExceptMapID = iMapID;
}

int KGCMatchManager::GetCurrentExceptMapID( void )
{
	if ( SiKGCPartyManager()->IsParty() && SiKGCPartyManager()->IsLocalPartyMasterUser() == false ) {
		m_iCurrentExceptMapID = -1;
	}

	return m_iCurrentExceptMapID;
}

void KGCMatchManager::InitMatchInfo()
{
	m_iCurrentState = MSTATE_NONE;
	//m_iCurrentMode = MMATCHMODE_1VS1;
	//m_iCurrentExceptMapID  = -1;

	m_tmWaitTime = 0;
	m_tmSendWaitTime = 0;
	m_mapCompleteUser.clear();

}

void KGCMatchManager::SetRoomUser()
{
	m_vecRoomUser.clear();
	for ( std::map<int, std::vector<KPartyMember> >::iterator mit = m_mapCompleteUser.begin() ; mit != m_mapCompleteUser.end() ; ++mit ) {
		for ( std::vector<KPartyMember>::iterator vit = mit->second.begin() ; vit != mit->second.end() ; ++vit ) {
			m_vecRoomUser.push_back(*vit);
		}
	}
}

int KGCMatchManager::GetStartAutoGameMapID()
{
	std::set<EGCGameStage> setGameStage = SiKGCRoomManager()->GetGameModeListInChannel(SiKGCChannelManager()->GetCurrentChannelType());

	for ( std::vector<KPartyMember>::iterator vit = m_vecRoomUser.begin() ; vit != m_vecRoomUser.end() ; ++vit ) {
		setGameStage.erase(static_cast<EGCGameStage>(vit->m_nExceptModeID));
	}

	if ( setGameStage.empty() ) return GC_GS_FOREST_OF_ELF;

	int iMapCnt = static_cast<int>(setGameStage.size());

	int iSelMap = rand() % iMapCnt;

	int i = 0;
	for ( std::set<EGCGameStage>::iterator sit = setGameStage.begin() ; sit != setGameStage.end() ; ++sit, ++i ) {
		if ( i == iSelMap )
			return *sit;
	}

	return GC_GS_FOREST_OF_ELF;
}

bool KGCMatchManager::GetMatchModeState( int iMode, int &iType )
{
	KEVENT_MATCH_MODE_STATE_ACK::iterator mit = m_mapMatchModeState.find(iMode);
	if ( mit == m_mapMatchModeState.end() ) { 
		iType = 0;
		return true;
	}

	std::map<int,std::pair<int,int>>::iterator mitMode = m_kMatchOpenTime.m_mapModeMaxMinInfo.find(iMode);
	if ( mitMode == m_kMatchOpenTime.m_mapModeMaxMinInfo.end() ) {
		iType = 0;
		return true;
	}

	if ( mitMode->second.first > mit->second ) {
		iType = 0;
	}
	else if ( mitMode->second.second < mit->second ) {
		iType = 2;
	}
	else {
		iType = 1;
	}

	return true;
}

bool KGCMatchManager::GetCurrentOpenTime( KMatchOpenTime &kOpenTime )
{
	int iDate = g_kGlobalValue.m_tmServerTime.GetDayOfWeek() - 1;

	std::map<int, KMatchOpenTime>::iterator mit = m_kMatchOpenTime.m_mapOpenTimeTable.find(iDate);

	if ( mit == m_kMatchOpenTime.m_mapOpenTimeTable.end() ) return false;

	kOpenTime = mit->second;

	return true;
}

bool KGCMatchManager::IsAutoMatchOpenTime()
{
	if ( m_kMatchOpenTime.m_bOpenTimeEnable == false) return true;

	KMatchOpenTime kOpenTime;
	if ( GetCurrentOpenTime(kOpenTime) == false )
		return false;

	int iBeginTime = kOpenTime.m_prBeginTime.first * 60 + kOpenTime.m_prBeginTime.second;
	int iEndTime = kOpenTime.m_prEndTime.first * 60 + kOpenTime.m_prEndTime.second;
	int iCurrentTime = g_kGlobalValue.m_tmServerTime.GetHour() * 60 + g_kGlobalValue.m_tmServerTime.GetMinute();

	return iBeginTime <= iCurrentTime && 
		 iEndTime >= iCurrentTime;
}

