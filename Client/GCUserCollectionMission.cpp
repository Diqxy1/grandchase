#include "stdafx.h"
#include ".\GCUserCollectionMission.h"
#include "mission/GCCTMissionManager.h"


//

//
//


//

#include "gcui/KGCTitleWindow.h"


KGCUserCollectionMission::KGCUserCollectionMission(void)
: m_dwDeleteReadyMissionID( 0 )
{
    m_vecCompleteThisTime.clear();
}

KGCUserCollectionMission::~KGCUserCollectionMission(void)
{
    ClearUserCollectionMission();
}

void KGCUserCollectionMission::SetUserCollectionMissionList( const std::vector<KCollectionData>& kList )
{
    std::vector< DWORD > vecBefore;
    GetUnCompleteCollectionMission( vecBefore );

    std::vector<KCollectionData>::const_iterator vit;
    for( vit = kList.begin(); vit != kList.end() ; ++vit )
    {
        SetUserCollectionMission( (*vit) );

        // 2번이면 완료. 1번이면 진행중
        if( 2 == vit->m_dwState )
            m_setCompleteMission.insert( vit->m_dwMissionID );
    }
	RefreshUserCollectionMission();

    std::vector< DWORD > vecAfter;
    GetUnCompleteCollectionMission( vecAfter );

    m_vecCompleteThisTime = GetCompleteCollectionMission( vecBefore, vecAfter );    
}

