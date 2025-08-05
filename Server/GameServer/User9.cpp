#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "support_User.h"
#include "DepotManager.h"
#include "LevelResetEvent.h"
#include "ResultManager.h"
#include "MissionManager.h"
#include "MissionInfo.h"
#include "SignBoard.h"
#include "SkillManager.h"
#include "Log4.h"
#include "Room.h"
#include "GachaLottery.h"
#include "CharDefault.h"
#include "OlympicManager.h"
#include "SHCheckPoint.h"
#include "CYOUBilling/CYOUBilling.h"
#include "CnConnector.h"
#include "ItemManager.h" // for gp item attribute,GP아이템속성
#include "boost/bind.hpp" // gp item 속성.
#include "ItemCompose.h"
#include "LoudMessage.h"
#include "GachaLottery.h"
#include "HeroDungeonManager.h"
#include "RitasChristmas.h"
#include "BuffDonation.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include "GameServer.h"
#include "CharSlotManager.h"
#include "SlangFilter.h"
#include "ConnectionGiftBox.h"
#include "JumpingChar.h"
#include "GachaPong.h"

#define CLASS_TYPE  KUser

IMPL_ON_FUNC_NOPARAM( EVENT_DEPOT_INFO_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    
    KEVENT_DEPOT_INFO_ACK kPacket;

    if ( !m_mapDepotInfo.empty() ) {
        SET_ERR_GOTO( ERR_DEPOT_09, END_PROC );
    }

    QUEUEING_ID_TO_DB_CHECK( EVENT_DEPOT_INFO_REQ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, NetError::NET_OK );     // 성공
        ERR_CASE( ERR_DEPOT_09, kPacket.m_nOK, NetError::ERR_DEPOT_09 ); // 창고 정보 반복 요청
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"창고 정보 요청중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( kPacket.m_nOK != NetError::NET_OK ) {
        SiKDepotManager()->GetDepotMaxSizeInfo( kPacket );
        SEND_PACKET( EVENT_DEPOT_INFO_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"창고 기본 정보 불러오기 요청, User : " << GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( m_mapDepotInfo.size() )
        << END_LOG;

}

IMPL_ON_FUNC( EVENT_DEPOT_INFO_ACK )
{ // 서버 접속 후 1회만 호출.
    if ( !kPacket_.m_mapDepotInfo.empty() ) {
        m_mapDepotInfo = kPacket_.m_mapDepotInfo;
    }

    SiKDepotManager()->GetDepotMaxSizeInfo( kPacket_ );
    kPacket_.m_SlotExtendItemID = SiKDepotManager()->GetSlotExtendItemID();
    kPacket_.m_TabExtendItemID = SiKDepotManager()->GetTabExtendItemID();
    
    SEND_RECEIVED_PACKET( EVENT_DEPOT_INFO_ACK );

    GetDepotInfo();

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"창고 기본 정보 불러오기 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( m_mapDepotInfo.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_DEPOT_CHAR_TAB_INFO_REQ, KDepotInfo )
{
    KEVENT_DEPOT_CHAR_TAB_INFO_ACK kPacket;

    std::map< char, KDepotInfo >::iterator mit = m_mapDepotInfo.find( kPacket_.m_cTabID );
    
    if ( mit == m_mapDepotInfo.end() || mit->second.m_bDBSelectItem == false ) {
        QUEUEING_EVENT_TO_DB_CHECK( EVENT_DEPOT_CREATE_REQ, kPacket_ ); // 서버 접속 후  디비 호출은 1회만 일어나야 함.
    }
    else {        
        kPacket.m_mapDepotInfo = m_mapDepotInfo;
        kPacket.m_nOK = 0;
        m_kInventory.FindDepotItemByTab( kPacket_.m_cTabID, kPacket.m_vecItem ) ;

        if ( !m_mapDepotPetInfo.empty() ) {
            kPacket.m_mapPetInfo = m_mapDepotPetInfo;
        }

        SEND_PACKET( EVENT_DEPOT_CHAR_TAB_INFO_ACK );

        START_LOG( clog, L"DB요청 없이 서버 캐싱 창고 정보 응답, User : " << GetName() )
            << BUILD_LOG( kPacket.m_mapDepotInfo.size() )
            << BUILD_LOG( kPacket.m_vecItem.size() )
            << BUILD_LOG( kPacket.m_mapPetInfo.size() )
            << END_LOG;
    }
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, NetError::NET_OK );     // 성공
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"창고 정보 요청중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }
    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_DEPOT_CHAR_TAB_INFO_ACK );
    }
    _LOG_SUCCESS( kPacket.m_nOK == 0, L"창고 캐릭터,탭 해당 정보 불러오기 요청, User : " << GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( m_mapDepotInfo.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_DEPOT_CHAR_TAB_INFO_NOT )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    //kPacket_.m_kDepotInfo.m_bDBSelectItem = true; // DB에서 데이터(아이템) 읽었는지 저장해서 한 번만 읽는 기능 구현
    m_mapDepotInfo = kPacket_.m_mapDepotInfo;

    if ( !kPacket_.m_mapPetInfo.empty() ) { // 캐릭터,탭 별로 가져온 펫 정보를 '창고 펫 정보' 컨테이너에 추가
        std::map< GCITEMUID, KPetInfo >::iterator mitPet;
        for( mitPet = kPacket_.m_mapPetInfo.begin(); mitPet != kPacket_.m_mapPetInfo.end(); ++mitPet ) {
        
            if( m_mapDepotPetInfo.insert( std::make_pair( mitPet->first, mitPet->second ) ).second == false ) {

                START_LOG( cerr, L"중복된 창고 펫 정보 있음 , User : " << GetName() )
                    << BUILD_LOG( mitPet->first )
                    << END_LOG;
            }
        }
    }

    if ( !m_mapDepotPetInfo.empty() ) {
        kPacket_.m_mapPetInfo = m_mapDepotPetInfo;
    }
    
    if( !kPacket_.m_mapStrengthItem.empty() ) {
        UpdateStrengthEquipItemInfo( kPacket_.m_mapStrengthItem, kPacket_.m_vecItem, kPacket_.m_vecInvenItem );
    }

    if ( !kPacket_.m_vecItem.empty() ) {
        // todo : 창고 아이템 UID 로 인벤토리에서 검색해서, delstate 가 255 아닌 아이템은
        // 창고에 넣지 않고, 디비의 창고 테이블에서 삭제하는 기능 구현 필요.
        m_kInventory.AddDepotItemList( kPacket_.m_vecItem );
    }

    SEND_RECEIVED_PACKET( EVENT_DEPOT_CHAR_TAB_INFO_NOT );

   _LOG_SUCCESS( kPacket_.m_nOK == 0, L"창고 캐릭터,탭 해당 정보 불러오기 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( m_mapDepotInfo.size() )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( kPacket_.m_mapPetInfo.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_DEPOT_CREATE_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    //kPacket_.m_kDepotInfo.m_bDBSelectItem = true; // DB에서 데이터(아이템) 읽었는지 저장해서 한 번만 읽는 기능 구현
    m_mapDepotInfo[SiKDepotManager()->GetInitTabID()] = kPacket_.m_mapDepotInfo[SiKDepotManager()->GetInitTabID()];

    //if ( !kPacket_.m_mapPetInfo.empty() ) { // 캐릭터,탭 별로 가져온 펫 정보를 '창고 펫 정보' 컨테이너에 추가
    //    std::map< GCITEMUID, KPetInfo >::iterator mitPet;
    //    for( mitPet = kPacket_.m_mapPetInfo.begin(); mitPet != kPacket_.m_mapPetInfo.end(); ++mitPet ) {

    //        if( m_mapDepotPetInfo.insert( std::make_pair( mitPet->first, mitPet->second ) ).second == false ) {

    //            START_LOG( cerr, L"중복된 창고 펫 정보 있음 , User : " << GetName() )
    //                << BUILD_LOG( mitPet->first )
    //                << END_LOG;
    //        }
    //    }
    //}

    //if ( !m_mapDepotPetInfo.empty() ) {
    //    kPacket_.m_mapPetInfo = m_mapDepotPetInfo;
    //}

    //if( !kPacket_.m_mapStrengthItem.empty() ) {
    //    UpdateStrengthEquipItemInfo( kPacket_.m_mapStrengthItem, kPacket_.m_vecItem, kPacket_.m_vecInvenItem );
    //}

    //if ( !kPacket_.m_vecItem.empty() ) {
    //    m_kInventory.AddDepotItemList( kPacket_.m_vecItem );
    //}

    SEND_RECEIVED_PACKET( EVENT_DEPOT_CHAR_TAB_INFO_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"창고 캐릭터,탭 생성 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( m_mapDepotInfo.size() )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( kPacket_.m_mapPetInfo.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_DEPOT_INSERT_ITEM_REQ, KDepotItem )
{ // 인벤토리에서 창고로 이동
    KItemPtr spItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    KItemPtr spDepotItem = m_kInventory.FindDepotItemByKey( kPacket_.m_kDepotKey );

    std::map< char, KDepotInfo >::iterator mitDepot;
    std::map< GCITEMUID, KPetInfo >::iterator mitPet;

    KEVENT_DEPOT_INSERT_ITEM_ACK kPacket;
    KItem kDepotItem;
    KInvenDepotMoveLog kInvenDepotMoveLog;

    //if ( SiKGSSimLayer()->m_nTestInvenToDepotItemCount != 0 ) {
    //    kPacket_.m_nCount = SiKGSSimLayer()->m_nTestInvenToDepotItemCount;
    //}
    if ( SiKDepotManager()->IsRestrictItem( kPacket_.m_ItemID ) == true ) {
        SET_ERR_GOTO( ERR_DEPOT_15 , END_PROC );
    }

    if ( spItem == NULL ) {
        SET_ERR_GOTO( ERR_DEPOT_00 , END_PROC );
    }

    if ( spItem->m_nPeriod != KItem::UNLIMITED_ITEM ) {
        SET_ERR_GOTO( ERR_DEPOT_04, END_PROC );
    }

    if ( spItem->m_cInvenType == KItem::DEPOT ) { // 창고 타입과 아이템 타입 일치 검사하기
        SET_ERR_GOTO( ERR_DEPOT_01, END_PROC );
    }

    if ( spDepotItem ) {
        if ( spItem->m_ItemID == spDepotItem->m_ItemID && spItem->m_nCount != KItem::UNLIMITED_ITEM && kPacket_.m_nCount > 0 ) {
        }
        else {
            SET_ERR_GOTO( ERR_DEPOT_02, END_PROC );
        }
        
    }

    // 수량 아이템 이동 로그에 대해서만 남긴다.
    if( kPacket_.m_nCount > 0 ) {
        kInvenDepotMoveLog.m_nType = KInvenDepotMoveLog::INVEN_TO_DEPOT;
        kInvenDepotMoveLog.m_dwUID = GetUID();
        kInvenDepotMoveLog.m_cCharType = GetCurrentCharType();
        kInvenDepotMoveLog.m_nRequestCount = kPacket_.m_nCount;

        if( spItem ) {
            kInvenDepotMoveLog.m_ItemIDFrom = spItem->m_ItemID;
            kInvenDepotMoveLog.m_ItemUIDFrom = spItem->m_ItemUID;
            kInvenDepotMoveLog.m_nCountFrom = spItem->m_nCount;
        }

        if( spDepotItem ) {
            kInvenDepotMoveLog.m_ItemIDTo = spDepotItem->m_ItemID;
            kInvenDepotMoveLog.m_ItemUIDTo = spDepotItem->m_ItemUID;
            kInvenDepotMoveLog.m_nCountTo = spDepotItem->m_nCount;
        }
        InsertInvenDepotMoveInfo( kInvenDepotMoveLog );
    }

    mitDepot = m_mapDepotInfo.find( kPacket_.m_kDepotKey.m_cTabID );
    if ( mitDepot == m_mapDepotInfo.end() ) {
        SET_ERR_GOTO( ERR_DEPOT_13, END_PROC );
    }
    if ( mitDepot->second.m_bDBSelectItem == false ) {
        SET_ERR_GOTO( ERR_DEPOT_13, END_PROC );
    }

    // 영구 아이템을 창고에 넣는 경우
    if ( spItem->m_nCount == KItem::UNLIMITED_ITEM && spItem->m_nPeriod == KItem::UNLIMITED_ITEM ) {
        // 모든 조건이 성공이면, 창고 상태로 바꾸고, 컨테이너에 등록
        if( false == m_kInventory.MoveInvenToDepot( *spItem, kPacket_.m_kDepotKey ) ) {
            SET_ERR_GOTO( ERR_DEPOT_03, END_PROC );
        }
    }

    // 수량 아이템을 창고에 넣는 경우
    if ( spItem->m_nCount != KItem::UNLIMITED_ITEM && kPacket_.m_nCount > 0 ) { 

        // 수량을 합치는 경우에 등급이 같으면 합치자. 속성, 강화레벨, 소켓, 코디 아이템 여부는 다른 경우가 없다고 가정한다.
        if( false == m_kInventory.MoveInvenToDepotCount( *spItem, kPacket_, kDepotItem ) ) {
            SET_ERR_GOTO( ERR_DEPOT_03, END_PROC );
        }
    }    

    // 펫 컨테이너 삭제/추가 처리.
    mitPet = m_mapPetInfo.find( kPacket_.m_ItemUID );
    if ( mitPet != m_mapPetInfo.end() ) {
        if( m_mapDepotPetInfo.insert( std::make_pair( mitPet->first, mitPet->second) ).second == true ) {
            DeletePet( mitPet->first );
        }        
    }
    // 인벤토리는 빈 상태로 보내는 경우가 있다. 영구아이템을 옮기는 경우와 수량아이템을 다 옮기는 경우이다.
    // 아래 코드 수정 바람
    kPacket.m_kInvenItem = *spItem;
    
    if ( kDepotItem.m_ItemID != 0 ) { // 수량 아이템이 복사되면서 새 아이템이 생성된 경우
        kPacket.m_kDepotItem = kDepotItem;
    }
    else { // 영구 아이템의 창고 이동 경우 혹은 수량 아이템 전체를 옮기는 경우
        kPacket.m_kDepotItem = *spItem;
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_DEPOT_00,      kPacket.m_nOK, NetError::ERR_DEPOT_00 );
        ERR_CASE( ERR_DEPOT_01,      kPacket.m_nOK, NetError::ERR_DEPOT_01 );
        ERR_CASE( ERR_DEPOT_02,      kPacket.m_nOK, NetError::ERR_DEPOT_02 ); //창고 슬롯이 비어있지 않음.
        ERR_CASE( ERR_DEPOT_03,      kPacket.m_nOK, NetError::ERR_DEPOT_03 );
        ERR_CASE( ERR_DEPOT_04,      kPacket.m_nOK, NetError::ERR_DEPOT_04 );
        ERR_CASE( ERR_DEPOT_13,      kPacket.m_nOK, NetError::ERR_DEPOT_13 );
        ERR_CASE( ERR_DEPOT_15,      kPacket.m_nOK, NetError::ERR_DEPOT_15 ); // 창고 보관 불가 아이템

    default:
        START_LOG( cerr, L" 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"창고에 아이템 넣기, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_ItemID )
        //<< BUILD_LOGc( kPacket_.m_kDepotKey.m_cCharType )
        << BUILD_LOGc( kPacket_.m_kDepotKey.m_cTabID )
        << BUILD_LOGc( kPacket_.m_kDepotKey.m_cSlotID )
        << BUILD_LOG( kPacket.m_kDepotItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kDepotItem.m_nCount )
        << BUILD_LOG( kPacket_.m_nCount )
        << END_LOG;
    
    SEND_PACKET( EVENT_DEPOT_INSERT_ITEM_ACK );
}

_IMPL_ON_FUNC( EVENT_DEPOT_DELETE_ITEM_REQ, KDepotItem )
{
    SET_ERROR( ERR_UNKNOWN );

    KItemPtr spItem = m_kInventory.FindDepotItemByItemUID( kPacket_.m_ItemUID );
    KItemPtr spDepotItem = m_kInventory.FindDepotItemByKey( kPacket_.m_kDepotKey );
    KItemPtr spResultItem;
    KInvenDepotMoveLog kInvenDepotMoveLog;

    KItem kInvenItem;
    KEVENT_DEPOT_DELETE_ITEM_ACK kPacket;
    std::map< GCITEMUID, KPetInfo >::iterator mitPet;

    if ( spItem == NULL ) {
        SET_ERR_GOTO( ERR_DEPOT_00 , END_PROC );
    }

    if ( spItem->m_nPeriod != KItem::UNLIMITED_ITEM ) { // 기간제인데 영구 아이템이 아닌경우 에러
        SET_ERR_GOTO( ERR_DEPOT_04, END_PROC );
    }

    if ( spItem->m_cInvenType == KItem::INVENTORY ) { // 인벤토리 아이템인데 지우려고 함
        SET_ERR_GOTO( ERR_DEPOT_07, END_PROC );
    }

    if ( spDepotItem == NULL ) {
        SET_ERR_GOTO( ERR_DEPOT_05, END_PROC );
    }

    if ( spItem != spDepotItem ) {
        SET_ERR_GOTO( ERR_DEPOT_05, END_PROC );
    }

    // 수량 아이템 이동 로그에 대해서만 남긴다.
    if( kPacket_.m_nCount > 0 ) {
        kInvenDepotMoveLog.m_nType = KInvenDepotMoveLog::DEPOT_TO_INVEN;
        kInvenDepotMoveLog.m_dwUID = GetUID();
        kInvenDepotMoveLog.m_cCharType = GetCurrentCharType();
        kInvenDepotMoveLog.m_nRequestCount = kPacket_.m_nCount;

        if( spItem ) {
            kInvenDepotMoveLog.m_ItemIDFrom = spItem->m_ItemID;
            kInvenDepotMoveLog.m_ItemUIDFrom = spItem->m_ItemUID;
            kInvenDepotMoveLog.m_nCountFrom = spItem->m_nCount;
        }
    }

    if ( spItem->m_nCount == KItem::UNLIMITED_ITEM && spItem->m_nPeriod == KItem::UNLIMITED_ITEM ) {
    // 영구 아이템을 창고에서 인벤으로 옮김
        if( false == m_kInventory.MoveDepotToInven( *spItem, kPacket_.m_kDepotKey ) ) {
            SET_ERR_GOTO( ERR_DEPOT_06, END_PROC );
        }
    }

    // 수량 아이템을 창고에서 인벤으로 넣는 경우
    if ( spItem->m_nCount != KItem::UNLIMITED_ITEM && kPacket_.m_nCount > 0 ) { 

        // 수량을 합치는 경우에 등급이 같으면 합치자. 속성, 강화레벨, 소켓, 코디 아이템 여부는 다른 경우가 없다고 가정한다.
        if( false == m_kInventory.MoveDepotToInvenCount( *spItem, kPacket_, kInvenItem ) ) {

        }
    }

    if ( spItem->m_nCount == 0 ) {
        // 창고 컨테이너에서 지우고, DB 에서도 삭제
        //m_kInventory.ReserveDeleteCountItem( kPacket_.m_kDepotKey, *spItem );
        // 0 개 수량 아이템은 삭제 대기 컨테이너에 따로 담자. 창고에서 지울 아이템은 depotkey 가 필요하기 때문에...
    }

    // 펫 컨테이너 삭제/추가 처리.
    mitPet = m_mapDepotPetInfo.find( kPacket_.m_ItemUID );
    if ( mitPet != m_mapDepotPetInfo.end() ) {
        if( m_mapPetInfo.insert( std::make_pair( mitPet->first, mitPet->second) ).second == true ) {
            
            m_mapDepotPetInfo.erase( mitPet );
        }
    }

    spResultItem = m_kInventory.FindDepotItemByItemUID( kPacket_.m_ItemUID );
    if( spResultItem == NULL ) {
        //kPacket.m_kDepotItem
    }
    kPacket.m_kDepotItem = *spItem;

    if ( kInvenItem.m_ItemID != 0 ) {
        kPacket.m_kInvenItem = kInvenItem;
    }
    else {
        kPacket.m_kInvenItem = *spItem;
    }

    // 수량 아이템 이동 로그에 대해서만 남긴다.
    if( kPacket_.m_nCount > 0 ) {
        kInvenDepotMoveLog.m_ItemIDTo = kInvenItem.m_ItemID;
        kInvenDepotMoveLog.m_ItemUIDTo = kInvenItem.m_ItemUID;
        kInvenDepotMoveLog.m_nCountTo = kInvenItem.m_nCount;

        InsertInvenDepotMoveInfo( kInvenDepotMoveLog );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_DEPOT_00,      kPacket.m_nOK, NetError::ERR_DEPOT_00 );
        ERR_CASE( ERR_DEPOT_01,      kPacket.m_nOK, NetError::ERR_DEPOT_01 );
        ERR_CASE( ERR_DEPOT_02,      kPacket.m_nOK, NetError::ERR_DEPOT_02 );
        ERR_CASE( ERR_DEPOT_04,      kPacket.m_nOK, NetError::ERR_DEPOT_04 );
        ERR_CASE( ERR_DEPOT_05,      kPacket.m_nOK, NetError::ERR_DEPOT_05 );
        ERR_CASE( ERR_DEPOT_06,      kPacket.m_nOK, NetError::ERR_DEPOT_06 );
        ERR_CASE( ERR_DEPOT_07,      kPacket.m_nOK, NetError::ERR_DEPOT_07 );

    default:
        START_LOG( cerr, L" 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"창고에서 아이템 삭제, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_ItemID )
        //<< BUILD_LOGc( kPacket_.m_kDepotKey.m_cCharType )
        << BUILD_LOGc( kPacket_.m_kDepotKey.m_cTabID )
        << BUILD_LOGc( kPacket_.m_kDepotKey.m_cSlotID )
        << BUILD_LOG( kPacket.m_kInvenItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kInvenItem.m_nCount )
        << BUILD_LOGc( kPacket.m_kInvenItem.m_cInvenType )
        << END_LOG;

    SEND_PACKET( EVENT_DEPOT_DELETE_ITEM_ACK );

}
// 창고 내 아이템의 슬롯 이동
IMPL_ON_FUNC( EVENT_DEPOT_MOVE_ITEM_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_DEPOT_MOVE_ITEM_ACK kPacket;

    KItemPtr spItem = m_kInventory.FindDepotItemByItemUID( kPacket_.m_ItemUID );
    KItemPtr spDepotItemSrc = m_kInventory.FindDepotItemByKey( kPacket_.m_kOldPos );
    KItemPtr spDepotItemDest = m_kInventory.FindDepotItemByKey( kPacket_.m_kNewPos );

    if ( spItem == NULL ) { // 아이템이 있는가
        SET_ERR_GOTO( ERR_DEPOT_00 , END_PROC );
    }

    if ( spDepotItemSrc == NULL ) { // 창고 아이템이 맞는가
        SET_ERR_GOTO( ERR_DEPOT_05, END_PROC );
    }

    if ( spDepotItemDest ) { // 옮기려는 슬롯은 비었는가
        SET_ERR_GOTO( ERR_DEPOT_02, END_PROC );
    }

    if( false == m_kInventory.ChangeDepotSlot( *spItem, kPacket_.m_kOldPos, kPacket_.m_kNewPos ) ) {
        SET_ERR_GOTO( ERR_DEPOT_08, END_PROC );
    }

    kPacket.m_kSrcItem = *spItem;
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_DEPOT_00,      kPacket.m_nOK, NetError::ERR_DEPOT_00 );
        ERR_CASE( ERR_DEPOT_02,      kPacket.m_nOK, NetError::ERR_DEPOT_02 );
        ERR_CASE( ERR_DEPOT_05,      kPacket.m_nOK, NetError::ERR_DEPOT_05 );
        ERR_CASE( ERR_DEPOT_08,      kPacket.m_nOK, NetError::ERR_DEPOT_08 );

    default:
        START_LOG( cerr, L" 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"창고 아이템 슬롯 이동, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket.m_kSrcItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kSrcItem.m_nCount )
        << BUILD_LOGc( kPacket.m_kSrcItem.m_cInvenType )
        << END_LOG;

    SEND_PACKET( EVENT_DEPOT_MOVE_ITEM_ACK );
}

IMPL_ON_FUNC( EVENT_DEPOT_EXTEND_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_DEPOT_EXTEND_ACK kPacket;

    KDepotExtend kDBPacket_;
    kDBPacket_.m_nExtendType = kPacket_.m_nExtendType;
    //kDBPacket_.m_kDepot.m_cCharType = kPacket_.m_CharType;
    kDBPacket_.m_kDepot.m_cTabID = kPacket_.m_TabID;

    std::map< char, KDepotInfo >::iterator mit;

    SiKDepotManager()->SetExtendInfo( kDBPacket_ );

    KItemPtr spItem = m_kInventory.FindItemByItemID( kDBPacket_.m_kItem.m_ItemID );

    if ( spItem == NULL ) {
        SET_ERR_GOTO( ERR_DEPOT_00, END_PROC );
    }
    
    //int iTabTotalSize = 0;
    mit = m_mapDepotInfo.find( kPacket_.m_TabID );

    // 슬롯 확장 요청인데 해당 캐릭터 창고의 캐싱된 데이터가 없는 경우의 오류처리
    if ( ( KDepotExtend::SLOT_EXTEND == kPacket_.m_nExtendType ) && ( mit == m_mapDepotInfo.end() ) ) {
        SET_ERR_GOTO( ERR_DEPOT_10, END_PROC );
    }

    int iSlotPreTotalSize = 0;

    if ( mit != m_mapDepotInfo.end() ) {
        iSlotPreTotalSize = mit->second.m_nSlotSize;
        //iTabTotalSize = mit->second.m_cTabID;
    }
    //else {
    //    SET_ERR_GOTO( ERR_DEPOT_10, END_PROC ); // 창고 캐싱 데이터가 없는 상태. 
    //}
    // 현재 창고가 확장 가능한가?  현재 슬롯 사이즈 + 확장할 사이즈 > 허용 최대 사이즈
    if ( iSlotPreTotalSize + kDBPacket_.m_kDepot.m_nSlotSize > SiKDepotManager()->GetMaxSlotSize() ) {
        SET_ERR_GOTO( ERR_DEPOT_11, END_PROC );
    }

    // 확장할 탭이 최대 탭 사이즈를 초과하지 않는가
    if ( kPacket_.m_TabID > SiKDepotManager()->GetMaxTabSize() ) {
        SET_ERR_GOTO( ERR_DEPOT_11, END_PROC );
    }

    kDBPacket_.m_kItem = *spItem;

    SET_ERROR( NET_OK );

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_DEPOT_EXTEND_REQ, kDBPacket_ );

END_PROC:

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_DEPOT_00,      kPacket.m_nOK, NetError::ERR_DEPOT_00 );
        ERR_CASE( ERR_DEPOT_10,      kPacket.m_nOK, NetError::ERR_DEPOT_10 );
        ERR_CASE( ERR_DEPOT_11,      kPacket.m_nOK, NetError::ERR_DEPOT_11 );
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"창고 확장 요청중 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( NetError::NET_OK != kPacket.m_nOK ) {
        SEND_PACKET( EVENT_DEPOT_EXTEND_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"창고 확장 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kDBPacket_.m_nExtendType )
        //<< BUILD_LOGc( kPacket_.m_CharType )
        << BUILD_LOGc( kPacket_.m_TabID )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_DEPOT_EXTEND_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    KItemPtr spItem = m_kInventory.FindItemByItemUID( kPacket_.m_kUseItem.m_ItemUID );
    
    std::map< char, KDepotInfo >::iterator mit = m_mapDepotInfo.find( kPacket_.m_kDepot.m_cTabID );

    if (!IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_DEPOT_12) )  {
        goto END_PROC;
    }

    if ( !spItem ) {
        SET_ERR_GOTO( ERR_DEPOT_00, END_PROC );
    }
   
    spItem->m_nCount = kPacket_.m_kUseItem.m_nCount;
    spItem->m_nInitCount = spItem->m_nCount;

    kPacket_.m_kUseItem = *spItem;

    if ( kPacket_.m_kUseItem.m_nCount == 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kUseItem.m_ItemID, kPacket_.m_kUseItem.m_ItemUID );
    }    

    if (mit == m_mapDepotInfo.end() ) {
        kPacket_.m_kDepot.m_bDBSelectItem = true;
        m_mapDepotInfo[kPacket_.m_kDepot.m_cTabID] = kPacket_.m_kDepot;
    }
    else {
        mit->second.m_nSlotSize += kPacket_.m_kDepot.m_nSlotSize;
        kPacket_.m_kDepot = mit->second;
    }

END_PROC:
    SEND_RECEIVED_PACKET( EVENT_DEPOT_EXTEND_ACK );

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_DEPOT_00,      kPacket_.m_nOK, NetError::ERR_DEPOT_00 ); // 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_DEPOT_12,      kPacket_.m_nOK, NetError::ERR_DEPOT_12 ); // 창고 확장 아이템 DB 수량 동기화 실패.
        ERR_CASE( ERR_DEPOT_14,      kPacket_.m_nOK, NetError::ERR_DEPOT_14 ); // 창고 확장 DB SP 실행 실패.

    default:
        START_LOG( cerr, L"창고 확장 응답중 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"창고 확장 응답, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_UPDATE_DEPOT_DATA_REQ )
{
    KDepotUpdateData kDepotUpdateData;
    kDepotUpdateData.m_bAckToUser = true;
    GetDBDepotUpdateData( kDepotUpdateData );

    int kPacket = 0;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_UPDATE_DEPOT_DATA_REQ, kDepotUpdateData );

    SET_ERROR( NET_OK );
