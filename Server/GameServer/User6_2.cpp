#include "User.h"
#include <dbg/dbg.hpp>
#include "UserFSM.h"
#include "GSSimLayer.h"
#include "Channel.h"
#include "GameServer.h"
#include "PetHelper.h"
#include <boost/algorithm/string.hpp> // boost::trim
#include "SlangFilter.h"
#include "GCHelper.h"
#include "SquareManager.h"
#include "Square.h"
#include "ChannelManager.h"
#include "SignBoard.h"
#include "LogManager.h"
#include "support_User.h"
#include "LoudMessage.h"
#include "ResultManager.h"
#include "SkillManager.h"
#include "GachaManager.h"
#include "FailRate.h"
#include "CoupleSystem.h"
#include "StatCoupleRoom.h"
#include "DonationManager.h"
#include "LunaNewYear.h"
#include "CharacterPollManager.h"
#include "GPCapsuleManager.h"
#include "GameCouponManager.h"
#include "RecomHelper.h"
#include "CnConnector.h"
#include "PreHackingCheckManager.h"
#include "MissionPack.h"
#include "Rainbow.h"
#include "CooperationEvent.h"
#include "MissionManager.h"
#include "ItemTradeManager.h"
#include "MissionEvent.h"
#include "SubscriptionManager.h"
#include "NpcGift.h"
#include "DicePlay.h"
#include "CountType.h"
#include "GWCSupport.h"
#include "MissionInfo.h"
#include "StatLoadingInfo.h"
#include "ItemManager.h"
#include <boost/lexical_cast.hpp>

#define CLASS_TYPE  KUser

IMPL_ON_FUNC( EVENT_GACHA_REWARD_LIST_REQ )
{
    VERIFY_STATE( ( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ) );

    bool bClear = CheckGachaObtainedByType( kPacket_.m_nVersion, KGachaManager::GRT_TOTAL_REWARD );
    KGachaRewardList kPacket;
    kPacket.m_dwGachaItemID = kPacket_.m_dwGachaItemID;
    kPacket.m_nVersion = kPacket_.m_nVersion;

    SiKGachaManager()->GetRewardItemList( kPacket_.m_nVersion, kPacket.m_dwGachaItemID, kPacket.m_mapCollectionList );
    kPacket.m_bAccessoriesEnable = SiKGachaManager()->GetAccessoriesEnable( kPacket_.m_nVersion );
    SiKGachaManager()->GetGachaNeedKeyInfo( kPacket_.m_nVersion, kPacket.m_dwGachaItemID, bClear, kPacket.m_mapNeedKeyInfo );

    SEND_PACKET( EVENT_GACHA_REWARD_LIST_ACK );
    START_LOG( clog, L"가차 보상 아이템 리스트 응답.. Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOG( kPacket_.m_dwGachaItemID )
        << BUILD_LOG( kPacket.m_bAccessoriesEnable )
        << BUILD_LOG( kPacket.m_dwGachaItemID )
        << BUILD_LOG( kPacket.m_mapCollectionList.size() )
        << BUILD_LOG( kPacket.m_mapNeedKeyInfo.size() )
        << BUILD_LOG( bClear )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_GACHA_ACTION_REQ )
{
    VERIFY_STATE( ( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ) );

    KEVENT_GACHA_ACTION_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkGachaItem;
    KItemPtr pkKeyItem;
    int nNeedKeyCount = INT_MAX;
    bool bGainReelPointItem = false; // 릴포인트로 아이템을 받았는지 여부.
    bool bClear = CheckGachaObtainedByType( kPacket_.m_nVersion, KGachaManager::GRT_TOTAL_REWARD );
    KDB_EVENT_GACHA_ACTION_REQ kDBPacket_;
    KCharacterInfo* pkCharInfo = NULL;
    std::map< int, GCITEMID > mapNotExistList;

    SET_ERROR( ERR_UNKNOWN );

    // 해당 아이템이 가차 아이템인가?
    if ( false == SiKGachaManager()->IsGachaItem( kPacket_.m_nVersion, kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_GACHA_03, END_PROC );
    }

    // 해당 Type이 현재 오픈되어 있는가( 장비, 악세사리 )
    if ( false == SiKGachaManager()->IsGachaTypeEnable( kPacket_.m_nVersion, kPacket_.m_dwType ) ) {
        SET_ERR_GOTO( ERR_GACHA_12, END_PROC );
    }

    // 해당 아이템을 보유 하고 있는가?
    pkGachaItem = m_kInventory.FindItemByItemID( kPacket_.m_ItemID );
    if ( !pkGachaItem ) {
        SET_ERR_GOTO( ERR_GACHA_04, END_PROC );
    }

    if ( pkGachaItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_GACHA_04, END_PROC );
    }

    // 열쇠 아이템은 보유 하고 있는가?
    pkKeyItem =  m_kInventory.FindItemByItemID( SiKGachaManager()->GetKeyItemID( kPacket_.m_nVersion, kPacket_.m_ItemID ) );
    if ( !pkKeyItem ) {
        SET_ERR_GOTO( ERR_GACHA_05, END_PROC );
    }

    if ( pkKeyItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_GACHA_05, END_PROC );
    }

    // 열쇠 아이템의 수량은 충분한가.
    nNeedKeyCount = SiKGachaManager()->GetNeedKeyCount( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_nLevel, bClear );
    if ( pkKeyItem->m_nCount < nNeedKeyCount ) {
        SET_ERR_GOTO( ERR_GACHA_05, END_PROC );
    }

    // 가챠아이템ID로 해당 수집아이템List 받아오기
    if ( false == GetGachaNotExistList( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_dwType, mapNotExistList ) ) {
        SET_ERR_GOTO( ERR_GACHA_07, END_PROC ); // 가차 수집 아이템 리스트가 없습니다.
    }

    // Gamble
    if ( false == SiKGachaManager()->DoGamble( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_dwType, kPacket_.m_nLevel, kPacket_.m_kPostItem, kPacket_.m_bRewardType ) ) {
        goto END_PROC;
    }

    // Reel Point 사용시
    if ( true == SiKGachaManager()->IsReelPointEnable( kPacket_.m_nVersion ) ) {
        float fRate = static_cast< float >( rand() ) / static_cast< float >( RAND_MAX );
        int nReelPointSum = GetGachaReelPointSum( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_dwType, kPacket_.m_nLevel );

        // RP합산
        nReelPointSum += CalcGachaReelPoint( kPacket_.m_nVersion, kPacket_.m_ItemID, 2, fRate );

        // 현재 릴포인트가 스크립트에 지정된 MaxReelPoint 보다 높으면, 내가 못받은 아이템이 있으면.. 없는 아이템중 하나를 랜덤지급
        int nMaxReelPoint = SiKGachaManager()->GetGachaMaxReelPoint( kPacket_.m_nVersion );
        if ( nReelPointSum >= nMaxReelPoint && false == mapNotExistList.empty() ) {
            int nIdx = static_cast< int >( ( mapNotExistList.size() * fRate ) );
            if ( nIdx >= static_cast< int >( mapNotExistList.size() ) || nIdx < 0 ) {
                nIdx = 0; // 범위 안넘어가도록 보정(안전코드)
            }

            // 선택한 아이템 정보 받아오기(실패시 내부에서 에러값 설정)
            if ( false == SiKGachaManager()->GetGachaCollectItem( kPacket_.m_nVersion, kPacket_.m_ItemID, mapNotExistList[ nIdx ], kPacket_.m_kPostItem, kPacket_.m_dwType ) ) {
                SET_ERR_GOTO( ERR_GACHA_03, END_PROC );
            }

            // RP로 아이템을 획득했으니, RP 초기화.
            nReelPointSum = 0;

            START_LOG( clog, L"RP로 아이템 획득함." )
                << BUILD_LOG( kPacket_.m_ItemID )
                << BUILD_LOG( mapNotExistList[ nIdx ] )
                << BUILD_LOG( kPacket_.m_nVersion )
                << BUILD_LOG( kPacket_.m_dwType )
                << BUILD_LOG( kPacket_.m_kPostItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_kPostItem.m_nDuration )
                << BUILD_LOG( kPacket_.m_kPostItem.m_nPeriod )
                << END_LOG;

            // 아이템 받았다.
            bGainReelPointItem = true;
            kPacket_.m_bRewardType = true;
        }

        // RP 갱신
        SetGachaReelPointSum( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_dwType, kPacket_.m_nLevel, nReelPointSum );
    }

    if ( false == bGainReelPointItem ) {
        // 이미 보유한 수집 아이템인지 확인
        if ( true == SiKGachaManager()->IsCollectionItem( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_kPostItem.m_ItemID, kPacket_.m_dwType ) ) {
            // 보유중인 수집 아이템이면. 꽝 대체 아이템에서 선택 한다.
            if ( ( NULL != m_kInventory.FindItemByItemID( kPacket_.m_kPostItem.m_ItemID ) )
                || ( NULL != m_kInventory.FindDepotItemByItemID( kPacket_.m_kPostItem.m_ItemID ) ) ) {
                if ( false == SiKGachaManager()->DoGambleFail( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_nLevel, kPacket_.m_kPostItem ) ) {
                    goto END_PROC;
                }
                kPacket_.m_bRewardType = false;
            }
        }
    }

    // DBQueuing
    kDBPacket_.m_nVersion = kPacket_.m_nVersion;
    kDBPacket_.m_dwType = kPacket_.m_dwType;
    kDBPacket_.m_nLevel = kPacket_.m_nLevel;
    kDBPacket_.m_ItemID = kPacket_.m_ItemID;
    kDBPacket_.m_keyItem = *pkKeyItem;
    kDBPacket_.m_kGachaItem = *pkGachaItem;
    kDBPacket_.m_kPostItem = kPacket_.m_kPostItem;
    kDBPacket_.m_bRewardType = kPacket_.m_bRewardType;
    kDBPacket_.m_nNeedKeyCount = nNeedKeyCount;

    // 로그 기록을 위해 캐릭터 정보 추가
    kDBPacket_.m_cCharType = kPacket_.m_cCharType;
    kDBPacket_.m_dwCharLevel = 0; // 캐릭터 정보가 없을 경우 
    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if ( NULL != pkCharInfo ) {
        kDBPacket_.m_dwCharLevel = pkCharInfo->m_dwLevel;
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GACHA_ACTION_REQ, kDBPacket_ );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GACHA_01,     kPacket.m_nOK, 1 ); // 꽝 입니다.
        ERR_CASE( ERR_GACHA_02,     kPacket.m_nOK, 2 ); // 가차 아이템인데 로터리에 등록 되지 않음.
        ERR_CASE( ERR_GACHA_03,     kPacket.m_nOK, 3 ); // 해당 아이템은 가차 아이템이 아님.
        ERR_CASE( ERR_GACHA_04,     kPacket.m_nOK, 4 ); // 해당 가차 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_GACHA_05,     kPacket.m_nOK, 5 ); // 충분한 열쇠 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_DB_COMMON_00, kPacket.m_nOK, 6 ); // 트랜잭션 에러 )
        ERR_CASE( ERR_DB_COMMON_01, kPacket.m_nOK, 7 ); // DB에 해당 유저가 존재하지 않음
        ERR_CASE( ERR_DB_COMMON_02, kPacket.m_nOK, 8 ); // DB에 존재하지 않는 아이템. )
        ERR_CASE( ERR_GACHA_07,     kPacket.m_nOK, 9 ); // 가차 수집 아이템 리스트가 없습니다.
        ERR_CASE( ERR_GACHA_12,     kPacket.m_nOK, 10 ); // 가차 아이템 DB 동기화 실패. )
        ERR_CASE( ERR_GACHA_00,     kPacket.m_nOK, 11 ); // 등록되지 않은 가차 아이템. )
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"가차 돌리는 중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GACHA_ACTION_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"가차 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nVersion )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_nVersion ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_GACHA_LEVEL_ACTION_REQ, KEVENT_GACHA_ACTION_REQ )
{ // ACK는 EVENT_GACHA_ACTION_ACK 를 사용한다.
    VERIFY_STATE( ( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ) );

    KEVENT_GACHA_ACTION_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkGachaItem;
    KItemPtr pkKeyItem;
    int nNeedKeyCount = INT_MAX;
    bool bGainReelPointItem = false; // 릴포인트로 아이템을 받았는지 여부.
    bool bClear = CheckGachaObtainedByType_Level( kPacket_.m_nVersion, KGachaManager::GRT_TOTAL_REWARD );
    KDB_EVENT_GACHA_ACTION_REQ kDBPacket_;
    KCharacterInfo* pkCharInfo = NULL;
    std::map< int, GCITEMID > mapNotExistList;

    SET_ERROR( ERR_UNKNOWN );

    // 해당 아이템이 가차 아이템인가?
    if ( false == SiKGachaManager()->IsGachaItem( kPacket_.m_nVersion, kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_GACHA_03, END_PROC );
    }

    // 해당 Type이 현재 오픈되어 있는가( 장비, 악세사리 )
    if ( false == SiKGachaManager()->IsGachaTypeEnable( kPacket_.m_nVersion, kPacket_.m_dwType ) ) {
        SET_ERR_GOTO( ERR_GACHA_12, END_PROC );
    }

    // 해당 아이템을 보유 하고 있는가?
    pkGachaItem = m_kInventory.FindItemByItemID( kPacket_.m_ItemID );
    if ( !pkGachaItem ) {
        SET_ERR_GOTO( ERR_GACHA_04, END_PROC );
    }

    if ( pkGachaItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_GACHA_04, END_PROC );
    }

    // 열쇠 아이템은 보유 하고 있는가?
    pkKeyItem =  m_kInventory.FindItemByItemID( SiKGachaManager()->GetKeyItemID( kPacket_.m_nVersion, kPacket_.m_ItemID ) );
    if ( !pkKeyItem ) {
        SET_ERR_GOTO( ERR_GACHA_05, END_PROC );
    }

    if ( pkKeyItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_GACHA_05, END_PROC );
    }

    // 열쇠 아이템의 수량은 충분한가.
    nNeedKeyCount = SiKGachaManager()->GetNeedKeyCount( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_nLevel, bClear );
    if ( pkKeyItem->m_nCount < nNeedKeyCount ) {
        SET_ERR_GOTO( ERR_GACHA_05, END_PROC );
    }

    // 가챠아이템ID로 해당 수집아이템List 받아오기
    if ( false == GetGachaNotExistList_Level( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_dwType, kPacket_.m_nLevel, mapNotExistList ) ) {
        SET_ERR_GOTO( ERR_GACHA_07, END_PROC ); // 가차 수집 아이템 리스트가 없습니다.
    }

    // Gamble
    if ( false == SiKGachaManager()->DoGamble( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_dwType, kPacket_.m_nLevel, kPacket_.m_kPostItem, kPacket_.m_bRewardType ) ) {
        goto END_PROC;
    }

    // Reel Point 사용시
    if ( true == SiKGachaManager()->IsReelPointEnable( kPacket_.m_nVersion ) ) {
        float fRate = static_cast< float >( rand() ) / static_cast< float >( RAND_MAX );
        int nReelPointSum = GetGachaReelPointSum( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_dwType, kPacket_.m_nLevel );

        // RP합산
        nReelPointSum += CalcGachaReelPoint( kPacket_.m_nVersion, kPacket_.m_ItemID, 2, fRate );

        // 현재 릴포인트가 스크립트에 지정된 MaxReelPoint 보다 높으면, 내가 못받은 아이템이 있으면.. 없는 아이템중 하나를 랜덤지급
        int nMaxReelPoint = SiKGachaManager()->GetGachaMaxReelPoint( kPacket_.m_nVersion );
        if ( nReelPointSum >= nMaxReelPoint && false == mapNotExistList.empty() ) {
            int nIdx = static_cast< int >( ( mapNotExistList.size() * fRate ) );
            if ( nIdx >= static_cast< int >( mapNotExistList.size() ) || nIdx < 0 ) {
                nIdx = 0; // 범위 안넘어가도록 보정(안전코드)
            }

            // 선택한 아이템 정보 받아오기(실패시 내부에서 에러값 설정)
            if ( false == SiKGachaManager()->GetGachaCollectItem( kPacket_.m_nVersion, kPacket_.m_ItemID, mapNotExistList[ nIdx ], kPacket_.m_kPostItem, kPacket_.m_dwType ) ) {
                SET_ERR_GOTO( ERR_GACHA_03, END_PROC );
            }

            // RP로 아이템을 획득했으니, RP 초기화.
            nReelPointSum = 0;

            START_LOG( clog, L"RP로 아이템 획득함." )
                << BUILD_LOG( kPacket_.m_ItemID )
                << BUILD_LOG( mapNotExistList[ nIdx ] )
                << BUILD_LOG( kPacket_.m_nVersion )
                << BUILD_LOG( kPacket_.m_dwType )
                << BUILD_LOG( kPacket_.m_kPostItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_kPostItem.m_nDuration )
                << BUILD_LOG( kPacket_.m_kPostItem.m_nPeriod )
                << END_LOG;

            // 아이템 받았다.
            bGainReelPointItem = true;
            kPacket_.m_bRewardType = true;
        }

        // RP 갱신
        SetGachaReelPointSum( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_dwType, kPacket_.m_nLevel, nReelPointSum );
    }

    if ( false == bGainReelPointItem ) {
        // 이미 보유한 수집 아이템인지 확인
        if ( true == SiKGachaManager()->IsCollectionItem( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_kPostItem.m_ItemID, kPacket_.m_dwType ) ) {
            // 보유중인 수집 아이템이면. 꽝 대체 아이템에서 선택 한다.
            if ( ( NULL != m_kInventory.FindItemByItemIDandLevel( kPacket_.m_kPostItem.m_ItemID, kPacket_.m_nLevel ) )
                || ( NULL != m_kInventory.FindDepotItemByItemIDandLevel( kPacket_.m_kPostItem.m_ItemID, kPacket_.m_nLevel ) ) ) {
                if ( false == SiKGachaManager()->DoGambleFail( kPacket_.m_nVersion, kPacket_.m_ItemID, kPacket_.m_nLevel, kPacket_.m_kPostItem ) ) {
                    goto END_PROC;
                }
                kPacket_.m_bRewardType = false;
            }
        }
    }

    // DBQueuing
    kDBPacket_.m_nVersion = kPacket_.m_nVersion;
    kDBPacket_.m_dwType = kPacket_.m_dwType;
    kDBPacket_.m_nLevel = kPacket_.m_nLevel;
    kDBPacket_.m_ItemID = kPacket_.m_ItemID;
    kDBPacket_.m_keyItem = *pkKeyItem;
    kDBPacket_.m_kGachaItem = *pkGachaItem;
    kDBPacket_.m_kPostItem = kPacket_.m_kPostItem;
    kDBPacket_.m_bRewardType = kPacket_.m_bRewardType;
    kDBPacket_.m_nNeedKeyCount = nNeedKeyCount;

    // 로그 기록을 위해 캐릭터 정보 추가
    kDBPacket_.m_cCharType = kPacket_.m_cCharType;
    kDBPacket_.m_dwCharLevel = 0; // 캐릭터 정보가 없을 경우 
    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if ( NULL != pkCharInfo ) {
        kDBPacket_.m_dwCharLevel = pkCharInfo->m_dwLevel;
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GACHA_ACTION_REQ, kDBPacket_ );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GACHA_01,     kPacket.m_nOK, 1 ); // 꽝 입니다.
        ERR_CASE( ERR_GACHA_02,     kPacket.m_nOK, 2 ); // 가차 아이템인데 로터리에 등록 되지 않음.
        ERR_CASE( ERR_GACHA_03,     kPacket.m_nOK, 3 ); // 해당 아이템은 가차 아이템이 아님.
        ERR_CASE( ERR_GACHA_04,     kPacket.m_nOK, 4 ); // 해당 가차 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_GACHA_05,     kPacket.m_nOK, 5 ); // 충분한 열쇠 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_DB_COMMON_00, kPacket.m_nOK, 6 ); // 트랜잭션 에러 )
        ERR_CASE( ERR_DB_COMMON_01, kPacket.m_nOK, 7 ); // DB에 해당 유저가 존재하지 않음
        ERR_CASE( ERR_DB_COMMON_02, kPacket.m_nOK, 8 ); // DB에 존재하지 않는 아이템. )
        ERR_CASE( ERR_GACHA_07,     kPacket.m_nOK, 9 ); // 가차 수집 아이템 리스트가 없습니다.
        ERR_CASE( ERR_GACHA_12,     kPacket.m_nOK, 10 ); // 가차 아이템 DB 동기화 실패. )
        ERR_CASE( ERR_GACHA_00,     kPacket.m_nOK, 11 ); // 등록되지 않은 가차 아이템. )
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"가차 돌리는 중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GACHA_ACTION_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"가차 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nVersion )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_nVersion ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GACHA_ACTION_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );

    KItemPtr pkGachaItem;
    KItemPtr pkKeyItem;
    int nNeedKeyCount = INT_MAX;
    bool bClear = CheckGachaObtainedByType( kPacket_.m_nVersion, KGachaManager::GRT_TOTAL_REWARD );

    if ( !IS_CORRECT( NET_OK ) && !IS_CORRECT( ERR_GACHA_12 ) && !IS_CORRECT( ERR_GACHA_13 ) ) {
        goto END_PROC;
    }

    // 가차 아이템 찾아서 숫자 하나 줄인다.
    pkGachaItem = m_kInventory.FindItemByItemID( kPacket_.m_gachaItem.m_ItemID );
    pkKeyItem = m_kInventory.FindItemByItemID( SiKGachaManager()->GetKeyItemID( kPacket_.m_nVersion, kPacket_.m_gachaItem.m_ItemID ) );
    nNeedKeyCount = SiKGachaManager()->GetNeedKeyCount( kPacket_.m_nVersion, kPacket_.m_gachaItem.m_ItemID, kPacket_.m_nLevel, bClear );

    if ( pkGachaItem && pkGachaItem->m_nCount > 0 ) {
        pkGachaItem->m_nCount = kPacket_.m_gachaItem.m_nCount;
        pkGachaItem->m_nInitCount = kPacket_.m_gachaItem.m_nCount;
        kPacket_.m_gachaItem = *pkGachaItem;
    }
    else {
        START_LOG( cerr, L"가차 응답에서 가차 아이템을 찾을수 없음. Name : " << m_strName )
            << BUILD_LOG( kPacket_.m_gachaItem.m_ItemID ) << END_LOG;
    }

    if ( pkGachaItem && pkGachaItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_gachaItem.m_ItemID, kPacket_.m_gachaItem.m_ItemUID );
    }

    // 열쇠 아이템 찾아서 지정된 수만큼 줄인다.
    if ( pkKeyItem ) {
        //pkKeyItem->m_nCount = std::max<int>( 0, pkKeyItem->m_nCount - nNeedKeyCount );
        pkKeyItem->m_nCount = kPacket_.m_keyItem.m_nCount;
        pkKeyItem->m_nInitCount = kPacket_.m_keyItem.m_nCount;
        kPacket_.m_keyItem = *pkKeyItem;
    }
    else {
        START_LOG( cerr, L"가차 응답에서 열쇠 아이템을 찾을수 없음. Name : " << m_strName )
            << BUILD_LOG( SiKGachaManager()->GetKeyItemID( kPacket_.m_nVersion, kPacket_.m_gachaItem.m_ItemID ) )
            << BUILD_LOG( nNeedKeyCount ) << END_LOG;
    }

    if ( pkKeyItem && pkKeyItem->m_nCount <= 0 ) {
        m_kInventory.RemoveItem( kPacket_.m_keyItem.m_ItemID, kPacket_.m_keyItem.m_ItemUID );
    }


    // 열쇠나 가챠아이템을 넣어주는 경우, 현재 수량 꼬이지 않도록.
    m_kInventory.AddItemList( kPacket_.m_vecReward );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_GACHA_01,         kPacket_.m_nOK, 1 );    // 꽝 입니다
        ERR_CASE( ERR_GACHA_02,         kPacket_.m_nOK, 2 );    // 가차 아이템인데 로터리에 등록 되지 않음
        ERR_CASE( ERR_GACHA_03,         kPacket_.m_nOK, 3 );    // 해당 아이템은 가차 아이템이 아님
        ERR_CASE( ERR_GACHA_04,         kPacket_.m_nOK, 4 );    // 해당 가차 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_GACHA_05,         kPacket_.m_nOK, 5 );    // 충분한 열쇠 아이템을 보유하고 있지 않음.
        ERR_CASE( ERR_DB_COMMON_00,     kPacket_.m_nOK, 6 );    //트랜잭션 에러 )
        ERR_CASE( ERR_DB_COMMON_01,     kPacket_.m_nOK, 7 );    //DB에 해당 유저가 존재하지 않음. )
        ERR_CASE( ERR_DB_COMMON_02,     kPacket_.m_nOK, 8 );    //DB에 존재하지 않는 아이템. )
        ERR_CASE( ERR_GACHA_12,         kPacket_.m_nOK, 9 );    // 가차 아이템 DB 동기화 실패.
        ERR_CASE( ERR_GACHA_13,         kPacket_.m_nOK, 10 );    // 가차 열쇠 아이템 DB 동기화 실패. 
        ERR_CASE( ERR_GACHA_07,         kPacket_.m_nOK, 11 );     // 가차 수집 아이템 리스트가 없습니다.
        ERR_CASE( ERR_GACHA_14,         kPacket_.m_nOK, 12 );    // 오픈되지 않은 가챠 Type입니다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );  // 처리 지연.

    default:
        START_LOG( cerr, L"가차 돌리는 중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << END_LOG;
        kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_GACHA_ACTION_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L" 가차 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOG( kPacket_.m_gachaItem.m_ItemID )
        << BUILD_LOG( bClear ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_GACHA_OBTAINED_SET_REWARD_REQ )
{
    SEND_DATA( EVENT_GACHA_OBTAINED_SET_REWARD_ACK, m_mapGachaSetObtained );
}

IMPL_ON_FUNC_NOPARAM( EVENT_GACHA_LEVEL_OBTAINED_SET_REWARD_REQ )
{
    SEND_DATA( EVENT_GACHA_LEVEL_OBTAINED_SET_REWARD_ACK, m_mapGachaSetObtainedLevel );
}

IMPL_ON_FUNC( EVENT_GACHA_SET_REWARD_REQ )
{
    VERIFY_STATE( ( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ) );
    std::map< int, std::set< GCITEMID > > mapCollectionList;
    std::map< int, std::set< GCITEMID > >::iterator mit;
    std::set< GCITEMID >::iterator sit;
    KEVENT_GACHA_SET_REWARD_ACK kPacket;
    KCharacterInfo* pkCharInfo = NULL;

    SET_ERROR( ERR_UNKNOWN );

    // 획득 가능한 레벨인가?
    if ( false == SiKGachaManager()->CheckGachaRewardLevel( kPacket_.m_nVersion, kPacket_.m_gachaItemID, kPacket_.m_nLevel ) ) {
        SET_ERR_GOTO( ERR_GACHA_15, END_PROC );
    }

    // 이미 획득 했던 가차 아이디 인가?
    if ( false == CheckGachaObtained( kPacket_.m_nVersion, kPacket_.m_gachaItemID, kPacket_.m_dwType ) ) {
        SET_ERR_GOTO( ERR_GACHA_06, END_PROC );
    }

    // 요청 타입이 최종 보상(펫)일 경우 장비 세트 보상을 먼저 받았는가?
    if ( KGachaManager::GRT_TOTAL_REWARD == kPacket_.m_dwType
        && true == CheckGachaObtained( kPacket_.m_nVersion, kPacket_.m_gachaItemID, KGachaManager::GRT_EQUIP_REWARD ) ) {
        SET_ERR_GOTO( ERR_GACHA_08, END_PROC );
    }

    // 해당 아이디의 목록을 가져 온다.
    SiKGachaManager()->GetRewardItemList( kPacket_.m_nVersion, kPacket_.m_gachaItemID, mapCollectionList );
    mit = mapCollectionList.find( kPacket_.m_dwType );
    if ( mapCollectionList.empty() || mit == mapCollectionList.end() ) {
        SET_ERR_GOTO( ERR_GACHA_07, END_PROC );
    }

    // 목록이 나의 인벤토리에 모두 있는가?
    for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {
        KItemPtr pkItem = m_kInventory.FindItemByItemID( *sit );
        if ( NULL == pkItem ) {
            SET_ERR_GOTO( ERR_GACHA_08, END_PROC );
        }
    }

    // 로그 기록을 위해 캐릭터 정보 추가
    kPacket_.m_cCharType = kPacket_.m_cCharType;
    kPacket_.m_dwCharLevel = 0; // 캐릭터 정보가 없을 경우 
    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if ( NULL != pkCharInfo ) {
        kPacket_.m_dwCharLevel = pkCharInfo->m_dwLevel;
    }

    // DB 큐잉..
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GACHA_SET_REWARD_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );
        ERR_CASE( ERR_GACHA_06,         kPacket.m_nOK, 1 ); // 이미 획득한 가차 세트 입니다. )
        ERR_CASE( ERR_GACHA_07,         kPacket.m_nOK, 2 ); // 가차 수집 아이템 리스트가 없습니다. )
        ERR_CASE( ERR_GACHA_08,         kPacket.m_nOK, 3 ); // 가차 수집 아이템이 부족 합니다. )
        ERR_CASE( ERR_GACHA_09,         kPacket.m_nOK, 4 ); // 가차 세트 보상 리스트 정보 가져오기 실패. )
        ERR_CASE( ERR_GACHA_10,         kPacket.m_nOK, 5 ); // 가차 세트 보상 완료 기록 실패. )
        ERR_CASE( ERR_GACHA_11,         kPacket.m_nOK, 6 ); // 가차 세트 보상 아이템 요청시 DB 오류. )
        ERR_CASE( ERR_GACHA_15,         kPacket.m_nOK, 7 ); //  획득 가능한 레벨이 아닙니다. )
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG( cerr, L"가차 세트 보상 요청시 알수 없는 요류. Name : " << m_strName  )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GACHA_SET_REWARD_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" 가차 보상 요청 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nVersion )
        << BUILD_LOG( kPacket_.m_gachaItemID )
        << BUILD_LOG( kPacket_.m_dwType ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_GACHA_SET_REWARD_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    std::wstring strLoudMsg;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        goto END_PROC;
    }

    InsertGachaObtained( kPacket_.m_nVersion, kPacket_.m_gachaItemID, kPacket_.m_dwType );
    m_kInventory.AddItemList( kPacket_.m_vecReward );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_GACHA_06,         kPacket_.m_nOK, 1 ); // 이미 획득한 가차 세트 입니다. )
        ERR_CASE( ERR_GACHA_07,         kPacket_.m_nOK, 2 ); // 가차 수집 아이템 리스트가 없습니다. )
        ERR_CASE( ERR_GACHA_08,         kPacket_.m_nOK, 3 ); // 가차 수집 아이템이 부족 합니다. )
        ERR_CASE( ERR_GACHA_09,         kPacket_.m_nOK, 4 ); // 가차 세트 보상 리스트 정보 가져오기 실패. )
        ERR_CASE( ERR_GACHA_10,         kPacket_.m_nOK, 5 ); // 가차 세트 보상 완료 기록 실패. )
        ERR_CASE( ERR_GACHA_11,         kPacket_.m_nOK, 6 ); // 가차 세트 보상 아이템 요청시 DB 오류. )
        ERR_CASE( ERR_GACHA_15,         kPacket_.m_nOK, 7 ); //  획득 가능한 레벨이 아닙니다. )
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG( cerr, L"가차 세트 보상 요청시 알수 없는 요류. Name : " << m_strName  )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_GACHA_SET_REWARD_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L" 가차 세트 보상 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_gachaItemID  )
        << BUILD_LOG( kPacket_.m_vecReward.size() ) << END_LOG;

    if ( kPacket_.m_nOK == 0 && SiKLoudMessage()->GetGachaMsg( (int)kPacket_.m_gachaItemID, m_strNickName, strLoudMsg, GetLanguageCode() ) ) {
        if ( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) ) {
            std::wstring strLanguageCode = boost::lexical_cast< std::wstring >( GetLanguageCode() );
            strLoudMsg = strLanguageCode + L":" + strLoudMsg;
        }

        KSignBoardData kData;
        kData.m_dwColor         = 0L;
        kData.m_ItemID          = 0L;
        kData.m_dwSenderUID     = 0L;
        kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN_EX_GAME;
        kData.m_strMsg.swap( strLoudMsg );
        kData.m_strSenderNick.clear();
        SiKSignBoard()->QueueingAdminData( kData );
    }
}

