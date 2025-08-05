#include "stdafx.h"
#include ".\gcusermission.h"
#include "mission/GCMissionManager.h"

#include "KGCRoomManager.h"

//

//
//
#include "MyD3D.h"
#include "GCStateMachine.h"
//
#include "gcui/KGCMissionUI_S4.h"

KGCUserMission::KGCUserMission(void)
: m_dwDeleteReadyMissionID( 0 )
, m_bShowMissionUI(false)
{
    m_vecCompleteThisTime.clear();
	m_vecGuildExceptionMissionID.clear();
}

KGCUserMission::~KGCUserMission(void)
{
    ClearUserMission();
}

void KGCUserMission::SetUserMissionList( const std::vector<KMissionData>& kList, bool bNewMision/*==true*/ )
{
    std::vector< DWORD > vecBefore;
    GetUnCompleteMission( vecBefore );

    std::vector<KMissionData>::const_iterator vit;
    for( vit = kList.begin(); vit != kList.end() ; ++vit )
    {
        SetUserMission( (*vit), bNewMision );
    }
	RefreshUserMission();

    std::vector< DWORD > vecAfter;
    GetUnCompleteMission( vecAfter );

    m_vecCompleteThisTime = GetCompleteMission( vecBefore, vecAfter );
}

void KGCUserMission::SetUserMission( const KMissionData& kData, bool bNewMision/*=true*/ )
{
    typedef std::map< DWORD, std::vector< std::pair< int, int > > > UserMissionList;
    typedef std::vector< std::pair< int, int > > SubMissionList;

    const SMissionInfo *sMissionInfo = SiKGCMissionManager()->GetMissionInfo( kData.m_dwMissionID );
    
	if( sMissionInfo == NULL )
		return;

    SubMissionList vecCompletion;
    for( int i = 0 ; i < (int)sMissionInfo->m_vecSubMissions.size() ; ++i )
    {
        const SSubMissionInfo* sSubMission = SiKGCMissionManager()->GetSubMission( sMissionInfo->m_vecSubMissions[i] );

		//서브미션이 -1로 셋팅되있으면 아무것도 하지 않는다.
		if( sMissionInfo->m_vecSubMissions[i] == -1 )
			continue;

		if( sSubMission == NULL )
		{
			ASSERT( !"SubMissionID is not in list" );
		}

        int iSubMissionID = sSubMission->m_iSubMissionID;
        int iCount = 0;
        if( sSubMission->m_sCompletion.m_dwItemID != 0 )
        {
            iCount = g_pItemMgr->GetInventoryItemDuration( sSubMission->m_sCompletion.m_dwItemID/10 );
        }
        vecCompletion.push_back( SubMissionList::value_type(iSubMissionID, iCount ) );
    }

	SetInsertMission( kData.m_dwMissionID, vecCompletion, bNewMision );

    if( m_mapUserMission.count( kData.m_dwMissionID ) == 0 )
    {
        ASSERT( !"이건 있을 수 없다. 방금 넣은 미션이 없다니!!" );
        return;
    }

    SubMissionList& vecUpdateCompletion = m_mapUserMission[kData.m_dwMissionID];
    for( int i = 0 ; i < (int)vecUpdateCompletion.size() ; ++i )
    {
        int iCompletionID = vecUpdateCompletion[i].first;
        if( kData.m_mapCompletionInfo.count( iCompletionID ) == 0 )
        {
            //이건 아이템을 체크 해야 됩니다.
            continue;
        }

        std::map<int,int>::const_iterator mit = kData.m_mapCompletionInfo.find( iCompletionID );
        if( mit == kData.m_mapCompletionInfo.end() )
            continue;

        vecUpdateCompletion[i].second = mit->second;
    }
}

void KGCUserMission::RefreshUserMission()  //유저의 미션 수행 상황을 다시 알아봅니다.
{
    RefreshMissionItemCount();
}

void KGCUserMission::RefreshMissionItemCount()
{
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        for( int i = 0 ; i < (int)mit->second.size() ; ++i )
        {
			if( mit->second[i].first == -1 )
				continue; 

            const SSubMissionInfo* sInfo = SiKGCMissionManager()->GetSubMission( mit->second[i].first );
            if( sInfo == NULL )
            {
                ASSERT_MBOX( "Not in Mission script : " << mit->first );
                continue;
            }
            if( sInfo->m_sCompletion.m_dwItemID == -1 || 
                sInfo->m_sCompletion.m_dwItemID == 0 )
                continue;

            mit->second[i].second = g_pItemMgr->GetInventoryItemDuration( sInfo->m_sCompletion.m_dwItemID/10 );
        }
    }
}