END_PROC:

    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,            kPacket, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket, -98 );   // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"창고 DB 업데이트 요청, 알 수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( GetName() ) << END_LOG;
        kPacket = 99;
    }

    if ( kPacket != NetError::NET_OK ) {
        SEND_PACKET( EVENT_UPDATE_DEPOT_DATA_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_UPDATE_DEPOT_DATA_ACK, KDepotUpdateData )
{
    KEVENT_UPDATE_DEPOT_DATA_ACK kPacket;

    std::vector<KItem>::const_iterator vit;
    for ( vit = kPacket_.m_vecInvenCountItem.begin(); vit != kPacket_.m_vecInvenCountItem.end(); ++vit ) {
        if ( vit->m_bReserveNewUID == false ) {
            continue;
        }
        KItemPtr spDepotItem = m_kInventory.FindDepotItemByItemID( vit->m_ItemID );

        if ( spDepotItem == NULL || spDepotItem->m_bReserveNewUID == false ) {
            continue;
        }

        std::pair< GCITEMUID, GCITEMUID > prBeforeAfter;

        KItem kItem = *spDepotItem;
        // key 를 안바꾸고 데이터만 바꾸는 문제가 있어서, 기존 데이터 삭제하고 새로운 데이터로 다시 넣음
        prBeforeAfter.first = spDepotItem->m_ItemUID;
        
        KDepotKey depotKey( kItem.m_cTabID, kItem.m_cSlotID );
        m_kInventory.DeleteDepotItem( kItem, depotKey );
        
        kItem.m_ItemUID = vit->m_ItemUID;
        kItem.m_bReserveNewUID = false;

        m_kInventory.AddDepotItem( kItem );

        prBeforeAfter.second = kItem.m_ItemUID;
        kPacket.m_vecDepotItem.push_back( prBeforeAfter );
        
    }
    for ( vit = kPacket_.m_vecDeleteDepotCountItem.begin(); vit != kPacket_.m_vecDeleteDepotCountItem.end(); ++vit ) {
        if ( vit->m_bReserveNewUID == false ) {
            continue;
        }
        KItemPtr spItem = m_kInventory.FindDurationItemByItemID( vit->m_ItemID );

        if ( spItem == NULL || spItem->m_bReserveNewUID == false ) {
            continue;
        }
        
        std::pair< GCITEMUID, GCITEMUID > prBeforeAfter;
        prBeforeAfter.first = spItem->m_ItemUID; // beforeUID
        
        KItem kItem = *spItem;
        m_kInventory.RemoveItem( kItem.m_ItemUID );
                
        kItem.m_ItemUID = vit->m_ItemUID;
        kItem.m_bReserveNewUID = false;
        kItem.m_nInitCount = kItem.m_nCount; // 2013-05-31 00:07 수량 증가 버그 수정.

        prBeforeAfter.second = kItem.m_ItemUID; // afterUID

        m_kInventory.AddItem( kItem );

        kPacket.m_vecInvenItem.push_back( prBeforeAfter );
        
    }
    
    SEND_PACKET( EVENT_UPDATE_DEPOT_DATA_ACK );
}


IMPL_ON_FUNC( EVENT_ADD_SKILL_SLOT_OPEN_REQ )
{
    KEVENT_ADD_SKILL_SLOT_OPEN_ACK kPacket;
    KItemPtr pkItem;
    std::map<char, int>::iterator mit;

    SET_ERROR( ERR_UNKNOWN );

    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_ItmeID );

    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_SLOTINDEX_OPEN_01, END_PROC );
    }

    if( !SiKSkillManager()->CheckAPCharAddSlotOpen( kPacket_.m_ItmeID, kPacket_.m_cCharType ) ) {
        SET_ERR_GOTO( ERR_SLOTINDEX_OPEN_02, END_PROC );
    }

    // 실제로 오픈이 가능한지 체크.
    mit = m_mapSlotOpenInfo.find( kPacket_.m_cCharType );
    if( mit == m_mapSlotOpenInfo.end() ) {
        SET_ERR_GOTO( ERR_SLOTINDEX_OPEN_03, END_PROC );
    }

    if( SiKSkillManager()->GetMaxCharSlotNum() <= mit->second ) {
        SET_ERR_GOTO( ERR_SLOTINDEX_OPEN_04, END_PROC );
    }

    kPacket_.m_nAddSlotIndex = mit->second + 1; // 추가되는 SlotIndex는 현재 숫자의 1을 더한값이다.
    kPacket_.m_kUseItem = *pkItem;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ADD_SKILL_SLOT_OPEN_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                     kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_SLOTINDEX_OPEN_01,      kPacket.m_nOK, 1 ); // 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_SLOTINDEX_OPEN_02,      kPacket.m_nOK, 2 ); // 선택된 아이템으로 슬롯 인덱스 추가 안됨.
        ERR_CASE( ERR_SLOTINDEX_OPEN_03,      kPacket.m_nOK, 3 ); // 해당 캐릭터는 슬롯 오픈 불가한 캐릭터.
        ERR_CASE( ERR_SLOTINDEX_OPEN_04,      kPacket.m_nOK, 4 ); // 이미 최대 슬롯 인덱스까지 오픈했음.
        ERR_CASE( ERR_SLOTINDEX_OPEN_05,      kPacket.m_nOK, 5 ); // 슬롯 오픈 DB처리 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,       kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"슬롯 오픈 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"슬롯 오픈 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_ADD_SKILL_SLOT_OPEN_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ADD_SKILL_SLOT_OPEN_ACK )
{
    KItemPtr pkItem;
    std::map<char, int>::iterator mit;
    std::vector<KItem>::iterator vit;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    vit = kPacket_.m_vecUseItem.begin();
    pkItem = m_kInventory.FindItemByItemID( vit->m_ItemID );

    if ( pkItem && pkItem->m_nCount > 0 ) {
        pkItem->m_nCount = vit->m_nCount;
        pkItem->m_nInitCount = vit->m_nCount;
        kPacket_.m_vecUseItem.clear();
        kPacket_.m_vecUseItem.push_back( *pkItem );
    }

    mit = m_mapSlotOpenInfo.find( kPacket_.m_cCharType );
    if( mit != m_mapSlotOpenInfo.end() ) {
        mit->second = kPacket_.m_nAddSlotIndex; // 최대 slot index 갱신.
    }
    kPacket_.m_mapSlotOpenInfo = m_mapSlotOpenInfo;

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                     kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_SLOTINDEX_OPEN_01,      kPacket_.m_nOK, 1 ); // 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_SLOTINDEX_OPEN_02,      kPacket_.m_nOK, 2 ); // 선택된 아이템으로 슬롯 인덱스 추가 안됨.
        ERR_CASE( ERR_SLOTINDEX_OPEN_03,      kPacket_.m_nOK, 3 ); // 해당 캐릭터는 슬롯 오픈 불가한 캐릭터.
        ERR_CASE( ERR_SLOTINDEX_OPEN_04,      kPacket_.m_nOK, 4 ); // 이미 최대 슬롯 인덱스까지 오픈했음.
        ERR_CASE( ERR_SLOTINDEX_OPEN_05,      kPacket_.m_nOK, 5 ); // 슬롯 오픈 DB처리 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,       kPacket_.m_nOK, -98 );// 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"슬롯 오픈 요청 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L" 슬롯 오픈 요청 응답, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_nAddSlotIndex )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() )
        << BUILD_LOG( kPacket_.m_mapSlotOpenInfo.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ADD_SKILL_SLOT_OPEN_ACK );
}

IMPL_ON_FUNC( EVENT_LEVEL_RESET_EVENT_INFO_ACK )
{
    //  유저정보에 DB에서 가져온 정보 셋팅
    m_mapLevelResetCharInfo = kPacket_.m_mapResetCharInfo;

    //  기본 이벤트 정보 패킷에 실어서 User한테 보내주자.
    SiKLevelResetEvent()->GetResetItemList( kPacket_.m_mapGPResetItemList, kPacket_.m_mapCashResetItemList );
    SiKLevelResetEvent()->GetRewardList( kPacket_.m_mapGPRewardItemList, kPacket_.m_mapCashRewardItemList );
    kPacket_.m_nLevel = SiKLevelResetEvent()->GetRewardLevel();

    //  유저한테 정보를 보내준다.
    LIF( SendPacket( KUserEvent::EVENT_LEVEL_RESET_EVENT_INFO_ACK, kPacket_ ) );


    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"레벨 초기화 이벤트 정보 전송 User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_mapResetCharInfo.size() )
        << BUILD_LOG( kPacket_.m_mapGPResetItemList.size() )
        << BUILD_LOG( kPacket_.m_mapCashResetItemList.size() )        
        << BUILD_LOG( kPacket_.m_mapGPRewardItemList.size() )
        << BUILD_LOG( kPacket_.m_mapCashRewardItemList.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_LEVEL_RESET_EVENT_ITEM_REQ )
{
    _VERIFY_STATE(( 1, KUserFSM::STATE_CHANNEL));


    KEVENT_LEVEL_RESET_EVENT_ITEM_ACK    kPacket;
    KDB_EVENT_LEVEL_RESET_EVENT_ITEM_REQ kDBPacket;


    KItemPtr pkItem;
    KCharacterInfo* pkCharInfo = NULL;

    char cCharType = kPacket_.m_cCharType;

    if ( !SiKLevelResetEvent()->IsRun() )
    {
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_01, END_PROC );
    }

    if ( GetCurrentCharType() != kPacket_.m_cCharType ) {
        START_LOG( cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName() )
            << END_LOG;
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_06, END_PROC );
    }

    // 캐릭터 정보를 얻는다
    pkCharInfo = GetCharacterInfo( cCharType );
    if ( NULL == pkCharInfo ) {
        START_LOG( cwarn, L"유저 캐릭터 정보가 없음. LoginID : " << GetName() ) << END_LOG;
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_06, END_PROC );
    }

    bool bCashType = false;
    
    if ( !SiKLevelResetEvent()->IsResetItemID( cCharType, kPacket_.m_ItemID, bCashType ) ) 
    {
        START_LOG( cwarn, L"리셋 아이템이 아님 LoginID : " << GetName() << kPacket_.m_ItemID ) << END_LOG;
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_09, END_PROC );
    }

    // 해당 아이템이 내 인벤에 존재하는지.
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
    if( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_02, END_PROC );
    }
    kDBPacket.m_kItem = *pkItem;

    //  경험치 초기화 ( 경험치는 여기서 먼저 초기화 해준다. ) 
    //  아이템 사용 실패하면 어떡하지???
    //  
    {
        pkCharInfo->m_biExp = std::min<__int64>( SiKResultManager()->GetExpByLv( 0 ), pkCharInfo->m_biExp );
        
        __int64 diffExp = pkCharInfo->m_biExp - pkCharInfo->m_biInitExp;

        pkCharInfo->m_biInitExp = pkCharInfo->m_biExp;
        pkCharInfo->m_dwLevel = (DWORD)SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );
        // SPX 계산
        SiKSkillManager()->GetSkillPointInfo( cCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );

        kDBPacket.m_biDiffExp = diffExp;

        m_mapInitExp[pkCharInfo->m_cCharType] = pkCharInfo->m_biExp; // 통계 및 경험치 롤백 방지용 변수를 바꿔야 함.
    }

    //  삭제할 미션 체크
    {
        // 지우려고하는 미션이 존재하는가        
        if( !GetMissionListOfType( KMissionInfo::MISSION_TYPE_NORMAL, cCharType, kDBPacket.m_vecMissionList ) )
        {
            SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_07, END_PROC );
        }
    }
    
    
    kDBPacket.m_cCharType = cCharType;
    kDBPacket.m_bCashType = bCashType;
    kDBPacket.m_dwVersion = SiKLevelResetEvent()->GetVersion();


    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_LEVEL_RESET_EVENT_ITEM_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_01, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_02, kPacket.m_nOK, 2 ); // 인벤에 없음
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_03, kPacket.m_nOK, 3 ); // 수량이 모자름
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_06, kPacket.m_nOK, 6 ); // 캐릭터 타입 오류
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_07, kPacket.m_nOK, 7 ); // 미션 초기화 오류        
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_08, kPacket.m_nOK, 8 ); // 미션 초기화 오류    
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_09, kPacket.m_nOK, 9 ); // 아이템 ID 오류
        

    default:
        START_LOG( cerr, L"레벨 초기화 아이템 사용 요청 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }
    
    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_DATA( EVENT_LEVEL_RESET_EVENT_ITEM_ACK, kPacket );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 , L"레벨 초기화 아이템 사용 요청 User : " << GetName() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( cCharType )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket.m_bCashType )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_LEVEL_RESET_EVENT_ITEM_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    KItemPtr pkItem;
    std::map<char,std::pair<bool,bool>>::iterator iter;

    if ( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    // 아이템의 수량 감소
    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_kItem.m_ItemID );
    if ( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_02, END_PROC );
    }
    
    if ( pkItem ) {
        pkItem->m_nCount = kPacket_.m_kItem.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kItem.m_nCount;

        if ( pkItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( kPacket_.m_kItem.m_ItemID, kPacket_.m_kItem.m_ItemUID );
            START_LOG( clog, L"수량이 없는 레벨 리셋 아이템 인벤토리에서 제거.. LoginID : " << GetName() )
                << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
                << END_LOG;
        }
    }

    // 캐릭터 정보를 얻는다
    KCharacterInfo* pkCharInfo = NULL;
    
    // 캐릭터 정보를 얻는다
    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if ( NULL == pkCharInfo ) {
        START_LOG( cwarn, L"유저 캐릭터 정보가 없음. LoginID : " << GetName() )
            << END_LOG;
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_ITEM_06, END_PROC );
    }

    kPacket_.m_biExp = pkCharInfo->m_biExp;
    kPacket_.m_kSkillInfo = pkCharInfo->m_kSkillInfo;

    // 스킬 초기화
    {
        ResetEquipSkill( kPacket_.m_cCharType );
        LIF( ResetTrainedSkill( kPacket_.m_cCharType ) );
        kPacket_.m_mapTrainedSkill = m_mapTrainedSkill; // 수정된 배운 스킬정보 가져오기.
    }   


    //  장착 장비 해제
    {
        //  장착 정보 초기화
        pkCharInfo->m_vecEquipItems.clear();

        //  펫 정보 초기화
        pkCharInfo->m_kEquipPetInfo.Init();

        //  코디 초기화
        std::map<char,KLookEquipChar>::iterator iter = m_mapLookEquip.find( kPacket_.m_cCharType );
        if ( iter != m_mapLookEquip.end() )
        {
            iter->second.UnAllEquipItem();
        }

        //  변경된 캐릭터 정보 확인
        m_setChangeEquipInfo.insert( kPacket_.m_cCharType );
    }

    //  미션 삭제
    {
        std::vector< DWORD >::iterator iter = kPacket_.m_vecMissionList.begin();
        std::vector< DWORD >::iterator iter_end = kPacket_.m_vecMissionList.end();

        for( ; iter != iter_end; ++iter )
        {
            if( !EraseMissionDataFromSlot( *iter ) )
            {
                START_LOG( cerr, L"실제 미션 제거 시점에서 해당 미션을 찾을수 없음.. Name : " << m_strName )
                    << BUILD_LOG( *iter )
                    << END_LOG;
            }
        }
    }

    // 레벨 리셋 캐릭터 정보 셋팅
    iter =  m_mapLevelResetCharInfo.find( kPacket_.m_cCharType );
    if ( iter != m_mapLevelResetCharInfo.end() )
    {
        iter->second = std::make_pair(kPacket_.m_bCashType, false);
    }
    else
    {
        m_mapLevelResetCharInfo.insert( std::make_pair( kPacket_.m_cCharType, std::make_pair(false,kPacket_.m_bCashType) ) );
    }


END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_01, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_02, kPacket_.m_nOK, 2 ); // 인벤에 없음
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_03, kPacket_.m_nOK, 3 ); // 수량이 모자름
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_04, kPacket_.m_nOK, 4 ); // 초기화 셋팅 오류
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_05, kPacket_.m_nOK, 5 ); // Exp 동기화 오류
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_06, kPacket_.m_nOK, 6 ); // 캐릭터 타입 오류
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_07, kPacket_.m_nOK, 7 ); // 미션 초기화 오류
        ERR_CASE( ERR_USE_LEVEL_RESET_ITEM_08, kPacket_.m_nOK, 8 ); // 아이템 개수 동기화 실패       

    default:
        START_LOG( cerr, L"레벨 초기화 아이템 사용 요청 중 알수 없는 오류2 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_biExp )
        << END_LOG;


    SEND_RECEIVED_PACKET( EVENT_LEVEL_RESET_EVENT_ITEM_ACK );
}

IMPL_ON_FUNC( EVENT_LEVEL_RESET_EVENT_REWARD_ACK )
{
    KSignBoardData kData;
    std::wstring wstrData;
    std::map<char,std::pair<bool,bool>>::iterator iter;

    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

    if ( !SiKLoudMessage()->GetLevelResetMsg( m_strNickName, SiKLevelResetEvent()->GetRewardLevel(), wstrData, GetLanguageCode() ) ) {
        START_LOG( cerr, L"공지 내용 가져오기 실패" ) << END_LOG;
        SET_ERR_GOTO( ERR_USE_LEVEL_RESET_REWARD_02, END_PROC );        
    }

    // 레벨 리셋 캐릭터 보상 정보 셋팅
    iter =  m_mapLevelResetCharInfo.find( kPacket_.m_cCharType );
    if ( iter != m_mapLevelResetCharInfo.end() )
    {
        iter->second.second = true;
    }


    kData.m_dwColor         = 0L;
    kData.m_ItemID          = 0L;
    kData.m_dwSenderUID     = 0L;
    kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN;   // 운영자용 전광판
    kData.m_strSenderNick.clear();
    kData.m_strMsg = wstrData;

    SiKSignBoard()->QueueingAdminData( kData );

    //SEND_DATA( EVENT_SIGN_BOARD_NOT, kData );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_USE_LEVEL_RESET_REWARD_01, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_USE_LEVEL_RESET_REWARD_02, kPacket_.m_nOK, 0 ); // 공지 출력 실패

    default:
        START_LOG( cerr, L"레벨 초기화 보상 요청 중 알수 없는 오류2 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << END_LOG;

    if ( kPacket_.m_nOK == 0 )
    {
        SEND_RECEIVED_PACKET( EVENT_LEVEL_RESET_EVENT_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_TEST_ADD_ITEM_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    if( !SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
    {
        SET_ERR_GOTO( ERR_TEST_ADD_ITEM_01, END_PROC );
    }

    QUEUEING_EVENT_TO_DB( EVENT_TEST_ADD_ITEM_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:

    KEVENT_TEST_ADD_ITEM_ACK kPacket;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_TEST_ADD_ITEM_01, kPacket.m_nOK, 1);
    default:
        START_LOG( cerr, L"아이템을 추가하는데 알수 없는 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_itemID )
            << BUILD_LOG( kPacket_.m_nDuration )
            << BUILD_LOG( kPacket_.m_nPeriod )
            << END_LOG;
    }

    if ( kPacket.m_nOK != 0 ) 
    {
        SEND_RECEIVED_PACKET( EVENT_TEST_ADD_ITEM_ACK );
    }
}

IMPL_ON_FUNC( EVENT_TEST_ADD_ITEM_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_TEST_ADD_ITEM_01, kPacket_.m_nOK, 1);  //  테스트 서버가 아님.
        ERR_CASE( ERR_TEST_ADD_ITEM_02, kPacket_.m_nOK, 2);  //  DB아이템 등록 실패

    default:
        START_LOG( cerr, L"아이템 추가중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_TEST_ADD_ITEM_ACK );
}

IMPL_ON_FUNC(EVENT_NEW_CHAR_CHOICE_REQ)
{
    KEVENT_NEW_CHAR_CHOICE_ACK kPacket;
    kPacket.m_nOK = -99;
    DWORD dwLimitLevel = 0;
    SET_ERROR(ERR_UNKNOWN);

    if (IsCharExist(kPacket_.m_cCharType)) {
        SET_ERR_GOTO(ERR_NEWCHAR_CHOICE_01, END_PROC);
    }

    if (SiKCharDefault()->CheckCharCreateEnable(kPacket_.m_cCharType, dwLimitLevel)) {
        if (!CheckCharLimitLevel(dwLimitLevel)) {
            SET_ERR_GOTO(ERR_NEWCHAR_CHOICE_13, END_PROC);
        }
    }

    SiKCharDefault()->GetDefaultItemInfo(kPacket_.m_cCharType, kPacket_.m_vecDefaultReward);
    SiKCharDefault()->GetDefaultSlotItemInfo(kPacket_.m_cCharType, kPacket_.m_vecDefaultSlotReward);
    CheckGiftFirstCharacter(static_cast<int>(kPacket_.m_cCharType), kPacket_.m_vecGiftFirstCharacter);
    kPacket_.m_dwSlotNumber = m_mapCharacterInfo.size();

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_NEW_CHAR_CHOICE_REQ, kPacket_);
    SET_ERROR(NET_OK);

END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_NEWCHAR_CHOICE_01, kPacket.m_nOK, 1);
        ERR_CASE(ERR_NEWCHAR_CHOICE_02, kPacket.m_nOK, 2);
        ERR_CASE(ERR_NEWCHAR_CHOICE_03, kPacket.m_nOK, 3);
        ERR_CASE(ERR_NEWCHAR_CHOICE_04, kPacket.m_nOK, 4);
        ERR_CASE(ERR_NEWCHAR_CHOICE_05, kPacket.m_nOK, 5);
        ERR_CASE(ERR_NEWCHAR_CHOICE_06, kPacket.m_nOK, 6);
        ERR_CASE(ERR_NEWCHAR_CHOICE_13, kPacket.m_nOK, 13);
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98);

    default:
        kPacket.m_nOK = 99;
        START_LOG(cerr, L"캐릭터 선택 획득 요청중 알수 없는 오류, Name : " << m_strName)
            << L"MSG : " << NetError::GetLastNetErrMsg()
            << END_LOG;
    }

    if (kPacket.m_nOK != 0) {
        SEND_PACKET(EVENT_NEW_CHAR_CHOICE_ACK);
    }

    LOG_SUCCESS(kPacket.m_nOK == 0)
        << L"Msg : " << NetError::GetLastNetErrMsg()
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket.m_nOK)
        << END_LOG;
}