_IMPL_ON_FUNC( EVENT_GACHA_LEVEL_SET_REWARD_REQ, KEVENT_GACHA_SET_REWARD_REQ )
{
    VERIFY_STATE( ( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ) );
    std::map< int, std::set< GCITEMID > > mapCollectionList;
    std::map< int, std::set< GCITEMID > >::iterator mit;
    std::set< GCITEMID >::iterator sit;
    KEVENT_GACHA_SET_REWARD_ACK kPacket;
    KCharacterInfo* pkCharInfo = NULL;

    SET_ERROR( ERR_UNKNOWN );

    // 캐릭터 정보를 얻는다
    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if( pkCharInfo ) {
        kPacket_.m_dwCharLevel = pkCharInfo->m_dwLevel;
    } else {
        kPacket_.m_dwCharLevel = 0; // 캐릭터 정보가 없을 경우 
    }
    // 획득 가능한 레벨인가?
    if ( false == SiKGachaManager()->CheckGachaRewardLevel( kPacket_.m_nVersion, kPacket_.m_gachaItemID, kPacket_.m_nLevel ) ) {
        SET_ERR_GOTO( ERR_GACHA_15, END_PROC );
    }

    // 이미 획득 했던 가차 아이디 인가?
    if ( false == CheckGachaObtained_Level( kPacket_.m_nVersion, kPacket_.m_gachaItemID, kPacket_.m_dwType, kPacket_.m_nLevel ) ) {
        SET_ERR_GOTO( ERR_GACHA_06, END_PROC );
    }

    // 요청 타입이 최종 보상(펫)일 경우 장비 세트 보상을 먼저 받았는가?
    if ( KGachaManager::GRT_TOTAL_REWARD == kPacket_.m_dwType
        && true == CheckGachaObtained_Level( kPacket_.m_nVersion, kPacket_.m_gachaItemID, KGachaManager::GRT_EQUIP_REWARD, kPacket_.m_nLevel ) ) {
        SET_ERR_GOTO( ERR_GACHA_08, END_PROC );
    }

    // 해당 아이디의 목록을 가져 온다.
    SiKGachaManager()->GetRewardItemList( kPacket_.m_nVersion, kPacket_.m_gachaItemID, mapCollectionList );
    mit = mapCollectionList.find( kPacket_.m_dwType );
    if ( mapCollectionList.empty() || mit == mapCollectionList.end() ) {
        SET_ERR_GOTO( ERR_GACHA_07, END_PROC );
    }

    // 목록이 나의 인벤토리에 모두 있는가?
    for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {
        KItemPtr pkItem = m_kInventory.FindItemByItemIDandLevel( *sit, kPacket_.m_nLevel ); //  2011/10/19 woosh 레벨도 같아야 한다
        if ( NULL == pkItem ) {
            SET_ERR_GOTO( ERR_GACHA_08, END_PROC );
        }
    }

    // 로그 기록을 위해 캐릭터 정보 추가
    kPacket_.m_cCharType = kPacket_.m_cCharType;
    kPacket_.m_dwCharLevel = 0; // 캐릭터 정보가 없을 경우 
    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if ( NULL != pkCharInfo ) {
        kPacket_.m_dwCharLevel = pkCharInfo->m_dwLevel;
    }

    // DB 큐잉..
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GACHA_LEVEL_SET_REWARD_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );
        ERR_CASE( ERR_GACHA_06,         kPacket.m_nOK, 1 ); // 이미 획득한 가차 세트 입니다. )
        ERR_CASE( ERR_GACHA_07,         kPacket.m_nOK, 2 ); // 가차 수집 아이템 리스트가 없습니다. )
        ERR_CASE( ERR_GACHA_08,         kPacket.m_nOK, 3 ); // 가차 수집 아이템이 부족 합니다. )
        ERR_CASE( ERR_GACHA_09,         kPacket.m_nOK, 4 ); // 가차 세트 보상 리스트 정보 가져오기 실패. )
        ERR_CASE( ERR_GACHA_10,         kPacket.m_nOK, 5 ); // 가차 세트 보상 완료 기록 실패. )
        ERR_CASE( ERR_GACHA_11,         kPacket.m_nOK, 6 ); // 가차 세트 보상 아이템 요청시 DB 오류. )
        ERR_CASE( ERR_GACHA_15,         kPacket.m_nOK, 7 ); //  획득 가능한 레벨이 아닙니다. )
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG( cerr, L"가차 세트 보상 요청시 알수 없는 요류. Name : " << m_strName  )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if ( kPacket.m_nOK != 0 ) {
        SEND_PACKET( EVENT_GACHA_LEVEL_SET_REWARD_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" 가차 보상 요청 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOG( kPacket_.m_gachaItemID )
        << BUILD_LOG( kPacket_.m_dwType )
        << BUILD_LOG( kPacket_.m_nLevel ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_GACHA_LEVEL_SET_REWARD_ACK, KEVENT_GACHA_SET_REWARD_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    std::wstring strLoudMsg;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        goto END_PROC;
    }

    InsertGachaObtained_Level( kPacket_.m_nVersion, kPacket_.m_gachaItemID, kPacket_.m_dwType, kPacket_.m_nLevel );
    m_kInventory.AddItemList( kPacket_.m_vecReward );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_GACHA_06,         kPacket_.m_nOK, 1 ); // 이미 획득한 가차 세트 입니다. )
        ERR_CASE( ERR_GACHA_07,         kPacket_.m_nOK, 2 ); // 가차 수집 아이템 리스트가 없습니다. )
        ERR_CASE( ERR_GACHA_08,         kPacket_.m_nOK, 3 ); // 가차 수집 아이템이 부족 합니다. )
        ERR_CASE( ERR_GACHA_09,         kPacket_.m_nOK, 4 ); // 가차 세트 보상 리스트 정보 가져오기 실패. )
        ERR_CASE( ERR_GACHA_10,         kPacket_.m_nOK, 5 ); // 가차 세트 보상 완료 기록 실패. )
        ERR_CASE( ERR_GACHA_11,         kPacket_.m_nOK, 6 ); // 가차 세트 보상 아이템 요청시 DB 오류. )
        ERR_CASE( ERR_GACHA_15,         kPacket_.m_nOK, 7 ); //  획득 가능한 레벨이 아닙니다. )
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG( cerr, L"가차 세트 보상 요청시 알수 없는 요류. Name : " << m_strName  )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_GACHA_LEVEL_SET_REWARD_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L" 가차 세트 보상 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOG( kPacket_.m_gachaItemID  )
        << BUILD_LOG( kPacket_.m_vecReward.size() )
        << BUILD_LOG( kPacket_.m_nLevel ) << END_LOG;

    if ( kPacket_.m_nOK == 0 && SiKLoudMessage()->GetGachaMsg( (int)kPacket_.m_gachaItemID, m_strNickName, strLoudMsg, GetLanguageCode() ) ) {
        if ( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) ) {
            std::wstring strLanguageCode = boost::lexical_cast< std::wstring >( GetLanguageCode() );
            strLoudMsg = strLanguageCode + L":" + strLoudMsg;
        }

        KSignBoardData kData;
        kData.m_dwColor         = 0L;
        kData.m_ItemID          = 0L;
        kData.m_dwSenderUID     = 0L;
        kData.m_eType           = KSignBoardData::SBT_SERVER_ADMIN_EX_GAME;
        kData.m_strMsg.swap( strLoudMsg );
        kData.m_strSenderNick.clear();
        SiKSignBoard()->QueueingAdminData( kData );
    }
}