void KGCUserMission::RefreshMissionPeriod()
{

}

int KGCUserMission::CompleteMission( DWORD dwMissionID )    //수행 중인 미션을 완료 합니다. (완료가 가능할 시 )
{
    if( IsUserMission( dwMissionID ) == false )
        return UM_MISSION_NOT_EXIST;

    int iResult = SiKGCMissionManager()->CheckCompleteMission( dwMissionID, m_mapUserMission[dwMissionID] );
   
    switch( iResult )
    {
    case KGCMissionManager::GC_MM_OK:
        iResult = UM_OK;
        break;
    case KGCMissionManager::GC_MM_NOT_ENOUGH:
        iResult = UM_NOT_ENOUGH;
        break;
    case KGCMissionManager::GC_MM_MISSION_NOT_EXIST:
        ASSERT_MBOX( "Mission is Not Exist : "<< dwMissionID );
        iResult = UM_NOT_ENOUGH;
        break;
    case KGCMissionManager::GC_MM_SUBMISSION_NOT_EXIST:
        ASSERT_MBOX( "SubMission is Not Exist : "<< dwMissionID );
        iResult = UM_NOT_ENOUGH;
        break;
    case KGCMissionManager::GC_MM_CONDITION_NOT_EXIST:
        ASSERT_MBOX( "Condition is Not Exist : "<< dwMissionID );
        iResult = UM_NOT_ENOUGH;
        break;
    default:
        iResult = UM_NOT_ENOUGH;
        break;
    }

    //  에러와 상관없이 있으면 무조껀 지운다.
    DeleteMission( dwMissionID );
    
    return iResult;   //혹시나..
}

int KGCUserMission::DeleteMission( DWORD dwMissionID )    //유저가 미션을 삭제 합니다.
{
    if( IsUserMission( dwMissionID ) == false )
        return UM_MISSION_NOT_EXIST;

    m_mapUserMission.erase( dwMissionID );

    return UM_OK;
}

int KGCUserMission::AddMission( DWORD dwMissionID )       //미션을 등록합니다.
{
    if( IsUserMission( dwMissionID ) == true )
        return UM_MISSION_ALREADY_EXIST;

    const SMissionInfo* psInfo = SiKGCMissionManager()->GetMissionInfo( dwMissionID );

    if( psInfo == NULL )
        return UM_MISSION_NOT_EXIST;

    std::vector< std::pair< int, int > > vecSubMission;
    for( int i = 0 ; i < (int)psInfo->m_vecSubMissions.size() ; ++i )
    {
        vecSubMission.push_back( std::make_pair( psInfo->m_vecSubMissions[i], 0 ) );
    }

	SetInsertMission( dwMissionID, vecSubMission );

    RefreshUserMission();

    return UM_OK;
}

bool KGCUserMission::IsUserMission( DWORD dwMissionID )
{
    if( m_mapUserMission.count( dwMissionID ) == 0 )
        return false;
    return true;
}

int KGCUserMission::MissionCharacter( DWORD dwMissionID )
{
    return SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter;
}

int KGCUserMission::MissionType( DWORD dwMissionID )
{
    return SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iMissionType;
}

bool KGCUserMission::GetCharMission( int iCharKind , std::vector< DWORD >& vecList , bool bcommon) //해당 케릭터의 미션 리스트를 받아옵니다.
{
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        const SMissionInfo* sInfo = SiKGCMissionManager()->GetMissionInfo( mit->first );

        if( sInfo == NULL )
        {
            ASSERT_MBOX( "Not in Mission script : " << mit->first );
            continue;
        }

        if( sInfo->m_iCharacter == iCharKind ||( sInfo->m_iCharacter == -1 && bcommon) )
        {
            vecList.push_back( mit->first );
        }
    }

    return true;
}

void KGCUserMission::LoadGuideExceptionMssion(std::string strFileName_ )
{
	m_vecGuildExceptionMissionID.clear();
	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr , strFileName_.c_str(), false ) != false )
	{
		LUA_BEGIN_TABLE( "Table", return;)
		{
			for( int i = 1; ; i++ ){
				DWORD dwMissionID;
				LUA_GET_VALUE_NOASSERT(i,dwMissionID,/*IS_ACTIVE_EVENT[i]*/break);
				m_vecGuildExceptionMissionID.push_back(dwMissionID);
			}
		}
		LUA_END_TABLE(/* RecomLevel */;);
	}
	return;

}

void KGCUserMission::ClearUserMission()
{
    m_mapUserMission.clear();
}

