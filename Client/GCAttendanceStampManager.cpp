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
    // �̺�Ʈ ������ ���� ���� ����
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
    // �̺�Ʈ �Ⱓ�� �Ѿ�� ������ �����
    if( nIndex > GetDayCount() )
        return ASS_LOCK;

    // �̺�Ʈ �Ⱓ�̶��
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
    // ���� ȹ������ ����
    m_kUserAccumulateData.m_vecRewardCompleteDay.swap( kRecv_.m_vecRewardCompleteDay );

    // ������ ����
    g_pItemMgr->m_kInventory.AddItemList( kRecv_.m_vecRewardItem, true );

    // ���� ������ �κ��丮 ����
    for( std::vector<KItem>::iterator it = kRecv_.m_vecRewardItem.begin(); it != kRecv_.m_vecRewardItem.end(); ++it ) {
        GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( (*it).m_ItemID );

        // �̼��� �ٷ� ����ؾ� �Ѵٸ� ���
        if( m_bUseImmediatelyRegisterMission && pItemInfo->eItemKind == GIK_MISSION_SCROLL ){ 
            RegisterMission( (*it) );
        } else {
            // ȹ�� UI
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPETITION_EVENT_BOX,
                L"", g_pkStrLoader->GetString( STR_ID_MISSION_DLG3 ),
                KGCUIScene::GC_MBOX_USE_NORMAL, (*it).m_ItemID * 10, (*it).m_ItemUID, false, true );
        }
    }

    // �κ��丮 ����
    if( g_MyD3D->m_pStateMachine->IsStateMyInfo() )
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
    
    // UI ���� ����
    g_pkUIScene->m_pkAttendanceStampDlg->UpdateSlotInfo();
}

void GCAttendanceStampManager::RegisterMission( IN KItem& kItem )
{
    GCITEMID dwMissionID = kItem.m_ItemID * 10;
    GCITEMUID dwMissionUID = kItem.m_ItemUID;

    // �̼� ��ũ��Ʈ�� �̼� ������ ���� �ϴ��� üũ 
    if( SiKGCMissionManager()->IsMissionExist( dwMissionID ) == false )
        return;

    // �̼��� ����Ϸ��� �ɸ��Ͱ� �����ϴ��� üũ
    if( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter != -1 &&
       g_kGlobalValue.m_kUserInfo.IsCharExist( SiKGCMissionManager()->GetMissionInfo( dwMissionID )->m_iCharacter ) == false )
        return;

    // ���� ������ �̼� ��� ������ ������ ���߾����� üũ
    int iAvailableCount = 0;
    for( int i = 0 ; i < (int)g_kGlobalValue.m_kUserInfo.vecCharInfo.size() ; ++i )
    {
        SCharInfo& sInfo = g_kGlobalValue.m_kUserInfo.vecCharInfo[i];
        if( SiKGCMissionManager()->IsAvailableRegister( dwMissionID, sInfo ) == KGCMissionManager::GC_MM_OK )
            ++iAvailableCount;
    }

    if( iAvailableCount == 0 )
        return;

    //���� �̼��� �ΰ��� �ߺ��ؼ� ������ �� ����
    if( g_kGlobalValue.m_kUserMission.IsUserMission( dwMissionID ) == true ||
       g_kGlobalValue.m_kUserMission.IsInvalidMission( dwMissionID ) != 0 )
    {
        return;
    }

    Result_Kid_Gift = INT_MAX;
    KP2P::GetInstance()->Send_RegistMission( dwMissionID, dwMissionUID );
    g_MyD3D->WaitForServerAck( Result_Kid_Gift, INT_MAX, 5000, TIME_OUT_VALUE );

    std::wstring str;
    // ������ Procedure���� �ۼ��ϰ���
    switch( Result_Kid_Gift )
    {
    case 0:  // ���� 
        //ȭ�鿡 ������Ʈ ����
        g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR0);
        break;
        break;
    case 2:  // ���� ��ϵ� �̼���
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR6 );
        break;
    case 3:  // �̹� �Ϸ��� �̼���
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR7 );
        break;
    case 4:  // �̼� ������ ��á��
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR8 );
        break;
    case 5:  // ��ϵ� �̼ǰ� ������ �̼��̹Ƿ� ��ϺҰ�.
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR15);
        break;
    case 8:  // ĳ���Ͱ� �������� ����
        str = g_pkStrLoader->GetString( STR_ID_REGISTER_MISSION_ERROR2 );
        break;
    case 9:  // ���� ���°� ���� ����.
        {
#if defined( NATION_USA )
            int nPromotion = SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMissionInfo()->m_iPromotion + 1;
#else 
            int nPromotion = SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMissionInfo()->m_iPromotion;
#endif 
            str = g_pkStrLoader->GetReplacedString( STR_ID_REGISTER_MISSION_ERROR16, "i", nPromotion );
        }
        break;
    case 10: // ĳ���� ���� ������ �������� ����
        str = g_pkStrLoader->GetReplacedString(STR_ID_REGISTER_MISSION_ERROR12,"i",SiKGCMissionManager()->GetMissionWithID( dwMissionID )->GetMinLevel() );
        break;
    case 11: // �����ϴ� �̼��� �ƴ�.
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR5);
        break;
    case 14: // �������� �����ϰ� ���� ����.
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
    case 7:  // �̼� ���� �������� ����.
        str = g_pkStrLoader->GetString(STR_ID_REGISTER_MISSION_ERROR4);
        break;
    case 1:  // ������ �������� ����
    case 6:  // �̼� �������� �ƴѵ� �̼����� ����Ϸ��� ����
    case 12: // Count SubMission ����� ���� ������ ����.
    case 13: // ��Ͻ� DB ����.
    case -99: // �˼����� ����
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