IMPL_ON_FUNC( EVENT_GACHA_SET_REWARD_LIST_REQ )
{
    std::vector< KDropItemInfo > vecItems;
    KGachaRewardList kPacket;

    kPacket.m_nVersion = kPacket_.m_nVersion;
    kPacket.m_dwGachaItemID = kPacket_.m_dwGachaItemID;

    SiKGachaManager()->GetGachaSetRewardList( kPacket_.m_nVersion, kPacket.m_dwGachaItemID, kPacket.m_mapSetRewardList );
    kPacket.m_bAccessoriesEnable = SiKGachaManager()->GetAccessoriesEnable( kPacket_.m_nVersion );

    SEND_PACKET( EVENT_GACHA_SET_REWARD_LIST_ACK );

    START_LOG( clog, L"가차 세트 보상 아이템 리스트 응답.. Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_nVersion )
        << BUILD_LOG( kPacket_.m_dwGachaItemID )
        << BUILD_LOG( kPacket.m_bAccessoriesEnable )
        << BUILD_LOG( kPacket.m_dwGachaItemID )
        << BUILD_LOG( kPacket.m_mapSetRewardList.size() )
        << END_LOG;
}

_IMPL_ON_FUNC( EVENT_UPDATE_GACHA_KEY_BUY_COUNT_ACK, PAIR_INT )
{
    m_nGachaKeyBuyCount = kPacket_.first;
    m_nGPKeyBuyCount    = kPacket_.second;

    SEND_DATA( EVENT_GACHA_KEY_BUY_COUNT_NOT, kPacket_ );

    START_LOG( clog, L"가차 키 구매 횟수 ..Name : " << m_strName )
        << BUILD_LOG( m_nGachaKeyBuyCount )
        << BUILD_LOG( m_nGPKeyBuyCount )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CLIENT_FAILED_GAME_START_NOT )
{
    switch( kPacket_.m_dwType )
    {
    case KFailRate::DIFFERENT_LOADING_DATA:
        SiKFailRate()->IncreaseCount( KFailRate::DIFFERENT_LOADING_DATA );
        break;
    case KFailRate::EXCEED_LOADING_TIME:
        SiKFailRate()->IncreaseCount( KFailRate::EXCEED_LOADING_TIME );
        break;
    case KFailRate::USER_QUIT_GAME:
        SiKFailRate()->IncreaseCount( KFailRate::USER_QUIT_GAME );
        break;
    }
    KStatLoadingInfoPtr spStat = GET_STAT( KStatLoadingInfo, SI_STAT_LOADINGINFO );
    if( spStat ) {
        spStat->AddStat( kPacket_.m_dwType, kPacket_.m_nSecond, kPacket_.m_dwRoomTotalUser, kPacket_.m_dwMapID, kPacket_.m_dwFailType, kPacket_.m_dwFailReason );
    }
}

_IMPL_ON_FUNC( EVENT_FULL_COUPLE_INFO_ACK, KCoupleInfo )
{
    KUserPtr spUser;
    if( kPacket_.m_dwPartnerUID == 0 )
    {
        SetCoupleState( KCoupleSystem::CS_NONE );
    }
    else
    {
        m_kCoupleInfo   = kPacket_;
        m_kInitEquip    = m_kCoupleInfo.m_vecMyEquip;
        SetCoupleState( KCoupleSystem::CS_COUPLE );

        ConnectionExpiredCoupleItem();
        spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( GetPartnerUID() );
        if( spUser )
        {
            if( !SiKCoupleSystem()->ChkEquipEqual( m_kCoupleInfo.m_vecPartnerEquip, spUser->m_kCoupleInfo.m_vecMyEquip ) )
            {
                m_kCoupleInfo.m_vecPartnerEquip = spUser->m_kCoupleInfo.m_vecMyEquip;
            }

            if( !SiKCoupleSystem()->ChkEquipEqual( m_kCoupleInfo.m_vecMyEquip, spUser->m_kCoupleInfo.m_vecPartnerEquip ) )
            {
                std::vector<DWORD> vecEquip;
                spUser->m_kCoupleInfo.m_vecPartnerEquip = m_kCoupleInfo.m_vecMyEquip;
                SiKCoupleSystem()->GetEquipList( spUser->m_kCoupleInfo.m_vecPartnerEquip, vecEquip );
                spUser->SendPacket( KUserEvent::EVENT_EQUIP_COUPLE_ITEM_NOT, vecEquip );
            }
            //캐릭터 타입도 동기화.
            m_kCoupleInfo.m_cPartnerCharType = spUser->m_kCoupleInfo.m_cCharType;

            if( spUser->m_kCoupleInfo.m_cPartnerCharType != m_kCoupleInfo.m_cCharType )
            {
                spUser->m_kCoupleInfo.m_cPartnerCharType = m_kCoupleInfo.m_cCharType;
                spUser->SendPacket( EVENT_TYPE::EVENT_CHANGE_COUPLE_CHAR_NOT, m_kCoupleInfo.m_cCharType );
            }

            // 인벤도 확인 하자..
            if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
            {
                DumpCoupleInfo();
                spUser->DumpCoupleInfo();
            }
        }
        DumpCoupleInven();
    }

    SEND_DATA( EVENT_FULL_COUPLE_INFO_NOT, m_kCoupleInfo );

    if( IsCouple() && spUser && spUser->m_nCoupleWindowState != 0 )
    {
        SEND_DATA( EVENT_COUPLE_WINDOW_STATE_NOT, spUser->m_nCoupleWindowState );
    }

}

IMPL_ON_FUNC( EVENT_MAKE_COUPLE_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    _VERIFY_STATE((4, KUserFSM::STATE_ROOM, KUserFSM::STATE_SQUARE,KUserFSM::STATE_AGIT_JOIN,KUserFSM::STATE_AGIT));

    KEVENT_MAKE_COUPLE_ACK kPacket;
    KItemPtr pkItem;
    KUserPtr spUser;
    kPacket_.m_dwRequester = GetUID();
    kPacket_.m_strRequesterNick = m_strNickName;

    // 자신에게 커플 신청 하는가?
    if( kPacket_.m_dwRequester == kPacket_.m_dwPartnerUID )
    {
        SET_ERR_GOTO( ERR_COUPLE_17, END_PROC );
    }

    // 내 상태는 커플 신청 가능 상태 인가?
    if( !ChkMakeCoupleState( this, GetStateID() ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_16, END_PROC );
    }

    // 아이템이 반지인가?
    if( !SiKCoupleSystem()->IsRingItem( kPacket_.m_kRingItem.m_dwID ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_15, END_PROC );
    }
    // 아이템이 존재 하는가?
    pkItem = m_kInventory.FindItemByItemUID( kPacket_.m_kRingItem.m_dwUID );
    if( pkItem == NULL )
    {
        SET_ERR_GOTO( ERR_COUPLE_14, END_PROC );
    }

    // 커플 신청 하려는 대상 유저는 존재하는가?
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwPartnerUID );
    if( !spUser )
    {
        SET_ERR_GOTO( ERR_COUPLE_05, END_PROC );
    }

    if( spUser->IsInDenyList( GetUID() ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_25, END_PROC );
    }
    // 대상 유저는 커플 신청 해도 되는 상태 인가?
    if( !ChkMakeCoupleState( spUser.get(), GetStateID() ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_07, END_PROC );
    }

    spUser->SetCoupleState( KCoupleSystem::CS_REQUEST_FROM );
    // 대상 유저에게 패킷을 보내자.
    if( !spUser->SendPacket( EVENT_TYPE::EVENT_MAKE_COUPLE_ASK, kPacket_ ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_18, END_PROC );
    }

    // 중요 데이터를 시스템에 저장하자.
    if( !SiKCoupleSystem()->AddCoupleRequestInfo( GetUID(), spUser->GetUID(), kPacket_.m_kRingItem ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_16, END_PROC );

    }
    SetCoupleState( KCoupleSystem::CS_REQUEST );

    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_16,    kPacket.m_nOK, 1 ); // 커플 요청을 할수 있는 상태가 아님.
        ERR_CASE( ERR_COUPLE_15,    kPacket.m_nOK, 2 ); // 해당 아이템은 반지 아이템이 아님
        ERR_CASE( ERR_COUPLE_14,    kPacket.m_nOK, 3 ); // 해당 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_COUPLE_05,    kPacket.m_nOK, 4 ); // 상대 유저가 동일 서버에 존재하지 않음
        ERR_CASE( ERR_COUPLE_07,    kPacket.m_nOK, 5 ); // 커플 요청 받을수 있는 상태가 아님.
        ERR_CASE( ERR_COUPLE_17,    kPacket.m_nOK, 6 ); // 자신에게 커플 신청 할수 없음
        ERR_CASE( ERR_COUPLE_18,    kPacket.m_nOK, 7 ); // 대상 유저에게 신청 패킷을 보내지 못했음.
        ERR_CASE( ERR_COUPLE_23,    kPacket.m_nOK, 8 ); // 커플 신청 정보가 없음.
        ERR_CASE( ERR_COUPLE_24,    kPacket.m_nOK, 9 ); // 커플 신청 거부 하였음.
        ERR_CASE( ERR_COUPLE_25,    kPacket.m_nOK, 10 ); // 커플 거절 상태 입니다.

    default:
        START_LOG( cerr, L"커플 맺기 신청중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_dwPartnerUID )
            << BUILD_LOG( kPacket_.m_strMsg ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
    {
        SEND_PACKET( EVENT_MAKE_COUPLE_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"커플 맺기 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwPartnerUID )
        << BUILD_LOG( kPacket_.m_kRingItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kRingItem.m_dwUID )
        << BUILD_LOG( kPacket_.m_strMsg ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_MAKE_COUPLE_ANS, bool )
{
    SRequestInfo    coupleReqInfo;
    KUserPtr        spUser;
    KEVENT_MAKE_COUPLE_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );
    // 나에게 커플 신청한 기록이 존재 하는가?
    if( !SiKCoupleSystem()->GetCoupleRequestInfo( GetUID(), coupleReqInfo ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_23, END_PROC );
    }

    // 서버에 나에게 커플 신청한 사람이 존재 하는가?
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( coupleReqInfo.m_dwRequester );
    if( !spUser )
    {
        SiKCoupleSystem()->DeleteCoupleRequestInfo( coupleReqInfo.m_dwRequester, coupleReqInfo.m_dwRequestee );
        m_kCoupleInfo.m_dwCoupleState = KCoupleSystem::CS_NONE;
        SET_ERR_GOTO( ERR_COUPLE_05, END_PROC );
    }

    spUser->QueueingEvent( KUserEvent::EVENT_MAKE_COUPLE_ANS_FROM, kPacket_ );
    SiKCoupleSystem()->UpdateCoupleReqInfoState( GetUID(), KCoupleSystem::MCS_ANS_FROM );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_16,    kPacket.m_nOK, 1 ); // 커플 요청을 할수 있는 상태가 아님.
        ERR_CASE( ERR_COUPLE_15,    kPacket.m_nOK, 2 ); // 해당 아이템은 반지 아이템이 아님
        ERR_CASE( ERR_COUPLE_14,    kPacket.m_nOK, 3 ); // 해당 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_COUPLE_05,    kPacket.m_nOK, 4 ); // 상대 유저가 동일 서버에 존재하지 않음
        ERR_CASE( ERR_COUPLE_07,    kPacket.m_nOK, 5 ); // 커플 요청 받을수 있는 상태가 아님.
        ERR_CASE( ERR_COUPLE_17,    kPacket.m_nOK, 6 ); // 자신에게 커플 신청 할수 없음
        ERR_CASE( ERR_COUPLE_18,    kPacket.m_nOK, 7 ); // 대상 유저에게 신청 패킷을 보내지 못했음.
        ERR_CASE( ERR_COUPLE_23,    kPacket.m_nOK, 8 ); // 커플 신청 정보가 없음.
        ERR_CASE( ERR_COUPLE_24,    kPacket.m_nOK, 9 ); // 커플 신청 거부 하였음.
        ERR_CASE( ERR_COUPLE_25,    kPacket.m_nOK, 10 ); // 커플 거절 상태 입니다.

    default:
        START_LOG( cerr, L"커플 요청 응답중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg ) << END_LOG;
        kPacket.m_nOK = 99;

    };

    if( kPacket_ && kPacket.m_nOK != 0 ) // 수락한 상태에서 오류가 발생 하면..
    {
        SEND_PACKET( EVENT_MAKE_COUPLE_ACK );
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"커플 신청에 대한 응답. : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_MAKE_COUPLE_ANS_FROM, bool )
{
    SRequestInfo coupleReqInfo;
    coupleReqInfo.m_dwRequestee = 0;
    coupleReqInfo.m_dwRequester = 0;
    KEVENT_MAKE_COUPLE_ACK kPacket;
    KMakeCoupleDBData_Req   kDBPacket;

    KUserPtr spUser;
    SET_ERROR( ERR_UNKNOWN );

    // 신청 정보 확인
    if( !SiKCoupleSystem()->GetCoupleRequestInfo( GetUID(), coupleReqInfo ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_23, END_PROC );
    }

    // 대상 유저 존재 확인
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( coupleReqInfo.m_dwRequestee );
    if( !spUser )
    {
        SET_ERR_GOTO( ERR_COUPLE_05, END_PROC );
    }

    // 승락 여부 확인
    if( !kPacket_ )
    {
        SET_ERR_GOTO( ERR_COUPLE_24, END_PROC );
    }

    // 아이템이 반지인가?
    if( !SiKCoupleSystem()->IsRingItem( coupleReqInfo.m_dwItemID ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_15, END_PROC );
    }

    if( !SiKCoupleSystem()->GetCoupleItem( coupleReqInfo.m_dwItemID, kDBPacket.m_kCoupleEquip ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_21, END_PROC );
    }

    // 아이템이 존재 하는가?
    if( !m_kInventory.FindItemByItemUID( coupleReqInfo.m_dwItemUID ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_15, END_PROC );
    }

    // DB 큐잉
    SET_ERROR( NET_OK );
    kDBPacket.m_dwRequester                 = GetUID();
    kDBPacket.m_dwPartnerUID                = coupleReqInfo.m_dwRequestee;
    kDBPacket.m_kCoupleEquip.m_dwItemUID    = coupleReqInfo.m_dwItemUID;
    kDBPacket.m_kRingItem.m_dwID            = coupleReqInfo.m_dwItemID;
    kDBPacket.m_kRingItem.m_dwUID           = coupleReqInfo.m_dwItemUID;

    QUEUEING_EVENT_TO_DB( EVENT_MAKE_COUPLE_REQ, kDBPacket );
    SiKCoupleSystem()->UpdateCoupleReqInfoState( GetUID(), KCoupleSystem::MCS_DB_WORK );

    START_LOG( clog, L"커플 맺기 DB 요청.. Name : " << GetName() )
        << BUILD_LOG( kDBPacket.m_dwRequester )
        << BUILD_LOG( kDBPacket.m_dwPartnerUID )
        << BUILD_LOG( kDBPacket.m_kRingItem.m_dwID )
        << BUILD_LOG( kDBPacket.m_kRingItem.m_dwUID ) << END_LOG;

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_16,    kPacket.m_nOK, 1 ); // 커플 요청을 할수 있는 상태가 아님.
        ERR_CASE( ERR_COUPLE_15,    kPacket.m_nOK, 2 ); // 해당 아이템은 반지 아이템이 아님
        ERR_CASE( ERR_COUPLE_14,    kPacket.m_nOK, 3 ); // 해당 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_COUPLE_05,    kPacket.m_nOK, 4 ); // 상대 유저가 동일 서버에 존재하지 않음
        ERR_CASE( ERR_COUPLE_07,    kPacket.m_nOK, 5 ); // 커플 요청 받을수 있는 상태가 아님.
        ERR_CASE( ERR_COUPLE_17,    kPacket.m_nOK, 6 ); // 자신에게 커플 신청 할수 없음
        ERR_CASE( ERR_COUPLE_18,    kPacket.m_nOK, 7 ); // 대상 유저에게 신청 패킷을 보내지 못했음.
        ERR_CASE( ERR_COUPLE_23,    kPacket.m_nOK, 8 ); // 커플 신청 정보가 없음.
        ERR_CASE( ERR_COUPLE_24,    kPacket.m_nOK, 9 ); // 커플 신청 거부 하였음.
        ERR_CASE( ERR_COUPLE_25,    kPacket.m_nOK, 10 ); // 커플 거절 상태 입니다.


    default:
        START_LOG( cerr, L"커플 맺기 신청중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
    {

        if( kPacket.m_nOK != 8 )
            SiKCoupleSystem()->DeleteCoupleRequestInfo( coupleReqInfo.m_dwRequester, coupleReqInfo.m_dwRequestee );

        m_kCoupleInfo.m_dwCoupleState = KCoupleSystem::CS_NONE;

        if( spUser )
            spUser->m_kCoupleInfo.m_dwCoupleState = KCoupleSystem::CS_NONE;

        SEND_PACKET( EVENT_MAKE_COUPLE_ACK );
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"커플 요청에 대한 응답 처리 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

}

IMPL_ON_FUNC( EVENT_MAKE_COUPLE_ACK )
{
    KEVENT_MAKE_COUPLE_ACK kPartnerData;
    KUserPtr spUser;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        goto END_PROC;
    }
    // 아이템 제거
    LIF( m_kInventory.RemoveItem( kPacket_.m_kRingItem.m_dwID, kPacket_.m_kRingItem.m_dwUID ) );
    kPacket_.m_kCoupleInfo.m_dwCoupleState = KCoupleSystem::CS_COUPLE;
    AdjustCoupleData( kPacket_.m_kCoupleInfo );
    kPacket_.m_kCoupleInfo = m_kCoupleInfo;

    // 데이터 셋팅
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwRequestee );
    if( spUser )
    {
        KCoupleInfo kInfo = kPacket_.m_kCoupleInfo;
        // 대상 유저에게  데이터 셋팅한다.
        SiKCoupleSystem()->SwapCoupleInfo( kInfo );
        kInfo.m_strPartnerNick = m_strNickName;
        kInfo.m_strPartnerNickColor = m_strNickColor;
        spUser->AdjustCoupleData( kInfo );
        kPartnerData.m_kCoupleInfo = spUser->m_kCoupleInfo;
    }


END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_16,    kPacket_.m_nOK, 1 ); // 커플 요청을 할수 있는 상태가 아님.
        ERR_CASE( ERR_COUPLE_15,    kPacket_.m_nOK, 2 ); // 해당 아이템은 반지 아이템이 아님
        ERR_CASE( ERR_COUPLE_14,    kPacket_.m_nOK, 3 ); // 해당 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_COUPLE_05,    kPacket_.m_nOK, 4 ); // 상대 유저가 동일 서버에 존재하지 않음
        ERR_CASE( ERR_COUPLE_07,    kPacket_.m_nOK, 5 ); // 커플 요청 받을수 있는 상태가 아님.
        ERR_CASE( ERR_COUPLE_17,    kPacket_.m_nOK, 6 ); // 자신에게 커플 신청 할수 없음
        ERR_CASE( ERR_COUPLE_18,    kPacket_.m_nOK, 7 ); // 대상 유저에게 신청 패킷을 보내지 못했음.
        ERR_CASE( ERR_COUPLE_23,    kPacket_.m_nOK, 8 ); // 커플 신청 정보가 없음.
        ERR_CASE( ERR_COUPLE_24,    kPacket_.m_nOK, 9 ); // 커플 신청 거부 하였음.
        ERR_CASE( ERR_COUPLE_25,    kPacket_.m_nOK, 10 ); // 커플 거절 상태 입니다.
        ERR_CASE( ERR_COUPLE_00,    kPacket_.m_nOK, 11 ); // DB에 나의 유저 정보가 존재하지 않음.
        ERR_CASE( ERR_COUPLE_01,    kPacket_.m_nOK, 12 ); // DB에 상대방의 유저 정보가 존재하지 않음.
        ERR_CASE( ERR_COUPLE_02,    kPacket_.m_nOK, 13 ); // DB에 내가 이미 커플 상태임 
        ERR_CASE( ERR_COUPLE_03,    kPacket_.m_nOK, 14 ); // DB에 상대방이 이미 커플 상태임
        ERR_CASE( ERR_DB_COMMON_00, kPacket_.m_nOK, 15 ); // DB에러

    default:
        START_LOG( cerr, L"커플 맺기 완료중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }
    kPartnerData.m_nOK = kPacket_.m_nOK;
    // 요청 목록에서 지운다.
    SiKCoupleSystem()->DeleteCoupleRequestInfo( kPacket_.m_dwRequester, kPacket_.m_dwRequestee );

    // 두사람에게 모두 보낸다.
    SiKGSSimLayer()->m_kActorManager.SendTo( kPacket_.m_dwRequestee, KUserEvent::EVENT_MAKE_COUPLE_ACK, kPartnerData );
    SEND_RECEIVED_PACKET( EVENT_MAKE_COUPLE_ACK );

    SCoupleSignBoard sBoardInfo;
    if( kPacket_.m_nOK == 0 &&
        SiKCoupleSystem()->GetCoupleSignBoard( kPacket_.m_kRingItem.m_dwID, sBoardInfo ) )
    {
        SendCoupleNotToSquareUser( sBoardInfo );
    }

    START_LOG( clog, L"커플 맺기 Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwRequester )
        << BUILD_LOG( kPacket_.m_dwRequestee ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_USE_COUPLE_ITEM_REQ, KCoupleItemData )
{
    SET_ERROR( ERR_UNKNOWN );
    kPacket_.m_kItem.m_cItemType = 0;
    kPacket_.m_dwPartnerUID = GetPartnerUID();
    KCoupleItemData kPacket = kPacket_;
    KCoupleEquip kCoupleItem;
    // 내가 이미 커플 상태 인가?
    if( m_kCoupleInfo.m_dwCoupleState != KCoupleSystem::CS_COUPLE )
    {
        SET_ERR_GOTO( ERR_COUPLE_19, END_PROC );
    }

    if( SiKCoupleSystem()->IsRingItem( kPacket_.m_kItem.m_dwID ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_27, END_PROC );
    }

    // 이미 커플 인벤에 있는가?
    if( IsInCoupleInven( kPacket_.m_kItem.m_dwID ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_20, END_PROC );
    }

    // 이녀석이 커플 아이템인가?
    if( !SiKCoupleSystem()->GetCoupleItem( kPacket_.m_kItem.m_dwID, kCoupleItem ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_21, END_PROC );
    }

    // Item 보유 하고 있는가?
    if( m_kInventory.FindItemByItemUID( kPacket_.m_kItem.m_dwUID ) == NULL )
    {
        SET_ERR_GOTO( ERR_COUPLE_14, END_PROC );
    }

    SET_ERROR( NET_OK );
    QUEUEING_EVENT_TO_DB( EVENT_USE_COUPLE_ITEM_REQ, kPacket_ );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_19,    kPacket.m_nOK, 1 );
        ERR_CASE( ERR_COUPLE_20,    kPacket.m_nOK, 2 );
        ERR_CASE( ERR_COUPLE_21,    kPacket.m_nOK, 3 );
        ERR_CASE( ERR_COUPLE_14,    kPacket.m_nOK, 4 );
        ERR_CASE( ERR_COUPLE_00,    kPacket.m_nOK, 5 );
        ERR_CASE( ERR_COUPLE_01,    kPacket.m_nOK, 6 );
        ERR_CASE( ERR_COUPLE_06,    kPacket.m_nOK, 7 );
        ERR_CASE( ERR_COUPLE_27,    kPacket.m_nOK, 8 );

    default:
        START_LOG( cerr, L"커플 복장 사용시 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_bEquip )
            << BUILD_LOG( kPacket_.m_kItem.m_dwID )
            << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_USE_COUPLE_ITEM_ACK );
    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_bEquip )
        << BUILD_LOG( kPacket_.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;

}

_IMPL_ON_FUNC( EVENT_USE_COUPLE_ITEM_ACK, KCoupleItemData )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( kPacket_.m_nOK != 0 )
        goto END_PROC;

    // 인벤토리에 해당 아이템을 추가 한다.
    if( !IsInCoupleInven( kPacket_.m_kItem.m_dwID ) )
    {
        AddMyInven( kPacket_.m_kItem );
    }

    // 즉시 착용 이면 착용을 한다.
    if( kPacket_.m_bEquip )
        EquipCoupleItem( kPacket_.m_kItem.m_dwID );

    // 사용한 아이템을 제거 한다.
    //EraseNormalItem( kPacket_.m_kItem.m_dwUID, kPacket_.m_kItem.m_dwID );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_19,    kPacket_.m_nOK, 1 ); // 현재 커플 상태가 아님
        ERR_CASE( ERR_COUPLE_20,    kPacket_.m_nOK, 2 ); // 해당 아이템은 이미 옷장에 있음.
        ERR_CASE( ERR_COUPLE_21,    kPacket_.m_nOK, 3 ); // 해당 아이템은 커플용 아이템이 아님.
        ERR_CASE( ERR_COUPLE_14,    kPacket_.m_nOK, 4 ); // 해당 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_COUPLE_00,    kPacket_.m_nOK, 5 ); // 나의 유저 정보가 존재하지 않음.
        ERR_CASE( ERR_COUPLE_01,    kPacket_.m_nOK, 6 ); // 상대방의 유저 정보가 존재하지 않음.
        ERR_CASE( ERR_COUPLE_06,    kPacket_.m_nOK, 7 ); // 동일한 아이템이 창고에 존재
        ERR_CASE( ERR_COUPLE_27,    kPacket_.m_nOK, 8 ); // 반지 아이템을 우째 하려고 했음.
    default:
        START_LOG( cerr, L"커플 복장 사용시 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_bEquip )
            << BUILD_LOG( kPacket_.m_kItem.m_dwID )
            << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    if( kPacket_.m_nOK == 0 )
    {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( GetPartnerUID(), KUserEvent::EVENT_USE_COUPLE_ITEM_NOT, kPacket_ );
    }

    SEND_RECEIVED_PACKET( EVENT_USE_COUPLE_ITEM_ACK );

    LOG_SUCCESS( kPacket_.m_nOK == 0)
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_bEquip )
        << BUILD_LOG( kPacket_.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_USE_COUPLE_ITEM_NOT, KCoupleItemData )
{
    // 파트너가 뭔가 뻘짓을 했다.. 알려 주마.. ^^;;
    // 인벤토리에 해당 아이템을 추가 한다.
    if( !IsInCoupleInven( kPacket_.m_kItem.m_dwID ) )
    {
        AddPartnerInven( kPacket_.m_kItem );
    }

    // 즉시 착용 이면 착용을 한다.
    if( kPacket_.m_bEquip )
        EquipCoupleItem( kPacket_.m_kItem.m_dwID, true );

    SEND_RECEIVED_PACKET( EVENT_USE_COUPLE_ITEM_NOT );
}

_IMPL_ON_FUNC( EVENT_EQUIP_COUPLE_ITEM_REQ, std::vector<GCITEMID> )
{
    SET_ERROR( ERR_UNKNOWN );

    KEVENT_EQUIP_COUPLE_ITEM_ACK kPacket;
    kPacket.m_nOK = 99;

    // 내가 이미 커플 상태 인가?
    if( m_kCoupleInfo.m_dwCoupleState != KCoupleSystem::CS_COUPLE )
    {
        SET_ERR_GOTO( ERR_COUPLE_19, END_PROC );
    }

    // 커플 인벤에 있는가?
    if( !IsInCoupleInven( kPacket_ ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_22, END_PROC );
    }

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    {
        DumpCoupleEquip( m_kCoupleInfo.m_vecMyEquip, m_strName );
    }

    DumpCoupleInven();

    // 장착 하기
    if( !EquipCoupleItem( kPacket_ ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_21, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_19,    kPacket.m_nOK, 1 ); // 현재 커플 상태가 아님.
        ERR_CASE( ERR_COUPLE_22,    kPacket.m_nOK, 2 ); // 커플 인벤에 없는 아이템입니다..
        ERR_CASE( ERR_COUPLE_21,    kPacket.m_nOK, 3 ); // 해당 아이템은 커플용 아이템이 아님.

    default:
        START_LOG( cerr, L"커플 복장 장착시 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.size() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK == 0 )
    {
        SiKGSSimLayer()->m_kActorManager.QueueingEventTo( GetPartnerUID(), KUserEvent::EVENT_EQUIP_COUPLE_ITEM_NOT, kPacket_ );
    }

    kPacket.m_vecEquip = m_kCoupleInfo.m_vecMyEquip;
    SEND_PACKET( EVENT_EQUIP_COUPLE_ITEM_ACK );

    std::wstringstream stm;
    std::copy( kPacket_.begin(), kPacket_.end(), 
        std::ostream_iterator<std::vector<DWORD>::value_type,wchar_t>( stm, L", " ) );

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecEquip.size() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( stm.str() ) << END_LOG;

    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    {
        DumpCoupleEquip( m_kCoupleInfo.m_vecMyEquip, m_strName );
    }
}

_IMPL_ON_FUNC( EVENT_EQUIP_COUPLE_ITEM_NOT, std::vector<GCITEMID> )
{
    EquipCoupleItem( kPacket_, true );
    SEND_RECEIVED_PACKET( EVENT_EQUIP_COUPLE_ITEM_NOT );
}

_IMPL_ON_FUNC( EVENT_COUPLE_INFORMATION_REQ, DWORD )
{
    KEVENT_COUPLE_INFORMATION_ACK kPacket;
    kPacket.m_nOK = 99;
    do{
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_ );
        if( !spUser )
        {
            kPacket.m_nOK = 1;
            break;
        }
        // 커플 정보가 있는가?
        if( spUser->m_kCoupleInfo.m_dwCoupleState != KCoupleSystem::CS_COUPLE )
        {
            kPacket.m_nOK = 2;
            break;
        }

        kPacket.m_nOK = 0;
        // 커플 정보를 만들어 보세.
        spUser->MakeCoupleInfomation( kPacket ); // 커플이 없으면 안에서 바로 리턴 하기 때문에.

        {
            KStatCoupleRoomPtr spStat = GET_STAT( KStatCoupleRoom, SI_STAT_COUPLE_ROOM );
            if( spStat && kPacket_ != 0 )
            {
                spStat->AddStat( false );
            }
        }
    }while(false);

    SEND_PACKET( EVENT_COUPLE_INFORMATION_ACK );

    START_LOG( clog, L"커플 정보 요청.. Name : " << m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwRequesterUID )
        << BUILD_LOG( kPacket.m_dwRequesteeUID )
        << BUILD_LOG( kPacket.m_strRequester )
        << BUILD_LOG( kPacket.m_strRequestee )
        << BUILD_LOGc( kPacket.m_cRequesterChar )
        << BUILD_LOGc( kPacket.m_cRequesteeChar )
        << BUILD_LOG( kPacket.m_tmRegDate )
        << BUILD_LOG( kPacket.m_vecRequesterEquip.size() )
        << BUILD_LOG( kPacket.m_vecRequesteeEquip.size() ) << END_LOG;

}

_IMPL_ON_FUNC( EVENT_SET_COUPLE_REQ_DENY, DWORD )
{
    SetCoupleDeny( kPacket_ );
}

IMPL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_REQ )
{
    bool bIsInCoupleInven = false;
    KUserPtr spUser;
    KEVENT_CHANGE_COUPLE_RING_ACK kPacket;
    SRequestInfo sReqInfo;

    SET_ERROR( ERR_UNKNOWN );
    // 현재 커플 상태 인가?
    if( !IsCouple() )
    {
        SET_ERR_GOTO( ERR_COUPLE_19, END_PROC );
    }

    // 해당 아이템이 반지 아이템인가?
    if( !SiKCoupleSystem()->IsRingItem( kPacket_.m_kItem.m_dwID ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_15, END_PROC );
    }

    // 교체 정보가 있는가?
    if( SiKCoupleSystem()->IsInRingReqInfo( GetUID() ) ||
        SiKCoupleSystem()->IsInRingReqInfo( GetPartnerUID() ))
    {
        SET_ERR_GOTO( ERR_COUPLE_23, END_PROC );
    }

    bIsInCoupleInven = IsInCoupleInven( kPacket_.m_kItem.m_dwID );
    if( kPacket_.m_bInven )
    {
        if( bIsInCoupleInven )
            SET_ERR_GOTO( ERR_COUPLE_20, END_PROC );
        // 해당 아이템이 인벤에 있는가?
        if( !m_kInventory.FindItem( kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID ) )
            SET_ERR_GOTO( ERR_COUPLE_14, END_PROC );
    }
    else
    {
        // 옷장에 있는가?
        if( !bIsInCoupleInven )
            SET_ERR_GOTO( ERR_COUPLE_22, END_PROC );
    }

    // 대상 유저가 접속 중인가?
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( GetPartnerUID() );
    if( !spUser )
    {
        SET_ERR_GOTO( ERR_COUPLE_05, END_PROC );
    }

    // Save Data
    sReqInfo.m_dwItemID         = kPacket_.m_kItem.m_dwID;
    sReqInfo.m_dwItemUID        = kPacket_.m_kItem.m_dwUID;
    sReqInfo.m_dwRequester      = GetUID();
    sReqInfo.m_dwRequestee      = GetPartnerUID();
    sReqInfo.m_bInCoupleInven   = !kPacket_.m_bInven;
    if( !SiKCoupleSystem()->AddRingRequestInfo( sReqInfo ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_23, END_PROC );
    }

    // Send To Partner;
    if( !spUser->SendPacket( EVENT_TYPE::EVENT_CHANGE_COUPLE_RING_ASK, kPacket_ ) )
    {
        SiKCoupleSystem()->DeleteRingRequestInfo( GetUID(), GetPartnerUID() );
        SET_ERR_GOTO( ERR_COUPLE_18, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_19, kPacket.m_nOK, 1 );
        ERR_CASE( ERR_COUPLE_15, kPacket.m_nOK, 2 );
        ERR_CASE( ERR_COUPLE_20, kPacket.m_nOK, 3 );
        ERR_CASE( ERR_COUPLE_14, kPacket.m_nOK, 4 );
        ERR_CASE( ERR_COUPLE_22, kPacket.m_nOK, 5 );
        ERR_CASE( ERR_COUPLE_05, kPacket.m_nOK, 6 );
        ERR_CASE( ERR_COUPLE_23, kPacket.m_nOK, 7 );
        ERR_CASE( ERR_COUPLE_24, kPacket.m_nOK, 8 );

    default:
        kPacket.m_nOK = 99;
    }

    // SEND to me...
    if( kPacket.m_nOK != 0 )
    {
        SEND_PACKET( EVENT_CHANGE_COUPLE_RING_ACK );
    }

    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_GLOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_bInven )
        << BUILD_LOG( kPacket_.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kItem.m_dwUID )
        << BUILD_LOG( kPacket_.m_strMsg ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_ANS, bool )
{
    KUserPtr spUser;
    KEVENT_CHANGE_COUPLE_RING_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );
    // 현재 커플 상태 인가?
    if( !IsCouple() )
    {
        SET_ERR_GOTO( ERR_COUPLE_19, END_PROC );
    }

    // 교체 정보가 있는가?
    if( !SiKCoupleSystem()->IsInRingReqInfo( GetUID() ) ||
        !SiKCoupleSystem()->IsInRingReqInfo( GetPartnerUID() ))
    {
        SET_ERR_GOTO( ERR_COUPLE_23, END_PROC );
    }

    // 대상 유저가 접속 중인가?
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( GetPartnerUID() );
    if( !spUser )
    {
        SET_ERR_GOTO( ERR_COUPLE_05, END_PROC );
    }

    // 응답..(누군가에게)
    spUser->QueueingEvent( KUserEvent::EVENT_CHANGE_COUPLE_RING_ANS_FROM, kPacket_ );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_19, kPacket.m_nOK, 1 ); // 현재 커플 상태가 아님.
        ERR_CASE( ERR_COUPLE_15, kPacket.m_nOK, 2 ); // 해당 아이템은 반지 아이템이 아님.
        ERR_CASE( ERR_COUPLE_20, kPacket.m_nOK, 3 ); // 해당 아이템은 이미 옷장에 있음.
        ERR_CASE( ERR_COUPLE_14, kPacket.m_nOK, 4 ); // 해당 아이템을 보유하고 있지 않습니다.
        ERR_CASE( ERR_COUPLE_22, kPacket.m_nOK, 5 ); // 커플 인벤에 없는 아이템입니다.
        ERR_CASE( ERR_COUPLE_05, kPacket.m_nOK, 6 ); // 상대 유저가 동일 서버에 존재하지 않음
        ERR_CASE( ERR_COUPLE_23, kPacket.m_nOK, 7 ); // 신청 정보가 없음.
        ERR_CASE( ERR_COUPLE_24, kPacket.m_nOK, 8 ); // 대상이 거절 하였음.

    default:
        kPacket.m_nOK = 99;
    }

    // 실패일때 응답.
    if( kPacket.m_nOK != 0 )
    {
        SEND_PACKET( EVENT_CHANGE_COUPLE_RING_ACK );
        SiKCoupleSystem()->DeleteRingRequestInfo( GetUID(), GetPartnerUID() );
    }

    START_LOG( clog, L"커플링 교체 유저의 응답.. Name : " << m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_ANS_FROM, bool )
{
    KEVENT_CHANGE_COUPLE_RING_ACK kPacket;
    KEVENT_CHANGE_COUPLE_RING_REQ kDBPacket;
    SRequestInfo kReqData;
    // 현재 커플 상태 인가?
    SET_ERROR( ERR_UNKNOWN );
    // 현재 커플 상태 인가?
    if( !IsCouple() )
    {
        SET_ERR_GOTO( ERR_COUPLE_19, END_PROC );
    }
    // OK 인가?
    if( !kPacket_ )
    {
        SET_ERR_GOTO( ERR_COUPLE_24, END_PROC );
    }

    if( !SiKCoupleSystem()->GetRingRequestInfo( GetUID(), kReqData ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_23, END_PROC );
    }

    if( kReqData.m_bInCoupleInven )
    {
        // 해당 아이템이 옷장에 있다면.
        if( !IsInCoupleInven( kReqData.m_dwItemID ) )
        {
            SET_ERR_GOTO( ERR_COUPLE_22, END_PROC );
        }
        // 처리 해서 본인에게 Quueuing
        kDBPacket.m_bInven = false;
        kDBPacket.m_kItem.m_dwID  = kReqData.m_dwItemID;
        kDBPacket.m_kItem.m_dwUID = kReqData.m_dwItemUID;
        kPacket.m_nOK = NetError::NET_OK;
    }
    else
    {
        // 해당 아이템이 인벤에 있는가?
        KItemPtr pkItem = m_kInventory.FindItem( kReqData.m_dwItemID, kReqData.m_dwItemUID );
        if( !pkItem ) {
            SET_ERR_GOTO( ERR_COUPLE_14, END_PROC );
        }

        // 처리 해서 DB로
        kDBPacket.m_bInven = true;
        kDBPacket.m_kItem.m_dwID    = kReqData.m_dwItemID;
        kDBPacket.m_kItem.m_dwUID   = kReqData.m_dwItemUID;
        kPacket.m_nOK = NetError::NET_OK;
    }

    QUEUEING_EVENT_TO_DB( EVENT_CHANGE_COUPLE_RING_REQ, kDBPacket );
    SiKCoupleSystem()->UpdateRingReqInfo( GetUID(), KCoupleSystem::MCS_DB_WORK );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_19, kPacket.m_nOK, 1 );
        ERR_CASE( ERR_COUPLE_15, kPacket.m_nOK, 2 );
        ERR_CASE( ERR_COUPLE_20, kPacket.m_nOK, 3 );
        ERR_CASE( ERR_COUPLE_14, kPacket.m_nOK, 4 );
        ERR_CASE( ERR_COUPLE_22, kPacket.m_nOK, 5 );
        ERR_CASE( ERR_COUPLE_05, kPacket.m_nOK, 6 );
        ERR_CASE( ERR_COUPLE_23, kPacket.m_nOK, 7 );
        ERR_CASE( ERR_COUPLE_24, kPacket.m_nOK, 8 );
    default:
        START_LOG( cerr, L"링 교체 응답중 알수 없는 오류.. Name : " << m_strName ) << END_LOG;
        kPacket.m_nOK = 99;        
    }

    // 대상이 수락 했는데 실패 했으면.... ^^;;
    if( kPacket.m_nOK != 0  )
    {
        SEND_PACKET( EVENT_CHANGE_COUPLE_RING_ACK );
        SiKGSSimLayer()->m_kActorManager.SendTo( GetPartnerUID(), KUserEvent::EVENT_CHANGE_COUPLE_RING_ACK, kPacket );
        SiKCoupleSystem()->DeleteRingRequestInfo( GetUID(), GetPartnerUID() );
    }

    START_LOG( clog, L" Name : " << m_strName )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kReqData.m_dwItemID )
        << BUILD_LOG( kReqData.m_dwItemUID )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_ACK )
{
    KUserPtr spUser;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        goto END_PROC;
    }
    // 처리는 끝났다 적용 하자..

    // 인벤에 있다면
    if( kPacket_.m_bInven )
    {
        // 지우자..(링이니까..)
        m_kInventory.RemoveItem( kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );

        // 커플 인벤에 넣자...
        if( !IsInCoupleInven( kPacket_.m_kItem.m_dwID ) )
        {
            AddMyInven( kPacket_.m_kItem );
        }
    }


    ChangeRing( m_kCoupleInfo.m_vecMyEquip, kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );
    ChangeRing( m_kCoupleInfo.m_vecPartnerEquip, kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );

    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( GetPartnerUID() );
    if( spUser )
    {
        spUser->ChangeRing( spUser->m_kCoupleInfo.m_vecMyEquip, kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );
        spUser->ChangeRing( spUser->m_kCoupleInfo.m_vecPartnerEquip, kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );
        if( kPacket_.m_bInven )
            AddPartnerInven( kPacket_.m_kItem );
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,        kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_COUPLE_19, kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_COUPLE_15, kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_COUPLE_20, kPacket_.m_nOK, 3 );
        ERR_CASE( ERR_COUPLE_14, kPacket_.m_nOK, 4 );
        ERR_CASE( ERR_COUPLE_22, kPacket_.m_nOK, 5 );
        ERR_CASE( ERR_COUPLE_05, kPacket_.m_nOK, 6 );
        ERR_CASE( ERR_COUPLE_23, kPacket_.m_nOK, 7 );
        ERR_CASE( ERR_COUPLE_24, kPacket_.m_nOK, 8 );
    default:
        START_LOG( cerr, L"링 교체 응답중 알수 없는 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;        
    }

    // 요청 목록에서 지운다.
    SiKCoupleSystem()->DeleteRingRequestInfo( GetUID(), GetPartnerUID() );

    // 두사람에게 모두 보낸다.
    SiKGSSimLayer()->m_kActorManager.SendTo( GetPartnerUID(), KUserEvent::EVENT_CHANGE_COUPLE_RING_ACK, kPacket_ );
    SEND_RECEIVED_PACKET( EVENT_CHANGE_COUPLE_RING_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"커플릴 교체 ACK.. Name : " << m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetPartnerUID() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_bInven )
        << BUILD_LOG( kPacket_.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_DIVORCE_COUPLE_REQ, std::wstring )
{
    KDiborceDBData kDBPacket;
    kDBPacket.m_strMsg.clear();
    KUserPtr spUser;
    int kPacket = 99;
    // 커플인가?
    if( !IsCouple() )
    {
        SEND_PACKET( EVENT_DIVORCE_COUPLE_ACK );
        START_LOG( cwarn, L"커플 상태가 아닌데 커플 해제 패킷을 보냈음.") << END_LOG;
        return;
    }

    // 대상 유저가 존재하는가?
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( GetPartnerUID() );
    PAIR_DWORD prData( GetUID(), GetPartnerUID() );

    if( spUser ) {
        spUser->InitCoupleInfo();
        spUser->m_kInitEquip.clear();
        LIF( spUser->SendPacket( EVENT_TYPE::EVENT_DIVORCE_COUPLE_NOT, kPacket_ ) );
        kDBPacket.m_dwPartnerUID = prData.first;
        _QUEUEING_EVENT_TO_DB( EVENT_DIVORCE_COUPLE_REQ, kDBPacket, spUser );

        spUser->CoupleDivorceNotityToOther( spUser.get(), prData );
        if( spUser->m_pkRoom )
        {
            spUser->m_pkRoom->DivorceUser( spUser->m_strName );
        }
    }

    InitCoupleInfo();
    m_kInitEquip.clear();
    boost::replace_all( kPacket_, L"'", L"''" );

    kDBPacket.m_dwPartnerUID =  prData.second;
    kDBPacket.m_strMsg = kPacket_;
    QUEUEING_EVENT_TO_DB( EVENT_DIVORCE_COUPLE_REQ, kDBPacket );
    kPacket = 0;

    SEND_PACKET( EVENT_DIVORCE_COUPLE_ACK );
    CoupleDivorceNotityToOther( this, prData );
    if( m_pkRoom )
    {
        m_pkRoom->DivorceUser( m_strName );
    }

    START_LOG( clog, L" 커플 해제 완료. Name : " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;   
}

_IMPL_ON_FUNC( EVENT_DIVORCE_COUPLE_NOT, std::wstring )
{
    // 게임 점속후 헤어짐이 알려 졌을때..
    SEND_RECEIVED_PACKET( EVENT_DIVORCE_COUPLE_NOT );
}

_IMPL_ON_FUNC( EVENT_CHANGE_COUPLE_CHAR_REQ, char )
{
    SET_ERROR( ERR_UNKNOWN );
    KUserPtr spUser;

    if( false == IsCharExist( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_COUPLE_26, END_PROC );
    }

    m_kCoupleInfo.m_cCharType = kPacket_;

    // 파트너가 접속중이면 알림
    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( GetPartnerUID() );
    if ( spUser ) {
        spUser->m_kCoupleInfo.m_cPartnerCharType = m_kCoupleInfo.m_cCharType;
        spUser->SendPacket( EVENT_TYPE::EVENT_CHANGE_COUPLE_CHAR_NOT, kPacket_ );
    }
    SET_ERROR( NET_OK );

END_PROC:
    kPacket_ = m_kCoupleInfo.m_cCharType;
    SEND_RECEIVED_PACKET( EVENT_CHANGE_COUPLE_CHAR_ACK );

    _LOG_CONDITION( IS_CORRECT( NET_OK), clog, cwarn, L"Ret : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetName() )
        << BUILD_LOGc( kPacket_ ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_COUPLE_CHAT_REQ, KChatData )
{
    KUserPtr spUser;
    int kPacket = 99;
    SET_ERROR( ERR_UNKNOWN );
    if( !IsCouple() )
    {
        SET_ERR_GOTO( ERR_COUPLE_19, END_PROC );
    }

    if( !CheckChatEnable() ) { // 채팅 블럭 상태.
        SET_ERR_GOTO( ERR_COUPLE_28, END_PROC );
    }

    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( GetPartnerUID() );
    if( !spUser )
    {
        SET_ERR_GOTO( ERR_COUPLE_01, END_PROC );
    }

    kPacket_.m_dwSenderUID      = GetUID();
    kPacket_.m_strSenderNick    = m_strNickName;
    kPacket_.m_dwReceiverUID    = GetPartnerUID();
    kPacket_.m_strReceiverNick  = GetPartnerNick();
    if( kPacket_.m_strChatMsg.size() > 100 )
        kPacket_.m_strChatMsg.resize( 100 );

    if( !spUser->SendPacket( KUserEvent::EVENT_COUPLE_CHAT_NOT, kPacket_ ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_18, END_PROC );
    }

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,        kPacket, 0 ); // 성공
        ERR_CASE( ERR_COUPLE_19, kPacket, 1 ); // 현재 커플 상태가 아님.
        ERR_CASE( ERR_COUPLE_01, kPacket, 2 ); // 상대방의 유저 정보가 존재하지 않음.
        ERR_CASE( ERR_COUPLE_18, kPacket, 3 ); // 대상 유저에게 패킷을 보내지 못했음.
        ERR_CASE( ERR_COUPLE_28, kPacket, 4 ); // 채팅 블럭 상태임.
    default:
        START_LOG( cerr, L"커플 채팅간 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket = 99;
    }

    SEND_PACKET( EVENT_COUPLE_CHAT_ACK );

    START_LOG( clog, L"커플간 채팅. Name : " << m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket )
        << BUILD_LOGc( kPacket_.m_cChatType )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_strSenderNick )
        << BUILD_LOG( kPacket_.m_dwReceiverUID )
        << BUILD_LOG( kPacket_.m_strReceiverNick )
        << BUILD_LOG( kPacket_.m_dwChatColor )
        << BUILD_LOG( kPacket_.m_strChatMsg ) << END_LOG;

    // 채팅기록 남기기(내부에서 옵션 체크)
    SendChatLogNot( L"CoupleChat", -1, kPacket_.m_strChatMsg );
}

_IMPL_ON_FUNC( EVENT_COUPLE_WINDOW_STATE_REQ, int )
{
    KUserPtr spUser;
    SET_ERROR( ERR_UNKNOWN );
    if( !IsCouple() )
    {
        SET_ERR_GOTO( ERR_COUPLE_19, END_PROC );
    }
    m_nCoupleWindowState = kPacket_;

    spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( GetPartnerUID() );
    if( !spUser )
    {
        SET_ERR_GOTO( ERR_COUPLE_01, END_PROC );
    }

    if( !spUser->SendPacket( KUserEvent::EVENT_COUPLE_WINDOW_STATE_NOT, m_nCoupleWindowState ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_18, END_PROC );
    }

    {
        KStatCoupleRoomPtr spStat = GET_STAT( KStatCoupleRoom, SI_STAT_COUPLE_ROOM );
        if( spStat && kPacket_ == 3 )
        {
            spStat->AddStat( true );
        }
    }
    SET_ERROR( NET_OK );

END_PROC:
    START_LOG( clog, L"커플 창 상태. Name : " << m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_ ) << END_LOG;

}

IMPL_ON_FUNC( EVENT_WEEKLY_ATTEND_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    // 출석포인트 보너스 지급 적용.
    m_kGCPoint.SetGCPoint( kPacket_.m_nGCPoint );
    m_kGCPoint.SetInitGCPoint( kPacket_.m_nGCPoint );

    // 인벤에 넣어주자.
    m_kInventory.AddItemList( kPacket_.m_vecInv );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,          kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_CALENDAR_15, kPacket_.m_nOK, 9 ); // 주개근 출석포인트 DB저장 실패
    default:
        START_LOG( cerr, L"주개근 보상처리 중 알수 없는 오류 Name : " << GetName() )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"주개근 보상처리 결과 : " << kPacket_.m_nOK )
        << BUILD_LOGc( kPacket_.m_cMonth )
        << BUILD_LOGc( kPacket_.m_cWeekID )
        << BUILD_LOG( kPacket_.m_nIncGCPoint )
        << BUILD_LOG( kPacket_.m_nGCPoint )
        << BUILD_LOG( kPacket_.m_vecInv.size() )
        << BUILD_LOG( kPacket_.m_nFailResult )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_WEEKLY_ATTEND_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_PET_COSTUM_LIST_REQ )
{
    std::map< std:: pair<DWORD,char>, std::set<DWORD> > kPacket;
    SiKPetHelper()->GetCoustumeList( kPacket );
    __JIF( !kPacket.empty(), return );
    SEND_PACKET( EVENT_PET_COSTUM_LIST_ACK );
}
_IMPL_ON_FUNC( EVENT_GMTOOL_USER_LIST_REQ, int )
{
    if( !IsAdmin() ) return;

    JIF( m_pkChannel );

    KPageInfo kPacket;
    kPacket.m_nMaxPage = 0;
    kPacket.m_nCurrentPage = 0;
    kPacket.m_buffCompList.Clear();

    // 내부에서 에러 셋팅 되어서 옴..
    m_pkChannel->GetAllUserList( kPacket_, kPacket );
    SetTick( SEND_USER_LIST );

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_LIST_00, kPacket.m_nOK, 1 ); // 목록이 비어 있습니다.
        ERR_CASE( ERR_LIST_03, kPacket.m_nOK, 4 ); // 목록 전송 주기가 짧습니다.
    default:
        START_LOG( cerr, L" 알수 없는 Err , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    SEND_PACKET( EVENT_GMTOOL_USER_LIST_ACK );

    START_LOG( clog, L"GM툴에서 채널로비 유저 리스트 요청, Name : " << m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( kPacket.m_nCurrentPage )
        << BUILD_LOG( kPacket.m_nMaxPage ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_GMTOOL_SQUARE_USER_LIST_REQ, PAIR_DWORD_INT )
{
    if( !IsAdmin() ) return;

    JIF( m_pkChannel );

    KPageInfo kPacket;
    kPacket.m_nMaxPage = 0;
    kPacket.m_nCurrentPage = 0;
    kPacket.m_buffCompList.Clear();

    KSquarePtr spSquare = SiKSquareManager()->GetSquare( kPacket_.first );
    if( !spSquare )
    {
        kPacket.m_nOK = 1;
        START_LOG( cerr, L"요청한 광장이 존재 하지 않음. Name : " << m_strName )
            << BUILD_LOG( kPacket_.first )
            << BUILD_LOG( kPacket_.second ) << END_LOG;
        SEND_PACKET( EVENT_GMTOOL_SQUARE_USER_LIST_ACK );
        return;
    }

    spSquare->GetAllUserList( kPacket_.second, kPacket );
    kPacket.m_nOK = 0;
    SEND_PACKET( EVENT_GMTOOL_SQUARE_USER_LIST_ACK );
    START_LOG( clog, L"GM툴에서 광장내 유저 리스트 요청, Name : " << m_strName )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nCurrentPage )
        << BUILD_LOG( kPacket.m_nMaxPage )
        << BUILD_LOG( kPacket.m_buffCompList.GetLength() ) << END_LOG;
}
IMPL_ON_FUNC_NOPARAM( EVENT_GET_USER_DONATION_INFO_REQ )
{
    KEVENT_GET_USER_DONATION_INFO_ACK kPacket;
    kPacket.m_nAccDonationPoint     = m_nAccDonationPoint;
    kPacket.m_nDonationState        = SiKDonationManager()->GetDonationState();
    kPacket.m_nMinDonationPoint     = SiKDonationManager()->GetMinDonationPoint();
    kPacket.m_dwDonationUnit        = SiKDonationManager()->GetDonationUnit();

    SET_ERROR( ERR_UNKNOWN );
    if( kPacket.m_nDonationState == KDonationManager::DS_NONE )
    {
        SET_ERR_GOTO( ERR_DONATION_00, END_PROC );
    }

    QUEUEING_ID_TO_DB( EVENT_GET_USER_DONATION_INFO_REQ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );
        ERR_CASE( ERR_DONATION_00,  kPacket.m_nOK, 1 ); // 현재 기부 기간이 아님
    default:
        START_LOG( cerr, L"유저 기부 정보 요청중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_GET_USER_DONATION_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_GET_USER_DONATION_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    m_nAccDonationPoint             = kPacket_.m_nAccDonationPoint;
    kPacket_.m_nDonationState       = SiKDonationManager()->GetDonationState();
    kPacket_.m_nMinDonationPoint    = SiKDonationManager()->GetMinDonationPoint();
    kPacket_.m_dwDonationUnit       = SiKDonationManager()->GetDonationUnit();

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_DONATION_00,  kPacket_.m_nOK, 1 );  // 현재 기부 기간이 아님
    default:
        START_LOG( cerr, L"유저 기부 정보 요청중 알수 없는 오류. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_GET_USER_DONATION_INFO_ACK );

    START_LOG( clog, L"유저 기부 정보 가져오기 응답 Name : " << m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nDonationState )
        << BUILD_LOG( kPacket_.m_nAccDonationPoint ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_DONATION_INFO_REQ )
{
    // 일단 DB에 먼저 가서 해당 유저의 획득한 Grade List를 가져오는게 첫번째~!
    QUEUEING_ID_TO_DB( EVENT_DONATION_INFO_REQ );
}

IMPL_ON_FUNC( EVENT_DONATION_INFO_ACK )
{
    // DB에서 획득한 GradeList를 함께 전달.
    kPacket_.m_nDonationState        = SiKDonationManager()->GetDonationState();
    kPacket_.m_nAccDonationPoint    = 0;
    kPacket_.m_biTotalDonationPoint  = 0;

    if( kPacket_.m_nDonationState != KDonationManager::DS_NONE )
    {
        kPacket_.m_nAccDonationPoint    = m_nAccDonationPoint;
        SiKDonationManager()->GetDonationRank( kPacket_.m_vecDonationRank );
        kPacket_.m_biTotalDonationPoint = SiKDonationManager()->GetTotalDonation();
    }

    // 현재 기부 상태에 대한 전체 정보 가져오기.
    SiKDonationManager()->GetTotalGradeInfo( kPacket_.m_vecDonationInfo );

    SEND_RECEIVED_PACKET( EVENT_DONATION_INFO_ACK );

    START_LOG( clog, L"현재 기부 상태 요청 Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_nDonationState )
        << BUILD_LOG( kPacket_.m_nAccDonationPoint )
        << BUILD_LOG( kPacket_.m_biTotalDonationPoint )
        << BUILD_LOG( kPacket_.m_vecDonationRank.size() )
        << BUILD_LOG( kPacket_.m_setSuccessGrade.size() )
        << BUILD_LOG( kPacket_.m_vecDonationInfo.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_DONATION_POINT_REQ )
{
    KEVENT_DONATION_POINT_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );

    // 기부 가능한 기간 인가..
    if( SiKDonationManager()->GetDonationState() != KDonationManager::DS_DONATION )
    {
        SET_ERR_GOTO( ERR_DONATION_00, END_PROC );
    }

    // 최소 기부 포인트 보다 적은가?
    if( kPacket_.m_nDonationPoint < SiKDonationManager()->GetMinDonationPoint() )
    {
        SET_ERR_GOTO( ERR_DONATION_03, END_PROC );
    }

    if( SiKDonationManager()->GetDonationUnit() == 0 ) // GP 이면..
    {
        // 나의 GP는 충분 한가?
        if( kPacket_.m_nDonationPoint > GetGP() )
        {
            SET_ERR_GOTO( ERR_DONATION_01, END_PROC );
        }
    }
    else
    {
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemID( static_cast<GCITEMID>(SiKDonationManager()->GetDonationUnit()) );
        if( pkItem == NULL || pkItem->m_nCount < kPacket_.m_nDonationPoint )
        {
            SET_ERR_GOTO( ERR_DONATION_01, END_PROC );
        }
        kPacket.m_kDonationUnit = *pkItem;
    }

    // 기부로 인한 아이템 획득 체크.
    SiKDonationManager()->GetDonationReword( m_nAccDonationPoint, kPacket_.m_nDonationPoint, kPacket_.m_vecRewards );

    // DB로 큐잉.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_DONATION_POINT_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );
        ERR_CASE( ERR_DONATION_00,      kPacket.m_nOK, 1 );     // 현재 기부 기간이 아님
        ERR_CASE( ERR_DONATION_01,      kPacket.m_nOK, 2 );     // 보유 금액이 기부 금액 보다 적음
        ERR_CASE( ERR_DONATION_02,      kPacket.m_nOK, 3 );     // 기부 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 처리중.

    default:
        START_LOG( cerr, L"기부 요청중 알수 없는 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
    {
        kPacket.m_nDonationState        = SiKDonationManager()->GetDonationState();
        kPacket.m_biTotalDonationPoint  = SiKDonationManager()->GetTotalDonation();
        kPacket.m_nAccDonationPoint     = m_nAccDonationPoint;
        kPacket.m_nCurrentGP            = GetGP();
        SEND_PACKET( EVENT_DONATION_POINT_ACK );
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"기부 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( GetGP() )
        << BUILD_LOG( m_nAccDonationPoint )
        << BUILD_LOG( kPacket_.m_nDonationPoint )
        << BUILD_LOG( kPacket_.m_vecRewards.size() )
        << BUILD_LOG( SiKDonationManager()->GetDonationState() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_DONATION_POINT_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    if( SiKDonationManager()->GetDonationUnit() == 0 ) { // GP 이면..
        // GP 깎고
        bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
        DecreaseGP( KGpStatInfo::GDT_DONATION_POINT, bDBUpdate, kPacket_.m_nAccDonationPoint );
    }
    else {
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemID( static_cast<GCITEMID>(SiKDonationManager()->GetDonationUnit()) );
        if( pkItem != NULL )
        {
            pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - kPacket_.m_nAccDonationPoint );
            kPacket_.m_kDonationUnit = *pkItem;
        }
    }

    // 기부 누적 금액 적고.
    m_nAccDonationPoint += kPacket_.m_nAccDonationPoint;
    m_kInventory.AddItemList( kPacket_.m_vecRewardItems );

    SiKDonationManager()->UpdateDonationRankPerUser( m_nAccDonationPoint, m_strNickName );
    SiKDonationManager()->AddDeltaDonationPoint( kPacket_.m_nAccDonationPoint );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_DONATION_00,      kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_DONATION_01,      kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_DONATION_02,      kPacket_.m_nOK, 3 );
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );

    default:
        START_LOG( cerr, L"기부 요청 응답 중 알수 없는 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    kPacket_.m_biTotalDonationPoint = SiKDonationManager()->GetTotalDonation();
    kPacket_.m_nAccDonationPoint    = m_nAccDonationPoint;
    kPacket_.m_nCurrentGP           = GetGP();
    kPacket_.m_nDonationState       = SiKDonationManager()->GetDonationState();

    SEND_RECEIVED_PACKET( EVENT_DONATION_POINT_ACK );
    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"기부 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nDonationState )
        << BUILD_LOG( kPacket_.m_nAccDonationPoint )
        << BUILD_LOG( kPacket_.m_nCurrentGP )
        << BUILD_LOG( kPacket_.m_vecRewardItems.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_ADD_EXP_REQ, KAddExpInfo )
{
    KCharacterInfo* pkCharInfo = NULL;
    SET_ERROR( ERR_UNKNOWN );
    if( !SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) && !IsAdmin() )
    {
        SET_ERR_GOTO( ERR_ADD_EXP_00, END_PROC );
    }

    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if( pkCharInfo == NULL )
    {
        SET_ERR_GOTO( ERR_ADD_EXP_01, END_PROC );
    }

    pkCharInfo->m_biExp = std::max<__int64>( pkCharInfo->m_biExp + kPacket_.m_biExp, USER_MIN_EXP );
    pkCharInfo->m_dwLevel = (DWORD)SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

    // SPX 계산
    SiKSkillManager()->GetSkillPointInfo( kPacket_.m_cCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );

    kPacket_.m_biExp = pkCharInfo->m_biExp;
    kPacket_.m_kSkillInfo = pkCharInfo->m_kSkillInfo;

    m_mapInitExp[pkCharInfo->m_cCharType] = pkCharInfo->m_biExp; // 통계 및 경험치 롤백 방지용 변수를 바꿔야 함.	

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_ADD_EXP_00, kPacket_.m_nOK, 1);
        ERR_CASE( ERR_ADD_EXP_01, kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_ADD_EXP_02, kPacket_.m_nOK, 3 );
    default:
        START_LOG( cerr, L"경험치 더하는데 알수 없는 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << BUILD_LOG( kPacket_.m_biExp )
            << BUILD_LOG( kPacket_.m_kSkillInfo.m_nLvSPPoint )
            << BUILD_LOG( kPacket_.m_kSkillInfo.m_nMaxSPPoint ) << END_LOG;
    }

    {
        std::wstringstream stm;
        stm << KNC_TIME_STRING << L"|" << m_strName
            << L"|" << kPacket_.m_nOK << L"|" << kPacket_.m_cCharType << L"|" << kPacket_.m_biExp
            << L"|" << SiKGSSimLayer()->GetTypeFlag() << L"|" << IsAdmin();
        SiKLogManager()->Get( KLogManager::LT_ADD_EXP_USER ) << stm.str() << GLOG::endl;
    }

    LOG_CONDITION( kPacket_.m_nOK == 0, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_biExp )
        << BUILD_LOG( kPacket_.m_kSkillInfo.m_nLvSPPoint )
        << BUILD_LOG( kPacket_.m_kSkillInfo.m_nMaxSPPoint ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ADD_EXP_ACK );
}

_IMPL_ON_FUNC( EVENT_CHECK_LUNA_NEWYEAR_ACK, bool )
{
    m_bLunaNewYearEventDone = kPacket_;
}

IMPL_ON_FUNC_NOPARAM( EVENT_LUNA_NEWYEAR_REQ )
{
    KEVENT_LUNA_NEWYEAR_ACK kPacket;
    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간인가?
    if( !SiKLunaNewYear()->IsActive() ) {
        SET_ERR_GOTO( ERR_LUNA_NEWYEAR_00, END_PROC );
    }

    // 오늘은 이미 받았는가?
    if( m_bLunaNewYearEventDone ) {
        SET_ERR_GOTO( ERR_LUNA_NEWYEAR_01, END_PROC );
    }

    QUEUEING_ID_TO_DB_CHECK( EVENT_LUNA_NEWYEAR_REQ );
    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );
        ERR_CASE( ERR_LUNA_NEWYEAR_00,  kPacket.m_nOK, 1 );
        ERR_CASE( ERR_LUNA_NEWYEAR_01,  kPacket.m_nOK, 2 );
        ERR_CASE( ERR_LUNA_NEWYEAR_02,  kPacket.m_nOK, 3 );

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        START_LOG( cerr, L"세뱃돈 요청중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( !IS_CORRECT( NET_OK ) )
        SEND_PACKET( EVENT_LUNA_NEWYEAR_ACK );

    _LOG_SUCCESS( IS_CORRECT( NET_OK ), L"세뱃돈 이벤트 요청 Name : " << m_strName )
        << L"MSG : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_LUNA_NEWYEAR_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK ) )
        goto END_PROC;

    // 이벤트 수행 정보 셋팅 한다.
    m_bLunaNewYearEventDone = true;

    // 적용 한다.
    IncreaseGP( KGpStatInfo::GIT_LUNA_NEWYEAR, false, kPacket_.m_nRewardGP );
    kPacket_.m_nCurrentGP = GetGP();
    LIF( m_kInventory.AddItemList( kPacket_.m_vecRewardItems ) );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_LUNA_NEWYEAR_00,  kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님
        ERR_CASE( ERR_LUNA_NEWYEAR_01,  kPacket_.m_nOK, 2 ); // 오늘 이미 세배를 했음
        ERR_CASE( ERR_LUNA_NEWYEAR_02,  kPacket_.m_nOK, 3 ); // 메니저에 보상 정보가 없음
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다
    default:
        START_LOG( cerr, L"세뱃돈 응답중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    SEND_RECEIVED_PACKET( EVENT_LUNA_NEWYEAR_ACK );
    _LOG_CONDITION( IS_CORRECT( NET_OK ), clog, cwarn, L"세뱃돈 이벤트 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nRewardGP )
        << BUILD_LOG( kPacket_.m_nCurrentGP )
        << BUILD_LOG( kPacket_.m_vecRewardItems.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_CHAR_POLL_TERM_REQ )
{
    KEVENT_CHAR_POLL_TERM_ACK kPacket;
    kPacket.m_bIsPollTerm = SiKCharacterPollManager()->IsEnablePoll();
    kPacket.m_dwPollItemID = SiKCharacterPollManager()->GetPollItemID();
    SEND_PACKET( EVENT_CHAR_POLL_TERM_ACK );
}

IMPL_ON_FUNC( EVENT_CHAR_POLL_REQ )
{
    KEVENT_CHAR_POLL_ACK kPacket;
    KItemPtr pkItem;
    SET_ERROR( ERR_UNKNOWN );

    // 현재 투표 기간인지
    if( !SiKCharacterPollManager()->IsEnablePoll() )
    {
        SET_ERR_GOTO( ERR_CHAR_POLL_00, END_PROC );
    }

    // 요청한 캐릭터가 옳은지.
    if( !SiKCharacterPollManager()->IsCorrectChar( kPacket_.m_cCharType ) )
    {
        SET_ERR_GOTO( ERR_CHAR_POLL_01, END_PROC );
    }

    // 요청한 아이템이 투표 아이템이 맞는지.
    if( !SiKCharacterPollManager()->IsPollItem( kPacket_.m_itemID ) )
    {
        SET_ERR_GOTO( ERR_CHAR_POLL_02, END_PROC );
    }

    // 1개 이상 투표 하는지
    if( kPacket_.m_nUseCount <= 0 )
    {
        SET_ERR_GOTO( ERR_CHAR_POLL_03, END_PROC );
    }

    // 요청한 아이템이 인벤에 존재 하는지.
    
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_itemID );
    if( pkItem == NULL )
    {
        SET_ERR_GOTO( ERR_CHAR_POLL_04, END_PROC );
    }

    // 요청한 아이템 수량이 충분한지.
    if( pkItem->m_nCount < kPacket_.m_nUseCount )
    {
        SET_ERR_GOTO( ERR_CHAR_POLL_05, END_PROC );
    }

    // 투표.
    SiKCharacterPollManager()->AddPoll( kPacket_.m_cCharType, kPacket_.m_nUseCount );

    // 아이템 차감.
    pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - kPacket_.m_nUseCount );

    // 남은 아이템 정보 전달.
    kPacket.m_kItem = *pkItem;
    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,  kPacket.m_nOK, 0 );
        ERR_CASE( ERR_CHAR_POLL_00,  kPacket.m_nOK, 1 ); // 인기 투표 기간이 아님
        ERR_CASE( ERR_CHAR_POLL_01,  kPacket.m_nOK, 2 ); // 적용 되는 캐릭터가 아님
        ERR_CASE( ERR_CHAR_POLL_02,  kPacket.m_nOK, 3 ); // 투표용 아이템이 아님
        ERR_CASE( ERR_CHAR_POLL_03,  kPacket.m_nOK, 4 ); // 투표용 아이템을 1개 이상 사용해야 함
        ERR_CASE( ERR_CHAR_POLL_04,  kPacket.m_nOK, 5 ); // 투표용 아이템을 보유 하고 있지 않음
        ERR_CASE( ERR_CHAR_POLL_05,  kPacket.m_nOK, 6 ); // 투표용 아이템의 수량이 부족
    default:
        START_LOG( cerr, L"인기 투표중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    SEND_PACKET( EVENT_CHAR_POLL_ACK );

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L" 인기 투표 요청 Name : " << m_strName )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_itemID )
        << BUILD_LOG( kPacket_.m_nUseCount )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_kItem.m_nCount )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_GACHA_COIN_COUNT_REQ )
{
    _VERIFY_STATE(( 4, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_SQUARE, KUserFSM::STATE_AGIT ));
    //가챠 코인은 아이템이 아니다. 단지 코인의 개수만 있다.
    //게임내 구매든 웹 구매든 모두 웹구매 처리된다.
    START_LOG( clog, L"가챠 코인수 갱신 요청 받음. Name : " << m_strName ) << END_LOG;

    if( SiKGSSimLayer()->m_bGachaEnable == false )
    {
        int kPacket = 0;
        SEND_PACKET( EVENT_GACHA_COIN_COUNT_ACK );
        return;
    }

    QUEUEING_ID_TO_DB( EVENT_DB_GACHA_COIN_COUNT_REQ );
}

_IMPL_ON_FUNC( EVENT_GACHA_COIN_COUNT_ACK, int )
{
    JIF( PacketHackCheck( nFrom_ ) );

    START_LOG( clog, L"가챠 코인수 갱신됨." )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GACHA_COIN_COUNT_ACK );
}

IMPL_ON_FUNC( EVENT_BUY_FOR_VIP_REQ )
{
    _VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));

    KDB_EVENT_BUY_FOR_VIP_REQ kDBPacket;
    KEVENT_BUY_FOR_VIP_ACK kPacket;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    if ( GetCurrentCharType() != kPacket_.m_nCharType ) {
        START_LOG( cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName() )
            << END_LOG;
        SET_ERR_GOTO( ERR_VIP_ERR_04, END_PROC );
    }

    // 해당 아이템을 보유 하고 있는가?
    pkItem = m_kInventory.FindDurationItemByItemID( KEVENT_BUY_FOR_VIP_REQ::ROYAL_VIP_BADGE_GOODS_ID );
    if( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_VIP_ERR_00, END_PROC );
    }

    // Vip 개수는 충분한가?
    if( pkItem->m_nCount < kPacket_.m_nItemPrice ) {
        SET_ERR_GOTO( ERR_VIP_ERR_00, END_PROC );
    }

    // 요청한 VIP Item 정보가 서버의 정보가 같은가?
    if( SiKGCHelper()->IsCurrentVipItemInfo( kPacket_.m_nCharType, kPacket_.m_kBuyItem.m_ItemID, kPacket_.m_nItemPrice ) == false )
    {
        SET_ERR_GOTO( ERR_VIP_ERR_01, END_PROC );
    }

    kDBPacket.m_kPacket = kPacket_;
    kDBPacket.m_kVIPItem = *pkItem;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BUY_FOR_VIP_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = -99;

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK,   0 ); // 성공
        ERR_CASE( ERR_VIP_ERR_00,       kPacket.m_nOK,   1 ); // VIP뱃지 보유량 부족.
        ERR_CASE( ERR_VIP_ERR_01,       kPacket.m_nOK,   2 ); // 요청한 VIP Item 정보가 서버정보와 다름.
        ERR_CASE( ERR_VIP_ERR_02,       kPacket.m_nOK,   3 ); // VIP 아이템 얻기 DB 작업 실패
        ERR_CASE( ERR_VIP_ERR_03,       kPacket.m_nOK,   4 ); // VIP왕실뱃지 깎기 실패
        ERR_CASE( ERR_VIP_ERR_04,       kPacket.m_nOK,   5 ); // 캐릭터 정보가 없음
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 진행중인 작업입니다.
    default:
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"VIP ITEM 구매시 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || kPacket.m_nOK == -98 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOGc( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_kBuyItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kBuyItem.m_nDuration )
        << BUILD_LOG( kPacket_.m_kBuyItem.m_nPeriod )
        << BUILD_LOG( kPacket_.m_nItemPrice ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        SEND_PACKET( EVENT_BUY_FOR_VIP_ACK );
    }

    if( NetError::GetLastNetError() == NetError::ERR_VIP_ERR_01)
    {
        SendVIPItemListNot(); // 서버정보와 다르면 로얄VIP뱃지 ItemList 재전달.
    }
}