DWORD KGCUserMission::IsInvalidMission( DWORD dwMissionID )
{
    std::vector< DWORD > vecInvalidList;
    if( GetInvalidMissionList( dwMissionID , vecInvalidList ) == false )
        return 0;

    for( int i = 0 ; i < (int)vecInvalidList.size() ; ++i )
    {
        if( IsUserMission( vecInvalidList[i] ) == true )
            return vecInvalidList[i];
    }

    return 0;
}

bool KGCUserMission::GetInvalidMissionList( DWORD dwMissionID , std::vector< DWORD >& vecList, bool bNewMission/*==true*/ )
{
    if( IsUserMission( dwMissionID ) == false )
        return false;

    const SMissionInfo* sInfo = SiKGCMissionManager()->GetMissionInfo( dwMissionID );
    vecList.insert( vecList.end(), sInfo->m_vecInvalidMissions.begin(), sInfo->m_vecInvalidMissions.end() );
    return true;
}

int KGCUserMission::GetCompleteMissionNum()
{
    int iCompleteCount = 0;
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        if( IsCompleteMission( mit->first ) == true )
            ++iCompleteCount;
    }

    return iCompleteCount;
}

int KGCUserMission::GetCompleteMissionNum(int iCharType)
{
    int iCompleteCount = 0;
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        if( IsCompleteMission( mit->first ) == true &&
            ((MissionCharacter( mit->first ) == -1) || (MissionCharacter( mit->first ) == iCharType)) )
            ++iCompleteCount;
    }

    return iCompleteCount;
}

bool KGCUserMission::IsCompleteMission( DWORD dwMissionID )
{
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    mit = m_mapUserMission.find( dwMissionID );

    if( mit == m_mapUserMission.end() )
        return false;

	if( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter != -1 &&
		!g_kGlobalValue.m_kUserInfo.IsCharExist( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter ) )
		return false;
    
    int iCompleteSubMissionCount = 0;
    int iSize = (int)mit->second.size();
    for( int i = 0 ; i < iSize; ++i )
    {
        if( IsCompleteSubMission( mit->second[i].first ,mit->second[i].second ) == true )
            ++iCompleteSubMissionCount;
    }

    if( iSize == iCompleteSubMissionCount )
        return true;

    return false;
}

bool KGCUserMission::IsCompleteSubMission( int iSubMissionID, int iCount )
{
	if( iSubMissionID == -1 )
		return true;

    const SSubMissionInfo* sInfo = SiKGCMissionManager()->GetSubMission( iSubMissionID );
    if( sInfo == NULL )
    {
        ASSERT( !"서브미션이 스크립트에 없거나 스크립트가 잘못 로딩되었다." );
        return false;
    }
    if( sInfo->m_sCompletion.m_iCount <= iCount )
    {
        return true;
    }
    return false;
}

void KGCUserMission::SetCheck( DWORD dwMissionID, bool bOnOff /*= true */)
{
    if( bOnOff == true )
    {
        if( m_setCheckedMission.count( dwMissionID ) != 0 )
            return;
        m_setCheckedMission.insert( dwMissionID );
    }
    else
    {
        if( m_setCheckedMission.count( dwMissionID ) == 0 )
            return;
        m_setCheckedMission.erase( dwMissionID );
    }
}

bool KGCUserMission::GetCheck( DWORD dwMissionID )
{
    if( m_setCheckedMission.count( dwMissionID ) == 0 )
        return false;
    return true;
}

std::vector< DWORD > KGCUserMission::GetCharacterMission( int iCharKind )  // 해당 케릭터가 수행할 수 있는 미션 리스트를 받습니다.
{
    std::vector< DWORD > vecList;
    vecList.reserve( 15 );

    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        if( MissionCharacter( mit->first ) == iCharKind )
            vecList.push_back( mit->first );
    }

    return vecList;
}

std::vector< DWORD > KGCUserMission::GetCharacterAndCommonMission( int iCharKind )  // 해당 케릭터가 수행할 수 있는 미션 리스트를 받습니다.
{
    std::vector< DWORD > vecList;
    vecList.reserve( 15 );

    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        if( MissionCharacter( mit->first ) == iCharKind || MissionCharacter( mit->first ) == -1 )
            vecList.push_back( mit->first );
    }

    return vecList;
}

std::vector< DWORD > KGCUserMission::GetTypeFromCharacterMission( int iCharKind, int iTabKind )
{
    std::vector< DWORD > vecList;
    vecList.reserve( 15 );

    //iTabKind : 전체(0), 일반(1), 캐릭터(2), 이벤트(3)
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        if( ((-1 == MissionCharacter( mit->first )) || (MissionCharacter( mit->first ) == iCharKind)) && 
            (0 == iTabKind || MissionType( mit->first ) == iTabKind) )
            vecList.push_back( mit->first );
    }

    return vecList;
}