IMPL_ON_FUNC(EVENT_NEW_CHAR_CHOICE_ACK)
{
    SET_ERROR(ERR_UNKNOWN);
    NetError::SetLastNetError(kPacket_.m_nOK);
    std::set<int> setDungeonInfo;
    std::set<int> setSkillInfo;
    std::map<char, KCharacterInfo>::iterator mit;
    KQuickSlot kQuickSlot;
    std::vector<KItem>::const_iterator vit;
    std::map<int, KDungeonUserInfo> mapDifficulty;
    mapDifficulty.clear();

    if (NetError::GetLastNetError() != NetError::NET_OK) {
        goto END_PROC;
    }

    LIF(m_mapCharacterInfo.insert(std::make_pair(kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo)).second);
    LIF(AddStatInitExpInfo(kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo.m_biInitExp, kPacket_.m_kCharacterInfo.m_biInitTotalExp));

    if (true == SiKVitalityManager()->IsRun()) {
        int nVitalityForCharacterCreation = 0;
        if (1 == m_mapCharacterInfo.size()) {
            nVitalityForCharacterCreation = SiKVitalityManager()->GetVitalityForFirstCharacterCreation();
        }
        else {
            nVitalityForCharacterCreation = SiKVitalityManager()->GetVitalityForCharacterCreation();
        }
        m_kVitalityUserData.SetVitality(static_cast<int>(kPacket_.m_kCharacterInfo.m_cCharType), nVitalityForCharacterCreation);

        SendVitalityInfo(KVitalityManager::EVST_CREATE_CHAR);

        {
            KDB_EVENT_VITALITY_RECHARGE_NOT kDBPacket;

            kDBPacket.m_nVitalityUpdateType = KVitalityManager::EVUT_INIT;
            kDBPacket.m_nRechargePoint = m_kVitalityUserData.m_nRechargePoint;
            kDBPacket.m_nRechargePointCounter = m_kVitalityUserData.m_nRechargePointCounter;
            kDBPacket.m_nRemainVitality = m_kVitalityUserData.GetVitality(static_cast<int>(kPacket_.m_kCharacterInfo.m_cCharType));
            kDBPacket.m_kCharacterInfo = kPacket_.m_kCharacterInfo;

            QUEUEING_EVENT_TO_DB(DB_EVENT_VITALITY_RECHARGE_NOT, kDBPacket);
        }
    }

    {
        KLookEquipChar kLookEquip;
        kLookEquip.SetCharacter(kPacket_.m_kCharacterInfo.m_cCharType);
        m_mapLookEquip.insert(std::map<char, KLookEquipChar>::value_type(kLookEquip.GetCharacter(), kLookEquip));
    }

    {
        bool bWeaponLock = false;
        m_mapChangeWeapon[kPacket_.m_kCharacterInfo.m_cCharType] = std::make_pair(bWeaponLock, 0);
        kPacket_.m_kCharacterInfo.m_bChangeWeaponLock = bWeaponLock;
    }

    for (int i = 0; i < NUM_GC_GAME_MODE; ++i) {
        const int& nModeID = i;

        if (false == KRoom::IsDifficultyMode(nModeID)) {
            continue;
        }

        if (mapDifficulty.find(nModeID) == mapDifficulty.end()) {
            KDungeonUserInfo kInfo;
            mapDifficulty.insert(std::make_pair(nModeID, kInfo));
        }

        int nLastDifficulty = SiKResultManager()->GetLastDifficulty(nModeID);
        if (nLastDifficulty > -1) {
            if (mapDifficulty[nModeID].m_kModeDifficulty.Get(nLastDifficulty)) {
                mapDifficulty[nModeID].m_kClearData.m_bClear = true;
            }
        }

        KHeroDungeon kInfo;
        bool bHeroDungeon = false;
        if( SiKHeroDungeonManager()->IsHeroDungeon( nModeID ) )
            bHeroDungeon = SiKHeroDungeonManager()->GetHeroDungeonInfo( nModeID, kInfo );

        if (!bHeroDungeon || (bHeroDungeon && !kInfo.m_bResetDifficulty)) {
            for (int i = GC_GMD_NORMAL_BEGIN; i < nLastDifficulty; ++i) {
                if (false == mapDifficulty[nModeID].m_kModeDifficulty.Get(i)) {
                    break;
                }

                mapDifficulty[nModeID].m_kClearData.m_shMaxDifficulty = i;
            }
        }
    }

    if (SiKCharDefault()->CheckCharDungeonInfo(kPacket_.m_kCharacterInfo.m_cCharType, setDungeonInfo)) {
        std::set<int>::iterator sit;
        for (sit = setDungeonInfo.begin(); sit != setDungeonInfo.end(); ++sit) {
            std::map<int, KDungeonUserInfo>::iterator mit;
            mit = mapDifficulty.find(*sit);
            if (mit == mapDifficulty.end()) {
                KDungeonUserInfo kInfo;
                kInfo.m_kModeDifficulty.Set(0);
                mapDifficulty.insert(std::make_pair(*sit, kInfo));
            }
            else {
                mit->second.m_kModeDifficulty.Set(0);
            }
        }
    }

    UpdateCharDifficultInfo(kPacket_.m_kCharacterInfo.m_cCharType, mapDifficulty);
    kPacket_.m_mapDifficulty = mapDifficulty;
    m_mapOldDifficulty.clear();

    if (SiKCharDefault()->CheckCharSkillInfo(kPacket_.m_kCharacterInfo.m_cCharType, setSkillInfo)) {
        std::set<int>::iterator sit;
        for (sit = setSkillInfo.begin(); sit != setSkillInfo.end(); ++sit) {
            if (!InsertDefaultSkillInfo(*sit, kPacket_.m_kCharacterInfo)) {
                START_LOG(cerr, L"캐릭터 기본 스킬 지급 실패")
                    << BUILD_LOGc(kPacket_.m_kCharacterInfo.m_cCharType)
                    << BUILD_LOG(*sit)
                    << END_LOG;
                continue;
            }
        }
    }

    GetSkillInfoData(kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_vecSPInfo);

    {
        KSHCheckPoint::SHChecData shDataNew;
        shDataNew.m_cCurrentCharID = kPacket_.m_kCharacterInfo.m_cCharType;
        shDataNew.m_cOldLevel = static_cast<char>(kPacket_.m_kCharacterInfo.m_dwLevel);
        shDataNew.m_cLevel = static_cast<char>(kPacket_.m_kCharacterInfo.m_dwLevel);
        shDataNew.m_strLogin = GetName();

        KEVENT_SHANGHAI_DROP_REQ shReqNew;
        shReqNew.m_vecDropItems.clear();
        shReqNew.m_vecDropPostItems.clear();
        shReqNew.m_vecAutoMission.clear();
        SiKSHCheckPoint()->NewCharacterDropCheck(shReqNew.m_vecDropItems, shReqNew.m_vecDropPostItems, shReqNew.m_vecAutoMission, shDataNew, KUser::USER_CT_TOTAL);
        SiKSHCheckPoint()->NewCharacterDropCheck(shReqNew.m_vecDropItems, shReqNew.m_vecDropPostItems, shReqNew.m_vecAutoMission, shDataNew, GetUserChannelType());

        if (!shReqNew.m_vecDropItems.empty() || !shReqNew.m_vecDropPostItems.empty() || !shReqNew.m_vecAutoMission.empty()) {
            shReqNew.m_cCharType = kPacket_.m_kCharacterInfo.m_cCharType;
            CheckLevelUpDrop(shReqNew);
        }
    }

    // À¯Àú º¸À¯ ½½·Ô °¹¼ö °»½Å.
    if (m_dwUserRemainIndexCnt > 0) {
        m_dwUserRemainIndexCnt -= 1;
    }
    kPacket_.m_dwUserRemainIndexCnt = m_dwUserRemainIndexCnt;

    m_mapBaseBonus[kPacket_.m_kCharacterInfo.m_cCharType] = kPacket_.m_kCharacterInfo.m_kBonusPoint.m_nBaseBonus;
    m_mapSpecialBonus[kPacket_.m_kCharacterInfo.m_cCharType] = kPacket_.m_kCharacterInfo.m_kBonusPoint.m_nSpecialBonus;

    DWORD dwSlotIndex = 0;
    for (vit = kPacket_.m_vecSlotItem.begin(); vit != kPacket_.m_vecSlotItem.end(); ++vit) {
        KSlotData kData;
        kData.m_nSlotIndex = dwSlotIndex;
        kData.m_kItem.m_dwID = vit->m_ItemID;
        kData.m_kItem.m_dwUID = vit->m_ItemUID;
        kData.m_kItem.m_cItemType = kPacket_.m_kCharacterInfo.m_cCharType;

        kQuickSlot.m_vecItemSlot.push_back(kData);

        ++dwSlotIndex;
    }
    m_mapQuickSlot[kPacket_.m_kCharacterInfo.m_cCharType] = kQuickSlot;

    kPacket_.m_kSlotData = kQuickSlot;
    SetCharEquipItem(kPacket_.m_vecRewardItem, kPacket_.m_kCharacterInfo);
    ApplyCharEquipItem(kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo.m_vecEquipItems);
    UpdateCharEquipInfo(kPacket_.m_kCharacterInfo.m_cCharType);

    SET_ERROR(NET_OK);

END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket_.m_nOK, 0);
        ERR_CASE(ERR_NEWCHAR_CHOICE_01, kPacket_.m_nOK, 1);
        ERR_CASE(ERR_NEWCHAR_CHOICE_02, kPacket_.m_nOK, 2);
        ERR_CASE(ERR_NEWCHAR_CHOICE_03, kPacket_.m_nOK, 3);
        ERR_CASE(ERR_NEWCHAR_CHOICE_04, kPacket_.m_nOK, 4);
        ERR_CASE(ERR_NEWCHAR_CHOICE_05, kPacket_.m_nOK, 5);
        ERR_CASE(ERR_NEWCHAR_CHOICE_06, kPacket_.m_nOK, 6);
        ERR_CASE(ERR_NEWCHAR_CHOICE_07, kPacket_.m_nOK, 7);
        ERR_CASE(ERR_NEWCHAR_CHOICE_08, kPacket_.m_nOK, 8);
        ERR_CASE(ERR_NEWCHAR_CHOICE_09, kPacket_.m_nOK, 9);
        ERR_CASE(ERR_NEWCHAR_CHOICE_10, kPacket_.m_nOK, 10);
        ERR_CASE(ERR_NEWCHAR_CHOICE_11, kPacket_.m_nOK, 11);
        ERR_CASE(ERR_NEWCHAR_CHOICE_12, kPacket_.m_nOK, 12);
        ERR_CASE(ERR_NEWCHAR_CHOICE_13, kPacket_.m_nOK, 13);
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98);
    default:
        kPacket_.m_nOK = 99;
        START_LOG(cerr, L"캐릭터 획득 카드 사용중 알수 없는 오류, Name : " << m_strName)
            << L"MSG : " << NetError::GetLastNetErrMsg() << END_LOG;
    }

    SEND_RECEIVED_PACKET(EVENT_NEW_CHAR_CHOICE_ACK);

    LOG_SUCCESS(kPacket_.m_nOK == 0)
        << L"Msg : " << NetError::GetLastNetErrMsg()
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket_.m_nOK)
        << BUILD_LOGc(kPacket_.m_kCharacterInfo.m_cCharType)
        << BUILD_LOG(kPacket_.m_mapDifficulty.size())
        << BUILD_LOG(setDungeonInfo.size())
        << BUILD_LOG(kPacket_.m_vecRewardItem.size())
        << BUILD_LOG(kPacket_.m_vecSlotItem.size())
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_OLYMPIC_SCHEDULE_REQ )
{
    KEVENT_OLYMPIC_SCHEDULE_ACK kPacket;
    kPacket.m_nOK = 0;

    SiKOlympicManager()->GetOlympicScheduleInfo( kPacket.m_vecOlympicSchedule );

    SEND_PACKET( EVENT_OLYMPIC_SCHEDULE_ACK );

    START_LOG( clog, L"올림픽 스케줄 정보 요청.. Login : " << GetName() )
        << BUILD_LOG( kPacket.m_vecOlympicSchedule.size() )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_OLYMPIC_INFO_REQ )
{
    KEVENT_OLYMPIC_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    KDB_EVENT_OLYMPIC_INFO_REQ kDBPacket;
    SET_ERROR( ERR_UNKNOWN );

    if( !SiKOlympicManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_01, END_PROC );
    }

    SiKOlympicManager()->GetOlympicRegDateRank( kPacket.m_vecRegDateRank );
    SiKOlympicManager()->GetOlympicTotalRank( kPacket.m_vecTotalRank );
    SiKOlympicManager()->GetOlympicGradeRewardList( kPacket.m_mapGradeRewardList );
    SiKOlympicManager()->GetOlympicGradeDesc( kPacket.m_mapGradeDesc );
    kPacket.m_bPlayEnable = m_kOlympicUserData.GetPlayEnable();
    kPacket.m_dwGrade = m_kOlympicUserData.GetUserGrade();
    kPacket.m_dwCountryID = SiKOlympicManager()->GetCountryID();

    if( m_kOlympicUserData.IsLoaded() ) {
        // 현재 진행중인 게임이 없다면 유저 데이터의 m_bPlayEnable을 false로 설정함
        if ( false == SiKOlympicManager()->IsGamePlayEnable() ) {
            m_kOlympicUserData.SetPlayEnable( false );
            kPacket.m_bPlayEnable = m_kOlympicUserData.GetPlayEnable();
        }
        SET_ERR_GOTO( ERR_OLYMPIC_09, END_PROC );
    }

    SiKOlympicManager()->GetCurrentOlympicGameInfo( kDBPacket.m_tmTodayDate, kDBPacket.m_nGameType, kDBPacket.m_nTeamType );

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_OLYMPIC_INFO_REQ, kDBPacket );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_09,           kPacket.m_nOK, 9 ); // DB에서 데이터 이미 가져왔음.
        ERR_CASE( ERR_OLYMPIC_10,           kPacket.m_nOK, 10 ); // 유저 TimeIndex정보 가져오기 실패.
        ERR_CASE( ERR_OLYMPIC_11,           kPacket.m_nOK, 11 ); // 유저 누적 포인트 정보 가져오기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 이벤트 기본정보 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || NetError::GetLastNetError() == NetError::ERR_OLYMPIC_01, L"올림픽 이벤트 기본정보 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecRegDateRank.size() )
        << BUILD_LOG( kPacket.m_vecTotalRank.size() )
        << BUILD_LOG( kPacket.m_mapGradeRewardList.size() )
        << BUILD_LOG( kPacket.m_mapGradeDesc.size() )
        << BUILD_LOG( kPacket.m_bPlayEnable )
        << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_OLYMPIC_INFO_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OLYMPIC_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    SiKOlympicManager()->GetOlympicRegDateRank( kPacket_.m_vecRegDateRank );
    SiKOlympicManager()->GetOlympicTotalRank( kPacket_.m_vecTotalRank );
    SiKOlympicManager()->GetOlympicGradeRewardList( kPacket_.m_mapGradeRewardList );
    SiKOlympicManager()->GetOlympicGradeDesc( kPacket_.m_mapGradeDesc );

    if( kPacket_.m_nTimeIndex == -1 ) {
        kPacket_.m_nTimeIndex = SiKOlympicManager()->GetCurrentTimeIndex();
    }

    // 유저데이터의 m_bPlayEnable 세팅
    if( SiKOlympicManager()->CheckUserTimeIndex( kPacket_.m_tmDBSaveDate, kPacket_.m_nTimeIndex ) ) {
        // 유저데이터의 m_nCurrentTimeIndex 세팅
        m_kOlympicUserData.SetTimeIndex( SiKOlympicManager()->GetCurrentTimeIndex(), true );
    } else {
        m_kOlympicUserData.SetTimeIndex( SiKOlympicManager()->GetCurrentTimeIndex(), false );
    }
    // 현재 진행중인 게임이 없다면 유저 데이터의 m_bPlayEnable을 false로 설정함
    if ( false == SiKOlympicManager()->IsGamePlayEnable() ) {
        m_kOlympicUserData.SetPlayEnable( false );
    }

    // 유저데이터의 m_tmTodayDate, m_nMyAccumulateData 세팅
    m_kOlympicUserData.SetUserOlympicUserData( kPacket_.m_tmTodayDate, kPacket_.m_nMyAccumulateData );
    // 유저데이터의 m_dwGrade 세팅
    m_kOlympicUserData.SetUserGrade( kPacket_.m_dwGrade );

    kPacket_.m_bPlayEnable = m_kOlympicUserData.GetPlayEnable();
    kPacket_.m_nTimeIndex = m_kOlympicUserData.GetUserTimeIndex();
    kPacket_.m_dwCountryID = SiKOlympicManager()->GetCountryID();

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket_.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_09,           kPacket_.m_nOK, 9 ); // DB에서 데이터 이미 가져왔음.
        ERR_CASE( ERR_OLYMPIC_10,           kPacket_.m_nOK, 10 ); // 유저 TimeIndex정보 가져오기 실패.
        ERR_CASE( ERR_OLYMPIC_11,           kPacket_.m_nOK, 11 ); // 유저 누적 포인트 정보 가져오기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 정보 요청중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_INFO_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 정보 요청 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_vecRegDateRank.size() )
        << BUILD_LOG( kPacket_.m_vecTotalRank.size() )
        << BUILD_LOG( kPacket_.m_mapGradeRewardList.size() )
        << BUILD_LOG( kPacket_.m_mapGradeDesc.size() )
        << BUILD_LOG( kPacket_.m_bPlayEnable )
        << BUILD_LOGtm( CTime(kPacket_.m_tmTodayDate) )
        << BUILD_LOG( kPacket_.m_nTimeIndex )
        << BUILD_LOG( kPacket_.m_nMyAccumulateData )
        << BUILD_LOG( kPacket_.m_dwCountryID )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_OLYMPIC_JOIN_GAME_REQ )
{
    KEVENT_OLYMPIC_JOIN_GAME_ACK kPacket;
    kPacket.m_nOK = -99;
    KDB_EVENT_OLYMPIC_JOIN_GAME_REQ kDBPacket;
    SET_ERROR( ERR_UNKNOWN );

    if( !SiKOlympicManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_01, END_PROC );
    }

    if( !SiKOlympicManager()->IsGamePlayEnable() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_07, END_PROC );
    }

    if ( false == m_kOlympicUserData.GetPlayEnable() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_16, END_PROC );
    }

    SiKOlympicManager()->GetCurrentOlympicGameInfo( kDBPacket.m_tmTodayDate, kDBPacket.m_nGameType, kDBPacket.m_nTeamType );

    if( !SiKOlympicManager()->GetGameInfo( kDBPacket.m_nGameType, kDBPacket.m_nDefaultCount, kDBPacket.m_nRewardCount, kDBPacket.m_nOnePointMaxPoint, kDBPacket.m_nTotalMaxPoint ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_02, END_PROC );
    }

    m_kOlympicUserData.GetUserOlympicUserData( kDBPacket.m_tmTodayDate, kDBPacket.m_nMyAccumulateData );
    SiKOlympicManager()->GetGameRewardInfo( kDBPacket.m_nGameType, kDBPacket.m_vecReward );

    if ( kDBPacket.m_nGameType == KOlympicGameInfo::EOGT_BOSSGATE ) {
        kDBPacket.m_vecReward.clear();
    }

    kDBPacket.m_nTimeIndex = m_kOlympicUserData.GetUserTimeIndex();

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_OLYMPIC_JOIN_GAME_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_02,           kPacket.m_nOK, 2 ); // 설정된 게임의 정보가 없음.
        ERR_CASE( ERR_OLYMPIC_07,           kPacket.m_nOK, 7 ); // 올림픽 게임참여 가능한 날짜가 아님.
        ERR_CASE( ERR_OLYMPIC_08,           kPacket.m_nOK, 8 ); // 올림픽 게임참여 보상 아이템 DB지급 실패.
        ERR_CASE( ERR_OLYMPIC_16,           kPacket.m_nOK, 16 ); // 현재 진행중인 올림픽 미니게임에 이미 참여하였음.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 이벤트 게임 참여 요청중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"올림픽 이벤트 게임 참여 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGameType )
        << BUILD_LOG( kPacket.m_nTeamType )
        << BUILD_LOG( kPacket.m_nMyAccumulateData )
        << BUILD_LOG( kPacket.m_nDefaultCount )
        << BUILD_LOG( kPacket.m_nRewardCount )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_OLYMPIC_JOIN_GAME_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OLYMPIC_JOIN_GAME_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

    // 유저데이터의 m_nRemainTryCount 초기값 세팅
    m_kOlympicUserData.SetRemainTryCount( kPacket_.m_nDefaultCount );

    m_kOlympicUserData.SetTimeIndex( kPacket_.m_nTimeIndex, false );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket_.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_02,           kPacket_.m_nOK, 2 ); // 설정된 게임의 정보가 없음.
        ERR_CASE( ERR_OLYMPIC_07,           kPacket_.m_nOK, 7 ); // 올림픽 게임참여 가능한 날짜가 아님.
        ERR_CASE( ERR_OLYMPIC_08,           kPacket_.m_nOK, 8 ); // 올림픽 게임참여 보상 아이템 DB지급 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 게임 참여 요청중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_JOIN_GAME_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 이벤트 게임 참여 요청 응답, User : " << GetName() )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOG( kPacket_.m_nMyAccumulateData )
        << BUILD_LOG( kPacket_.m_nDefaultCount )
        << BUILD_LOG( kPacket_.m_nRewardCount )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_nTimeIndex )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_OLYMPIC_ALARM_NOT )
{
    m_kOlympicUserData.SetTimeIndex( kPacket_.m_nTimeIndex, true );
    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_ALARM_NOT );
}

IMPL_ON_FUNC( EVENT_OLYMPIC_WATERBOMB_RESULT_REQ )
{
    KEVENT_OLYMPIC_WATERBOMB_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKOlympicManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_01, END_PROC );
    }

    // 도전 포인트가 있는지 확인
    if ( false == m_kOlympicUserData.CheckRemainTryPoint() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_03, END_PROC );
    }

    kPacket.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();
    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

    // 점수 제한 체크.
    // 스크립트에 설정된 최대값에 대하여 보정처리를 하고 에러처리는 GameType이 없는 경우에 대해서만 처리.
    if ( !SiKOlympicManager()->CheckGameResultData( kPacket_.m_nGameType, kPacket.m_nMyAccumulateData, kPacket_.m_nIncreasedData ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_02, END_PROC );
    }

    // 점수를 얻은 경우 누적 포인트를 DB에 갱신(diff 전달)
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_OLYMPIC_WATERBOMB_RESULT_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_02,           kPacket.m_nOK, 2 ); // 설정된 게임의 정보가 없음.
        ERR_CASE( ERR_OLYMPIC_03,           kPacket.m_nOK, 3 ); // 도전 포인트가 모자람.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 이벤트 물폭탄 게임결과 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"올림픽 이벤트 물폭탄 게임결과 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << BUILD_LOG( kPacket.m_nRemainTryCount )
        << BUILD_LOG( kPacket.m_nIncreasedData )
        << BUILD_LOG( kPacket.m_nMyAccumulateData )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_OLYMPIC_WATERBOMB_RESULT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OLYMPIC_WATERBOMB_RESULT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_OLYMPIC_12 ) ) {
        goto END_PROC;
    }

    // 유저 데이터 갱신하고
    m_kOlympicUserData.DecreaseRemainTryCount( 1 );
    m_kOlympicUserData.IncreaseMyAccumulateData( kPacket_.m_tmTodayDate, kPacket_.m_nIncreasedData );

    // 갱신된 데이터를 패킷에 저장
    kPacket_.m_nRemainTryCount = m_kOlympicUserData.GetRemainTryCount();
    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_OLYMPIC_05,           kPacket_.m_nOK, 5 ); // 올림픽 누적 포인트 DB 갱신 실패.
        ERR_CASE( ERR_OLYMPIC_12,           kPacket_.m_nOK, 12 ); // 물폭탄으로 획득한 포인트 없음.
    default:
        START_LOG( cerr, L"올림픽 물폭탄 누적 포인트 갱신 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_WATERBOMB_RESULT_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 이벤트 물폭탄 게임결과 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nRemainTryCount)
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_nMyAccumulateData )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ )
{
    KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKOlympicManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_01, END_PROC );
    }

    // 도전 포인트가 있는지 확인
    if ( false == m_kOlympicUserData.CheckRemainTryPoint() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_03, END_PROC );
    }

    // 승/패/무 확률 계산
    if ( false == SiKOlympicManager()->DoOlympicGawibawibo( m_kOlympicUserData.GetRemainTryCount(), kPacket_.m_nResult ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_04, END_PROC );
    }

    kPacket.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();
    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

    if ( kPacket_.m_nResult == KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_WIN ) {
        kPacket_.m_nIncreasedData = 1;
    }
    else {
        kPacket_.m_nIncreasedData = 0;
    }

    // 점수 제한 체크.
    // 스크립트에 설정된 최대값에 대하여 보정처리를 하고 에러처리는 GameType이 없는 경우에 대해서만 처리.
    if ( !SiKOlympicManager()->CheckGameResultData( kPacket_.m_nGameType, kPacket.m_nMyAccumulateData, kPacket_.m_nIncreasedData ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_02, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_02,           kPacket.m_nOK, 2 ); // 설정된 게임의 정보가 없음.
        ERR_CASE( ERR_OLYMPIC_03,           kPacket.m_nOK, 3 ); // 도전 포인트가 모자람.
        ERR_CASE( ERR_OLYMPIC_04,           kPacket.m_nOK, 4 ); // 지정된 도전 포인트 정보가 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 이벤트 가위바위보 게임결과 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"올림픽 이벤트 가위바위보 게임결과 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << BUILD_LOG( kPacket.m_nResult )
        << BUILD_LOG( kPacket.m_nRemainTryCount )
        << BUILD_LOG( kPacket.m_nIncreasedData )
        << BUILD_LOG( kPacket.m_nMyAccumulateData )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_OLYMPIC_06 ) ) {
        goto END_PROC;
    }

    // 유저 데이터 갱신하고
    if ( kPacket_.m_nResult != KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_DRAW ) {
        // 비기면 감소 안함
        m_kOlympicUserData.DecreaseRemainTryCount( 1 );
    }

    m_kOlympicUserData.IncreaseMyAccumulateData( kPacket_.m_tmTodayDate, kPacket_.m_nIncreasedData );

    // 갱신된 데이터를 패킷에 저장
    kPacket_.m_nRemainTryCount = m_kOlympicUserData.GetRemainTryCount();
    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_OLYMPIC_05,           kPacket_.m_nOK, 5 ); // 올림픽 누적 포인트 DB 갱신 실패.
        ERR_CASE( ERR_OLYMPIC_06,           kPacket_.m_nOK, 6 ); // 올림픽 누적 포인트가 최대치임.
    default:
        START_LOG( cerr, L"올림픽 가위바위보 누적 포인트 갱신 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 이벤트 가위바위보 게임결과 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nResult)
        << BUILD_LOG( kPacket_.m_nRemainTryCount)
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_nMyAccumulateData )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_OLYMPIC_DICEPLAY_RESULT_REQ )
{
    KEVENT_OLYMPIC_DICEPLAY_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    kPacket_.m_nFirstDiceNum = 0;
    kPacket_.m_nSecondDiceNum = 0;

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKOlympicManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_01, END_PROC );
    }

    // 도전 포인트가 있는지 확인
    if ( false == m_kOlympicUserData.CheckRemainTryPoint() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_03, END_PROC );
    }

    kPacket.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();
    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

    SiKOlympicManager()->GetOlympicDiceNum( kPacket_.m_nFirstDiceNum, kPacket_.m_nSecondDiceNum );
    kPacket_.m_nIncreasedData = kPacket_.m_nFirstDiceNum + kPacket_.m_nSecondDiceNum;

    // 점수 제한 체크.
    // 스크립트에 설정된 최대값에 대하여 보정처리를 하고 에러처리는 GameType이 없는 경우에 대해서만 처리.
    if( !SiKOlympicManager()->CheckGameResultData( kPacket_.m_nGameType, kPacket.m_nMyAccumulateData, kPacket_.m_nIncreasedData ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_02, END_PROC );
    }

    // 점수를 얻은 경우 누적 포인트를 DB에 갱신(diff 전달)
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_OLYMPIC_DICEPLAY_RESULT_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_02,           kPacket.m_nOK, 2 ); // 설정된 게임의 정보가 없음.
        ERR_CASE( ERR_OLYMPIC_03,           kPacket.m_nOK, 3 ); // 도전 포인트가 모자람.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 이벤트 주사위 게임결과 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"올림픽 이벤트 주사위 게임결과 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << BUILD_LOG( kPacket.m_nMyAccumulateData )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_OLYMPIC_DICEPLAY_RESULT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OLYMPIC_DICEPLAY_RESULT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_OLYMPIC_06 ) ) {
        goto END_PROC;
    }

    // 유저 데이터 갱신하고
    // 주사위 두개가 동일 눈이 나왔을 경우 기회를 한 번 더 준다
    if ( kPacket_.m_nFirstDiceNum != kPacket_.m_nSecondDiceNum ) {
        m_kOlympicUserData.DecreaseRemainTryCount( 1 );
    }
    m_kOlympicUserData.IncreaseMyAccumulateData( kPacket_.m_tmTodayDate, kPacket_.m_nIncreasedData );

    // 갱신된 데이터를 패킷에 저장
    kPacket_.m_nRemainTryCount = m_kOlympicUserData.GetRemainTryCount();
    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_OLYMPIC_05,           kPacket_.m_nOK, 5 ); // 올림픽 누적 포인트 DB 갱신 실패.
        ERR_CASE( ERR_OLYMPIC_06,           kPacket_.m_nOK, 6 ); // 올림픽 누적 포인트가 최대치임.
    default:
        START_LOG( cerr, L"올림픽 주사위 누적 포인트 갱신 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_DICEPLAY_RESULT_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 이벤트 주사위 게임결과 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nFirstDiceNum )
        << BUILD_LOG( kPacket_.m_nSecondDiceNum )
        << BUILD_LOG( kPacket_.m_nRemainTryCount)
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_nMyAccumulateData )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_OLYMPIC_TREEDROP_RESULT_REQ )
{
    KEVENT_OLYMPIC_TREEDROP_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKOlympicManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_01, END_PROC );
    }

    kPacket.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();
    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

    // 점수 제한 체크.
    // 스크립트에 설정된 최대값에 대하여 보정처리를 하고 에러처리는 GameType이 없는 경우에 대해서만 처리.
    if( !SiKOlympicManager()->CheckGameResultData( kPacket_.m_nGameType, kPacket.m_nMyAccumulateData, kPacket_.m_nIncreasedData ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_02, END_PROC );
    }

    // 점수를 얻은 경우 누적 포인트를 DB에 갱신(diff 전달)
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_OLYMPIC_TREEDROP_RESULT_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_02,           kPacket.m_nOK, 2 ); // 설정된 게임의 정보가 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 통나무깨기 게임결과 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"올림픽 이벤트 통나무깨기 게임결과 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << BUILD_LOG( kPacket.m_nIncreasedData )
        << BUILD_LOG( kPacket.m_nMyAccumulateData )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_OLYMPIC_TREEDROP_RESULT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OLYMPIC_TREEDROP_RESULT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) || IS_CORRECT( ERR_OLYMPIC_06 ) ) {
        goto END_PROC;
    }

    // 유저 데이터 갱신하고
    m_kOlympicUserData.IncreaseMyAccumulateData( kPacket_.m_tmTodayDate, kPacket_.m_nIncreasedData );

    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_05,           kPacket_.m_nOK, 5 ); // 올림픽 누적 포인트 DB 갱신 실패.
        ERR_CASE( ERR_OLYMPIC_06,           kPacket_.m_nOK, 6 ); // 올림픽 누적 포인트가 최대치임.
    default:
        START_LOG( cerr, L"올림픽 통나무깨기 누적 포인트 갱신 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_TREEDROP_RESULT_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 이벤트 통나무깨기 게임결과 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_nMyAccumulateData )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_OLYMPIC_BALLOON_RESULT_REQ )
{
    KEVENT_OLYMPIC_BALLOON_RESULT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKOlympicManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_01, END_PROC );
    }

    kPacket.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();
    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

    // 점수 제한 체크.
    // 스크립트에 설정된 최대값에 대하여 보정처리를 하고 에러처리는 GameType이 없는 경우에 대해서만 처리.
    if( !SiKOlympicManager()->CheckGameResultData( kPacket_.m_nGameType, kPacket.m_nMyAccumulateData, kPacket_.m_nIncreasedData ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_02, END_PROC );
    }

    // 점수를 얻은 경우 누적 포인트를 DB에 갱신(diff 전달)
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_OLYMPIC_BALLOON_RESULT_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_02,           kPacket.m_nOK, 2 ); // 설정된 게임의 정보가 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 풍선터뜨리기 게임결과 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"올림픽 이벤트 풍선터뜨리기 게임결과 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nGameType )
        << BUILD_LOG( kPacket_.m_nTeamType )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << BUILD_LOG( kPacket.m_nIncreasedData )
        << BUILD_LOG( kPacket.m_nMyAccumulateData )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_OLYMPIC_BALLOON_RESULT_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OLYMPIC_BALLOON_RESULT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) || IS_CORRECT( ERR_OLYMPIC_06 ) ) {
        goto END_PROC;
    }

    // 유저 데이터 갱신하고
    m_kOlympicUserData.IncreaseMyAccumulateData( kPacket_.m_tmTodayDate, kPacket_.m_nIncreasedData );

    kPacket_.m_nMyAccumulateData = m_kOlympicUserData.GetMyAccumulateData();

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_05,           kPacket_.m_nOK, 5 ); // 올림픽 누적 포인트 DB 갱신 실패.
        ERR_CASE( ERR_OLYMPIC_06,           kPacket_.m_nOK, 6 ); // 올림픽 누적 포인트가 최대치임.
    default:
        START_LOG( cerr, L"올림픽 풍선터뜨리기 누적 포인트 갱신 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_BALLOON_RESULT_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 이벤트 풍선터뜨리기 게임결과 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << BUILD_LOG( kPacket_.m_nMyAccumulateData )
        << BUILD_LOGtm( CTime( kPacket_.m_tmTodayDate ) )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_OLYMPIC_REWARD_REQ )
{
    KEVENT_OLYMPIC_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    DWORD dwMaxGrade = 0;

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKOlympicManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_OLYMPIC_01, END_PROC );
    }

    dwMaxGrade = SiKOlympicManager()->GetMaxGrade();

    if( !SiKOlympicManager()->CheckGradeEnable( kPacket_.m_dwGrade ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_13, END_PROC );
    }

    // 유저의 등급 정보 체크.
    if( !m_kOlympicUserData.CheckUserGrade( kPacket_.m_dwGrade, dwMaxGrade ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_13, END_PROC );
    }

    // 해당 등급의 보상 아이템 가져오기.
    if( !SiKOlympicManager()->GetGradeRewardList( kPacket_.m_dwGrade, kPacket_.m_vecReward ) ) {
        SET_ERR_GOTO( ERR_OLYMPIC_14, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_OLYMPIC_REWARD_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_08,           kPacket.m_nOK, 8 ); // 올림픽 게임참여 보상 아이템 DB지급 실패.
        ERR_CASE( ERR_OLYMPIC_13,           kPacket.m_nOK, 13 ); // 해당 등급의 보상 받을수 없음.
        ERR_CASE( ERR_OLYMPIC_14,           kPacket.m_nOK, 14 ); // 등급 보상 아이템 정보 가져오기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 보상 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"올림픽 보상 요청 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_OLYMPIC_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_OLYMPIC_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 유저 등급 정보 갱신.
    m_kOlympicUserData.UpdateUserGrade();
    kPacket_.m_dwGrade = m_kOlympicUserData.GetUserGrade();

    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_OLYMPIC_01,           kPacket_.m_nOK, 1 ); // 올림픽 이벤트 기간이 아님.
        ERR_CASE( ERR_OLYMPIC_08,           kPacket_.m_nOK, 8 ); // 올림픽 게임참여 보상 아이템 DB지급 실패.
        ERR_CASE( ERR_OLYMPIC_13,           kPacket_.m_nOK, 13 ); // 해당 등급의 보상 받을수 없음.
        ERR_CASE( ERR_OLYMPIC_14,           kPacket_.m_nOK, 14 ); // 등급 보상 아이템 정보 가져오기 실패.
        ERR_CASE( ERR_OLYMPIC_15,           kPacket_.m_nOK, 15 ); // 등급 보상 아이템 지급 DB기록 실패. )
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"올림픽 보상 요청 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_OLYMPIC_REWARD_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 보상 요청 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_dwGrade )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_OLYMPIC_BOSSGATE_RESULT_NOT )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) )
        goto END_PROC;

    // 유저 데이터 갱신하고
    m_kOlympicUserData.IncreaseMyAccumulateData( kPacket_.m_tmTodayDate, kPacket_.m_nIncreasedData );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_OLYMPIC_05,           kPacket_.m_nOK, 5 ); // 올림픽 누적 포인트 DB 갱신 실패.
    default:
        START_LOG( cerr, L"올림픽 보스게이트 누적 포인트 갱신 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"올림픽 보스게이트 게임결과 처리, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nIncreasedData )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_CHAR_LEVEL_REWARD_EVENT_NOT )
{
    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
    SEND_RECEIVED_PACKET( EVENT_CHAR_LEVEL_REWARD_EVENT_NOT );

    START_LOG( clog, L"캐릭터 레벨별 획득 아이템 전달" )
        << BUILD_LOG( kPacket_.m_setCharInfo.size() )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_GP_ATTRIBUTE_RANDOM_REQ )
{
    KEVENT_GP_ATTRIBUTE_RANDOM_ACK kPacket;
    KDB_EVENT_GP_ATTRIBUTE_RANDOM_REQ kDBPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkUseItem;
    KItemPtr pkInvenItem;

    std::vector< KAttributeInfo > vecAttributeList;
    std::vector< KAttributeInfo >::iterator vit;
    std::list<std::pair<int,float>> listRandomAttributes;
    std::list<std::pair<int,float>> listFullAttributes;
    std::list<std::pair<int,float>>::iterator lit;

    SET_ERROR( ERR_UNKNOWN );

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    if( !pkInvenItem  ) { // 랜덤 속성 선택한 아이템 존재하지 않음.
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_00, END_PROC );
    }

    // GP 랜덤 속성 재료 아이템 있어야 한다.
    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
    if( !pkUseItem ) { // 랜덤 속성 아이템이 존재하지 않음.
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_00, END_PROC );
    }

    kDBPacket.m_kUseItem = *pkUseItem;

    // 속성 랜덤 아이템인지 체크.
    if( !SiKItemManager()->m_kAttributeDecider.CheckRandomGP_AttributeItem( pkInvenItem->m_cGradeID, pkUseItem->m_ItemID ) ) {
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_04, END_PROC );
    }

    // 코디 아이템인지 체크.
    if( pkInvenItem->CheckItemType(KItem::TYPE_LOOK) ) {
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_02, END_PROC );
    }

    kDBPacket.m_ItemID = kPacket_.m_ItemID;
    kDBPacket.m_kItem = *pkInvenItem;

    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( pkInvenItem->m_ItemID );
    int nLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );

    // 레벨에 맞게 속성 변경해야 하는지 체크.
    int nItemLevel = 0;
    if( SiKItemManager()->m_kItemLevelDecider.IsSelectiveType( nLevelTableID ) ) {
        nItemLevel = static_cast<int>( pkInvenItem->m_sEquipLevel );
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_SELECTIVE );
    }

    int nAttributeCount = SiKItemManager()->GetAttributeCountFromGrade( pkInvenItem->m_cGradeID );
    // 랜덤 속성이 적용된 리스트 가져오기.
    SiKItemManager()->m_kAttributeDecider.GetSelectiveAttributeList( nAttributeTableID, nAttributeCount, (int)pkInvenItem->m_cGradeID, kDBPacket.m_listAttributeList );

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_GP_ATTRIBUTE_RANDOM_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_00, kPacket.m_nOK, 1 );   // 랜덤 속성을 적용할 아이템이 인벤에 없음.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_01, kPacket.m_nOK, 2 );   // 랜덤 속성 적용할 아이템이 에픽 등급 이상이 아님.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_02, kPacket.m_nOK, 3 );   // 랜덤 속성 적용할 아이템이 코디아이템임.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_03, kPacket.m_nOK, 4 );   // 랜덤 속성 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_04, kPacket.m_nOK, 5 );   // 랜덤 속성을 할수 있는 아이템이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GP_ATTRIBUTE_RANDOM_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"GP 아이템 랜덤 속성 적용 요청 MSG : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GP_ATTRIBUTE_RANDOM_ACK )
{
    KItemPtr pkInvenItem;
    KItemPtr pkUseItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_RANDOM_ATTRIBUTE_SELECT_05 ) ) {
        goto END_PROC;
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );
    if( !pkInvenItem ) {
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_00, END_PROC );
    }

    // 속성 초기화한 아이템 리스트.
    pkInvenItem->m_vecAttribute = kPacket_.m_kItem.m_vecAttribute;
    kPacket_.m_kItem = *pkInvenItem;

    // 사용한 속성 초기화 아이템 갯수 차감.
    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_kUseItem.m_ItemID );
    if( !pkUseItem ) {
        SET_ERR_GOTO( ERR_RANDOM_ATTRIBUTE_SELECT_00, END_PROC );
    }
    pkUseItem->m_nCount = kPacket_.m_kUseItem.m_nCount;
    pkUseItem->m_nInitCount = kPacket_.m_kUseItem.m_nCount;
    kPacket_.m_vecItem.push_back( *pkUseItem );

    if ( pkUseItem && pkUseItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kUseItem.m_ItemID, kPacket_.m_kUseItem.m_ItemUID );
    }


