#include "stdafx.h"
#include "KGCRoomListManager.h"
#include "KGCWorldMapManager.h"
#include "MyD3D.h"
#include "./mission/GCMissionManager.h" 
#include "GCGlobal.h"
#include "QuestGameManager.h"
#include "KGCRoomManager.h"
#include "GCTextColorManager.h"
#include "KGCDungeonInfoManager.h"


ImplementSingleton( KGCRoomListManager )

KGCRoomListManager::KGCRoomListManager(void)
: m_iCurrentLevel(0)
, m_iCurrentPage(0)
, m_iRoomFilter(GC_GMC_INVALID)
, m_bRoomDungeon(false)
, m_bWaitRoom(false)
, m_iCreateRoomTitleColor(-1)
{
	LoadBossImageFileInfo();
	m_mapDungeonPlayInfo.clear();
	LoadRoomOptionsFile();
}

KGCRoomListManager::~KGCRoomListManager(void)
{
}

void KGCRoomListManager::LoadRoomOptionsFile()
{
	HANDLE hRoomOptions = NULL;
	hRoomOptions = CreateFile(L"RoomOption.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hRoomOptions != INVALID_HANDLE_VALUE)
	{
		DWORD dwVer;
		DWORD dwNumberOfBytesRead;
		ReadFile(hRoomOptions, &dwVer, sizeof(dwVer), &dwNumberOfBytesRead, NULL);

		if (dwVer == 1001)
		{
			ReadFile(hRoomOptions, &m_kCreateRoomConfiguration.m_dwEnabledOptions, sizeof(m_kCreateRoomConfiguration.m_dwEnabledOptions), &dwNumberOfBytesRead, NULL);
			ReadFile(hRoomOptions, &m_kCreateRoomConfiguration.m_dwDisabledChars, sizeof(m_kCreateRoomConfiguration.m_dwDisabledChars), &dwNumberOfBytesRead, NULL);
			ReadFile(hRoomOptions, &m_kCreateRoomConfiguration.m_dwAllowedRankings, sizeof(m_kCreateRoomConfiguration.m_dwAllowedRankings), &dwNumberOfBytesRead, NULL);
			ReadFile(hRoomOptions, &m_kListRoomFilters.m_dwEnabledOptions, sizeof(m_kCreateRoomConfiguration.m_dwEnabledOptions), &dwNumberOfBytesRead, NULL);
			ReadFile(hRoomOptions, &m_kListRoomFilters.m_dwDisabledChars, sizeof(m_kCreateRoomConfiguration.m_dwDisabledChars), &dwNumberOfBytesRead, NULL);
			ReadFile(hRoomOptions, &m_kListRoomFilters.m_dwAllowedRankings, sizeof(m_kCreateRoomConfiguration.m_dwAllowedRankings), &dwNumberOfBytesRead, NULL);
		}

		CloseHandle(hRoomOptions);
	}
}

void KGCRoomListManager::SaveRoomOptionsFile()
{
	HANDLE hRoomOptions = NULL;
	hRoomOptions = CreateFile(L"RoomOption.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hRoomOptions != INVALID_HANDLE_VALUE)
	{
		DWORD dwVer = 1001;
		DWORD dwNumberOfBytesWritten;
		WriteFile(hRoomOptions, &dwVer, sizeof(dwVer), &dwNumberOfBytesWritten, NULL);
		WriteFile(hRoomOptions, &m_kCreateRoomConfiguration.m_dwEnabledOptions, sizeof(m_kCreateRoomConfiguration.m_dwEnabledOptions), &dwNumberOfBytesWritten, NULL);
		WriteFile(hRoomOptions, &m_kCreateRoomConfiguration.m_dwDisabledChars, sizeof(m_kCreateRoomConfiguration.m_dwDisabledChars), &dwNumberOfBytesWritten, NULL);
		WriteFile(hRoomOptions, &m_kCreateRoomConfiguration.m_dwAllowedRankings, sizeof(m_kCreateRoomConfiguration.m_dwAllowedRankings), &dwNumberOfBytesWritten, NULL);
		WriteFile(hRoomOptions, &m_kListRoomFilters.m_dwEnabledOptions, sizeof(m_kCreateRoomConfiguration.m_dwEnabledOptions), &dwNumberOfBytesWritten, NULL);
		WriteFile(hRoomOptions, &m_kListRoomFilters.m_dwDisabledChars, sizeof(m_kCreateRoomConfiguration.m_dwDisabledChars), &dwNumberOfBytesWritten, NULL);
		WriteFile(hRoomOptions, &m_kListRoomFilters.m_dwAllowedRankings, sizeof(m_kCreateRoomConfiguration.m_dwAllowedRankings), &dwNumberOfBytesWritten, NULL);

		CloseHandle(hRoomOptions);
	}
}


void KGCRoomListManager::LoadBossImageFileInfo()
{
	KLuaManager luaMgr;

	m_mapBossImageInfo.clear();
	m_mapRecomUserNUM.clear();

	if( GCFUNC::LoadLuaScript( luaMgr , "BossImageInfo.lua" ) != false )
	{
		LUA_BEGIN_TABLE( "BOSS_RECOM_USER", return; ) {
			for ( int i = 1 ; ; i++ )
			{
				int iDungeon = 0;
				std::map<int,std::pair< int, int >> mapRecomUserNUM;
				LUA_BEGIN_TABLE(i, break;){
					LUA_GET_VALUE_NOASSERT("DUNGEON_NUM",iDungeon, LUA_END_TABLE(); break;);
					LUA_BEGIN_TABLE("LEVEL_TABLE", break; ){
						for ( int j = 1 ; ; j++ )
						{
							int iDungeonLevel = 0;
							std::pair< int, int > pairRecomInfo;

							LUA_BEGIN_TABLE( j, break; ){
								LUA_GET_VALUE_NOASSERT("DUNGEON_LEVEL",iDungeonLevel, LUA_END_TABLE(); continue;);
								LUA_GET_VALUE_NOASSERT("RECOM_MIN_USER",pairRecomInfo.first, LUA_END_TABLE(); continue;);
								LUA_GET_VALUE_NOASSERT("RECOM_MAX_USER",pairRecomInfo.second, LUA_END_TABLE(); continue;);
							} LUA_END_TABLE(break;);

							mapRecomUserNUM.insert(std::pair< int,std::pair< int, int > >(iDungeonLevel,pairRecomInfo));
						}
					} LUA_END_TABLE( break; );
				} LUA_END_TABLE(break;);
				
				m_mapRecomUserNUM.insert(std::pair<int, std::map<int,std::pair<int,int>> >( iDungeon, mapRecomUserNUM ));
			}
		} LUA_END_TABLE( return;)

		LUA_BEGIN_TABLE("BOSS_IMAGE_INFO", return; ){
			for ( int i = 1 ; ; i++ )
			{
				int iDungeon = 0;
				SBossImageInfo Info;

				LUA_BEGIN_TABLE(i, break;){
					LUA_GET_VALUE_NOASSERT("DUNGEON_NUM",iDungeon, LUA_END_TABLE(); break;);
					LUA_GET_VALUE_NOASSERT("FILE_NUM",Info.iBigImageFileIndex, LUA_END_TABLE(); break;);
					LUA_GET_VALUE_NOASSERT("INDEX_NUM",Info.iBigImageOrderIndex, LUA_END_TABLE(); break;);
					LUA_GET_VALUE_NOASSERT("MINI_FILE_NUM",Info.iSmallImageFileIndex, LUA_END_TABLE(); break;);
					LUA_GET_VALUE_NOASSERT("MINI_INDEX_NUM",Info.iSmallImageOrderIndex, LUA_END_TABLE(); break;);
                    LUA_GET_VALUE_NOASSERT("ROOM_FILE_NUM",Info.iRoomBigImageFileIndex, LUA_END_TABLE(); break;);
                    LUA_GET_VALUE_NOASSERT("ROOM_INDEX_NUM",Info.iRoomBigImageOrderIndex, LUA_END_TABLE(); break;);

					LUA_GET_VALUE_NOASSERT("BREAK_FILE_NUM", Info.iBreakBigImageFileIndex, LUA_END_TABLE(); break;);
					LUA_GET_VALUE_NOASSERT("BREAK_INDEX_NUM", Info.iBreakBigImageOrderIndex, LUA_END_TABLE(); break;);
					LUA_GET_VALUE_NOASSERT("BREAK_ROOM_FILE_NUM", Info.iBreakRoomBigImageFileIndex, LUA_END_TABLE(); break;);
					LUA_GET_VALUE_NOASSERT("BREAK_ROOM_INDEX_NUM", Info.iBreakRoomBigImageOrderIndex, LUA_END_TABLE(); break;);

				}LUA_END_TABLE(break;);

				m_mapBossImageInfo.insert(std::pair<int,SBossImageInfo>(iDungeon, Info));
			}

		}LUA_END_TABLE(return;);

	}



}

bool KGCRoomListManager::GetBossBigImageInfo( OUT int &iFileNum, OUT int &iPictureNum, OUT bool &bHellMod )
{
	int iGameMode = static_cast<int>(SiKGCWorldMapManager()->GetGameMode());

	std::map<int, SBossImageInfo>::iterator mit = m_mapBossImageInfo.find(iGameMode);
	if(mit == m_mapBossImageInfo.end()) return false;

	if (m_iCurrentLevel == GC_DUNGEON_LEVEL_BREAK) {
		iFileNum = mit->second.iBreakBigImageFileIndex;
		iPictureNum = mit->second.iBreakBigImageOrderIndex; 
		bHellMod = m_iCurrentLevel == GC_DUNGEON_LEVEL_BREAK;
	}
	else {
		iFileNum = mit->second.iBigImageFileIndex;
		iPictureNum = mit->second.iBigImageOrderIndex;
		bHellMod = m_iCurrentLevel == GC_DUNGEON_LEVEL_HELL;
	}

	return true;
}

bool KGCRoomListManager::GetRoomBossBigImageInfo( OUT int &iFileNum, OUT int &iPictureNum )
{
    int iGameMode = static_cast<int>(SiKGCRoomManager()->GetGameMode());

    std::map<int, SBossImageInfo>::iterator mit = m_mapBossImageInfo.find(iGameMode);
    if(mit == m_mapBossImageInfo.end()) return false;

	if (m_iCurrentLevel == GC_DUNGEON_LEVEL_BREAK) {
		iFileNum = mit->second.iBreakRoomBigImageFileIndex;
		iPictureNum = mit->second.iBreakRoomBigImageOrderIndex;
	}
	else {
		iFileNum = mit->second.iRoomBigImageFileIndex;
		iPictureNum = mit->second.iRoomBigImageOrderIndex;
	}

    return true;
}


int KGCRoomListManager::GetNumMission()
{
	int iMissionCount = 0;

	int iLevel = m_iCurrentLevel;
	EGCGameMode eMode = SiKGCWorldMapManager()->GetGameMode();

	std::vector<DWORD> vecMissionList = g_kGlobalValue.m_kUserMission.GetDungeonMissionList( eMode );

	PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
	for( std::vector<DWORD>::iterator vit = vecMissionList.begin() ; vit != vecMissionList.end() ; ++vit )
	{
		const SMissionInfo* pInfo = SiKGCMissionManager()->GetMissionInfo( *vit );
		if( pPlayer && ( 0 <= pInfo->m_iCharacter) && (pPlayer->m_kUserInfo.GetCurrentChar().iCharType != pInfo->m_iCharacter) )
			continue;

		for(int iSub = 0 ; iSub < static_cast<int>(pInfo->m_vecSubMissions.size()) ; ++iSub ) 
		{
			const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission( pInfo->m_vecSubMissions[iSub] );
			for( int iCon = 0 ; iCon < static_cast<int>(sSubMissionInfo->m_vecConditionID.size()) ; ++ iCon )
			{
				const SConditionInfo* sCondition = SiKGCMissionManager()->GetConditionInfo( sSubMissionInfo->m_vecConditionID[iCon] );
				if( NULL == sCondition )
					continue;

				// 레벨이 조건인 미션이다.(던전이나 몬스터는 지정되지 않음)
				if( pPlayer && ((sCondition->m_pairMonLevelRange.first >= 0) || (sCondition->m_pairMonLevelRange.second >= 0)) )
				{
					// first가 2고 second가 2면, 내 레벨기준으로 -2 ~ 2사이 몬스터를 잡는게 조건임.
					int iLowLevel, iHighLevel;
					if( (sCondition->m_pairMonLevelRange.first >= 0) )
						iLowLevel = pPlayer->GetCurrentChar().iLevel -  sCondition->m_pairMonLevelRange.first;
					else
						iLowLevel = 0;

					if( (sCondition->m_pairMonLevelRange.second >= 0) )
						iHighLevel = pPlayer->GetCurrentChar().iLevel + sCondition->m_pairMonLevelRange.second;
					else
						iHighLevel = MAX_LEVEL;

					if( !g_MyD3D->m_pkQuestGameMgr->IsQuestLevelFromInputLevel( iLowLevel, iHighLevel, eMode, m_iCurrentLevel ) )
						continue;
				}
				else if( !( (sCondition->m_iGameMode == eMode && sCondition->m_pairDifficultRange.first <= m_iCurrentLevel) && 
					(pInfo->m_iCharacter == -1 || pInfo->m_iCharacter == g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType)) )
					continue;
				++iMissionCount;
				break;
			}

		}
	}
	return iMissionCount;
}

std::wstring KGCRoomListManager::GetLevelText(int iLevel) 
{
	std::wstring strTemp;
	switch( iLevel ) {
		case KRLM_LEVEL_1 :
			strTemp = g_pkStrLoader->GetString(STR_ID_DUNGEON_LEVEL_1);
			break;
		case KRLM_LEVEL_2 :
			strTemp = g_pkStrLoader->GetString(STR_ID_DUNGEON_LEVEL_2);
			break;
		case KRLM_LEVEL_3 :
			strTemp = g_pkStrLoader->GetString(STR_ID_DUNGEON_LEVEL_3);
			break;
		case KRLM_LEVEL_HELL :
			strTemp = g_pkStrLoader->GetString(STR_ID_DUNGEON_LEVEL_HELL);
			break;
		case KRLM_LEVEL_BREAK:
			strTemp = g_pkStrLoader->GetString(STR_ID_DUNGEON_LEVEL_BREAK);
			break;
		default:
			break;
	}

	return strTemp;
}

int KGCRoomListManager::ConvertPremium(DWORD dwPremium)
{
	if (!m_bRoomDungeon)
	{
		return KRLM_PRIMIUM_NOT;
	}

	if (dwPremium & KPremiumInfo::TYPE_GOLDSTAGE) {
		return KRLM_PRIMIUM_GOLDEN_TICKET;
	}
	else if (dwPremium & KPremiumInfo::TYPE_GCPREMIUM) {
		return KRLM_PRIMIUM_GCCLUB_PREMIUM;
	}
	else if (dwPremium & KPremiumInfo::TYPE_GCBLOG) {
		return KRLM_PRIMIUM_GCBLOG;
	}
	else if (dwPremium & KPremiumInfo::TYPE_GCCLUB) {
		return KRLM_PRIMIUM_GCCLUB;
	}

	return KRLM_PRIMIUM_NOT;
}

bool KGCRoomListManager::GetBossSmallImageInfo( OUT int &iFileNum, OUT int &iPictureNum, OUT bool &bLock, int iGameMode )
{
	std::map<int, SBossImageInfo>::iterator mit = m_mapBossImageInfo.find(iGameMode);
	if(mit == m_mapBossImageInfo.end()) return false;

	iFileNum = mit->second.iSmallImageFileIndex;
	iPictureNum = mit->second.iSmallImageOrderIndex;	
	bLock = !SiKGCWorldMapManager()->IsPlayableDungeon(static_cast<EGCGameMode>(iGameMode), g_kGlobalValue.m_kUserInfo.GetCurrentCharType() );

	return true;
}

void KGCRoomListManager::SetCurrentLevel( int iLevel )
{
	m_iCurrentLevel = iLevel;
	SiKGCRoomManager()->SetSubDungeonGameMode(m_iCurrentLevel);
	//던전 레벨 이벤트를 날린다.
	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_DUNGEON_ROOM_SELECT_LEVEL ) );

}