int KGCUserMission::GetCharMissionNum( int iCharKind )
{
    std::vector<DWORD> vecCharMission;
    GetCharMission( iCharKind, vecCharMission );
    return (int)vecCharMission.size();
}

int KGCUserMission::GetCharCompleteMissionNum( int iCharKind )
{
    std::vector<DWORD> vecCharMission;
    GetCharMission( iCharKind , vecCharMission );

    int iCompeteCount = 0;
    for( int i = 0 ; i < (int)vecCharMission.size() ; ++i )
    {
        if( IsCompleteMission( vecCharMission[i] ) == true )
            ++iCompeteCount;
    }

    return iCompeteCount;
}

int KGCUserMission::GetTypeFromCharacterMissionNum( int iCharKind, int iTabKind )
{
    std::vector< DWORD > vecList;
    vecList.reserve( 15 );

    //iTabKind : 전체(0), 일반(1), 캐릭터(2), 이벤트(3)
    int iCnt = 0;
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        if( ((-1 == MissionCharacter( mit->first )) || (MissionCharacter( mit->first ) == iCharKind)) && 
            (0 == iTabKind || MissionType( mit->first ) == iTabKind) )
            ++iCnt;
    }

    return iCnt;
}

bool KGCUserMission::IsFailedMission( DWORD dwMissionID )
{
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    mit = m_mapUserMission.find( dwMissionID );

    if( mit == m_mapUserMission.end() )
        return false;

    //추후에 조건이 생기면 구현, 아직은 실패해서 더이상 수행할 수 없는 조건이 없다.

    return false;
}

int KGCUserMission::GetCompletionCount( DWORD dwMissionID, int iSubMissionID )
{
	RefreshMissionItemCount();
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    mit = m_mapUserMission.find( dwMissionID );

    if( mit == m_mapUserMission.end() )
        return 0;

    for( int i = 0 ; i < (int)mit->second.size() ; ++i )
    {
        if( mit->second[i].first == iSubMissionID )
        {
            return mit->second[i].second;
        }
    }
    return 0;
}

void KGCUserMission::InitVirtualMission()
{
	//일단 가상의 카운터는 비워준다.
	m_vecVirtualCount.clear();
    m_vecVirtualCountMainMissionID.clear();

	//유저가 현재 진행중인 SubMission의 정보들로 채워준다.
	std::vector< DWORD > vecCharMissionList;
	GetCharMission( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, vecCharMissionList, true );
    PLAYER *pPlayer = g_MyD3D->GetMyPlayer();
    if( NULL == pPlayer )
        return;

	for( int i = 0 ; i < (int)vecCharMissionList.size() ; ++i )
	{
		for( int j = 0 ; j < (int)m_mapUserMission[vecCharMissionList[i]].size() ; ++j )
		{
            // 미션 수행 가능레벨 체크( -1이면 제한없음 )
            const SMissionInfo* MainMission = SiKGCMissionManager()->GetMissionInfo( vecCharMissionList[i] );
            if( pPlayer == NULL || MainMission == NULL )
                continue;

            int iLimitMin = MainMission->m_pairLevelLimit.first;
            int iLimitMax = MainMission->m_pairLevelLimit.second;
            if( 0 > MainMission->m_pairLevelLimit.first ) iLimitMin = 0;
            if( 0 > MainMission->m_pairLevelLimit.second ) iLimitMax = pPlayer->GetCurrentChar().iLevel;
            if( iLimitMin > pPlayer->GetCurrentChar().iLevel || pPlayer->GetCurrentChar().iLevel > iLimitMax )
                continue;

			m_vecVirtualCount.push_back( m_mapUserMission[vecCharMissionList[i]][j] );
            m_vecVirtualCountMainMissionID.push_back( vecCharMissionList[i] );
		}
	}
}