END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_00, kPacket_.m_nOK, 1 );   // 랜덤 속성을 적용할 아이템이 인벤에 없음.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_01, kPacket_.m_nOK, 2 );   // 랜덤 속성 적용할 아이템이 에픽 등급 이상이 아님.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_02, kPacket_.m_nOK, 3 );   // 랜덤 속성 적용할 아이템이 코디아이템임.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_03, kPacket_.m_nOK, 4 );   // 랜덤 속성 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_04, kPacket_.m_nOK, 5 );   // 랜덤 속성을 할수 있는 아이템이 아님.
        ERR_CASE( ERR_RANDOM_ATTRIBUTE_SELECT_05, kPacket_.m_nOK, 6 );   // 랜덤 속성 초기화 아이템 DB 동기화 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_GP_ATTRIBUTE_RANDOM_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"GP 랜덤 속성 적용 응답 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_vecAttribute.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_INIT_ITEM_GP_ATTRIBUTE_REQ )
{
    KEVENT_INIT_ITEM_GP_ATTRIBUTE_ACK kPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkUseItem;
    KItemPtr pkInvenItem;

    SET_ERROR( ERR_UNKNOWN );

    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_ItemID );
    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    if( !pkInvenItem || !pkUseItem )
    {
        SET_ERR_GOTO( ERR_ATTRIBUTE_INIT_01, END_PROC );
    }

    // 속성 초기화 아이템인지 체크.
    if( !SiKItemManager()->m_kAttributeDecider.CheckInitGP_AttributeItem( pkInvenItem->m_cGradeID, kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_INIT_05, END_PROC );
    }

    // 코디 아이템인지 체크.
    if( pkInvenItem->CheckItemType(KItem::TYPE_LOOK) ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_INIT_03, END_PROC );
    }

    kPacket_.m_kItem = *pkInvenItem;
    kPacket_.m_kUseItem = *pkUseItem;
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_INIT_ITEM_GP_ATTRIBUTE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_INIT_01, kPacket.m_nOK, 1 );   // 속성을 초기화 할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ATTRIBUTE_INIT_02, kPacket.m_nOK, 2 );   // 속성 초기화할 아이템이 에픽 등급 이상이 아님.
        ERR_CASE( ERR_ATTRIBUTE_INIT_03, kPacket.m_nOK, 3 );   // 속성 초기화할 아이템이 코디아이템임.
        ERR_CASE( ERR_ATTRIBUTE_INIT_04, kPacket.m_nOK, 4 );   // 속성초기화 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_ATTRIBUTE_INIT_05, kPacket.m_nOK, 5 );   // 속성 초기화를 할수 있는 아이템이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_INIT_ITEM_GP_ATTRIBUTE_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"속성 초기화 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_INIT_ITEM_GP_ATTRIBUTE_ACK )
{
    KItemPtr pkInvenItem;
    KItemPtr pkUseItem;
    std::list<std::pair<int,float>> listAttributes;
    std::list<std::pair<int,float>>::iterator lit;
    std::set<DWORD> setRandomAttributeIndex;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );
    pkUseItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_kUseItem.m_ItemID );
    if( !pkInvenItem || !pkUseItem ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_01, END_PROC );
    }

    // 속성 초기화한 아이템 리스트.
    pkInvenItem->m_vecAttribute = kPacket_.m_kItem.m_vecAttribute;
    kPacket_.m_kItem = *pkInvenItem;

    // 사용한 속성 초기화 아이템 갯수 차감.
    pkUseItem->m_nCount = kPacket_.m_kUseItem.m_nCount;
    pkUseItem->m_nInitCount = kPacket_.m_kUseItem.m_nCount;
    kPacket_.m_vecItem.push_back( *pkUseItem );

    if ( pkUseItem && pkUseItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_kUseItem.m_ItemID, kPacket_.m_kUseItem.m_ItemUID );
    }

    ////
    kPacket_.m_nSelectCount = 0;
    for( int i = 0 ; i < (int)pkInvenItem->m_vecAttribute.size() ; ++i )
    {
        if( pkInvenItem->m_vecAttribute[i].m_cState != KAttributeInfo::ATS_OPENED )
            continue;

        ++kPacket_.m_nSelectCount;
    }
    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( pkInvenItem->m_ItemID );
    int nLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );

    int nItemLevel = 0;
    if( SiKItemManager()->m_kItemLevelDecider.IsSelectiveType( nLevelTableID ) ) {
        nItemLevel = static_cast<int>( pkInvenItem->m_sEquipLevel );
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_SELECTIVE );
    }

    SiKItemManager()->m_kAttributeDecider.GetAttributeFullList( nAttributeTableID, (int)pkInvenItem->m_cGradeID, listAttributes );

    kPacket_.m_vecAttributeList;
    for( lit = listAttributes.begin() ; lit != listAttributes.end() ; ++lit )
    {
        KAttributeInfo kAttInfo;
        kAttInfo.m_cSlotID = 0;
        kAttInfo.m_cState = KAttributeInfo::ATS_OPENED;
        kAttInfo.m_cType = lit->first;
        kAttInfo.m_fValue = lit->second;

        kPacket_.m_vecAttributeList.push_back( kAttInfo );
    }

    // 랜덤 속성 리스트 전달.
    SiKItemManager()->m_kAttributeDecider.GetRandomAttributeIndex( nAttributeTableID, (int)pkInvenItem->m_cGradeID, setRandomAttributeIndex );
    kPacket_.m_setRandomAttributeIndex.swap( setRandomAttributeIndex );
    
    //
 
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_INIT_01, kPacket_.m_nOK, 1 );   // 속성을 초기화 할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ATTRIBUTE_INIT_02, kPacket_.m_nOK, 2 );   // 속성 초기화할 아이템이 에픽 등급 이상이 아님.
        ERR_CASE( ERR_ATTRIBUTE_INIT_03, kPacket_.m_nOK, 3 );   // 속성 초기화할 아이템이 코디아이템임.
        ERR_CASE( ERR_ATTRIBUTE_INIT_04, kPacket_.m_nOK, 4 );   // 속성초기화 DB에 기록 하는데 오류 발생.
        ERR_CASE( ERR_ATTRIBUTE_INIT_05, kPacket_.m_nOK, 5 );   // 속성 초기화를 할수 있는 아이템이 아님.
        ERR_CASE( ERR_ATTRIBUTE_INIT_06, kPacket_.m_nOK, 6 );
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_INIT_ITEM_GP_ATTRIBUTE_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"GP 속성 초기화 응답 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName)
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_vecAttribute.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ITEM_COMPOSE_REQ, std::vector<GCITEMUID> )
{
    KDB_EVENT_ITEM_COMPOSE_REQ kDBPacket;
    KEVENT_ITEM_COMPOSE_ACK kPacket;
    kPacket.m_nOK = -99;
    std::vector<GCITEMUID>::iterator vit;
    DWORD dwMaxMaterialListNum = SiKItemCompose()->GetMaxMaterialListNum();

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKItemCompose()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_ITEM_COMPOSE_01, END_PROC );
    }

    if( dwMaxMaterialListNum != kPacket_.size() ) {
        SET_ERR_GOTO( ERR_ITEM_COMPOSE_02, END_PROC );
    }

    for( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {
        KItemPtr pkItemComposeItem = m_kInventory.FindItemByItemUID( *vit );
        if( !pkItemComposeItem ) {
            SET_ERR_GOTO( ERR_ITEM_COMPOSE_03, END_PROC );
        }

        if( !SiKItemCompose()->CheckMaterialInfo( pkItemComposeItem->m_ItemID ) ) {
            SET_ERR_GOTO( ERR_ITEM_COMPOSE_04, END_PROC );
        }
        kDBPacket.m_vecMaterialID.push_back( pkItemComposeItem->m_ItemID );
    }

    if( !SiKItemCompose()->DoGamble( kDBPacket.m_kDropItemInfo ) ) {
        SET_ERR_GOTO( ERR_ITEM_COMPOSE_05, END_PROC );
    }

    kDBPacket.m_vecMaterialUID = kPacket_;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_ITEM_COMPOSE_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ITEM_COMPOSE_01,      kPacket.m_nOK, 1 ); //  아이템 합성 이벤트 기간이 아님.
        ERR_CASE( ERR_ITEM_COMPOSE_02,      kPacket.m_nOK, 2 ); //  재료 갯수가 서버 설정값이랑 틀림.
        ERR_CASE( ERR_ITEM_COMPOSE_03,      kPacket.m_nOK, 3 ); //  재료 아이템이 인벤토리에 존재하지 않음.
        ERR_CASE( ERR_ITEM_COMPOSE_04,      kPacket.m_nOK, 4 ); //  서버에서 설정된 재료 아이템이 아님.
        ERR_CASE( ERR_ITEM_COMPOSE_05,      kPacket.m_nOK, 5 ); //  보상 아이템 정보 가져오기 실패.
        ERR_CASE( ERR_ITEM_COMPOSE_06,      kPacket.m_nOK, 6 ); //  보상 아이템 DB 지급실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );
    default:
        START_LOG( cerr, L"아이템 합성 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"아이템 합성 요청 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kPacket_.size() )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_ITEM_COMPOSE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ITEM_COMPOSE_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    RemoveItemFromInven( kPacket_.m_vecMaterialInfo );
    m_kInventory.AddItemList( kPacket_.m_vecReward );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ITEM_COMPOSE_01,      kPacket_.m_nOK, 1 ); //  아이템 합성 이벤트 기간이 아님.
        ERR_CASE( ERR_ITEM_COMPOSE_02,      kPacket_.m_nOK, 2 ); //  재료 갯수가 서버 설정값이랑 틀림.
        ERR_CASE( ERR_ITEM_COMPOSE_03,      kPacket_.m_nOK, 3 ); //  재료 아이템이 인벤토리에 존재하지 않음.
        ERR_CASE( ERR_ITEM_COMPOSE_04,      kPacket_.m_nOK, 4 ); //  서버에서 설정된 재료 아이템이 아님.
        ERR_CASE( ERR_ITEM_COMPOSE_05,      kPacket_.m_nOK, 5 ); //  보상 아이템 정보 가져오기 실패.
        ERR_CASE( ERR_ITEM_COMPOSE_06,      kPacket_.m_nOK, 6 ); //  보상 아이템 DB 지급실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );
    default:
        START_LOG( cerr, L"아이템 합성 응답 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_ITEM_COMPOSE_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"아이템 합성 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_vecMaterialInfo.size() )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_GACHA_LOTTERY_ACTION_REQ, char )
{
    KEVENT_GACHA_LOTTERY_ACTION_ACK kPacket;
    KDB_EVENT_GACHA_LOTTERY_ACTION_REQ kDBPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkGachaLotteryItem;
    GCITEMID GachaLotteryItemID = SiKGachaLottery()->GetGachaLotteryItemID();

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKGachaLottery()->IsRun() ) {
        SET_ERR_GOTO( ERR_GACHA_LOTTERY_01, END_PROC );
    }

    pkGachaLotteryItem = m_kInventory.FindDurationItemByItemID( GachaLotteryItemID );
    if( !pkGachaLotteryItem ) {
        SET_ERR_GOTO( ERR_GACHA_LOTTERY_02, END_PROC );
    }

    if( !SiKGachaLottery()->DoGambleRewardType( kPacket_, kDBPacket.m_dwRewardType ) ) {
        SET_ERR_GOTO( ERR_GACHA_LOTTERY_03, END_PROC );
    }

    if( !SiKGachaLottery()->DoGambleItemInfo( kPacket_, kDBPacket.m_dwRewardType, kDBPacket.m_kRewardItem ) ) {
        SET_ERR_GOTO( ERR_GACHA_LOTTERY_04, END_PROC );
    }

    kDBPacket.m_cCharType = kPacket_;
    kDBPacket.m_DisplayItemID = kDBPacket.m_kRewardItem.m_ItemID;
    kDBPacket.m_kGachaLotteryItem = *pkGachaLotteryItem;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_GACHA_LOTTERY_ACTION_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GACHA_LOTTERY_01,     kPacket.m_nOK, 1 ); // 가챠복권 이벤트 기간이 아님.
        ERR_CASE( ERR_GACHA_LOTTERY_02,     kPacket.m_nOK, 2 ); // 가챠복권 아이템이 없음.
        ERR_CASE( ERR_GACHA_LOTTERY_03,     kPacket.m_nOK, 3 ); // 가챠복권 성공 실패 결과 처리 에러.
        ERR_CASE( ERR_GACHA_LOTTERY_04,     kPacket.m_nOK, 4 ); // 가챠복권 보상 아이템 가져오기 실패.
        ERR_CASE( ERR_GACHA_LOTTERY_05,     kPacket.m_nOK, 5 ); // 가챠복권 보상 아이템 DB 지급 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"가챠 복권 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"가챠 복권 요청 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOGc( kDBPacket.m_cCharType )
        << BUILD_LOG( kDBPacket.m_DisplayItemID )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_GACHA_LOTTERY_ACTION_ACK );
    }
}