bool KGCRoomListManager::IsHellMode()
{
	return m_iCurrentLevel == KRLM_LEVEL_HELL;
}

bool KGCRoomListManager::IsBreakMode()
{
	return m_iCurrentLevel == KRLM_LEVEL_BREAK;
}

void KGCRoomListManager::ClearRoomList()
{
	m_vecRoomList.clear();
	m_iCurrentPage = 0;
	m_iMaxPage = 0;
}

void KGCRoomListManager::UpdateRoomList( std::vector<KSimpleRoomInfo>& vecRoomList, int iCurrentPage, int iMaxPage )
{
	ClearRoomList();

	m_iMaxPage = iMaxPage;
	SetCurrentPage( iCurrentPage, false );

	m_vecRoomList = vecRoomList;
}


void KGCRoomListManager::SetWaitRoom( bool bWaitRoom )
{
	m_bWaitRoom = bWaitRoom;

	if ( bWaitRoom )
		m_vecPreAllRoomList = m_vecRoomList;
	else
		m_vecPreWaitRoomList = m_vecRoomList;

	if( timeGetTime() - KP2P::GetInstance()->GetTimer(KP2P::ESPTUpdateRoomList) < 1000 * 2/*초*/ && m_iCurrentPage == 0 ) {
		std::vector<KSimpleRoomInfo> vecRoomList;

		vecRoomList = m_vecRoomList;

		if ( bWaitRoom ) {
			vecRoomList = m_vecPreWaitRoomList;		
		}
		else {
			if( m_vecPreAllRoomList.empty() == false )
				vecRoomList = m_vecPreAllRoomList;
		}

		UpdateRoomList(vecRoomList, m_iCurrentPage, m_iMaxPage);


		g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_UPDATE_ROOM_LIST ) );


		// UI 업데이트..
		if( g_MyD3D->m_pStateMachine != NULL )
		{
			ClearRoomFilter();
			for( int i = 0 ; i < GC_GMC_COUNT ; ++i )
			{
				EChannelType eChannel = (EChannelType)SiKGCChannelManager()->GetCurrentChannelType();
				if( SiKGCRoomManager()->IsAvailableCategory( eChannel, static_cast<EGCGameModeCategory>(i) ) == false )
					continue;

				KGCRoomListManager::EFILTER_CATEGORY eCategory;
				eCategory = GetFilterEnumFromCategory( (EGCGameModeCategory)i );
				SetFilterEnable( eCategory );
			}
		}

		return;
	}
	KP2P::GetInstance()->SetTimer(KP2P::ESPTUpdateRoomList);
	KP2P::GetInstance()->SetTimer(KP2P::ESPTRoomList);

	SendRoomListReq( false );
}