// 레벨업 후 미션조건( 내 캐릭터 레벨 (-) ~ (+) 몬스터 Kill)에 만족될 경우가 생김
void KGCUserMission::AddVirtualMissionToLevelUp()
{
    std::vector< DWORD > vecCharMissionList;
    GetCharMission( g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType, vecCharMissionList, true );
    PLAYER *pPlayer = g_MyD3D->GetMyPlayer();
    if( NULL == pPlayer || 0 >= vecCharMissionList.size())
        return;

    std::vector< DWORD > vecCheckVirtualMission;
    std::vector< std::pair< int, int > >::iterator vecIter = m_vecVirtualCount.begin();
    for(; vecIter != m_vecVirtualCount.end(); ++vecIter)
        vecCheckVirtualMission.push_back( vecIter->first );

    for( int i = 0 ; i < (int)vecCharMissionList.size() ; ++i )
    {
        for( int j = 0 ; j < (int)m_mapUserMission[vecCharMissionList[i]].size() ; ++j )
        {
            // 미션 수행 가능레벨 체크( -1이면 제한없음 )
            const SMissionInfo* MainMission = SiKGCMissionManager()->GetMissionInfo( vecCharMissionList[i] );
            if( pPlayer == NULL || MainMission == NULL )
                continue;

            // 이미 Virtual에 등록된 미션은 제외
            if( vecCheckVirtualMission.end() != find(vecCheckVirtualMission.begin(), vecCheckVirtualMission.end(), m_mapUserMission[vecCharMissionList[i]][j].first) )
                continue;

            // 내 캐릭터 레벨 조건검사
            int iLimitMin = MainMission->m_pairLevelLimit.first;
            int iLimitMax = MainMission->m_pairLevelLimit.second;
            if( 0 > MainMission->m_pairLevelLimit.first ) iLimitMin = 0;
            if( 0 > MainMission->m_pairLevelLimit.second ) iLimitMax = pPlayer->GetCurrentChar().iLevel;
            if( iLimitMin > pPlayer->GetCurrentChar().iLevel || pPlayer->GetCurrentChar().iLevel > iLimitMax )
                continue;

            m_vecVirtualCount.push_back( m_mapUserMission[vecCharMissionList[i]][j] );
            m_vecVirtualCountMainMissionID.push_back( vecCharMissionList[i] );
        }
    }
}

void KGCUserMission::VirtualKillMonster( int iMonsterID, int iMonLevel, int eNPCType )
{
    // NPC몬스터는 미션처리를 하지않는다.
    if( eNPCType )
        return;

    // 미션처리하면 안되는 몬스터이다.
    if( 0 < iMonsterID && iMonsterID < static_cast<int>(g_kMonsterManager.m_vecMonsterTable.size()) )
    {
        const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ iMonsterID ];
        if( kMonsterTemplate.bNotMissionCount )
            return;
    }

	for( int i = 0 ; i < (int)m_vecVirtualCount.size() ; ++i )
	{
		//서브미션의 목표 카운트보다 작은지 큰지 계산합니다.
		const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission( m_vecVirtualCount[i].first );

		if( sSubMissionInfo == NULL )
			continue;

		if( sSubMissionInfo->m_sAction.m_bMonDrop == true )
			continue;

		if( m_vecVirtualCount[i].second >= sSubMissionInfo->m_sCompletion.m_iCount )
			continue;

		//몬스터를 사냥하는 서브미션의 조건에 맞는지 체크합니다.
		SConditionInfo sConditionInfo;
		sConditionInfo.m_iMonsterID = iMonsterID;
		sConditionInfo.m_bWin = false;
		sConditionInfo.m_iGameMode = SiKGCRoomManager()->GetGameMode();
        sConditionInfo.m_pairDifficultRange.first = SiKGCRoomManager()->GetDungeonLevel();
		if( SiKGCMissionManager()->CheckSubMissionCondition( m_vecVirtualCount[i].first, sConditionInfo, iMonLevel ) == false )
			continue;		//조건이 안맞으니 다음~

		m_vecVirtualCount[i].second += sSubMissionInfo->m_sAction.m_iCount;

		//메시지를 뿌려 줘야 겠군요.
        g_pkGameOverlayUI->PushMissionProcessStr( g_pkStrLoader->GetReplacedString( STR_ID_MISSION_MESSAGE1, "lii", SiKGCMissionManager()->GetString( sSubMissionInfo->m_iTitleID ), m_vecVirtualCount[i].second, sSubMissionInfo->m_sCompletion.m_iCount ) );
	}
}

void KGCUserMission::VirtualGetMissionItem( DWORD dwItemID )
{
	for( int i = 0 ; i < (int)m_vecVirtualCount.size() ; ++i )
	{
		//서브미션의 목표 카운트보다 작은지 큰지 계산합니다.
		const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission( m_vecVirtualCount[i].first );

		if( sSubMissionInfo == NULL )
			continue;

		if( m_vecVirtualCount[i].second >= sSubMissionInfo->m_sCompletion.m_iCount )
			continue;

		if( sSubMissionInfo->m_sCompletion.m_dwItemID != dwItemID * 10 )
			continue;

		int iBeforeCount = m_vecVirtualCount[i].second;
		m_vecVirtualCount[i].second += sSubMissionInfo->m_sAction.m_iCount;
		int iAfterCount = m_vecVirtualCount[i].second;

		//메시지를 뿌려 줘야 겠군요.
        g_pkGameOverlayUI->PushMissionProcessStr( g_pkStrLoader->GetReplacedString( STR_ID_MISSION_MESSAGE1, "lii", SiKGCMissionManager()->GetString( sSubMissionInfo->m_iTitleID ), m_vecVirtualCount[i].second, sSubMissionInfo->m_sCompletion.m_iCount ) );
	}
}