void KGCUserCollectionMission::SetUserCollectionMission( const KCollectionData& kData )
{
    typedef std::map< DWORD, std::vector< std::pair< int, int > > > UserCollectionMissionList;
    typedef std::vector< std::pair< int, int > > SubCollectionMissionList;

    const SMissionInfo *sMissionInfo = SiKGCCTMissionManager()->GetMissionInfo( kData.m_dwMissionID );
    
	if( sMissionInfo == NULL  )
		return;

    SubCollectionMissionList vecCompletion;
    for( int i = 0 ; i < (int)sMissionInfo->m_vecSubMissions.size() ; ++i )
    {
        const SSubMissionInfo* sSubMission = SiKGCCTMissionManager()->GetSubMission( sMissionInfo->m_vecSubMissions[i] );

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
        vecCompletion.push_back( SubCollectionMissionList::value_type(iSubMissionID, iCount ) );
    }
    if ( false == m_mapUserCollectionMission.insert( UserCollectionMissionList::value_type( kData.m_dwMissionID, vecCompletion ) ).second )
    {
        std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit = m_mapUserCollectionMission.find( kData.m_dwMissionID );
        if ( mit == m_mapUserCollectionMission.end() )
        {
            m_mapUserCollectionMission.insert( UserCollectionMissionList::value_type( kData.m_dwMissionID, vecCompletion ));
        }
        else
        {
            m_mapUserCollectionMission[kData.m_dwMissionID] = vecCompletion;
        }

    }

    if( m_mapUserCollectionMission.count( kData.m_dwMissionID ) == 0 )
    {
        ASSERT( !"이건 있을 수 없다. 방금 넣은 미션이 없다니!!" );
        return;
    }

    SubCollectionMissionList& vecUpdateCompletion = m_mapUserCollectionMission[kData.m_dwMissionID];
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

void KGCUserCollectionMission::RefreshUserCollectionMission()  //유저의 미션 수행 상황을 다시 알아봅니다.
{
    RefreshMissionItemCount();
}

void KGCUserCollectionMission::RefreshMissionItemCount()
{
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserCollectionMission.begin() ; mit != m_mapUserCollectionMission.end() ; ++mit )
    {
        for( int i = 0 ; i < (int)mit->second.size() ; ++i )
        {
            const SSubMissionInfo* sInfo = SiKGCCTMissionManager()->GetSubMission( mit->second[i].first );
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

void KGCUserCollectionMission::RefreshMissionPeriod()
{

}

int KGCUserCollectionMission::CompleteCollectionMission( DWORD dwMissionID )    //수행 중인 미션을 완료 합니다. (완료가 가능할 시 )
{
    if( IsUserCollectionMission( dwMissionID ) == false )
        return UM_MISSION_NOT_EXIST;

    int iResult = SiKGCCTMissionManager()->CheckCompleteMission( dwMissionID, m_mapUserCollectionMission[dwMissionID] );
    
    switch( iResult )
    {
    case KGCCTMissionManager::GC_MM_OK:
		DeleteCollectionMission( dwMissionID );

        if( !IsCompleteCTMission( dwMissionID ) )
            m_setCompleteMission.insert( dwMissionID );

        return UM_OK;
    case KGCCTMissionManager::GC_MM_NOT_ENOUGH:
        return UM_NOT_ENOUGH;
    case KGCCTMissionManager::GC_MM_MISSION_NOT_EXIST:
        ASSERT_MBOX( "Mission is Not Exist : "<< dwMissionID );
        return UM_NOT_ENOUGH;
    case KGCCTMissionManager::GC_MM_SUBMISSION_NOT_EXIST:
        ASSERT_MBOX( "SubMission is Not Exist : "<< dwMissionID );
        return UM_NOT_ENOUGH;
    case KGCCTMissionManager::GC_MM_CONDITION_NOT_EXIST:
        ASSERT_MBOX( "Condition is Not Exist : "<< dwMissionID );
        return UM_NOT_ENOUGH;
    default:
        return UM_NOT_ENOUGH;
    }
    return UM_NOT_ENOUGH;   //혹시나..
}

int KGCUserCollectionMission::DeleteCollectionMission( DWORD dwMissionID )    //유저가 미션을 삭제 합니다.
{
    if( IsUserCollectionMission( dwMissionID ) == false )
        return UM_MISSION_NOT_EXIST;

    m_mapUserCollectionMission.erase( dwMissionID );

    return UM_OK;
}

int KGCUserCollectionMission::AddCollectionMission( DWORD dwMissionID )       //미션을 등록합니다.
{
    if( IsUserCollectionMission( dwMissionID ) == true )
        return UM_MISSION_ALREADY_EXIST;

    const SMissionInfo* psInfo = SiKGCCTMissionManager()->GetMissionInfo( dwMissionID );

    if( psInfo == NULL )
        return UM_MISSION_NOT_EXIST;

    std::vector< std::pair< int, int > > vecSubMission;
    for( int i = 0 ; i < (int)psInfo->m_vecSubMissions.size() ; ++i )
    {
        vecSubMission.push_back( std::make_pair( psInfo->m_vecSubMissions[i], 0 ) );
    }
    m_mapUserCollectionMission.insert( std::make_pair( dwMissionID, vecSubMission ) );

    RefreshUserCollectionMission();

    return UM_OK;
}

bool KGCUserCollectionMission::IsUserCollectionMission( DWORD dwMissionID )
{
    if( m_mapUserCollectionMission.count( dwMissionID ) == 0 )
        return false;
    return true;
}

int KGCUserCollectionMission::MissionCharacter( DWORD dwMissionID )
{
    return SiKGCCTMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter;
}

bool KGCUserCollectionMission::GetCharMission( int iCharKind , std::vector< DWORD >& vecList ) //해당 케릭터의 미션 리스트를 받아옵니다.
{
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserCollectionMission.begin() ; mit != m_mapUserCollectionMission.end() ; ++mit )
    {
        const SMissionInfo* sInfo = SiKGCCTMissionManager()->GetMissionInfo( mit->first );

        if( sInfo == NULL )
        {
            ASSERT_MBOX( "Not in Mission script : " << mit->first );
            continue;
        }

        if( sInfo->m_iCharacter == iCharKind )
        {
            vecList.push_back( mit->first );
        }
    }

    return true;
}

void KGCUserCollectionMission::ClearUserCollectionMission()
{
    m_setCompleteMission.clear();
    m_mapUserCollectionMission.clear();
}

DWORD KGCUserCollectionMission::IsInvalidCollectionMission( DWORD dwMissionID )
{
    std::vector< DWORD > vecInvalidList;
    if( GetInvalidCollectionMissionList( dwMissionID , vecInvalidList ) == false )
        return 0;

    for( int i = 0 ; i < (int)vecInvalidList.size() ; ++i )
    {
        if( IsUserCollectionMission( vecInvalidList[i] ) == true )
            return vecInvalidList[i];
    }

    return 0;
}

bool KGCUserCollectionMission::GetInvalidCollectionMissionList( DWORD dwMissionID , std::vector< DWORD >& vecList )
{
    if( IsUserCollectionMission( dwMissionID ) == false )
        return false;

    const SMissionInfo* sInfo = SiKGCCTMissionManager()->GetMissionInfo( dwMissionID );
    vecList.insert( vecList.end(), sInfo->m_vecInvalidMissions.begin(), sInfo->m_vecInvalidMissions.end() );
    return true;
}

int KGCUserCollectionMission::GetCompleteMissionNum()
{
    int iCompleteCount = 0;
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserCollectionMission.begin() ; mit != m_mapUserCollectionMission.end() ; ++mit )
    {
        if( IsCompleteCollectionMission( mit->first ) == true )
            ++iCompleteCount;
    }

    return iCompleteCount;
}

bool KGCUserCollectionMission::IsCompleteCollectionMission( DWORD dwMissionID )
{
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    mit = m_mapUserCollectionMission.find( dwMissionID );

    if( mit == m_mapUserCollectionMission.end() )
        return false;

	if( SiKGCCTMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter != -1 &&
		!g_kGlobalValue.m_kUserInfo.IsCharExist( SiKGCCTMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter ) )
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

bool KGCUserCollectionMission::IsCompleteSubMission( int iSubMissionID, int iCount )
{
    const SSubMissionInfo* sInfo = SiKGCCTMissionManager()->GetSubMission( iSubMissionID );
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

std::vector< DWORD > KGCUserCollectionMission::GetCharacterCollectionMission( int iCharKind )  // 해당 케릭터가 수행할 수 있는 미션 리스트를 받습니다.
{
    std::vector< DWORD > vecList;
    vecList.reserve( 15 );

    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserCollectionMission.begin() ; mit != m_mapUserCollectionMission.end() ; ++mit )
    {
        if( MissionCharacter( mit->first ) == iCharKind )
            vecList.push_back( mit->first );
    }

    return vecList;
}

int KGCUserCollectionMission::GetCharMissionNum( int iCharKind )
{
    std::vector<DWORD> vecCharMission;
    GetCharMission( iCharKind, vecCharMission );
    return (int)vecCharMission.size();
}

int KGCUserCollectionMission::GetCharCompleteMissionNum( int iCharKind )
{
    std::vector<DWORD> vecCharMission;
    GetCharMission( iCharKind , vecCharMission );

    int iCompeteCount = 0;
    for( int i = 0 ; i < (int)vecCharMission.size() ; ++i )
    {
        if( IsCompleteCollectionMission( vecCharMission[i] ) == true )
            ++iCompeteCount;
    }

    return iCompeteCount;
}

bool KGCUserCollectionMission::IsFailedCollectionMission( DWORD dwMissionID )
{
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    mit = m_mapUserCollectionMission.find( dwMissionID );

    if( mit == m_mapUserCollectionMission.end() )
        return false;

    //추후에 조건이 생기면 구현, 아직은 실패해서 더이상 수행할 수 없는 조건이 없다.

    return false;
}

int KGCUserCollectionMission::GetCompletionCount( DWORD dwMissionID, int iSubMissionID )
{
	RefreshMissionItemCount();
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    mit = m_mapUserCollectionMission.find( dwMissionID );

    if( mit == m_mapUserCollectionMission.end() )
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

void KGCUserCollectionMission::GetUnCompleteCollectionMission( std::vector<DWORD>& vecMission )
{
    // 완료 안된 미션
    vecMission.clear();
    std::map< DWORD, std::vector< std::pair< int, int > > >::iterator mit;
    for( mit = m_mapUserCollectionMission.begin() ; mit != m_mapUserCollectionMission.end() ; ++mit )
    {
        if( IsCompleteCollectionMission( mit->first ) == false )
        {
            vecMission.push_back( mit->first );
        }
    }
}

// Before 중에 After에 없는게 이번에 클리어된 미션이다
std::vector<DWORD> KGCUserCollectionMission::GetCompleteCollectionMission( const std::vector<DWORD>& vecBefore, const std::vector<DWORD>& vecAfter )
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

bool KGCUserCollectionMission::IsCompleteCTMission( DWORD dwMissionID )
{
    std::set< DWORD >::iterator itCur = m_setCompleteMission.begin();
    for(;itCur != m_setCompleteMission.end();++itCur)
    {        
        if( dwMissionID == (*itCur) )
            return true;
    }

    return false;
}

int KGCUserCollectionMission::GetNewCompleteTitleID( void )
{
	for(int j=0;j<(int)m_vecCompleteThisTime.size();++j)
	{
		if( false == g_pkUIScene->m_pkTitleCollection->IsCurrentUseMissionID( m_vecCompleteThisTime[j] ) )
			continue;

		// 이놈이 완료된 미션인지 체크하는 부분
		const SMissionInfo *sMissionInfo = SiKGCCTMissionManager()->GetMissionInfo( m_vecCompleteThisTime[j] );

		if( sMissionInfo == NULL )
			continue;

		int iCompleteSubCount = 0;
		for(int i=0;i<(int)sMissionInfo->m_vecSubMissions.size();++i)
		{
			const SSubMissionInfo* sSubInfo = SiKGCCTMissionManager()->GetSubMission( sMissionInfo->m_vecSubMissions[i] );

			const int iCurCount = g_kGlobalValue.m_kUserCollectionMission.GetCompletionCount( sMissionInfo->m_dwID, sMissionInfo->m_vecSubMissions[i] );
			const int iCompleteCount = sSubInfo->m_sCompletion.m_iCount;

			if( iCurCount >= iCompleteCount )
				iCompleteSubCount++;
		}

		// 서브미션이 모두 완료되었으니... 완료된 미션이다.
		if( sMissionInfo->m_vecSubMissions.size() == iCompleteSubCount )
		{
			const SMissionInfo* sMissionInfo = SiKGCCTMissionManager()->GetMissionInfo( m_vecCompleteThisTime[j] );
			if( sMissionInfo )
			{
				for(int i=0;i<(int)sMissionInfo->m_vecRewardItem.size();++i)
				{
					if( g_pkUIScene->m_pkTitleCollection->IsTitleItemID( sMissionInfo->m_vecRewardItem[i].m_ItemID ) )
						return (int)sMissionInfo->m_vecRewardItem[i].m_ItemID;
				}
			}
		}
	}

	return -1;
}