void KGCRoomListManager::SendRoomListReq( bool bSendDelay_ /*= true*/ )
{
	if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
		return;

	int currentDifficulty = 0;

	if (IsHellMode()) {
		currentDifficulty = GC_DUNGEON_LEVEL_HELL;
	}
	else if (IsBreakMode()) {
		currentDifficulty = GC_DUNGEON_LEVEL_BREAK;
	}
	else {
		currentDifficulty = m_iCurrentLevel;
	}

	KP2P::GetInstance()->Send_RoomListReq( bSendDelay_, GetWaitRoom(), GetRoomFilter(), currentDifficulty, GetCurrentPage() );
}

void KGCRoomListManager::SetRoomFilter( int iFilter, bool bDungeon )
{
	m_bRoomDungeon = bDungeon;
	m_iRoomFilter = iFilter;
}

bool KGCRoomListManager::SetCurrentPage( int iPage_, bool bSendPacket )
{
	if( iPage_ < 0 || iPage_ >= GetMaxPage() )
		return false;

	m_iCurrentPage = iPage_;

	int currentDifficulty = 0;

	if (IsHellMode()) {
		currentDifficulty = GC_DUNGEON_LEVEL_HELL;
	}
	else if (IsBreakMode()) {
		currentDifficulty = GC_DUNGEON_LEVEL_BREAK;
	}
	else {
		currentDifficulty = m_iCurrentLevel;
	}

	if ( bSendPacket ) {
		KP2P::GetInstance()->Send_RoomListReq( false, GetWaitRoom(), GetRoomFilter(), currentDifficulty, iPage_ );
	}

	return true;
}