void KGCUserMission::GetUnCompleteMission( std::vector<DWORD>& vecMission )
{
    // 완료 안된 미션
    vecMission.clear();
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        if( IsCompleteMission( mit->first ) == false )
        {
            vecMission.push_back( mit->first );
        }
    }
}

// Before 중에 After에 없는게 이번에 클리어된 미션이다
std::vector<DWORD> KGCUserMission::GetCompleteMission( const std::vector<DWORD>& vecBefore, const std::vector<DWORD>& vecAfter )
{
    std::vector< DWORD > vecComplete;

    for( int i = 0; i < (int)vecBefore.size(); ++i )
    {
        std::vector<DWORD>::const_iterator vit;
        vit = std::find( vecAfter.begin(), vecAfter.end(), vecBefore[i] );

        if( vit == vecAfter.end() )
        {
            vecComplete.push_back( vecBefore[i] );
        }
    }

    return vecComplete;
}

void KGCUserMission::SetAutoMission( const std::vector<KMissionData>& kMission )
{
	m_vecAutoMission = kMission;
}

void KGCUserMission::ShowAutoMission()
{
	switch( g_MyD3D->m_pStateMachine->GetState() )
	{
	case GS_GUILD:
	case GS_SHOP_CASH:
    case GS_SHOP_GP:
	case GS_MY_INFO:
	case GS_OPTION:
	case GS_MY_INFO_FROM_ROOM:
	case GS_WORLDMAP:
        if ( g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL_DUNGEON_OUT && (!m_vecAutoMission.empty() || m_bShowMissionUI) ) {
            m_vecAutoMission.clear();
            m_bShowMissionUI = false;
            return;
        }
        break;
	case GS_MATCH:
		break;
	case GS_ROOM:
        if (  SiKGCChannelManager()->CheckChannel( CT_AUTOMATCH ) == true )
            return;
        else if ( g_pkGameOverlayUI->m_pkAddtionGuide->GetGuideUserState() == STATE_PROGRESS_IN_TUTORIAL_DUNGEON_OUT && !m_vecAutoMission.empty() ) {
            m_vecAutoMission.clear();
            m_bShowMissionUI = false;
			return;
        }
		break;
	case GS_INIT_LOADING:
	case GS_LOGO:
	case GS_LOGIN:
	case GS_INTRO:
	case GS_SERVER:
	case GS_CHAR_SELECT:
	case GS_CHAR_GAIN:
	case GS_TUTORIAL:
	case GS_GAME:
	case GS_GAME_LOADING:
	case GS_GAME_END:
	case GS_REPLAY:
	default:
		return;
	}

    // 가이드 때문에 미션UI를 못 쓰는 상황이라면 미션리스트를 비워서 자동으로 못 뜨게 함
    if( g_MyD3D->m_kGuideMgr.CanUseMsgBox() == false )
    {
        m_vecAutoMission.clear();
        m_bShowMissionUI = false;
    }

	if( !m_vecAutoMission.empty() || m_bShowMissionUI )
	{
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_MISSION_UI_S4_DLG );
        m_vecAutoMission.clear();
        m_bShowMissionUI = false;
	}
}

bool KGCUserMission::IsNewMission(int iCharType)
{
    std::map< DWORD, bool >::iterator it = m_mapUserNewMission.begin();

    for(;it != m_mapUserNewMission.end();it++)
    {
        if( ((MissionCharacter( it->first ) == -1) || (MissionCharacter( it->first ) == iCharType)) && it->second )
            return true;
    }

    return false;
}

bool KGCUserMission::IsNewMission( DWORD dwMissionID )
{
	if( m_mapUserNewMission.count(dwMissionID) > 0 )
		return m_mapUserNewMission[dwMissionID];

	return false;
}

bool KGCUserMission::IsNewMission( void )
{
	std::map< DWORD, bool >::iterator it = m_mapUserNewMission.begin();

	for(;it != m_mapUserNewMission.end();it++)
	{
		if( it->second )
			return true;
	}

	return false;
}

void KGCUserMission::SetNewMission( DWORD dwMissionID, bool bNew )
{
	if( m_mapUserNewMission.count(dwMissionID) )
		m_mapUserNewMission[dwMissionID] = bNew;
	else
		m_mapUserNewMission.insert( std::make_pair( dwMissionID, bNew ) );
}