IMPL_ON_FUNC( EVENT_GACHA_LOTTERY_ACTION_ACK )
{
    KItemPtr pkGachaLotteryItem;
    std::wstring wstrData;
    std::wstring wstrGrade;
    GCITEMID GachaLotteryItemID = SiKGachaLottery()->GetGachaLotteryItemID();

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkGachaLotteryItem = m_kInventory.FindDurationItemByItemID( GachaLotteryItemID );
    if( pkGachaLotteryItem ) {
        pkGachaLotteryItem->m_nCount = kPacket_.m_kUseLotteryItem.m_nCount;
        pkGachaLotteryItem->m_nInitCount = kPacket_.m_kUseLotteryItem.m_nCount;
        kPacket_.m_vecUseItem.push_back( *pkGachaLotteryItem );
    }

    m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

    // 갸차 복권 등급 보상 공지.
    if( SiKGachaLottery()->GetGachaLotteryNoticeEnable() ) {
        if( SiKGachaLottery()->CheckItemGrade( kPacket_.m_cCharType, kPacket_.m_DisplayItemID, wstrGrade ) ) {
            if( SiKLoudMessage()->GetGachaLotteryMsg( m_strNickName, wstrGrade, wstrData, GetLanguageCode() ) ) {
                SiKGachaLottery()->SendGachaLotteryMsg( wstrData );
            }
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_GACHA_LOTTERY_01,     kPacket_.m_nOK, 1 ); // 가챠복권 이벤트 기간이 아님.
        ERR_CASE( ERR_GACHA_LOTTERY_02,     kPacket_.m_nOK, 2 ); // 가챠복권 아이템이 없음.
        ERR_CASE( ERR_GACHA_LOTTERY_03,     kPacket_.m_nOK, 3 ); // 가챠복권 성공 실패 결과 처리 에러.
        ERR_CASE( ERR_GACHA_LOTTERY_04,     kPacket_.m_nOK, 4 ); // 가챠복권 보상 아이템 가져오기 실패.
        ERR_CASE( ERR_GACHA_LOTTERY_05,     kPacket_.m_nOK, 5 ); // 가챠복권 보상 아이템 DB 지급 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"가챠 복권 응답 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_GACHA_LOTTERY_ACTION_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"가챠 복권 처리 응답, User : " << GetName() )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwRewardType )
        << BUILD_LOG( kPacket_.m_DisplayItemID )
        << BUILD_LOG( kPacket_.m_kUseLotteryItem.m_nCount )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SKILL_OPENKEY_USE_REQ, int )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_SKILL_OPENKEY_USE_ACK kPacket;
    KDB_EVENT_SKILL_OPENKEY_USE_REQ kDBPacket;
    kPacket.m_nOK = -99;
    const KCharacterInfo* pkChar = NULL;
    SSkillElement kSkill;
    KChangeUserSkillBroad kChangeUserSkillBroad;
    KItemPtr pkSkillOpenKeyItem; // 스킬오픈 Key 아이템.
    GCITEMID SkillOpenKeyItemID = SiKSkillManager()->GetSkillOpenKeyInfo();

    pkSkillOpenKeyItem = m_kInventory.FindDurationItemByItemID( SkillOpenKeyItemID );

    if( !pkSkillOpenKeyItem ) {
        SET_ERR_GOTO( ERR_SKILL_32, END_PROC );
    }

    // 해당 스킬이 스킬 오픈 Key로 열리는 스킬인가? 4단필,MP4스킬만 오픈 가능.
    if( !SiKSkillManager()->IsSkillOpenKeyUseSkill( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_SKILL_33, END_PROC );
    }

    // 이 스킬이 존재 하는가?
    if( !SiKSkillManager()->GetSkill( kPacket_, kSkill ) ) {
        SET_ERR_GOTO( ERR_SKILL_00, END_PROC );
    }

    if( SiKSkillManager()->IsOldSkillTreeUseCharacter( kSkill.m_cCharType ) == false ) { // 스킬 시스템을 사용하는 캐릭터가 맞는지 확인
        SET_ERR_GOTO( ERR_SKILL_34, END_PROC );
    }

    // Lock Group 오픈이 안되었는가?
    if( kSkill.m_nLockGroupID >= 0 && IsLockedGroup( kSkill ) ) {
        SET_ERR_GOTO( ERR_SKILL_25, END_PROC );
    }

    // 이미 획득한 스킬인가?
    if( IsTrainedSkill( kSkill ) ) {
        SET_ERR_GOTO( ERR_SKILL_01, END_PROC );
    }

    // 해당 캐릭터가 존재 하는가?
    pkChar = GetCharacterInfo( kSkill.m_cCharType );
    if( !pkChar ) {
        SET_ERR_GOTO( ERR_SKILL_03, END_PROC );
    }

    // 전직은 만족하는가?
    if( pkChar->m_cPromotion < kSkill.m_cPromotion ) {
        SET_ERR_GOTO( ERR_SKILL_04, END_PROC );
    }

    // 해당 캐릭터 레벨이 만족 하는가?
    if( pkChar->m_dwLevel < (DWORD)kSkill.m_nCharLv ) {
        SET_ERR_GOTO( ERR_SKILL_05, END_PROC );
    }

    kDBPacket.m_nSkillID = kPacket_;
    kDBPacket.m_kUseItem = *pkSkillOpenKeyItem;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_SKILL_OPENKEY_USE_REQ, kDBPacket );

    // 스킬정보를 방원에게 전송해 줘야 하는지 체크.
    if( SiKSkillManager()->CheckAbilitySkill( kSkill.m_cCharType, kSkill.m_nID ) ) {
        kChangeUserSkillBroad.m_dwUserUID = GetUID();
        GetSkillInfoData( kSkill.m_cCharType, kChangeUserSkillBroad.m_vecTrainedSkill );
        if( m_pkRoom ) {
            m_pkRoom->UpdateSkillInfo( m_strName, kSkill.m_cCharType );

            LIF( m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_SKILL_UPDATE_BROAD, kChangeUserSkillBroad, true ) );
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );
        ERR_CASE( ERR_SKILL_00,         kPacket.m_nOK, 1 ); // 존재하지 않는 스킬입니다.
        ERR_CASE( ERR_SKILL_01,         kPacket.m_nOK, 2 ); // 이미 획득한 스킬 입니다.
        ERR_CASE( ERR_SKILL_03,         kPacket.m_nOK, 3 ); // 해당 캐릭터가 존재하지 않음.
        ERR_CASE( ERR_SKILL_04,         kPacket.m_nOK, 4 ); // 해당 전직을 만족하지 못함.
        ERR_CASE( ERR_SKILL_05,         kPacket.m_nOK, 5 ); // 레벨을 만족하지 못함.
        ERR_CASE( ERR_SKILL_25,         kPacket.m_nOK, 6 ); // Lock이 해제되지 않은 스킬을 배우려고 함.
        ERR_CASE( ERR_SKILL_32,         kPacket.m_nOK, 7 ); // 스킬 오픈키 아이템이 없음.
        ERR_CASE( ERR_SKILL_33,         kPacket.m_nOK, 8 ); // 스킬 오픈키로 오픈 가능한 스킬이 아님.
        ERR_CASE( ERR_SKILL_34,         kPacket.m_nOK, NetError::ERR_SKILL_34 );   // 이전 스킬시스템을 사용하는 캐릭터가 아님.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"스킬 오픈키 사용시 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"스킬 오픈키 사용요청 처리 요청, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_ )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_SKILL_OPENKEY_USE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_SKILL_OPENKEY_USE_ACK )
{
    KItemPtr pkSkillOpenKeyItem; // 스킬오픈 Key 아이템.
    GCITEMID SkillOpenKeyItemID = SiKSkillManager()->GetSkillOpenKeyInfo();
    std::vector<KItem>::iterator vit;
    SSkillElement kSkill;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 이 스킬이 존재 하는가?
    if( SiKSkillManager()->GetSkill( kPacket_.m_nSkillID, kSkill ) ) {
        // 해당 캐릭터 전직의 배운 스킬에 넣어 준다.
        InsertSkillInfo( kSkill );
    }

    pkSkillOpenKeyItem = m_kInventory.FindDurationItemByItemID( SkillOpenKeyItemID );
    if( pkSkillOpenKeyItem ) {
        if( !kPacket_.m_vecUseItem.empty() ) {
            vit = kPacket_.m_vecUseItem.begin();
            pkSkillOpenKeyItem->m_nCount = vit->m_nCount;
            pkSkillOpenKeyItem->m_nInitCount = vit->m_nCount;
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_SKILL_00,         kPacket_.m_nOK, 1 ); // 존재하지 않는 스킬입니다.
        ERR_CASE( ERR_SKILL_01,         kPacket_.m_nOK, 2 ); // 이미 획득한 스킬 입니다.
        ERR_CASE( ERR_SKILL_03,         kPacket_.m_nOK, 3 ); // 해당 캐릭터가 존재하지 않음.
        ERR_CASE( ERR_SKILL_04,         kPacket_.m_nOK, 4 ); // 해당 전직을 만족하지 못함.
        ERR_CASE( ERR_SKILL_05,         kPacket_.m_nOK, 5 ); // 레벨을 만족하지 못함.
        ERR_CASE( ERR_SKILL_25,         kPacket_.m_nOK, 6 ); // Lock이 해제되지 않은 스킬을 배우려고 함.
        ERR_CASE( ERR_SKILL_32,         kPacket_.m_nOK, 7 ); // 스킬 오픈키 아이템이 없음.
        ERR_CASE( ERR_SKILL_33,         kPacket_.m_nOK, 8 ); // 스킬 오픈키로 오픈 가능한 스킬이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"스킬 오픈키 사용처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_SKILL_OPENKEY_USE_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"스킬 오픈키 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nSkillID )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_SPECIFIC_ITEM_BREAKUP_INFO_REQ )
{
    KEVENT_SPECIFIC_ITEM_BREAKUP_INFO_ACK kPacket;

    SiKItemManager()->GetItemBreakupInfo( kPacket );

    SEND_COMPRESS_PACKET( EVENT_SPECIFIC_ITEM_BREAKUP_INFO_ACK, kPacket );

    START_LOG( clog, L"아이템 해체 정보 요청.. Login: " << GetName() )
        << BUILD_LOG( kPacket.m_fMaterialRatioMin )
        << BUILD_LOG( kPacket.m_fMaterialRatioMax )
        << BUILD_LOG( kPacket.m_mapMaterialItemID.size() )
        << BUILD_LOG( kPacket.m_mapMaterialInfo.size() )
        << BUILD_LOG( kPacket.m_mapSpecificBreakupInfo.size() )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_HERO_ITEM_UPGRADE_INFO_REQ )
{
    KEVENT_HERO_ITEM_UPGRADE_INFO_ACK kPacket;
    kPacket.clear();

    SiKHeroDungeonManager()->m_kHeroItemUpgradeManager.GetHeroItemUpgradeInfo( kPacket );

    SEND_PACKET( EVENT_HERO_ITEM_UPGRADE_INFO_ACK );

    START_LOG( clog, L"영웅던전 아이템 업그레이드 정보 전송 완료 Login : " << GetName() )
        << BUILD_LOG( kPacket.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_HERO_ITEM_UPGRADE_REQ )
{
    KHeroItemUpgradeInfo kUpgradeInfo;
    std::vector<KDropItemInfo> vecRequireMaterialList; // 스크립트 설정 된 재료 아이템 리스트
    std::vector<KDropItemInfo>::iterator vitRequireMaterial; // 사용 요청 한 재료 아이템과 비교 할 때 사용
    std::vector<GCITEMUID>::iterator vitRecvedReqMaterials; // 사용 하고 싶은 재료들 req받음
    KDropItemInfo kRewardInfo; // 보상으로 설정 될 아이템 정보
    KItemPtr pkStrengthItem;

    KDB_EVENT_HERO_ITEM_UPGRADE_REQ kDBPacket;
    KEVENT_HERO_ITEM_UPGRADE_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );

    // 영던 아이템 업그레이드 정보 얻기
    if ( false == SiKHeroDungeonManager()->m_kHeroItemUpgradeManager.GetUpgradeInfoByKey( kPacket_.m_prKey, kUpgradeInfo, kRewardInfo ) ) {
        SET_ERR_GOTO( ERR_HEROITEM_UPGRADE_01, END_PROC );
    }
    vecRequireMaterialList = kUpgradeInfo.m_vecMaterials;

    // 클라에서 받은 사용 요청 재료들 유효성 체크
    if ( false == kPacket_.m_vecMaterials.empty() ) {
        for ( vitRecvedReqMaterials = kPacket_.m_vecMaterials.begin() ; vitRecvedReqMaterials != kPacket_.m_vecMaterials.end() ; ++vitRecvedReqMaterials ) {
            KItemPtr pkItem = m_kInventory.FindItemByItemUID( *vitRecvedReqMaterials );
            if ( pkItem == NULL ) {
                START_LOG( cwarn, L"재료로 사용 요청 한 아이템이 인벤에 존재 하지 않음" )
                    << BUILD_LOG( *vitRecvedReqMaterials )
                    << END_LOG;

                SET_ERR_GOTO( ERR_HEROITEM_UPGRADE_02, END_PROC );
            }

            // 재료로 요청 한 아이템이 실제 재료 아이템이 맞는지 확인
            vitRequireMaterial = std::find_if( vecRequireMaterialList.begin(), vecRequireMaterialList.end(),
                boost::bind( &KDropItemInfo::m_ItemID, _1 ) == pkItem->m_ItemID );
            if ( vecRequireMaterialList.end() == vitRequireMaterial ) {
                // 요청한 아이템이 스크립트에는 설정 안돼있다! 고투 에러!!
                START_LOG( cwarn, L"재료로 사용 요청 한 아이템이 업그레이드에 필요 한 재료로 설정 되어 있지 않음" )
                    << BUILD_LOG( pkItem->m_ItemID )
                    << END_LOG;

                SET_ERR_GOTO( ERR_HEROITEM_UPGRADE_03, END_PROC );
            }

            if ( pkItem->m_EnchantEquipItemUID != 0 ) {
                pkStrengthItem = m_kInventory.FindItemByItemUID( pkItem->m_EnchantEquipItemUID );
                if( pkStrengthItem ) {
                    kDBPacket.m_vecStrengthItem.push_back( pkStrengthItem->m_ItemUID );
                }
            }

            if ( vitRequireMaterial->m_nDuration == KItem::UNLIMITED_ITEM ) {
                // 구매재료 영구아이템이 없음.
                if ( pkItem->m_nCount != KItem::UNLIMITED_ITEM ||
                    pkItem->m_nInitCount != KItem::UNLIMITED_ITEM ) {
                        // 구매 재료 영구아이템이 없음
                        SET_ERR_GOTO( ERR_HEROITEM_UPGRADE_04, END_PROC );
                }

                kDBPacket.m_vecDelMaterials.push_back( pkItem->m_ItemUID );
            }
            else {
                // 구매재료 수량아이템의 수량이 부족함.
                if ( pkItem->m_nCount < vitRequireMaterial->m_nDuration ) {
                    // 수량성 구매 재료(영웅 주화)의 수량이 부족함
                    SET_ERR_GOTO( ERR_HEROITEM_UPGRADE_05, END_PROC );
                }
                // 수량 아이템에 대해서만 넣자. (영웅 주화)
                kDBPacket.m_kItem = *pkItem;
                kDBPacket.m_nDecreaseCount = vitRequireMaterial->m_nDuration; // DB에서 감소 시킬 개수 설정 (스크립트 설정된걸로)
            }
        }
    } // if ( false == kPacket_.m_vecMaterials.empty() ) {
    else {
        SET_ERR_GOTO( ERR_HEROITEM_UPGRADE_08, END_PROC );
    }

    // 업그레이드 성공 시 삽입 할 아이템 설정
    kDBPacket.m_rewardItem = kRewardInfo;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_HERO_ITEM_UPGRADE_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,				   kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_HEROITEM_UPGRADE_01, kPacket.m_nOK, 1 ); // 키 값에 해당 하는 업그레이드 데이터가 없음.
        ERR_CASE( ERR_HEROITEM_UPGRADE_02, kPacket.m_nOK, 2 ); // 재료로 사용 요청한 아이템이 인벤에 없음.
        ERR_CASE( ERR_HEROITEM_UPGRADE_03, kPacket.m_nOK, 3 ); // 재료로 사용 요청한 아이템이 실제 재료 아이템이 아님.
        ERR_CASE( ERR_HEROITEM_UPGRADE_04, kPacket.m_nOK, 4 ); // 장비 재료 아이템이 없음.
        ERR_CASE( ERR_HEROITEM_UPGRADE_05, kPacket.m_nOK, 5 ); // 수량 재료 아이템 수량이 부족함.
        ERR_CASE( ERR_HEROITEM_UPGRADE_08, kPacket.m_nOK, 8 ); // 재료로 사용 요청한 리스트가 비었음.
        ERR_CASE( ERR_NOT_FINISHED_YET,	   kPacket.m_nOK, -98 ); // 아직 처리중인 작업
    default:
        START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    START_LOG( clog, L"영던 아이템 업그레이드 요청" )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_prKey.first )
        << BUILD_LOG( kPacket_.m_prKey.second )
        << BUILD_LOG( kPacket_.m_vecMaterials.size() )
        << END_LOG;

    if ( false == IS_CORRECT( NET_OK ) ) {
        SEND_PACKET( EVENT_HERO_ITEM_UPGRADE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_HERO_ITEM_UPGRADE_ACK )
{
    std::vector<GCITEMUID>::iterator vit;
    std::vector<KItem>::iterator vitCount;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_HEROITEM_UPGRADE_07 ) ) {
        goto END_PROC;
    }

    if ( !kPacket_.m_vecReward.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecReward );

        // 구매재료 영구아이템 제거
        for ( vit = kPacket_.m_vecDelMaterials.begin() ; vit != kPacket_.m_vecDelMaterials.end() ; ++vit ) {
            LIF( m_kInventory.RemoveItem( *vit ) );
        }

        for ( vitCount = kPacket_.m_vecDecreaseMaterial.begin() ; vitCount != kPacket_.m_vecDecreaseMaterial.end() ; ++vitCount ) {
            KItemPtr pkItem = m_kInventory.FindDurationItemByItemUID( vitCount->m_ItemUID );
            if ( !pkItem ) {
                START_LOG( cwarn, L"구매재료 수량아이템이 인벤에 없음. ItemID : " << vitCount->m_ItemID ) << END_LOG;
                continue;
            }

            // 구매재료 수량 아이템 삭감
            pkItem->m_nCount = std::max<int>( 0, vitCount->m_nCount );
            pkItem->m_nInitCount = std::max<int>( 0, vitCount->m_nCount );

            if ( pkItem->m_nCount <= 0 ) {
                m_kInventory.RemoveItem( pkItem->m_ItemID, pkItem->m_ItemUID );
            }
        }
    }

    if( !kPacket_.m_vecStrengthItem.empty() ) {
        for( vit = kPacket_.m_vecStrengthItem.begin() ; vit != kPacket_.m_vecStrengthItem.end() ; ++vit ) {
            LIF( m_kInventory.RemoveItem( *vit ) );
        }
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,				   kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_HEROITEM_UPGRADE_06, kPacket_.m_nOK, 6 ); // 구매재료 아이템 정보가 없음
        ERR_CASE( ERR_HEROITEM_UPGRADE_07, kPacket_.m_nOK, 7 ); // 구매재료 아이템이 없음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    START_LOG( clog, L"영던 아이템 업그레이드 요청 결과" )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket_.m_vecDecreaseMaterial.size() )
        << BUILD_LOG( kPacket_.m_vecDelMaterials.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_HERO_ITEM_UPGRADE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_USER_INFO_REQ )
{
    KEVENT_RITAS_CHRISTMAS_USER_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    kPacket.m_nStartCoinCount                               = m_kRitasChristmasUserData.m_nStartCoinCount;
    kPacket.m_tmStartCoinReceived                           = m_kRitasChristmasUserData.m_tmStartCoinReceived;
    kPacket.m_nContinueCoinCount                            = m_kRitasChristmasUserData.m_nContinueCoinCount;
    kPacket.m_nPremiumRewardCount                           = m_kRitasChristmasUserData.m_nPremiumRewardCount;
    kPacket.m_nMaxPoint                                     = m_kRitasChristmasUserData.m_nMaxPoint;

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 유저 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"리타의 크리스마스 이벤트 유저 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nStartCoinCount )
        << BUILD_LOG( kPacket.m_tmStartCoinReceived )
        << BUILD_LOG( kPacket.m_nContinueCoinCount )
        << BUILD_LOG( kPacket.m_nPremiumRewardCount )
        << BUILD_LOG( kPacket.m_nMaxPoint )
        << END_LOG;

    SEND_PACKET( EVENT_RITAS_CHRISTMAS_USER_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_USER_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 유저 객체에
    // 시작 코인 수, 시작 코인을 마지막으로 받은 시간, 재도전 코인 수, 프리미엄 상자 수, 오늘 최고 점수 갱신

    m_kRitasChristmasUserData.m_nCurrentStageNum            = 0;     // 게임 진행 중 사용
    m_kRitasChristmasUserData.m_nInitialStartCoinCount      = kPacket_.m_nStartCoinCount;
    m_kRitasChristmasUserData.m_nStartCoinCount             = kPacket_.m_nStartCoinCount;
    m_kRitasChristmasUserData.m_tmStartCoinReceived         = kPacket_.m_tmStartCoinReceived;
    m_kRitasChristmasUserData.m_nContinueCoinCount          = kPacket_.m_nContinueCoinCount;
    m_kRitasChristmasUserData.m_nSpecialRewardCount         = 0;    // 게임 진행 중 사용
    m_kRitasChristmasUserData.m_nInitialPremiumRewardCount  = kPacket_.m_nPremiumRewardCount;
    m_kRitasChristmasUserData.m_nPremiumRewardCount         = kPacket_.m_nPremiumRewardCount;
    m_kRitasChristmasUserData.m_nMaxPoint                   = kPacket_.m_nMaxPoint;
    m_kRitasChristmasUserData.m_bReceivedLastRankInfo       = false;    // 랭킹정보 갱신시 다시 채워짐

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket_.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 유저 정보 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_RITAS_CHRISTMAS_USER_INFO_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"리타의 크리스마스 이벤트 유저 정보 요청 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nStartCoinCount )
        << BUILD_LOGtm( CTime( kPacket_.m_tmStartCoinReceived ) )
        << BUILD_LOG( kPacket_.m_nContinueCoinCount )
        << BUILD_LOG( kPacket_.m_nPremiumRewardCount )
        << BUILD_LOG( kPacket_.m_nMaxPoint )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_PLAY_INFO_REQ )
{
    KEVENT_RITAS_CHRISTMAS_PLAY_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    SiKRitasChristmas()->GetPlayInfo( kPacket.m_kPlayInfo );
    SiKRitasChristmas()->GetObjectInfo( kPacket.m_mapObjectInfo );
    SiKRitasChristmas()->GetAllStageInfo( kPacket.m_mapStageInfo );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 플레이 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"리타의 크리스마스 이벤트 플레이 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kPlayInfo.m_nSlotNum )
        << BUILD_LOG( kPacket.m_kPlayInfo.m_nBasketSize )
        << BUILD_LOG( kPacket.m_kPlayInfo.m_fBasketWidth )
        << BUILD_LOG( kPacket.m_kPlayInfo.m_fPlayerSpeed )
        << BUILD_LOG( kPacket.m_kPlayInfo.m_fCatchZoneTop )
        << BUILD_LOG( kPacket.m_kPlayInfo.m_fCatchZoneBottom )
        << BUILD_LOG( kPacket.m_mapObjectInfo.size() )
        << BUILD_LOG( kPacket.m_mapStageInfo.size() )
        << END_LOG;

    SEND_PACKET( EVENT_RITAS_CHRISTMAS_PLAY_INFO_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_RANK_INFO_REQ )
{
    KEVENT_RITAS_CHRISTMAS_RANK_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    // DB에서 랭킹정보를 가져왔었는지 확인을 함
    if ( false == m_kRitasChristmasUserData.m_bReceivedLastRankInfo ) {
        QUEUEING_ID_TO_DB( EVENT_RITAS_CHRISTMAS_RANK_INFO_REQ );

        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_18, END_PROC );
    }

    // 하루가 지나지 않았을 경우에는 캐싱된 데이터를 전달함
    kPacket.m_vecRankInfo                       = m_kRitasChristmasUserData.m_vecRankInfo;

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_18,     kPacket.m_nOK, 18 ); // 랭킹데이터 최초 요청.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 랭킹 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 18, L"리타의 크리스마스 이벤트 랭킹 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecRankInfo.size() )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::ERR_RITAS_CHRISTMAS_ERROR_18 ) {
        SEND_PACKET( EVENT_RITAS_CHRISTMAS_RANK_INFO_ACK );
    }
}

IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_RANK_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 유저 객체에 랭킹정보 기록
    m_kRitasChristmasUserData.m_vecRankInfo             = kPacket_.m_vecRankInfo;
    m_kRitasChristmasUserData.m_bReceivedLastRankInfo   = true;

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket_.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 랭킹 정보 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_RITAS_CHRISTMAS_RANK_INFO_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"리타의 크리스마스 이벤트 랭킹 정보 요청 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecRankInfo.size() )
        << BUILD_LOG( m_kRitasChristmasUserData.m_bReceivedLastRankInfo )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_STAGE_START_REQ )
{
    KEVENT_RITAS_CHRISTMAS_STAGE_START_ACK kPacket;
    kPacket.m_nOK = -99;
    KRitasChristmasStageInfo kStageInfo;
    int nFirstStageNum = 0;
    int nNextStageNum = 0;
    int nStartCoinConsumeCount = 0;
    int nContinueCoinConsumeCount = 0;
    int nRemainContinueCoin = 0;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    // 요청한 스테이지 정보가 올바른지 확인
    // 해당 스테이지 정보가 있는지 확인
    if ( false == SiKRitasChristmas()->GetStageInfo( kPacket_.m_nStageNum, kStageInfo ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_09, END_PROC );
    }

    switch ( kPacket_.m_nStartType ) {
        // 처음부터 시작일 경우
        case KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ::ST_FIRST:
            // 시작 코인이 충분한지 확인
            if ( m_kRitasChristmasUserData.m_nStartCoinCount <= 0 ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_11, END_PROC );
            }

            if ( false == SiKRitasChristmas()->GetFirstStageNum( nFirstStageNum ) ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_09, END_PROC );
            }

            // 시작 코인 차감
            nStartCoinConsumeCount = SiKRitasChristmas()->GetCoinPerGame();
            m_kRitasChristmasUserData.m_nStartCoinCount = std::max< int >( 0, m_kRitasChristmasUserData.m_nStartCoinCount - nStartCoinConsumeCount );

            // 기타 정보 세팅
            m_kRitasChristmasUserData.m_nCurrentStageNum = nFirstStageNum;
            m_kRitasChristmasUserData.m_nCurrentPoint = 0;
            m_kRitasChristmasUserData.m_bContinuable = true;
            m_kRitasChristmasUserData.m_nSpecialRewardCount = 0;

            break;
        // 다음 스테이지일 경우
        case KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ::ST_NEXT:
            // 요청한 스테이지 번호가 올바른지 확인
            if ( false == SiKRitasChristmas()->GetNextStageNum( m_kRitasChristmasUserData.m_nCurrentStageNum, nNextStageNum ) ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_09, END_PROC );
            }

            if ( kPacket_.m_nStageNum != nNextStageNum ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_12, END_PROC );
            }

            m_kRitasChristmasUserData.m_nCurrentStageNum = nNextStageNum;

            break;
        // 재도전일 경우
        case KEVENT_RITAS_CHRISTMAS_STAGE_START_REQ::ST_RETRY:
            // 재도전 가능한지 확인
            if ( false == m_kRitasChristmasUserData.m_bContinuable ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_13, END_PROC );
            }

            // 재도전 코인이 충분한지 확인
            if ( m_kRitasChristmasUserData.m_nContinueCoinCount <= 0 ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_14, END_PROC );
            }

            // 요청한 스테이지 번호가 올바른지 확인
            if ( kPacket_.m_nStageNum != m_kRitasChristmasUserData.m_nCurrentStageNum ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_12, END_PROC );
            }

            if ( false == SiKRitasChristmas()->GetContinueCoinConsumeCount( kPacket_.m_nStageNum, nContinueCoinConsumeCount ) ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_09, END_PROC );
            }

            nRemainContinueCoin = m_kRitasChristmasUserData.m_nContinueCoinCount - nContinueCoinConsumeCount;
            if ( nRemainContinueCoin >= 0 ) {
                m_kRitasChristmasUserData.m_nContinueCoinCount = nRemainContinueCoin;

                // DB에 재도전 코인 차감요청. 음수 diff를 전달한다
                nContinueCoinConsumeCount = -nContinueCoinConsumeCount;
                QUEUEING_EVENT_TO_DB( DB_EVENT_RITAS_CHRISTMAS_UPDATE_CONTINUE_COIN_NOT, nContinueCoinConsumeCount );
            }
            else {
                m_kRitasChristmasUserData.m_nContinueCoinCount = 0;
            }

            m_kRitasChristmasUserData.m_bContinuable = false;

            break;

        default:
            SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_10, END_PROC );
            break;
    }

    kPacket.m_nCurrentStageNum      = m_kRitasChristmasUserData.m_nCurrentStageNum;
    kPacket.m_nCurrentPoint         = m_kRitasChristmasUserData.m_nCurrentPoint;
    kPacket.m_nStartCoinCount       = m_kRitasChristmasUserData.m_nStartCoinCount;

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_09,     kPacket.m_nOK, 9 ); // 스테이지 정보가 없음.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_10,     kPacket.m_nOK, 10 ); // 유효하지 않은 시작타입.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_11,     kPacket.m_nOK, 11 ); // 시작 코인이 충분하지 않음.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_12,     kPacket.m_nOK, 12 ); // 요청한 스테이지가 다음 스테이지가 아님.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_13,     kPacket.m_nOK, 13 ); // 재도전을 할 수 없는 상태임.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_14,     kPacket.m_nOK, 14 ); // 재도전 코인이 충분하지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 스테이지 시작 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"리타의 크리스마스 이벤트 스테이지 시작 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nCurrentStageNum )
        << BUILD_LOG( kPacket.m_nStartCoinCount )
        << BUILD_LOG( kPacket_.m_nStartType )
        << BUILD_LOG( kPacket_.m_nStageNum )
        << END_LOG;

    SEND_PACKET( EVENT_RITAS_CHRISTMAS_STAGE_START_ACK );
}

IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_EMPTY_BASKET_REQ )
{
    KEVENT_RITAS_CHRISTMAS_EMPTY_BASKET_ACK kPacket;
    kPacket.m_nOK = -99;
    int nGetPoint = 0;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    // 받은 오브젝트 수가 바구니 최대 사이즈보다 큰지 확인하기
    if ( false == CheckRitasChristmasBasketSize( kPacket_.size() ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_15, END_PROC );
    }

    // 점수 계산하기
    if ( false == CalcRitasChristmasPoint( kPacket_, nGetPoint ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_16, END_PROC );
    }

    // 유저 객체에 얻은 점수 갱신하기
    m_kRitasChristmasUserData.m_nCurrentPoint = std::max< int >( 0, m_kRitasChristmasUserData.m_nCurrentPoint + nGetPoint );

    // 결과 패킷 세팅 후 전달하기
    kPacket.m_nCurrentPoint = m_kRitasChristmasUserData.m_nCurrentPoint;

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_15,     kPacket.m_nOK, 15 ); // 바구니에 담긴 오브젝트 수가 최대 바구니 크기보다 큼.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_16,     kPacket.m_nOK, 16 ); // 오브젝트 정보가 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 바구니 비우기 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"리타의 크리스마스 이벤트 바구니 비우기 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nCurrentPoint )
        << BUILD_LOG( kPacket_.size() )
        << END_LOG;

    SEND_PACKET( EVENT_RITAS_CHRISTMAS_EMPTY_BASKET_ACK );
}

IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_STAGE_END_REQ )
{
    KEVENT_RITAS_CHRISTMAS_STAGE_END_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_bGotNewRecord = false;
    kPacket.m_bClearLastStage = false;
    int nGetBonusPoint = 0;
    int nGetPremiumRewardCount = 0;
    KRitasChristmasStageInfo kStageInfo;
    int nDiffPoint = 0;
    int nNextStageNum = 0;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    // 요청한 스테이지 정보가 올바른지 확인
    // 해당 스테이지 정보가 있는지 확인
    if ( false == SiKRitasChristmas()->GetStageInfo( kPacket_.m_nStageNum, kStageInfo ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_09, END_PROC );
    }

    // 획득 점수 계산
    // 남은 게임 시간을 받아서 추가 점수 계산하기
    nGetBonusPoint = static_cast< int >( kPacket_.m_fRemainTime * kStageInfo.m_nRemainTimeBonusPointPerSec );

    // 피버 오브젝트 얻은 수를 받아서 추가 점수 계산하고 프리미엄 상자 수 계산하기
    CalcRitasChristmasFeverBonusPoint( kPacket_.m_nGetFeverObjectCount, nGetBonusPoint, nGetPremiumRewardCount );

    // 유저 객체에 스테이지 클리어 보너스 점수 더하기, 프리미엄 상자 수 더하기
    m_kRitasChristmasUserData.m_nCurrentPoint = std::max< int >( 0, m_kRitasChristmasUserData.m_nCurrentPoint + nGetBonusPoint );
    m_kRitasChristmasUserData.m_nPremiumRewardCount = std::max< int >( 0, m_kRitasChristmasUserData.m_nPremiumRewardCount + nGetPremiumRewardCount );

    // 종료 타입별 처리
    switch ( kPacket_.m_nEndType ) {
        case KEVENT_RITAS_CHRISTMAS_STAGE_END_REQ::ET_STAGE_CLEAR:
            // 스테이지 클리어인 경우
            // 스페셜 보상 추가하기
            m_kRitasChristmasUserData.m_nSpecialRewardCount = std::max< int >( 0, m_kRitasChristmasUserData.m_nSpecialRewardCount + kStageInfo.m_nClearRewardCount );

            // 마지막 스테이지를 클리어 했는지 확인하여 알려준다
            if ( false == SiKRitasChristmas()->GetNextStageNum( kPacket_.m_nStageNum, nNextStageNum ) ) {
                kPacket.m_bClearLastStage = true;
            }

            break;
        case KEVENT_RITAS_CHRISTMAS_STAGE_END_REQ::ET_GAME_OVER:
            // 게임 오버인 경우
            // 최고 점수를 갱신했을 만 DB에 기록 갱신 요청
            nDiffPoint = m_kRitasChristmasUserData.m_nCurrentPoint - m_kRitasChristmasUserData.m_nMaxPoint;

            if ( nDiffPoint > 0 ) {
                QUEUEING_EVENT_TO_DB( DB_EVENT_RITAS_CHRISTMAS_UPDATE_RECORD_NOT, nDiffPoint );

                m_kRitasChristmasUserData.m_nMaxPoint = m_kRitasChristmasUserData.m_nCurrentPoint;

                // 최고 점수를 갱신했다고 전달
                kPacket.m_bGotNewRecord = true;
            }

            break;
        default:
            SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_17, END_PROC );
            break;
    }

    // 현재 점수, 시작 코인 수, 재도전 코인 수, 이어하기 가능 여부, 스페셜 상자 수, 프리미엄 상자 수, 오늘 내 최고 점수 전달
    kPacket.m_nCurrentPoint         = m_kRitasChristmasUserData.m_nCurrentPoint;
    kPacket.m_nStartCoinCount       = m_kRitasChristmasUserData.m_nStartCoinCount;
    kPacket.m_nContinueCoinCount    = m_kRitasChristmasUserData.m_nContinueCoinCount;
    kPacket.m_bContinuable          = m_kRitasChristmasUserData.m_bContinuable;
    kPacket.m_nSpecialRewardCount   = m_kRitasChristmasUserData.m_nSpecialRewardCount;
    kPacket.m_nPremiumRewardCount   = m_kRitasChristmasUserData.m_nPremiumRewardCount;
    kPacket.m_nMaxPoint             = m_kRitasChristmasUserData.m_nMaxPoint;

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_09,     kPacket.m_nOK, 9 ); // 스테이지 정보가 없음.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_17,     kPacket.m_nOK, 17 ); // 유효하지 않은 종료타입.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 스테이지 종료 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"리타의 크리스마스 이벤트 스테이지 종료 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nCurrentPoint )
        << BUILD_LOG( kPacket.m_nStartCoinCount )
        << BUILD_LOG( kPacket.m_nContinueCoinCount )
        << BUILD_LOG( kPacket.m_bContinuable )
        << BUILD_LOG( kPacket.m_nSpecialRewardCount )
        << BUILD_LOG( kPacket.m_nPremiumRewardCount )
        << BUILD_LOG( kPacket.m_nMaxPoint )
        << BUILD_LOG( kPacket.m_bGotNewRecord )
        << BUILD_LOG( kPacket.m_bClearLastStage )
        << BUILD_LOG( kPacket_.m_nEndType )
        << BUILD_LOG( kPacket_.m_nStageNum )
        << BUILD_LOG( kPacket_.m_fRemainTime )
        << BUILD_LOG( kPacket_.m_nGetFeverObjectCount )
        << BUILD_LOG( nGetBonusPoint )
        << END_LOG;

    SEND_PACKET( EVENT_RITAS_CHRISTMAS_STAGE_END_ACK );
}

IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_GET_REWARD_REQ )
{
    KEVENT_RITAS_CHRISTMAS_GET_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    Kairo kReward;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    kPacket.m_nAfterSpecialRewardCount = m_kRitasChristmasUserData.m_nSpecialRewardCount;
    kPacket.m_nAfterPremiumRewardCount = m_kRitasChristmasUserData.m_nPremiumRewardCount;

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    // 상자 타입이 유효한지 확인
    if ( KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_SPECIAL != kPacket_.m_nOpenBoxType
        && KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_PREMIUM != kPacket_.m_nOpenBoxType ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_04, END_PROC );
    }

    // 상자 구성품 정보가 서버에 유효한지 확인
    if ( false == SiKRitasChristmas()->IsRegisteredRewardInfo() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_05, END_PROC );
    }

    GCITEMID dwPremiumKeyItemID = SiKRitasChristmas()->GetPremiumKeyItemID();
    switch ( kPacket_.m_nOpenBoxType ) {
        // 스페셜 상자 타입일 경우
        case KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_SPECIAL:
            // 스페셜 상자 수량이 남아있는지 확인
            if ( m_kRitasChristmasUserData.m_nSpecialRewardCount <= 0 ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_06, END_PROC );
            }

            break;
        // 프리미엄 상자 타입일 경우
        case KEVENT_RITAS_CHRISTMAS_GET_REWARD_REQ::BT_PREMIUM:
            // 프리미엄 상자 수량이 남아있는지 확인
            if ( m_kRitasChristmasUserData.m_nPremiumRewardCount <= 0 ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_06, END_PROC );
            }
            // 열쇠 아이템을 보유하고 있는지 확인
            pkItem = m_kInventory.FindDurationItemByItemID( dwPremiumKeyItemID );
            if ( NULL == pkItem || pkItem->m_nCount <= 0 ) {
                SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_07, END_PROC );
            }

            kPacket_.m_kPremiumKey = *pkItem;

            break;
        default:
            SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_04, END_PROC );

            break;
    }

    // 상자 정보 가져와서
    if ( false == SiKRitasChristmas()->GetRewardKairo( kPacket_.m_nOpenBoxType, kReward ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_04, END_PROC );
    }

    // 상자 구성품 뽑기
    if ( false == GetRitasChristmasReward( kReward, kPacket_.m_kAddItem ) ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_08, END_PROC );
    }

    kPacket_.m_nBeforeSpecialRewardCount = m_kRitasChristmasUserData.m_nSpecialRewardCount;
    kPacket_.m_nBeforePremiumRewardCount = m_kRitasChristmasUserData.m_nPremiumRewardCount;

    // 뽑힌 상자 구성품 아이템 추가와 열쇠 아이템 수량 감소 DB 요청
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_RITAS_CHRISTMAS_GET_REWARD_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_04,     kPacket.m_nOK, 4 ); // 유효하지 않은 상자 타입.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_05,     kPacket.m_nOK, 5 ); // 보상 상자 정보가 없음.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_06,     kPacket.m_nOK, 6 ); // 상자 수량이 부족함.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_07,     kPacket.m_nOK, 7 ); // 프리미엄 키 아이템이 부족함.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_08,     kPacket.m_nOK, 8 ); // 상자를 열었는데 꽝이 나옴.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 보상 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"리타의 크리스마스 이벤트 보상 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nAfterSpecialRewardCount )
        << BUILD_LOG( kPacket.m_nAfterPremiumRewardCount )
        << BUILD_LOG( kPacket_.m_nOpenBoxType )
        << BUILD_LOG( kPacket_.m_kPremiumKey.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPremiumKey.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kPremiumKey.m_nCount )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_RITAS_CHRISTMAS_GET_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_GET_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    KItemPtr pkPremiumKeyItem;

    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_RITAS_CHRISTMAS_ERROR_03 ) ) {
        goto END_PROC;
    }

    // 남은 스페셜 상자 수, 프리미엄 상자 수, 프리미엄 키 아이템 수량, 상자에서 얻은 아이템 정보를 DB 스레드에서 넘겨받았음
    // 스페셜 상자 수 갱신
    m_kRitasChristmasUserData.m_nSpecialRewardCount = kPacket_.m_nAfterSpecialRewardCount;

    // 프리미엄 상자 수 갱신
    m_kRitasChristmasUserData.m_nPremiumRewardCount = kPacket_.m_nAfterPremiumRewardCount;

    // 프리미엄 키 아이템 수량 갱신
    pkPremiumKeyItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kPremiumKey.m_ItemUID );
    if( NULL != pkPremiumKeyItem ) {
        pkPremiumKeyItem->m_nCount = kPacket_.m_kPremiumKey.m_nCount;
        pkPremiumKeyItem->m_nInitCount = kPacket_.m_kPremiumKey.m_nCount;

        if ( pkPremiumKeyItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( pkPremiumKeyItem->m_ItemUID );
        }
    }

    // 상자에서 얻은 아이템 정보 갱신
    m_kInventory.AddItemList( kPacket_.m_vecGetItems );

    // 남은 시작 코인 수, 남은 재도전 코인 수 알림
    kPacket_.m_nStartCoinCount = m_kRitasChristmasUserData.m_nStartCoinCount;
    kPacket_.m_nContinueCoinCount = m_kRitasChristmasUserData.m_nContinueCoinCount;

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_02,     kPacket_.m_nOK, 2 ); // 보상 아이템 얻기 DB 작업 실패.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_03,     kPacket_.m_nOK, 3 ); // 프리미엄 키 아이템 DB 동기화 실패.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_04,     kPacket_.m_nOK, 4 ); // 유효하지 않은 상자 타입.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 보상 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_RITAS_CHRISTMAS_GET_REWARD_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"리타의 크리스마스 이벤트 보상 요청 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nAfterSpecialRewardCount )
        << BUILD_LOG( kPacket_.m_nAfterPremiumRewardCount )
        << BUILD_LOG( kPacket_.m_kPremiumKey.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPremiumKey.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kPremiumKey.m_nCount )
        << BUILD_LOG( kPacket_.m_vecGetItems.size() )
        << BUILD_LOG( kPacket_.m_nStartCoinCount )
        << BUILD_LOG( kPacket_.m_nContinueCoinCount )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_REQ )
{
    KEVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK kPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkItem;
    KItem kExchangeItem;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    // 교환 아이템을 갖고 있는지 확인
    pkItem = m_kInventory.FindDurationItemByItemID( SiKRitasChristmas()->GetContinueCoinExchangeItemID() );
    if ( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_19, END_PROC );
    }

    kExchangeItem = *pkItem;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_REQ, kExchangeItem );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_19,     kPacket.m_nOK, 19 ); // 도전 코인 교환 아이템이 없음.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_20,     kPacket.m_nOK, 20 ); // 재도전 코인 교환 아이템의 수량이 부족함(DB).
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_21,     kPacket.m_nOK, 21 ); // 재도전 코인 교환 중 재도전 코인(포인트) DB 갱신 실패.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_22,     kPacket.m_nOK, 22 ); // 재도전 코인 교환 중 교환 아이템의 수량 DB 갱신 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 재도전 코인 충전 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"리타의 크리스마스 이벤트 재도전 코인 충전 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kExchangeItem.m_ItemID )
        << BUILD_LOG( kExchangeItem.m_nCount )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK );
    }
}

IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    KItemPtr pkExchangeConsumeItem;

    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    // 재도전 코인 수 갱신
    m_kRitasChristmasUserData.m_nContinueCoinCount += kPacket_.m_nAfterContinueCoinCount;

    kPacket_.m_nAfterContinueCoinCount = m_kRitasChristmasUserData.m_nContinueCoinCount;

    // 재도전 코인 교환 아이템 수량 갱신
    pkExchangeConsumeItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kConsumeItem.m_ItemUID );
    if( NULL == pkExchangeConsumeItem ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_19, END_PROC );
    }
    pkExchangeConsumeItem->m_nCount = kPacket_.m_kConsumeItem.m_nCount;
    pkExchangeConsumeItem->m_nInitCount = kPacket_.m_kConsumeItem.m_nCount;

    if ( pkExchangeConsumeItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( pkExchangeConsumeItem->m_ItemUID );
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_19,     kPacket_.m_nOK, 19 ); // 도전 코인 교환 아이템이 없음.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_20,     kPacket_.m_nOK, 20 ); // 재도전 코인 교환 아이템의 수량이 부족함(DB).
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_21,     kPacket_.m_nOK, 21 ); // 재도전 코인 교환 중 재도전 코인(포인트) DB 갱신 실패.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_22,     kPacket_.m_nOK, 22 ); // 재도전 코인 교환 중 교환 아이템의 수량 DB 갱신 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 재도전 코인 충전 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_RITAS_CHRISTMAS_EXCHANGE_CONTINUE_COIN_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"리타의 크리스마스 이벤트 재도전 코인 충전 요청 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nAfterContinueCoinCount )
        << BUILD_LOG( kPacket_.m_kConsumeItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kConsumeItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kConsumeItem.m_nCount )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_RITAS_CHRISTMAS_RANK_UPDATE_TIME_CHANGED_NOT, bool )
{
    // 랭킹 정보를 DB에서 새로 받을 수 있도록 마지막 랭킹 정보를 가져왔는지 여부를 설정 한다
    m_kRitasChristmasUserData.m_bReceivedLastRankInfo = kPacket_;

    START_LOG( clog, L"리타의 크리스마스 이벤트 마지막 랭킹 정보를 가져왔는지 여부 설정, User :" << GetName() )
        << BUILD_LOG( kPacket_ )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_RITAS_CHRISTMAS_REWARD_VIEW_INFO_REQ )
{
    KEVENT_RITAS_CHRISTMAS_REWARD_VIEW_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_RITAS_CHRISTMAS_ERROR_01, END_PROC );
    }

    SiKRitasChristmas()->GetRewardViewListInfo( kPacket.m_vecRewardViewListInfo );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_RITAS_CHRISTMAS_ERROR_01,     kPacket.m_nOK, 1 ); // 리타의 크리스마스 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"리타의 크리스마스 이벤트 안내용 아이템 리스트 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"리타의 크리스마스 이벤트 안내용 아이템 리스트 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecRewardViewListInfo.size() )
        << END_LOG;

    SEND_PACKET( EVENT_RITAS_CHRISTMAS_REWARD_VIEW_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_UPDATE_ITEM_COUNT_NOT, std::vector< KItem > )
{
    BOOST_FOREACH( KItem& kItem, kPacket_ ) {
        m_kInventory.RemoveItem( kItem.m_ItemID, kItem.m_ItemUID );
    }

    START_LOG( clog, L"아이템 수량 동기화 처리, User : " << GetName() )
        << BUILD_LOG( kPacket_.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_UPDATE_ITEM_COUNT_NOT );
}

IMPL_ON_FUNC( EVENT_BUFF_DONATION_LOGIN_INFO_NOT )
{
    SiKBuffDonation()->GetDonationDataForFirstLogin( kPacket_ );

    // DB에서 얻은 값이 없으면 기부 내역 없음으로 설정
    if ( 0 == kPacket_.m_biTotalDonationPoint ) {
        SetDonationBuffEnable( false );
    }
    else {
        SetDonationBuffEnable( true );
    }

    // 발렌타인 버프 기부 이벤트 정보 전달
    SEND_RECEIVED_PACKET( EVENT_BUFF_DONATION_LOGIN_INFO_NOT );

    START_LOG( clog, L"접속 시 발렌타인 버프 기부 데이터 전달" )
        << BUILD_LOG( kPacket_.m_dwDonationItemID )
        << BUILD_LOG( kPacket_.m_biTotalDonationPoint )
        << BUILD_LOG( kPacket_.m_nBuffStateFlag )
        << BUILD_LOG( kPacket_.m_biActiveBuffPoint )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_BUFF_DONATION_TOTAL_DATA_NOT )
{
    // centerproxy에서 여기까지 왔으면 버프 On/Off 둘 중 하나임
    // 기부자에 한해서만 버프를 걸어줄지 확인
    if ( FLAG_BUFF_ON == kPacket_.m_nBuffStateFlag && true == SiKBuffDonation()->IsUseIndividualBuffSystem() ) {
        // 기부를 한번이라도 해서 버프 걸어줘야 하는 유저인가
        if ( true == GetDonationBuffEnable() ) { // 센터에서는 일단 전체로 가정하고 ON으로 세팅해서 NOT보냄
            kPacket_.m_nBuffStateFlag = FLAG_BUFF_ON;
        }
        else {
            kPacket_.m_nBuffStateFlag = FLAG_BUFF_OFF;
        }
    }

    // 기부 데이터 갱신 통지
    SEND_RECEIVED_PACKET( EVENT_BUFF_DONATION_TOTAL_DATA_NOT );

    START_LOG( clog, L"버프 기부데이터 갱신 패킷 전송 완료" )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_biTotalDonationPoint )
        << BUILD_LOG( kPacket_.m_nBuffStateFlag )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_BUFF_DONATION_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KDB_EVENT_BUFF_DONATION_REQ kDBPacket;
    KEVENT_BUFF_DONATION_ACK kPacket;
    KItemPtr pkItem;

    // 이벤트 기간인가?
    if( false == SiKBuffDonation()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_BUFF_DONATION_ERROR_01, END_PROC );
    }

    // 기부로 사용 요청 한 아이템이 실제 기부 아이템인지 확인
    if ( false == SiKBuffDonation()->CheckDonationItemValidation( kPacket_.m_dwDonationItemID ) ) {
        SET_ERR_GOTO( ERR_BUFF_DONATION_ERROR_02, END_PROC );
    }

    // 인벤에 기부 아이템이 내 인벤에 존재 하는지
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_dwDonationItemID );
    if( pkItem == NULL ) {
        SET_ERR_GOTO( ERR_BUFF_DONATION_ERROR_03, END_PROC );
    }

    // 요청 수량만큼 인벤에 가지고 있나
    if ( pkItem->m_nCount < kPacket_.m_nItemCount ) {
        SET_ERR_GOTO( ERR_BUFF_DONATION_ERROR_04, END_PROC );
    }

    // 기부 했는지 여부 확인 (DB에서 기록해주기 위함)
    kDBPacket.m_bFlag = GetDonationBuffEnable();

    // DB패킷 세팅
    kDBPacket.m_kItem = *pkItem;
    SiKBuffDonation()->GetRewardItemList( kDBPacket.m_vecReward );
    kDBPacket.m_nItemCount = kPacket_.m_nItemCount;

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_BUFF_DONATION_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                         kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_BUFF_DONATION_ERROR_01,     kPacket.m_nOK, NetError::ERR_BUFF_DONATION_ERROR_01 ); // 발렌타인 기부 이벤트 기간이 아님.
        ERR_CASE( ERR_BUFF_DONATION_ERROR_02,     kPacket.m_nOK, NetError::ERR_BUFF_DONATION_ERROR_02 ); // 기부하려고 하는 아이템이 실제 기부 아이템이 아님.
        ERR_CASE( ERR_BUFF_DONATION_ERROR_03,     kPacket.m_nOK, NetError::ERR_BUFF_DONATION_ERROR_03 ); // 기부 하려는 아이템이 내 인벤에 없음.
        ERR_CASE( ERR_BUFF_DONATION_ERROR_04,     kPacket.m_nOK, NetError::ERR_BUFF_DONATION_ERROR_04 ); // 기부 요청한 수량만큼 인벤토리에 보유하고 있지 않음.
    default:
        START_LOG( cerr, L"발렌타인 버프 기부 요청 시 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    START_LOG( clog, L"발렌타인 버프 기부 이벤트 요청 처리 결과 User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwDonationItemID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( kDBPacket.m_vecReward.size() )
        << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_BUFF_DONATION_ACK );
    }
}

// 기부 응답
IMPL_ON_FUNC( EVENT_BUFF_DONATION_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_BUFF_DONATION_ERROR_06 ) ) {
        SiKBuffDonation()->GetTotalDonationPoint( kPacket_.m_biTotalDonationPoint );
        goto END_PROC;
    }

    if ( !kPacket_.m_vecRewardItem.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecRewardItem );

        KItemPtr pkItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kDonationItem.m_ItemUID );
        if ( !pkItem ) {
            START_LOG( cwarn, L"발렌타인 버프 기부 아이템이 인벤에 없음. ItemID : " << kPacket_.m_kDonationItem.m_ItemID ) << END_LOG;
            goto END_PROC;
        }

        // 기부에 사용 된 아이템 차감
        pkItem->m_nCount = std::max<int>( 0, kPacket_.m_kDonationItem.m_nCount );
        pkItem->m_nInitCount = std::max<int>( 0, kPacket_.m_kDonationItem.m_nCount );

        if ( pkItem->m_nCount <= 0 ) {
            m_kInventory.RemoveItem( pkItem->m_ItemID, pkItem->m_ItemUID );
        }
    }

    // 나는 기부를 한번이라도 했다
    if ( false == GetDonationBuffEnable() ) {
        SetDonationBuffEnable( true );
    }

    // 발렌타인기부 객체의 total값 update함수 호출
    SiKBuffDonation()->UpdateDonationPointDiffReqToCenter( kPacket_.m_nItemCount );
    SiKBuffDonation()->GetTotalDonationPoint( kPacket_.m_biTotalDonationPoint );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                     kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_BUFF_DONATION_ERROR_05, kPacket_.m_nOK, NetError::ERR_BUFF_DONATION_ERROR_05 ); // 기부 후 교환 받는 아이템 지급 실패.
        ERR_CASE( ERR_BUFF_DONATION_ERROR_06, kPacket_.m_nOK, NetError::ERR_BUFF_DONATION_ERROR_06 ); // 기부에 사용 된 아이템 차감 실패.
        ERR_CASE( ERR_BUFF_DONATION_ERROR_07, kPacket_.m_nOK, NetError::ERR_BUFF_DONATION_ERROR_07); // 기부 데이터 기록 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업
        default:
            START_LOG( cerr, L" 발렌타인 버프 기부 응답 중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    START_LOG( clog, L"발렌타인 버프 기부 응답 결과" )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kDonationItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kDonationItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( kPacket_.m_biTotalDonationPoint )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUFF_DONATION_ACK );
}

_IMPL_ON_FUNC( EVENT_ACCUMULATE_ATTENDANCE_REQ, DWORD )
{
    START_LOG( clog, L"누적 출석 이벤트 스크립트 변경시 접속중인 유저 데이터 요청: " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    QUEUEING_ID_TO_DB( EVENT_ACCUMULATE_ATTENDANCE_REQ );
}

IMPL_ON_FUNC( EVENT_ACCUMULATE_ATTENDANCE_ACK )
{
    // 출석을 하고 누적 된 출석 일수/보상 지급 정보 유저의 누적출석 객체에 갱신
    m_kAccumulateAttendanceUserData.UpdateUserAccumulateAttendanceData( kPacket_ );

    START_LOG( clog, L"누적 출석 데이터 갱신 완료" )
        << BUILD_LOG( kPacket_.m_nAccumulateCount )
        << BUILD_LOG( kPacket_.m_vecRewardCompleteDay.size() )
        << END_LOG;
}

// 미니게임 클릭 시 최초 한번 요청한다
IMPL_ON_FUNC_NOPARAM( EVENT_ACCUMULATE_ATTENDANCE_INFO_REQ )
{
    KEVENT_ACCUMULATE_ATTENDANCE_INFO_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );

    // 누적 출석 기간인가?
    if ( false == SiKAccumulateAttendanceManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_ACCUMULATE_ATTENDANCE_ERROR_01, END_PROC );
    }

    // 스크립트 정보 세팅
    SiKAccumulateAttendanceManager()->GetAccumulateAttendanceScriptInfo( kPacket );
    // 유저의 누적 출석 정보 세팅
    m_kAccumulateAttendanceUserData.GetUserAccumulateAttendanceData( kPacket.m_kUserAccumulateData );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, NetError::NET_OK );     // 성공
        ERR_CASE( ERR_ACCUMULATE_ATTENDANCE_ERROR_01, kPacket.m_nOK, NetError::ERR_ACCUMULATE_ATTENDANCE_ERROR_01 ); // 누적 출석 기간이 아니다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"누적 출석 데이터 요청 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_ACCUMULATE_ATTENDANCE_INFO_ACK );

    START_LOG( clog, L"누적 출석 데이터 요청 응답" )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_bUseImmediatelyRegisterMission )
        << BUILD_LOG( kPacket.m_mapCumulativeAttendanceRewardList.size() )
        << BUILD_LOG( kPacket.m_kUserAccumulateData.m_nAccumulateCount )
        << BUILD_LOG( kPacket.m_kUserAccumulateData.m_vecRewardCompleteDay.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ, int )
{
    KDB_EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ kDBPacket;
    KEVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK kPacket;

    kDBPacket.m_nDayIndex = kPacket_;

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_ACCUMULATE_ATTENDANCE_INFO_ACK kRewardDayInfo; // 에러나도 출석 받은 날짜는 보내준다
    m_kAccumulateAttendanceUserData.GetUserAccumulateAttendanceData( kRewardDayInfo.m_kUserAccumulateData );
    kPacket.m_vecRewardCompleteDay = kRewardDayInfo.m_kUserAccumulateData.m_vecRewardCompleteDay;
    kPacket.m_vecRewardItem.clear();

    // 누적 출석 기간인가??
    if( false == SiKAccumulateAttendanceManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_ACCUMULATE_ATTENDANCE_ERROR_01, END_PROC );
    }

    // 클라이언트에서 요청한 날짜가 유효한가 (최대 보상 가능일자 확인)
    if( !SiKAccumulateAttendanceManager()->CheckRewardDayValid( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_ACCUMULATE_ATTENDANCE_ERROR_02, END_PROC );
    }

    // 이미 보상을 받은 날짜인가?
    if( true == m_kAccumulateAttendanceUserData.IsRewardCompleteDay( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_ACCUMULATE_ATTENDANCE_ERROR_03, END_PROC );
    }

    // 해당 날짜의 보상 리스트 가져오기
    if( true != SiKAccumulateAttendanceManager()->GetReward( kPacket_, kDBPacket.m_vecReward ) ) {
        SET_ERR_GOTO( ERR_ACCUMULATE_ATTENDANCE_ERROR_04, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_ACCUMULATE_ATTENDANCE_REWARD_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                             kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_ACCUMULATE_ATTENDANCE_ERROR_01, kPacket.m_nOK, NetError::ERR_ACCUMULATE_ATTENDANCE_ERROR_01 ); // 누적 출석 기간이 아니다.
        ERR_CASE( ERR_ACCUMULATE_ATTENDANCE_ERROR_02, kPacket.m_nOK, NetError::ERR_ACCUMULATE_ATTENDANCE_ERROR_02 ); // 보상을 요청한 날짜가 유효하지 않다.
        ERR_CASE( ERR_ACCUMULATE_ATTENDANCE_ERROR_03, kPacket.m_nOK, NetError::ERR_ACCUMULATE_ATTENDANCE_ERROR_03 ); // 이미 보상을 받은 날짜이다.
        ERR_CASE( ERR_ACCUMULATE_ATTENDANCE_ERROR_04, kPacket.m_nOK, NetError::ERR_ACCUMULATE_ATTENDANCE_ERROR_04 ); // 해당 날짜의 보상 정보가 없다.
        ERR_CASE( ERR_NOT_FINISHED_YET,               kPacket.m_nOK, -98 );// 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"누적 출석 보상 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99;
    }

    START_LOG( clog, L"누적 출석 보상 요청 처리 완료" )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kDBPacket.m_nDayIndex )
        << BUILD_LOG( kDBPacket.m_vecReward.size() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket.m_vecRewardCompleteDay.size() )
        << END_LOG;

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( !kPacket_.m_vecRewardItem.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
    }

    // 유저 보상 완료 목록 갱신
    m_kAccumulateAttendanceUserData.UpdateUserAttendanceRewardCompleteData( kPacket_.m_vecRewardCompleteDay );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                             kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_ACCUMULATE_ATTENDANCE_ERROR_05, kPacket_.m_nOK, NetError::ERR_ACCUMULATE_ATTENDANCE_ERROR_05 ); // 누적 출석 보상 아이템 지급 실패
        ERR_CASE( ERR_ACCUMULATE_ATTENDANCE_ERROR_06, kPacket_.m_nOK, NetError::ERR_ACCUMULATE_ATTENDANCE_ERROR_06 ); // 누적 출석 보상 지급 정보 갱신 실패(보상 중복 신청 or 유저 정보 없음)
    default:
        START_LOG( cerr, L"누적 출석 보상 요청 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_ACCUMULATE_ATTENDANCE_REWARD_ACK );

    START_LOG( clog, L"누적 출석 보상 응답 " )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecRewardCompleteDay.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_CYOU_CASH_CHANGE_REQ )
{
    // consumed points 의 한계는 9999 포인트이다.
    KCYOUCashData kData;
    //kData.m_strServerIP = SiKGameServer()->GetIP();
    kData.m_strLogin = m_strBillingAccount;// KncUtil::toNarrowString( GetName() );
    kData.m_strUserIP = GetIPStr();
    
    std::wstring wstrNick(L"");
    GetCharNickNameInfo( GetCurrentCharType(), wstrNick ); 
    kData.m_strCharNickname = KncUtil::toNarrowString( wstrNick );
    
    //kData.m_strCharNickname = boost::lexical_cast<std::string>( static_cast<int>( GetCurrentCharType() ) );
    
    kData.m_dwUserUID = GetUID();
    kData.m_nCashPoint = static_cast<int>(kPacket_.m_dwPoint);

    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_CYOU_CASH_CHANGE_REQ, kData ) );
}

_IMPL_ON_FUNC( EVENT_CYOU_CASH_CHANGE_ACK, KCYOUCashPoint )
{
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"중국 캐시 충전 결과, User ID : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwCashPoint )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CYOU_CASH_CHANGE_ACK );
}
_IMPL_ON_FUNC( EVENT_CYOU_USER_HEART_BEAT_ACK, int )
{
    NetError::SetLastNetError( kPacket_ );

    switch ( NetError::GetLastNetError() ) {
        case NetError::ERR_NUSER_VERIFY_60:
        case NetError::ERR_NUSER_VERIFY_68:
        case NetError::ERR_NUSER_VERIFY_61:

            break;
        default:
            ;
    }

    LOG_SUCCESS( kPacket_ == 0 )
        << BUILD_LOG( NetError::GetLastNetError() )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CYOU_USER_HEART_BEAT_ACK );
}
_IMPL_ON_FUNC( EVENT_ADD_SLOT_REQ, GCITEMID )
{
    KEVENT_ADD_SLOT_ACK kPacket;
    KDB_EVENT_ADD_SLOT_REQ kDBPacket;
    kPacket.m_nOK = -99;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    // 해당 아이템이 존재 하는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_ );
    if ( !pkItem ) {
        SET_ERR_GOTO( ERR_ADD_SLOT_01, END_PROC );
    }

    // 현재 최대 슬롯 갯수를 초과하는가?
    DWORD dwMaxSlotNum = SiKCharSlotManager()->GetMaxSlotNum();
    if( dwMaxSlotNum <= m_dwUserRemainIndexCnt ) {
        SET_ERR_GOTO( ERR_ADD_SLOT_02, END_PROC );
    }

    // 정의된 캐릭터 슬롯 추가 아이템인가?
    if( !SiKCharSlotManager()->GetAddSlotNum( kPacket_, kDBPacket.m_nAddSlotCnt ) ) {
        SET_ERR_GOTO( ERR_ADD_SLOT_03, END_PROC );
    }

    kDBPacket.m_kUseItem = *pkItem;
    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_ADD_SLOT_REQ, kDBPacket );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ADD_SLOT_01, kPacket.m_nOK, 1 ); // 슬롯 추가 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_ADD_SLOT_02, kPacket.m_nOK, 2 ); // 슬롯 추가를 더이상 할 수 없음.
        ERR_CASE( ERR_ADD_SLOT_03, kPacket.m_nOK, 3 ); // 정의되지 않은 캐릭터 슬롯 추가 아이템.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"캐릭터 슬롯 추가.. Name : " << m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_ADD_SLOT_ACK );
    }
}
IMPL_ON_FUNC( EVENT_ADD_SLOT_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    KItemPtr pkItem;
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kUseItem.m_ItemUID );
    if( pkItem ) {
        pkItem->m_nCount = kPacket_.m_kUseItem.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kUseItem.m_nCount;
    }

    // 유저 보유 슬롯 갯수 갱신.
    m_dwUserRemainIndexCnt += kPacket_.m_nAddSlotCnt;
    kPacket_.m_dwRemainIndexCnt = m_dwUserRemainIndexCnt;

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ADD_SLOT_01, kPacket_.m_nOK, 1 ); // 슬롯 추가 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_ADD_SLOT_02, kPacket_.m_nOK, 2 ); // 슬롯 추가를 더이상 할 수 없음.
        ERR_CASE( ERR_ADD_SLOT_03, kPacket_.m_nOK, 3 ); // 정의되지 않은 캐릭터 슬롯 추가 아이템.
        ERR_CASE( ERR_ADD_SLOT_04, kPacket_.m_nOK, 4 ); // 슬롯 추가 아이템 DB 동기화 실패.
        default:
            START_LOG( cerr, L" 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
                << BUILD_LOG( m_strName ) << END_LOG;
            kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_ADD_SLOT_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"유저 슬롯 추가에 대한 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nAddSlotCnt )
        << BUILD_LOG( kPacket_.m_dwRemainIndexCnt )
        << END_LOG;
}
IMPL_ON_FUNC_NOPARAM( EVENT_CASHBACK_EXTRA_RATIO_INFO_REQ )
{
    KEVENT_CASHBACK_EXTRA_RATIO_INFO_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    QUEUEING_ID_TO_DB( EVENT_CASHBACK_EXTRA_RATIO_INFO_REQ );

    SET_ERROR( NET_OK );

//END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"캐시백 추가 비율 정보 요청 처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"캐시백 추가 비율 정보 요청 처리, User : " << GetName() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << END_LOG;

    if ( 0 != kPacket.m_nOK ) {
        SEND_PACKET( EVENT_CASHBACK_EXTRA_RATIO_INFO_ACK );
    }
}