IMPL_ON_FUNC( EVENT_BUY_FOR_VIP_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    KItemPtr pkItem;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }
    // 낙장 불입.

        // DB작업 성공하면 아이템정보 동기화.
        pkItem = m_kInventory.FindItem( kPacket_.m_kVipBadge.m_ItemID, kPacket_.m_kVipBadge.m_ItemUID );
        if( NULL != pkItem ) {
            pkItem->m_nInitCount = kPacket_.m_kVipBadge.m_nCount;
            pkItem->m_nCount = kPacket_.m_kVipBadge.m_nCount;
            kPacket_.m_kVipBadge = *pkItem;
        }

        // 구매한 아이템을 넣어 준다.
        m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( kPacket_.m_nOK )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK,   0 ); // 성공
        ERR_CASE( ERR_VIP_ERR_00,       kPacket_.m_nOK,   1 ); // VIP뱃지 보유량 부족.
        ERR_CASE( ERR_VIP_ERR_01,       kPacket_.m_nOK,   2 ); // 요청한 VIP Item 정보가 서버정보와 다름.
        ERR_CASE( ERR_VIP_ERR_02,       kPacket_.m_nOK,   3 ); // VIP 아이템 얻기 DB 작업 실패
        ERR_CASE( ERR_VIP_ERR_03,       kPacket_.m_nOK,   4 ); // VIP왕실뱃지 깎기 실패
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 진행중인 작업입니다.
    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"VIP ITEM 구매시 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || kPacket_.m_nOK == -98 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kVipBadge.m_ItemID )
        << BUILD_LOG( kPacket_.m_kVipBadge.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kVipBadge.m_nCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BUY_FOR_VIP_ACK );
}

_IMPL_ON_FUNC( EVENT_USER_PING_INFO_NOT, VECTOR_PING_INFO )
{
    std::vector<KStatPingInfo> ::iterator vit;

    for( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit )
    {
        KStatPingInfo kKey;
        kKey.m_nType            = vit->m_nType;
        kKey.m_wstrSendCCode    = vit->m_wstrSendCCode;
        kKey.m_wstrRecvCCode    = vit->m_wstrRecvCCode;
        kKey.m_nPingTime        = vit->m_nPingTime;

        boost::trim( vit->m_wstrSendCCode );
        boost::trim( vit->m_wstrRecvCCode );

        if( kKey.m_wstrSendCCode.empty() ) kKey.m_wstrSendCCode = L"ZZ";
        if( kKey.m_wstrRecvCCode.empty() ) kKey.m_wstrRecvCCode = L"ZZ";

        if( !kKey.m_wstrSendCCode.empty() && !kKey.m_wstrRecvCCode.empty() )
        {
            // 국가 코드를 항상 대소 구분해서 낮은 나라부터 적기( ex) US, VN이면 US가 먼저 )
            if( kKey.m_wstrSendCCode > kKey.m_wstrRecvCCode )
            {
                kKey.m_wstrSendCCode.swap( kKey.m_wstrRecvCCode );
            }
            AddStatisticsPing( kKey );
        }
    }
}

IMPL_ON_FUNC( EVENT_OPEN_CAPSULE_REQ )
{
    JIF( SiKGPCapsuleManager()->GetGPCapsuleEnable() == true );

    KCapsuleInfo kCapsuleInfo;
    KDropItemInfo kRewardItem;

    KDB_EVENT_OPEN_CAPSULE_REQ kDBPacket;
    KEVENT_OPEN_CAPSULE_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nRemain = 0;
    kPacket.m_nItemGrade = -1;

    SET_ERROR( ERR_UNKNOWN );

    // 해당 캡슐 정보 얻어오기
    if( SiKGPCapsuleManager()->GetCurrentCapsuleInfo( kPacket_.m_nCharType, kPacket_.m_nItemKind, kPacket_.m_nCapsuleIndex, kPacket_.m_nPrice, kCapsuleInfo ) == false )
    {
        START_LOG( cerr, L"해당 캡슐 정보가 없음." ) << END_LOG;
        SET_ERR_GOTO( ERR_CAPSULE_00, END_PROC );
    }

    // 화폐 타입에 따른 체크 
    if( KCapsule::PT_GP == kPacket_.m_nPriceType ) {
        // GP는 충분한가?
        if( GetGP() < kPacket_.m_nPrice ) {
            START_LOG( cerr, L"캡슐가격보다 GP가 부족함." ) << END_LOG;
            SET_ERR_GOTO( ERR_CAPSULE_01, END_PROC );
        }
    }
    else if( KCapsule::PT_CRYSTAL == kPacket_.m_nPriceType ) {
        // 해당 아이템을 보유 하고 있는가?
        KItemPtr pkItem = m_kInventory.FindItemByItemID( CRYSTAL_GOODS_ID );
        if( NULL == pkItem ) {
            SET_ERR_GOTO( ERR_CAPSULE_08, END_PROC );
        }
        // 크리스탈 개수는 충분한가?
        if( pkItem->m_nCount < kPacket_.m_nPrice ) {
            SET_ERR_GOTO( ERR_CAPSULE_09, END_PROC );
        }
    }
    else {
        START_LOG( cerr, L"캡슐 화폐 타입설정이 잘못 되었음." ) << END_LOG;
        SET_ERR_GOTO( ERR_CAPSULE_10, END_PROC );
    }

    // 가격정보(갯수)가 동일한가?
    if( kCapsuleInfo.m_nPrice != kPacket_.m_nPrice ) {
        START_LOG( cerr, L"서버와 캡슐가격이 다름" ) << END_LOG;
        SET_ERR_GOTO( ERR_CAPSULE_02, END_PROC );
    }

    // 획득할 아이템의 등급을 결정하자.
    if( SiKGPCapsuleManager()->GetItemGrade( kCapsuleInfo, kPacket.m_nItemGrade ) == false ) {
        START_LOG( cerr, L"캡슐 아이템 등급 결정 실패" ) << END_LOG;
        SET_ERR_GOTO( ERR_CAPSULE_03, END_PROC );
    }

    // 아이템 뽑기(내부에서 에러값 세팅)
    if( SiKGPCapsuleManager()->OpenCapsule( kCapsuleInfo, kPacket.m_nItemGrade, kRewardItem ) == false ) {
        START_LOG( cerr, L"캡슐 아이템 뽑기 실패" ) << END_LOG;
        goto END_PROC;
    }

    // DB에 넘겨줄 데이터 세팅.
    kDBPacket.m_nCharType = kPacket_.m_nCharType;
    kDBPacket.m_nItemkind = kPacket_.m_nItemKind;
    kDBPacket.m_prLevelRange = kCapsuleInfo.m_prLevelRange;
    kDBPacket.m_kRewardItem = kRewardItem;
    kDBPacket.m_nPriceType = kPacket_.m_nPriceType;
    kDBPacket.m_nPrice = kPacket_.m_nPrice;
    kDBPacket.m_nItemGrade = kPacket.m_nItemGrade;

    // DB에 Item 넣고, GP캡슐 구매 로그 남기자.
    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_OPEN_CAPSULE_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_CAPSULE_00,       kPacket.m_nOK, 1 ); // 해당하는 GP 캡슐 정보가 없음.
        ERR_CASE( ERR_CAPSULE_01,       kPacket.m_nOK, 2 ); // GP가 부족함.
        ERR_CASE( ERR_CAPSULE_02,       kPacket.m_nOK, 3 ); // 가격 정보가 다르다.
        ERR_CASE( ERR_CAPSULE_03,       kPacket.m_nOK, 4 ); // 아이템 등급 확률 이상.
        ERR_CASE( ERR_CAPSULE_04,       kPacket.m_nOK, 5 ); // 캡슐 뽑기 꽝. 100퍼센트를 보장하면 나오면 안됨.
        ERR_CASE( ERR_CAPSULE_05,       kPacket.m_nOK, 6 ); // 캡슐 아이템인데, Lottry에 등록되지 않음.
        ERR_CASE( ERR_CAPSULE_06,       kPacket.m_nOK, 7 ); // 없는 캡슐 아이템 등급입니다.
        ERR_CASE( ERR_CAPSULE_07,       kPacket.m_nOK, 8 ); // GP캡슐 아이템 얻기 DB 작업 실패
        ERR_CASE( ERR_CAPSULE_08,       kPacket.m_nOK, 9 ); // 크리스탈 아이템 가져오기 실패
        ERR_CASE( ERR_CAPSULE_09,       kPacket.m_nOK, 10 ); // 크리스탈 보유량이 부족함
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"GP 캡슐 열기 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nCharType )
        << BUILD_LOG( kPacket_.m_nItemKind )
        << BUILD_LOG( kPacket_.m_nCapsuleIndex )
        << BUILD_LOG( kPacket_.m_nPrice )
        << BUILD_LOG( GetGP() )
        << BUILD_LOG( kRewardItem.m_ItemID )
        << BUILD_LOG( kRewardItem.m_nDuration )
        << BUILD_LOG( kRewardItem.m_nPeriod )
        << BUILD_LOG( kCapsuleInfo.m_prLevelRange.first )
        << BUILD_LOG( kCapsuleInfo.m_prLevelRange.second ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        SEND_PACKET( EVENT_OPEN_CAPSULE_ACK );
    }

    if( NetError::GetLastNetError() == NetError::ERR_CAPSULE_00 ||
        NetError::GetLastNetError() == NetError::ERR_CAPSULE_02 )
    {
        SendCapsuleListNot(); // 정보가 다르니 리스트를 다시 보낸다.
    }
}

IMPL_ON_FUNC( EVENT_OPEN_CAPSULE_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    int nPrice = kPacket_.m_nRemain; // 캡슐가격( 크리스탈 갯수 )

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    // 화폐 타입이 GP일 경우 
    if( KCapsule::PT_GP == kPacket_.m_nPriceType )
    {
        bool bDBUpdate = false; // DB에서 GP 갱신 처리 하지 않았음.
        DecreaseGP( KGpStatInfo::GDT_OPEN_CAPSULE, bDBUpdate, nPrice );
        kPacket_.m_nRemain = GetGP(); // 현재 GP 잔액 전달.
    }

    // 화폐 타입이 크리스탈일 경우 
    if( KCapsule::PT_CRYSTAL == kPacket_.m_nPriceType )
    {
        // 해당 아이템을 보유 하고 있는가?
        KItemPtr pkItem = m_kInventory.FindItemByItemID( CRYSTAL_GOODS_ID );
        if( NULL != pkItem )
        {
            // 낙장불입
            pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - nPrice );
            // 수량 Item Check Tip.
            // 현재 크리스탈 갯수만 차감한다. 아이템 수량 정보는 두가지로 구분된다.
            // dwCurrentNum은 현재 갯수, dwInitNum은 DB에서 얻어올때의 갯수이다.
            // DB에서의 처리가 이루어 지지 않았으면 dwCurrentNum의 값만 감소시키고,
            // DB에서의 처리가 이루어 진 상태이면 dwInitNum의 값도 함께 감소시킨다.
            // if( pkItem->m_dwCurrentNum <= 0 )
            //    EraseDurationItem( pkItem->m_dwGoodsUID, pkItem->m_dwGoodsID );
            // 위의 코드같이 dwInitNum값이 감소되지 않으면 해당 아이템이 0이되더라고
            // 삭제하면 안된다.
            kPacket_.m_nRemain = pkItem->m_nCount;

        }
    }

    LIF( m_kInventory.AddItemList( kPacket_.m_vecItem ) );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_CAPSULE_00,       kPacket_.m_nOK, 1 ); // 해당하는 GP 캡슐 정보가 없음.
        ERR_CASE( ERR_CAPSULE_01,       kPacket_.m_nOK, 2 ); // GP가 부족함.
        ERR_CASE( ERR_CAPSULE_02,       kPacket_.m_nOK, 3 ); // 가격 정보가 다르다.
        ERR_CASE( ERR_CAPSULE_03,       kPacket_.m_nOK, 4 ); // 아이템 등급 확률 이상.
        ERR_CASE( ERR_CAPSULE_04,       kPacket_.m_nOK, 5 ); // 캡슐 뽑기 꽝. 100퍼센트를 보장하면 나오면 안됨.
        ERR_CASE( ERR_CAPSULE_05,       kPacket_.m_nOK, 6 ); // 캡슐 아이템인데, Lottry에 등록되지 않음.
        ERR_CASE( ERR_CAPSULE_06,       kPacket_.m_nOK, 7 ); // 없는 캡슐 아이템 등급입니다.
        ERR_CASE( ERR_CAPSULE_07,       kPacket_.m_nOK, 8 ); // GP캡슐 아이템 얻기 DB 작업 실패
        ERR_CASE( ERR_CAPSULE_08,       kPacket_.m_nOK, 9 ); // 크리스탈 아이템 가져오기 실패
        ERR_CASE( ERR_CAPSULE_09,       kPacket_.m_nOK, 10 ); // 크리스탈 보유량이 부족함
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"GP 캡슐 처리에 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_CONDITION( kPacket_.m_nOK == 0 || kPacket_.m_nOK == -98, clog, cerr )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( kPacket_.m_nRemain )
        << BUILD_LOG( kPacket_.m_nItemGrade )
        << BUILD_LOG( nPrice ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_OPEN_CAPSULE_ACK );

    // 여기서부턴 성공한 유저만 처리한다.
    if( kPacket_.m_nOK != 0 )
        return;

    // 레어나 이벤트 아이템이면 광장 유저에게 알려주자.
    if( kPacket_.m_nItemGrade == KGPCapsuleManager::IG_RARE ||
        kPacket_.m_nItemGrade == KGPCapsuleManager::IG_EVENT )
    {
        GCITEMID itemID = 0;

        if( !kPacket_.m_vecItem.empty() ) {
            itemID = kPacket_.m_vecItem.front().m_ItemID;
        }
        else {
            START_LOG( cerr, L"레어/이벤트 아이템을 획득했는데, 받은 아이템이 비어있다. ItemGrade:" << kPacket_.m_nItemGrade ) << END_LOG;
            return;
        }

        KEVENT_CAPSULE_RESULT_NOT kNot;
        kNot.m_dwUserUID = GetUID();
        kNot.m_nItemGrade = kPacket_.m_nItemGrade;
        kNot.m_dwItemID = itemID;

        SiKSquareManager()->SendToAll( KUserEvent::EVENT_CAPSULE_RESULT_NOT, kNot );
    }
}

IMPL_ON_FUNC_NOPARAM( EVENT_GAME_COUPON_LIST_REQ )
{
    JIF( SiKGSSimLayer()->m_bEnableGameCoupon == true );

    // 클라이언트에서 쿠폰NPC 클릭시에 게임쿠폰정보 리스트를 요청한다.
    KEVENT_GAME_COUPON_LIST_ACK kPacket;

    SiKGameCouponManager()->GetGameCouponInfo( kPacket );
    START_LOG( clog, L"GameCoupon list Notice. size : " << kPacket.size() ) << END_LOG;

    SEND_COMPRESS_PACKET( EVENT_GAME_COUPON_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EVENT_USE_GAME_COUPON_REQ )
{
    JIF( SiKGSSimLayer()->m_bEnableGameCoupon == true );

    KEVENT_USE_GAME_COUPON_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );

    // 유효한 쿠폰 이벤트인가?
    if( SiKGameCouponManager()->IsValidGameCouponInfo( kPacket_.m_nEventUID ) == false )
    {
        START_LOG( cerr, L"해당 이벤트가 없거나 이벤트 기간이 아님. EventUID : " << kPacket_.m_nEventUID << L",CouponID : " << kPacket_.m_strCouponID ) << END_LOG;
        SET_ERR_GOTO( ERR_GAME_COUPON_01, END_PROC );
    }

    // DB에 넘어오면 안되는 스트링이 있는가?
    if( SiKSlangFilter()->IsInQueryInvalidChar( kPacket_.m_strCouponID ) == true )
    {
        START_LOG( cerr, L"입력할 수 없는 문자가 포함되어 있습니다. EventUID : " << kPacket_.m_nEventUID << L",CouponID : " << kPacket_.m_strCouponID ) << END_LOG;
        SET_ERR_GOTO( ERR_GAME_COUPON_02, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_USE_GAME_COUPON_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GAME_COUPON_00,   kPacket.m_nOK, 1 ); // 유저 정보가 없음.
        ERR_CASE( ERR_GAME_COUPON_01,   kPacket.m_nOK, 2 ); // 해당 이벤트가 없거나 이벤트 기간이 아님.
        ERR_CASE( ERR_GAME_COUPON_02,   kPacket.m_nOK, 3 ); // 입력할 수 없는 문자가 포함되어 있습니다.
        ERR_CASE( ERR_GAME_COUPON_03,   kPacket.m_nOK, 4 ); // 쿠폰 번호가 없거나 틀리거나 이미 사용되었음.
        ERR_CASE( ERR_GAME_COUPON_04,   kPacket.m_nOK, 5 ); // 게임쿠폰 아이템넣기 DB작업 실패
        ERR_CASE( ERR_GAME_COUPON_05,   kPacket.m_nOK, 6 ); // 쿠폰이 존재하지 않음
        ERR_CASE( ERR_GAME_COUPON_06,   kPacket.m_nOK, 7 ); // 쿠폰 체크 조건이 다름_패스워드
        ERR_CASE( ERR_GAME_COUPON_07,   kPacket.m_nOK, 8 ); // 쿠폰 체크 조건이 다름_이벤트
        ERR_CASE( ERR_GAME_COUPON_08,   kPacket.m_nOK, 9 ); // 패스워드가 틀림
        ERR_CASE( ERR_GAME_COUPON_09,   kPacket.m_nOK, 11 ); // 쿠폰과 이벤트 정보가 연결되어 있지 않음
        ERR_CASE( ERR_GAME_COUPON_10,   kPacket.m_nOK, 12 ); // 쿠폰과 아이템 정보가 연결되어 있지 않음
        ERR_CASE( ERR_GAME_COUPON_11,   kPacket.m_nOK, 13 ); // 일일 입력 한도 횟수 초과
        ERR_CASE( ERR_DB_COMMON_00,     kPacket.m_nOK, 14 ); // 트랜잭션 에러
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"게임쿠폰 사용중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nEventUID )
        << BUILD_LOG( kPacket_.m_strCouponID ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        SEND_PACKET( EVENT_USE_GAME_COUPON_ACK );
    }
}

IMPL_ON_FUNC( EVENT_USE_GAME_COUPON_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT(NET_OK) ) {
        goto END_PROC;
    }

    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_GAME_COUPON_00,   kPacket_.m_nOK, 1 ); // 유저 정보가 없음.
        ERR_CASE( ERR_GAME_COUPON_01,   kPacket_.m_nOK, 2 ); // 해당 이벤트가 없거나 이벤트 기간이 아님.
        ERR_CASE( ERR_GAME_COUPON_02,   kPacket_.m_nOK, 3 ); // 입력할 수 없는 문자가 포함되어 있습니다.
        ERR_CASE( ERR_GAME_COUPON_03,   kPacket_.m_nOK, 4 ); // 쿠폰 번호가 없거나 틀리거나 이미 사용되었음.
        ERR_CASE( ERR_GAME_COUPON_04,   kPacket_.m_nOK, 5 ); // 게임쿠폰 아이템넣기 DB작업 실패
        ERR_CASE( ERR_GAME_COUPON_05,   kPacket_.m_nOK, 6 ); // 쿠폰이 존재하지 않음
        ERR_CASE( ERR_GAME_COUPON_06,   kPacket_.m_nOK, 7 ); // 쿠폰 체크 조건이 다름_패스워드
        ERR_CASE( ERR_GAME_COUPON_07,   kPacket_.m_nOK, 8 ); // 쿠폰 체크 조건이 다름_이벤트
        ERR_CASE( ERR_GAME_COUPON_08,   kPacket_.m_nOK, 9 ); // 패스워드가 틀림
        ERR_CASE( ERR_GAME_COUPON_09,   kPacket_.m_nOK, 11 ); // 쿠폰과 이벤트 정보가 연결되어 있지 않음
        ERR_CASE( ERR_GAME_COUPON_10,   kPacket_.m_nOK, 12 ); // 쿠폰과 아이템 정보가 연결되어 있지 않음
        ERR_CASE( ERR_GAME_COUPON_11,   kPacket_.m_nOK, 13 ); // 일일 입력 한도 횟수 초과
        ERR_CASE( ERR_DB_COMMON_00,     kPacket_.m_nOK, 14 ); // 트랜잭션 에러
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"게임쿠폰 사용응답에서 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"쿠폰 사용 응답. : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_USE_GAME_COUPON_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_GAME_COUPON_FAIL_NOT )
{
    START_LOG( clog, L"게임쿠폰 입력 연속 3회 실패. Name : " << m_strName ) << END_LOG;
    QUEUEING_ID_TO_DB( EVENT_GAME_COUPON_FAIL_NOT );
}

IMPL_ON_FUNC_NOPARAM( EVENT_CHECK_RECOMMEND_ENABLE_REQ )
{
    KEVENT_CHECK_RECOMMEND_ENABLE_ACK kPacket;
    kPacket.m_nOK = 99;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간 인가?
    if( !SiKRecomHelper()->IsEventTerm() )
        SET_ERR_GOTO( ERR_RECOMMEND_00, END_PROC );

    // 신규 혹은 복귀 유저 인가?(추천 할수 있는 상태 인가?)
    if( m_MyRecomInfo.m_nState != KRecomHelper::RBS_NONE )
        SET_ERR_GOTO( ERR_RECOMMEND_01, END_PROC );

    QUEUEING_ID_TO_DB_CHECK( EVENT_CHECK_RECOMMEND_ENABLE_REQ );

    SET_ERROR( NET_OK );

END_PROC:
    SiKRecomHelper()->GetRecommendeeReward( kPacket.m_mapRecomBonus );
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket.m_nOK, 1 ); //이벤트 기간이 아님.
        ERR_CASE( ERR_RECOMMEND_01, kPacket.m_nOK, 2 ); //신규 혹은 복귀 유저가 아님.
        ERR_CASE( ERR_RECOMMEND_02, kPacket.m_nOK, 3 ); //DB 처리 오류.
        ERR_CASE( ERR_RECOMMEND_03, kPacket.m_nOK, 4 ); //추천을 할수 없는 상태임.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        kPacket.m_nOK = -99;
        START_LOG( cerr, L"추천 여부 확인 요청중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    START_LOG( clog, L"추천 여부 확인 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_mapRecomBonus.size() ) << END_LOG;

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_CHECK_RECOMMEND_ENABLE_ACK );
}