void KGCUserMission::SetInsertMission( DWORD dwMissionID, std::vector< std::pair< int, int > > &vList, bool bNewMision/*=true*/ )
{
	if( bNewMision )
	{
		if( 0 == m_mapUserMission.count( dwMissionID ) )
			SetNewMission( dwMissionID, true );
	}

	m_mapUserMission.insert( std::make_pair( dwMissionID, vList ) );
}

void KGCUserMission::SetPackageMission(std::map< int, std::set< int> >& _mapPackageMission )
{
	std::map< int, std::set< int> >::iterator mit = _mapPackageMission.begin();

	for(; mit != _mapPackageMission.end(); ++mit)
	{
		m_setMissionPackageItem.insert( mit->first / 10 );
		m_setPackageMission.insert( mit->second.begin(), mit->second.end() );
	}
}

bool KGCUserMission::IsPackageMission( DWORD _dwMissionID )
{
	bool bPackageMission = ( m_setPackageMission.end() != m_setPackageMission.find( (int)_dwMissionID ) );

	return bPackageMission;
}

bool KGCUserMission::IsMissionPackageItem( int _iItemID)
{
	bool bMissionPackageItem = ( m_setMissionPackageItem.end() != m_setMissionPackageItem.find( _iItemID ) );

	return bMissionPackageItem;
}

bool KGCUserMission::IsMissionDungeon( EGCGameMode eGameMode )
{
    PLAYER *pPlayer = &g_MyD3D->m_TempPlayer;
    if( NULL == pPlayer )
        return false;

    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
		//예외 MissionID에 등록되셨으면, 넌 던젼(월드맵)에 표시 안해준다.
		if (std::find(m_vecGuildExceptionMissionID.begin(),m_vecGuildExceptionMissionID.end(),mit->first)!=m_vecGuildExceptionMissionID.end())
			continue;

#if defined(WORLD_MAP_MISSION_ICON)
        const SMissionInfo* MainMission = SiKGCMissionManager()->GetMissionInfo( mit->first );
        if( NULL == MainMission )
            continue;

        // 미션 수행 가능레벨 체크( -1이면 제한없음 )
        int iLimitMin = MainMission->m_pairLevelLimit.first;
        int iLimitMax = MainMission->m_pairLevelLimit.second;
        if( 0 > MainMission->m_pairLevelLimit.first ) iLimitMin = 0;
        if( 0 > MainMission->m_pairLevelLimit.second ) iLimitMax = pPlayer->GetCurrentChar().iLevel;
        if( iLimitMin > pPlayer->GetCurrentChar().iLevel || pPlayer->GetCurrentChar().iLevel > iLimitMax )
            continue;

        // 현재 플레이중인 케릭터와 공용 미션만 표시 
        if( MainMission->m_iCharacter != -1 &&
            MainMission->m_iCharacter != pPlayer->GetCurrentChar().iCharType )
            continue;
#endif

        for( int i = 0 ; i < (int)mit->second.size() ; ++i )
        {
            if( mit->second[i].first == -1 )
                continue; 

            //서브미션의 목표 카운트보다 작은지 큰지 계산합니다.
            const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission( mit->second[i].first );

            if( sSubMissionInfo == NULL )
                continue;

            if( sSubMissionInfo->m_sCompletion.m_iCount <= mit->second[i].second )
                continue;

            const SSubMissionInfo* sInfo = SiKGCMissionManager()->GetSubMission( mit->second[i].first );
            if( sInfo == NULL )
            {
                ASSERT_MBOX( "Not in Mission script : " << mit->first );
                continue;
            }

            for( int j = 0 ; j < (int)sInfo->m_vecConditionID.size() ; ++j )
            {
                const SConditionInfo* sCondition = SiKGCMissionManager()->GetConditionInfo( sInfo->m_vecConditionID[j] );
                if( !sCondition )
                    continue;

                // 레벨이 조건인 미션이다.(던전이나 몬스터는 지정되지 않음)
                if( (sCondition->m_pairMonLevelRange.first >= 0) || (sCondition->m_pairMonLevelRange.second >= 0) )
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

                    if( !g_MyD3D->m_pkQuestGameMgr->IsQuestLevelFromInputLevel( iLowLevel, iHighLevel, eGameMode ) )
                        continue;
                }
                else if( sCondition->m_iGameMode != eGameMode )
                    continue;
                return true;
            }
        }
    }
    return false;
}