IMPL_ON_FUNC( EVENT_CASHBACK_EXTRA_RATIO_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_NOT_FINISHED_YET,             kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"캐시백 추가 비율 정보 응답 처리 중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_CASHBACK_EXTRA_RATIO_INFO_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"캐시백 추가 비율 정보 요청 처리 응답, User : " << GetName() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOG( kPacket_.m_nFirstBuyRatio )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_CHAR_SELECT_JOIN_REQ )
{
    if( IsDataReceive() ) {
        START_LOG( cerr, L"유저 캐릭터 선택씬에서 정보 받았는데 다시 요청함" )
            << END_LOG;
        return;
    }
    SendUserDetailInfo();
    m_bDataReceive = true;

    START_LOG( clog, L"유저 캐릭터 선택씬에서 월드맵 입장시에 유저 남은 정보 전달" )
        << END_LOG;
}

_IMPL_ON_FUNC( DB_EVENT_ERRAND_USER_INFO_REQ, DWORD )
{
    START_LOG( clog, L"디오의 심부름왕 이벤트 스크립트 변경시 접속중인 유저 데이터 요청: " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    QUEUEING_ID_TO_DB( DB_EVENT_ERRAND_USER_INFO_REQ );
}

_IMPL_ON_FUNC( EVENT_ERRAND_USER_INFO_ACK, KErrandUserInfoDataStruct )
{    
    m_kErrandUserData.UpdateUserDateFromDB( kPacket_ );

    // 오늘 첫 접속이면 바로 플레이 가능하게 해준다
    if ( true == m_bFirstLoginToday || ( m_iSessionInfo == SI_NEW_ACCOUNT ) ) {
        // playable true로 변경
        m_kErrandUserData.SetPlayable( true );
        // 대기 시간 0으로 변경
        m_kErrandUserData.ClearRemainTime();
        START_LOG( clog, L"심부름 이벤트 일일 첫접속이라서 플레이 가능한 상태로 만들어 줌" ) << END_LOG;
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_ERRAND_INIT_INFO_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_ERRAND_INIT_INFO_ACK kPacket;

    // 이벤트가 진행 중인지
    if ( false == SiKErrandManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_ERRAND_ERROR_01, END_PROC );
    }

    // 스크립트 설정 세팅
    SiKErrandManager()->GetClientInitInfo( kPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,              kPacket.m_nOK, NetError::NET_OK ); // 
        ERR_CASE( ERR_ERRAND_ERROR_01, kPacket.m_nOK, NetError::ERR_ERRAND_ERROR_01 ); // 심부름왕 이벤트 기간이 아니다
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"심부름 이벤트 초기 정보 요청시 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_ERRAND_INIT_INFO_ACK );

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nPlayTime )
        << BUILD_LOG( kPacket.m_nDelayTime )
        << BUILD_LOG( kPacket.m_nTimeIncreaseRate )
        << BUILD_LOG( kPacket.m_nMaxStage )
        << BUILD_LOG( kPacket.m_nMarryAppearInterval )
        << BUILD_LOG( kPacket.m_nMarryKeepAliveTime )
        << BUILD_LOG( kPacket.m_mapErrandStageInfo.size() )
        << BUILD_LOG( kPacket.m_vecRewardItemForDisplay.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ERRAND_REWARD_REQ, int )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_ERRAND_REWARD_ACK kPacket;
    KDB_EVENT_ERRAND_REWARD_REQ kDBPacket;
    std::vector< PAIR_INT > vecReceived;
    kPacket.m_vecRewardItem.clear();
    vecReceived.clear();

    // 이벤트가 진행 중인지
    if ( false == SiKErrandManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_ERRAND_ERROR_01, END_PROC );
    }

    // 칭찬(성공) 횟수가 유효한지 확인
    if ( false == SiKErrandManager()->IsValidCount( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_ERRAND_ERROR_02, END_PROC );
    }

    if ( false == m_kErrandUserData.GetRewardEnableState() ) {
        SET_ERR_GOTO( ERR_ERRAND_ERROR_05, END_PROC );
    }

    // 최종 보상 받은 정보를 얻는다
    m_kErrandUserData.GetReceivedSpecialRewardInfo( vecReceived );

    // 내부에서 AOLottery의 랜덤 보상을 얻는다
    if ( false == SiKErrandManager()->GetRewardItem( kPacket_, vecReceived, kDBPacket ) ) {
        SET_ERR_GOTO( ERR_ERRAND_ERROR_03, END_PROC );
    }

    // 바로 위에서 랜덤 보상을 얻었을 때 최종 보상을 얻었을 때만 캐싱 데이터도 갱신
    if ( false == kDBPacket.m_vecSpecialReward.empty() ) {
        m_kErrandUserData.SetReceivedSpecialRewardInfo( vecReceived );
    }

    // 보상 요청 불가능한 상태로 변경
    m_kErrandUserData.SetRewardEnableState( false );

    QUEUEING_EVENT_TO_DB( DB_EVENT_ERRAND_REWARD_REQ, kDBPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, NetError::NET_OK );
        ERR_CASE( ERR_ERRAND_ERROR_01,  kPacket.m_nOK, NetError::ERR_ERRAND_ERROR_01 ); // 심부름왕 이벤트 기간이 아니다
        ERR_CASE( ERR_ERRAND_ERROR_02,  kPacket.m_nOK, NetError::ERR_ERRAND_ERROR_02 ); // 부름왕 칭찬(성공) 횟수가 유효하지 않다
        ERR_CASE( ERR_ERRAND_ERROR_03,  kPacket.m_nOK, NetError::ERR_ERRAND_ERROR_03 ); // 심부름왕 보상 요청 처리 시 AOLottery에서 계산 실패
        ERR_CASE( ERR_ERRAND_ERROR_05,  kPacket.m_nOK, NetError::ERR_ERRAND_ERROR_05 ); // 심부름왕 보상 가능한 상태가 아니다
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"심부름왕 보상 요청 시 알수 없는 오류 발생, Name : " << m_strName )
            << BUILD_LOGc( kPacket_ )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_ERRAND_REWARD_ACK );
    }

    LOG_CONDITION( NetError::GetLastNetError() == NetError::NET_OK || NetError::GetLastNetError() == NetError::ERR_NOT_FINISHED_YET, clog, cerr )
        << L"심부름왕 게임 종료 후 보상 요청 : " << (kPacket.m_nOK == 0? L"성공":L"실패" ) << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( vecReceived.size() )
        << BUILD_LOG( kDBPacket.m_vecReward.size() )
        << BUILD_LOG( kDBPacket.m_vecSpecialReward.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_ERRAND_REWARD_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( false == IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if ( false == kPacket_.m_vecRewardItem.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
    }

    if ( false == kPacket_.m_vecSpecialRewardItem.empty() ) {
        // 획득 아이템 넣기
        m_kInventory.AddItemList( kPacket_.m_vecSpecialRewardItem );
    }

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_ERRAND_ERROR_01, kPacket_.m_nOK, NetError::ERR_ERRAND_ERROR_01 ); // 심부름왕 이벤트 기간이 아니다
        ERR_CASE( ERR_ERRAND_ERROR_02,  kPacket_.m_nOK, NetError::ERR_ERRAND_ERROR_02 ); // 부름왕 칭찬(성공) 횟수가 유효하지 않다
        ERR_CASE( ERR_ERRAND_ERROR_03,  kPacket_.m_nOK, NetError::ERR_ERRAND_ERROR_03 ); // 심부름왕 보상 요청 처리 시 AOLottery에서 계산 실패
        ERR_CASE( ERR_ERRAND_ERROR_04,  kPacket_.m_nOK, NetError::ERR_ERRAND_ERROR_04 ); // 심부름왕 DB보상 아이템 지급 실패
        ERR_CASE( ERR_ERRAND_ERROR_06,  kPacket_.m_nOK, NetError::ERR_ERRAND_ERROR_06 ); // 최종보상 받은 개수 캐싱 데이터가 이상하다
    default:
        START_LOG( cerr, L"심부름왕 보상 요청 처리중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET( EVENT_ERRAND_REWARD_ACK );

    START_LOG( clog, L"심부름왕 보상 응답 " )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( kPacket_.m_vecSpecialRewardItem.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ERRAND_GAME_END_REQ, int )
{
    SET_ERROR( ERR_UNKNOWN );

    int nPlayCount = 1;
    KEVENT_ERRAND_GAME_END_ACK kPacket;

    kPacket.m_nOK = -99;

    // 이벤트가 진행 중인지
    if ( false == SiKErrandManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_ERRAND_ERROR_01, END_PROC );
    }

    // 플레이 카운트는 기록(Log 데이터)
    m_kErrandUserData.IncreaseDataForLog( nPlayCount ,kPacket_ );

    // 보상 지급 완료 후 대기시간 적용
    m_kErrandUserData.SetPlayable( false );
    m_kErrandUserData.ResetRemainTime();

    // 게임 종료 후 보상 가능한 상태로 변경
    m_kErrandUserData.SetRewardEnableState( true );

    m_kErrandUserData.GetPlayable( kPacket.m_bPlayAble );
    kPacket.m_bSuccess = SiKErrandManager()->CheckFinalSuccess( kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,              kPacket.m_nOK, NetError::NET_OK ); // 성공.
        ERR_CASE( ERR_ERRAND_ERROR_01, kPacket.m_nOK, NetError::ERR_ERRAND_ERROR_01 ); // 심부름왕 이벤트 기간이 아니다
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"심부름 이벤트 초기 정보 요청시 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_ERRAND_GAME_END_ACK );

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_bPlayAble )
        << BUILD_LOG( kPacket_ )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_ERRAND_USER_INFO_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_ERRAND_USER_INFO_ACK kPacket;

    // 이벤트가 진행 중인지
    if ( false == SiKErrandManager()->IsRun() ) {
        SET_ERR_GOTO( ERR_ERRAND_ERROR_01, END_PROC );
    }

    // 유저 데이터 세팅
    m_kErrandUserData.GetUserDataForClient( kPacket );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,              kPacket.m_nOK, NetError::NET_OK ); // 
        ERR_CASE( ERR_ERRAND_ERROR_01, kPacket.m_nOK, NetError::ERR_ERRAND_ERROR_01 ); // 심부름왕 이벤트 기간이 아니다
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"심부름 이벤트 유저 데이터 요청시 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_PACKET( EVENT_ERRAND_USER_INFO_ACK );

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << L"심부름왕 유저 데이터 요청 결과 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_bPlayable )
        << BUILD_LOG( kPacket.m_nRemainTime )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_RKTORNADO_ITEM_LIST_NOT )
{
    START_LOG( clog, L"릭쿤토네이도 스크립트 변경시 접속중인 유저들에게 스크립트 데이터 전달: " << m_strName )
        << BUILD_LOG( kPacket_.m_bEventEnable )
        << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_RKTORNADO_ITEM_LIST_NOT );
}

_IMPL_ON_FUNC( EVENT_UPDATEPLAN_GET_GACHA_USER_DB_DATA_NOT, DWORD )
{
    START_LOG( clog, L"가챠 스크립트 변경시 접속중인 유저 데이터 요청: " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    QUEUEING_ID_TO_DB( EVENT_CHECK_GACHA_REEL_POINT_REQ );
}

IMPL_ON_FUNC_NOPARAM( EVENT_GACHA_DATA_CHANGE_NOT )
{
    KEVENT_GACHA_DATA_CHANGE_NOT kPacket;

    GetGachaUseVersionsInfo( kPacket );
    SEND_PACKET( EVENT_GACHA_DATA_CHANGE_NOT );

    START_LOG( clog, L"가챠 스크립트 변경시 유저에게 알림: " << m_strName )
        << BUILD_LOG( kPacket.size() )
        << END_LOG;
}

IMPL_ON_FUNC(EVENT_LOOK_INVENTORY_EXTEND_REQ)
{
    KItemPtr pkItem;
    int nMaxInvenCapacity = SiKLookEquipment()->GetMaxInvenSize();
    int nCharLookInvenCapacity = GetCharLookInvenCapacity();
    KEVENT_LOOK_INVENTORY_EXTEND_ACK kPacket;
    SET_ERROR(ERR_UNKNOWN);

    if (nCharLookInvenCapacity >= nMaxInvenCapacity) {
        SET_ERR_GOTO(ERR_INVEN_03, END_PROC);
    }

    if (!SiKLookEquipment()->GetItemExtendSize(kPacket_.m_UseItemID, kPacket_.m_nExtendSize)) {
        SET_ERR_GOTO(ERR_INVEN_00, END_PROC);
    }

    if (nCharLookInvenCapacity + kPacket_.m_nExtendSize > nMaxInvenCapacity) {
        SET_ERR_GOTO(ERR_INVEN_03, END_PROC);
    }

    pkItem = m_kInventory.FindDurationItemByItemID(kPacket_.m_UseItemID);
    if (pkItem == NULL) {
        SET_ERR_GOTO(ERR_INVEN_01, END_PROC);
    }

    kPacket_.m_kUseItem = *pkItem;
    kPacket_.m_cCharType = m_cCurrentCharacter;
    GetCharSlotNum(kPacket_.m_dwSlotNum);

    QUEUEING_EVENT_TO_DB_CHECK(EVENT_LOOK_INVENTORY_EXTEND_REQ, kPacket_);
    SET_ERROR(NET_OK);

END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0);
        ERR_CASE(ERR_INVEN_00, kPacket.m_nOK, 1);
        ERR_CASE(ERR_INVEN_01, kPacket.m_nOK, 2);
        ERR_CASE(ERR_INVEN_03, kPacket.m_nOK, 3);
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98);
    default:
        kPacket.m_nOK = -99;
        START_LOG(cerr, L"코디 인벤토리 확장.. 알수 없는 오류")
            << BUILD_LOG(m_strName)
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
    }

    LOG_SUCCESS(kPacket.m_nOK == 0)
        << BUILD_LOG(m_strName)
        << BUILD_LOG(kPacket_.m_UseItemID)
        << BUILD_LOG(nCharLookInvenCapacity)
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << BUILD_LOG(nMaxInvenCapacity) << END_LOG;

    if (NetError::GetLastNetError() != NetError::NET_OK) {
        SEND_PACKET(EVENT_LOOK_INVENTORY_EXTEND_ACK);
    }
}

IMPL_ON_FUNC(EVENT_LOOK_INVENTORY_EXTEND_ACK)
{
    KItemPtr pkItem; // ½ºÅ³¿ÀÇÂ Key ¾ÆÀÌÅÛ.
    NetError::SetLastNetError(kPacket_.m_nOK);
    if (!IS_CORRECT(NET_OK) && !IS_CORRECT(ERR_INVEN_15)) {
        goto END_PROC;
    }

    UpdateCharLookInvenCapacity(kPacket_.m_nExtendSize);
    kPacket_.m_nLookItemInvenCapacity = GetCharLookInvenCapacity();

    pkItem = m_kInventory.FindDurationItemByItemID(kPacket_.m_kUseItem.m_ItemID);
    if (pkItem) {
        pkItem->m_nCount = kPacket_.m_kUseItem.m_nCount;
        pkItem->m_nInitCount = kPacket_.m_kUseItem.m_nCount;
    }

END_PROC:
    switch (NetError::GetLastNetError()) {
        ERR_CASE(NET_OK, kPacket_.m_nOK, 0);       // ¼º°ø
        ERR_CASE(ERR_INVEN_00, kPacket_.m_nOK, 1); // ¾ÆÀÌÅÛ ¾ÆÀÌµð°¡ ´Ù¸§
        ERR_CASE(ERR_INVEN_01, kPacket_.m_nOK, 2); // ¾ÆÀÌÅÛÀ» º¸À¯ÇÏ°í ÀÖÁö ¾ÊÀ½
        ERR_CASE(ERR_INVEN_03, kPacket_.m_nOK, 3); // ´õÀÌ»ó ÀÎº¥Åä¸®¸¦ È®ÀåÇÒ¼ö ¾øÀ½
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98); // ¾ÆÁ÷ Ã³¸®ÁßÀÎ ÀÛ¾÷ÀÔ´Ï´Ù.
    default:
        START_LOG(cerr, L"코디 인벤 확장 아이템 사용처리중 알 수 없는 오류 Name  : " << GetName())
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET(EVENT_LOOK_INVENTORY_EXTEND_ACK);

    _LOG_SUCCESS(kPacket_.m_nOK == 0, L"코디 인벤 확장 아이템 사용 처리 응답, User : " << GetName())
        << BUILD_LOG(kPacket_.m_kUseItem.m_ItemID)
        << END_LOG;
}

//IMPL_ON_FUNC_NOPARAM( EVENT_CYOU_USER_AGREEMENT_REQ )
//{
//    KCYOUAuthUserInfo kData;
//    kData.m_strLogin = KncUtil::toNarrowString( GetName() );
//    kData.m_dwUID = GetUID();
//    kData.m_strIP = GetIPStr();
//    
//    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_CYOU_USER_AGREEMENT_REQ, kData ) );
//
//    START_LOG( clog, L"중국 유저 동의" )
//        << BUILD_LOG( GetName() )
//        << BUILD_LOG( KncUtil::toWideString( GetIPStr() ) )
//        << BUILD_LOG( KncUtil::toWideString( kData.m_strIP ) )
//        << END_LOG;
//
//}


IMPL_ON_FUNC_NOPARAM( EVENT_NEW_USER_TUTORIAL_CLEAR_NOT )
{
    m_bTutorialEnable = true;
    time_t tmFirstLoginTime = KncUtil::TimeToInt( m_tmFirstLoginTime );
    QUEUEING_EVENT_TO_DB( EVENT_NEW_USER_TUTORIAL_CLEAR_NOT, tmFirstLoginTime );
    START_LOG( clog, L"신규 유저 튜토리얼 진행완료" << m_strName )
        << END_LOG;
}


_IMPL_ON_FUNC( EVENT_ATTRIBUTE_MIGRATION_INFO_REQ, GCITEMUID )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_ATTRIBUTE_MIGRATION_INFO_ACK kPacket;

    // 아이템 속성 정보 가져오기.
    KItemPtr pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_ );
    if( pkInvenItem == NULL ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_04, END_PROC );
    }

    kPacket.m_vecAttributeList = pkInvenItem->m_vecAttribute;

    // 성장 개편 재설정 사용가능한지 체크.
    if( !SiKItemManager()->m_kAttributeDecider.CheckMigrationAttributeEventEnable() ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_01, END_PROC );
    }

    if( SiKItemManager()->m_kAttributeDecider.CheckUserCountCheckEnable() ) { // 횟수 제한 체크시
        if( !CheckUserAttributeMigrationPossible( kPacket_, pkInvenItem->m_tRegDate, kPacket.m_dwChangePossbileCount ) ) {
            SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_03, END_PROC );
        }
    }
    else {
        // 횟수 제한 체크 안할 경우 이벤트 기간인지 체크
        if( !SiKItemManager()->m_kAttributeDecider.IsEventTerm() ) {
            SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_02, END_PROC );
        }
        kPacket.m_bEventEnable = true;
    }

    // 아이템 변경 가능한 속성 정보 가져오기.
    SiKItemManager()->m_kAttributeDecider.GetChangeAttributeList( kPacket.m_setChangeAttributeList );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_01, kPacket.m_nOK, 1 ); // 성장 개편 재설정 기능 사용 불가능.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_02, kPacket.m_nOK, 2 ); // 성장 개편 재설정 이벤트 기간 아님.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_03, kPacket.m_nOK, 3 ); // 성장 개편 재설정 가능 횟수 초과.
    default:
        START_LOG( cerr, L"성장 개편 속성 변경 정보 요청중 알 수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_ATTRIBUTE_MIGRATION_INFO_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 3, L"성장 개편 속성 변경 정보 불러오기 요청, User : " << GetName() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_bEventEnable )
        << BUILD_LOG( kPacket.m_dwChangePossbileCount )
        << BUILD_LOG( kPacket.m_setChangeAttributeList.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_ATTRIBUTE_MIGRATION_SELECT_REQ )
{
    struct SAttributeEqualCheck {
    private:
        KAttributeInfo m_kAttribute;
    public:
        SAttributeEqualCheck( KAttributeInfo& kAtt_ ) {
            m_kAttribute = kAtt_;
        }
        bool operator() ( const std::pair<int,float>& kInfo_ ) {
            static const double EPSILON = 2.2204460492503131e-016;
            return  m_kAttribute.m_cType == kInfo_.first && 
                (static_cast<double>(m_kAttribute.m_fValue - kInfo_.second)<=EPSILON); 
        }
    };

    KEVENT_ATTRIBUTE_MIGRATION_SELECT_ACK kPacket;
    std::list<std::pair<int,float>> listAttributes;
    std::list<std::pair<int,float>>::iterator lit;
    std::vector< KAttributeInfo >::iterator vit;
    std::vector< KAttributeInfo >::iterator vitInven;
    KItemPtr pkInvenItem;
    DWORD dwChangePossbileCount = 0;
    std::vector< KAttributeInfo > vecAddAttributeList = kPacket_.m_vecAddAttributeList;

    SET_ERROR( ERR_UNKNOWN );

    // 성장 개편 재설정 사용가능한지 체크.
    if( !SiKItemManager()->m_kAttributeDecider.CheckMigrationAttributeEventEnable() ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_01, END_PROC );
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_ItemUID );
    if( pkInvenItem == NULL ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_04, END_PROC );
    }

    if( SiKItemManager()->m_kAttributeDecider.CheckUserCountCheckEnable() ) { // 횟수 제한 체크시
        if( !CheckUserAttributeMigrationPossible( kPacket_.m_ItemUID, pkInvenItem->m_tRegDate, dwChangePossbileCount ) ) {
            SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_03, END_PROC );
        }
    }
    else {
        // 횟수 제한 체크 안할 경우 이벤트 기간인지 체크
        if( !SiKItemManager()->m_kAttributeDecider.IsEventTerm() ) {
            SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_02, END_PROC );
        }
        kPacket_.m_bEventEnable = true;
    }

    kPacket_.m_kItem = *pkInvenItem;

    // 클라이언트에서 전달된 속성이 추가&삭제 되는 속성 체크
    for( vit = kPacket_.m_vecDelAttributeList.begin(); vit != kPacket_.m_vecDelAttributeList.end(); ++vit ) {
        if( !SiKItemManager()->m_kAttributeDecider.CheckChangeAttribute( vit->m_cType ) ) {
            SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_05, END_PROC );
        }
    }

    for( vit = kPacket_.m_vecAddAttributeList.begin(); vit != kPacket_.m_vecAddAttributeList.end(); ++vit ) {
        if( !SiKItemManager()->m_kAttributeDecider.CheckChangeAttribute( vit->m_cType ) ) {
            SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_05, END_PROC );
        }
    }

    // 삭제하는 속성을 아이템 속성에서 변경해야 함.
    for( vit = kPacket_.m_vecDelAttributeList.begin(); vit != kPacket_.m_vecDelAttributeList.end(); ++vit ) {
        vitInven = std::find_if( pkInvenItem->m_vecAttribute.begin(), pkInvenItem->m_vecAttribute.end(), boost::bind( &KAttributeInfo::m_cType, _1 ) == vit->m_cType );

        if( vitInven == pkInvenItem->m_vecAttribute.end() ) {
            continue;
        }
        vitInven->Init(); // 속성정보 초기화.
    }

    // 속성을 선택할 수 있는 갯수를 확인 합니다.
    for ( vit = pkInvenItem->m_vecAttribute.begin() ; vit != pkInvenItem->m_vecAttribute.end() ; ++vit ) {
        if ( vit->m_cState == KAttributeInfo::ATS_USING ) { 
            vitInven = std::find_if( kPacket_.m_vecAddAttributeList.begin(), kPacket_.m_vecAddAttributeList.end(), boost::bind( &KAttributeInfo::m_cType, _1 ) == vit->m_cType );

            if( vitInven != kPacket_.m_vecAddAttributeList.end() && SiKItemManager()->m_kAttributeDecider.CheckChangeAttribute( vit->m_cType ) ) {
                continue;
            }

            kPacket_.m_vecAddAttributeList.push_back( *vit );
        }
    } // for 와 아래 if 가 필요없어 보이는데?

    if( kPacket_.m_vecAddAttributeList.size() != pkInvenItem->m_vecAttribute.size() )
    {
        SET_ERR_GOTO( ERR_ATTRIBUTE_SELECT_03, END_PROC );
    }

    //해당 아이템의 Attribute List에 선택한 속성들이 있는지 확인합니다.
    int nAttributeTableID = SiKItemManager()->GetAttributeTableID( pkInvenItem->m_ItemID );
    int nLevelTableID = SiKItemManager()->GetLevelTableID( pkInvenItem->m_ItemID );

    // 레벨에 맞게 속성 변경해야 하는지 체크.
    int nItemLevel = 0;
    if( SiKItemManager()->m_kItemLevelDecider.IsSelectiveType( nLevelTableID ) ) {
        nItemLevel = static_cast<int>( pkInvenItem->m_sEquipLevel );
        SiKItemManager()->m_kItemLevelDecider.CheckItemLevelAttribute( nLevelTableID, nAttributeTableID, nItemLevel, KItemLevelTable::ILT_SELECTIVE );
    }

    SiKItemManager()->m_kAttributeDecider.GetAttributeFullList( nAttributeTableID, (int)pkInvenItem->m_cGradeID, listAttributes );

    for( vit = vecAddAttributeList.begin() ; vit != vecAddAttributeList.end() ; ++vit ) {
        lit = std::find_if( listAttributes.begin(), listAttributes.end(), SAttributeEqualCheck( *vit ) );

        if( lit == listAttributes.end() )
            SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_06, END_PROC );
        else 
            listAttributes.erase( lit ); // 같은 속성을 보내 올까봐 한번 검색 된것은 삭제 시킴.
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ATTRIBUTE_MIGRATION_SELECT_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_01, kPacket.m_nOK, 1 ); // 성장 개편 재설정 기능 사용 불가능.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_02, kPacket.m_nOK, 2 ); // 성장 개편 재설정 이벤트 기간 아님.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_03, kPacket.m_nOK, 3 ); // 성장 개편 재설정 가능 횟수 초과.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_04, kPacket.m_nOK, 4 ); // 속성을 선택할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_05, kPacket.m_nOK, 5 ); // 선택된 속성은 변경 가능한 속성이 아님.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_06, kPacket.m_nOK, 6 ); // 선택한 속성이 선택 가능한 속성에 포함되지 않습니다.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_07, kPacket.m_nOK, 7 ); // 속성을 DB 에 기록 하는데 오류 발생.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket.m_nOK = -99; // unknown error
        break;
    }

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_ATTRIBUTE_MIGRATION_SELECT_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"성장 개편 속성 변경 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_vecAddAttributeList.size() )
        << BUILD_LOG( kPacket_.m_vecDelAttributeList.size() )
        << BUILD_LOG( kPacket_.m_bEventEnable )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_ATTRIBUTE_MIGRATION_SELECT_ACK )
{
    KItemPtr pkInvenItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    pkInvenItem = m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_ItemUID );

    if( pkInvenItem == NULL ) {
        SET_ERR_GOTO( ERR_ATTRIBUTE_MIGRATION_04, END_PROC );
    }

    pkInvenItem->m_vecAttribute = kPacket_.m_kItem.m_vecAttribute;
    kPacket_.m_kItem = *pkInvenItem;

    if( !kPacket_.m_bEventEnable ) {
        IncreaseAttributeChangePossbileCount( kPacket_.m_kItem.m_ItemUID, kPacket_.m_dwChangePossbileCount );
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_01, kPacket_.m_nOK, 1 ); // 성장 개편 재설정 기능 사용 불가능.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_02, kPacket_.m_nOK, 2 ); // 성장 개편 재설정 이벤트 기간 아님.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_03, kPacket_.m_nOK, 3 ); // 성장 개편 재설정 가능 횟수 초과.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_04, kPacket_.m_nOK, 4 ); // 속성을 선택할 아이템이 인벤에 없음.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_05, kPacket_.m_nOK, 5 ); // 선택된 속성은 변경 가능한 속성이 아님.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_06, kPacket_.m_nOK, 6 ); // 선택한 속성이 선택 가능한 속성에 포함되지 않습니다.
        ERR_CASE( ERR_ATTRIBUTE_MIGRATION_07, kPacket_.m_nOK, 7 ); // 속성을 DB 에 기록 하는데 오류 발생.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << END_LOG;
        kPacket_.m_nOK = -99; // unknown error
        break;
    }

    SEND_RECEIVED_PACKET( EVENT_ATTRIBUTE_MIGRATION_SELECT_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"속성 설정 응답 MSG :" << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName)
        << BUILD_LOG( kPacket_.m_bEventEnable )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kItem.m_vecAttribute.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_USER_ATTRIBUTE_MIGRATION_INFO_ACK )
{
    m_mapAttributeMigrationItemInfo.swap( kPacket_ );

    START_LOG( clog, L"유저 속성 재설정 정보 전달.. Login : " << GetName() )
        << BUILD_LOG( m_mapAttributeMigrationItemInfo.size() )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_PACKAGE_INFO_DETAIL_REQ )
{
    KEVENT_PACKAGE_INFO_DETAIL_ACK kPacket;
    std::vector<std::map<GCITEMID,std::vector<KDropItemInfo>>> vecList;
    std::vector<std::map<GCITEMID,std::vector<KDropItemInfo>>>::iterator vit;
    SiKItemManager()->GetPackageInfo( vecList );
    kPacket.m_nMaxIndex = vecList.size();
    kPacket.m_nCurrentIndex = kPacket_.m_nCurrentIndex;
    int nIndex = 1;

    for( vit = vecList.begin(); vit != vecList.end(); ++vit ) {
        if( nIndex != kPacket.m_nCurrentIndex ) {
            ++nIndex;
            continue;
        }
        kPacket.m_mapPackageInfo = *vit;
        SEND_COMPRESS_PACKET( EVENT_PACKAGE_INFO_DETAIL_ACK, kPacket );
        ++nIndex;
        START_LOG( clog, L"패키지아이템 정보 전달.. Login : " << GetName() )
            << BUILD_LOG( kPacket_.m_nMaxIndex )
            << BUILD_LOG( kPacket_.m_nCurrentIndex )
            << BUILD_LOG( kPacket.m_nMaxIndex )
            << BUILD_LOG( kPacket.m_nCurrentIndex )
            << BUILD_LOG( kPacket.m_mapPackageInfo.size() )
            << END_LOG;
    }
}
IMPL_ON_FUNC( EVENT_USER_CHAT_CHANGE_BY_ADMIN_REQ )
{
    int kPacket = -99;
    if( !IsAdmin() ) {
        return;
    }

    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwUserUID ) );

    if( !spUser ) {
        kPacket = -1; //유저없음
    }
    else {
        if( spUser->ChangeUserChatState( kPacket_.m_bChatEnable ) ) {
            kPacket = 0; // 정상적으로 변경.
        }
        else {
            kPacket = -2; // 이미 요청한 상태임.
        }
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_bChatEnable )
        << BUILD_LOG( kPacket ) << END_LOG;

    if( kPacket == 0 ) {
        SiKGSSimLayer()->m_kActorManager.SendTo( kPacket_.m_dwUserUID, KUserEvent::EVENT_BAD_USER_INFO_NOT, spUser->m_kBadUserInfo );
        QUEUEING_EVENT_TO_DB( EVENT_USER_CHAT_CHANGE_BY_ADMIN_REQ, kPacket_ );
    }

    SEND_PACKET( EVENT_USER_CHAT_CHANGE_BY_ADMIN_ACK );
}

IMPL_ON_FUNC( EVENT_JOIN_ROOM_INFO_DIVIDE_REQ )
{
    KEVENT_JOIN_ROOM_INFO_DIVIDE_ACK kPacket;
    std::vector<KInDoorUserInfo>::iterator vit;
    kPacket.m_nOK = 0;
    kPacket.m_MaxIndex = kPacket_.m_nMaxIndex;
    kPacket.m_nCurrentIndex = kPacket_.m_nCurrentIndex;
    int nIndex = 0;

    for( vit = m_vecIndoorUserTemp.begin(); vit != m_vecIndoorUserTemp.end(); ++vit ) {
        if( nIndex == kPacket.m_nCurrentIndex ) {
            kPacket.m_kInDoorUserInfo = *vit;
            SEND_COMPRESS_PACKET( EVENT_JOIN_ROOM_INFO_DIVIDE_ACK, kPacket );

            START_LOG( clog, L"룸 유저 정보 전달.. Login : " << GetName() )
                << BUILD_LOG( kPacket.m_MaxIndex )
                << BUILD_LOG( kPacket.m_nCurrentIndex )
                << BUILD_LOG( kPacket.m_kInDoorUserInfo.m_strLogin )
                << END_LOG;
        }
        ++nIndex;
    }

    if( kPacket.m_MaxIndex == ( kPacket.m_nCurrentIndex+1 ) ) { // 데이터 다 보냈으면 초기화 하기.
        JIF( m_pkChannel );
        JIF( m_pkRoom );
        //여기서부터 성공이다.
        //방의 모든 유저에게 브로드 케스팅 한다. 방포인터는 검증된 상태이다.
        const KInDoorUserInfo* pkInDoorUserInfo = m_pkRoom->FindInDoorUserInfo( m_strName );
        if ( pkInDoorUserInfo ) {
            m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_JOIN_ROOM_BROAD, (*pkInDoorUserInfo), true );

            START_LOG_WITH_NAME( clog )
                << BUILD_LOG( pkInDoorUserInfo->m_nState )
                << BUILD_LOG( pkInDoorUserInfo->m_iPlayerIndex ) 
                << BUILD_LOG( pkInDoorUserInfo->m_strGuildName )
                << BUILD_LOG( pkInDoorUserInfo->m_bIsHost )
                << BUILD_LOG( pkInDoorUserInfo->m_bIsLive)
                << BUILD_LOG( pkInDoorUserInfo->m_bIsObserver )
                << BUILD_LOGc( pkInDoorUserInfo->m_cAuthLevel )
                << BUILD_LOGc( pkInDoorUserInfo->m_cCharIndex )
                << BUILD_LOGc( pkInDoorUserInfo->m_cPromotion )
                << BUILD_LOG( pkInDoorUserInfo->m_dwGuildUID )
                << BUILD_LOGc( pkInDoorUserInfo->m_cPCBangType ) << END_LOG;

            DumpInDoorCharInfo( pkInDoorUserInfo->m_vecCharInfo );

            if ( SiKGSSimLayer()->CheckTypeFlag( ST_GUILD ) ) {
                KRoomInfo kRoomInfo;
                m_pkRoom->GetRoomInfo( kRoomInfo );
                m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_GUILD_ROOM_INFO_CHANGE_NOT, kRoomInfo );
            }
        }
        m_vecIndoorUserTemp.clear();
    }
}

