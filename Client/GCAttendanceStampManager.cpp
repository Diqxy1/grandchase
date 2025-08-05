#include "stdafx.h"
#include "GCAttendanceStampManager.h"
#include "GCStateMachine.h"

ImplementSingleton(GCAttendanceStampManager);

GCAttendanceStampManager::GCAttendanceStampManager(void)
: m_iUiType(0)
{

}

GCAttendanceStampManager::~GCAttendanceStampManager(void)
{

}

void GCAttendanceStampManager::SetEventInfo( KEVENT_ACCUMULATE_ATTENDANCE_INFO_ACK& kRecv_ )
{
    // 이벤트 정보와 현재 나의 상태
    m_bUseImmediatelyRegisterMission = kRecv_.m_bUseImmediatelyRegisterMission;
    m_mapCumulativeAttendanceRewardList = kRecv_.m_mapCumulativeAttendanceRewardList;
    m_kUserAccumulateData = kRecv_.m_kUserAccumulateData;
}

int GCAttendanceStampManager::GetDayCount()
{
    return static_cast<int>( m_mapCumulativeAttendanceRewardList.size() );
}

bool GCAttendanceStampManager::GetSlotReward( int nIndex, VEC_REWARD& vecReward )
{
    std::map<int, VEC_REWARD>::iterator itReward = m_mapCumulativeAttendanceRewardList.find( nIndex );

    if( itReward != m_mapCumulativeAttendanceRewardList.end() ){
        vecReward = (*itReward).second;
        return true;
    } else {
        return false;
    }
}

AttendanceSlotState GCAttendanceStampManager::GetSlotState( int nIndex )
{
    // 이벤트 기간을 넘어가는 슬롯은 잠그자
    if( nIndex > GetDayCount() )
        return ASS_LOCK;

    // 이벤트 기간이라면
    if( nIndex <= m_kUserAccumulateData.m_nAccumulateCount )
    {
        std::vector<int>::iterator vit = std::find( m_kUserAccumulateData.m_vecRewardCompleteDay.begin(), m_kUserAccumulateData.m_vecRewardCompleteDay.end(), nIndex );
        if( vit != m_kUserAccumulateData.m_vecRewardCompleteDay.end() )
            return ASS_STAMPED;
        else
            return ASS_CLAIMABLE;
    } else {
        return ASS_OPEN;
    }
}

void GCAttendanceStampManager::OnEventAccumulateAttendanceRewardAck( IN KEVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK& kRecv_ )
{
    // 보상 획득정보 갱신
    m_kUserAccumulateData.m_vecRewardCompleteDay.swap( kRecv_.m_vecRewardCompleteDay );

    // 아이템 지급
    g_pItemMgr->m_kInventory.AddItemList( kRecv_.m_vecRewardItem, true );

    // 보상 아이템 인벤토리 갱신
    for( std::vector<KItem>::iterator it = kRecv_.m_vecRewardItem.begin(); it != kRecv_.m_vecRewardItem.end(); ++it ) {
        GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( (*it).m_ItemID );

        // 미션을 바로 등록해야 한다면 등록
        if( m_bUseImmediatelyRegisterMission && pItemInfo->eItemKind == GIK_MISSION_SCROLL ){ 
            RegisterMission( (*it) );
        } else {
            // 획득 UI
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
                L"", g_pkStrLoader->GetString( STR_ID_MISSION_DLG3 ),
                KGCUIScene::GC_MBOX_USE_NORMAL, (*it).m_ItemID * 10, (*it).m_ItemUID, false, true );
        }
    }

    // 인벤토리 갱신
    if( g_MyD3D->m_pStateMachine->IsStateMyInfo() )
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    
    // UI 슬롯 갱신
    g_pkUIScene->m_pkAttendanceStampDlg->UpdateSlotInfo();
}