bool KGCUserMission::IsMissionMonster( EGCMonster eMonster, int iMonLevel )
{

    for( int i = 0 ; i < (int)m_vecVirtualCount.size() ; ++i )
    {
        //서브미션의 목표 카운트보다 작은지 큰지 계산합니다.
        const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission( m_vecVirtualCount[i].first );

        if( sSubMissionInfo == NULL )
            continue;

        if( sSubMissionInfo->m_sAction.m_bMonDrop == true )
            continue;

        if( m_vecVirtualCount[i].second >= sSubMissionInfo->m_sCompletion.m_iCount )
            continue;

        //몬스터를 사냥하는 서브미션의 조건에 맞는지 체크합니다.
        SConditionInfo sConditionInfo;
        sConditionInfo.m_iMonsterID = eMonster;
        sConditionInfo.m_bWin = false;
        sConditionInfo.m_iGameMode = SiKGCRoomManager()->GetGameMode();
        sConditionInfo.m_pairDifficultRange.first = SiKGCRoomManager()->GetDungeonLevel();
        if( SiKGCMissionManager()->CheckSubMissionCondition( m_vecVirtualCount[i].first, sConditionInfo, iMonLevel ) == false )
            continue;		//조건이 안맞으니 다음~

        return true;
    }

    return false;
}

std::vector<DWORD> KGCUserMission::GetDungeonMissionList( EGCGameMode eGameMode, int iDunLevel, bool bComplate )
{
    std::vector<DWORD> vecMissionList;
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;

    PLAYER *pPlayer = &(g_MyD3D->m_TempPlayer);
    if( NULL == pPlayer )
        return vecMissionList;

    if( SiKGCWorldMapManager()->IsInfinityDungeon( eGameMode ) ||
        SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST81 ) { 
        return vecMissionList;
    }

    for( mit = m_mapUserMission.begin() ; mit != m_mapUserMission.end() ; ++mit )
    {
        for( int i = 0 ; i < (int)mit->second.size() ; ++i )
        {
            if( mit->second[i].first == -1 )
                continue; 

            // 미션 수행 가능레벨 체크( -1이면 제한없음 )
            const SMissionInfo* MainMission = SiKGCMissionManager()->GetMissionInfo( mit->first );
            if( pPlayer == NULL || MainMission == NULL )
                continue;

            int iLimitMin = MainMission->m_pairLevelLimit.first;
            int iLimitMax = MainMission->m_pairLevelLimit.second;
            if( 0 > MainMission->m_pairLevelLimit.first ) iLimitMin = 0;
            if( 0 > MainMission->m_pairLevelLimit.second ) iLimitMax = pPlayer->GetCurrentChar().iLevel;
            if( iLimitMin > pPlayer->GetCurrentChar().iLevel || pPlayer->GetCurrentChar().iLevel > iLimitMax )
                continue;

            //서브미션의 목표 카운트보다 작은지 큰지 계산합니다.
            const SSubMissionInfo* sSubMissionInfo = SiKGCMissionManager()->GetSubMission( mit->second[i].first );

            if( sSubMissionInfo == NULL )
                continue;

            //  조건을 충족한것은 체크하지 않겠다.
            if( bComplate && sSubMissionInfo->m_sCompletion.m_iCount <= mit->second[i].second )
                continue;

            const SSubMissionInfo* sInfo = SiKGCMissionManager()->GetSubMission( mit->second[i].first );
            if( sInfo == NULL )
            {
                ASSERT_MBOX( "Not in Mission script : " << mit->first );
                continue;
            }

            bool bFind = false;
            for( int j = 0 ; j < (int)sInfo->m_vecConditionID.size() ; ++j )
            {
                const SConditionInfo* sCondition = SiKGCMissionManager()->GetConditionInfo( sInfo->m_vecConditionID[j] );
                if( NULL == sCondition )
                    continue;

                // 레벨이 조건인 미션이다.(던전이나 몬스터는 지정되지 않음)
                if( (sCondition->m_pairMonLevelRange.first >= 0) || (sCondition->m_pairMonLevelRange.second >= 0) )
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

                    if( !g_MyD3D->m_pkQuestGameMgr->IsQuestLevelFromInputLevel( iLowLevel, iHighLevel, eGameMode, iDunLevel ) )
                        continue;
                }
                else if( sCondition->m_iGameMode != eGameMode )
                    continue;

                bFind = true;
                break;
            }

            if( bFind )
            {
                vecMissionList.push_back( mit->first );
                break;
            }
        }
    }
    return vecMissionList;
}

DWORD KGCUserMission::GetVirtualCountMainMissionID( int index )
{
    if ( index < (int)m_vecVirtualCountMainMissionID.size() )
    {
        return m_vecVirtualCountMainMissionID[index];
    }
    else
        return -1;
}