IMPL_ON_FUNC( EVENT_JUMPING_CHAR_INFO_NOT )
{
//     m_bJumpCharEnable = kPacket_.m_bJumpingCharEnable;
//     kPacket_.m_dwCharJumpingLevel = SiKJumpingChar()->GetJumpingCharLevel();
//     SiKJumpingChar()->GetJumpingCharEnableList( kPacket_.m_setJumpingCharList );
// 
//     SEND_RECEIVED_PACKET( EVENT_JUMPING_CHAR_INFO_NOT );
// 
//     START_LOG( clog, L"점핑 캐릭터정보 전달.. Login : " << GetName() )
//         << BUILD_LOG( kPacket_.m_bJumpingCharEnable )
//         << BUILD_LOG( kPacket_.m_dwCharJumpingLevel )
//         << BUILD_LOG( kPacket_.m_setJumpingCharList.size() )
//         << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_JUMPING_CHAR_REWARD_REQ )
{
//     SET_ERROR( ERR_UNKNOWN );
// 
//     KEVENT_JUMPING_CHAR_REWARD_ACK kPacket;
//     VECTOR_STRUCT_DROPITEM kDBPacket;
// 
//     // 이벤트 기간인지 체크.
//     if( !SiKJumpingChar()->IsEventTerm() ) {
//         SET_ERR_GOTO( ERR_JUMPING_CHAR_01, END_PROC );
//     }
// 
//     // 점핑보상을 받았는지 체크.
//     if( !m_bJumpCharEnable ) {
//         SET_ERR_GOTO( ERR_JUMPING_CHAR_02, END_PROC );
//     }
// 
//     if( !SiKJumpingChar()->GetJumpingCharDefaultReward( kDBPacket ) ) {
//         SET_ERR_GOTO( ERR_JUMPING_CHAR_03, END_PROC );
//     }
// 
//     QUEUEING_EVENT_TO_DB_CHECK( EVENT_JUMPING_CHAR_REWARD_REQ, kDBPacket );
// 
//     SET_ERROR( NET_OK );
// 
// END_PROC:
//     switch ( NetError::GetLastNetError() ) {
//         ERR_CASE( NET_OK, kPacket.m_nOK, 0 );     // 성공
//         ERR_CASE( ERR_JUMPING_CHAR_01, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
//         ERR_CASE( ERR_JUMPING_CHAR_02, kPacket.m_nOK, 2 ); // 이미 점핑 캐릭터 보상을 받았음.
//         ERR_CASE( ERR_JUMPING_CHAR_03, kPacket.m_nOK, 3 ); // 점핑 캐릭터 기본 보상 세팅되지 않았음.
//         ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중인 작업입니다.
//     default:
//         START_LOG( cerr, L"점핑 캐릭터 기본 보상 정보 요청중 알 수 없는 오류 Name : " << GetName() )
//             << BUILD_LOG( NetError::GetLastNetError() )
//             << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
//         kPacket.m_nOK = -99;
//     }
// 
//     if ( NetError::GetLastNetError() != NetError::NET_OK ) {
//         SEND_PACKET( EVENT_JUMPING_CHAR_REWARD_ACK );
//     }
// 
//     _LOG_SUCCESS( kPacket.m_nOK == 0, L"점핑 캐릭터 기본 보상 정보 요청, User : " << GetName() )
//         << BUILD_LOG( kPacket.m_nOK )
//         << END_LOG;
}

IMPL_ON_FUNC( EVENT_JUMPING_CHAR_REWARD_ACK )
{
//     NetError::SetLastNetError( kPacket_.m_nOK );
//     if ( !IS_CORRECT( NET_OK ) ) {
//         goto END_PROC;
//     }
// 
//     m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
//     m_bJumpCharEnable = true;
// 
// END_PROC:
//     switch( NetError::GetLastNetError() )
//     {
//         ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );     // 성공
//         ERR_CASE( ERR_JUMPING_CHAR_01, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님.
//         ERR_CASE( ERR_JUMPING_CHAR_02, kPacket_.m_nOK, 2 ); // 이미 점핑 캐릭터 보상을 받았음.
//         ERR_CASE( ERR_JUMPING_CHAR_03, kPacket_.m_nOK, 3 ); // 점핑 캐릭터 기본 보상 세팅되지 않았음.
//         ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
//     default:
//         START_LOG_WITH_NAME( cerr )
//             << BUILD_LOG( NetError::GetLastNetErrMsg() )
//             << END_LOG;
//         kPacket_.m_nOK = -99; // unknown error
//         break;
//     }
// 
//     SEND_RECEIVED_PACKET( EVENT_JUMPING_CHAR_REWARD_ACK );
// 
//     _LOG_SUCCESS( kPacket_.m_nOK == 0, L"점핑 캐릭터 기본 보상 정보 요청 MSG :" << NetError::GetLastNetErrMsg() )
//         << BUILD_LOG( m_strName)
//         << BUILD_LOG( kPacket_.m_vecRewardItem.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_JUMPING_CHAR_REQ, char )
{
//     SET_ERROR( ERR_UNKNOWN );
// 
//     KEVENT_JUMPING_CHAR_ACK kPacket;
//     KDB_EVENT_JUMPING_CHAR_REQ kDBPacket;
//     std::map<char, KCharacterInfo>::iterator mit;
//     std::vector<int>::iterator vit;
//     std::vector<int>::iterator vitMission;
//     std::vector<int> vecSkillGroupInfo;
//     KMissionInfo kMissionInfo;
//     std::vector<int> vecSkillInfo;
// 
//     // 이벤트 기간인지 체크.
//     if( !SiKJumpingChar()->IsEventTerm() ) {
//         SET_ERR_GOTO( ERR_JUMPING_CHAR_01, END_PROC );
//     }
// 
//     // 점핑보상을 받았는지 체크.
//     if( !m_bJumpCharEnable ) {
//         SET_ERR_GOTO( ERR_JUMPING_CHAR_02, END_PROC );
//     }
// 
//     // 캐릭터를 보유하고 있는지 체크.
//     mit = m_mapCharacterInfo.find( kPacket_ );
//     if( mit == m_mapCharacterInfo.end() ) {
//         kDBPacket.m_kCharacterInfo.m_cCharType = kPacket_;
//         kDBPacket.m_nType = KDB_EVENT_JUMPING_CHAR_REQ::JP_CREATE;
//     } else {
//         kDBPacket.m_kCharacterInfo = mit->second;
//         kDBPacket.m_nType = KDB_EVENT_JUMPING_CHAR_REQ::JP_UPDATE;
//     }
// 
//     if( kDBPacket.m_nType == KDB_EVENT_JUMPING_CHAR_REQ::JP_UPDATE ) {
//         if( SiKJumpingChar()->GetJumpingCharLevel() <= kDBPacket.m_kCharacterInfo.m_dwLevel ) {
//             SET_ERR_GOTO( ERR_JUMPING_CHAR_06, END_PROC );
//         }
//     }
// 
//     if( !SiKJumpingChar()->GetJumpingCharReward( kPacket_, kDBPacket.m_vecReward ) ) {
//         SET_ERR_GOTO( ERR_JUMPING_CHAR_04, END_PROC );
//     }
// 
//     LIF( SiKJumpingChar()->GetJumpingCharMissionClearList( kPacket_, kDBPacket.m_vecCompleteMissionInfo ) );
// 
//     // 스크립트에 등록된 미션이 유효한 미션인지 체크.
//     for( vitMission = kDBPacket.m_vecCompleteMissionInfo.begin(); vitMission != kDBPacket.m_vecCompleteMissionInfo.end(); ++vitMission ) {
//         if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, *vitMission ) ) {
//             START_LOG( cerr, L"Invalid Complete Mission" << *vitMission )
//                 << END_LOG;
//             SET_ERR_GOTO( ERR_JUMPING_CHAR_08, END_PROC );
//         }
//     }
// 
//     // 등록되어 있는 미션정보 가져오기.
//     for( vit = kDBPacket.m_vecCompleteMissionInfo.begin(); vit != kDBPacket.m_vecCompleteMissionInfo.end(); ++vit ) {
//         if( IsRegistedMission( *vit ) ) {
//             kDBPacket.m_setDeleteMissionInfo.insert( *vit );
//         }
//     }
// 
//     LIF( SiKJumpingChar()->GetJumpingCharRegisterMission( kPacket_, kDBPacket.m_vecRegisterMissionInfo ) );
// 
//     // 스크립트에 등록된 미션이 유효한 미션인지 체크.
//     for( vitMission = kDBPacket.m_vecRegisterMissionInfo.begin(); vitMission != kDBPacket.m_vecRegisterMissionInfo.end(); ) {
//         if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, *vitMission ) ) {
//             START_LOG( cerr, L"Invalid Register Mission" << *vitMission )
//                 << END_LOG;
//             SET_ERR_GOTO( ERR_JUMPING_CHAR_08, END_PROC );
//         }
// 
//         // 등록되어 있는 미션인지 체크.
//         if( IsRegistedMission( *vitMission ) ) {
//             vitMission = kDBPacket.m_vecRegisterMissionInfo.erase( vitMission );
//             continue;
//         }
//         ++vitMission;
//     }
// 
//     // 등록되어 있는 미션정보인지 체크.
//     for( vit = kDBPacket.m_vecRegisterMissionInfo.begin(); vit != kDBPacket.m_vecRegisterMissionInfo.end(); ++vit ) {
//         if( IsRegistedMission( *vit ) ) {
//             kDBPacket.m_setDeleteMissionInfo.insert( *vit );
//         }
//     }
// 
//     // 스킬그룹 오픈 가능한지 체크.
//     if( SiKJumpingChar()->GetJumpingCharSkillGroupList( kDBPacket.m_kCharacterInfo.m_cCharType, vecSkillGroupInfo ) ) {
//         std::vector<int>::iterator vit;
//         std::pair<char,int> prData;
//         for( vit = vecSkillGroupInfo.begin(); vit != vecSkillGroupInfo.end(); ++vit ) {
//             prData.first = -1;
//             prData.second = *vit;
// 
//             if( !SiKSkillManager()->IsLockGroupID( *vit, prData.first ) ) {
//                 continue;
//             }
// 
//             if( IsInUnLockedGroup( prData.first, *vit ) ) {
//                 continue;
//             }
// 
//             kDBPacket.m_vecSkillGroupInfo.push_back( *vit );
//         }
//     }
// 
//     // 스킬정보 오픈.
//     if( SiKJumpingChar()->GetJumpingCharSkillList( kDBPacket.m_kCharacterInfo.m_cCharType, vecSkillInfo ) ) {
//         SSkillElement kSkill;
//         for( vit = vecSkillInfo.begin(); vit != vecSkillInfo.end(); ++vit ) {
//             if( !SiKSkillManager()->GetSkill( *vit, kSkill ) ) {
//                 continue;
//             }
// 
//             if( IsTrainedSkill( kSkill ) ) { // 배운 스킬은 넣지 않는다.
//                 continue;
//             }
//             kDBPacket.m_vecRewardSkillInfo.push_back( *vit );
//         }
//     }
// 
//     // DB로그 추가를 위해서 변경되기전 정보 Keep.
//     kDBPacket.m_kCharacterInfoLog = kDBPacket.m_kCharacterInfo;
// 
//     QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_JUMPING_CHAR_REQ, kDBPacket );
// 
//     SET_ERROR( NET_OK );
// 
// END_PROC:
//     switch ( NetError::GetLastNetError() ) {
//         ERR_CASE( NET_OK, kPacket.m_nOK, 0 );     // 성공
//         ERR_CASE( ERR_JUMPING_CHAR_01, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
//         ERR_CASE( ERR_JUMPING_CHAR_02, kPacket.m_nOK, 2 ); // 이미 점핑 캐릭터 보상을 받았음.
//         ERR_CASE( ERR_JUMPING_CHAR_04, kPacket.m_nOK, 4 ); // 점핑 캐릭터 보상 세팅되지 않았음.
//         ERR_CASE( ERR_JUMPING_CHAR_05, kPacket.m_nOK, 5 ); // 점핑 캐릭터 미션 클리어 정보 세팅되지 않았음.
//         ERR_CASE( ERR_JUMPING_CHAR_06, kPacket.m_nOK, 6 ); // 점핑 캐릭터 레벨보다 보유한 캐릭터 레벨이 높음.
//         ERR_CASE( ERR_JUMPING_CHAR_07, kPacket.m_nOK, 7 ); // DB 점핑 캐릭터 생성 실패.
//         ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중인 작업입니다.
//     default:
//         START_LOG( cerr, L"캐릭터 점핑 요청중 알 수 없는 오류 Name : " << GetName() )
//             << BUILD_LOG( NetError::GetLastNetError() )
//             << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
//         kPacket.m_nOK = -99;
//     }
// 
//     if ( NetError::GetLastNetError() != NetError::NET_OK ) {
//         SEND_PACKET( EVENT_JUMPING_CHAR_ACK );
//     }
// 
//     _LOG_SUCCESS( kPacket.m_nOK == 0, L"캐릭터 점핑 요청, User : " << GetName() )
//         << BUILD_LOGc( kPacket_ )
//         << BUILD_LOG( kPacket.m_nOK )
//         << END_LOG;
}

IMPL_ON_FUNC( EVENT_JUMPING_CHAR_ACK )
{
//     std::map<char, KCharacterInfo>::iterator mit;
//     std::vector<int> vecDungeonClearInfo;
//     std::vector<int> vecSkillInfo;
//     std::vector<int>::iterator vitSkill;
//     std::set<int>::iterator sitMission;
//     std::vector<int>::iterator vitMission;
//     std::vector<KMissionData>::iterator vitRegisterMission;
//     std::set< int >::iterator sit;
//     NetError::SetLastNetError( kPacket_.m_nOK );
//     if ( !IS_CORRECT( NET_OK ) ) {
//         goto END_PROC;
//     }
// 
//     if( kPacket_.m_nType == KEVENT_JUMPING_CHAR_ACK::JP_CREATE ) {
//         LIF( m_mapCharacterInfo.insert( std::make_pair( kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo ) ).second );
//         LIF( AddStatInitExpInfo( kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_kCharacterInfo.m_biInitExp, kPacket_.m_kCharacterInfo.m_biInitTotalExp ) );
// 
//         // 신규 캐릭터 획득시 룩 정보 추가.
//         KLookEquipChar kLookEquip;
//         kLookEquip.SetCharacter( kPacket_.m_kCharacterInfo.m_cCharType );
//         m_mapLookEquip.insert( std::map<char,KLookEquipChar>::value_type( kLookEquip.GetCharacter(), kLookEquip ) );
// 
//         // 신규 캐릭터 무기교체 잠금
//         bool bWeaponLock = false;
//         m_mapChangeWeapon[kPacket_.m_kCharacterInfo.m_cCharType] = std::make_pair( bWeaponLock, 0 );
//         kPacket_.m_kCharacterInfo.m_bChangeWeaponLock = bWeaponLock;
//     } else {
//         mit = m_mapCharacterInfo.find( kPacket_.m_kCharacterInfo.m_cCharType );
//         if( mit != m_mapCharacterInfo.end() ) {
//             mit->second = kPacket_.m_kCharacterInfo;
//         }
//     }
// 
//     // 던전 클리어 정보 넣어주기.
//     if( SiKJumpingChar()->GetJumpingCharDungeonClearList( kPacket_.m_kCharacterInfo.m_cCharType, vecDungeonClearInfo ) ) {
//         std::vector<int>::iterator vit;
//         for( vit = vecDungeonClearInfo.begin(); vit != vecDungeonClearInfo.end(); ++vit ) {
//             std::map<int,KDungeonUserInfo>::iterator mit;
//             mit  = m_mapDifficulty.find( *vit );
//             if( mit == m_mapDifficulty.end() ) {
//                 KDungeonUserInfo kInfo;
//                 m_mapDifficulty.insert( std::make_pair( *vit, kInfo ) );
//             }
//             int nLastDifficulty = SiKResultManager()->GetLastDifficulty( *vit );
//             if ( nLastDifficulty > -1 ) {
//                 for( int i = 0; i <= nLastDifficulty; ++i ) {
//                     m_mapDifficulty[*vit].m_kModeDifficulty.Set( i );
//         }
//                 m_mapDifficulty[*vit].m_kClearData.m_shMaxDifficulty = static_cast<SHORT>( nLastDifficulty );
//                 m_mapDifficulty[*vit].m_kClearData.m_shLastDifficulty = static_cast<SHORT>( nLastDifficulty );
//                 m_mapDifficulty[*vit].m_kClearData.m_bClear = true;
//     }
//         }
//     }
//     kPacket_.m_mapDifficulty = m_mapDifficulty; // 던전정보를 전달한다.
// 
//     // 스킬 정보 저장.
//     for( vitSkill = kPacket_.m_vecRewardSkillInfo.begin(); vitSkill != kPacket_.m_vecRewardSkillInfo.end(); ++vitSkill ) {
//         SSkillElement kSkill;
//         if( !SiKSkillManager()->GetSkill( *vitSkill, kSkill ) ) {
//             continue;
//         }
//         LIF( InsertSkillInfo( kSkill ) );
//     }
// 
//     for( vitSkill = kPacket_.m_vecUnLockSkillGroup.begin(); vitSkill != kPacket_.m_vecUnLockSkillGroup.end(); ++vitSkill ) {
//         AddUnLockedGroup( kPacket_.m_kCharacterInfo.m_cCharType, *vitSkill );
//     }
// 
//     GetSkillInfoData( kPacket_.m_kCharacterInfo.m_cCharType, kPacket_.m_vecSPInfo );
// 
// 
//     for( sitMission = kPacket_.m_setDeleteMissionInfo.begin(); sitMission != kPacket_.m_setDeleteMissionInfo.end(); ++sitMission ) {
//         EraseMissionDataFromSlot( *sitMission );
//     }
// 
//     for( vitMission = kPacket_.m_vecCompleteMissionInfo.begin(); vitMission != kPacket_.m_vecCompleteMissionInfo.end(); ++vitMission ) {
//         UpdateCompleteMission( *vitMission );
//     }
// 
//     // 등록된 미션 추가.
//     for( vitRegisterMission = kPacket_.m_vecMissionSlot.begin(); vitRegisterMission != kPacket_.m_vecMissionSlot.end(); ++vitRegisterMission ) {
//         m_vecMissionSlot.push_back( *vitRegisterMission );
//     }
// 
//     kPacket_.m_vecMissionSlot.clear(); // 초기화.
//     kPacket_.m_vecMissionSlot = m_vecMissionSlot;
// 
//     // 캐릭터 정보 다시 가져오기.
//     mit = m_mapCharacterInfo.find( kPacket_.m_kCharacterInfo.m_cCharType );
//     if( mit != m_mapCharacterInfo.end() ) {
//         kPacket_.m_kCharacterInfo = mit->second;
//     }
// 
//     m_kInventory.AddItemList( kPacket_.m_vecRewardItem );
//     m_bJumpCharEnable = true;
// 
//     // 캐릭터 전용 던전 캐릭터인지 체크.
//     sit = m_setNonGraduateCharacters.find( kPacket_.m_kCharacterInfo.m_cCharType );
//     if( sit != m_setNonGraduateCharacters.end() ) {
//         m_setNonGraduateCharacters.erase( sit );
//         }
// 
// END_PROC:
//     switch( NetError::GetLastNetError() )
//     {
//         ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );     // 성공
//         ERR_CASE( ERR_JUMPING_CHAR_01, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님.
//         ERR_CASE( ERR_JUMPING_CHAR_02, kPacket_.m_nOK, 2 ); // 이미 점핑 캐릭터 보상을 받았음.
//         ERR_CASE( ERR_JUMPING_CHAR_04, kPacket_.m_nOK, 4 ); // 점핑 캐릭터 보상 세팅되지 않았음.
//         ERR_CASE( ERR_JUMPING_CHAR_05, kPacket_.m_nOK, 5 ); // 점핑 캐릭터 미션 클리어 정보 세팅되지 않았음.
//         ERR_CASE( ERR_JUMPING_CHAR_06, kPacket_.m_nOK, 6 ); // 점핑 캐릭터 레벨보다 보유한 캐릭터 레벨이 높음.
//         ERR_CASE( ERR_JUMPING_CHAR_07, kPacket_.m_nOK, 7 ); // DB 점핑 캐릭터 생성 실패.
//         ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 처리중인 작업입니다.
//     default:
//         START_LOG_WITH_NAME( cerr )
//             << BUILD_LOG( NetError::GetLastNetErrMsg() )
//             << END_LOG;
//         kPacket_.m_nOK = -99; // unknown error
//         break;
//     }
// 
//     SEND_RECEIVED_PACKET( EVENT_JUMPING_CHAR_ACK );
// 
//     _LOG_SUCCESS( kPacket_.m_nOK == 0, L"점핑 캐릭터 기본 보상 정보 요청 MSG :" << NetError::GetLastNetErrMsg() )
//         << BUILD_LOG( m_strName)
//         << BUILD_LOG( kPacket_.m_vecRewardItem.size() ) << END_LOG;
}

_IMPL_ON_FUNC(EVENT_GACHA_PONG_PLAY_INFO_REQ, DWORD)
{
    if (SiKGachaPong()->IsRun())
    {
        //The player will receive the broadcast info
        SiKGachaPong()->UnsubscribePlayer(m_dwUID); //Verify if the player was playing in another machine
        SiKGachaPong()->SubscribePlayer(kPacket_, m_dwUID);

        int iMachineCapsules = SiKGachaPong()->GetMachineCapsules(kPacket_);
        int iMachineRewardCount = SiKGachaPong()->GetMachineMainRewardCnt(kPacket_);
        DWORD dwCoin = SiKGachaPong()->GetMachineCoin(kPacket_);
        DWORD dwRewardID = SiKGachaPong()->GetMachineRewardID(kPacket_);

        KEVENT_GACHA_PONG_PLAY_INFO_NOT kPacket;
        kPacket.m_dwLeftCapsules = iMachineCapsules;
        kPacket.m_dwMainRewardCnt = iMachineRewardCount;
        kPacket.m_prMachineInfo.first = kPacket_;		//machine number
        kPacket.m_prMachineInfo.second = dwRewardID;	//RewardID

        START_LOG(clog, L"Gacha Pon Machine Info ")
            << BUILD_LOG(m_dwUID)
            << BUILD_LOG(iMachineCapsules)
            << BUILD_LOG(iMachineRewardCount)
            << BUILD_LOG(dwCoin)
            << BUILD_LOG(dwRewardID)
            << END_LOG;

        SEND_COMPRESS_PACKET(EVENT_GACHA_PONG_PLAY_INFO_NOT, kPacket);
    }
}

IMPL_ON_FUNC_NOPARAM(EVENT_GACHA_PONG_PLAY_INFO_UNSUBSCRIBE_REQ)
{
    if (SiKGachaPong()->IsRun())
    {
        START_LOG(clog, L"Unsubscribe player ")
            << BUILD_LOG(m_strName)
            << END_LOG;

        SiKGachaPong()->UnsubscribePlayer(m_dwUID);
    }
}

_IMPL_ON_FUNC(EVENT_GACHA_PONG_ACTION_REQ, PAIR_DWORD)
{
    //kPacket_.first	--> Machine Number
    //kPacket_.second	--> Main RewardId

    KEVENT_GACHA_PONG_ACTION_ACK kPacket;
    KDB_EVENT_GACHA_PONG_ACTION_REQ kDBPacket;
    kPacket.m_nOK = -99;
    SET_ERROR(ERR_UNKNOWN);

    if (SiKGachaPong()->IsRun())
    {
        GCITEMID dwMachineCoin = SiKGachaPong()->GetMachineCoin(kPacket_.first);

        KItemPtr pkItem = m_kInventory.FindDurationItemByItemID(dwMachineCoin);

        if (!pkItem || pkItem->m_nCount < 1)
        {
            SET_ERR_GOTO(ERR_GACHA_PONG_01, END_PROC);
        }

        if (kPacket_.second != SiKGachaPong()->GetMachineRewardID(kPacket_.first))
        {
            SET_ERR_GOTO(ERR_GACHA_PONG_02, END_PROC);
        }

        kPacket.m_bSuccess = SiKGachaPong()->CrackABall(kPacket_.first, kDBPacket.m_kRewardItem, kDBPacket.m_iCapsule, kDBPacket.m_nNumberOfCapsule);

        kDBPacket.m_dwMachineNumber = kPacket_.first;
        kDBPacket.m_MainRewardItemID = kPacket_.second;
        kDBPacket.m_kGachaPonCoin = *pkItem;

        QUEUEING_EVENT_TO_DB_CHECK(DB_EVENT_GACHA_PONG_ACTION_REQ, kDBPacket);

        SET_ERROR(NET_OK);
    }

END_PROC:
    switch (NetError::GetLastNetError())
    {
        ERR_CASE(NET_OK, kPacket.m_nOK, 0); // All fine
        ERR_CASE(ERR_GACHA_PONG_01, kPacket.m_nOK, 0x70D); // Player don't have coins to play
        ERR_CASE(ERR_GACHA_PONG_02, kPacket.m_nOK, 0x70B); // The Reward information mismatch from configuration
    default:
        START_LOG(cerr, L" Failed to crack the gacha pon capsule : " << NetError::GetLastNetErrMsg())
            << BUILD_LOG(m_strName) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS(kPacket.m_nOK == 0, L"Gacha Pon Result, User : " << GetName())
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(m_strName)
        << BUILD_LOG(NetError::GetLastNetErrMsg())
        << BUILD_LOG(kPacket.m_bSuccess)
        << BUILD_LOG(kDBPacket.m_kRewardItem.m_ItemID)
        << END_LOG;

    if (NetError::GetLastNetError() != NetError::NET_OK)
    {
        SEND_PACKET(EVENT_GACHA_PONG_ACTION_ACK);
    }
}

IMPL_ON_FUNC(EVENT_GACHA_PONG_PLAY_INFO_NOT)
{
    SEND_RECEIVED_PACKET(EVENT_GACHA_PONG_PLAY_INFO_NOT);
}

IMPL_ON_FUNC(EVENT_GACHA_PONG_ACTION_ACK)
{
    KItemPtr pkGachaPonCoin;
    std::wstring wstrData;
    std::wstring wstrGrade;
    GCITEMID GachaPonCoinID = SiKGachaPong()->GetMachineCoin(kPacket_.m_prMachineInfo.first);

    NetError::SetLastNetError(kPacket_.m_nOK);
    if (!IS_CORRECT(NET_OK))
    {
        kPacket_.m_bSuccess = false;
        goto END_PROC;
    }

    pkGachaPonCoin = m_kInventory.FindDurationItemByItemID(GachaPonCoinID);
    if (pkGachaPonCoin)
    {
        pkGachaPonCoin->m_nCount = kPacket_.m_kGachaPonCoin.m_nCount;
        pkGachaPonCoin->m_nInitCount = kPacket_.m_kGachaPonCoin.m_nCount;
        //kPacket_.m_vecReward.push_back( *pkGachaPonCoin );
        kPacket_.m_kGachaPonCoin = *pkGachaPonCoin;
    }

    m_kInventory.AddItemList(kPacket_.m_vecRewardItems);

    // °¼Â÷ º¹±Ç µî±Þ º¸»ó °øÁö.
    if (SiKGachaPong()->GetGachaPongNoticeEnable() && kPacket_.m_bSuccess)
    {
        if (SiKLoudMessage()->GetGachaPonMsg(m_strNickName, wstrData, GetLanguageCode()))
        {
            SiKGachaPong()->SendGachaPongMsg(wstrData);
        }
    }

END_PROC:
    switch (NetError::GetLastNetError()) {
        ERR_CASE(NET_OK, kPacket_.m_nOK, 0);
        ERR_CASE(ERR_GACHA_PONG_01, kPacket_.m_nOK, 0x70D); // Player don't have coins to play
        ERR_CASE(ERR_GACHA_PONG_02, kPacket_.m_nOK, 0x70B); // The Reward information mismatch from configuration
        ERR_CASE(ERR_GACHA_PONG_03, kPacket_.m_nOK, 3); // 
        ERR_CASE(ERR_GACHA_PONG_04, kPacket_.m_nOK, 4); // 
        ERR_CASE(ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98); // 
    default:
        START_LOG(cerr, L"Wrong code recvd. Name : " << GetName())
            << BUILD_LOG(NetError::GetLastNetError())
            << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    SEND_RECEIVED_PACKET(EVENT_GACHA_PONG_ACTION_ACK);

    _LOG_SUCCESS(kPacket_.m_nOK == 0, L"Capsule obtained from Gacha Pon, User : " << GetName())
        << BUILD_LOG(m_dwUID)
        << BUILD_LOG(kPacket_.m_prMachineInfo.first)
        << BUILD_LOG(kPacket_.m_prMachineInfo.second)
        << BUILD_LOG(kPacket_.m_kGachaPonCoin.m_nCount)
        << BUILD_LOG(kPacket_.m_vecRewardItems.size())
        << END_LOG;
}