IMPL_ON_FUNC( EVENT_CHECK_RECOMMEND_ENABLE_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    SiKRecomHelper()->GetRecommendeeReward( kPacket_.m_mapRecomBonus );

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_RECOMMEND_01, kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_RECOMMEND_02, kPacket_.m_nOK, 3 );
        ERR_CASE( ERR_RECOMMEND_03, kPacket_.m_nOK, 4 );
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"추천 여부 확인 응답중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_CHECK_RECOMMEND_ENABLE_ACK );
    START_LOG( clog, L"추천 여부 확인 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_mapRecomBonus.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_RECOMMEND_USER_REQ )
{
    KEVENT_RECOMMEND_USER_ACK  kPacket;
    KDB_EVENT_RECOMMEND_USER_REQ kDBPacket;
    kPacket.m_nOK = 99;
    kPacket.m_nRecomInfo.m_nState = kPacket_.m_nState;

    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간 인가?
    if( !SiKRecomHelper()->IsEventTerm() )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_00, END_PROC );
    }

    // 신규 혹은 복귀 유저 인가?(추천 할수 있는 상태 인가?)
    if( m_MyRecomInfo.m_nState != KRecomHelper::RBS_NONE || IsExpAccount() )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_01, END_PROC );
    }

    if( kPacket_.m_nState < KRecomHelper::RUO_RECOMMEND ||
        kPacket_.m_nState > KRecomHelper::RUO_DEFER )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_05, END_PROC );
    }

    if( kPacket_.m_nState == KRecomHelper::RUO_DEFER ) //나중에 추천 이면 여기서 끝내자..
    {
        SET_ERR_GOTO( NET_OK, END_PROC );
    }

    // 사용 할수 없는 문자가 포함 되어 있는가?
    if( kPacket_.m_nState == KRecomHelper::RUO_RECOMMEND && // 추천을 하는 경우
        (kPacket_.m_strNick.empty() || (int)(kPacket_.m_strNick.size()) > SiKGSSimLayer()->m_uiNickNameAvailable ) )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_04, END_PROC );
    }

    kDBPacket.m_nState = kPacket_.m_nState;
    kDBPacket.m_strNick = kPacket_.m_strNick;
    kDBPacket.m_nCurrentLevel = 0;

    // 추천을 한 캐릭터의 레벨을 DB로 전달
    KCharacterInfo* pkCharacterInfo = GetCurrentCharacter();
    if( NULL != pkCharacterInfo ) {
        kDBPacket.m_nCurrentLevel = static_cast<int>( pkCharacterInfo->m_dwLevel );
    }

    QUEUEING_EVENT_TO_DB_CHECK( DB_EVENT_RECOMMEND_USER_REQ, kDBPacket );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RECOMMEND_01, kPacket.m_nOK, 2 ); // 신규 혹은 복귀 유저가 아님.
        ERR_CASE( ERR_RECOMMEND_04, kPacket.m_nOK, 3 ); // 사용할수 없는 문자. 공백이 포함 되어 있음.
        ERR_CASE( ERR_RECOMMEND_05, kPacket.m_nOK, 4 ); // 추천 요청시 잘못된 요청 범위.
        ERR_CASE( ERR_RECOMMEND_02, kPacket.m_nOK, 5 ); // DB 처리 오류.
        ERR_CASE( ERR_RECOMMEND_03, kPacket.m_nOK, 6 ); // 추천을 할수 없는 상태임.
        ERR_CASE( ERR_RECOMMEND_06, kPacket.m_nOK, 7 ); // 추천 대상 유저가 존재하지 않음.
        ERR_CASE( ERR_RECOMMEND_12, kPacket.m_nOK, 8 ); // 더이상 이유저를 추천 할수 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"추천 요청중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_nState )
            << BUILD_LOG( kPacket_.m_strNick ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"추천 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nState )
        << BUILD_LOG( kPacket_.m_strNick )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_RECOMMEND_USER_ACK );
}

IMPL_ON_FUNC( EVENT_RECOMMEND_USER_ACK )
{
    std::pair< int, int > prRecomInfo; 

    NetError::SetLastNetError( kPacket_.m_nOK );
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_RECOMMEND_01, kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_RECOMMEND_04, kPacket_.m_nOK, 3 );
        ERR_CASE( ERR_RECOMMEND_05, kPacket_.m_nOK, 4 );
        ERR_CASE( ERR_RECOMMEND_02, kPacket_.m_nOK, 5 );
        ERR_CASE( ERR_RECOMMEND_03, kPacket_.m_nOK, 6 );
        ERR_CASE( ERR_RECOMMEND_06, kPacket_.m_nOK, 7 );
        ERR_CASE( ERR_RECOMMEND_12, kPacket_.m_nOK, 8 );
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"추천 응답중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( kPacket_.m_nRecomInfo.m_dwLoginUID )
            << BUILD_LOG( kPacket_.m_nRecomInfo.m_nState )
            << BUILD_LOG( kPacket_.m_nRecomInfo.m_strNick ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_RECOMMEND_USER_ACK );

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"접속시 추천 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nRecomInfo.m_dwLoginUID )
        << BUILD_LOG( kPacket_.m_nRecomInfo.m_nState )
        << BUILD_LOG( kPacket_.m_nRecomInfo.m_strNick ) << END_LOG;

    if( kPacket_.m_nRecomInfo.m_nState == KRecomHelper::RUO_RECOMMEND && kPacket_.m_nOK == 0 )
    {
        KPacketNameOK kState;
        kState.m_nOK = 0;
        kState.m_strName = m_strNickName;
        KSerBuffer kEventBuff;
        SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_RECOM_CHANGE_STATE_NOT, kState, kEventBuff );
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_nRecomInfo.m_dwLoginUID );
        if( spUser )
        {
            spUser->QueueingEvent( kEventBuff );
        }
        else
        {
            QUEUE_SERVER_RELAY_UID( EVENT_RECOM_CHANGE_STATE_NOT, kEventBuff, GetUID(), GetName(), kPacket_.m_nRecomInfo.m_dwLoginUID );
            //SiKCnConnector()->SendServerRelayPacket( KUserEvent::EVENT_RECOM_CHANGE_STATE_NOT, kEventBuff, GetUID(), GetName(), kPacket_.m_nRecomInfo.m_dwLoginUID, std::wstring(L""), true );
        }
    }

    // 추천 당시의 레벨을 캐싱 데이터에도 기록
    KCharacterInfo* pkCharacterInfo = GetCurrentCharacter();
    if( NULL != pkCharacterInfo ) {
        int nLevel = kPacket_.m_nRecomInfo.m_nLevel;
        prRecomInfo = std::make_pair( nLevel, nLevel );
    }
    SetNewRecomUser( prRecomInfo );
}

IMPL_ON_FUNC_NOPARAM( EVENT_RECOMMEND_FULL_INFO_REQ )
{
    KEVENT_RECOMMEND_FULL_INFO_ACK kPacket;
    kPacket.m_nOK = 99;

    // 이벤트 기간 인가?
    if( !SiKRecomHelper()->IsLimitEventTerm() )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_00, END_PROC );
    }

    QUEUEING_ID_TO_DB_CHECK( EVENT_RECOMMEND_FULL_INFO_REQ );
    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님. 
        ERR_CASE( ERR_RECOMMEND_02, kPacket.m_nOK, 2 ); // DB 처리 오류
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"추천 정보 요청중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_RECOMMEND_USER_ACK );
}

IMPL_ON_FUNC( EVENT_RECOMMEND_FULL_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    m_MyRecomInfo       = kPacket_.m_kMyInfo;
    m_vecRecommenders   = kPacket_.m_vecRecommenders;
    m_nRecomBonus       = kPacket_.m_nRecomBonusState;
    m_kInventory.AddItemList( kPacket_.m_vecNormalReward );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket_.m_nOK, 1 );
        ERR_CASE( ERR_RECOMMEND_02, kPacket_.m_nOK, 2 );
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"추천 정보 응답중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_RECOMMEND_FULL_INFO_ACK );
    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"추천 정보 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nRecomBonusState )
        << BUILD_LOG( kPacket_.m_vecNewAttendance.size() )
        << BUILD_LOG( kPacket_.m_vecNormalReward.size() )
        << BUILD_LOG( kPacket_.m_vecRecommenders.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_RECOMMENDER_ACCEPT_REQ, std::vector<DWORD> )
{
    std::vector<DWORD> vecTmp;
    KEVENT_RECOMMENDER_ACCEPT_ACK kPacket;
    kPacket.m_nOK = 99;
    kPacket.m_vecRecommenders = m_vecRecommenders;

    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간인가?
    if( !SiKRecomHelper()->IsEventTerm() )
        SET_ERR_GOTO( ERR_RECOMMEND_00, END_PROC );

    if( kPacket_.empty() )
        SET_ERR_GOTO( ERR_RECOMMEND_09, END_PROC );

    // 요청 수가 대기자 수보다 많은가?
    SiKRecomHelper()->GetRecommenders( m_vecRecommenders, KRecomHelper::RABS_WAIT, vecTmp );
    if( vecTmp.size() < kPacket_.size() )
        SET_ERR_GOTO( ERR_RECOMMEND_07, END_PROC );

    // 요청수가 대기자 수안에 포함 되는가?
    std::sort( vecTmp.begin(), vecTmp.end() );
    std::sort( kPacket_.begin(), kPacket_.end() );
    if( !std::includes( vecTmp.begin(), vecTmp.end(), kPacket_.begin(), kPacket_.end() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_07, END_PROC );

    // 승인수 + 요청수가 최대 숫자보다 많은가?
    SiKRecomHelper()->GetRecommenders( m_vecRecommenders, KRecomHelper::RABS_ACCEPT, vecTmp );
    if( SiKRecomHelper()->GetMaxAcceptNum() < kPacket_.size() + vecTmp.size() )
        SET_ERR_GOTO( ERR_RECOMMEND_08, END_PROC );

    QUEUEING_EVENT_TO_DB( EVENT_RECOMMENDER_ACCEPT_REQ, kPacket_ );

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RECOMMEND_07, kPacket.m_nOK, 2 ); // 대기 리스트에 없는 사람 승인 하려 했음.
        ERR_CASE( ERR_RECOMMEND_08, kPacket.m_nOK, 3 ); // 추천 승인 최대숫자 초과.
        ERR_CASE( ERR_RECOMMEND_09, kPacket.m_nOK, 4 ); // 추천 승인 패킷이 비어 있음.
        ERR_CASE( ERR_RECOMMEND_02, kPacket.m_nOK, 5 ); // DB 처리 오류.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"추천 승인 요청중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.size() ) << END_LOG;
    }

    if( kPacket.m_nOK != 0 )
    {
        kPacket.m_vecRecommenders = m_vecRecommenders;
        SEND_PACKET( EVENT_RECOMMEND_USER_ACK );
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"추천 승인 요청중 : " <<  NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_RECOMMENDER_ACCEPT_ACK, KRecommenderAcceptDBAck )
{
    NetError::SetLastNetError( kPacket_.m_kPacket.m_nOK );

    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    m_vecRecommenders = kPacket_.m_kPacket.m_vecRecommenders;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket_.m_kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RECOMMEND_07, kPacket_.m_kPacket.m_nOK, 2 ); // 대기 리스트에 없는 사람 승인 하려 했음.
        ERR_CASE( ERR_RECOMMEND_08, kPacket_.m_kPacket.m_nOK, 3 ); // 추천 승인 최대숫자 초과.
        ERR_CASE( ERR_RECOMMEND_09, kPacket_.m_kPacket.m_nOK, 4 ); // 추천 승인 패킷이 비어 있음.
        ERR_CASE( ERR_RECOMMEND_02, kPacket_.m_kPacket.m_nOK, 5 ); // DB 처리 오류.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_kPacket.m_nOK, -98 );
    default:
        kPacket_.m_kPacket.m_nOK = 99;
        START_LOG( cerr, L"추천 승인 요청중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_kPacket.m_vecRecommenders.size() ) << END_LOG;
    }

    kPacket_.m_kPacket.m_vecRecommenders = m_vecRecommenders;
    SEND_RECEIVED_PACKET( EVENT_RECOMMENDER_ACCEPT_ACK );

    _LOG_CONDITION( kPacket_.m_kPacket.m_nOK == 0, clog, cwarn, L"추천 승인 응답 : " <<  NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_kPacket.m_vecRecommenders.size() )
        << BUILD_LOG( kPacket_.m_vecReqUserUIDs.size() ) << END_LOG;

    if( kPacket_.m_kPacket.m_nOK == 0 )
    {
        KPacketNameOK kState;
        kState.m_nOK = 1;// 나를 승락 했음..
        kState.m_strName = m_strNickName;
        KSerBuffer kEventBuff;
        SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_RECOM_CHANGE_STATE_NOT, kState, kEventBuff );

        std::vector<DWORD>::iterator vit;
        for( vit = kPacket_.m_vecReqUserUIDs.begin() ; vit != kPacket_.m_vecReqUserUIDs.end() ; ++vit )
        {
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vit );
            if( spUser )
                spUser->QueueingEvent( kEventBuff );
            else
                QUEUE_SERVER_RELAY_UID( EVENT_RECOM_CHANGE_STATE_NOT, kEventBuff, GetUID(), GetName(), *vit );
            //SiKCnConnector()->SendServerRelayPacket( KUserEvent::EVENT_RECOM_CHANGE_STATE_NOT, kState, GetUID(), GetName(), *vit, std::wstring(L"") );
        }
    }
}

_IMPL_ON_FUNC( EVENT_ATTENDANCE_BONUS_REQ, int )
{
    KEVENT_ATTENDANCE_BONUS_ACK kPacket;
    kPacket.m_nOK           = 99;
    kPacket.m_nRecomBonus   = m_nRecomBonus;

    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인가?
    if( !SiKRecomHelper()->IsLimitEventTerm() )
        SET_ERR_GOTO( ERR_RECOMMEND_00, END_PROC );

    // 아이템을 얻을수 있는 날짜인가?
    if( !SiKRecomHelper()->CheckRecomBonus( kPacket_ ) )
        SET_ERR_GOTO( ERR_RECOMMEND_10, END_PROC );

    // 내각 획득한 보너스를 다시 요청하지 않는가?
    if( kPacket_ <= m_nRecomBonus )
        SET_ERR_GOTO( ERR_RECOMMEND_11, END_PROC );

    // 다음것을 요청 하고 있는가?
    if( kPacket_ != m_nRecomBonus + SiKRecomHelper()->GetRecomBonusGap() )
        SET_ERR_GOTO( ERR_RECOMMEND_10, END_PROC );

    QUEUEING_EVENT_TO_DB( EVENT_ATTENDANCE_BONUS_REQ, kPacket_ );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RECOMMEND_10, kPacket.m_nOK, 2 ); // 추천 보너스 범위 오류.
        ERR_CASE( ERR_RECOMMEND_11, kPacket.m_nOK, 3 ); // 이미 추천 보너스를 받았음.
        ERR_CASE( ERR_RECOMMEND_02, kPacket.m_nOK, 4 ); // DB 처리 오류.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"추천 승인 요청중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_ ) << END_LOG;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_ATTENDANCE_BONUS_ACK );

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"추천인 출석 보상 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( m_nRecomBonus ) << END_LOG;

}

IMPL_ON_FUNC( EVENT_ATTENDANCE_BONUS_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    m_nRecomBonus = kPacket_.m_nRecomBonus;
    m_kInventory.AddItemList( kPacket_.m_vecNormalReward );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RECOMMEND_10, kPacket_.m_nOK, 2 ); // 추천 보너스 범위 오류.
        ERR_CASE( ERR_RECOMMEND_11, kPacket_.m_nOK, 3 ); // 이미 추천 보너스를 받았음.
        ERR_CASE( ERR_RECOMMEND_02, kPacket_.m_nOK, 4 ); // DB 처리 오류.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"추천 승인 요청중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
    }

    kPacket_.m_nRecomBonus = m_nRecomBonus;
    SEND_RECEIVED_PACKET( EVENT_ATTENDANCE_BONUS_ACK );

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"추천인 출석 보상 응답중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nRecomBonus )
        << BUILD_LOG( kPacket_.m_vecNormalReward.size() )
        << BUILD_LOG( m_nRecomBonus ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_RECOM_DAILY_ATTENDANCE_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    if( kPacket_.m_bFirst )
        m_MyRecomInfo.m_nState = KRecomHelper::RBS_ATTEND;
    m_kInventory.AddItemList( kPacket_.m_vecNormalReward );
    SEND_RECEIVED_PACKET( EVENT_RECOM_DAILY_ATTENDANCE_ACK );

    START_LOG( clog, L"출석 도장 찍어서 보상 들어 추천  감.. Name : " << m_strName )
        << BUILD_LOG( kPacket_.m_bFirst )
        << BUILD_LOG( kPacket_.m_vecNormalReward.size() ) << END_LOG;

}

_IMPL_ON_FUNC( EVENT_RECOMMENDER_DENY_REQ, std::vector<DWORD> )
{
    std::vector<DWORD> vecTmp;
    KEVENT_RECOMMENDER_DENY_ACK kPacket;
    kPacket.m_nOK = 99;

    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간 인가?
    if( !SiKRecomHelper()->IsEventTerm() )
        SET_ERR_GOTO( ERR_RECOMMEND_00, END_PROC );

    // 빈 패킷을 보냈을까?
    if( kPacket_.empty() )
        SET_ERR_GOTO( ERR_RECOMMEND_14, END_PROC );

    // 나의 목록에 존재하는 애들인가?
    SiKRecomHelper()->GetRecommenders( m_vecRecommenders, KRecomHelper::RABS_WAIT, vecTmp );
    std::sort( vecTmp.begin(), vecTmp.end() );
    std::sort( kPacket_.begin(), kPacket_.end() );
    if( !std::includes( vecTmp.begin(), vecTmp.end(), kPacket_.begin(), kPacket_.end() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_13, END_PROC );

    QUEUEING_EVENT_TO_DB( EVENT_RECOMMENDER_DENY_REQ,kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RECOMMEND_14, kPacket.m_nOK, 2 ); // 거절 요청 패킷이 비어 있음
        ERR_CASE( ERR_RECOMMEND_13, kPacket.m_nOK, 3 ); // 거절 하려는 유저가 대기 리스트에 없음

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"추천 거절 요청중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.size() ) << END_LOG;
    }

    if( kPacket.m_nOK != 0 )
    {
        kPacket.m_vecRecommenders = m_vecRecommenders;
        SEND_PACKET( EVENT_RECOMMENDER_DENY_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_RECOMMENDER_DENY_ACK, KRecommenderAcceptDBAck )
{
    NetError::SetLastNetError( kPacket_.m_kPacket.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    m_vecRecommenders = kPacket_.m_kPacket.m_vecRecommenders;

END_PROC:
    kPacket_.m_kPacket.m_vecRecommenders = m_vecRecommenders;
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_kPacket.m_nOK, 0 );
        ERR_CASE( ERR_RECOMMEND_00, kPacket_.m_kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RECOMMEND_14, kPacket_.m_kPacket.m_nOK, 2 ); // 거절 요청 패킷이 비어 있음
        ERR_CASE( ERR_RECOMMEND_13, kPacket_.m_kPacket.m_nOK, 3 ); // 거절 하려는 유저가 대기 리스트에 없음

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_kPacket.m_nOK, -98 );
    default:
        kPacket_.m_kPacket.m_nOK = 99;
        START_LOG( cerr, L"추천자 거절 응답중 알수 없는 오류 , Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_kPacket.m_vecRecommenders.size() ) << END_LOG;
    }

    SEND_DATA( EVENT_RECOMMENDER_DENY_ACK, kPacket_.m_kPacket );

    _LOG_SUCCESS( kPacket_.m_kPacket.m_nOK == 0, L"추천자 거절 응답 : " <<  NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_kPacket.m_vecRecommenders.size() )
        << BUILD_LOG( kPacket_.m_vecReqUserUIDs.size() ) << END_LOG;

    if( kPacket_.m_kPacket.m_nOK == 0 )
    {
        KPacketNameOK kState;
        kState.m_nOK = 2;// 나를 거절 했음
        kState.m_strName = m_strNickName;
        KSerBuffer kEventBuff;
        SiKGCHelper()->MakeUserBuff( KUserEvent::EVENT_RECOM_CHANGE_STATE_NOT, kState, kEventBuff );

        std::vector<DWORD>::iterator vit;
        for( vit = kPacket_.m_vecReqUserUIDs.begin() ; vit != kPacket_.m_vecReqUserUIDs.end() ; ++vit )
        {
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *vit );
            if( spUser )
            {
                spUser->m_MyRecomInfo.m_nState = KRecomHelper::RBS_NONE;
                spUser->QueueingEvent( kEventBuff );
            }
            else
                QUEUE_SERVER_RELAY_UID( EVENT_RECOM_CHANGE_STATE_NOT, kEventBuff, GetUID(), GetName(), *vit );
            //SiKCnConnector()->SendServerRelayPacket( KUserEvent::EVENT_RECOM_CHANGE_STATE_NOT, kState, GetUID(), GetName(), *vit, std::wstring(L"") );
        }
    }
}

_IMPL_ON_FUNC( EVENT_RECOM_CHANGE_STATE_NOT, KPacketNameOK )
{
    ON_EVENT_RECOMMEND_FULL_INFO_REQ( KUserEvent::FT_NONE );
    SEND_RECEIVED_PACKET( EVENT_RECOM_CHANGE_STATE_NOT );
}

IMPL_ON_FUNC( EVENT_ONLINE_FRIENDS_NOT )
{
    m_vecOnlineFriends.swap( kPacket_ );

//     START_LOG( clog, L"온라인 친구 리스트 받음, Name: " << m_strName )
//         << BUILD_LOG( kPacket_.size() )
//         << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CHECK_GACHA_REEL_POINT_ACK, GAGHA_REEL_POINT )
{
    m_mapGachaReelPoint.swap( kPacket_ );
    START_LOG( clog, L"가챠 RP합 DB에서 받아옴. size : " << m_mapGachaReelPoint.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_SET_EXP_REQ, KAddExpInfo )
{
    KCharacterInfo* pkCharInfo = NULL;
    SET_ERROR( ERR_UNKNOWN );

    if( !SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) && !IsAdmin() )
    {
        // 사내에서만 쓸 수 있다.
        SET_ERR_GOTO( ERR_ADD_EXP_00, END_PROC );
    }

    pkCharInfo = GetCharacterInfo( kPacket_.m_cCharType );
    if( pkCharInfo == NULL )
    {
        // 해당 캐릭터가 없다.
        SET_ERR_GOTO( ERR_ADD_EXP_01, END_PROC );
    }

    // 요청한 경험치로 설정(최소 경험치2000 보장)
    pkCharInfo->m_biExp = std::max<__int64>( kPacket_.m_biExp, USER_MIN_EXP );
    pkCharInfo->m_dwLevel = (DWORD)SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );
    // SPX 계산
    SiKSkillManager()->GetSkillPointInfo( kPacket_.m_cCharType, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );

    kPacket_.m_biExp = pkCharInfo->m_biExp;
    kPacket_.m_kSkillInfo = pkCharInfo->m_kSkillInfo;

    SET_ERROR( NET_OK );

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_SET_EXP_00,   kPacket_.m_nOK, 1 );    // 테스트 서버가 아님.
        ERR_CASE( ERR_SET_EXP_01,   kPacket_.m_nOK, 2 );    // 해당 캐릭터가 없음
    default:
        START_LOG( cerr, L"경험치 설정시 알수 없는 오류.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << BUILD_LOG( kPacket_.m_biExp )
            << BUILD_LOG( kPacket_.m_kSkillInfo.m_nLvSPPoint )
            << BUILD_LOG( kPacket_.m_kSkillInfo.m_nMaxSPPoint ) << END_LOG;
    }

    LOG_CONDITION( kPacket_.m_nOK == 0, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_biExp )
        << BUILD_LOG( kPacket_.m_kSkillInfo.m_nLvSPPoint )
        << BUILD_LOG( kPacket_.m_kSkillInfo.m_nMaxSPPoint ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SET_EXP_ACK );
}

_IMPL_ON_FUNC( EVENT_CHANGE_VIRTUAL_CASH_COUPON_REQ, KComplexItem )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK kPacket;
    kPacket.m_kItem = kPacket_;
    kPacket.m_nVirtualCashPoint = 0;

    KVirtualCashCouponInfo kCouponInfo;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );

    if( !SiKGCHelper()->GetCouponInfo( kPacket_.m_dwID, kCouponInfo ) ) {
        SET_ERR_GOTO( ERR_VIRTUAL_CASH_COUPON_00, END_PROC );
    }

    pkItem = m_kInventory.FindDurationItem(kPacket_.m_dwID, kPacket_.m_dwUID);
    if (pkItem == NULL) {
        SET_ERR_GOTO(ERR_VIRTUAL_CASH_COUPON_02, END_PROC);
    }
    if (pkItem->m_nCount < kPacket_.m_iDuration)
    {
        SET_ERR_GOTO(ERR_VIRTUAL_CASH_COUPON_02, END_PROC);
    }

    kPacket.m_nVirtualCashPoint = kCouponInfo.m_nRewardCash * kPacket_.m_iDuration;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHANGE_VIRTUAL_CASH_COUPON_REQ, kPacket );

    SET_ERROR( NET_OK );
END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_00, kPacket.m_nOK, 1 );  // 해당 아이템은 쿠폰 아이템이 아님.
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_01, kPacket.m_nOK, 2 );  // 해당 쿠폰을 사용하면 최대 GP 보유량을 초과 합니다.
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_02, kPacket.m_nOK, 3 );  // 쿠폰 교환시 해당 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_03, kPacket.m_nOK, 4 );  // 쿠폰 아이템 제거시 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );

    default:
        START_LOG( cerr, L"가상 캐쉬 쿠폰 사용 요청중 알수 없는 오류 name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_dwID )
            << BUILD_LOG( kPacket_.m_dwUID )
            << BUILD_LOG( kPacket_.m_iDuration ) << END_LOG;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" 가상 캐쉬 쿠폰 사용 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_dwID )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_iDuration )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    pkItem = m_kInventory.FindDurationItem( kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );

    if ( pkItem != NULL ) {
        pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - kPacket_.m_kItem.m_iDuration );
        kPacket_.m_vecItem.push_back( *pkItem );
    }

    m_nVirtualCashPoint            += kPacket_.m_nVirtualCashPoint;
    m_nVirtualCashPoint             = std::max<int>( 0, m_nVirtualCashPoint );
    m_nVirtualCashPoint             = std::min<int>( SiKGSSimLayer()->m_nMaxVirtualCash, m_nVirtualCashPoint );
    kPacket_.m_nVirtualCashPoint    = m_nVirtualCashPoint;

END_PROC:

    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_00, kPacket_.m_nOK, 1 );  // 해당 아이템은 쿠폰 아이템이 아님.
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_01, kPacket_.m_nOK, 2 );  // 해당 쿠폰을 사용하면 최대 GP 보유량을 초과 합니다.
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_02, kPacket_.m_nOK, 3 );  // 쿠폰 교환시 해당 아이템이 인벤토리에 없음.
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_03, kPacket_.m_nOK, 4 );  // 쿠폰 아이템 제거시 실패.
        ERR_CASE( ERR_VIRTUAL_CASH_COUPON_04, kPacket_.m_nOK, NetError::ERR_VIRTUAL_CASH_COUPON_04 ); // 가상 캐쉬 쿠폰 교환 DB 에러. 
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );
    default:
        START_LOG( cerr, L"쿠폰 사용 요청중 알수 없는 오류 name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_kItem.m_dwID )
            << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;
    }

    SEND_RECEIVED_PACKET( EVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"쿠폰 교환 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nVirtualCashPoint )
        << BUILD_LOG( kPacket_.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kItem.m_dwUID ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_GET_GCCLUB_CONNECT_BONUS_ACK, std::vector<KItem> )
{
    JIF( PacketHackCheck( nFrom_ ) );
    JIF( SiKGCHelper()->GetGCClubExtensionEnable() == true );

    m_kInventory.AddItemList( kPacket_ );

    START_LOG( clog, L"GC클럽 접속보너스 아이템 지급됨." )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GET_GCCLUB_CONNECT_BONUS_ACK );
}

_IMPL_ON_FUNC( EVENT_GET_GCCLUB_ATTEND_BONUS_ACK, std::vector<KItem> )
{
    JIF( PacketHackCheck( nFrom_ ) );
    JIF( SiKGCHelper()->GetGCClubExtensionEnable() == true );

    m_kInventory.AddItemList( kPacket_ );

    START_LOG( clog, L"GC클럽 출석보너스 아이템 지급됨." )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GET_GCCLUB_ATTEND_BONUS_ACK );
}

_IMPL_ON_FUNC( EVENT_MISSION_PACK_REGIST_REQ, KSimpleItem )
{
    // FSM 체크
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_MISSION_PACK_REGIST_ACK kPacket;


    kPacket.m_kItem = kPacket_;

    START_LOG( clog, L"미션팩 등록, Name:" << m_strName )
        << BUILD_LOG( kPacket_.m_dwID )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOGc( kPacket_.m_cItemType ) << END_LOG;

    if( !SiKMissionPack()->GetMissionPackInfo( kPacket_.m_dwID, kPacket.m_setMissionList ) ) {
        // 아이템이 미션 패키지 ID 에 있는 것인지 확인.
        SET_ERR_GOTO( ERR_MISSION_PACK_00, END_PROC ); //미션 패키지 등록시 해당 아이템이 미션패키지 아이템 아님. 
    }

    // 인벤에도 있는지 확인
    if ( !m_kInventory.FindDurationItem( kPacket_.m_dwID, kPacket_.m_dwUID ) ) {
        SET_ERR_GOTO( ERR_MISSION_PACK_01, END_PROC );
    }
    
    // DB로 넘김..
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_MISSION_PACK_REGIST_REQ, kPacket );

    SET_ERROR( NET_OK );
    return;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_MISSION_PACK_00,  kPacket.m_nOK, 1 );    //미션 패키지 등록시 해당 아이템이 미션패키지 아이템 아님. )
        ERR_CASE( ERR_MISSION_PACK_01,  kPacket.m_nOK, 2 );    //미션 패키지 등록요청시 해당 아이템이 인벤토리에 없음. ) 
        ERR_CASE( ERR_MISSION_PACK_02,  kPacket.m_nOK, 3 );    //미션 패키지를 통한 미션 등록 실패 ) 이미 등록된 미션입니다.
        ERR_CASE( ERR_MISSION_PACK_03,  kPacket.m_nOK, 4 );    //미션 패키지를 통한 미션 등록 실패 ) 이미 완료한 미션입니다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 미션팩 등록 요청 중이다.

    default:
        break;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"미션팩 등록  : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket_.m_dwID )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOGc( kPacket_.m_cItemType )
        << BUILD_LOG( kPacket.m_setMissionList.size() ) << END_LOG;

    SEND_PACKET( EVENT_MISSION_PACK_REGIST_ACK );
}

IMPL_ON_FUNC( EVENT_MISSION_PACK_REGIST_ACK )
{

    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK ) ) {
        goto END_PROC;
    }

    if( !kPacket_.m_vecAutoMission.empty() )
        m_vecMissionSlot.insert( m_vecMissionSlot.end(), kPacket_.m_vecAutoMission.begin(), kPacket_.m_vecAutoMission.end() );

    pkItem = m_kInventory.FindDurationItem( kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID );
    if ( pkItem != NULL ) {
        pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount -1 );
    }

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_MISSION_PACK_00,  kPacket_.m_nOK, 1 );    //미션 패키지 등록시 해당 아이템이 미션패키지 아이템 아님. )
        ERR_CASE( ERR_MISSION_PACK_01,  kPacket_.m_nOK, 2 );    //미션 패키지 등록요청시 해당 아이템이 인벤토리에 없음. ) 
        ERR_CASE( ERR_MISSION_PACK_02,  kPacket_.m_nOK, 3 );    //미션 패키지를 통한 미션 등록 실패 ) 이미 등록된 미션입니다.
        ERR_CASE( ERR_MISSION_PACK_03,  kPacket_.m_nOK, 4 );    //미션 패키지를 통한 미션 등록 실패 ) 이미 완료한 미션입니다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 미션팩 등록 요청 중이다.
    }

    SEND_RECEIVED_PACKET( EVENT_MISSION_PACK_REGIST_ACK );

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"미션팩 등록 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecAutoMission.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_CLOSE_CONNECTION_NOT )
{
    // 타쓰레드 에서 Action 하는것을 막기 위해서.. ㅡㅡ
    SEND_RECEIVED_PACKET( EVENT_CLOSE_CONNECTION_NOT );

    if ( KEVENT_CLOSE_CONNECTION_NOT::CCT_DUPLICATION_CYOU == kPacket_.m_nCloseType ) {
        ReserveDestroy( DRW_DUPLICATE_CONNECTION_CYOU );
    }
    else {
        ReserveDestroy( DRW_DUPLICATE_CONNECTION );
    }

    _LOG_SUCCESS( kPacket_.m_nCloseType == KEVENT_CLOSE_CONNECTION_NOT::CCT_NORMAL, L"Close Connection Not.. LoginID : " << GetName() )
        << BUILD_LOG( kPacket_.m_nCloseType )
        << BUILD_LOG( kPacket_.m_nSocketErr )
        << BUILD_LOG( kPacket_.m_strMessage ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_RAINBOW_JACKPOT_ITEM_REQ )
{
    KEVENT_RAINBOW_JACKPOT_ITEM_ACK kPacket;
    KEVENT_RAINBOW_JACKPOT_ITEM_REQ kPacket_;
    kPacket.m_nOK = 99;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인가?
    if( !SiKRainbow()->IsRainbowEventTerm() )
        SET_ERR_GOTO( ERR_RAIN_BOW_00, END_PROC );

    if( !SiKRainbow()->IsJackPotTime() )
        SET_ERR_GOTO( ERR_RAIN_BOW_05, END_PROC );

    if( SiKRainbow()->IsReceivedUser( GetUID() ) )
        SET_ERR_GOTO( ERR_RAIN_BOW_02, END_PROC );

    kPacket_.m_nRainBowID       = SiKRainbow()->GetRainbowID();
    kPacket_.m_nDailyEventID    = SiKRainbow()->GetDailyEventID();
    kPacket_.m_nJackPotID       = SiKRainbow()->GetJackPotEventID();
    if( !SiKRainbow()->GetJacPotItemList( kPacket_.m_vecRewardList ) )
        SET_ERR_GOTO( ERR_RAIN_BOW_06, END_PROC );

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_RAINBOW_JACKPOT_ITEM_REQ, kPacket_ );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_RAIN_BOW_00,  kPacket.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RAIN_BOW_01,  kPacket.m_nOK, 2 ); // 유저 정보가 없음.
        ERR_CASE( ERR_RAIN_BOW_02,  kPacket.m_nOK, 3 ); // 잭팟 아이템을 이미 받아 갔음.
        ERR_CASE( ERR_RAIN_BOW_03,  kPacket.m_nOK, 4 ); // 등록된지 않은 이벤트.
        ERR_CASE( ERR_RAIN_BOW_04,  kPacket.m_nOK, 5 ); // 무지개 DB처리 오류.
        ERR_CASE( ERR_RAIN_BOW_05,  kPacket.m_nOK, 6 ); // 잭팟 기간이 아님.
        ERR_CASE( ERR_RAIN_BOW_06,  kPacket.m_nOK, 7 ); // 잭팟 보상 리스트가 존재하지 않음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"잭팟 아이템 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 3, L"잭팟 아이템 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName ) << END_LOG;

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_RAINBOW_JACKPOT_ITEM_ACK );

}