void KGCRoomListManager::SetFilterEnable( EFILTER_CATEGORY eCategory_ )
{
	if( eCategory_ < 0 || eCategory_ >= NUM_CATEGORY )
	{
		return;
	}
	m_setRoomFilter.insert( eCategory_ );
}

void KGCRoomListManager::SetFilterDIsable( EFILTER_CATEGORY eCategory_ )
{
	if( eCategory_ < 0 || eCategory_ >= NUM_CATEGORY || eCategory_ == ECATEGORY_ALL )
	{
		return;
	}
	m_setRoomFilter.erase( eCategory_ );
}

bool KGCRoomListManager::IsFilterEnabled( EFILTER_CATEGORY eCategory_ )
{
	if( eCategory_ < 0 || eCategory_ >= NUM_CATEGORY )
	{
		return false;
	}
	return m_setRoomFilter.find( eCategory_ ) != m_setRoomFilter.end();
}

void KGCRoomListManager::ClearRoomFilter()
{
	m_setRoomFilter.clear();
}

KGCRoomListManager::EFILTER_CATEGORY KGCRoomListManager::GetFilterEnumFromCategory( EGCGameModeCategory eCategory_ )
{
	switch( eCategory_ )
	{
	case GC_GMC_MATCH:             // 대전
		return ECATEGORY_MATCH;
	case GC_GMC_TAG_MATCH:         // 태그 매치
		return ECATEGORY_TAGMATCH;
	case GC_GMC_GUILD_BATTLE:      // 길드전
		return ECATEGORY_GUILD;
	case GC_GMC_MONSTER_CRUSADER:  // 몬스터 원정대
		return ECATEGORY_MONSTERCRUSADER;
	case GC_GMC_MONSTER_HUNT:      // 몬스터 대전
		return ECATEGORY_MONSTERHUNT;
	case GC_GMC_DEATHMATCH:        // 데스매치
		return ECATEGORY_DEATHMATCH;
    case GC_GMC_FATAL_DEATHMATCH:        // 데스매치
        return ECATEGORY_FATAL_DEATHMATCH;
	case GC_GMC_ANGELS_EGG:        // 천사알깨기
		return ECATEGORY_ANGLE_EGG;
	case GC_GMC_DOTA:              // 점령전
		return ECATEGORY_DOTA;
	default:
		return ECATEGORY_ALL;
	}
	return ECATEGORY_ALL;
}