void GCAttendanceStampManager::RegisterMission( IN KItem& kItem )
{
    GCITEMID dwMissionID = kItem.m_ItemID * 10;
    GCITEMUID dwMissionUID = kItem.m_ItemUID;

    // 미션 스크립트에 미션 내용이 존재 하는지 체크 
    if( SiKGCMissionManager()->IsMissionExist( dwMissionID ) == false )
        return;

    // 미션을 등록하려는 케릭터가 존재하는지 체크
    if( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter != -1 &&
       g_kGlobalValue.m_kUserInfo.IsCharExist( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter ) == false )
        return;

    // 현재 유저가 미션 등록 가능한 조건을 갖추었는지 체크
    int iAvailableCount = 0;
    for( int i = 0 ; i < (int)g_kGlobalValue.m_kUserInfo.vecCharInfo.size() ; ++i )
    {
        SCharInfo& sInfo = g_kGlobalValue.m_kUserInfo.vecCharInfo[i];
        if( SiKGCMissionManager()->IsAvailableRegister( dwMissionID, sInfo ) == KGCMissionManager::GC_MM_OK )
            ++iAvailableCount;
    }

    if( iAvailableCount == 0 )
        return;

    //같은 미션은 두개를 중복해서 진행할 수 없음
    if( g_kGlobalValue.m_kUserMission.IsUserMission( dwMissionID ) == true ||
       g_kGlobalValue.m_kUserMission.IsInvalidMission( dwMissionID ) != 0 )
    {
        return;
    }

    Result_Kid_Gift = INT_MAX;
    KP2P::GetInstance()->Send_RegistMission( dwMissionID, dwMissionUID );
    g_MyD3D->WaitForServerAck( Result_Kid_Gift, INT_MAX, 5000, TIME_OUT_VALUE );

    std::wstring str;
    // 성공은 Procedure에서 작성하겠음
    switch( Result_Kid_Gift )
    {
    case 0:  // 성공 
        //화면에 업데이트 해줌
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR0);
        break;
        break;
    case 2:  // 현재 등록된 미션임
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR6 );
        break;
    case 3:  // 이미 완료한 미션임
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR7 );
        break;
    case 4:  // 미션 슬롯이 꽉찼음
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR8 );
        break;
    case 5:  // 등록된 미션과 연관된 미션이므로 등록불가.
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR15);
        break;
    case 8:  // 캐릭터가 존재하지 않음
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR2 );
        break;
    case 9:  // 전직 상태가 맞지 않음.
        {
#if defined( NATION_USA )
            int nPromotion = SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMissionInfo()->m_iPromotion + 1;
#else 
            int nPromotion = SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMissionInfo()->m_iPromotion;
#endif 
            str = g_pkStrLoader->GetReplacedString( STR_ID_REGISTER_MISSION_ERROR16, "i", nPromotion );
        }
        break;
    case 10: // 캐릭터 레벨 제한을 만족하지 못함
        str = g_pkStrLoader->GetReplacedString(STR_ID_REGISTER_MISSION_ERROR12,"i",SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMinLevel() );
        break;
    case 11: // 존재하는 미션이 아님.
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR5);
        break;
    case 14: // 아이템을 보유하고 있지 않음.
        str = g_pkStrLoader->GetReplacedString( STR_ID_REGISTER_MISSION_ERROR11, "i", Result_Kid_Gift );
        g_pItemMgr->m_kInventory.RemoveItem( dwMissionUID );
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
        break;
    case 15:
        str = g_pkStrLoader->GetString( STR_ID_ALREADY_ACQUIRED_CHARACTER );
        break;
    case 16:
        str = g_pkStrLoader->GetString( STR_ID_BUY_CERT_FAILED_JOBLEVEL );
        break;
    case 7:  // 미션 정보 가져오기 실패.
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR4);
        break;
    case 1:  // 유저가 존재하지 않음
    case 6:  // 미션 아이템이 아닌데 미션으로 등록하려고 했음
    case 12: // Count SubMission 목록을 가져 오는중 실패.
    case 13: // 등록시 DB 오류.
    case -99: // 알수없는 오류
        str = g_pkStrLoader->GetReplacedString( STR_ID_REGISTER_MISSION_ERROR11, "i", Result_Kid_Gift );
        break;
    case 200://TimeOut
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR3 );
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );
        break;
    }
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, str );
}

void GCAttendanceStampManager::SetUIType( int UiType )
{
    m_iUiType = UiType;
}

int GCAttendanceStampManager::GetUIType()
{
    return m_iUiType;
}