IMPL_ON_FUNC( EVENT_RAINBOW_JACKPOT_ITEM_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    NetError::SetLastNetError( kPacket_.m_nOK );

    if( IS_CORRECT( ERR_RAIN_BOW_02 ) || IS_CORRECT( NET_OK) )
        SiKRainbow()->InsertReceivedUser( GetUID() );

    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    // 아이템을 내 인벤에 적용 한다.
    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_RAIN_BOW_00,  kPacket_.m_nOK, 1 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_RAIN_BOW_01,  kPacket_.m_nOK, 2 ); // 유저 정보가 없음.
        ERR_CASE( ERR_RAIN_BOW_02,  kPacket_.m_nOK, 3 ); // 잭팟 아이템을 이미 받아 갔음.
        ERR_CASE( ERR_RAIN_BOW_03,  kPacket_.m_nOK, 4 ); // 등록되지 않은 이벤트.
        ERR_CASE( ERR_RAIN_BOW_04,  kPacket_.m_nOK, 5 ); // 무지개 DB처리 오류.
        ERR_CASE( ERR_RAIN_BOW_05,  kPacket_.m_nOK, 6 ); // 잭팟 기간이 아님.
        ERR_CASE( ERR_RAIN_BOW_06,  kPacket_.m_nOK, 7 ); // 잭팟 보상 리스트가 존재하지 않음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"잭팟 아이템 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 3, L"잭판 아이템 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_RAINBOW_JACKPOT_ITEM_ACK );
}

IMPL_ON_FUNC( EVENT_EFFECT_TEX_REPORT )
{
    if( kPacket_.empty() ) return;
    QUEUEING_EVENT_TO_DB( EVENT_EFFECT_TEX_REPORT, kPacket_ );
    START_LOG( clog, L"텍스쳐 리포팅 일어남.. Name : " << m_strName << L", Size : " << kPacket_.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_CO_OP_ITEM_BUY_REQ, KCoopEventItem )
{
    // 전제 조건은 이중접속은 완벽하게 차단이 된다는 조건임..
    KEVENT_CO_OP_ITEM_BUY_ACK kPacket;
    KItemPtr kItem;
    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간인가?
    if( !SiKCooperationEvent()->IsEventerTerm() )
        SET_ERR_GOTO( ERR_CO_OP_00, END_PROC );

    // 요청한 아이템이 이벤트 아이템인가?
    if( !SiKCooperationEvent()->GetItemInfo( kPacket_.m_nSlotID, kPacket_ ) )
        SET_ERR_GOTO( ERR_CO_OP_01, END_PROC );

    // 돈 아이템은 충분히 있는가?
    kItem = m_kInventory.FindDurationItemByItemID( SiKCooperationEvent()->GetMoneyItem() );
    if( !kItem || kItem->m_nCount < kPacket_.m_nPrice )
        SET_ERR_GOTO( ERR_CO_OP_02, END_PROC );

    // 그럼 큐잉 하자.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CO_OP_ITEM_BUY_REQ, kPacket_ );
    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_CO_OP_00,     kPacket.m_nOK, 1 );    // 제휴 이벤트 기간이 아님.
        ERR_CASE( ERR_CO_OP_01,     kPacket.m_nOK, 2 );    // 요청한 아이템은 이벤트 아이템이아님.
        ERR_CASE( ERR_CO_OP_02,     kPacket.m_nOK, 3 );    // 충분한 화폐아이템이 없음.
        ERR_CASE( ERR_CO_OP_03,     kPacket.m_nOK, 4 );    // 제휴 아이템 얻기 실패_쿼리 실패.
        ERR_CASE( ERR_CO_OP_04,     kPacket.m_nOK, 5 );    // 제휴 아이템 얻기 실패_없는 아이템.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"제휴 이벤트 아이템 교환 요청중 알수 없는 오류: " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"제휴 이벤트 아이템 교환 요청중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nSlotID ) << END_LOG;

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_CO_OP_ITEM_BUY_ACK );

}

IMPL_ON_FUNC( EVENT_CO_OP_ITEM_BUY_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );

    KCoopEventItem kEventItem;
    KItemPtr pkMoneyItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    // 아이템 넣기.
    LIF( m_kInventory.AddItemList( kPacket_.m_vecItem ) );

    // 화폐 아이템 가져 오기..
    pkMoneyItem = m_kInventory.FindItemByItemID( SiKCooperationEvent()->GetMoneyItem() );
    if( !pkMoneyItem )
    {
        START_LOG( cerr, L"제휴 아이템 구매결과 화폐가 없음.. Name : " << GetName() << L", 구매한 아이템 : " << kPacket_.m_kMoneyItem.m_ItemID )
            << BUILD_LOG( SiKCooperationEvent()->GetMoneyItem() ) << END_LOG;
        //기본값 셋팅
        kPacket_.m_kMoneyItem.m_ItemID          = SiKCooperationEvent()->GetMoneyItem();
        kPacket_.m_kMoneyItem.m_nInitCount   = 0;
        kPacket_.m_kMoneyItem.m_nCount       = 0;
        kPacket_.m_kMoneyItem.m_ItemUID         = 0;
        goto END_PROC;
    }

    // 구매 아이템 정보 가져 오기
    if( !SiKCooperationEvent()->GetItemInfo( (int)kPacket_.m_kMoneyItem.m_ItemID, kEventItem ) )
    {
        START_LOG( cerr, L"제휴 아이템 구매결과 대상 아이템이 없음. Name : " << GetName() << L", 구매한 아이템 : " << kPacket_.m_kMoneyItem.m_ItemID )
            << BUILD_LOG( SiKCooperationEvent()->GetMoneyItem() ) << END_LOG;
        kPacket_.m_kMoneyItem = *pkMoneyItem;
        goto END_PROC;
    }

    // 화폐 아이템 해당 수량 만큼 제거
    pkMoneyItem->m_nCount = std::max<int>(0, pkMoneyItem->m_nCount - kEventItem.m_nPrice );

    kPacket_.m_kMoneyItem = *pkMoneyItem;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 );    // 성공
        ERR_CASE( ERR_CO_OP_00,     kPacket_.m_nOK, 1 );    // 제휴 이벤트 기간이 아님.
        ERR_CASE( ERR_CO_OP_01,     kPacket_.m_nOK, 2 );    // 요청한 아이템은 이벤트 아이템이아님.
        ERR_CASE( ERR_CO_OP_02,     kPacket_.m_nOK, 3 );    // 충분한 화폐아이템이 없음.
        ERR_CASE( ERR_CO_OP_03,     kPacket_.m_nOK, 4 );    // 제휴 아이템 얻기 실패_쿼리 실패.
        ERR_CASE( ERR_CO_OP_04,     kPacket_.m_nOK, 5 );    // 제휴 아이템 얻기 실패_없는 아이템.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"제휴 이벤트 아이템 교환 요청중 알수 없는 오류: " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0, L"제휴 이벤트 아이템 교환 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_kMoneyItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kMoneyItem.m_nCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( kEventItem.m_nSlotID )
        << BUILD_LOG( kEventItem.m_dwItemID )
        << BUILD_LOG( kEventItem.m_nPrice ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CO_OP_ITEM_BUY_ACK );
}

_IMPL_ON_FUNC( EVENT_GET_USER_HACKING_COUNT_ACK, int )
{
    m_nHackingCount = kPacket_;
    START_LOG( clog, L"누적해킹횟수 : " << kPacket_ ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_ITEM_TRADE_REQ )
{
    KEVENT_ITEM_TRADE_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkItem;
    SET_ERROR( ERR_UNKNOWN );

    // 아이템 교환 사용 여부 체크 
    JIF( SiKItemTradeManager()->GetItemTradeEnable() == true );

    // 교환할 아이템 보유 여부 체크 
    // 해당 아이템을 보유 하고 있는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_nItemID );
    if( NULL == pkItem )
    {
        SET_ERR_GOTO( ERR_ITEM_TRADE_00, END_PROC );
    }
    // 교환할 아이템 개수는 충분한가?
    if( pkItem->m_nCount < kPacket_.m_nItemCount )
    {
        SET_ERR_GOTO( ERR_ITEM_TRADE_01, END_PROC );
    }
    // 보상 교환 아이템 가져오기. Type을 체크한다. 
    // 0 : 특정 갯수 교환
    // 1 : 전체 갯수 교환
    if( kPacket_.m_nType == 0 ) {
        if( !SiKItemTradeManager()->GetItemTradeInfo( kPacket_.m_nItemID, kPacket_.m_nItemCount, kPacket_.m_kRewardItem ) )
            SET_ERR_GOTO( ERR_ITEM_TRADE_02, END_PROC );
    }
    else if( kPacket_.m_nType == 1 ) {
        if( !SiKItemTradeManager()->GetItemAllTradeInfo( kPacket_.m_nItemID, kPacket_.m_kRewardItem ) ) {
            SET_ERR_GOTO( ERR_ITEM_TRADE_02, END_PROC );
        }
        else {
            // 현재 전체 아이템의 갯수를 보상아이템 갯수에 넣어준다.
            if ( kPacket_.m_kRewardItem.m_nDuration > 0 )
                kPacket_.m_kRewardItem.m_nDuration *= pkItem->m_nCount;

            if ( kPacket_.m_kRewardItem.m_nPeriod > 0 )
                kPacket_.m_kRewardItem.m_nPeriod *= pkItem->m_nCount;

            kPacket_.m_nItemCount = pkItem->m_nCount;
        }
    }
    else {
        SET_ERR_GOTO( ERR_ITEM_TRADE_04, END_PROC );
    }
    // DB에 아이템을 넣고 해당 교환 로그를 남기자!
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ITEM_TRADE_REQ, kPacket_ );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ITEM_TRADE_00,  kPacket.m_nOK, 1 ); // 해당 아이템 가져오기 실패.
        ERR_CASE( ERR_ITEM_TRADE_01,  kPacket.m_nOK, 2 ); // 해당 아이템 갯수가 부족하다.
        ERR_CASE( ERR_ITEM_TRADE_02,  kPacket.m_nOK, 3 ); // 서버 보상 교환 아이템 가져오기 실패.
        ERR_CASE( ERR_ITEM_TRADE_03,  kPacket.m_nOK, 4 ); // 해당 아이템 DB처리 오류
        ERR_CASE( ERR_ITEM_TRADE_04,  kPacket.m_nOK, 5 ); // 교환 아이템 타입 설정 오류
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"아이템 교환 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nItemID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_nDuration )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_nPeriod ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        SEND_PACKET( EVENT_ITEM_TRADE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ITEM_TRADE_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    KItemPtr pkItem;

    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    int nCount = kPacket_.m_nItemCount; // 교환후 감소될 아이템 갯수
    // 해당 아이템을 보유 하고 있는가?
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_nItemID );
    if ( NULL != pkItem ) {
        //개수 차감.
        pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - kPacket_.m_nItemCount );
        kPacket_.m_kUseItem = *pkItem;
    }
    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,             kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ITEM_TRADE_00,  kPacket_.m_nOK, 1 ); // 해당 아이템 가져오기 실패.
        ERR_CASE( ERR_ITEM_TRADE_01,  kPacket_.m_nOK, 2 ); // 해당 아이템 갯수가 부족하다.
        ERR_CASE( ERR_ITEM_TRADE_02,  kPacket_.m_nOK, 3 ); // 서버 보상 교환 아이템 가져오기 실패.
        ERR_CASE( ERR_ITEM_TRADE_03,  kPacket_.m_nOK, 4 ); // 해당 아이템 DB처리 오류
        ERR_CASE( ERR_ITEM_TRADE_04,  kPacket_.m_nOK, 5 ); // 교환 아이템 타입 설정 오류.
    default:
        START_LOG( cerr, L"아이템 교환에 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_CONDITION( kPacket_.m_nOK == 0 || kPacket_.m_nOK == -98, clog, cerr )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nItemID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( kPacket_.m_kUseItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kUseItem.m_nCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ITEM_TRADE_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_MISSION_DATE_CHANGE_REQ )
{
    // 하루가 지나고 새로운 미션을 받는 부분에 대한 처리 진행.
    // 일단 현재의 이벤트 정보와 지워져야 할 MissionID들을 받아온다.
    KEVENT_MISSION_DATE_CHANGE_ACK kPacket;
    kPacket.m_nOK = 99;
    SET_ERROR( ERR_UNKNOWN );

    KMissionCurrentEvent kData;
    KEVENT_MISSION_ITEM_NOT kPacket_;
    std::set<int>::iterator sit;

    if( IsExpAccount() )
    {
        SET_ERR_GOTO( ERR_MISSION_EVENT_02, END_PROC );
    }

    SiKMissionEvent()->GetEventData( kData );

    // DB로 넘겨야 하니깐 해당일의 정보를 담아두자~!
    kPacket_.m_CurrentData = kData;

    // 일단 아이템 검색을 진행한다.
    for( sit = kData.m_setItemList.begin(); sit != kData.m_setItemList.end(); ++sit )
    {
        KItemPtr pkItem = m_kInventory.FindDurationItemByItemID( *sit );
        if( pkItem )
        {
            // 가지고 있는 아이템의 갯수를 0으로 만들어 준다.
            pkItem->m_nCount = 0;
            // 해당 아이템을 클라이언트에게 전송할 패킷에 세팅한다.
            KItem kdItemInfo;
            kdItemInfo = *pkItem;
            kPacket_.m_vecItemDel.push_back( kdItemInfo );
        }

        // 해당 미션이 등록되어 있는지 확인한다.
        // 해당 ID의 경우 메인 미션 ID이므로 메인 미션 ID로 해서 
        // 서브 미션 ID도 함께 지워주는 작업을 진행한다.

        // 지우려고 하는 미션이 슬롯에 존재하는가?
        if( IsRegistedMission( *sit ) )
        {
            KEVENT_REMOVE_MISSION kPacket1;
            kPacket1.m_dwMissionID = *sit;

            // DB큐잉
            QUEUEING_EVENT_TO_DB_CHECK( EVENT_REMOVE_MISSION_REQ, kPacket1 );

            if( !EraseMissionDataFromSlot( *sit ) )
            {
                START_LOG( cerr, L"실제 미션 제거 시점에서 해당 미션을 찾을수 없음.. Name : " << m_strName )
                    << BUILD_LOG( *sit ) << END_LOG;
            }
            else {
                kPacket_.m_setMissionList.insert( *sit );
            }
        }
    }

    // 해당 미션 아이템 넣어주기
    // 위에서 전체 아이템 리스트에 대해서 삭제 진행을 했기 때문에..,
    // DB에 해당 Item을 넣어주는 작업에 대해서 진행하자 ~~!
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_MISSION_ITEM_NOT, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_MISSION_EVENT_01,  kPacket.m_nOK, 2 ); // DB에 지급한 아이템 로그 기록 실패.
        ERR_CASE( ERR_MISSION_EVENT_02,  kPacket.m_nOK, 3 ); // 체험 계정이라서 아이템 지급 안함.
    default:
        START_LOG( cerr, L"미션 아이템 지급 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = 99;
    }

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_MISSION_DATE_CHANGE_ACK );

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"미션 아이템 지급 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

}

IMPL_ON_FUNC( EVENT_MISSION_DATE_CHANGE_ACK )
{
    // DB에 다녀오고 난 후에 대한 처리 진행.
    // 이미 모든 처리가 다 되었으니깐.., 따로 특별한 처리는 에러처리 정도..,?
    // 체험 계정인지를 체크.
    NetError::SetLastNetError( kPacket_.m_nOK );

    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    // 획득한 아이템이 수량 아이템을 넣어두자..,
    m_kInventory.AddItemList( kPacket_.m_vecItemAdd );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_MISSION_EVENT_01,  kPacket_.m_nOK, 2 ); // DB에 지급한 아이템 로그 기록 실패.
        ERR_CASE( ERR_MISSION_EVENT_02,  kPacket_.m_nOK, 3 ); // 체험 계정이라서 아이템 지급 안함.
    default:
        START_LOG( cerr, L"미션 아이템 지급중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"미션 이벤트 아이템 지급 로그 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItemDel.size() )
        << BUILD_LOG( kPacket_.m_vecItemAdd.size() )
        << BUILD_LOG( kPacket_.m_setMissionList.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_MISSION_DATE_CHANGE_ACK );
}

_IMPL_ON_FUNC( EVENT_MY_SUBSCRIPTION_INFO_ACK, MAP_INT_DWORD )
{
    m_mapMySubscriptionGiftCount = kPacket_;
    START_LOG( clog, L"응모 이벤트별 획득한 경품 개수정보 받아옴. size : " << kPacket_.size() ) << END_LOG;
    if( SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
    {
        for( MAP_INT_DWORD::const_iterator cmit = kPacket_.begin() ; cmit != kPacket_.end() ; ++cmit )
        {
            START_LOG( cwarn, L"EventUID : " << cmit->first << L", My Subscript Coust : " << cmit->second  ) << END_LOG;
        }
        //std::Warn << dbg::endl;
    }

    // 응모이벤트 전체정보 가져오기
    std::map< int, KSubscription > mapEventInfo;
    SiKSubscriptionManager()->GetEventInfo( mapEventInfo );

    // 클라이언트에 응모이벤트 정보 전달
    SendSubscriptionInfoNotice( mapEventInfo );
}

_IMPL_ON_FUNC( EVENT_SUBSCRIPTION_INFO_UPDATE_NOT, KECN_SUBSCRIPTION_INFO_UPDATE_NOT )
{
    // 클라이언트에 응모이벤트 정보 전달
    SendSubscriptionInfoNotice( kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EVENT_CURRENT_SUBSCRIPTION_GIFT_NUM_REQ )
{
    KEVENT_CURRENT_SUBSCRIPTION_GIFT_NUM_ACK kPacket;
    MAP_INT_INT mapCurrentGiftNum; // 현재 남은 경품의 수(이벤트별)
    SiKSubscriptionManager()->GetCurrentGiftNum( mapCurrentGiftNum );

    // 응모이벤트별 나의 응모 횟수 정보를 합쳐준다.
    for( MAP_INT_INT::iterator mit = mapCurrentGiftNum.begin() ; mit != mapCurrentGiftNum.end() ; ++mit )
    {
        kPacket[mit->first] = std::make_pair( mit->second, m_mapMySubscriptionGiftCount[mit->first] );
    }

    START_LOG( clog, L"Subscription size : " << kPacket.size() ) << END_LOG;
    SEND_PACKET( EVENT_CURRENT_SUBSCRIPTION_GIFT_NUM_ACK );
}

IMPL_ON_FUNC( EVENT_SUBSCRIPT_REQ )
{
    // 광장 응모하기
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));

    KItemPtr pkTicketItem;
    int nReqGiftCount = 0; // 요청 경품 수량
    KEVENT_SUBSCRIPT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nEventUID = -99;
    kPacket.m_nMySubscriptionGiftCout = 0;

    MAP_INT_DWORD::const_iterator cmit = m_mapMySubscriptionGiftCount.find( kPacket_.m_nEventUID );
    if( cmit != m_mapMySubscriptionGiftCount.end() )
    {
        kPacket.m_nMySubscriptionGiftCout = cmit->second;  // 현재 응모횟수
    }

    SET_ERROR( ERR_UNKNOWN );

    // 응모이벤트 정보 받아오기
    if( SiKSubscriptionManager()->GetCurrentEvent( kPacket_.m_nEventUID, kPacket_.m_kEventInfo ) == false )
    {
        START_LOG( cwarn, L"응모 이벤트 정보가 낡았음. EventUID : " << kPacket_.m_nEventUID ) << END_LOG;
        // 응모이벤트 정보 새로 보내주자.
        KECN_SUBSCRIPTION_INFO_UPDATE_NOT mapEventInfo;
        SiKSubscriptionManager()->GetEventInfo( mapEventInfo );
        SendSubscriptionInfoNotice( mapEventInfo );

        SET_ERR_GOTO( ERR_SUBSCRIPTION_00, END_PROC );
    }

    // 1회 응모시 필요한 응모권 개수를 만족하는가?
    if( kPacket_.m_kEventInfo.m_nNeedTicketNum > kPacket_.m_nTicketCount )
    {
        SET_ERR_GOTO( ERR_SUBSCRIPTION_01, END_PROC );
    }

    // 응모권 아이템 인벤에서 찾기(수량성)
    pkTicketItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_kEventInfo.m_dwTicketItemID );
    if( NULL == pkTicketItem )
    {
        SET_ERR_GOTO( ERR_SUBSCRIPTION_02, END_PROC );
    }

    // 티켓 아이템 정보 설정.
    kPacket.m_kTicketItem = *pkTicketItem;
    kPacket_.m_kTicketItem = *pkTicketItem;

    // 충분한 수량의 응모권을 보유 하고 있는가?
    if( pkTicketItem->m_nCount < kPacket_.m_nTicketCount )
    {
        SET_ERR_GOTO( ERR_SUBSCRIPTION_02, END_PROC );
    }

    // 요청 경품 수량 = 응모권 응모개수 / 1회 응모시 응모권 필요수
    nReqGiftCount = kPacket_.m_nTicketCount / kPacket_.m_kEventInfo.m_nNeedTicketNum;

    // 남은 경품수량보다 많이 요청했는가?
    if( kPacket_.m_kEventInfo.m_nCurrentGiftNum < nReqGiftCount )
    {
        SET_ERR_GOTO( ERR_SUBSCRIPTION_03, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_SUBSCRIPT_REQ, kPacket_ );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_SUBSCRIPTION_00,  kPacket.m_nOK, 1 ); // 해당 이벤트가 없음
        ERR_CASE( ERR_SUBSCRIPTION_01,  kPacket.m_nOK, 2 ); // 1회 응모시 필요한 응모권 개수보다 적음
        ERR_CASE( ERR_SUBSCRIPTION_02,  kPacket.m_nOK, 3 ); // 요청한 응모권 수량보다 인벤 수량이 적다.
        ERR_CASE( ERR_SUBSCRIPTION_03,  kPacket.m_nOK, 4 ); // 남은 경품수량보다 많이 요청했다.
        ERR_CASE( ERR_SUBSCRIPTION_04,  kPacket.m_nOK, 5 ); // 응모하기 DB작업 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"응모이벤트 응모 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_nEventUID )
        << BUILD_LOG( kPacket_.m_nTicketCount ) << END_LOG;

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_SUBSCRIPT_ACK );
}

IMPL_ON_FUNC( EVENT_SUBSCRIPT_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    KItemPtr pkTicketItem;

    // 현재 응모횟수 정보 갱신
    m_mapMySubscriptionGiftCount[ kPacket_.m_nEventUID ] += kPacket_.m_nMySubscriptionGiftCout; // 이전 정보와 합산
    kPacket_.m_nMySubscriptionGiftCout = m_mapMySubscriptionGiftCount[ kPacket_.m_nEventUID ];

    if( kPacket_.m_nOK != 0 )
    {
        // 응모하기 DB작업 실패.
        SET_ERR_GOTO( ERR_SUBSCRIPTION_04, END_PROC );
    }

    // 응모권 아이템 정보 갱신
    pkTicketItem = m_kInventory.FindDurationItemByItemUID( kPacket_.m_kTicketItem.m_ItemUID );
    if( NULL != pkTicketItem )
    {
        pkTicketItem->m_nInitCount = kPacket_.m_kTicketItem.m_nCount;
        pkTicketItem->m_nCount = kPacket_.m_kTicketItem.m_nCount;
    }

    // 경품 아이템 정보 갱신
    m_kInventory.AddItemList( kPacket_.m_vecItem );

    // 남은 경품 개수 정보갱신
    if( SiKSubscriptionManager()->SetCurrentGiftNum( kPacket_.m_nEventUID, kPacket_.m_nCurrentGiftNum ) == false )
    {
        START_LOG( cwarn, L"응모 이벤트 정보가 낡았음. EventUID : " << kPacket_.m_nEventUID ) << END_LOG;
        // 응모이벤트 정보 새로 보내주자.
        KECN_SUBSCRIPTION_INFO_UPDATE_NOT mapEventInfo;
        SiKSubscriptionManager()->GetEventInfo( mapEventInfo );
        SendSubscriptionInfoNotice( mapEventInfo );
    }
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_SUBSCRIPTION_00,  kPacket_.m_nOK, 1 ); // 해당 이벤트가 없음
        ERR_CASE( ERR_SUBSCRIPTION_01,  kPacket_.m_nOK, 2 ); // 1회 응모시 필요한 응모권 개수보다 적음
        ERR_CASE( ERR_SUBSCRIPTION_02,  kPacket_.m_nOK, 3 ); // 요청한 응모권 수량보다 인벤 수량이 적다.
        ERR_CASE( ERR_SUBSCRIPTION_03,  kPacket_.m_nOK, 4 ); // 남은 경품수량보다 많이 요청했다.
        ERR_CASE( ERR_SUBSCRIPTION_04,  kPacket_.m_nOK, 5 ); // 응모하기 DB작업 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"응모이벤트 응모 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == -98 )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nEventUID )
        << BUILD_LOG( kPacket_.m_nCurrentGiftNum )
        << BUILD_LOG( kPacket_.m_nMySubscriptionGiftCout )
        << BUILD_LOG( kPacket_.m_kTicketItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kTicketItem.m_ItemUID )
        << BUILD_LOG( kPacket_.m_kTicketItem.m_nCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_SUBSCRIPT_ACK );
}

_IMPL_ON_FUNC( EVENT_NPC_GIFTS_REQ, int )
{
    // 광장 NPC클릭시 아이템받기 요청
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));

    KEVENT_NPC_GIFTS_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nEventID = -99;
    kPacket.m_vecItem.clear();

    KEVENT_NPC_GIFTS_REQ kDBPacket;
    kDBPacket.m_nEventID = kPacket_;
    kDBPacket.m_vecReqItem.clear();

    SET_ERROR( ERR_UNKNOWN );

    // 이벤트Item 정보 받아오기
    if( SiKNpcGift()->GetEventItemList( kPacket_, kDBPacket.m_vecReqItem ) == false )
    {
        SET_ERR_GOTO( ERR_NPC_GIFT_00, END_PROC );
    }

    // 보상 아이템이 있는지?
    if( kDBPacket.m_vecReqItem.empty() )
    {
        SET_ERR_GOTO( ERR_NPC_GIFT_01, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_NPC_GIFTS_REQ, kDBPacket );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_NPC_GIFT_00,  kPacket.m_nOK, 1 ); // 해당 이벤트가 없음
        ERR_CASE( ERR_NPC_GIFT_01,  kPacket.m_nOK, 2 ); // 보상 아이템이 비어있음
        ERR_CASE( ERR_NPC_GIFT_02,  kPacket.m_nOK, 3 ); // 1일1회 보상 아이템이 이미 받았음
        ERR_CASE( ERR_NPC_GIFT_03,  kPacket.m_nOK, 3 ); // 아이템넣기 DB작업 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"NPC 아이템받기 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L" : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kDBPacket.m_nEventID )
        << BUILD_LOG( kDBPacket.m_vecReqItem.size() ) << END_LOG;

    if( kPacket.m_nOK != 0 )
        SEND_PACKET( EVENT_NPC_GIFTS_ACK );
}

IMPL_ON_FUNC( EVENT_NPC_GIFTS_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    // 광장 NPC클릭시 아이템받기 결과
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        goto END_PROC;
    }

    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_NPC_GIFT_00,  kPacket_.m_nOK, 1 ); // 해당 이벤트가 없음
        ERR_CASE( ERR_NPC_GIFT_01,  kPacket_.m_nOK, 2 ); // 보상 아이템이 비어있음
        ERR_CASE( ERR_NPC_GIFT_02,  kPacket_.m_nOK, 3 ); // 1일1회 보상 아이템이 이미 받았음
        ERR_CASE( ERR_NPC_GIFT_03,  kPacket_.m_nOK, 3 ); // 아이템넣기 DB작업 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 ); // 처리중.
    default:
        START_LOG( cerr, L"NPC 아이템받기 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_CONDITION( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 3, clog, cerr )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItem.size() )
        << BUILD_LOG( kPacket_.m_nEventID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_NPC_GIFTS_ACK );
}