std::pair<int,int> KGCRoomListManager::GetRecomUser( int iGameMode, int iLevel )
{
	std::map<int, std::map<int,std::pair< int, int >>>::iterator mit = m_mapRecomUserNUM.find(iGameMode);
	if ( mit != m_mapRecomUserNUM.end() ) {
		std::map<int,std::pair< int, int >>::iterator mitRecom = mit->second.find(iLevel);
		if ( mitRecom != mit->second.end() ) {
			return mitRecom->second;
		}
	}

	std::pair<int,int> temp;
	switch(iLevel){
		case KRLM_LEVEL_1 :
			temp.first = 1;
			temp.second = 1;
			break;
		case KRLM_LEVEL_2 :
			temp.first = 1;
			temp.second = 2;
			break;
		case KRLM_LEVEL_3 :
			temp.first = 2;
			temp.second = 3;
			break;
		case KRLM_LEVEL_HELL:
			temp.first = 3;
			temp.second = 4;
			break;
		case KRLM_LEVEL_BREAK:
			temp.first = 4;
			temp.second = 5;
			break;
		default:
			temp.first = 1;
			temp.second = 1;
			break;
	}
	return temp;
}

DWORD KGCRoomListManager::GetCreateRoomTitleColor( )
{
	return SiGCTextColorManager()->GetColor(m_iCreateRoomTitleColor);
}

void KGCRoomListManager::SetCreateRoomTitleColor( int iIndex )
{
	m_iCreateRoomTitleColor = iIndex;
	g_pkUIMgr->SendEvent( KActionEvent( NULL, KGCUIScene::D3DWE_DUNGEON_CLICK_TITLE_COLOR_BTN ) );

}

std::vector<GCITEMID> KGCRoomListManager::GetCurrentModeAndLevelRewardItem( )
{
	std::vector<GCITEMID> vecTemp;

    EGCGameMode emGameMode;
    
    if( g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
    {
        emGameMode = SiKGCRoomManager()->GetGameMode();
    }
    else
    {
        emGameMode = SiKGCWorldMapManager()->GetGameMode();
    }


	SDungeonItemInfo info = SiKGCDungeonInfoManager()->GetDungeonInfo( emGameMode, m_iCurrentLevel);

	for ( std::vector<GCITEMID>::iterator vit = info.m_vecItemInfo.begin() ; vit != info.m_vecItemInfo.end() ; ++vit ) {
		if ( g_pItemMgr->IsCurrentCharacterItem(*vit) ) {
			vecTemp.push_back(*vit);
		}
	}
	return vecTemp;

}