_IMPL_ON_FUNC( EVENT_CHARACTER_STAT_INFO_REQ, PAIR_CHAR )
{
    KSimpleCharacterStat kPacket;
    kPacket.Init();

    // 현재 캐릭터 스텟 정보 뽑기.
    JIF( SiKPreHackingCheckManager()->GetCharacterStat( std::make_pair( kPacket_.first, kPacket_.second ), kPacket ) == true );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOGc( kPacket_.first )
        << BUILD_LOGc( kPacket_.second )
        << BUILD_LOG( kPacket.m_fIncHP )
        << BUILD_LOG( kPacket.m_fDamagefactor )
        << BUILD_LOG( kPacket.m_fWalkSpeed )
        << BUILD_LOG( kPacket.m_fRunSpeed ) << END_LOG;

    SEND_PACKET( EVENT_CHARACTER_STAT_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_CLIENT_HACKING_USER_NOT, int )
{
    int nCharType = (int)GetCurrentCharType();
    int nPromotion = (int)GetCurrentPromotion();

    START_LOG( cerr, L"클라이언트 해킹감지됨. type : " << kPacket_ << L",name :" << m_strName << L",CharType : " << nCharType << L",Promotion : " << nPromotion ) << END_LOG;

    // 해킹 체크여부 체크
    if( SiKPreHackingCheckManager()->CheckOption( kPacket_, KPreHackingCheckManager::OPT_CHECK_ENABLE ) == false ) return;
    // DB 기록여부 체크
    if( SiKPreHackingCheckManager()->CheckOption( kPacket_, KPreHackingCheckManager::OPT_WRITE_DB ) == false ) return;

    KDB_EVENT_DUNGEON_HACKING_USER_NOT kNot;
    kNot.m_nHackingType = kPacket_;
    kNot.m_nGameModeID  = -1;
    kNot.m_nDifficult   = -1;
    kNot.m_nCharType    = nCharType;
    kNot.m_nPromotion   = nPromotion;

    QUEUEING_EVENT_TO_DB( DB_EVENT_DUNGEON_HACKING_USER_NOT, kNot );
}

_IMPL_ON_FUNC( EVENT_UPDATEPLAN_BOARD_GAME_USER_INFO_NOT, DWORD )
{
    START_LOG( clog, L"주사위 이벤트 자동오픈시 접속중인 유저 데이터 요청: " << m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    QUEUEING_ID_TO_DB( EVENT_GET_USER_BOARD_GAME_INFO );
}

IMPL_ON_FUNC( EVENT_GET_USER_BOARD_GAME_INFO )
{
    // 유저의 보드게임 정보 초기화 세팅하기.
    // 특정 시간이 지났거나 할 경우에..,
    InitUserBoardInfo( kPacket_.m_nOK, kPacket_.m_kUserBoardInfo );

    // 주사위 초기화 시간을 가지고 있자.
    CTime tmDiceTodayTime( kPacket_.m_tmTodayTime);
    CTimeSpan ts(1,0,0,0);
    m_tmInitDiceTime = tmDiceTodayTime+ts;  // 초기화 진행 시간을 저장해 두자.

    // 클라이언트에게는 
    KEVENT_BOARD_GAME_INFO_NOT kPacket;
    // 말판 정보 읽어오기.
    SiKDicePlay()->GetMapInfo( kPacket.m_mapBoardInfo );
    SiKDicePlay()->GetRewardTotalList( kPacket.m_mapBoardReward );
    kPacket.m_dwFreeDiceTick = SiKDicePlay()->GetFreeTickGap();

    SEND_PACKET( EVENT_BOARD_GAME_INFO_NOT );

    START_LOG( clog, L" 유저에게 말판정보 전송 Name : " << m_strName )
        << BUILD_LOG( kPacket.m_mapBoardInfo.size() )
        << BUILD_LOG( kPacket.m_mapBoardReward.size() )
        << BUILD_LOGtm( CTime( kPacket_.m_tmGiveDiceLastTime ) )
        << BUILD_LOGtm( CTime( m_tmInitDiceTime ) )
        << BUILD_LOG( kPacket.m_dwFreeDiceTick ) << END_LOG;
}

// C->S 보드게임 말판 기존 정보 요청.
IMPL_ON_FUNC_NOPARAM( EVENT_BOARD_GAME_INFO_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_BOARD_GAME_INFO_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 일단 이벤트 기간이 아닐때 클릭하더라도 정보를 보내주자..,
    // 클라이언트 Display 문제가 있어서..,
    KUserBoardInfo kUserBoardInfo;
    kUserBoardInfo.Clear();
    GetUserBoardInfo( kUserBoardInfo );
    CheckUserBoardInfo( kUserBoardInfo ); // 주사위 이벤트 유저의 정보 계산.
    kPacket.m_kMyBoardInfo = kUserBoardInfo;

    // 주사위 이벤트 기간인지 체크.
    if( !SiKDicePlay()->IsRun() )
    {
        SET_ERR_GOTO( ERR_DICE_PLAY_02, END_PROC );
    }

    // 클라이언트에게 전달할 랭킹 정보 가져오기.
    SiKDicePlay()->GetDicePlayRank( kPacket.m_vecDicePlayRank );
    // 클라이언트에게 전달할 전체 보상 아이템 리스트 가져오기.
    SiKDicePlay()->GetRewardTotalList( kPacket.m_mapBoardReward );

    if( kUserBoardInfo.m_dwGiveCount == 0 ) SET_ERR_GOTO( ERR_DICE_PLAY_06, END_PROC );
    else if( kUserBoardInfo.m_dwDiceCount == 0 ) SET_ERR_GOTO( ERR_DICE_PLAY_03, END_PROC );
    else SET_ERR_GOTO( NET_OK, END_PROC );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공.
        ERR_CASE( ERR_DICE_PLAY_02,     kPacket.m_nOK, 1 );     // 이벤트 기간이 아님.
        ERR_CASE( ERR_DICE_PLAY_03,     kPacket.m_nOK, 2 );     // 주사위 돌릴 수 있는 기회 없음.
        ERR_CASE( ERR_DICE_PLAY_06,     kPacket.m_nOK, 3 );     // 무료 주사위 기회 지급 금일 완료.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"보드게임 정보 얻어오는 중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_BOARD_GAME_INFO_ACK );

    _LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, clog, cwarn, L"접속시 말판 정보 요청 " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kMyBoardInfo.m_dwRemainCount )
        << BUILD_LOG( kPacket.m_kMyBoardInfo.m_dwGiveCount )
        << BUILD_LOG( kPacket.m_kMyBoardInfo.m_dwRemainTime )
        << BUILD_LOG( kPacket.m_kMyBoardInfo.m_dwDiceCount ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_DICE_PLAY_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_DB_DICE_PLAY_REQ kDBPacket;
    kDBPacket.m_nOK = -99;
    KEVENT_DICE_PLAY_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_vecMissionList.clear();
    kPacket.m_vecItem.clear();
    std::vector<DWORD> vecMissionList;
    std::vector<DWORD>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );

    // 일단 이벤트 기간이 아닐때 클릭하더라도 정보를 보내주자..,
    // 클라이언트 Display 문제가 있어서..,
    KUserBoardInfo kUserBoardInfo;
    kUserBoardInfo.Clear();
    GetUserBoardInfo( kUserBoardInfo );
    CheckUserBoardInfo( kUserBoardInfo ); // 주사위 이벤트 유저의 정보 계산.
    kPacket.m_kMyBoardInfo = kUserBoardInfo;

    // 주사위 이벤트 기간인지 체크.
    if( !SiKDicePlay()->IsRun() )
    {
        SET_ERR_GOTO( ERR_DICE_PLAY_02, END_PROC );
    }

    if( kUserBoardInfo.m_dwDiceCount < 1 )
    {
        SET_ERR_GOTO( ERR_DICE_PLAY_03, END_PROC );
    }

    if( kUserBoardInfo.m_dwRemainCount < 1 )
    {
        SET_ERR_GOTO( ERR_DICE_PLAY_04, END_PROC );
    }
    DWORD dwDiceNum1 = 0;
    DWORD dwDiceNum2 = 0;
    SiKDicePlay()->GetDicePlayNum( dwDiceNum1, dwDiceNum2 ); // 주사위를 굴리자.
    kDBPacket.m_FirstDiceNum = dwDiceNum1;
    kDBPacket.m_SecondDiceNum = dwDiceNum2;
    // 주사위 다른 숫자가 나왔을 경우에만 감소시키자.
    // 같은 숫자가 나오면 주사위 한번 더 굴릴 기회를 준다.
    if( dwDiceNum1 != dwDiceNum2 )
    {
        kUserBoardInfo.m_dwRemainCount -= 1; // 주사위를 굴렸으면 주사위 굴릴 횟수를 감소시키자.
        kUserBoardInfo.m_dwDiceCount -= 1;
    }
    // 현재 나의주사위 칸 위치에다가 주사위 굴린 수만큼 더하자.
    kUserBoardInfo.m_dwCurrentPosition += ( dwDiceNum1 + dwDiceNum2 );
    // 주사위를 더했으면 한바퀴에 대한 처리를 먼저 해야 한다.
    // 왜냐면 합이 최대 칸의 수치보다 넘으면 해당 속성이 없으니깐..,
    DWORD dwMapCount = SiKDicePlay()->GetTotalMapCount();  // 맵 전체 칸수 얻어오고.
    if( kUserBoardInfo.m_dwCurrentPosition >= dwMapCount )
    {
        kUserBoardInfo.m_dwTotalCount += 1;         // 나의 전체 바퀴수 1 더해주고.
        kUserBoardInfo.m_dwCurrentPosition -= dwMapCount;   // 현재 나의 바퀴 계산해주고.
        DWORD dwMaxGoal = SiKDicePlay()->GetMaxGoal();  // 최대 목표 바퀴수 가져오고.
        DWORD dwDefault = 0;
        // 나의 바퀴수가 목표바퀴에 도달했는지 확인.
        if( kUserBoardInfo.m_dwTotalCount > dwMaxGoal )
        {
            dwDefault = dwMaxGoal+1;
            SiKDicePlay()->GetGoalReward( dwDefault, kDBPacket.m_vecGoalReward );
        }
        else
            SiKDicePlay()->GetGoalReward( kUserBoardInfo.m_dwTotalCount, kDBPacket.m_vecGoalReward );
    }

    DWORD dwType = 0;
    DWORD dwProperty = 0;
    dwType = kUserBoardInfo.m_dwCurrentPosition;
    SiKDicePlay()->GetMapIDType( dwType, dwProperty ); // 해당 칸의 속성 얻어오기.!
    // 현재 칸의 보상 아이템 리스트 얻어오기.!
    if( dwProperty == KEVENT_BOARD_GAME_INFO_ACK::P_SPECIALMISSION )
    {
        SiKDicePlay()->GetMissionList( vecMissionList );
    }
    else if( !SiKDicePlay()->GetRewardInfo( dwProperty, kDBPacket.m_vecItemReward ) )
    {
        START_LOG( cerr, L"현재 보드칸에 보상 정보 없음 ..  Name : " << m_strName )
            << BUILD_LOG( dwProperty ) << END_LOG;
        SET_ERR_GOTO( ERR_DICE_PLAY_01, END_PROC );
    }

    // 받을 수 있는 스페셜 미션 리스트 조사
    bool bCheck = false;
    vit = vecMissionList.begin();
    while( vit != vecMissionList.end() )
    {
        // 이런 미션이 존재 하는가?
        if( !SiKMissionManager()->IsInMissions( *vit ) )
        {
            bCheck = true;
            START_LOG( cerr, L"해당 미션이 존재하지 않습니다. MissionID : " << *vit ) << END_LOG;
        }

        // 이미 동일한 미션은 수행중인가?
        if( IsRegistedMission( *vit ) )
        {
            START_LOG( cwarn, L"동일한 미션을 이미 진행중입니다. MissionID : " << *vit ) << END_LOG;
            vit = vecMissionList.erase( vit );
            continue;
        }
        ++vit;
    }

    // 랜덤 셔플링
    std::random_shuffle( vecMissionList.begin(), vecMissionList.end() );

    if( !vecMissionList.empty() )
    {
        kDBPacket.m_vecMissionList.push_back( vecMissionList.front() );
    }

    if( bCheck ) kDBPacket.m_vecMissionList.clear();

    // DB에 전송해줄때의 처리된 유저의 보드게임 정보를 보내주자.
    kDBPacket.m_kMyBoardInfo = kUserBoardInfo;
    SetUserBoardInfo( kUserBoardInfo ); // 계산된 유저의 보드게임 정보 저장.
    //GetDBUpdateData( kDBPacket.m_kUserData ); // 현재 DB스레드에서 저장행위가 없음.

    // 미션 보상도 있지만 등록할 수 있는 미션이 없는 경우 OK값 설정해서 DB로 전달하자.
    if( dwProperty == KEVENT_BOARD_GAME_INFO_ACK::P_SPECIALMISSION &&
        kDBPacket.m_vecMissionList.empty() )
    {
        // 성공이지만 더이상 등록할 수 있는 미션이 없음.
        kDBPacket.m_nOK = NetError::ERR_DICE_PLAY_13;
    }

    // DB에 아이템을 넣고 클라이언트에게 주사위 굴린 결과값을 보내주자.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_DB_DICE_PLAY_REQ, kDBPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );      // 성공.
        ERR_CASE( ERR_DICE_PLAY_01,     kPacket.m_nOK, 1 );      // 해당 칸에 보상 리스트가 존재하지 않음.
        ERR_CASE( ERR_DICE_PLAY_02,     kPacket.m_nOK, 2 );      // 이벤트 기간이 아님.
        ERR_CASE( ERR_DICE_PLAY_03,     kPacket.m_nOK, 3 );      // 주사위 돌릴 수 있는 기회 없음
        ERR_CASE( ERR_DICE_PLAY_04,     kPacket.m_nOK, 4 );      // 오늘 주사위 굴릴수 있는 횟수 다 굴렸음.
        ERR_CASE( ERR_DICE_PLAY_05,     kPacket.m_nOK, 5 );      // 보상 아이템이 없으므로 DB거치지 않고 전송.
        ERR_CASE( ERR_DICE_PLAY_13,     kPacket.m_nOK, 8 );      // 성공이지만 더이상 등록할 수 있는 미션이 없음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );    // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"주사위 굴리는 중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_DICE_PLAY_ACK );
    }

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98 || kPacket.m_nOK == 5, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_kMyBoardInfo.m_dwRemainCount )
        << BUILD_LOG( kPacket.m_kMyBoardInfo.m_dwGiveCount )
        << BUILD_LOG( kPacket.m_kMyBoardInfo.m_dwRemainTime )
        << BUILD_LOG( kPacket.m_kMyBoardInfo.m_dwDiceCount )
        << BUILD_LOG( vecMissionList.size() )
        << BUILD_LOG( kDBPacket.m_vecItemReward.size() ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_DICE_PLAY_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    NetError::SetLastNetError( kPacket_.m_nOK );

    // 미션 스크롤에 미션 등록.
    std::vector<KMissionData>::iterator vit;

    // 성공이거나 성공인데 등록할 미션이 없는 경우가 아니면 실패.
    if( !IS_CORRECT( NET_OK) && !IS_CORRECT( ERR_DICE_PLAY_13 ) )
        goto END_PROC;

    // 주사위 이벤트 기간인지 체크.
    if( !SiKDicePlay()->IsRun() )
    {
        SET_ERR_GOTO( ERR_DICE_PLAY_02, END_PROC );
    }

    for( vit = kPacket_.m_vecMissionList.begin(); vit != kPacket_.m_vecMissionList.end(); ++vit )
    {
        m_vecMissionSlot.push_back( *vit );
        START_LOG( clog, L"새로운 미션 추가" )
            << BUILD_LOG( m_strName )
            << BUILD_LOG( m_vecMissionSlot.size() )
            << BUILD_LOG( kPacket_.m_vecMissionList.size() ) << END_LOG;
    }

    m_kInventory.AddItemList( kPacket_.m_vecItem );
    // 현재 유저의 주사위 굴린 정보 Ranking 계산을 위해서 전송.
    SiKDicePlay()->UpdateDicePlayRankPerUser( kPacket_.m_kMyBoardInfo.m_dwTotalCount,
        kPacket_.m_kMyBoardInfo.m_dwCurrentPosition,
        m_strNickName );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,             kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_DICE_PLAY_01,   kPacket_.m_nOK, 1 ); // 해당 칸에 보상 리스트가 존재하지 않음.
        ERR_CASE( ERR_DICE_PLAY_02,   kPacket_.m_nOK, 2 ); // 이벤트 기간이 아님.
        ERR_CASE( ERR_DICE_PLAY_03,   kPacket_.m_nOK, 3 ); // 주사위 돌릴 수 있는 기회 없음
        ERR_CASE( ERR_DICE_PLAY_04,   kPacket_.m_nOK, 4 ); // 오늘 주사위 굴릴수 있는 횟수 다 굴렸음.
        ERR_CASE( ERR_DICE_PLAY_05,   kPacket_.m_nOK, 5 ); // 보상 아이템이 없으므로 DB거치지 않고 전송.
        ERR_CASE( ERR_DICE_PLAY_09,   kPacket_.m_nOK, 6 ); // 존재하지 않는 유저이다. 
        ERR_CASE( ERR_DICE_PLAY_10,   kPacket_.m_nOK, 7 ); // 해당 아이템 DB처리 오류.
        ERR_CASE( ERR_DICE_PLAY_13,   kPacket_.m_nOK, 8 ); // 성공이지만 더이상 등록할 수 있는 미션이 없음.
    default:
        START_LOG( cerr, L"주사위를 굴리는 중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"주사위 아이템 지급 로그 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_FirstDiceNum )
        << BUILD_LOG( kPacket_.m_SecondDiceNum )
        << BUILD_LOG( kPacket_.m_kMyBoardInfo.m_dwRemainCount )
        << BUILD_LOG( kPacket_.m_kMyBoardInfo.m_dwGiveCount )
        << BUILD_LOG( kPacket_.m_kMyBoardInfo.m_dwRemainTime )
        << BUILD_LOG( kPacket_.m_kMyBoardInfo.m_dwDiceCount )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_DICE_PLAY_ACK );
}

IMPL_ON_FUNC( EVENT_DICE_ITEM_TRADE_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_DICE_ITEM_TRADE_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkItem;
    KUserBoardInfo kUserBoardInfo;
    kUserBoardInfo.Clear();

    SET_ERROR( ERR_UNKNOWN );

    // 주사위 이벤트 기간인지 체크.
    if ( false == SiKDicePlay()->IsRun() ) {
        SET_ERR_GOTO( ERR_DICE_PLAY_02, END_PROC );
    }

    GetUserBoardInfo( kUserBoardInfo );

    // 주사위 아이템을 보유 하고 있는가?
    GCITEMID dwDiceItemID = 0;
    dwDiceItemID = SiKDicePlay()->GetDiceItemID();
    pkItem = m_kInventory.FindDurationItemByItemID( dwDiceItemID );

    if ( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_DICE_PLAY_07, END_PROC );
    }

    if ( pkItem->m_nCount <= 0 ) {
        SET_ERR_GOTO( ERR_DICE_PLAY_07, END_PROC );
    }

    // 클라이언트의 아이템 번호와 서버의 아이템 번호 일치 체크 
    if ( kPacket_.m_dwItemID != dwDiceItemID ) {
        SET_ERR_GOTO( ERR_DICE_PLAY_08, END_PROC );
    }

    // 현재 유저의 보드 정보를 가져와서 아이템의 갯수만큼 더해주자.
    kUserBoardInfo.m_dwDiceCount += pkItem->m_nCount;

    kPacket.m_dwDiceCount = kUserBoardInfo.m_dwDiceCount;

    // 주사위 아이템 갯수를 0개로 세팅하고 해당 아이템 Vector를 클라이언트에게 전송.
    pkItem->m_nCount = 0;
    kPacket.m_vecUseItem.push_back( *pkItem );

    SetUserBoardInfo( kUserBoardInfo );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_DICE_PLAY_07,     kPacket.m_nOK, 1 );     // 주사위 아이템 가져오기 실패
        ERR_CASE( ERR_DICE_PLAY_08,     kPacket.m_nOK, 2 );     // 주사위 아이템 번호가 서버랑 틀리다
        ERR_CASE( ERR_DICE_PLAY_14,     kPacket.m_nOK, 3 );     // 말판 아이템이 존재 하지 않음
        ERR_CASE( ERR_DICE_PLAY_02,     kPacket.m_nOK, 4 );     // 이벤트 기간이 아님.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"주사위 아이템 교환작업중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    SEND_PACKET( EVENT_DICE_ITEM_TRADE_ACK );

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98 || kPacket.m_nOK == 3, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwItemID )
        << BUILD_LOG( kPacket.m_dwDiceCount ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_DICE_ITME_BUY_CHECK_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_DICE_ITME_BUY_CHECK_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    // 서버 클라이언트 주사위 아이템 ItemID 체크
    GCITEMID dwDiceItemID = 0;
    dwDiceItemID = SiKDicePlay()->GetDiceItemID();
    if( dwDiceItemID != kPacket_.m_dwItemID )
    {
        SET_ERR_GOTO( ERR_DICE_PLAY_08, END_PROC );
    }

    // DB에 ItemID를 보내서 주사위 아이템의 구매 여부에 대해서 판단하자.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_DICE_ITME_BUY_CHECK_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_DICE_PLAY_08,     kPacket.m_nOK, 2 );     // 주사위 아이템 번호가 서버랑 틀리다
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"주사위 아이템 구매체크중 알수 없는 오류 ...  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_DICE_ITME_BUY_CHECK_ACK );
    }

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_dwItemID ) << END_LOG;
}

IMPL_ON_FUNC( EVENT_DICE_ITME_BUY_CHECK_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,            kPacket_.m_nOK, 0 ); // 성공.
        ERR_CASE( ERR_DICE_PLAY_08,  kPacket_.m_nOK, 1 ); // 주사위 아이템 번호가 서버랑 틀리다.
        ERR_CASE( ERR_DICE_PLAY_11,  kPacket_.m_nOK, 2 ); // 해당 아이템은 최대 구매횟수 제한이 없음.
        ERR_CASE( ERR_DICE_PLAY_12,  kPacket_.m_nOK, 3 ); // 구매 제한 횟수를 초과하였음.
    default:
        START_LOG( cerr, L"주사위 아이템 체크 요청중 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"주사위 아이템 체크 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwItemID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_DICE_ITME_BUY_CHECK_ACK );
}

IMPL_ON_FUNC( EVENT_DONATION_ITEM_GET_REQ )
{
    // GRADE 패킷을 받았을때의 처리..,DB에 우선 확인처리 해야함..,
    KItemDonationInfo kGradeData;
    KEVENT_DB_DONATION_ITEM_GET_REQ kPacket;
    // Grade를 넣으면 해당 Grade 구조체가 리턴되면 함수 호출.
    SiKDonationManager()->GetGradeInfo( kPacket_.m_dwGrade, kGradeData );
    kPacket.m_kItemDonationInfo = kGradeData;

    // 해당 유저가 해당 아이템을 받을 자격이 되는지 체크.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_DB_DONATION_ITEM_GET_REQ, kPacket );

END_PROC:
    START_LOG( clog, L"클라이언트로부터 전송받은 Grade" )
        << BUILD_LOG(kPacket_.m_dwGrade)
        << BUILD_LOG(kGradeData.m_dwGrade)
        << BUILD_LOG(kGradeData.m_biGradeNum)
        << BUILD_LOG(kGradeData.m_vecRewardList.size())
        << BUILD_LOG(kGradeData.m_dwGradeState) << END_LOG;
}

IMPL_ON_FUNC( EVENT_DONATION_ITEM_GET_ACK )
{
    JIF( PacketHackCheck( nFrom_ ) );
    NetError::SetLastNetError( kPacket_.m_nOK );

    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    m_kInventory.AddItemList( kPacket_.m_vecItemIns );
END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,           kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_DONATION_04,  kPacket_.m_nOK, 4 );    // 유저 정보가 없음.
        ERR_CASE( ERR_DONATION_05,  kPacket_.m_nOK, 5 );    // DB 트랜잭션 에러.
        ERR_CASE( ERR_DONATION_06,  kPacket_.m_nOK, 6 );    // 해당 등급의 아이템 지급 받았음.
        ERR_CASE( ERR_DONATION_07,  kPacket_.m_nOK, 7 );    // DB에 해당 아이템 삽입 에러.
    default:
        START_LOG( cerr, L" 송편 기부 등급아이템 지급시 알수 없는 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_strName ) << END_LOG;
        kPacket_.m_nOK = 99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"송편 기부 등급 아이템 지급 로그 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_vecItemIns.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_DONATION_ITEM_GET_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_INVITE_FRIEND_LIST_REQ )
{
    VERIFY_STATE( ( 1, KUserFSM::STATE_ROOM ) );
    // 던전, 대전 채널에서만 가능.
    // 룸에서만 호출 가능..
    JIF( m_pkChannel );
    JIF( m_pkRoom );
    char cChannelType = m_pkChannel->GetType();
    if( !m_pkChannel->IsPvPChannel() &&
        cChannelType != KChannel::CT_DUNGEON ) 
    {
        START_LOG( cwarn, L"현재 채널에서는 초대가 불가능..."
            << "Name : " << m_strName
            << L", Channel Type : " << (int)(cChannelType) ) << END_LOG;
        return;
    }

    if( ::GetTickCount() - GetTick( FRIEND_INVITELIST_TICK ) > 10000 ) // 10초..
    {
        GetFriendInviteList( m_vecFriendInviteList, m_pkChannel->GetType() );
        SetTick( FRIEND_INVITELIST_TICK );
    }
    SEND_DATA( EVENT_INVITE_FRIEND_LIST_ACK, m_vecFriendInviteList );
    START_LOG( clog, L"초대 가능한 친구 리스트 전송.. Name : " << m_strName << L", Size : " << m_vecFriendInviteList.size() ) << END_LOG;
}

_IMPL_ON_FUNC( EVENT_JOIN_ROOM_OTHER_CHANNEL_REQ, KEVENT_INVITED_ROOM_FROM_SQUARE_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));
    VERIFY_STATE(( 1, KUserFSM::STATE_CHANNEL ));
    JIF( m_pkChannel );
    SET_ERROR( ERR_UNKNOWN );
    KJoinRoomReqInfo kReq;
    KJoinRoomAckInfo kAck;
    kAck.m_nOK = 99;
    KChannel* spSquare = m_pkChannel; // 임시로 저장해둔다.

    // 해당 유저를 찾아서
    KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( kPacket_.m_dwHostUID );
    if( !spUser )
    {
        SET_ERR_GOTO( ERR_CHANNEL_JOIN_ROOM_ERR_00, END_PROC );
    }
    // 채널 있는지, 같은지..
    KChannel* pkChannel = spUser->GetChannel();
    if( !pkChannel || pkChannel->GetUID() != kPacket_.m_dwChannelUID )
    {
        SET_ERR_GOTO( ERR_CHANNEL_JOIN_ROOM_ERR_01, END_PROC );
    }

    // 채널 등급이 맞는지 확인 하고.
    if( !pkChannel->CheckEnteranceGrade( m_cGrade ) )
    {
        SET_ERR_GOTO( ERR_CHANNEL_JOIN_ROOM_ERR_05, END_PROC );
    }

    KRoomManager* pkRoomMng = pkChannel->GetRoomManager();
    if( !pkRoomMng )
    {
        SET_ERR_GOTO( ERR_CHANNEL_JOIN_ROOM_ERR_02, END_PROC );
    }
    // 룸 존재 하는지 같은지..
    KRoom* pkRoom = spUser->GetRoom();
    if( !pkRoom || pkRoom->GetRoomID() != kPacket_.m_usRoomID )
    {
        SET_ERR_GOTO( ERR_CHANNEL_JOIN_ROOM_ERR_02,END_PROC );
    }
    // 이녀석 방장인지..
    KUser* pkUser = pkRoom->GetHostPtr();
    if( !pkUser || pkUser->GetUID() != kPacket_.m_dwHostUID )
    {
        SET_ERR_GOTO( ERR_CHANNEL_JOIN_ROOM_ERR_03, END_PROC );
    }

    if( pkRoom->GetStateID() != KRoomFSM::STATE_WAITING )
    {
        SET_ERR_GOTO( ERR_ROOM_05, END_PROC );
    }

    if( pkRoom->m_strPasswd != kPacket_.m_strRoomPasswd )
    {
        SET_ERR_GOTO( ERR_ROOM_06, END_PROC );
    }
    // 확인하고.. 방으로 입장. 처리..
    // 계급은 맞는지.. -- 이녀석 확실히..

    m_pkChannel->Leave( *this );
    //LeaveSquare();
    if( !SiKChannelManager()->EnterChannel( kPacket_.m_dwChannelUID, this ) )
    {
        START_LOG( cerr, L" 광장에서 방 접속시  채널 입장 실패.. Name : " << m_strName )
            << L"Msg : " << NetError::GetLastNetErrMsg() << END_LOG;
        SET_ERR_GOTO( ERR_CHANNEL_JOIN_ROOM_ERR_04, END_PROC );
    }

    kReq.m_uiJoinType           = KJoinRoomReqInfo::JOIN_NORMAL;
    kReq.m_usRoomID             = kPacket_.m_usRoomID;
    kReq.m_strRoomPasswd        = kPacket_.m_strRoomPasswd;
    kReq.m_cQuickJoinCategory   = -1;
    kReq.m_nQuickJoinModeID     = -1;
    kReq.m_vecKickedRoomID.clear();   
    kReq.m_nDifficult           = -1;
    kReq.m_kInDoorUserInfo      = kPacket_.m_kIndoorUserInfo;

    if ( GetCurrentCharType() != kReq.m_kInDoorUserInfo.m_cCharIndex ) { // 이경우는 발생하면 안된다.
        START_LOG( cerr, L"서버-클라이언트의 현재 캐릭터가 다르다, User: " << GetName() )
            << END_LOG;
        SET_ERR_GOTO( ERR_ROOM_35, END_PROC );
    }
    MakeCorrectInDoorInfo( kReq.m_kInDoorUserInfo );

    if( !pkRoomMng->JoinRoom( this, kReq, kAck ) ) // 안에서 SetError 하고 있음.
    {
        START_LOG( cwarn, L"광장에서 방입장 실패.. Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( NetError::GetLastNetError() ) << END_LOG;
        SET_ERR_GOTO( ERR_CHANNEL_JOIN_ROOM_ERR_04, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kAck.m_nOK, 0 ); 
        ERR_CASE( ERR_CHANNEL_JOIN_ROOM_ERR_00, kAck.m_nOK, 1 );  // 초대 요청한 유저가 존재하지 안음. )
        ERR_CASE( ERR_CHANNEL_JOIN_ROOM_ERR_01, kAck.m_nOK, 2 );  // 해당 채널과 요청 채널이 맞지 않음. )
        ERR_CASE( ERR_CHANNEL_JOIN_ROOM_ERR_02, kAck.m_nOK, 3 );  // 해당 Room과 요청 Room이 맞지 않음. )
        ERR_CASE( ERR_CHANNEL_JOIN_ROOM_ERR_03, kAck.m_nOK, 4 );  // 초대 요청자가 이미 방장이 아님. )
        ERR_CASE( ERR_CHANNEL_JOIN_ROOM_ERR_04, kAck.m_nOK, 5 );  // 상태 전이중 문제 발생 채널 로비로 강제 퇴장 )
        ERR_CASE( ERR_ROOM_04,                  kAck.m_nOK, 6 );  // 방 참여 실패. 잘못된 정보로 접속을 시도함.
        ERR_CASE( ERR_ROOM_19,                  kAck.m_nOK, 7 );  // 방 참여 실패. P2P 버전이 맞지 않음. )
        ERR_CASE( ERR_ROOM_05,                  kAck.m_nOK, 8 );  // 방 참여 실패. 현재 방이 대기상태가 아님. )
        ERR_CASE( ERR_ROOM_06,                  kAck.m_nOK, 9 );  // 방 참여 실패. 비밀번호가 다름. )
        ERR_CASE( ERR_ROOM_03,                  kAck.m_nOK, 10 ); // 방 접속 불가능. 방장 부재중 )
        ERR_CASE( ERR_ROOM_17,                  kAck.m_nOK, 11 ); // 방 참여 실패. 방장에게 접속 알림 패킷을 보내지 못함. )
        ERR_CASE( ERR_ROOM_35,                  kAck.m_nOK, 12 ); // 존재하지 않는 캐릭터가 선택되었습니다. )
        ERR_CASE( ERR_ROOM_36,                  kAck.m_nOK, 13 ); // 입장할수 있는 올바른 레벨이 아닙니다. )
        ERR_CASE( ERR_ROOM_01,                  kAck.m_nOK, 14 ); // 방 접속 불가능. 정원이 모두 찾습니다. )   // 접속자가 운영자 권한이 없음)
        ERR_CASE( ERR_ROOM_13,                  kAck.m_nOK, 15 ); // 방 참여 실패. 운영자 이벤트 기간중 이벤트 아이템을 이미 가지고 있음. )
        ERR_CASE( ERR_ROOM_30,                  kAck.m_nOK, 16 ); // 옵저버가 이미 존재하는 방에 옵저버가 들어갈려고 했다. )
        ERR_CASE( ERR_ROOM_33,                  kAck.m_nOK, 17 ); // 더이상 다른 길드는 입장할수 없습니다. )
        ERR_CASE( ERR_ROOM_34,                  kAck.m_nOK, 18 ); // 이미 동일한 길드원이 3명 있습니다. )
        ERR_CASE( ERR_ROOM_02,                  kAck.m_nOK, 19 ); // 방 접속 불가능. 현재 방에 동시에 다른 사용자가 접속중 임.)  // 현재 방에 똑같은 UID를 가진 사용자가 접속중임
        ERR_CASE( ERR_CHANNEL_JOIN_ROOM_ERR_05, kAck.m_nOK, 20 ); // 채널 입장 등급이 맞지 않아서 들어 갈 수 없음
    default:
        START_LOG( cerr, L"광장에서 룸 접속시 알수 없는 오류 : " << m_strName )
            << L"MSG : " << NetError::GetLastNetErrMsg() << END_LOG;
        kAck.m_nOK = 99;
    }

    if( kAck.m_nOK == 0 )
    {
        const KInDoorUserInfo* pkInDoorUserInfo = m_pkRoom->FindInDoorUserInfo( m_strName );
        if( pkInDoorUserInfo )
        {
            m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_JOIN_ROOM_BROAD, (*pkInDoorUserInfo), true );

            START_LOG( clog, L"광장에서 방으로 조인 .. Name : " << m_strName )
                << BUILD_LOG( pkInDoorUserInfo->m_nState )
                << BUILD_LOG( pkInDoorUserInfo->m_iPlayerIndex ) 
                << BUILD_LOG( pkInDoorUserInfo->m_strGuildName )
                << BUILD_LOG( pkInDoorUserInfo->m_bIsHost )
                << BUILD_LOG( pkInDoorUserInfo->m_bIsLive)
                << BUILD_LOG( pkInDoorUserInfo->m_bIsObserver )
                << BUILD_LOGc( pkInDoorUserInfo->m_cAuthLevel )
                << BUILD_LOGc( pkInDoorUserInfo->m_cCharIndex )
                << BUILD_LOGc( pkInDoorUserInfo->m_cPromotion )
                << BUILD_LOG( pkInDoorUserInfo->m_dwGuildUID ) << END_LOG;

            DumpInDoorCharInfo( pkInDoorUserInfo->m_vecCharInfo );
        }
    }

    SEND_COMPRESS_PACKET( EVENT_JOIN_ROOM_OTHER_CHANNEL_ACK, kAck );

    LOG_CONDITION( kAck.m_nOK == 0, clog, cwarn )
        << L" 광장에서의 방으로 접속 : " << kAck.m_nOK << dbg::endl
        << BUILD_LOG( kPacket_.m_dwChannelUID )
        << BUILD_LOG( kPacket_.m_dwHostUID )
        << BUILD_LOG( kPacket_.m_usRoomID )
        << BUILD_LOG( kPacket_.m_strRoomPasswd ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( EVENT_USER_BINGO_DATA_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_CHANNEL ));
    KEVENT_USER_BINGO_DATA_ACK kPacket;
    SET_ERROR( ERR_UNKNOWN );

    // 퀴즈 빙고 이벤트 기간 인가?
    if ( !SiKQuizBingo()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_00, END_PROC );
    }

    if ( m_kQuizBingo.IsLoaded() ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_01, END_PROC );
    }

    SET_ERROR( NET_OK );
    QUEUEING_ID_TO_DB_CHECK( EVENT_USER_BINGO_DATA_REQ );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_QUIZ_BINGO_00,    kPacket.m_nOK, 1 );     // 퀴즈 빙고 이벤트 기간이 아님.
        ERR_CASE( ERR_QUIZ_BINGO_01,    kPacket.m_nOK, 2 );     // 퀴즈 빙고 정보를 이미 로드 하였음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"빙고 정보 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 1, L"빙고 정보 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_USER_BINGO_DATA_ACK );
    }
}

IMPL_ON_FUNC( EVENT_USER_BINGO_DATA_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    m_kQuizBingo.SetBingoData( kPacket_ );
    kPacket_.m_bFinishToday = m_kQuizBingo.IsFinishedToday();
    kPacket_.m_nBingoCoinCount = m_kQuizBingo.GetCurrentBingoCoinCount();
    kPacket_.m_dwBingoBoardID = SiKQuizBingo()->GetBingoBoardID();
    kPacket_.m_dwBingoCoinID = SiKQuizBingo()->GetBingoCoinID();
    kPacket_.m_dwBingoResurrectionID = SiKQuizBingo()->GetResurrectionID();
    kPacket_.m_nAccPlayTime = m_kQuizBingo.GetCalcedAccTime();


END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_QUIZ_BINGO_00,    kPacket_.m_nOK, 1 );     // 퀴즈 빙고 이벤트 기간이 아님.
        ERR_CASE( ERR_QUIZ_BINGO_01,    kPacket_.m_nOK, 2 );     // 퀴즈 빙고 정보를 이미 로드 하였음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"빙고 정보 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"빙고 정보 데이터 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nAccPlayTime )
        << BUILD_LOG( kPacket_.m_nTotalBingoCoinCount )
        << BUILD_LOG( kPacket_.m_nUseBingoCoinCount )
        << BUILD_LOG( kPacket_.m_nBingoCoinCount )
        << BUILD_LOG( kPacket_.m_vecCorrectQuestions.size() )
        << BUILD_LOG( kPacket_.m_vecBingoData.size() )
        << BUILD_LOG( kPacket_.m_bFinishToday )
        << BUILD_LOG( kPacket_.m_dwBingoBoardID )
        << BUILD_LOG( kPacket_.m_dwBingoCoinID )
        << BUILD_LOG( kPacket_.m_dwBingoResurrectionID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_USER_BINGO_DATA_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_CHANGE_BINGO_COIN_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNEL ));
    KItemPtr pkItem;
    KEVENT_CHANGE_BINGO_COIN_ACK kPacket;
    int nCoinItemCoount = 0;

    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간 인가?
    if ( !SiKQuizBingo()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_00, END_PROC );
    }

    // 빙고 관련 데이터가 로드 되었는가?
    if ( !m_kQuizBingo.IsLoaded() ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_11, END_PROC );
    }

    //// 빙고 보드를 가지고 있는가?
    //if ( !m_kInventory.FindItemByItemID( SiKQuizBingo()->GetBingoBoardID() ) ) {
    //    SET_ERR_GOTO( ERR_QUIZ_BINGO_02, END_PROC );
    //}

    // 빙고 코인을 가지고 있는가?
    pkItem = m_kInventory.FindDurationItemByItemID( SiKQuizBingo()->GetBingoCoinID() );
    if ( !pkItem ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_03, END_PROC );
    }

    // 아이템 수량을 가져와기록 한다.
    nCoinItemCoount = pkItem->m_nCount;

    // 해당 코인 아이템의 수량을 초기화 한다.
    pkItem->m_nCount = 0;

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_CHANGE_BINGO_COIN_REQ, nCoinItemCoount );

    SET_ERROR( NET_OK );
END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_QUIZ_BINGO_00,    kPacket.m_nOK, 1 );     // 퀴즈 빙고 이벤트 기간이 아님.
        ERR_CASE( ERR_QUIZ_BINGO_11,    kPacket.m_nOK, 2 );     // 빙고 정보가 로드 되지 않았음
        ERR_CASE( ERR_QUIZ_BINGO_02,    kPacket.m_nOK, 3 );     // 빙고 보드 아이템을 가지고 있지 않음.
        ERR_CASE( ERR_QUIZ_BINGO_03,    kPacket.m_nOK, 4 );     // 빙고 코인 아이템을 가지고 있지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"빙고 코인 교환 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 1, L"빙고 코인 교환 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( nCoinItemCoount )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_CHANGE_BINGO_COIN_ACK );
    }
}

IMPL_ON_FUNC( EVENT_CHANGE_BINGO_COIN_ACK )
{
    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    m_kQuizBingo.SetTotalBingoCount( kPacket_.m_nTotalBingoCoinCount );
    m_kQuizBingo.SetUseBingoCount( kPacket_.m_nUseBingoCoinCount );
    kPacket_.m_nBingoCoinCount = m_kQuizBingo.GetCurrentBingoCoinCount();
    pkItem = m_kInventory.FindItemByItemID( SiKQuizBingo()->GetBingoCoinID() );
    if( pkItem ) {
        kPacket_.m_vecCoinItem.push_back( *pkItem );
    }


END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_QUIZ_BINGO_00,    kPacket_.m_nOK, 1 );     // 퀴즈 빙고 이벤트 기간이 아님.
        ERR_CASE( ERR_QUIZ_BINGO_11,    kPacket_.m_nOK, 2 );     // 빙고 정보가 로드 되지 않았음
        ERR_CASE( ERR_QUIZ_BINGO_02,    kPacket_.m_nOK, 3 );     // 빙고 보드 아이템을 가지고 있지 않음.
        ERR_CASE( ERR_QUIZ_BINGO_03,    kPacket_.m_nOK, 4 );     // 빙고 코인 아이템을 가지고 있지 않음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"빙고 코인 교환 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"빙고 코인 교환 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nTotalBingoCoinCount )
        << BUILD_LOG( kPacket_.m_nUseBingoCoinCount )
        << BUILD_LOG( kPacket_.m_nRequestCount )
        << BUILD_LOG( kPacket_.m_nBingoCoinCount )
        << BUILD_LOG( kPacket_.m_vecCoinItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_CHANGE_BINGO_COIN_ACK );
}

_IMPL_ON_FUNC( EVENT_BINGO_QUESTION_REQ, PAIR_INT )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNEL ));
    KEVENT_BINGO_QUESTION_ACK kPacket;
    int kReqPacket = -1;
    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간 인가?
    if ( !SiKQuizBingo()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_00, END_PROC );
    }

    // 빙고 관련 데이터가 로드 되었는가?
    if ( !m_kQuizBingo.IsLoaded() ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_11, END_PROC );
    }

    // 이미 풀은 문제 인가?
    if ( m_kQuizBingo.IsInBingoData( kPacket_ ) ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_07, END_PROC );
    }

    //// 빙고 보드를 가지고 있는가?
    //if ( !m_kInventory.FindItemByItemID( SiKQuizBingo()->GetBingoBoardID() ) ) {
    //    SET_ERR_GOTO( ERR_QUIZ_BINGO_02, END_PROC );
    //}

    // 빙고 코인을 보유 하고 있는가?
    if ( m_kQuizBingo.GetCurrentBingoCoinCount() <= 0 ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_05, END_PROC );
    }

    // 문제 가져 오기.
    if ( !SiKQuizBingo()->GetRandomQuestionID( m_kQuizBingo.GetCorrectQuestions(), kReqPacket ) ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_04, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BINGO_QUESTION_REQ, kReqPacket );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_QUIZ_BINGO_00,    kPacket.m_nOK, 1 );     // 퀴즈 빙고 이벤트 기간이 아님.
        ERR_CASE( ERR_QUIZ_BINGO_11,    kPacket.m_nOK, 2 );     // 빙고 정보가 로드 되지 않았음
        ERR_CASE( ERR_QUIZ_BINGO_02,    kPacket.m_nOK, 3 );     // 빙고 보드 아이템을 가지고 있지 않음.
        ERR_CASE( ERR_QUIZ_BINGO_05,    kPacket.m_nOK, 4 );     // 빙고 코인이 부족함.
        ERR_CASE( ERR_QUIZ_BINGO_04,    kPacket.m_nOK, 5 );     // 질문을 가져 오지 못함.
        ERR_CASE( ERR_QUIZ_BINGO_07,    kPacket.m_nOK, 6 );     // 이미 정답을 맞춘 칸 입니다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );    // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"빙고 문제 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == 6, clog, cwarn , L"빙고 문제 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        kPacket.m_nTotalBingoCoinCount = m_kQuizBingo.GetTotalBingoCount();
        kPacket.m_nUseBingoCoinCount = m_kQuizBingo.GetUseBingoCount();
        kPacket.m_nBingoCoinCount = m_kQuizBingo.GetCurrentBingoCoinCount();
        SEND_PACKET( EVENT_BINGO_QUESTION_ACK );
    }
}

IMPL_ON_FUNC( EVENT_BINGO_QUESTION_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    m_kQuizBingo.SetTotalBingoCount( kPacket_.m_nTotalBingoCoinCount );
    m_kQuizBingo.SetUseBingoCount( kPacket_.m_nUseBingoCoinCount );
    kPacket_.m_nBingoCoinCount = m_kQuizBingo.GetCurrentBingoCoinCount();

    if ( !SiKQuizBingo()->GetCompressedQuestion( kPacket_.m_nQuestionID, kPacket_.m_kQuestionBuff ) ) {
        LIF( SiKQuizBingo()->GetRandomQuestion( m_kQuizBingo.GetCorrectQuestions(), kPacket_.m_kQuestionBuff, kPacket_.m_nQuestionID ) );
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_QUIZ_BINGO_00,    kPacket_.m_nOK, 1 );     // 퀴즈 빙고 이벤트 기간이 아님.
        ERR_CASE( ERR_QUIZ_BINGO_11,    kPacket_.m_nOK, 2 );     // 빙고 정보가 로드 되지 않았음
        ERR_CASE( ERR_QUIZ_BINGO_02,    kPacket_.m_nOK, 3 );     // 빙고 보드 아이템을 가지고 있지 않음.
        ERR_CASE( ERR_QUIZ_BINGO_05,    kPacket_.m_nOK, 4 );     // 빙고 코인이 부족함.
        ERR_CASE( ERR_QUIZ_BINGO_04,    kPacket_.m_nOK, 5 );     // 질문을 가져 오지 못함.
        ERR_CASE( ERR_QUIZ_BINGO_07,    kPacket_.m_nOK, 6 );     // 이미 정답을 맞춘 칸 입니다.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.

    default:
        START_LOG( cerr, L"빙고 문제 응답 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn , L"빙고 문제 응답 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nTotalBingoCoinCount )
        << BUILD_LOG( kPacket_.m_nUseBingoCoinCount )
        << BUILD_LOG( kPacket_.m_nBingoCoinCount )
        << BUILD_LOG( kPacket_.m_nQuestionID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BINGO_QUESTION_ACK );
}

IMPL_ON_FUNC( EVENT_BINGO_ANSWER_REQ )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_ROOM, KUserFSM::STATE_CHANNEL ));

    KBingoAnswerAckData kPacket;
    KBingoAnswerReqForDB kDBPacket;
    kDBPacket.m_prPos = kPacket_.m_prPos;
    kDBPacket.m_nQuestionID = kPacket_.m_nQuestionID;
    KBingoQuestionInfo kQuestion;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간 인가?
    if ( !SiKQuizBingo()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_00, END_PROC );
    }

    // 빙고 관련 데이터가 로드 되었는가?
    if ( !m_kQuizBingo.IsLoaded() ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_11, END_PROC );
    }

    // 이미 풀은 문제 인가?
    if ( m_kQuizBingo.IsInBingoData( kPacket_.m_prPos) ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_07, END_PROC );
    }

    //// 빙고 보드를 가지고 있는가?
    //if ( !m_kInventory.FindItemByItemID( SiKQuizBingo()->GetBingoBoardID() ) ) {
    //    SET_ERR_GOTO( ERR_QUIZ_BINGO_02, END_PROC );
    //}

    // 존재 하는 문제 인가?
    if ( !SiKQuizBingo()->GetQuestion( kPacket_.m_nQuestionID, kQuestion) ) {
        SET_ERR_GOTO( ERR_QUIZ_BINGO_06, END_PROC );
    }

    // 부활 썼을 경우
    if ( kPacket_.m_bUseResurrection ) {
        KItemPtr pkItem;
        pkItem = m_kInventory.FindDurationItemByItemID( SiKQuizBingo()->GetResurrectionID() );
        if ( !pkItem ) {
            SET_ERR_GOTO( ERR_QUIZ_BINGO_10, END_PROC );
        }

        int nQuestionID = -1;
        std::pair<int,int> prPos;
        if( !m_kQuizBingo.GetLastFail( nQuestionID, prPos ) ) {
            SET_ERR_GOTO( ERR_QUIZ_BINGO_08, END_PROC );
        }

        // 부활을 썼지만 이전에 틀린 문제/위치가 아님.
        if( kPacket_.m_nQuestionID != nQuestionID || kPacket_.m_prPos != prPos ) {
            SET_ERR_GOTO( ERR_QUIZ_BINGO_09, END_PROC );
        }

        //m_kQuizBingo.ResetLastFail();
        pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount-1 );
    }

    // 정답인가?
    kDBPacket.m_bCorrectAnswer = (kQuestion.m_nAnswer == kPacket_.m_nAnswerNum);

    // 정답이라면 몇가지 처리를 해주자.
    if ( kDBPacket.m_bCorrectAnswer ) {
        // 빙고 판에 포지션을 찍어 주자.
        m_kQuizBingo.AddPos( kPacket_.m_prPos );

        // 정답을 기록 해두자.
        m_kQuizBingo.AddCorrectQuestion( kPacket_.m_nQuestionID );

        // 보상을 위해 추가로 빙고가 된것을 찾자.
        m_kQuizBingo.GetCalculatedBingoLine( kPacket_.m_prPos, kDBPacket.m_vecLineBingo );

        // 빙고 라인 수를 추가 해주자.
        m_kQuizBingo.AddBingoLineCount( (int)kDBPacket.m_vecLineBingo.size() );
        m_kQuizBingo.ResetLastFail();

    }
    else {
        // 틀리기만 했을 경우 실패를 기록 한다.
        m_kQuizBingo.SetLastFail( kPacket_.m_nQuestionID, kPacket_.m_prPos );
    }


    QUEUEING_EVENT_TO_DB_CHECK( EVENT_BINGO_ANSWER_REQ, kDBPacket );
    SET_ERROR( NET_OK );
END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_QUIZ_BINGO_00,    kPacket.m_nOK, 1 );     // 퀴즈 빙고 이벤트 기간이 아님.
        ERR_CASE( ERR_QUIZ_BINGO_11,    kPacket.m_nOK, 2 );     // 빙고 정보가 로드 되지 않았음.
        ERR_CASE( ERR_QUIZ_BINGO_02,    kPacket.m_nOK, 3 );     // 빙고 보드 아이템을 가지고 있지 않음.
        ERR_CASE( ERR_QUIZ_BINGO_05,    kPacket.m_nOK, 4 );     // 빙고 코인이 부족함.
        ERR_CASE( ERR_QUIZ_BINGO_06,    kPacket.m_nOK, 4 );     // 존재하지 않는 문제.
        ERR_CASE( ERR_QUIZ_BINGO_07,    kPacket.m_nOK, 5 );     // 이미 정답을 맞춘 칸 입니다.
        ERR_CASE( ERR_QUIZ_BINGO_08,    kPacket.m_nOK, 6 );     // 직전에 틀리지 않았는데 부활을 쓰려고함.
        ERR_CASE( ERR_QUIZ_BINGO_09,    kPacket.m_nOK, 6 );     // 부활을 썼지만 이전에 틀린 문제 또는 위치가 다름.
        ERR_CASE( ERR_QUIZ_BINGO_10,    kPacket.m_nOK, 7 );     // 부활 아이템을 보유 하고 있지 않음.

        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"빙고 정답 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == 1, L"빙고 정답 요청 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nQuestionID )
        << BUILD_LOG( kPacket_.m_nAnswerNum )
        << BUILD_LOG( kPacket_.m_bUseResurrection )
        << BUILD_LOG( kPacket_.m_prPos.first )
        << BUILD_LOG( kPacket_.m_prPos.second )
        << BUILD_LOG( kPacket.m_nOK ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_BINGO_ANSWER_ACK );
    }
}

_IMPL_ON_FUNC( EVENT_BINGO_ANSWER_ACK, KBingoAnswerAckData )
{
    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if ( !IS_CORRECT( NET_OK) ) {
        goto END_PROC;
    }

    pkItem = m_kInventory.FindItemByItemID( SiKQuizBingo()->GetResurrectionID() );
    if ( pkItem ) {
        kPacket_.m_vecResItem.push_back( *pkItem );
    }

    m_kInventory.AddItemList( kPacket_.m_vecItem );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,               kPacket_.m_nOK, 0 );     // 성공
        ERR_CASE( ERR_QUIZ_BINGO_00,    kPacket_.m_nOK, 1 );     // 퀴즈 빙고 이벤트 기간이 아님.
        ERR_CASE( ERR_QUIZ_BINGO_11,    kPacket_.m_nOK, 2 );     // 빙고 정보가 로드 되지 않았음.
        ERR_CASE( ERR_QUIZ_BINGO_02,    kPacket_.m_nOK, 3 );     // 빙고 보드 아이템을 가지고 있지 않음.
        ERR_CASE( ERR_QUIZ_BINGO_05,    kPacket_.m_nOK, 4 );     // 빙고 코인이 부족함.
        ERR_CASE( ERR_QUIZ_BINGO_06,    kPacket_.m_nOK, 4 );     // 존재하지 않는 문제.
        ERR_CASE( ERR_QUIZ_BINGO_07,    kPacket_.m_nOK, 5 );     // 이미 정답을 맞춘 칸 입니다.
        ERR_CASE( ERR_QUIZ_BINGO_08,    kPacket_.m_nOK, 6 );     // 직전에 틀리지 않았는데 부활을 쓰려고함.
        ERR_CASE( ERR_QUIZ_BINGO_09,    kPacket_.m_nOK, 6 );     // 부활을 썼지만 이전에 틀린 문제 또는 위치가 다름.
        ERR_CASE( ERR_QUIZ_BINGO_10,    kPacket_.m_nOK, 7 );     // 부활 아이템을 보유 하고 있지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"빙고 정답 요청 중 알수 없는 오류 Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 1, L"빙고 정답 응답 중 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_bCorrect )
        << BUILD_LOG( kPacket_.m_prPos.first )
        << BUILD_LOG( kPacket_.m_prPos.second )
        << BUILD_LOG( kPacket_.m_vecResItem.size() )
        << BUILD_LOG( kPacket_.m_vecItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_BINGO_ANSWER_ACK );
}

IMPL_ON_FUNC( EVENT_TOURNAMENT_DONATION_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));
    KEVENT_TOURNAMENT_DONATION_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkItem;

    SET_ERROR( ERR_UNKNOWN );
    // 기부 이벤트 기간인가
    if( !SiKGCHelper()->IsTourDonationEvent() ) {
        SET_ERR_GOTO( ERR_TOURNAMENT_DONATION_00, END_PROC );
    }

    // 서버와 클라이언트 기부 아이템 번호와 일치하는가?
    if( !SiKGCHelper()->IsDonationItemID( kPacket_.m_itemID ) ) {
        SET_ERR_GOTO( ERR_TOURNAMENT_DONATION_01, END_PROC );
    }

    // 기부할 아이템 보유 여부 체크 
    pkItem = m_kInventory.FindDurationItemByItemID( kPacket_.m_itemID );
    if( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_TOURNAMENT_DONATION_02, END_PROC );
    }

    // 기부할 아이템 개수는 충분한가?
    if( pkItem->m_nCount < kPacket_.m_nItemCount ) {
        SET_ERR_GOTO( ERR_TOURNAMENT_DONATION_03, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_TOURNAMENT_DONATION_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );                          // 성공
        ERR_CASE( ERR_TOURNAMENT_DONATION_00, kPacket.m_nOK, 1 );      // 이벤트 기간이 아님.
        ERR_CASE( ERR_TOURNAMENT_DONATION_01, kPacket.m_nOK, 2 );      // 서버 클라이언트간 기부 아이템 번호 불일치.
        ERR_CASE( ERR_TOURNAMENT_DONATION_02, kPacket.m_nOK, 3 );      // 기부 아이템 가져오기 실패.
        ERR_CASE( ERR_TOURNAMENT_DONATION_03, kPacket.m_nOK, 4 );      // 기부 아이템 갯수가 부족하다.
        ERR_CASE( ERR_TOURNAMENT_DONATION_04, kPacket.m_nOK, 5 );      // DB에 기부 정보 넣기 실패.
        ERR_CASE( ERR_NOT_FINISHED_YET,       kPacket.m_nOK, -98 );    // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"대회선수 기부요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_nType )
            << BUILD_LOG( kPacket_.m_itemID )
            << BUILD_LOG( kPacket_.m_nItemCount ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, clog, cwarn, L"대회선수 기부요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nType )
        << BUILD_LOG( kPacket_.m_itemID )
        << BUILD_LOG( kPacket_.m_nItemCount ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_TOURNAMENT_DONATION_ACK );
    }
}

IMPL_ON_FUNC( EVENT_TOURNAMENT_DONATION_ACK )
{
    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );
    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    pkItem = m_kInventory.FindItemByItemID( kPacket_.m_itemID );
    // 아이템 소유 여부 확인.
    if( NULL == pkItem ) {
        SET_ERR_GOTO( ERR_TOURNAMENT_DONATION_02, END_PROC );
    }

    // 기부할 아이템 개수는 충분한가?

    pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - kPacket_.m_nItemCount );
    kPacket_.m_nItemCount = pkItem->m_nCount;  // 현재 해당 아이템 갯수 전송.
    kPacket_.m_vecUseItem.push_back( *pkItem );


END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket_.m_nOK, 0 );                          // 성공
        ERR_CASE( ERR_TOURNAMENT_DONATION_00, kPacket_.m_nOK, 1 );      // 이벤트 기간이 아님.
        ERR_CASE( ERR_TOURNAMENT_DONATION_01, kPacket_.m_nOK, 2 );      // 서버 클라이언트간 기부 아이템 번호 불일치.
        ERR_CASE( ERR_TOURNAMENT_DONATION_02, kPacket_.m_nOK, 3 );      // 기부 아이템 가져오기 실패.
        ERR_CASE( ERR_TOURNAMENT_DONATION_03, kPacket_.m_nOK, 4 );      // 기부 아이템 갯수가 부족하다.
        ERR_CASE( ERR_TOURNAMENT_DONATION_04, kPacket_.m_nOK, 5 );      // DB에 기부 정보 넣기 실패.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"대회선수 기부요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_itemID )
            << BUILD_LOG( kPacket_.m_nItemCount ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L"대회선수 기부요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_itemID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( kPacket_.m_vecUseItem.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_TOURNAMENT_DONATION_ACK );
}

_IMPL_ON_FUNC( EVENT_ITEM_BUY_CHECK_REQ, GCITEMID )
{
    VERIFY_STATE((5,KUserFSM::STATE_CHANNEL,KUserFSM::STATE_CHANNELLOBBY,KUserFSM::STATE_ROOM,KUserFSM::STATE_AGIT,KUserFSM::STATE_SQUARE));

    const GCITEMID& ItemID = kPacket_;

    KEVENT_ITEM_BUY_CHECK_ACK kPacket;
    kPacket.m_nOK = 99;
    kPacket.m_ItemID = ItemID;

    SET_ERROR( ERR_UNKNOWN );

    // 아지트아이템 소지한도 체크
    if ( false == CheckItemCount( ItemID ) ) {
        SET_ERR_GOTO( ERR_ITEM_BUY_CHECK_05, END_PROC );
    }

    // 인벤토리 여유공간 확인 (내부에서 에러값 설정)
    if ( false == CheckItemInvenLimit( ItemID ) ) {
        goto END_PROC;
    }

    // 해당 아이템이 구매 제한 체크가 필요한 아이템인가?
    if( false == SiKItemManager()->IsBuyLimitItem( ItemID ) ) {
        SET_ERR_GOTO( ERR_ITEM_BUY_CHECK_00, END_PROC );
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_ITEM_BUY_CHECK_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ITEM_BUY_CHECK_00, kPacket.m_nOK, 1 ); // 구매제한 아이템이 아님(성공)
        ERR_CASE( ERR_ITEM_BUY_CHECK_01, kPacket.m_nOK, 2 ); // 구매제한 횟수 초과.
        ERR_CASE( ERR_ITEM_BUY_CHECK_02, kPacket.m_nOK, 3 ); // 인벤토리가 충분하지 않음.
        ERR_CASE( ERR_ITEM_BUY_CHECK_03, kPacket.m_nOK, 4 ); // 코디인벤토리가 충분하지 않음.
        ERR_CASE( ERR_ITEM_BUY_CHECK_04, kPacket.m_nOK, 5 ); // 아지트인벤토리가 충분하지 않음.
        ERR_CASE( ERR_ITEM_BUY_CHECK_05, kPacket.m_nOK, 6 ); // 아이템 소지한도 초과.
        ERR_CASE( ERR_NOT_FINISHED_YET,  kPacket.m_nOK, -98 ); // 아직 처리중인 작업.
    default:
        kPacket.m_nOK = -99;
        START_LOG( cerr, L" 아이템 구매제한 체크중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_ ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == 1 || kPacket.m_nOK == -98 || kPacket.m_nOK == 2, clog, cwarn, L"아이템 구매체크 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( ItemID )
        << BUILD_LOG( m_kInventory.GetTotalEnableSize() )
        << BUILD_LOG( GetCharInvenCapacity() )
        << BUILD_LOG( m_kInventory.GetLookItemNum() )
        << BUILD_LOG( SiKLookEquipment()->GetMaxInvenSize() ) << END_LOG;

    if ( NetError::GetLastNetError() != NetError::NET_OK ) {
        SEND_PACKET( EVENT_ITEM_BUY_CHECK_ACK );
    }
}

IMPL_ON_FUNC( EVENT_ITEM_BUY_CHECK_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if( !IS_CORRECT( NET_OK) )
        goto END_PROC;

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ITEM_BUY_CHECK_00, kPacket_.m_nOK, 1 ); // 구매제한 아이템이 아님(성공)
        ERR_CASE( ERR_ITEM_BUY_CHECK_01, kPacket_.m_nOK, 2 ); // 구매제한 횟수 초과.
        ERR_CASE( ERR_ITEM_BUY_CHECK_02, kPacket_.m_nOK, 3 ); // 인벤토리가 충분하지 않음.
        ERR_CASE( ERR_ITEM_BUY_CHECK_03, kPacket_.m_nOK, 4 ); // 코디인벤토리가 충분하지 않음.
        ERR_CASE( ERR_NOT_FINISHED_YET,  kPacket_.m_nOK, -98 ); // 아직 처리중인 작업.
    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"아이템 구매 체크 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 2, L"아이템 구매 체크 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_ITEM_BUY_CHECK_ACK );
}

IMPL_ON_FUNC( EVENT_WEB_GACHA_COIN_TRADE_REQ )
{
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_ROOM ));
    KEVENT_WEB_GACHA_COIN_TRADE_ACK kPacket;
    kPacket.m_nOK = 99;
    KItemPtr pkItem;
    SET_ERROR( ERR_UNKNOWN );

    // 해당 아이템을 보유 하고 있는가?
    pkItem = m_kInventory.FindDurationItemByItemID( KEVENT_WEB_GACHA_COIN_TRADE_REQ::GACHA_COIN_EXCHANGE_TICKET_GOODS_ID );

    if( NULL == pkItem )
    {
        SET_ERR_GOTO( ERR_WEB_GACHA_TRADE_00, END_PROC );
    }

    // 클라이언트의 아이템 번호와 서버의 아이템 번호 일치 체크 
    if( kPacket_.m_itemID != KEVENT_WEB_GACHA_COIN_TRADE_REQ::GACHA_COIN_EXCHANGE_TICKET_GOODS_ID )
    {
        SET_ERR_GOTO( ERR_WEB_GACHA_TRADE_01, END_PROC );
    }

    // 클라이언트의 가챠 코인권 갯수랑 서버의 갯수 체크
    if( pkItem->m_nCount != kPacket_.m_nItemCount )
    {
        SET_ERR_GOTO( ERR_WEB_GACHA_TRADE_02, END_PROC );
    }

    // DB에 아이템을 넣고 해당 웹 가챠 코인 전체값을 받아오자!
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_WEB_GACHA_COIN_TRADE_REQ, kPacket_ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_WEB_GACHA_TRADE_00,   kPacket.m_nOK, 1 ); // 웹 가챠 교환권 가져오기 실패
        ERR_CASE( ERR_WEB_GACHA_TRADE_01,   kPacket.m_nOK, 2 ); // 웹 가챠 교환권의 아이템 번호가 서버랑 틀리다
        ERR_CASE( ERR_WEB_GACHA_TRADE_02,   kPacket.m_nOK, 3 ); // 웹 가챠 교환권의 아이템갯수가 서버랑 틀리다
        ERR_CASE( ERR_NOT_FINISHED_YET, kPacket.m_nOK, -98 ); // 아직 처리중인 작업입니다.
    default:
        START_LOG( cerr, L"가챠코인 교환작업중 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    LOG_CONDITION( kPacket.m_nOK == 0 || kPacket.m_nOK == -98, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_itemID )
        << BUILD_LOG( kPacket_.m_nItemCount ) << END_LOG;

    if( NetError::GetLastNetError() != NetError::NET_OK )
    {
        SEND_PACKET( EVENT_WEB_GACHA_COIN_TRADE_ACK );
    }
}

IMPL_ON_FUNC( EVENT_WEB_GACHA_COIN_TRADE_ACK )
{
    int nCount = kPacket_.m_nItemCount; // 교환후 없어질 아이템 갯수
    // 해당 아이템을 보유 하고 있는가?
    KItemPtr pkItem = m_kInventory.FindDurationItemByItemID( KEVENT_WEB_GACHA_COIN_TRADE_REQ::GACHA_COIN_EXCHANGE_TICKET_GOODS_ID );
    if( NULL == pkItem )
    {
        SET_ERR_GOTO( ERR_WEB_GACHA_TRADE_00, END_PROC );
    }

    // 크리스탈 개수는 충분한가?
    pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - nCount ); // 해당 아이템 갯수 차감.
    kPacket_.m_nItemCount = pkItem->m_nCount;

    // 클라이언트에게 사용한 아이템 정보를 보내주기 위해서.
    kPacket_.m_vecUseItem.push_back( *pkItem );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,                  kPacket_.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_WEB_GACHA_TRADE_00,       kPacket_.m_nOK, 1 ); // 웹 가챠 교환권 가져오기 실패
        ERR_CASE( ERR_WEB_GACHA_TRADE_02,       kPacket_.m_nOK, 3 ); // 웹 가챠 교환권의 아이템갯수가 서버랑 틀리다
        ERR_CASE( ERR_WEB_GACHA_TRADE_03,       kPacket_.m_nOK, 4 ); // 존재하지 않는 유저이다.
        ERR_CASE( ERR_WEB_GACHA_TRADE_04,       kPacket_.m_nOK, 5 ); // 유저의 가챠코인이 존재하지 않는다.
        ERR_CASE( ERR_WEB_GACHA_TRADE_05,       kPacket_.m_nOK, 6 ); // 해당 아이템 DB처리 오류.
    default:
        START_LOG( cerr, L"아이템 교환에 알수 없는 오류 ..  Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        kPacket_.m_nOK = -99;
    }

    LOG_CONDITION( kPacket_.m_nOK == 0 || kPacket_.m_nOK == -98, clog, cerr )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_itemID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( kPacket_.m_nWebGachaCoin ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_WEB_GACHA_COIN_TRADE_ACK );
}

_IMPL_ON_FUNC( EVENT_ADVERTISING_EXPOSURE_COUNT_NOT, PAIR_INT )
{
    // 광고 노출 카운트
    const int& nKey = kPacket_.first; // 0부터 시작
    const int& nCount = kPacket_.second; // 클릭 회수

    int nAdType = nKey + CDT_AD_TYPE_0;
    __LIF( nAdType >= CDT_AD_TYPE_0 && nAdType <= CDT_AD_TYPE_9 );
    __LIF( nCount >= 0 );
    m_mapAdvertisingExposureCount[ nAdType ] += std::max<int>( nCount, 0 );
}

IMPL_ON_FUNC_NOPARAM( EVENT_GWC_EVENT_INFO_REQ )
{
    // ROOM안에 들어가서 장비창으로 들어가면 해당 유저의 상태정보는 ROOM 상태이기 때문에 ROOM STATE 추가.
    VERIFY_STATE(( 3, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_CHANNELLOBBY, KUserFSM::STATE_ROOM ));
    KEVENT_GWC_EVENT_INFO_ACK kPacket;
    SET_ERROR( ERR_UNKNOWN );

    // 이벤트 기간인가?
    if ( !SiKGWCSupport()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_GWC_SUPPORT_00, END_PROC );
    }

    QUEUEING_ID_TO_DB_CHECK( EVENT_GWC_EVENT_INFO_REQ );
    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );     // 성공.
        ERR_CASE( ERR_GWC_SUPPORT_00,       kPacket.m_nOK, 1 );     // 이벤트 기간이 아님
        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"GWC Event 정보 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"GWC Event 정보 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName ) << END_LOG;

    if( !IS_CORRECT( NET_OK ) )
        SEND_PACKET( EVENT_GWC_EVENT_INFO_ACK );
}

IMPL_ON_FUNC( EVENT_GWC_EVENT_INFO_ACK )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) )
        goto END_PROC;

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 );                      // 성공.
        ERR_CASE( ERR_GWC_SUPPORT_00,       kPacket_.m_nOK, 1 );    // 이벤트 기간이 아님

        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"GWC Event 정보 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cwarn, L"GWC Event 정보 요청 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GWC_EVENT_INFO_ACK );
}

_IMPL_ON_FUNC( EVENT_GWC_EVENT_DONATION_REQ, int )
{
    VERIFY_STATE(( 2, KUserFSM::STATE_CHANNEL, KUserFSM::STATE_ROOM ));
    KEVENT_GWC_EVENT_DONATION_ACK kPacket;
    KItemPtr pkItem;
    SET_ERROR( ERR_UNKNOWN );
    // 이벤트 기간인가?
    if ( !SiKGWCSupport()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_GWC_SUPPORT_00, END_PROC );
    }

    // 정보가 로드된 상태 인가?
    if ( !SiKGWCSupport()->IsInUserData( GetUID() ) ) {
        SET_ERR_GOTO( ERR_GWC_SUPPORT_01, END_PROC );
    }

    if ( kPacket_ <= 0 ) {
        SET_ERR_GOTO( ERR_GWC_SUPPORT_05, END_PROC );
    }
    // 아이템이 존재 하는가?
    pkItem = m_kInventory.FindDurationItemByItemID( SiKGWCSupport()->GetEventItem() );
    if ( !pkItem ) {
        SET_ERR_GOTO( ERR_GWC_SUPPORT_02, END_PROC );
    }

    // 아이템 수량은 옳은가?
    if ( pkItem->m_nCount < kPacket_ ) {
        SET_ERR_GOTO( ERR_GWC_SUPPORT_03, END_PROC );
    }
    // DB에 큐잉.
    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GWC_EVENT_DONATION_REQ, kPacket_ );

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GWC_SUPPORT_00,       kPacket.m_nOK, 1 );    // GWC 이벤트 기간이 아님. 
        ERR_CASE( ERR_GWC_SUPPORT_01,       kPacket.m_nOK, 2 );    // GWC 유저 정보가 로드 되지 않았음. 
        ERR_CASE( ERR_GWC_SUPPORT_02,       kPacket.m_nOK, 3 );    // GWC 이벤트 아이템이 존재 하지 않음. 
        ERR_CASE( ERR_GWC_SUPPORT_03,       kPacket.m_nOK, 4 );    // GWC 이벤트 아이템의 보유 수량이 요청 수량 보다 적음. 
        ERR_CASE( ERR_GWC_SUPPORT_05,       kPacket.m_nOK, 5 );    // GWC 기부 수량을 음수개 요청. 

        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"GWC Event 기부중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_ ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L"GWC Event 정보 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    if( !IS_CORRECT( NET_OK ) )
        SEND_PACKET( EVENT_GWC_EVENT_DONATION_ACK );
}

IMPL_ON_FUNC( EVENT_GWC_EVENT_DONATION_ACK )
{
    KItemPtr pkItem;
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) )
        goto END_PROC;

    m_kInventory.AddItemList( kPacket_.m_vecInv );

    pkItem = m_kInventory.FindDurationItemByItemID( SiKGWCSupport()->GetEventItem() );

    if ( pkItem ) {
        pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - kPacket_.m_nUseCount );
        kPacket_.m_vecInv.push_back( *pkItem );
    }

END_PROC:
    switch( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket_.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GWC_SUPPORT_00,       kPacket_.m_nOK, 1 );    // 
        ERR_CASE( ERR_GWC_SUPPORT_01,       kPacket_.m_nOK, 2 );    // 
        ERR_CASE( ERR_GWC_SUPPORT_02,       kPacket_.m_nOK, 3 );    // 
        ERR_CASE( ERR_GWC_SUPPORT_03,       kPacket_.m_nOK, 4 );    // 
        ERR_CASE( ERR_GWC_SUPPORT_05,       kPacket_.m_nOK, 5 );    // 

        ERR_CASE( ERR_NOT_FINISHED_YET,     kPacket_.m_nOK, -98 );   // 아직 처리중인 작업입니다.
    default:
        kPacket_.m_nOK = 99;
        START_LOG( cerr, L"GWC Event 기부 응답중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_nUseCount )
            << BUILD_LOG( kPacket_.m_nAccCount )
            << BUILD_LOG( kPacket_.m_vecInv.size() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket_.m_nOK == 0, clog, cerr, L"GWC Event 정보 응답 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nUseCount )
        << BUILD_LOG( kPacket_.m_nAccCount )
        << BUILD_LOG( kPacket_.m_vecInv.size() ) << END_LOG;

    SEND_RECEIVED_PACKET( EVENT_GWC_EVENT_DONATION_ACK );
}

IMPL_ON_FUNC_NOPARAM( EVENT_GWC_EVENT_RESULT_REQ )
{
    VERIFY_STATE(( 1, KUserFSM::STATE_SQUARE ));
    KEVENT_GWC_EVENT_RESULT_ACK kPacket;
    // 이벤트 기간인가?
    SET_ERROR( ERR_UNKNOWN );
    if ( !SiKGWCSupport()->IsEventTerm() ) {
        SET_ERR_GOTO( ERR_GWC_SUPPORT_00, END_PROC );
    }
    kPacket.m_nAccCount = SiKGWCSupport()->GetUserAccCount( GetUID() );
    SiKGWCSupport()->GetGWCData( kPacket.m_vecNationData );

    SET_ERROR( NET_OK );

END_PROC:
    switch ( NetError::GetLastNetError() ) {
        ERR_CASE( NET_OK,                   kPacket.m_nOK, 0 );    // 성공.
        ERR_CASE( ERR_GWC_SUPPORT_00,       kPacket.m_nOK, 1 );    // 이벤트 기간이 아님.
    default:
        kPacket.m_nOK = 99;
        START_LOG( cerr, L"GWC Event 기부 내용 요청중 알수 없는 오류, Name : " << m_strName )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    _LOG_CONDITION( kPacket.m_nOK == 0, clog, cwarn, L" GWC Event 기부 내용 요청 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nAccCount )
        << BUILD_LOG( kPacket.m_vecNationData.size() ) << END_LOG;
    SEND_PACKET( EVENT_GWC_EVENT_RESULT_ACK );
}
