#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include "MissionManager.h"
#include "GSSimLayer.h"
#include "User.h"
#include <boost/algorithm/string.hpp> //boost::to_lower_copy
#include "GameServer.h"
#include "IndigoUtil.h" // KServerOpenTime, boost::tuple
#include "FatigueSystem.h"
#include "LogManager.h" // 로그를 남기기 위해
#include <boost/tuple/tuple.hpp>
#include "ResultManager.h"
#include "EnchantStrong.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include "GCHelper.h"
#include "RankManager.h"
#include "ItemManager.h"
#include "CollectionManager.h"
#include "support_User.h"
#include "LoudMessage.h"
#include "GachaManager.h"
#include "CoupleSystem.h"

#include "EventType.h"
#include "LunaNewYear.h"
#include "CountType.h"
#include "RecomHelper.h"
#include "VirtualCash.h"
#include "DicePlay.h"
#include "GWCSupport.h"
#include "NewDonationManager.h"
#include "SkillManager.h"
#include "Kairo.h"

#include "GuildManager.h"

#include "SocialCommerce.h"

#define IS_PACKAGE(id) (id == 1 )
#define CLASS_TYPE KGSDBThread

_INT_IMPL_ON_FUNC( EVENT_REGIST_MISSION_REQ, KDB_EVENT_REGIST_MISSION_REQ )
{
    EVENT_CHECK_OUT( EVENT_REGIST_MISSION_REQ );

    KEVENT_REGIST_MISSION_ACK kPacket;
    KMissionData kMissionData;
    int nResultRegistMission = -99;
    
    kPacket.m_ItemID     = kPacket_.m_kPacket.m_ItemID;
    kPacket.m_ItemUID    = kPacket_.m_kPacket.m_ItemUID;

    if( !UpdateUserData( kPacket_.m_kUserData ) )
    {
        START_LOG( cerr, L"게임서버/DB간 사용자 정보 동기화 실패." )
            << BUILD_LOG( strLogin_ )
            << END_LOG;
    }

    if( !RegistMission( dwUID_, cCharType_, kPacket_.m_kPacket.m_ItemID, kPacket.m_kMissiondata, nResultRegistMission ) )
    {
        // 오류는 안에서 설정함.
        goto END_PROC;
    }

    SET_ERROR( NET_OK );
END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_REGIST_MISSION_ACK );
    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || NetError::GetLastNetError() == NetError::ERR_MISSION_02 )
        << L"미션 등록 로그... Msg : "  << NetError::GetLastNetErrMsg() << dbg::endl
        << L"Name : " << strLogin_ << dbg::endl
        << BUILD_LOG( kPacket_.m_kPacket.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPacket.m_ItemUID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_ItemID )
        << BUILD_LOG( kPacket.m_ItemUID )
        << BUILD_LOG( kPacket.m_kMissiondata.m_dwMissionID )
        << BUILD_LOGtm( CTime(kPacket.m_kMissiondata.m_tmRegDate) )
        << BUILD_LOGtm( CTime(kPacket.m_kMissiondata.m_tmEndDate) )
        << BUILD_LOG( kPacket.m_kMissiondata.m_mapCompletionInfo.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;


}

_INT_IMPL_ON_FUNC( EVENT_REMOVE_MISSION_REQ, KEVENT_REMOVE_MISSION )
{
    EVENT_CHECK_OUT( EVENT_REMOVE_MISSION_REQ );

    LIF( RemoveMission( dwUID_, cCharType_, kPacket_.m_dwMissionID, KMissionInfo::DEL_TYPE_USER_DEL ) );

    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_REMOVE_MISSION_ACK, kPacket_ );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L"미션 제거 로그... Msg : "<< NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG(  strLogin_ )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_dwMissionID )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_COMPLETE_MISSION_REQ )
{
    EVENT_CHECK_OUT( EVENT_COMPLETE_MISSION_REQ );

    KEVENT_COMPLETE_MISSION_ACK kPacket;
    kPacket.m_nOK                           = -10;
    kPacket.m_dwMissionID                   = kPacket_.m_dwMissionID;
    kPacket.vecDelItemInfo                  = kPacket_.vecDelItemInfo;
    kPacket.m_cPromotionLevel               = 0;
    kPacket.m_kCharacterInfo.m_cCharType    = -1;  // 클라이언트가 새 캐릭터를 인식하지 않도록 default -1.
    KMissionInfo kMissionInfo;
    std::vector<KMissionReward> vecRewards;
    int nResult = -99;
    int nOK = 0;

    if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, kPacket_.m_dwMissionID ) ) {
        SET_ERROR( ERR_MISSION_11 ); // 미션 정보 가져오기 실패
        goto END_PROC;
    }

    // 던전에서 받은 일반/에픽 미션의 경우 DB에 기록한다 (미션 개편)
    // 연계 미션(DungeonID가 -1)일경우 패스
    if ( -1 != kMissionInfo.m_nDungeonID ) {
    if ( false == InsertFinishedMission( dwUID_, cCharType_, kPacket_.m_dwMissionID ) ) {
        SET_ERROR( ERR_MISSION_39 );
        goto END_PROC;
    }
    }

    kPacket.m_cCharType = kMissionInfo.m_cCharType;

    // 미션 완료에 대한 DB처리.
    if( !CompleteMissionInfo( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, kMissionInfo.m_bUnique, KMissionInfo::DEL_TYPE_USER_COMPLETE, nResult ) ) {
        START_LOG( cerr, "미션 완료 DB처리 실패 nResult : " << nResult )
            << END_LOG;
        goto END_PROC;
    }

    kPacket.m_iExp      = kMissionInfo.m_nExp; // 이벤트 없으면  원래의 값 유지하기.
    kPacket.m_iGP       = kMissionInfo.m_nGp;

    SiKMissionManager()->GetMissionBonusExp( kPacket.m_iExp );
    SiKMissionManager()->GetMissionBonusGp( kPacket.m_iGP );

    // 보상 아이템 쿼리를 생성한다.
    if( !kMissionInfo.m_vecRewardItem.empty() )
    {
        vecRewards = kMissionInfo.m_vecRewardItem;
    }

    if( !kMissionInfo.m_vecSelectItem.empty() && kPacket_.m_dwSelection != 0 )
    {
        std::vector<KMissionReward>::const_iterator cvit;
        cvit = std::find_if( kMissionInfo.m_vecSelectItem.begin(), kMissionInfo.m_vecSelectItem.end(),
            boost::bind( &KMissionReward::m_ItemID, _1 ) ==  kPacket_.m_dwSelection );

        if( cvit != kMissionInfo.m_vecSelectItem.end() )
            vecRewards.push_back( *cvit );
    }

    if( !vecRewards.empty() )
    {
        std::vector<KMissionReward>::const_iterator cvitReward;
        for( cvitReward = vecRewards.begin() ; cvitReward != vecRewards.end() ; ++cvitReward )
        {
            std::vector<KItem> vecRewardItem;
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, cvitReward->m_ItemID, cvitReward->m_nCount, cvitReward->m_nPeriod, KItemManager::EWIT_MISSION, vecRewardItem, cvitReward->m_nGradeID ) ) {
                START_LOG( cerr, L"미션 보상 지급 실패 Name : " << strLogin_ )
                    << BUILD_LOG( cvitReward->m_ItemID )
                    << BUILD_LOG( cvitReward->m_nCount )
                    << BUILD_LOG( cvitReward->m_nPeriod ) << END_LOG;
                continue;
            }

            // 룩 아이템 변환
            if ( cvitReward->m_bIsLook ) {
                std::vector<KItem>::iterator vitItem;
                for ( vitItem = vecRewardItem.begin() ; vitItem != vecRewardItem.end() ; ++vitItem ) {
                    if( vitItem->m_cItemType == KItem::TYPE_LOOK ) {
                        continue;
                    }

                    if ( !m_kHelper.NormalItemToCoordiItem( dwUID_, vitItem->m_ItemUID, kPacket.m_cCharType, nOK ) ) {
                        START_LOG( cerr, L"DB작업중 룩 아이템 변환 실패. OK : " << nOK << L"ItemID : " << vitItem->m_ItemID << L",ItemUID : " << vitItem->m_ItemUID ) << END_LOG;
                        vitItem->DeleteItemType(KItem::TYPE_LOOK);
                        continue;
                    }

                    vitItem->AddItemType(KItem::TYPE_LOOK);
                    START_LOG( clog, L"룩 아이템으로 변환됨. ItemID : " << vitItem->m_ItemID << L",ItemUID : " << vitItem->m_ItemUID ) << END_LOG;
                }
            }

            if ( SiKGCHelper()->ProcessSnowCoin( cvitReward->m_ItemID, cvitReward->m_nCount, vecRewardItem ) ) {
                InsertSnowCoin( dwUID_, cvitReward->m_nCount );
                START_LOG( clog, L"미션 보상으로 스노우코인 지급됨. UserUID : " << strLogin_ )
                    << BUILD_LOG( cvitReward->m_ItemID )
                    << BUILD_LOG( cvitReward->m_nCount )
                    << BUILD_LOG( cvitReward->m_nPeriod ) <<  END_LOG;
            }

            // 보상 아이템 패킷에 넣기
            kPacket.m_vecInv.insert( kPacket.m_vecInv.end(), vecRewardItem.begin(), vecRewardItem.end() );
        }
    }

    // 060516. florist. 전직미션인 경우, 캐릭터 전직 시도.
    if( kPacket_.m_cCharPromotion < kMissionInfo.m_cChangeJob && kMissionInfo.m_cChangeJob > 0 ) {
        DBJIF( m_kODBC.Query( L"{ call dbo.up_character_promote ( N'%s', %d, %d ) }",
            strLogin_.c_str(),
            kMissionInfo.m_cCharType,
            kMissionInfo.m_cChangeJob ) )

            if( m_kODBC.BeginFetch() )
        {
            char cOK = -99;
            VERIFY_QUERY( m_kODBC >> cOK );

            if( cOK == 0 ) // 성공적으로 전직된 경우, 전직된 단계를 추가로 읽어낸다.
            {
                VERIFY_QUERY( m_kODBC >> kPacket.m_cPromotionLevel );
            }

            LOG_SUCCESS( cOK == 0 )
                << BUILD_LOG( strLogin_ )
                << BUILD_LOGc( kMissionInfo.m_cCharType )
                << BUILD_LOGc( kPacket.m_cPromotionLevel )
                << BUILD_LOGc( cOK )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

            m_kODBC.EndFetch();
        }
    } // end if
    else {
        kPacket.m_cPromotionLevel = 0; // 전직 하지 않을 경우..,
    }

    // 060524. florist. 새 캐릭터 획득 미션인 경우, 캐릭터 획득 시도.
/*  // 미션으로 캐릭터 획득하는 시스템은 없어졌다. 해당 부분 사용하지 않기 때문에 주석처리한다.
    if( kMissionInfo.m_cNewChar > -1 ) {
        LIF( CreateNewChar( dwUID_, strLogin_, kMissionInfo.m_cNewChar, kPacket.m_kCharacterInfo ) );
        // 기본 아이템 지급.
        if( !kPacket_.m_vecDefaultReward.empty() ) {
            std::vector<KDropItemInfo>::iterator vitReward;
            for ( vitReward = kPacket_.m_vecDefaultReward.begin(); vitReward != kPacket_.m_vecDefaultReward.end(); ++vitReward ) {
                if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
                    kMissionInfo.m_cNewChar,
                    vitReward->m_ItemID, 
                    vitReward->m_nDuration, 
                    vitReward->m_nPeriod, 
                    KItemManager::EWIT_CHAR_DEFAULT_REWARD, 
                    kPacket.m_vecInv ) ) {
                        START_LOG( cerr, L"캐릭터 생성 보상 아이템 지급 실패 UID : " << dwUID_ )
                            << BUILD_LOG( vitReward->m_ItemID )
                            << BUILD_LOG( vitReward->m_nDuration )
                            << BUILD_LOG( vitReward->m_nPeriod )
                            << END_LOG;
                        continue;
                }
            }
        }
    } // end if
*/
    if( kMissionInfo.m_dwChainMissionID > 0 )
    {
        KMissionData kData;
        int nResultRegistMission = -99;
        if( RegistMission( dwUID_, cCharType_, kMissionInfo.m_dwChainMissionID, kData, nResultRegistMission ) )
            kPacket.m_vecMissiondata.push_back( kData );
    }

    // 가이드에 해당하는 미션의 완료일 경우 FLAG_GUIDE_YET으로 설정
    if ( -1 != kPacket_.m_nGuideType ) {
        int nProvideFlag = KGuideCompleteInfo::FLAG_GUIDE_ITEM_NO_PROVIDE;
        int nCompleteFlag = KGuideCompleteInfo::FLAG_GUIDE_YET;
        UpdateSystemGuideUserData( dwUID_, kPacket_.m_nGuideType, nProvideFlag, nCompleteFlag );
    }

    SET_ERROR( NET_OK );
END_PROC:

    //정상적으로 완료 되면. 유저에게 큐잉한다.
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_COMPLETE_MISSION_ACK );
    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"미션 완료 요청 처리, Msg : " << NetError::GetLastNetErrMsg() )
        << L" Err Code : " << NetError::GetLastNetError() << dbg::endl
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwMissionID )
        << BUILD_LOGc( kPacket.m_cCharType )
        << BUILD_LOGc( kPacket.m_cPromotionLevel )
        << BUILD_LOG( kPacket.m_iExp )
        << BUILD_LOG( kPacket.m_iGP )
        << BUILD_LOG( kPacket.m_vecInv.size() )
        << BUILD_LOG( kPacket.vecDelItemInfo.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( kPacket.m_vecMissiondata.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_REQ )
{
    KEVENT_GMTOOL_AUTHENTICATION_ACK kPacket;
    kPacket.m_nOK           = NetError::ERR_UNKNOWN;
    kPacket.m_strLogin      = kPacket_.m_strLogin;
    kPacket.m_cAuthLevel    = 0;
    kPacket.m_dwUID         = 0;

    // DB에 계정이 있는지 확인한다.
    DBJIF( m_kODBC.Query( L"{ call dbo.up_query_user_passwd ( N'%s' ) }",
        kPacket.m_strLogin.c_str() ) );
    if( m_kODBC.BeginFetch() ) //사용자 존재함
    {
        std::wstring strPasswdDB;
        VERIFY_QUERY( m_kODBC >> strPasswdDB
                              >> kPacket.m_strLogin );
        m_kODBC.EndFetch();

        std::wstring strLwPasswdDB = boost::to_lower_copy( strPasswdDB );
        std::wstring strLwPasswdUser = boost::to_lower_copy( kPacket_.m_strPasswd );

        if( strLwPasswdDB != strLwPasswdUser  && !SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) ) // pw가 다른 경우 : 에러.
        {
            // 존재하는 사용자이나 passwd가 다름
            START_LOG( cerr, L"incorrect passwd. login : " << kPacket.m_strLogin )
                << BUILD_LOG( strLwPasswdDB )
                << BUILD_LOG( strLwPasswdUser )
                << END_LOG;

            kPacket.m_nOK = NetError::ERR_VERIFY_02; // 이미 존재하는 사용자 이나 패스워드가 다름
            goto GM_AUTH_END;
        }
    }
    else //사용자 존재안함
    {
        kPacket.m_nOK = NetError::ERR_VERIFY_00;   // 존재 하지 않는 사용자
        goto GM_AUTH_END;
    }
//////////////////////////////////////////////////////////////////////////
    // UID를 가져 오기 위해서..
    DBJIF( m_kODBC.Query( L"{ call dbo.UGGA_user_gameinfo_select ( N'%s', N'%s', N'%s' ) }",
        kPacket.m_strLogin.c_str(), L"0.0.0.0", SiKGameServer()->GetServerName().c_str()) );
    if( m_kODBC.BeginFetch() )
    {
        char cSex = 0;
        VERIFY_QUERY( m_kODBC >> cSex >> kPacket.m_dwUID );
        m_kODBC.EndFetch();
    }
//////////////////////////////////////////////////////////////////////////
    char cLastGrade, cCause;
    time_t tmLimit;
    int nPeriodType;
    LIF( m_kHelper.GetCommonSP().User_auth_select( kPacket.m_dwUID, cLastGrade, kPacket.m_cAuthLevel, nPeriodType, tmLimit, cCause ) );

    if ( kPacket.m_cAuthLevel < KUser::AL_ADMIN && !SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) ) {
        kPacket.m_nOK = NetError::ERR_VERIFY_06; // 운영자 권한이 없음
        goto GM_AUTH_END;
    }

    // check nickname.
/*
{ call dbo.up_query_user_nickname ( N'%s' ) }

{ call dbo.up_query_user_nickname ( @1 ) }
@1 ; Login nvarchar(20)

1 return ( @1 )
@1 ; Nickname nvarchar(24)
*/
    DBJIF( m_kODBC.Query( L"{ call dbo.up_query_user_nickname ( N'%s' ) }",
        kPacket.m_strLogin.c_str() ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_strNickName );
    }
    m_kODBC.EndFetch();

    if( kPacket.m_strNickName.empty() )
    {
        kPacket.m_nOK = NetError::ERR_VERIFY_03;   // 운영자 - 닉네임이 등록되어있지 않음.

        goto GM_AUTH_END;
    }

    DWORD dwUID = 0;
    LIF( LoginToUID( kPacket.m_strLogin.c_str(), dwUID ) );
    // 2006. 6. 26. florist. 운영툴 접속 계정이라 해도 캐릭터정보가 하나도 없으면 문제를 일으킨다.
    //캐릭터 정보 뽑기.
    DBJIF( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_select_20130304 ( %d ) }",
           dwUID) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        KCharacterInfo  kCharacterInfo;

        VERIFY_QUERY( m_kODBC
            >> kCharacterInfo.m_cCharType
            >> kCharacterInfo.m_cPromotion
            >> kCharacterInfo.m_biExp
            >> kCharacterInfo.m_iWin
            >> kCharacterInfo.m_iLose );

        //후처리
        kCharacterInfo.m_strCharName.clear();
        kCharacterInfo.m_dwLevel = SiKResultManager()->Exp_2_Level( kCharacterInfo.m_biExp );

        //읽어오는 것이 완료 되었다면 ack에 넣어주자.
        kPacket.m_mapCharacterInfo[kCharacterInfo.m_cCharType] = kCharacterInfo;

    } // end while

    kPacket.m_nOK = NetError::NET_OK;
    START_LOG( clog, L"영자툴 접속..." )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_mapCharacterInfo.size() )
        << BUILD_LOGc( kPacket.m_cAuthLevel )
        << BUILD_LOG( kPacket.m_strLogin )
        << BUILD_LOG( kPacket.m_strNickName )
        << BUILD_LOG( kPacket.m_dwUID ) << END_LOG;

GM_AUTH_END:

    QUEUING_ACK_TO_USER( EVENT_GMTOOL_AUTHENTICATION_ACK );
}


INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_TIME_LIMIT_EVENT_DATA )
{
    UpdateTimeEvent();
}

INT_IMPL_ON_FUNC( DB_EVENT_CREATE_INDIGO_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_CREATE_INDIGO_REQ );

    KDB_EVENT_CREATE_INDIGO_ACK kPacket;
    kPacket.m_dwChannelUID  = kPacket_.m_dwChannelUID;
    kPacket.m_kData.m_cOK   = -99;

    _DBJIF( m_kODBC.Query( L"{ call dbo.up_indigo_guarantee_record ( N'%s', %d ) }",
        kPacket_.m_strLogin.c_str(),
        kPacket_.m_dwUserUID ),
        goto create_indigo_end, (&m_kODBC) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_kData.m_cOK );

        if( kPacket.m_kData.m_cOK == 0 )
        {
            VERIFY_QUERY( m_kODBC >> kPacket.m_kData.m_iRP
                >> kPacket.m_kData.m_iWin
                >> kPacket.m_kData.m_iLose
                >> kPacket.m_kData.m_iRank );
        }
        m_kODBC.EndFetch();
    }

create_indigo_end:

    _LOG_CONDITION( kPacket.m_kData.m_cOK == 0, clog, cerr, L"Ret : " << (int)kPacket.m_kData.m_cOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket.m_dwChannelUID )
        << BUILD_LOG( kPacket.m_kData.m_iRP )
        << BUILD_LOG( kPacket.m_kData.m_iWin )
        << BUILD_LOG( kPacket.m_kData.m_iLose )
        << BUILD_LOG( kPacket.m_kData.m_iRank ) << END_LOG;

    QUEUING_ACK_TO_USER( DB_EVENT_CREATE_INDIGO_ACK );

}


INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_INDIGO_REQ )
{
    KIndigoData kPacket;
    kPacket.m_cOK = -99;

    _DBJIF( m_kODBC.Query( L"{ call dbo.up_indigo_update ( %d, %d, %d, %d ) }",
        kPacket_.m_dwUserUID,
        kPacket_.m_iRPChange,
        kPacket_.m_iWinChange,
        kPacket_.m_iLoseChange ),
        goto update_indigo_end, (&m_kODBC) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );

        if( kPacket.m_cOK == 0 )
        {
            VERIFY_QUERY( m_kODBC >> kPacket.m_iRP
                >> kPacket.m_iWin
                >> kPacket.m_iLose
                >> kPacket.m_iRank );
        }
        m_kODBC.EndFetch();
    }

update_indigo_end:

    LOG_CONDITION( kPacket.m_cOK == 0, clog, cerr )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_iRPChange )
        << BUILD_LOG( kPacket_.m_iWinChange )
        << BUILD_LOG( kPacket_.m_iLoseChange )
        << BUILD_LOG( kPacket.m_iRP )
        << BUILD_LOG( kPacket.m_iWin )
        << BUILD_LOG( kPacket.m_iLose )
        << BUILD_LOG( kPacket.m_iRank )
        << BUILD_LOGc( kPacket.m_cOK ) << END_LOG;

    QUEUING_ACK_TO_USER( DB_EVENT_UPDATE_INDIGO_ACK );

}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_CHECK_OPEN_TIME_REQ )
{
    short sDayOfWeek = -1;
    time_t tOpen, tClose;

    std::vector< boost::tuple<short,CTime,CTime> > vecData;

    DBJIF( m_kODBC.Query( L"{ call dbo.sp_executesql "
        L"( N'select DayOfWeek, [Open], [Close] from dbo.IndigoOpenTime with (nolock) option (maxdop 1)' ) }" ) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> sDayOfWeek >> tOpen >> tClose );

        vecData.push_back( boost::make_tuple( sDayOfWeek, CTime(tOpen), CTime(tClose) ) );
    }

    LOG_CONDITION( vecData.size() >= 7, clog, cerr )
        << BUILD_LOG( vecData.size() ) << END_LOG;

    SiKServerOpenTime()->UpdateDataFromDB( vecData );
}

INT_IMPL_ON_FUNC( EVENT_SHANGHAI_DROP_REQ )
{ // 클라이언트의 REQ가 아니므로 여기서 리턴 가능하다.
    KEVENT_SHANGHAI_DROP_ACK kPacket;

    if ( !kPacket_.m_vecDropItems.empty() ) {
        std::vector<KDropItemInfo>::iterator vit;
        for ( vit = kPacket_.m_vecDropItems.begin() ; vit != kPacket_.m_vecDropItems.end() ; ++vit ) {
            LIF( m_kHelper.InsertSingleItemJob( dwUID_, kPacket_.m_cCharType, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_SHANGHAI, kPacket.m_vecInv ) );
        }
    }

    kPacket.m_bPostItem = false;

    // 우편 등록
    if ( !kPacket_.m_vecDropPostItems.empty() ) {
        std::vector<KDropItemInfo>::iterator vit;
        for ( vit = kPacket_.m_vecDropPostItems.begin() ; vit != kPacket_.m_vecDropPostItems.end() ; ++vit ) {
            
            if ( SiKPostConfig()->IsPostEnable() )
            {
                if ( InsertShanghaiPost( dwUID_, -1, *vit ) )
                {
                    kPacket.m_bPostItem = true;
                }
            }
            else
            {
                LIF( m_kHelper.InsertSingleItemJob( dwUID_, kPacket_.m_cCharType, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_SHANGHAI, kPacket.m_vecInv ) );
        }
    }
    }

    // 미션 등록
    std::vector<DWORD>::iterator vit;
    for ( vit = kPacket_.m_vecAutoMission.begin() ; vit != kPacket_.m_vecAutoMission.end() ; ++vit ) {
        KMissionData kMissionData;
        int nResultRegistMission = -99;

        if ( !RegistMission( dwUID_, kPacket_.m_cCharType, *vit, kMissionData, nResultRegistMission ) ) {
            START_LOG( cerr, L"Mission Register Fail, Name : " << strLogin_ )
                << BUILD_LOG( *vit )
                << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
            continue;
        }
        kPacket.m_vecAutoMission.push_back( kMissionData );
    }

    if ( !kPacket.m_vecInv.empty() || !kPacket.m_vecAutoMission.empty() || kPacket.m_bPostItem ) {
        //상하이 드랍 성공한 것에 대한 고유 아이디를 넘겨줍시다.
        QUEUING_ACK_TO_USER( EVENT_SHANGHAI_DROP_ACK );
    }

    if ( kPacket.m_bPostItem ) 
    {
        std::vector<KPostItemInfo> vecPostItem;
        vecPostItem.clear();

        if ( false == GetNewPostLetterInfo( strLogin_, dwUID_, kPacket_.m_cCharType, vecPostItem ) ) 
        {
            return;
        }

        START_LOG( clog, L"신규 우편 전달.. LoginID : " << strLogin_ )
            << BUILD_LOG( vecPostItem.size() ) << END_LOG;

        _QUEUING_ACK_TO_USER( EVENT_NEW_POST_LETTER_INFO_ACK, vecPostItem );
    }
}

_INT_IMPL_ON_FUNC( EVENT_SHANGHAI_CHECK_REQ, DWORD ) // 상하이 드랍얻어오기.
{
    KncBitStream kPacket;
    kPacket.clear();
    int nOK = -10;

    //{ call dbo.up_select_check_list ( @1 ) }
    //@1 : @iLoginUID int
    //리턴 @vbList(만약 해당 유저가 존재하지 않는다면 리턴값 없음)
    // 유저 정보를 DB에서 가져온다.
    DBJIF( m_kODBC.Query( L"{ call dbo.up_select_check_list ( %d ) }",
        kPacket_ ) );

    if( m_kODBC.BeginFetch() )
    {
        KSerializer ks;
        KSerBuffer kbuff;
        VERIFY_QUERY( m_kODBC >> kbuff );

        ks.BeginReading( &kbuff );
        ks.Get( kPacket );
        ks.EndWriting();
        m_kODBC.EndFetch();
        nOK = 0;
    }
    else
    {
        m_kODBC.EndFetch();
        //{ call dbo.up_insert_check_list ( @iLoginUID, @vbList, [@bModify] ) }
        //@iLoginUID : 유저의 LoginUID
        //@vbList : 48바이트짜리 바이너리
        //@bModify : 0일 경우 insert, 1일 경우 update를 함. [기본값은 0]
        //
        //리턴 @iOK
        //0(성공)
        //-1(해당 유저가 없음)
        //-2(insert를 하려고 하는데 이미 데이터가 존재함)
        //-3(update를 하려고 하는데 데이터가 존재하지 않음)
        //-11이하(트랜잭션 에러)

        KSerializer ks;
        KSerBuffer buff;
        ks.BeginWriting( &buff );
        ks.Put( kPacket );
        ks.EndWriting();

        DBJIF( m_kODBC.QueryWithBlob( buff, L"{ call dbo.up_insert_check_list ( %d, ? ) }",
            kPacket_ ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();
    }

    QUEUING_ACK_TO_USER( EVENT_SHANGHAI_CHECK_ACK );
    LOG_CONDITION( nOK == 0 , clog, cerr )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kPacket.GetBitSize() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_RECOMMENDER_REQ, KPacketNameOK )
{
    // 2  추천인 닉네임이 비어 있음
    // 1  비추하였음
    // 0  정상적으로 추천인이 등록 되었음/하지 않았음
    // -1 추천하려는 사람의 정보가 없음 --> 이녀석인 경우 심각한 문제이다. 넘어가시오.
    // -2 이미 추천하였습니다.(이중접속등으로 인해서 다른넘이 먼저 등록했다. 일수 있음)
    // -3 다시 추천하세욤.(대상이 존재하지 않음)

    // 추천인 등록하지 않겠음    
    DWORD dwReconnenderUID = 0;
    DWORD dwReconnendeeUID = 0;
    int kPacket = -99;

    //유저의 로긴을 UID로 변환
    LIF( m_kHelper.LoginToUserUID( strLogin_, dwReconnenderUID ) );

    // 대상의 닉네임을 UID로 변환
    if( kPacket_.m_nOK != -1 )
    {
        if ( false == m_kHelper.NickToUserUID( kPacket_.m_strName, dwReconnendeeUID ) ) {
            kPacket = -3;
            QUEUING_ACK_TO_USER( EVENT_RECOMMENDER_ACK );
            return;
        }
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.up_recommender_check_status ( %d ) }",
        dwReconnenderUID ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket );
    }
    m_kODBC.EndFetch();

    if( kPacket != -1 )
    {
        START_LOG( cerr,L"추천인 등록 하면 안되는 상황인데 호출이 되었다. name : " << strLogin_ )
            << BUILD_LOG( kPacket ) << END_LOG;
    }


    DBJIF( m_kODBC.Query( L"{ call dbo.up_recommender_insert ( %d, %d ) }",
        dwReconnenderUID, dwReconnendeeUID ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket );
    }
    m_kODBC.EndFetch();

    if( kPacket_.m_nOK == -1 && kPacket == 0 ) // 비추 하였음.
        kPacket = 1;
    QUEUING_ACK_TO_USER( EVENT_RECOMMENDER_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_ACCUMULRATION_TIME_NOT )
{
    //call dbo.up_LimitGamePlay_Online @iLoginUID int, @iThresholdTime int, @iFullTime int
    //리턴값 select return @iOK, @iOnlineTime, @iOfflintTime
    //@iOK 설명
    //0(성공, 데이터 읽어가도 좋음)
    //-1(Users테이블에 유저 정보가 없음)
    //-11이하(트랜잭션 에러)
    //부가 설명 : 유저가 게임 시작할적에 읽어주면 되삼

    KEVENT_ACCUMULRATION_TIME_NOT kPacket;
    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.up_LimitGamePlay_Online ( %d, %d, %d ) }",
        dwUID_, SiKFatigueSystem()->GetThresholdTime(), SiKFatigueSystem()->GetClearAccOfflineTime() ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK >> kPacket.m_nAccOnlineTime >> kPacket.m_nAccOfflineTime );
    }
    m_kODBC.EndFetch();

    if( nOK == 0 )
    {
        QUEUING_ACK_TO_USER( EVENT_ACCUMULRATION_TIME_NOT );
    }

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kPacket.m_nAccOnlineTime )
        << BUILD_LOG( kPacket.m_nAccOfflineTime ) << END_LOG;
}

INT_IMPL_ON_FUNC(DB_EVENT_KAIRO_REQ)
{
    EVENT_CHECK_OUT(DB_EVENT_KAIRO_REQ);
    std::vector<KDropItemInfo>::iterator vit;
    std::vector<KItem>::iterator vitReward;
    std::vector<KItem> vecReward;
    vecReward.clear();
    KEVENT_KAIRO_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR(ERR_UNKNOWN);

    kPacket.m_vecItem = kPacket_.m_vecItem;

    for (vit = kPacket_.m_vecPostItem.begin(); vit != kPacket_.m_vecPostItem.end(); ++vit) {
        if (m_kHelper.InsertSingleItemJob(dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_KAIRO, kPacket.m_vecItem, vit->m_cGradeID, -1, vit->m_cEnchantLevel) == false) {
            START_LOG(cerr, L"카이로 아이템 DB넣기 작업 실패. UID : " << dwUID_)
                << BUILD_LOG(vit->m_ItemID)
                << END_LOG;
            SET_ERR_GOTO(ERR_KAIRO_03, END_PROC);
        }
    }

    if (!CheckItemDuration(dwUID_, kPacket_.m_kReqItemInfo.m_ItemUID, kPacket_.m_kReqItemInfo.m_nInitCount, kPacket_.m_kReqItemInfo.m_nCount)) {
        START_LOG(clog, L"카이로 상자 아이템 창고 수량 동기화. Name : " << strLogin_)
            << BUILD_LOG(dwUID_)
            << BUILD_LOG(kPacket_.m_kReqItemInfo.m_ItemUID)
            << BUILD_LOG(kPacket_.m_kReqItemInfo.m_nInitCount)
            << BUILD_LOG(kPacket_.m_kReqItemInfo.m_nCount)
            << END_LOG;
    }

    if (!UseCountItem(dwUID_, kPacket_.m_kReqItemInfo.m_ItemUID, 1)) {
        START_LOG(cerr, L"카이로 상자 개수 동기화 실패. Name : " << strLogin_) << END_LOG;
        SET_ERROR(ERR_KAIRO_04);
    }
    else {
        SET_ERROR(NET_OK);
    }

    kPacket_.m_kReqItemInfo.m_nCount -= 1;
    kPacket_.m_kReqItemInfo.m_nCount = std::max<int>(kPacket_.m_kReqItemInfo.m_nCount, 0);

    if (IS_CORRECT(ERR_KAIRO_04)) {
        kPacket_.m_kReqItemInfo.m_nCount = 0;
    }

    kPacket.m_kReqItemInfo = kPacket_.m_kReqItemInfo;
    kPacket.m_dwType = kPacket_.m_dwType;

    if (kPacket_.m_dwType == Kairo::RT_STRENGTH) {
        bool bStrengthEnable = false;
        bool bEquipEnable = false;
        for (vitReward = kPacket.m_vecItem.begin(); vitReward != kPacket.m_vecItem.end(); ++vitReward) {
            if (vitReward->m_ItemID == Kairo::IG_STRENGTH && !bStrengthEnable) {
                kPacket.m_kStrengthItem = *vitReward;
                bStrengthEnable = true;
            }
            else {
                kPacket.m_kEquipItem = *vitReward;
                bEquipEnable = true;
            }
        }
        if (bStrengthEnable && bEquipEnable) {
            kPacket.m_kStrengthItem.m_EnchantEquipItemUID = kPacket.m_kEquipItem.m_ItemUID;
            kPacket.m_kStrengthItem.m_cEnchantEquipGradeID = kPacket.m_kStrengthItem.m_cGradeID;
            kPacket.m_kEquipItem.m_EnchantEquipItemUID = kPacket.m_kStrengthItem.m_ItemUID;
            kPacket.m_kEquipItem.m_cEnchantLevel = kPacket.m_kStrengthItem.m_cEnchantLevel;
            kPacket.m_kEquipItem.m_cEnchantEquipGradeID = kPacket.m_kStrengthItem.m_cGradeID;

            if (!UpdateStrengthItemReward(dwUID_, kPacket.m_kStrengthItem, kPacket.m_kEquipItem, kPacket_.m_listAttributeList, cCharType_)) {
                START_LOG(cerr, L"카이로 아이템 강화석 갱신실패. UID : " << dwUID_) << END_LOG;
                goto END_PROC;
            }
        }
        else {
            START_LOG(cerr, L"카이로 아이템 강화석 정보 넣기 실패.") << END_LOG;
            SET_ERR_GOTO(ERR_KAIRO_06, END_PROC);
        }
    }

    for (vit = kPacket_.m_vecPostItem.begin(); vit != kPacket_.m_vecPostItem.end(); ++vit) {
        LIF(InsertKairoRewardInfo(dwUID_, kPacket_.m_kReqItemInfo.m_ItemID, kPacket_.m_dwType, *vit));
    }

    SET_ERROR(NET_OK);

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket_.m_vecPostItem.size())
        << BUILD_LOG(kPacket_.m_dwType)
        << BUILD_LOG(kPacket_.m_listAttributeList.size())
        << END_LOG;

    _QUEUING_ACK_TO_USER(EVENT_KAIRO_ACK, kPacket);

}

INT_IMPL_ON_FUNC(DB_EVENT_KAIRO_ONESHOT_REQ)
{
    EVENT_CHECK_OUT(DB_EVENT_KAIRO_ONESHOT_REQ);

    std::vector<KDropItemInfo>::iterator vit;
    std::vector<KItem>::iterator vitReward;
    std::vector<KItem> vecReward;
    vecReward.clear();
    KEVENT_KAIRO_ONESHOT_ACK kPacket;
    kPacket.m_nOK = -99;

    SET_ERROR(ERR_UNKNOWN);

    kPacket.m_vecItem = kPacket_.m_vecItem;

    for (vit = kPacket_.m_vecPostItem.begin(); vit != kPacket_.m_vecPostItem.end(); ++vit) {
        if (false == m_kHelper.InsertSingleItemJob(dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_KAIRO, kPacket.m_vecItem, vit->m_cGradeID, -1, vit->m_cEnchantLevel)) {
            START_LOG(cerr, L"카이로 아이템 DB넣기 작업 실패. UID : " << dwUID_)
                << BUILD_LOG(vit->m_ItemID)
                << END_LOG;

            SET_ERR_GOTO(ERR_KAIRO_03, END_PROC);
        }
    }

    int nDecreaseCount = 1;

    if (1 < kPacket_.m_iCount) {
        nDecreaseCount = kPacket_.m_iCount;
    }

    if (false == UpdateCountItem(dwUID_, kPacket_.m_kReqItemInfo.m_ItemUID, -nDecreaseCount)) {
        START_LOG(cerr, L"카이로 상자 개수 동기화 실패. Name : " << strLogin_) << END_LOG;

        SET_ERROR(ERR_KAIRO_04);
    }

    kPacket_.m_kReqItemInfo.m_nCount -= kPacket_.m_iCount;
    kPacket_.m_kReqItemInfo.m_nCount = std::max< int >(kPacket_.m_kReqItemInfo.m_nCount, 0);

    if (true == IS_CORRECT(ERR_KAIRO_04)) {
        kPacket_.m_kReqItemInfo.m_nCount = 0;
    }

    kPacket.m_kReqItemInfo = kPacket_.m_kReqItemInfo;
    kPacket.m_dwType = kPacket_.m_dwType;

    for (vit = kPacket_.m_vecPostItem.begin(); vit != kPacket_.m_vecPostItem.end(); ++vit) {
        LIF(InsertKairoRewardInfo(dwUID_, kPacket_.m_kReqItemInfo.m_ItemID, kPacket_.m_dwType, *vit));
    }

    SET_ERROR(NET_OK);

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS(NetError::GetLastNetError() == NetError::NET_OK)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket_.m_vecPostItem.size())
        << BUILD_LOG(kPacket_.m_dwType)
        << BUILD_LOG(kPacket_.m_listAttributeList.size())
        << END_LOG;

    _QUEUING_ACK_TO_USER(EVENT_KAIRO_ONESHOT_ACK, kPacket);
}

_INT_IMPL_ON_FUNC( EVENT_CREATE_PET_REQ, KDB_EVENT_CREATE_PET )
{
    EVENT_CHECK_OUT( EVENT_CREATE_PET_REQ );

    std::wstring wstrPetName = boost::replace_all_copy( kPacket_.m_kPacket.m_strPetName, L"'", L"''" );
    int nOK = InsertPetInfo( dwUID_, kPacket_.m_kPacket.m_cCharType, kPacket_.m_kPacket.m_dwPetUID, kPacket_.m_kPacket.m_dwPetID, wstrPetName, kPacket_.m_kPetCreateInfo.m_cPromotion, kPacket_.m_kPetCreateInfo.m_iSatiation );
    // 생성 실패)
    if( nOK != 0 )
    {
        switch( nOK ) {
            case -1: SET_ERROR( ERR_PET_03 ); break;    //-1(유저가 존재하지 않음)
            case -2: SET_ERROR( ERR_PET_04 ); break;    //-2(이 아이템이 펫 아이템일까요?)
            case -3: SET_ERROR( ERR_PET_01 ); break;    //-3(펫 아이템이 존재하지 않음)
            case -5: SET_ERROR( ERR_PET_02 ); break;    //-5(신규 생성하려는 팻이 이미 존재함)
            case -6: SET_ERROR( ERR_PET_05 ); break;    //-6(캐릭터가 존재하지 않음)
            default:
                SET_ERROR( ERR_UNKNOWN ); 
        }
        START_LOG( cerr, L" 펫 생성 중 오류 : " << NetError::GetLastNetErrMsg() )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        goto end_create_pet;
    }

    SetCharacterPetEquip( dwUID_, strLogin_, kPacket_.m_kPacket.m_dwPetUID, kPacket_.m_kPacket.m_cCharType );

    // 생성 성공 했으면 셋팅하자.
    kPacket_.m_kPetCreateInfo.m_dwUID           = kPacket_.m_kPacket.m_dwPetUID;
    kPacket_.m_kPetCreateInfo.m_dwID            = kPacket_.m_kPacket.m_dwPetID;
    kPacket_.m_kPetCreateInfo.m_strName         = kPacket_.m_kPacket.m_strPetName;
    kPacket_.m_kPetCreateInfo.m_cCharType       = kPacket_.m_kPacket.m_cCharType;
    kPacket_.m_kPetCreateInfo.m_vecEquipItem.clear();
    for( int i = 0; i <= 2; i++ )
        kPacket_.m_kPetCreateInfo.m_mapInitExp.insert( std::make_pair( i , SiKPetHelper()->GetPetMinExp() ) );
    kPacket_.m_kPetCreateInfo.m_dwEXP           = SiKPetHelper()->GetPetMinExp();
    kPacket_.m_kPetCreateInfo.m_iInitSatiation  = kPacket_.m_kPetCreateInfo.m_iSatiation;
    kPacket_.m_kPetCreateInfo.m_iLevel          = SiKPetHelper()->GetPetLevel( kPacket_.m_kPetCreateInfo.m_dwEXP );
    kPacket_.m_kPetCreateInfo.m_nHatchingID     = SiKPetHelper()->GetNextID( kPacket_.m_kPetCreateInfo.m_dwID );
    kPacket_.m_kPetCreateInfo.m_kCostume.m_dwID = 0;
    kPacket_.m_kPetCreateInfo.m_kCostume.m_dwUID = 0;
    kPacket_.m_kPetCreateInfo.m_kCostume.m_cItemType = -1;

    // 보상이 없는 펫도 있기 때문에 오류 처리를 하지 않는다.
    m_kHelper.InsertPetItem( dwUID_, kPacket_.m_kPacket.m_cCharType, kPacket_.m_kPacket.m_dwPetID, 0, kPacket_.m_vecRewardItem );

    SET_ERROR( NET_OK );

end_create_pet:
    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_CREATE_PET_ACK, kPacket_ );

    LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_DELETE_PET_REQ, KDB_EVENT_DELETE_PET )
{
    EVENT_CHECK_OUT( EVENT_DELETE_PET_REQ );
    kPacket_.m_iGP = 0;
    kPacket_.m_vecRewardItem.clear();
    __int64 waitUID = KItem::INVALID_ITEM;
    SET_ERROR( ERR_UNKNOWN );

    if( !UpdateUserData( kPacket_.m_kUserData ) )
    {
        START_LOG( cerr, L"게임서버/DB간 사용자 정보 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        SET_ERR_GOTO( ERR_DB_COMMON_03, end_del_pet );
    }

    // 펫 데이터 제거.
    if ( !m_kHelper.RemovePet( dwUID_, kPacket_.m_kPetDeleteInfo.m_dwID, kPacket_.m_kPetDeleteInfo.m_dwUID ) ) {
        goto end_del_pet;
    }

    // 펫 시민권 팔아서 보상 받기
    if ( !m_kHelper.SellItem( dwUID_, kPacket_.m_kPetDeleteInfo.m_dwID, kPacket_.m_kPetDeleteInfo.m_dwUID, kPacket_.m_iGP, waitUID, cCharType_ ) ) {
        SET_ERR_GOTO( ERR_PET_08, end_del_pet );
    }

    if ( waitUID != KItem::INVALID_ITEM ) {
        m_kHelper.UpdateWaitItemToEnabled( waitUID );
        LIF( m_kHelper.InsertSingleWaitItemToInvenOrVDepot( dwUID_, cCharType_, waitUID, KItemManager::EWIT_ITEM_SELL, kPacket_.m_vecRewardItem ) );
    }

    SET_ERROR( NET_OK );

end_del_pet:
    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_DELETE_PET_ACK, kPacket_ );

    LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_iGP )
        << BUILD_LOG( kPacket_.m_vecRewardItem.size() )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_CHANGE_PET_NAME_REQ )
{

    DBJIF( m_kODBC.Query( L"{ call dbo.gup_pet_rename ( %I64d, %d, %d, N'%s' ) }",
        kPacket_.m_dwPetUID, dwUID_, kPacket_.m_dwPetID, kPacket_.m_strPetNickName.c_str() ) );

    int nOK = -99;

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    if( nOK != 0 )
    {
        switch( nOK )
        {
        case -1: SET_ERROR( ERR_PET_03 ); break;
        case -2: SET_ERROR( ERR_PET_04 ); break;
        case -3: SET_ERROR( ERR_PET_06 ); break;
        case -5: SET_ERROR( ERR_PET_07 ); break;
            // 현재 사용하지 않는 에러이지만 에러처리는 해둠 혹시 모르니
        case -6: SET_ERROR( ERR_PET_22 ); break;
        }
        goto END_PROC;
    }

    SET_ERROR( NET_OK );

END_PROC: //신재익 때문이다. 다시 정리 해야 한다.
    kPacket_.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == 0 )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_dwPetUID )
        << BUILD_LOG( kPacket_.m_strPetNickName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    if( kPacket_.m_nOK != NetError::NET_OK )
        kPacket_.m_nOK = 2;

    _QUEUING_ACK_TO_USER( EVENT_CHANGE_PET_NAME_ACK, kPacket_ );

}


INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_LOUD_MSG )
{
    UpdateLoudMsg();
}

_INT_IMPL_ON_FUNC( EVENT_USE_INVENTORY_EXTEND_REQ, KExtendItem )
{
    EVENT_CHECK_OUT( EVENT_USE_INVENTORY_EXTEND_REQ );

    kPacket_.m_nOK = UpdateCharInvenCapacity( dwUID_, cCharType_, kPacket_.m_dwSlotNum, kPacket_.m_nInvenCapacity );

    switch( kPacket_.m_nOK )
    {
    case 0: 
        SET_ERROR( NET_OK );
        break;
    case -1: SET_ERROR( ERR_INVEN_04 ); // 해당 유저가 존재하지 않음
        break;         
    default:
        SET_ERROR( ERR_DB_COMMON_00 ); // 트랜잭션 에러
        START_LOG( cerr, L"트랜잭션 에러. return value : " << kPacket_.m_nOK ) << END_LOG;
    }

    if ( NetError::GetLastNetError() == NetError::NET_OK ) {
        // 동기화전 InitCount, Count값 변동값 있는지 체크.
        if( !CheckItemDuration( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_kItem.m_nInitCount, kPacket_.m_kItem.m_nCount ) ) {
            START_LOG( clog, L"인벤토리 확장 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
                << BUILD_LOG( kPacket_.m_kItem.m_nInitCount )
                << BUILD_LOG( kPacket_.m_kItem.m_nCount )
                << END_LOG;
        }

        if ( !UseCountItem( dwUID_, kPacket_.m_kItem.m_ItemUID, 1 ) ) {
            kPacket_.m_kItem.m_nCount = 0;
            SET_ERROR( ERR_INVEN_13 );
            
            START_LOG( cerr, L"인벤토리 확장 아이템 DB 동기화 실패, User UID : " << dwUID_ )
                << BUILD_LOG( kPacket_.m_itemID )
                << END_LOG;
        }
        else { // 아이템 DB 동기화 성공
            kPacket_.m_kItem.m_nCount -= 1;
            kPacket_.m_kItem.m_nCount = std::max<int>( kPacket_.m_kItem.m_nCount, 0 );
        }
    }

    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_USE_INVENTORY_EXTEND_ACK, kPacket_ ); // 로그는 유저에서 뿌리자.
}

INT_IMPL_ON_FUNC( EVENT_STAT_USER_HISTORY_NOT )
{
    //call dbo.sup_insert_user_trace (LoginUID int, Data varbinary(8000))
    //return 1 loop select (@iOK)
    //@iOK : 0(성공), -11이하(트랜잭션 에러)

    if( kPacket_.m_vecUserHistory.empty() ) return;

    if( kPacket_.m_vecUserHistory.size() > 200 )
        kPacket_.m_vecUserHistory.resize( 200 );

    KSerializer ks;
    KSerBuffer buff;
    ks.BeginWriting( &buff );
    ks.Put( kPacket_ );
    ks.EndWriting();

    _DBJIF( m_kODBCStat.QueryWithBlob( buff, L"{ call dbo.sup_insert_user_trace ( %d, ? ) }",
        dwUID_ ), return, (&m_kODBCStat) );

    m_kODBCStat.BeginFetch();
    m_kODBCStat.EndFetch();

    START_LOG( clog, L" 유저 발자취 기록 완료." ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_CLIENT_INFO )
{
    if( kPacket_.m_strIP.size() > 15 )
        kPacket_.m_strIP.resize( 15 );
    boost::replace_all( kPacket_.m_strIP, L"'", L"''" );


    if( kPacket_.m_strOSType.size() > 99 )
        kPacket_.m_strOSType.resize( 99 );
    boost::replace_all( kPacket_.m_strOSType, L"'", L"''" );

    if( kPacket_.m_strCPU.size() > 99 )
        kPacket_.m_strCPU.resize( 99 );
    boost::replace_all( kPacket_.m_strCPU, L"'", L"''" );

    if( kPacket_.m_strGraphicChip.size() > 99 )
        kPacket_.m_strGraphicChip.resize( 99 );
    boost::replace_all( kPacket_.m_strGraphicChip, L"'", L"''" );

    if( kPacket_.m_strDXversion.size() > 99 )
        kPacket_.m_strDXversion.resize( 99 );
    boost::replace_all( kPacket_.m_strDXversion, L"'", L"''" );

    /*
    L"{ call dbo.CSGAComputerSpec_insert_20130130 ( %d, N'%s', N'%s', N'%s', N'%s', N'%s', %d, %d, %d, %d, %d ) }"

    { call dbo.CSGAComputerSpec_insert_20130130 ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11 ) }
    @1 : @iLoginUID_input        int
    @2 : @strIP_input            nvarchar( 15 )
    @3 : @strOSName_input        nvarchar( 100 )
    @4 : @strDXVersionName_input nvarchar( 100 )
    @5 : @strCPUName_input       nvarchar( 100 )
    @6 : @strVideoName_input     nvarchar( 100 )
    @7 : @iCPUCore_input         int
    @8 : @iCPUClock_input        int
    @9 : @iMainMemory_input      int
    @10: @iVideoMemory_input     int
    @11: @iJoystick_input        int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 업음
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.CSGAComputerSpec_insert_20130130 ( %d, N'%s', N'%s', N'%s', N'%s', N'%s', %d, %d, %d, %d, %d ) }",
                                dwUID_, kPacket_.m_strIP.c_str(), kPacket_.m_strOSType.c_str(), kPacket_.m_strDXversion.c_str(),
                                kPacket_.m_strCPU.c_str(), kPacket_.m_strGraphicChip.c_str(), kPacket_.m_nCPUCore, kPacket_.m_nCPUClock,
                                kPacket_.m_iMainMemory,  kPacket_.m_iVMemory, (kPacket_.m_bjoystick)?(1):(0) ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L" 클라이언트 사양 조사.." )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( kPacket_.m_strOSType )
        << BUILD_LOG( kPacket_.m_strCPU )
        << BUILD_LOG( kPacket_.m_nCPUCore )
        << BUILD_LOG( kPacket_.m_nCPUClock )
        << BUILD_LOG( kPacket_.m_iMainMemory )
        << BUILD_LOG( kPacket_.m_strGraphicChip )
        << BUILD_LOG( kPacket_.m_iVMemory )
        << BUILD_LOG( kPacket_.m_bjoystick )
        << BUILD_LOG( kPacket_.m_strDXversion ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_DB_REPETITION_PROVIDE_REQ )
{
    START_LOG( clog, L"한시간 이벤트 DB에 서 받음." )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_strNick )
        << BUILD_LOG( kPacket_.m_dwEventUID )
        << BUILD_LOG( kPacket_.m_nCommentID ) << END_LOG;
    KEVENT_QUERY_INVENTORY_INFO_ACK kPacket;
    std::vector<KItem> vecRewardItems;
    //{ call dbo.TEG_timer_event_item_select ( IssueUID int ) }
    //n return
    //( ItemID int )
    //( Factor int )
    SET_ERROR( ERR_UNKNOWN );
    //std::vector< std::pair<int,int> > m_vecItems;
    //std::wstringstream stmDropItems; // 사용하지 않는 지역변수 주석처리.
    
    DBJIF( m_kODBC.Query( L"{ call dbo.TEG_timer_event_item_select ( %d ) }", kPacket_.m_dwEventUID ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KItem kItem;
        VERIFY_QUERY( m_kODBC >> kItem.m_ItemID >> kItem.m_nCount );
        kItem.m_nPeriod = -1;
        // TODO : 기간 정보도 받을수 있도록 수정 필요.
        vecRewardItems.push_back( kItem );
    }
    m_kODBC.EndFetch();

    //{ call TEG_timer_event_update ( IssueUID int, LoginUID int ) }
    //1 return
    //    ( OK int )
    //    OK
    //    0 : 성공
    //    -1 : 해당 이벤트를 읽어간 적이 없음
    //    -2 : 이미 아이템이 지급된 상태임
    //    -101이하 : 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call TEG_timer_event_update ( %d, %d ) }", kPacket_.m_dwEventUID, kPacket_.m_dwUID ) );
    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    if( nOK != 0 )
    {
        // 에러
        START_LOG( cerr, L"시간 드랍 이벤트 에러")
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( kPacket_.m_dwEventUID )
            << BUILD_LOG( kPacket_.m_dwUID )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    std::vector<KItem>::iterator vit;
    for( vit = vecRewardItems.begin() ; vit != vecRewardItems.end() ; ++vit )
    {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, KItemManager::EWIT_EVENT_REPETITION_PROVIDE, kPacket.m_vecInv ) );
    }


    //전체 메세지 준비하자..
    std::wstring strMsg;
    if( SiKLoudMessage()->GetLoudMsgByComment( kPacket_.m_nCommentID, kPacket_.m_strNick, strMsg, kPacket_.m_nLanguageType ) )
    {
        if( kPacket.m_vecInv.empty() && !strMsg.empty() ) // 아이템 줄게 없으면 기본적으로 메세지 박스 패킷을 보낸다.
        {
            _QUEUING_ACK_TO_USER( EVENT_MSG_NOT, strMsg );
            // 메세지 박스만 보여줄것이기 땜시롱.....
            boost::replace_all( strMsg, L"_", L" " );
        }
        kPacket.m_strMsg = strMsg;
    }

    if( !kPacket.m_vecInv.empty() ) // 줄게 있을때만 보낸다..
        QUEUING_ACK_TO_USER( EVENT_REPETITION_PROVIDE_NOT );
}

_INT_IMPL_ON_FUNC( EVENT_TRANSFORMATION_PET_REQ, KEVENT_DB_TRANSFORMATION_PET_REQ )
{
    //call dbo.gup_pet_promotion (PetUID bigint, LoginUID int, PetID int, PetName nvarchar(20), Promotion tinyint )
    //PetName : 유저가 지정한 펫의 이름
    //Promotion : 진화/퇴화 레벨(하향되어도 상관없음)
    //select 1 loop return @iOK int
    //0 :       성공
    //-1~-3 :   펫관련 공통 에러(나머지 저장 프로시저 찾으면 있을 것임)
    //-5 :      펫이 생성되지 않았음
    //-6 :      펫 전직레벨이 이상함
    //-11이하 : 트랜잭션 에러
    KEVENT_TRANSFORMATION_PET_ACK kPacket;
    kPacket.m_kPetInfo.Init();
    int nOK = 0;
    int nTransFormFactor = 0;
    KPetInfo kPetInfo;
    kPetInfo.Init();

    if( !UpdatePetData( kPacket_.m_kPetData.m_mapPetInfo, kPacket_.m_kPetData.m_mapCharacterInfo, dwUID_, strLogin_ ) )
    {
        START_LOG( cerr, L"게임서버/DB간 펫 정보 동기화 실패." )
            << BUILD_LOG( strLogin_ )
            << END_LOG;
    }

    std::map<GCITEMUID, KPetInfo>::iterator mit;
    mit = kPacket_.m_kPetData.m_mapPetInfo.find( kPacket_.m_kPacket.m_dwPetUID );

    if( mit == kPacket_.m_kPetData.m_mapPetInfo.end() ) {
        SET_ERROR( ERR_PET_07 );
        goto END_PROC;
    }
    kPetInfo = mit->second;

    nTransFormFactor = (kPacket_.m_kPacket.m_ePTT == KUser::PTT_EVOLUTION ? 1 : -1);
    SET_ERROR( ERR_UNKNOWN );
    DBJIF( m_kODBC.Query( L"{ call dbo.gup_pet_promotion ( %I64d, %d, %d, N'%s', %d ) }",
        kPetInfo.m_dwUID, dwUID_, kPetInfo.m_dwID,
        kPacket_.m_kPacket.m_strPetName.c_str(), kPetInfo.m_cPromotion+nTransFormFactor ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK ) {
    case 0: SET_ERROR( NET_OK ); break;
    case -1: SET_ERROR( ERR_PET_03 ); break;    // -1(유저가 존재하지 않음)
    case -2: SET_ERROR( ERR_PET_04 ); break;    // -2(이 아이템이 펫 아이템일까요?)
    case -3: SET_ERROR( ERR_PET_01 ); break;    // -3(펫 아이템이 존재하지 않음)
    case -5: SET_ERROR( ERR_PET_07 ); break;    // -5(펫 정보가 존재하지 않음)
    case -6: SET_ERROR( ERR_PET_28 ); break;    // -6(펫 전직 레벨 오류)
    default:
        SET_ERROR( ERR_UNKNOWN );
    }
    if( NetError::GetLastNetError() != NetError::NET_OK )
        goto END_PROC;

    m_kHelper.InsertPetItem( dwUID_, kPacket_.m_cCurrentChar, kPetInfo.m_dwID, kPetInfo.m_cPromotion + nTransFormFactor, kPacket.m_vecRewardItem );

    // ACK에 적용하기.
    // 전직 변경에 때른 현재 경험치 셋팅 않했음.
    kPacket.m_kPetInfo                  = kPetInfo;
    kPacket.m_kPetInfo.m_strName        = kPacket_.m_kPacket.m_strPetName;
    kPacket.m_kPetInfo.m_cPromotion     += nTransFormFactor;
    kPacket.m_dwItemID                  = kPacket_.m_kPacket.m_dwItemID; // ??
    kPacket.m_kPetInfo.m_kCostume.m_dwID        = 0;
    kPacket.m_kPetInfo.m_kCostume.m_dwUID       = 0;
    kPacket.m_kPetInfo.m_kCostume.m_cItemType   = -1;

    SET_ERROR( NET_OK );

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_TRANSFORMATION_PET_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_LOAD_POINTSYSTEM_INFO_REQ )
{
    KPointSystemInfo kPacket;
    CTime tmCurrent = CTime::GetCurrentTime();
    CTime tmCurrentDay( tmCurrent.GetYear(), tmCurrent.GetMonth(), tmCurrent.GetDay(), 0, 0, 0 );
    /*
    L"{ call dbo.PBGAPointBag_select_20130523 ( %d, %d ) }"

    { call dbo.PBGAPointBag_select_20130523 ( @01, @02 ) }
    @01iLoginUID_input int
   ,@02iCharType_input int

    n return ( @01, @02, @03 )
    @01: Point    int
    @02: PlayTime int
    @03: RegDateD smalldatetime
    */

    if( !m_kODBC.Query( L"{ call dbo.PBGAPointBag_select_20130523 ( %d, %d ) }", dwUID_, cCharType_ ) ) {
        START_LOG( cerr, L"GC포인트시스템 로드실패[dbo.gup_pointbag_get] " )
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( (&m_kODBC)->m_szDiagMsg )
            << BUILD_LOG( (wchar_t*)((&m_kODBC)->m_sqlState) )
            << BUILD_LOG( (&m_kODBC)->m_strLastQuery ) << END_LOG;
        return;
    }

    if( m_kODBC.BeginFetch() ) {
        kPacket.m_nOK = -99;
        int nPoint = -1;
        int nAccTime = -1;
        time_t tmDate = time_t(NULL);

        VERIFY_QUERY( m_kODBC >> nPoint >> nAccTime >> tmDate );
        m_kODBC.EndFetch();

        if( nPoint == -1 && nAccTime == -1 ) {
            START_LOG( cerr, L"유저가 존재하지 않음")
                << BUILD_LOG( strLogin_ )
                << BUILD_LOG( nAccTime )
                << BUILD_LOG( nPoint ) << END_LOG;
            QUEUING_ACK_TO_USER( EVENT_LOAD_POINTSYSTEM_INFO_ACK );
            return;
        }

        kPacket.m_nOK = 0;
        kPacket.m_nAccTime = nAccTime;
        kPacket.m_nPoint   = nPoint;
        // 가져온 날이 오늘 이전이면? 누적시간 초기화
        if( CTime( tmDate ) < tmCurrentDay ) {
            kPacket.m_nAccTime = 0;
            START_LOG( clog, L"가져온 날이 오늘이 아니면 누적시간 초기화")
                << BUILD_LOG( strLogin_)
                << BUILD_LOG( kPacket.m_nAccTime ) << END_LOG;
        }
        START_LOG( clog, L"포인트 정보 얻을 때" << strLogin_ )
            << BUILD_LOG( CTime(tmDate).GetYear() )
            << BUILD_LOG( CTime(tmDate).GetMonth() )
            << BUILD_LOG( CTime(tmDate).GetDay() )
            << BUILD_LOG( tmCurrentDay.GetYear() )
            << BUILD_LOG( tmCurrentDay.GetMonth() )
            << BUILD_LOG( tmCurrentDay.GetDay() )
            << BUILD_LOG( kPacket.m_nAccTime )
            << BUILD_LOG( kPacket.m_nPoint ) << END_LOG;
    }

    START_LOG( clog, L"포인트 정보 얻기.")
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nAccTime )
        << BUILD_LOG( kPacket.m_nPoint ) << END_LOG;

    // 달력 정보 가져 오기.
    UpdateCalendar( dwUID_, cCharType_, kPacket.m_vecCalendar );

    QUEUING_ACK_TO_USER( EVENT_LOAD_POINTSYSTEM_INFO_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_LOAD_NEW_CALENDAR_NOT, std::vector<KDailyInfo> ) // 새 달력 요청
{
    // 달력 정보 갱신.
    LIF( UpdatePointbagCalender( dwUID_, cCharType_, kPacket_ ) );

    std::vector<KDailyInfo> kPacket;
    UpdateCalendar( dwUID_, cCharType_, kPacket );

    QUEUING_ACK_TO_USER( EVENT_LOAD_NEW_CALENDAR_NOT );
}

_INT_IMPL_ON_FUNC( EVENT_GCPOINT_RECOMMEND_REQ, std::wstring )
{
    //call dbo.gup_pointbag_recommend (LoginUID, Nick nvarchar(24))
    //LoginUID : 추천하는 유저의 UID(신규유저)
    //Nick : 추천받은 유저의 Nick(기존유저)
    //    select 1 loop return (OK int)
    //OK : 0(성공), 
    //    -1(추천하는 유저의 포인트 정보가 없음),
    //    -2(추천받은 유저가 존재하지 않음),
    //    -3(추천받은 유저와 추천한 유저가 동일함).
    //    -4(추천받은 유저의 포인트 정보가 없음),
    //    -11이하(트랜잭션 에러)
    int kPacket = 0;

    if( !kPacket_.empty() )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.gup_pointbag_recommend ( %d, N'%s' ) }",
            dwUID_, kPacket_.c_str() ) );
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> kPacket );
            m_kODBC.EndFetch();
        }

        if( kPacket != 0 )
        {
            switch( kPacket )
            {
            case -1: kPacket = NetError::ERR_GCPOINT_00; break; // 추천하는 유저의 포인트 정보가 없음 
            case -2: kPacket = NetError::ERR_GCPOINT_01; break; // 추천받은 유저가 존재하지 않음
            case -3: kPacket = NetError::ERR_GCPOINT_02; break; // 추천받은 유저와 추천한 유저가 동일함
            case -4: kPacket = NetError::ERR_GCPOINT_03; break; // 추천받은 유저의 포인트 정보가 없음
            default:
                kPacket = NetError::ERR_UNKNOWN;
                START_LOG( cerr, L"알수 없는 오류값.. Name : " << strLogin_ )
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOG( kPacket_ ) << END_LOG;
                break;
            }
            QUEUING_ACK_TO_USER( EVENT_GCPOINT_RECOMMEND_ACK );
            return;
        }

        // NET_OK == 0 이므로..
        QUEUING_ACK_TO_USER( EVENT_GCPOINT_RECOMMEND_ACK );
        // GC 포인트에 꼽사리 낀 녀석 이다..
        // 위의 것이 실패 하면 아래 것은 호출하지 말자.
    }

    
    //--접속한 유저로부터 추천받은 닉네임을 기록
    //call dbo.fhup_invite_temp_update (LoginUID int, Nick nvarchar(24))
    //LoginUID : 추천하는 유저의 UID
    //Nick : 추천하는 유저로부터 입력받은 닉네임(만약 입력이 되지 않았다면 인자를 넣지 않으면 자동 처리됨)
    //select 1 loop return (OK int)
    //OK : 0(성공),
    //-1(이벤트 기간이 지났습니다),
    //-11이하(트랜잭션 에러)   
    if( kPacket_.empty() )
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.fhup_invite_temp_update ( %d ) }",
            dwUID_ ) );
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> kPacket );
        }
        m_kODBC.EndFetch();

        QUEUING_ACK_TO_USER( EVENT_FH_RECOMMEND_REGIST_ACK );
    }
    else
    {
        DBJIF( m_kODBC.Query( L"{ call dbo.fhup_invite_temp_update ( %d, N'%s' ) }",
            dwUID_, kPacket_.c_str() ) );
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> kPacket );
        }
        m_kODBC.EndFetch();

        QUEUING_ACK_TO_USER( EVENT_FH_RECOMMEND_REGIST_ACK );
    }
    
}

_INT_IMPL_ON_FUNC( EVENT_MONTHLY_ATTEND_NOT, KDailyInfo )
{
    /*
    L"{ call dbo.PBGAPointBagMonthReward_insert_20130523 ( %d, %d, N'%s' ) }"

    { call dbo.PBGAPointBagMonthReward_insert_20130523 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input   int
    @02: @02iCharType_input   int
    @03: @03sdtRegDateD_input smalldatetime

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 캐릭터 정보 없음
    -3       : 출석부 기본 정보 없음
    -4       : 이미 등록되어 있음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */
    KEVENT_MONTHLY_ATTEND_NOT kPacket;
    CTime tmCurrent( static_cast<int>(kPacket_.m_kDate.m_sYear),
                     static_cast<int>(kPacket_.m_kDate.m_cMonth),
                     1,0,0,0 );

    std::wstring strDate = (LPCTSTR)tmCurrent.Format( _T("%y%m%d") );
    DBJIF( m_kODBC.Query( L"{ call dbo.PBGAPointBagMonthReward_insert_20130523 ( %d, %d, N'%s' ) }",
        dwUID_, cCharType_, strDate.c_str() ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
        m_kODBC.EndFetch();

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( kPacket.m_nOK ) );

        if( kPacket.m_nOK != 0 ) {
            START_LOG( cerr, L"월개근 보상 정보 기록시 오류 .. Name : " << strLogin_ )
                << BUILD_LOG( m_kODBC.m_strLastQuery )
                << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
            return;
        }
    }

    GetReserveMonthlyList( dwUID_, cCharType_, kPacket.m_vecReservedReward );
    _QUEUING_ACK_TO_USER( EVENT_MONTHLY_ATTEND_NOT, kPacket );
}

INT_IMPL_ON_FUNC( DB_EVENT_MONTHLY_ATTEND_REWARD_REQ )
{
    //정보 업데이트
    // 성공시 아이템 증정. ^^
    /*
    L"{ call dbo.PBGAPointBagMonthReward_update_20130523 ( %d, %d, N'%s', %d ) }"

    { call dbo.PBGAPointBagMonthReward_update_20130523 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input   int
    @02: @02iCharType_input   int
    @03: @03sdtRegDateM_input smalldatetime
    @04: @04iItemID_input     int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 월개근 보상 정보 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    std::vector<KItem>::iterator vit;
    KEVENT_MONTHLY_ATTEND_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    int nItemType = -1;

    CTime tmRegDate( kPacket_.m_kReqDate.m_sYear, kPacket_.m_kReqDate.m_cMonth, kPacket_.m_kReqDate.m_cDay, 0, 0, 0 );
    std::wstring strDate = (LPCTSTR)tmRegDate.Format( _T("%y%m%d") );


    if( kPacket_.m_vecReqItem.empty() == false ) {
        //아무 보상이나 한개만 찔러넣어주면 됨.by현우형
        DBJIF( m_kODBC.Query( L"{ call dbo.PBGAPointBagMonthReward_update_20130523 ( %d, %d, N'%s', %d ) }",
            dwUID_, cCharType_, strDate.c_str(), kPacket_.m_vecReqItem[0].m_ItemID ) );
        if( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
            m_kODBC.EndFetch();
        }

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( kPacket.m_nOK ) );

        if( kPacket.m_nOK != 0 ) {
            switch( kPacket.m_nOK )
            {
            case -1: kPacket.m_nOK = NetError::ERR_GCPOINT_05; break;
            case -2: kPacket.m_nOK = NetError::ERR_GCPOINT_06; break;
            case -3: kPacket.m_nOK = NetError::ERR_GCPOINT_07; break;
            default:
                kPacket.m_nOK = NetError::ERR_UNKNOWN;
                START_LOG( cerr, L"월개근 보상 요청시 알수 없는 에러 발생. Name : " << strLogin_ )
                    << BUILD_LOG( kPacket_.m_vecReqItem[0].m_ItemID )
                    << BUILD_LOG( kPacket.m_nOK ) << END_LOG;
            }

            goto END_PROC;
        }
    }

    for( vit = kPacket_.m_vecReqItem.begin() ; vit != kPacket_.m_vecReqItem.end() ; ++vit ) {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, KItemManager::EWIT_ATTEND_REWARD, kPacket.m_vecInv ) );
    }

    // 월보상 미지급 리스트 재전달.

    GetReserveMonthlyList( dwUID_, cCharType_, kPacket.m_vecReservedReward );

    kPacket.m_nOK = NetError::NET_OK;

END_PROC:
    QUEUING_ACK_TO_USER( EVENT_MONTHLY_ATTEND_REWARD_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_MONTHLY_ATTEND_REWARD_LIST_NOT )
{
    std::vector<KSimpleDate> kPacket;
    GetReserveMonthlyList( dwUID_, cCharType_, kPacket );
    QUEUING_ACK_TO_USER( EVENT_MONTHLY_ATTEND_REWARD_LIST_NOT );
}

_INT_IMPL_ON_FUNC( EVENT_HATCHING_EGG_REQ, KEVENT_DB_HATCHING_EGG_REQ )
{
    KEVENT_HATCHING_EGG_ACK kPacket;
    kPacket.m_kPetInfo.Init();
    int nItemType = -1;
    std::vector<KItem> vecItemList;
    std::wstring wstrPetName;

    // 펫 제거
    //call dbo.gup_pet_remove (PetUID bigint, LoginUID int, PetID int)
    DBJIF( m_kODBC.Query( L"{ call dbo.gup_pet_remove ( %I64d, %d, %d ) }",
        kPacket_.m_kOldPet.m_dwUID, dwUID_, kPacket_.m_kOldPet.m_dwID ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );
    }
    m_kODBC.EndFetch();

    if( kPacket.m_nOK != 0 )
    {
        switch( kPacket.m_nOK ) {
                case -2: SET_ERROR( ERR_PET_03 ); break;    //-1(유저가 존재하지 않음)
                case -3: SET_ERROR( ERR_PET_04 ); break;    //-2(이 아이템이 펫 아이템일까요?)
                case -5: SET_ERROR( ERR_PET_06 ); break;    //-3(펫 아이템이 존재하지 않음)
                case -6: SET_ERROR( ERR_PET_07 ); break;    //-5(펫이 존재하지 않음)
                default:
                    SET_ERROR( ERR_UNKNOWN ); 
        }
        START_LOG( cerr, L" 펫 삭제 중 오류" ) << END_LOG;
        goto END_PROC;
    }

    // 펫 카드 제거
    LIF( PureEraseItem( strLogin_, dwUID_, kPacket_.m_kOldPet.m_dwUID, cCharType_ ) );

    // 새펫 카드 넣기
    if( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_dwNewPetID, -1, -1, KItemManager::EWIT_PET_SYSTEM, vecItemList ) == false )
    {
        SET_ERROR( ERR_PET_35 );
        START_LOG( cerr, L"부화시 새 펫 봉인 카드 넣기 실패 (InsertSingleItem)." )
            << BUILD_LOG( strLogin_ ) << END_LOG;
        goto END_PROC;
    }

    if( vecItemList.empty() )
    {
        START_LOG( cerr, L"펫 아이템을 넣는데 실패 했음." ) << END_LOG;
        SET_ERROR( ERR_PET_35 );
        goto END_PROC;
    }

    if( vecItemList.size() > 1 )
    {
        START_LOG( cerr, L"펫 봉인카드를 인벤에 넣는데 왜 2개 이상의 아이템이 들어가나..??" ) << END_LOG;

        std::vector<KItem>::iterator vit;
        for( vit = vecItemList.begin() ; vit != vecItemList.end() ; ++vit )
        {
            START_LOG( cerr, L"ItemID : " << vit->m_ItemID << ", Duration : " << vit->m_nCount << ", Period : " << vit->m_nPeriod ) << END_LOG;
        }
        SET_ERROR( ERR_PET_35 );
        goto END_PROC;
    }

    KItem& kItem = *vecItemList.begin();
    if( kItem.m_nPeriod != -1 )
    {
        START_LOG( cerr, L"펫 봉인 카드가 왜 수량인것이냐??" )
            << BUILD_LOG( kItem.m_ItemID )
            << BUILD_LOG( strLogin_ ) << END_LOG;
        SET_ERROR( ERR_PET_35 );
        goto END_PROC;
    }

    kPacket.m_kPetCard = kItem;
    kPacket.m_kPetCard.m_cEnchantLevel =  (SiKEnchantStrong()->IsEnchantEnableItem( kPacket.m_kPetCard.m_ItemID ) ? 0 : -1 );
/*
    // 새펫 추가
    //call dbo.gup_pet_create (PetUID bigint, LoginUID int, PetID int, PetName nvarchar(20), CharType tinyint)
    DBJIF( m_kODBC.Query( L"{ call dbo.gup_pet_create ( %I64d, %d, %d, N'%s' ) }",
        kPacket.m_kPetCard.m_ItemUID, dwUID_,
        kPacket.m_kPetCard.m_ItemID, kPacket_.m_strPetName.c_str() ) );

    int nOK = -99;    
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );

        if( nOK == 0 )
        {
            VERIFY_QUERY( m_kODBC >> kPacket.m_kPetInfo.m_cPromotion
                >> kPacket.m_kPetInfo.m_iSatiation );
        }
    }
    m_kODBC.EndFetch();
*/
    wstrPetName = boost::replace_all_copy( kPacket_.m_strPetName, L"'", L"''" );
    int nOK = InsertPetInfo( dwUID_, kPacket_.m_kOldPet.m_cCharType, kPacket.m_kPetCard.m_ItemUID, kPacket.m_kPetCard.m_ItemID, wstrPetName, kPacket.m_kPetInfo.m_cPromotion, kPacket.m_kPetInfo.m_iSatiation );

    // 생성 실패
    if( nOK != 0 ) {
        switch( nOK ) {
            case -1: SET_ERROR( ERR_PET_03 ); break;    //-1(유저가 존재하지 않음)
            case -2: SET_ERROR( ERR_PET_04 ); break;    //-2(이 아이템이 펫 아이템일까요?)
            case -3: SET_ERROR( ERR_PET_01 ); break;    //-3(펫 아이템이 존재하지 않음)
            case -5: SET_ERROR( ERR_PET_02 ); break;    //-5(신규 생성하려는 팻이 이미 존재함)
            case -6: SET_ERROR( ERR_PET_05 ); break;    //-6(캐릭터가 존재하지 않음)
            default:
                SET_ERROR( ERR_UNKNOWN ); 
        }
        START_LOG( cerr, L" 펫 생성 중 오류" ) << END_LOG;
        goto END_PROC;
    }

    SetCharacterPetEquip( dwUID_, strLogin_, kPacket.m_kPetCard.m_ItemUID, -1 );

    kPacket.m_kPetInfo.m_dwID               = kPacket.m_kPetCard.m_ItemID;
    kPacket.m_kPetInfo.m_dwUID              = kPacket.m_kPetCard.m_ItemUID;
    kPacket.m_kPetInfo.m_cCharType          = kPacket_.m_kOldPet.m_cCharType;
    kPacket.m_kPetInfo.m_strName            = kPacket_.m_strPetName;
    kPacket.m_kPetInfo.m_vecEquipItem.clear();
    for( int i = 0; i <= 2; i++ )
        kPacket.m_kPetInfo.m_mapInitExp.insert( std::make_pair( i , SiKPetHelper()->GetPetMinExp() ) );

    kPacket.m_kPetInfo.m_dwEXP              = SiKPetHelper()->GetPetMinExp();
    kPacket.m_kPetInfo.m_iInitSatiation     = kPacket.m_kPetInfo.m_iSatiation;
    kPacket.m_kPetInfo.m_iLevel             = SiKPetHelper()->GetPetLevel( kPacket.m_kPetInfo.m_dwEXP );
    kPacket.m_kPetInfo.m_nHatchingID        = SiKPetHelper()->GetNextID( kPacket.m_kPetInfo.m_dwID );

    // 보상 아이템 넣어주기.
    m_kHelper.InsertPetItem( dwUID_, kPacket_.m_cCurrentChar, kPacket.m_kPetInfo.m_dwID, 0, kPacket.m_vecItem );

    kPacket.m_kOldPetInfo = kPacket_.m_kOldPet;

    SET_ERROR( NET_OK );
END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_HATCHING_EGG_ACK );
    
    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_TIME_SYNCRONIZE_NOT )
{
    if( dwUID_ == 1 && !SiKGSSimLayer()->m_bInitTimeSync ) {
        START_LOG( clog, L"서버 처음 켜지고 동기화 무시.." ) << END_LOG;
        return;
    }

    if( !SiKGSSimLayer()->m_bTimeSync ) {
        START_LOG( clog, L"시간 동기화 무시.." ) << END_LOG;
        return;
    }

    SYSTEMTIME sysTime;
    DBJIF( m_kODBC.Query( L"SELECT GETDATE()" ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> sysTime );
        m_kODBC.EndFetch();
    }

    LIF( ::SetLocalTime( &sysTime ) );

    //START_LOG( clog, L"현재시각은... --> "
    //    << sysTime.wYear << L"-"
    //    << sysTime.wMonth << L"-"
    //    << sysTime.wDay << L" "
    //    << sysTime.wHour << L":"
    //    << sysTime.wMinute << L":"
    //    << sysTime.wSecond << L":"
    //    << sysTime.wMilliseconds << L" ( "
    //    << sysTime.wDayOfWeek << L" )" ) ) << END_LOG;
}

_INT_IMPL_ON_FUNC(EVENT_BUY_GCPOINT_ITEM_REQ, KDB_EVENT_BUY_GCPOINT_ITEM_REQ)
{
    EVENT_CHECK_OUT(EVENT_BUY_GCPOINT_ITEM_REQ);

    std::map<char, KCharacterInfo>::iterator mit;
    KEVENT_BUY_GCPOINT_ITEM_ACK kPacket;
    kPacket.m_nOK = -99;

    std::wstring wstrItemName = L"";
    bool bIsCashItem = (kPacket_.m_kBuyInfo.m_nMoneyType == KBuySellItemReq::EMT_CASH ? true : false);
    bool bIsBuyLimitItem = SiKItemManager()->IsBuyLimitItem(kPacket_.m_kBuyInfo.m_ItemID);

    CTime tmCurrent = CTime::GetCurrentTime();
    std::wstring strDate = (LPCTSTR)tmCurrent.Format(_T("%y%m%d"));
    if (kPacket_.m_kUserData.m_nDiffGCPoint != 0) {
        LIF(UpdateUserPointBagAccTime(dwUID_, cCharType_, kPacket_.m_kUserData.m_nDiffGCPoint, kPacket_.m_kUserData.m_nAccTime, strDate));
    }

    if (bIsBuyLimitItem) {
        if (false == CheckBuyLimitItem(dwUID_, kPacket_.m_kBuyInfo.m_ItemID)) {
            kPacket.m_nOK = -9;
            SET_ERR_GOTO(ERR_GCPOINT_16, END_PROC);
        }
    }

    kPacket.m_nOK = CheckPointBagItemInfo(dwUID_, cCharType_, kPacket_.m_kBuyInfo.m_ItemID);

    if (kPacket.m_nOK != 0) {
        switch (kPacket.m_nOK)
        {
        case -1: SET_ERROR(ERR_GCPOINT_09); break;
        case -2: SET_ERROR(ERR_DB_COMMON_02); break;
        case -3: SET_ERROR(ERR_GCPOINT_05); break;
        case -4: SET_ERROR(ERR_GCPOINT_11); break;
        case -5: SET_ERROR(ERR_INVEN_06); break;
        case -6: SET_ERROR(ERR_GCPOINT_10); break;
        default:
            START_LOG(cerr, L"ERROR!")
                << BUILD_LOG(strLogin_)
                << BUILD_LOG(kPacket.m_nOK)
                << BUILD_LOG(kPacket_.m_kBuyInfo.m_ItemID)
                << BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;
            SET_ERROR(ERR_UNKNOWN);
            kPacket.m_nOK = -99;
        }
        goto END_PROC;
    }

    KBuySellItemReq& kBuyInfo = kPacket_.m_kBuyInfo;
    int nCount = 0, nPrice = 0;

    if (!GetPointBagItemInfo(kBuyInfo.m_ItemID, kBuyInfo.m_nPeriod, nCount, nPrice)) {
        kPacket.m_nOK = -99;
        SET_ERR_GOTO(ERR_GCPOINT_14, END_PROC);
    }
    kPacket.m_nGCPoint = nPrice * kBuyInfo.m_nCount;
    kBuyInfo.m_nPrice = kPacket.m_nGCPoint;
    kBuyInfo.m_nCount *= nCount;

    __int64 waitItemUID = 0;
    kBuyInfo.m_nMoneyType = KItemManager::EMT_GCPOINT;
    if (m_kHelper.BuyItemToWait(dwUID_, strLogin_, kBuyInfo, std::wstring(L""), waitItemUID, wstrItemName) != 0)
    {
        kPacket.m_nOK = -99;
        SET_ERR_GOTO(ERR_GCPOINT_15, END_PROC);
    }

    if (waitItemUID == KItem::INVALID_ITEM)
    {
        kPacket.m_nOK = -99;
        SET_ERR_GOTO(ERR_GCPOINT_15, END_PROC);
    }

    m_kHelper.UpdateWaitItemToEnabled(waitItemUID);
    m_kHelper.InsertSingleWaitItemToInvenOrVDepot(dwUID_, cCharType_, waitItemUID, KItemManager::EWIT_GCPOINT_BUY, kPacket.m_vecItem);

    {
        int nItemPrice = -kPacket.m_nGCPoint;
        LIF(UpdateUserPointBagAccTime(dwUID_, cCharType_, nItemPrice, kPacket_.m_kUserData.m_nAccTime, strDate));
    }

    if (bIsBuyLimitItem) {
        CommitBuyLimitItem(dwUID_, kPacket_.m_kBuyInfo.m_ItemID);
    }

    kPacket.m_nOK = 0;
    SET_ERROR(NET_OK);
END_PROC:
    QUEUING_ACK_TO_USER(EVENT_BUY_GCPOINT_ITEM_ACK);

    LOG_SUCCESS(kPacket.m_nOK == 0)
        << BUILD_LOG(strLogin_)
        << BUILD_LOG(kPacket_.m_kBuyInfo.m_ItemID)
        << BUILD_LOG(bIsCashItem)
        << BUILD_LOG(bIsBuyLimitItem)
        << BUILD_LOG(kPacket.m_nOK)
        << BUILD_LOG(kPacket.m_nGCPoint)
        << BUILD_LOG(kPacket.m_vecItem.size())
        << BUILD_LOG(NetError::GetLastNetErrMsg()) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_SELL_GCPOINT_ITEM_REQ, KBuySellItemReq )
{
    KEVENT_SELL_GCPOINT_ITEM_ACK kPacket;
    kPacket.m_kItemInfo.m_dwID      = kPacket_.m_ItemID;
    kPacket.m_kItemInfo.m_dwUID     = kPacket_.m_ItemUID;
    kPacket.m_kItemInfo.m_cItemType = (char)(kPacket_.m_nItemType);

    if ( !PureEraseItem( strLogin_, dwUID_, kPacket_.m_ItemUID, cCharType_ ) ) {
        kPacket.m_nOK = 3;
        SET_ERROR( ERR_INVEN_01 );
    }
    else {
        kPacket.m_nOK = 0;
        SET_ERROR( NET_OK );
    }

    QUEUING_ACK_TO_USER( EVENT_SELL_GCPOINT_ITEM_ACK );
    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_ItemUID )
        << BUILD_LOG( kPacket_.m_nItemType )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_RESET_WIN_LOSE_REQ, KResetWinLoseData )
{
    //call dbo.wlup_win_lose_reset (LoginUID int, CharType nvarchar(4000))
    //CharType은 캐릭터 타입을 ,로 구분하여 보내주면 됨(예: 0,1,2,3,4)
    //끝자리에 콤마 붙여도 됨(내부 처리 완료)
    //
    //return 1 loop return (OK int)
    //
    //OK : 0(성공),
    //     -1 (CharType에 아무것도 넣어보내주지 않거나 널값임),
    //     -2 (유저가 존재하지 않음),
    //     -3 (잘못된 캐릭터타입을 입력했음),
    //     -4 (캐릭터 테이블에 캐릭터가 없음),
    //     -11이하 (트랜잭션 에러)

    std::wstringstream stm;
    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    USHORT usMaxString = 3500; // SP Parameter 데이터 타입이 varchar( 4000 ) 이므로 제한 필요.

    std::vector<char>::iterator vit;
    for( vit = kPacket_.m_vecChar.begin() ; vit != kPacket_.m_vecChar.end() ; ++vit ) {
        if( stm.str().size() == 0 ) {
            stm << L"{ call dbo.wlup_win_lose_reset ( ";
            stm << static_cast<int>(dwUID_) << ", '";
        }
        stm << static_cast<int>(*vit) << "," ;

        if( stm.str().size() > usMaxString ) {
            stm << L"' ) }";
            DBJIF( m_kODBC.QueryToWString( stm.str() ) );
            if( m_kODBC.BeginFetch() ) {
                int nOK = -99;
                VERIFY_QUERY( m_kODBC >> nOK );
                m_kODBC.EndFetch();

                switch( nOK ) {
                case 0:     SET_ERROR( NET_OK ); break;
                case -1:    SET_ERROR( ERR_RESET_WINLOSE_03 ); break;
                case -2:    SET_ERROR( ERR_RESET_WINLOSE_04 ); break;
                case -3:    SET_ERROR( ERR_RESET_WINLOSE_05 ); break;
                case -4:    SET_ERROR( ERR_RESET_WINLOSE_06 ); break;
                default:
                    SET_ERROR( ERR_UNKNOWN );
                }
            }
            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        }
    }

    if( stm.str().size() != 0 ) {
        stm << L"' ) }";
        SET_ERROR( ERR_UNKNOWN );
        DBJIF( m_kODBC.QueryToWString( stm.str() ) );
        if( m_kODBC.BeginFetch() ) {
            int nOK = -99;
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();

            switch( nOK ) {
            case 0:     SET_ERROR( NET_OK ); break;
            case -1:    SET_ERROR( ERR_RESET_WINLOSE_03 ); break;
            case -2:    SET_ERROR( ERR_RESET_WINLOSE_04 ); break;
            case -3:    SET_ERROR( ERR_RESET_WINLOSE_05 ); break;
            case -4:    SET_ERROR( ERR_RESET_WINLOSE_06 ); break;
            default:
                SET_ERROR( ERR_UNKNOWN );
            }
        }
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }

    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_RESET_WIN_LOSE_ACK, kPacket_ );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_vecChar.size() )
        << BUILD_LOG( stm.str() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_LUCKYDAY_LIST )
{ 
    // 운 좋은 날에 주는 아이템 목록
    //call dbo.gup_pointbag_luckyday_get (@RegDate smalldatetime)
    //@RegDate는 년월만 확실히 넣어주면 내부적으로 알아서 계산해서 월별 럭키데이 날짜를 리턴해줌
    //select n loop return (RegDate smalldatetime)
    std::vector<KSimpleDate> vecLuckyDayList; // 럭키데이 리스트
    vecLuckyDayList.clear();
    std::wstring strDate = (LPCTSTR)CTime::GetCurrentTime().Format( _T("%y%m%d") );
    time_t tmDate = time_t(NULL);

    DBJIF( m_kODBC.Query( L"{ call dbo.gup_pointbag_luckyday_get ( N'%s' ) }",
        strDate.c_str() ) );
    
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {   
        VERIFY_QUERY( m_kODBC >> tmDate );
        CTime Date( tmDate );
        KSimpleDate simpleDate( Date.GetYear(), Date.GetMonth(), Date.GetDay(), 0 );

        vecLuckyDayList.push_back( simpleDate );
    }
    SiKGSSimLayer()->UpdateLuckyDays( vecLuckyDayList );

    START_LOG( clog, L"행운의 날 리스트")
        << BUILD_LOG( vecLuckyDayList.size() ) << END_LOG;
}

// 패킷에 정확한 날짜가 있어야 되고, DB에서 받아온 아이템 목록을 유저로 전달
INT_IMPL_ON_FUNC( EVENT_LUCKYDAY_NOT )
{   //{ call dbo.gup_pointbag_luckyday_item_list_get ( @1 ) }
    //@1 ; RegDate smalldatetime
    //RegDate는 년월일이 정확해야 아이템을 줍니당~ 위에서 받은걸로 정확하게 넣어주면 되삼
    //select n loop return (ItemID int, ItemType int, Factor int)
    std::wstring strLogin;

    std::vector<KItem> vecRewardItemList; // 럭키데이 보상 아이템 리스트
    vecRewardItemList.clear();

    kPacket_.m_nOK = NetError::ERR_UNKNOWN;
    
    CTime tmRegDate( kPacket_.m_kDailyInfo.m_kDate.m_sYear, kPacket_.m_kDailyInfo.m_kDate.m_cMonth, 
                     kPacket_.m_kDailyInfo.m_kDate.m_cDay, 0, 0, 0 );
    std::wstring strDate = (LPCTSTR)tmRegDate.Format( _T("%y%m%d") );

    DBJIF( m_kODBC.Query( L"{ call dbo.gup_pointbag_luckyday_item_list_get ( N'%s' ) }", strDate.c_str() ) );
    
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KItem kItem;
        VERIFY_QUERY( m_kODBC >> kItem.m_ItemID >> kItem.m_nPeriod >> kItem.m_nCount >> kItem.m_cEnchantLevel );
        vecRewardItemList.push_back( kItem );
    }

    if( vecRewardItemList.empty() )
    {
        START_LOG( cerr, L"럭키 데이 설정 되어 있는데 아이템이 없음..Name : " << strLogin_ )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    std::vector<KItem>::iterator vit;
    for( vit = vecRewardItemList.begin() ; vit != vecRewardItemList.end() ; ++vit )
    {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, KItemManager::EWIT_ATTEND_REWARD, kPacket_.m_vecInv ) );
    }

    kPacket_.m_nOK = NetError::NET_OK;

    if( kPacket_.m_vecInv.empty() )
        kPacket_.m_nOK = NetError::ERR_LUCKYDAY_00;

   _QUEUING_ACK_TO_USER( EVENT_LUCKYDAY_NOT, kPacket_ );

    START_LOG( clog, L"럭키데이 아이템 리스트" )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( vecRewardItemList.size() )
        << BUILD_LOG( kPacket_.m_vecInv.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_STAT_PROMOTION_COUNT )
{
    //call dbo.cpup_character_promotion_count_update ( Data nvarchar(4000) )
    //Data 형식 : [CharType,Promotion,Count]n
    //CharType은 0부터(캐릭터는 0, 1, 2, 3, 4)
    //Promotion도 0부터(3차 전직까지라면 0, 1, 2, 3)
    //
    //  select 1 loop return ( OK int )
    //  0(성공),
    //  -1(잘못된 Data입력),
    //  -2(CharType이 존재하지 않음),
    //  -3(잘못된 Data입력),
    //  -4(Promotion이 존재하지 않음),
    //  -5(카운트가 음수로 왔음),
    //  -11이하(트랜잭션 에러)

    if( kPacket_.empty() ) return;

    std::wstringstream stm;
    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    USHORT usMaxString = 3500; // SP Parameter 데이터 타입이 varchar( 4000 ) 이므로 제한 필요.

    KEVENT_STAT_PROMOTION_COUNT::iterator mit;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        if( stm.str().size() == 0 ) {
            stm << L"{ call dbo.cpup_character_promotion_count_update ( '";
        }
        stm << static_cast<int>(mit->first >> 8) << L",";
        stm << static_cast<int>(mit->first & 0x00ff) << L",";
        stm << static_cast<int>(mit->second) << L",";

        if( stm.str().size() > usMaxString ) {
            stm << L"' ) }";
            //DBJIF( m_kODBC.Query( stm.str().c_str() ) );
            DBJIF( m_kODBC.QueryToWString( stm.str() ) );

            int nOK;
            if( m_kODBC.BeginFetch() ) {
                VERIFY_QUERY( m_kODBC >> nOK );
            }
            m_kODBC.EndFetch();

            LOG_SUCCESS( nOK == 0 )
                << BUILD_LOG( nOK )
                << BUILD_LOG( stm.str() ) << END_LOG;

            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        }
    }
    if( stm.str().size() != 0 ) {
        stm << L"' ) }";
        //DBJIF( m_kODBC.Query( stm.str().c_str() ) );
        DBJIF( m_kODBC.QueryToWString( stm.str() ) );

        int nOK;
        if( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( stm.str() ) << END_LOG;

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_YAHOO_BENEFIT_NOT )
{
    KInven_GP kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nGP = SiKGameServer()->GetPubEventGP();
    kPacket.m_vecItem.clear();

    std::vector<KItem> vecPubEventItem;
    SiKGameServer()->GetPubEventItemList( vecPubEventItem );

    std::vector<KItem>::iterator vit;
    for( vit = vecPubEventItem.begin() ; vit != vecPubEventItem.end() ; ++vit )
    {
        m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, KItemManager::EWIT_EVENT_YAHOO_BENEFIT, kPacket.m_vecItem );
    }

//    JIF( SiKGameServer()->GetPubEventBenefit( strLogin_, kPacket, m_kODBC ) );

    QUEUING_ACK_TO_USER( EVENT_YAHOO_BENEFIT_NOT );
}

_INT_IMPL_ON_FUNC( EVENT_DB_STAT_LOADING_TIME_NOT, KEVENT_MAP_DWORD_DWORD )
{
    JIF( !kPacket_.empty() );
//  call dbo.sup_client_loading_time_insert ( Data nvarchar(4000) )
//  Data의 구조 : [LoadingTime, Count,]*n
//  return 1 loop select ( OK int )
//  OK : 0(성공), -1(데이터짝이 맞지 않음), -2(중복된 데이터가 있음), -11이하(트랜잭션 에러)

    /*
    L"{ call dbo.CLGAClientLoadingTime_merge_20110525 ( %d, %d ) }"

    { call dbo.CLGAClientLoadingTime_merge_20110525 ( @1, @2 ) }
    @1: @iLoadingTime_ int
    @2: @iCNT_         int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜젝션 에러
    */

    //std::wstringstream stm;
    KEVENT_MAP_DWORD_DWORD::iterator mit;
    for ( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit ) {
        //stm << mit->first << L"," << mit->second << L",";
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.CLGAClientLoadingTime_merge_20110525 ( %d, %d ) }", 
            mit->first, mit->second ), return, (&m_kODBCStat) );

        int nOK = -99;

        if ( m_kODBCStat.BeginFetch() ) {
            VERIFY_QUERY( m_kODBCStat >> nOK );
        }
        m_kODBCStat.EndFetch();

        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBCStat.m_strLastQuery ) << END_LOG;
    }

    //_DBJIF( m_kODBCStat.Query( L"{ call dbo.sup_client_loading_time_insert ( N'%s' ) }",
    //    stm.str().c_str() ), return, (&m_kODBCStat) );

    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

_INT_IMPL_ON_FUNC( EVENT_INSERT_HACK_INFO_NOT, int )
{
    //call dbo.sup_user_hack_info_insert ( LoginUID int, Type tinyint )
    //return 1 loop select ( OK int )
    //OK : 0(성공),
    //-1(유저가 없음),
    //-11(트랜잭션 에러)

    int nOK = -99;
    _DBJIF( m_kODBCStat.Query( L"{ call dbo.sup_user_hack_info_insert ( %d, %d ) }",
        dwUID_, kPacket_ ), return, (&m_kODBCStat) );

    if( m_kODBCStat.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBCStat >> nOK );
    }
    m_kODBCStat.EndFetch();

    LOG_SUCCESS( nOK == 0 ) // 기록 남기다 잘못되면 로그를 남기자..
        << BUILD_LOG( nOK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_ ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_GET_NASTY_INFO_REQ )
{
    KEVENT_GET_NASTY_INFO_NOT kPacket;

    _DBJIF( m_kODBCStat.Query( L"{ call dbo.sup_user_hack_info_get ( %d ) }", dwUID_ ), return, (&m_kODBCStat) );

    while( m_kODBCStat.Fetch() != SQL_NO_DATA )  
    {
        int nType = 0;
        int nCount = 0;
        VERIFY_QUERY( m_kODBCStat >> nType >> nCount );
        kPacket.m_vecHackCount.push_back( std::make_pair(nType,nCount) );
    }
    m_kODBCStat.EndFetch();

    // 여기에 신고 카운트...
    //{ call dbo.sup_user_complaint_info_get ( @1 ) }
    //@1 ; LoginUID int
    //return 1 select ( Send int, Take int)
    //Send : 오늘 하루 동안 LoginUID가 신고한 횟수
    //Take : 최근 1주일간 LoginUID가 신고 당한 횟수 

    _DBJIF( m_kODBCStat.Query( L"{ call dbo.sup_user_complaint_info_get ( %d ) }", dwUID_ ), return, (&m_kODBCStat) );

    if( m_kODBCStat.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBCStat >> kPacket.m_nReport >> kPacket.m_nReported );
    }
    m_kODBCStat.EndFetch();

    QUEUING_ACK_TO_USER( EVENT_GET_NASTY_INFO_NOT );

    START_LOG( clog, L"찌질이 쉐퀴들..?이 애정이 있어서 그런거다.., Name : " << strLogin_ )
        << BUILD_LOG( kPacket.m_vecHackCount.size() )
        << BUILD_LOG( kPacket.m_nReported )
        << BUILD_LOG( kPacket.m_nReport ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_REPORT_USER_REQ )
{
    //call dbo.sup_user_complaint_info_insert ( SendLoginUID int, Type tinyint, Data nvarchar(4000) )
    //Data의 구조 : [ TakeLoginUID int, ]*n
    //return 1 select ( OK int, SerialNum int)
    //0 : 성공,  
    //-1 : 신고한 유저 정보 없음 
    //-2 : 해당 신고 타입이 없음
    //-11 : 트랜잭션 오류 
    //-22 : 신고당한 유저 정보 없음
    KEVENT_DB_REPORT_USER_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_dwSerialNum = 0;

    std::wstringstream stm;
    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    USHORT usMaxString = 3500; // SP Parameter 데이터 타입이 varchar( 4000 ) 이므로 제한 필요.
    std::vector<DWORD>::iterator vit;

    if( kPacket_.m_vecUser.empty() ) {
        SET_ERROR( ERR_REPORT_01 );
        goto END_PROC;
    }

    for( vit = kPacket_.m_vecUser.begin() ; vit != kPacket_.m_vecUser.end() ; ++vit ) {
        if( stm.str().size() == 0 ) {
            stm << L"{ call dbo.sup_user_complaint_info_insert ( ";
            stm << dwUID_ << L",";
            stm << kPacket_.m_nReason << L", '";
        }
        stm << *vit << L",";
        if( stm.str().size() > usMaxString ) {
            stm << L"' ) }";
            _DBJIF( m_kODBCStat.QueryToWString( stm.str() ), return, (&m_kODBCStat) );
            if( m_kODBCStat.BeginFetch() ) {
                VERIFY_QUERY( m_kODBCStat >> kPacket.m_nOK >> kPacket.m_dwSerialNum );
            }
            m_kODBCStat.EndFetch();

            kPacket.m_kReqPacket.m_nReason = kPacket_.m_nReason;
            kPacket.m_kReqPacket.m_strUser = kPacket_.m_strUser;

            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        }
    }
    if( stm.str().size() != 0 ) {
        stm << L"' ) }";
        _DBJIF( m_kODBCStat.QueryToWString( stm.str() ), return, (&m_kODBCStat) );
        if( m_kODBCStat.BeginFetch() ) {
            VERIFY_QUERY( m_kODBCStat >> kPacket.m_nOK >> kPacket.m_dwSerialNum );
        }
        m_kODBCStat.EndFetch();

        kPacket.m_kReqPacket.m_nReason = kPacket_.m_nReason;
        kPacket.m_kReqPacket.m_strUser = kPacket_.m_strUser;
    }

    switch( kPacket.m_nOK )
    {
    case 0:     SET_ERROR( NET_OK );            break;
    case -1:    SET_ERROR( ERR_REPORT_03 );     break; //신고한 유저 정보 없음 
    case -2:    SET_ERROR( ERR_REPORT_04 );     break; //해당 신고 타입이 없음
    case -22:   SET_ERROR( ERR_REPORT_05 );     break; //신고당한 유저 정보 없음
    case -11:   SET_ERROR( ERR_DB_COMMON_00 );  break; //트랜잭션 오류

    default:
        SET_ERROR( ERR_UNKNOWN );
    }

END_PROC:
    QUEUING_ACK_TO_USER( EVENT_REPORT_USER_ACK );

    kPacket.m_nOK = NetError::GetLastNetError();
    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << L"신고 DB 작업.. : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwSerialNum )
        << BUILD_LOG( kPacket_.m_nReason )
        << BUILD_LOG( stm.str() ) << END_LOG;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_GET_CHATTING_EVENT_MSG_REQ )
{
    KChatEventInfo kChatEventInfo;
    kChatEventInfo.m_nUID = -99;
    std::wstringstream stm;

    /*
    L"{ call dbo.ecup_event_chat_date_get }"

    { call dbo.ecup_event_chat_date_get }

    n return ( @1, @2, @3, @4, @5, @6 )
    @1 ; UID int
    @2 ; StartDate smalldatetime
    @3 ; EndDate   smalldatetime
    @4 ; GPRatio   float
    @5 ; EXPRatio  float
    @6 ; BuffRatio float
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.ecup_event_chat_date_get }" ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC   >> kChatEventInfo.m_nUID
                                >> kChatEventInfo.m_tmStart
                                >> kChatEventInfo.m_tmEnd
                                >> kChatEventInfo.m_fGPRatio
                                >> kChatEventInfo.m_fEXPRatio
                                >> kChatEventInfo.m_fBuffRatio );
    }
    m_kODBC.EndFetch();

    if( kChatEventInfo.m_nUID <= 0 )
    {
        SiKGSSimLayer()->ClearChatEvent();
        START_LOG( clog, L"채팅 이벤트가 등록 되지 않음.." ) << END_LOG;
        return;
    }


    DBJIF( m_kODBC.Query( L"{ call dbo.ecup_event_chat_word_get ( %d ) }",
        kChatEventInfo.m_nUID ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )  
    {
        std::wstring strMsg;
        VERIFY_QUERY( m_kODBC >> strMsg );
        boost::trim( strMsg ); // 혹시 모르니 트림 한다.. 앞뒤 공백은 모두 제거..
        kChatEventInfo.m_vecEventMsg.push_back( strMsg );
        stm << L"(" << strMsg << L"), ";
    }
    m_kODBC.EndFetch();

    if( kChatEventInfo.m_vecEventMsg.empty() )
    {
        SiKGSSimLayer()->ClearChatEvent();
        START_LOG( cerr, L"채팅 이벤트 문자열이 없음.." )
            << BUILD_LOG( kChatEventInfo.m_nUID )
            << BUILD_LOGtm( CTime(kChatEventInfo.m_tmStart) )
            << BUILD_LOGtm( CTime(kChatEventInfo.m_tmEnd) )
            << BUILD_LOG( kChatEventInfo.m_fGPRatio )
            << BUILD_LOG( kChatEventInfo.m_fEXPRatio ) << END_LOG;

        return;
    }

    SiKGSSimLayer()->SetChatEvent( kChatEventInfo );

    START_LOG( clog, L"채팅 이벤트 갱신.." )
        << BUILD_LOG( kChatEventInfo.m_nUID )
        << BUILD_LOGtm( CTime(kChatEventInfo.m_tmStart) )
        << BUILD_LOGtm( CTime(kChatEventInfo.m_tmEnd) )
        << BUILD_LOG( kChatEventInfo.m_fGPRatio )
        << BUILD_LOG( kChatEventInfo.m_fEXPRatio )
        << BUILD_LOG( kChatEventInfo.m_vecEventMsg.size() )
        << BUILD_LOG( stm.str() ) << END_LOG;

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}
//====================================================================================
// 2007-11-07 오전 10:12:24 : Asirion 
/*
{ call dbo.gup_strong_item_enchant ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }

{ call dbo.gup_strong_item_enchant ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 ) }
@1  ; LoginUID int
@2  ; StuffItemUID int --재료 : 무조건 수량 아이템. 개수만 차감. 깎일 개수가 보유 개수보다 크다면 에러 리턴. 성공/실패 여부 관계없이 차감됨.
@3  ; StuffItemID int
@4  ; StuffItemCount int
@5  ; AssistanceItemUID int --보조 : 무조건 영구 아이템. 성공/실패 여부 관계없이 삭제됨. 서버에서 자방해줄 것.
@6  ; AssistanceItemID int
@7  ; StrongItemUID int --강화 : 무조건 영구 아이템. 깨짐의 경우 삭제됨. 서버에서 자방해줄 것.
@8  ; StrongItemID int
@9  ; Broken bit
@10 ; StrongLevel tinyint = 0

OK 설명
0 : 성공
-1 : 유저가 존재하지 않음
-2 : 재료 아이템이 존재하지 않거나 있더라도 차감 개수보다 작음
-3 : 보조 아이템이 존재하지 않음
-4 : 강화될 수 있는 아이템이 아님(강화 아이템 리스트에 아이템이 등록되어 있지 않음)
-5 : 강화 아이템이 존재하지 않음
-101이하 : 트랜잭션 에러
*/

_INT_IMPL_ON_FUNC( EVENT_ENCHANT_REQ, KDB_EVENT_ENCHANT_REQ )
{
    EVENT_CHECK_OUT( EVENT_ENCHANT_REQ );

    KEVENT_ENCHANT_ACK kPacket;
    DBJIF( m_kODBC.Query( L"{ call dbo.gup_strong_item_enchant ( %d, %I64d, %d, %d, %I64d, %d, %I64d, %d, %d, %d ) }",
        kPacket_.m_dwLoginUID, kPacket_.m_EnchantStuffUID,
        kPacket_.m_EnchantStuffID,
        kPacket_.m_uiEnchantStuffCount,
        kPacket_.m_AssistItemUID,
        kPacket_.m_AssistItemID,
        kPacket_.m_ItemUID,
        kPacket_.m_ItemID,
        (kPacket_.m_bBreak?1:0),
        kPacket_.m_iEnchantLevel ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK
            >> kPacket.m_iRemainCoreCount );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( kPacket.m_nOK == 0, L"룬강화 결과.. OK : " << kPacket.m_nOK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if ( kPacket.m_nOK == 0 )
    {
        kPacket.m_EnchantAssistItemUID = static_cast<GCITEMUID>( kPacket_.m_AssistItemUID );
        kPacket.m_EnchantAssistItemID = kPacket_.m_AssistItemID;
        switch( kPacket_.m_eEnchantAction )
        {
        case KEnchantStrong::EA_SUCCESS:    kPacket.m_nOK = 0; break;
        case KEnchantStrong::EA_WEAKNESS:   kPacket.m_nOK = 1; break;
        case KEnchantStrong::EA_DROP:       kPacket.m_nOK = 2; break;
        case KEnchantStrong::EA_BREAK:      kPacket.m_nOK = 3; break; // DB에서 아이템 삭제가 일어난다.
        case KEnchantStrong::EA_SPECIFIC:   kPacket.m_nOK = 4; break; // 아이템 강화 레벨 특정 레벨 변환.
        default:
            {
                START_LOG( cerr, L"정의되지 않은 강화 동작..  Name : " << strLogin_ << L", kPacket_.m_eEnchantAction : " << kPacket_.m_eEnchantAction )
                    << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            }
        }
    }

    kPacket.m_StuffItemUID              = static_cast<GCITEMUID>( kPacket_.m_EnchantStuffUID );
    kPacket.m_EnchantItemUID            = static_cast<GCITEMUID>( kPacket_.m_ItemUID );
    kPacket.m_dwReduceStuffItemCount    = kPacket_.m_uiEnchantStuffCount;
    kPacket.m_nEnchantLevel             = kPacket_.m_iEnchantLevel;

    QUEUING_ACK_TO_USER( EVENT_ENCHANT_ACK );
}
_INT_IMPL_ON_FUNC( DB_EVENT_CASH_ERROR_REPORT, KDB_EVENT_CASH_ERROR_REPORT )
{
    //call dbo.beup_billing_error_count_update ( ErrorName nvarchar(30), Count int )
    //ErrorName : 에러 문자를 담아주셈
    //Count : 해당 에러 문자에 대한 카운트

    //return 1 loop select ( OK int )
    //OK
    //0(작업 성공)
    //-11이하(트랜잭션 에러)
    // lock 해야 하나...
    std::wstringstream stm;
    std::map<std::wstring, DWORD>::iterator mit;
    int iOK = 0;

    for( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit )
    {
        stm << mit->first << L" : " << mit->second << L"\r\n";
        DBJIF( m_kODBC.Query( L"{ call dbo.beup_billing_error_count_update ( N'%s', %d ) }",
            mit->first.c_str(), mit->second ) );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> iOK );
        }
        m_kODBC.EndFetch();

        LOG_SUCCESS( iOK == 0 )
            << BUILD_LOG( iOK )
            << L"빌링 오류 DB 기록 완료 " << dbg::endl
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    SiKLogManager()->Get( KLogManager::LT_GASH_ERR_STAT )
        << L" Bill Err : " << KNC_TIME_STRING << GLOG::endl
        << stm.str() << GLOG::endl;
    // TODO : 큐잉 방식으로 (다른 로그 처럼) 바꾸는 작업이 필요하다

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

INT_IMPL_ON_FUNC( DB_EVENT_BONUS_POINT_REFILL_REQ )
{
    // 해당 유저, 캐릭터의 충전 기록이 없는 경우에만 충전 처리한다
    if ( 0 < GetTodayCharEventCount( dwUID_, kPacket_.m_cCharType, UET_BONUS_POINT_REFILL ) ) {
        return;
    }

    int nOK = UpdateDefaultBonusLife( dwUID_, kPacket_.m_cCharType, kPacket_.m_dwSlotNum, kPacket_.m_nDiffBonusCount );

    if ( 0 == nOK ) {
        KDB_EVENT_BONUS_POINT_REFILL_ACK kPacket;
        kPacket.m_cCharType = kPacket_.m_cCharType;
        kPacket.m_dwSlotNum = kPacket_.m_dwSlotNum;
        kPacket.m_nDiffBonusCount = kPacket_.m_nDiffBonusCount;

        QUEUING_ACK_TO_USER( DB_EVENT_BONUS_POINT_REFILL_ACK );

        // 기본 보너스 포인트 리필을 성공한 뒤 리필여부를 갱신해준다
        SetTodayCharEventDoneDiff( dwUID_, kPacket_.m_cCharType, UET_BONUS_POINT_REFILL );
    }

    LOG_SUCCESS( nOK == 0 || nOK == -2 )
        << L"보너스 충전 결과 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( nOK )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOG( kPacket_.m_dwSlotNum )
        << BUILD_LOG( kPacket_.m_nDiffBonusCount ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_CHANGE_BONUS_COIN_REQ )
{
    EVENT_CHECK_OUT( EVENT_CHANGE_BONUS_COIN_REQ );

    KEVENT_CHANGE_BONUS_COIN_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_cCharType = kPacket_.m_cCharType;
    kPacket.m_nSpecialBonusDiff = kPacket_.m_nSpecialBonusDiff;

    SET_ERROR( ERR_UNKNOWN );

    // 충전 이전에 변경된 보너스 포인트 수량 DB 동기화
    if ( 0 != kPacket_.m_nSpecialBonusDiff ) {
        int nOK = UpdateSpecialBonusLife( dwUID_, kPacket_.m_cCharType, kPacket_.m_dwSlotNum, kPacket_.m_nSpecialBonusDiff );

        if ( 0 != nOK ) {
            SET_ERR_GOTO( ERR_BONUS_07, END_PROC );
        }
    }

    // 이번에 충전하는 보너스 포인트 DB 동기화
    int nOK = UpdateSpecialBonusLife( dwUID_, kPacket_.m_cCharType, kPacket_.m_dwSlotNum, kPacket_.m_ucCount );

    if ( 0 != nOK ) {
        SET_ERR_GOTO( ERR_BONUS_08, END_PROC );
    }

    if ( false == UseCountItem( dwUID_, kPacket_.m_kItem.m_dwUID, kPacket_.m_ucCount ) ) {
        START_LOG( cerr, L"부활 코인 아이템 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;

        SET_ERR_GOTO( ERR_BONUS_06, END_PROC );
    }

    kPacket.m_kDurItem.m_ItemUID = kPacket_.m_kItem.m_dwUID;
    kPacket.m_kDurItem.m_nCount = kPacket_.m_ucCount;

    kPacket.m_nSpecialBonusDiff = kPacket_.m_nSpecialBonusDiff + kPacket_.m_ucCount;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_CHANGE_BONUS_COIN_ACK );

    LOG_SUCCESS( NetError::NET_OK == kPacket.m_nOK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nSpecialBonusDiff )
        << BUILD_LOG( kPacket_.m_kItem.m_dwID )
        << BUILD_LOG( kPacket_.m_kItem.m_dwUID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_ucCount )
        << BUILD_LOG( kPacket_.m_dwSlotNum )
        << BUILD_LOG( kPacket_.m_nSpecialBonusDiff )
        << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_SLOT_INFO_NOT )
{
    KQuickSlot kPacket;
    kPacket.m_vecItemSlot.reserve( 5 );
    kPacket.m_vecEmoticonSlot.reserve( 15 );

    SelectCharQuickSlotInfo( dwUID_, cCharType_, kPacket.m_vecItemSlot, kPacket.m_vecEmoticonSlot );

    // 소트 하고....유니크 하고..제거 하고.
    std::vector< KSlotData >::iterator vit;
    if ( false == kPacket.m_vecItemSlot.empty() ) {
        std::sort( kPacket.m_vecItemSlot.begin(), kPacket.m_vecItemSlot.end(),
            boost::bind( &KSlotData::m_nSlotIndex, _1 ) < boost::bind( &KSlotData::m_nSlotIndex, _2 ) );
        vit = std::unique( kPacket.m_vecItemSlot.begin(), kPacket.m_vecItemSlot.end(),
            boost::bind( &KSlotData::m_nSlotIndex, _1 ) == boost::bind( &KSlotData::m_nSlotIndex, _2 ) );

        if ( vit != kPacket.m_vecItemSlot.end() ) {
            kPacket.m_vecItemSlot.erase( vit, kPacket.m_vecItemSlot.end() );
        }
    }

    if ( false == kPacket.m_vecEmoticonSlot.empty() ) {
        std::sort( kPacket.m_vecEmoticonSlot.begin(), kPacket.m_vecEmoticonSlot.end(),
            boost::bind( &KSlotData::m_nSlotIndex, _1 ) < boost::bind( &KSlotData::m_nSlotIndex, _2 ) );
        vit = std::unique( kPacket.m_vecEmoticonSlot.begin(), kPacket.m_vecEmoticonSlot.end(),
            boost::bind( &KSlotData::m_nSlotIndex, _1 ) == boost::bind( &KSlotData::m_nSlotIndex, _2 ) );

        if ( vit != kPacket.m_vecEmoticonSlot.end() ) {
            kPacket.m_vecItemSlot.erase( vit, kPacket.m_vecEmoticonSlot.end() );
        }
    }

    QUEUING_ACK_TO_USER( EVENT_SLOT_INFO_NOT );

    START_LOG( clog, L"퀵 슬롯 정보 가져오기.. Name : " << strLogin_ )
        << BUILD_LOG( kPacket.m_vecItemSlot.size() )
        << BUILD_LOG( kPacket.m_vecEmoticonSlot.size() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_STAT_CPL_INSERT, MAP_CPL_STAT )
{
    /*
    20081115. microcat.

    { call dbo.STGA_season_two_collect_cpl ( %d, %d, %d, %d, %d, %d, %d, %d, %I64d, %d ) }

    { call dbo.STGA_season_two_collect_cpl ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 ) }
    @1 ; ModeID smallint
    @2 ; Difficulty tinyint
    @3 ; WinLose bit
    @4 ; CharType tinyint
    @5 ; Promotion tinyint
    @6 ; Level tinyint
    @7 ; DataType tinyint
    @8 ; CPLCount int
    @9 ; CPLData bigint
    @10; UserUID int

    1 return ( @1 )
    @1 ; OK int

    OK
    0        ; 성공
    -1       ; SPTSeasonTwo 테이블에 ModeID와 Difficulty가 등록되어 있지 않음
    -101이하 ; 트랜잭션 에러
    */

    MAP_CPL_STAT::iterator mit;
    int nCount = 0;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.STGA_season_two_collect_cpl ( %d, %d, %d, %d, %d, %d, %d, %d, %I64d, %d ) }",
            mit->first.m_nModeID,
            mit->first.m_nDifficult,
            (int)(mit->first.m_bWinLose?1:0),
            (int)mit->first.m_cCharType,
            (int)mit->first.m_cPromotion,
            mit->first.m_nLevel,
            (int)mit->first.m_cDataType,
            mit->second.first,     // Count
            mit->second.second,    // Data
            (int)mit->first.m_dwUID ),  // UserUID
            return, (&m_kODBCStat) );
        
        int nOK = -99;
        if( m_kODBCStat.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBCStat >> nOK );
            m_kODBCStat.EndFetch();
        }

        LOG_SUCCESS( nOK == 0 )
            << L"CPL 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
            << BUILD_LOG( nOK )
            << BUILD_LOG( mit->first.m_dwUID )
            << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
    }
}

_INT_IMPL_ON_FUNC( EVENT_STAT_PLAY_TIME_INSERT, MAP_PLAY_TIME_STAT )
{
    /*
    20081115. microcat.

    { call dbo.STGA_season_two_collect_p ( %d, %d, %d, %d, %d, %d, %d ) }

    { call dbo.STGA_season_two_collect_p ( @1, @2, @3, @4, @5, @6, @7 ) }
    @1 ; ModeID smallint
    @2 ; Difficulty tinyint
    @3 ; WinLose bit
    @4 ; Party tinyint
    @5 ; Level tinyint
    @6 ; PCount int
    @7 ; PData bigint

    1 return ( @1 )
    @1 ; OK int

    OK
    0        ; 성공
    -1       ; SPTSeasonTwo 테이블에 ModeID와 Difficulty가 등록되어 있지 않음
    -101이하 ; 트랜잭션 에러
    */

    MAP_PLAY_TIME_STAT::iterator mit;
    int nCount = 0;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.STGA_season_two_collect_p ( %d, %d, %d, %d, %d, %d, %d ) }",
            mit->first.m_nModeID, 
            mit->first.m_nDifficult, 
            (int)(mit->first.m_bWinLose?1:0),
            (int)mit->first.m_cPartyNum,
            mit->first.m_nLevel,
            mit->second.first,
            mit->second.second ), return, (&m_kODBCStat) );
        int nOK = -99;
        if( m_kODBCStat.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBCStat >> nOK );
            m_kODBCStat.EndFetch();
        }

        LOG_SUCCESS( nOK == 0 )
            << L"Party 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
            << BUILD_LOG( nOK )
            << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
    }
}

_INT_IMPL_ON_FUNC( EVENT_STAT_MAP_COUNT, MAP_INT_DWORD )
{
    //call dbo.spt_season_two_map_collect
    //( Data nvarchar(4000) )
    //
    //Data = [MapID smallint, MapCount int,]
    //
    //return 1 loop select
    //( OK int )
    //
    //0 : 성공
    //-1 : 데이터의 짝이 맞지 않음
    //-2 : MapID가 존재하지 않음
    //-3 : Data에 중복된 MapID가 있음
    //-101이하 : 트랜잭션 에러

    std::wstringstream stm;
    MAP_INT_DWORD::iterator mit;
    boost::wformat fmter( L"%d,%d," );

    int nCount = 0;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        fmter % mit->first % mit->second;
        if( stm.str().size() + fmter.str().size() > 3500 )
        {
            _DBJIF( m_kODBCStat.Query( L"{ call dbo.spt_season_two_map_collect ( N'%s' ) }",
                stm.str().c_str() ), return, (&m_kODBCStat) );
            int nOK = -99;
            if( m_kODBCStat.BeginFetch() )
            {
                VERIFY_QUERY( m_kODBCStat >> nOK );
                m_kODBCStat.EndFetch();
            }

            LOG_SUCCESS( nOK == 0 )
                << L"Map 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
                << BUILD_LOG( nOK )
                << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
            stm.str(L"");
            ++nCount;
        }

        stm << fmter.str();
    }

    if( !stm.str().empty() )
    {
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.spt_season_two_map_collect ( N'%s' ) }",
            stm.str().c_str() ), return, (&m_kODBCStat) );
        int nOK = -99;
        if( m_kODBCStat.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBCStat >> nOK );
            m_kODBCStat.EndFetch();
        }

        LOG_SUCCESS( nOK == 0 )
            << L"Map 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
            << BUILD_LOG( nOK )
            << L"Last Query : " << m_kODBCStat.m_strLastQuery << dbg::endl
            << BUILD_LOG( nCount ) << END_LOG;
    }

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_INDIGO_RANK_NOT )
{
    //SiKGCHelper()->UpdateIndigoRank( m_kODBC );
    UpdateIndigoRank();
}

INT_IMPL_ON_FUNC( EVENT_NEW_CHARACTER_REQ )
{
    KEVENT_NEW_CHARACTER_ACK kPacket;
    kPacket.m_nOK = 99;
    kPacket.m_ItemUID = kPacket_.m_ItemUID;
    int nResult = -99;
    std::map<DWORD,int>::iterator mit;

    // 캐릭터 획득
    if( !CreateNewChar( dwUID_, strLogin_, kPacket_.m_cCharType, kPacket.m_kCharacterInfo ) ) {
        SET_ERR_GOTO( ERR_CHARACTER_02, END_PROC );
    }
    // 아이템 제거.
    // 이미 캐릭터 넣었으므로 실패 해도
    LIF( PureEraseItem( strLogin_, dwUID_, kPacket_.m_ItemUID, cCharType_ ) );

    if( !kPacket_.m_mapMissionState.empty() ) {
        for( mit = kPacket_.m_mapMissionState.begin(); mit != kPacket_.m_mapMissionState.end(); ++mit ) {
            KMissionInfo kMissionInfo;
            // 미션 정보 가져오기 실패.
            if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, mit->first ) ) {
                START_LOG( cerr, L"미션 정보 가져오기 실패" )
                    << END_LOG;
            } else {
                // 여기서 DB 프로시저의 구분이 있어야 한다. 
                // 이미 등록된 미션의 경우 기존에 있는 미션 완료 프로시저를 호출.
                // 등록되지 않은 미션의 경우 미션이 완료했다는 로그를 남기는 프로시저 호출.
                if( 1 == mit->second ) {
                    // 미션 완료에 대한 DB처리.
                    if( !CompleteMissionInfo( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, kMissionInfo.m_bUnique, KMissionInfo::DEL_TYPE_USER_COMPLETE, nResult ) ) {
                        START_LOG( cerr, L"미션 완료 처리 실패" )
                            << END_LOG;
                    }
                } else {
                    // 미션 완료에 대한 DB처리.
                    if( !PromotionCompleteMission( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, nResult ) ) {
                        START_LOG( cerr, L"미션 완료 처리 실패" )
                            << END_LOG;
                    }
                }
            }
        }
    }

    // 기본 아이템 지급.
    if( !kPacket_.m_vecDefaultReward.empty() ) {
        std::vector<KDropItemInfo>::iterator vitReward;
        for ( vitReward = kPacket_.m_vecDefaultReward.begin(); vitReward != kPacket_.m_vecDefaultReward.end(); ++vitReward ) {
            if ( false == m_kHelper.InsertSingleItemJob( dwUID_, 
                kPacket_.m_cCharType,
                vitReward->m_ItemID, 
                vitReward->m_nDuration, 
                vitReward->m_nPeriod, 
                KItemManager::EWIT_CHAR_DEFAULT_REWARD, 
                kPacket.m_vecRewardItem ) ) {
                    START_LOG( cerr, L"캐릭터 생성 보상 아이템 지급 실패 UID : " << dwUID_ )
                        << BUILD_LOG( vitReward->m_ItemID )
                        << BUILD_LOG( vitReward->m_nDuration )
                        << BUILD_LOG( vitReward->m_nPeriod )
                        << END_LOG;
                    continue;
            }
        }
    }

    kPacket.m_mapMissionState = kPacket_.m_mapMissionState;

    SET_ERROR( NET_OK );

END_PROC:
        kPacket.m_nOK = NetError::GetLastNetError();
        QUEUING_ACK_TO_USER( EVENT_NEW_CHARACTER_ACK );

        LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
            << L"Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( kPacket.m_nOK )
            << BUILD_LOG( kPacket_.m_ItemID )
            << BUILD_LOG( kPacket_.m_ItemUID )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << BUILD_LOG( kPacket.m_mapMissionState.size() )
            << END_LOG;
}

// 080215 tslayer 대만 정형화
//call dbo.GAS_agreement_system_update
//( LoginUID int )
//( Value int )
//select 1 loop return
//( OK int )
//0 : 성공
//-1 : 해당 유저 정보가 존재하지 않음
//-2 : 해당 유저의 동의 정보가 존재하지 않음
//-101이하 : 트랜잭션 에러
_INT_IMPL_ON_FUNC( EVENT_TW_USER_AGREEMENT_REQ, int )
{
    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.GAS_agreement_system_update ( %d, %d ) }",
        dwUID_, kPacket_ ) ); // DB 기록

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case  0:    SET_ERROR( NET_OK );                   break; // 성공
    case -1:    SET_ERROR( ERR_DB_COMMON_01 );         break;// DB에 해당 유저가 존재하지 않음.
    case -2:    SET_ERROR( ERR_TW_USER_AGREEMENT_00 ); break;// 해당 유저의 동의 정보가 존재하지 않음

    default: SET_ERROR(ERR_DB_COMMON_00); // 트랜잭션, 알 수 없는 에러
    }

    _QUEUING_ACK_TO_USER( EVENT_TW_USER_AGREEMENT_ACK , nOK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L"Error Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( nOK ) << END_LOG;
}

// 이모티콘
INT_IMPL_ON_FUNC( EVENT_EMOTICON_MIX_REQ )
{
    KEVENT_EMOTICON_MIX_ACK kPacket;
    kPacket.m_nOK               = 99;
    kPacket.m_setMixMaterial    = kPacket_.m_setMixMaterial;

    int nType = -99;
    SET_ERROR( ERR_UNKNOWN );

    if( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_dwResultItem, 1, KItem::UNLIMITED_ITEM, KItemManager::EWIT_MIX_EMOTICON, kPacket.m_vecResultItem ) == false )
    {
        START_LOG( cerr, L"이모티콘 조합후 아이템 얻기 DB쿼리 실패. Name : " << strLogin_ )
            << L"Query : " << m_kODBC.m_strLastQuery << END_LOG;
        SET_ERR_GOTO( ERR_EMOTICON_MIX_04, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_EMOTICON_MIX_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK ) 
        << L"이모티콘 DB 작업 : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecResultItem.size() ) << END_LOG;
}


_INT_IMPL_ON_FUNC( EVENT_STAT_GET_ITEM, MAP_ITEM_STAT )
{
    /*
    L"{ call dbo.ISGAItemSupplied_merge_20100603 ( %d, %d, %d, %d ) }"

    { call dbo.ISGAItemSupplied_merge_20100603 ( @1, @2, @3, @4 ) }
    @1 ; @iType_   int
    @2 ; @iItemID_ int
    @3 ; @iFactor_ int
    @4 ; @iCNT_    int

    1 select ( @1 )
    @1 ; OK int
    0        ; 성공
    -101이하 ; 트랜젝션 에러
    */
    MAP_ITEM_STAT::iterator mit;
    int nCount = 0;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.ISGAItemSupplied_merge_20100603 ( %d, %d, %d, %d ) }",
            mit->first.m_nMode, mit->first.m_dwItemID, mit->first.m_nFactor, mit->second ), return, (&m_kODBCStat) );

        int nOK = -99;
        if( m_kODBCStat.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBCStat >> nOK );
            m_kODBCStat.EndFetch();
        }
        ++nCount;

        LOG_SUCCESS( nOK == 0 )
            << L"아이템 획득 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
            << BUILD_LOG( nOK )
            << L"Last Query : " << m_kODBCStat.m_strLastQuery << dbg::endl
            << BUILD_LOG( nCount ) << END_LOG;
    }

/*
20081106. microcat.

{ call dbo.SIS_supplied_item_collect ( N'%s' ) }

{ call dbo.SIS_supplied_item_collect ( @1 ) }
@1 ; Data nvarchar(4000) ; [Type int,ItemID int,Factor int,Count int,]n

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 에러
-2 ; 데이터의 짝이 맞지 않음
-101이하 ; 트랜잭션 에러
*/
/*
    boost::wformat fmter( L"%d,%d,%d,%d," );
    std::wstringstream stm;
    MAP_ITEM_STAT::iterator mit;
    int nCount = 0;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        fmter   % mit->first.m_nMode
                % mit->first.m_dwItemID
                % mit->first.m_nFactor
                % mit->second;

        if( stm.str().size() + fmter.str().size() > 4000 )
        {
            _DBJIF( m_kODBCStat.Query( L"{ call dbo.SIS_supplied_item_collect ( N'%s' ) }",
                stm.str().c_str() ), return, (&m_kODBCStat) );
            int nOK = -99;
            if( m_kODBCStat.BeginFetch() )
            {
                VERIFY_QUERY( m_kODBCStat >> nOK );
                m_kODBCStat.EndFetch();
            }

            LOG_SUCCESS( nOK == 0 )
                << L"아이템 획득 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
                << BUILD_LOG( nOK )
                << L"Last Query : " << m_kODBCStat.m_strLastQuery << dbg::endl;

            stm.str(L"");
            ++nCount;
        }
        stm << fmter.str();
    }

    if( !stm.str().empty() )
    {
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.SIS_supplied_item_collect ( N'%s' ) }",
            stm.str().c_str() ), return, (&m_kODBCStat) );
        int nOK = -99;
        if( m_kODBCStat.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBCStat >> nOK );
            m_kODBCStat.EndFetch();
        }

        LOG_SUCCESS( nOK == 0 )
            << L"아이템 획득 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
            << BUILD_LOG( nOK )
            << L"Last Query : " << m_kODBCStat.m_strLastQuery << dbg::endl
            << BUILD_LOG( nCount ) << END_LOG;
    }

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
*/
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_CONNECTION_MSG_REQ )
{
    std::vector<int> vecMsgID;
    std::vector<std::wstring> vecMessage;

    m_kHelper.GetConnectionMsgUserSelect( dwUID_, vecMsgID );
    if( vecMsgID.empty() ) return;

    std::map<int, std::wstring> mapMessage;
    SiKGCHelper()->GetConnectionMessage( mapMessage );

    std::vector<int>::iterator vit;
    for ( vit = vecMsgID.begin() ; vit != vecMsgID.end() ; ++vit ) {

        std::map<int, std::wstring>::iterator mit;
        mit = mapMessage.find( *vit );
        if ( mit == mapMessage.end() ) {

            std::wstring strMsg;
            m_kHelper.GetConnectionMsgSelect( *vit, strMsg );

            if ( !strMsg.empty() ) {
                mapMessage[*vit] = strMsg;
                vecMessage.push_back( strMsg );
            }
        }
        else {
            vecMessage.push_back( mit->second );
        }
    }

    SiKGCHelper()->SetConnectionMessage( mapMessage );
    if( vecMessage.empty() ) return;

    _QUEUING_ACK_TO_USER( EVENT_CONNECTION_MSG_ACK, vecMessage );

    START_LOG( clog, L"접속 메세지 받음. Name : " << strLogin_ )
        << BUILD_LOG( vecMsgID.size() )
        << BUILD_LOG( vecMessage.size() ) << END_LOG;

}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_DB_UPDATE_RANK_DATA_REQ )
{
    //call dbo.RGR_rank_grandchaser_daily_select
    //call dbo.RGR_rank_grandchaser_weekly_select
    //call dbo.RGR_rank_grandchaser_monthly_select
    //call dbo.RGR_rank_grandchaser_exp_select
    //( LoginUID int = null )
    //( Nick nvarchar(24) = null )
    //( TabType tinyint = 0 )
    //
    //call dbo.RGR_rank_grandchaser_daily_select
    //call dbo.RGR_rank_grandchaser_weekly_select
    //call dbo.RGR_rank_grandchaser_monthly_select
    //리턴값
    //( TabType tinyint )
    //( Rank int )
    //( LoginUID int )
    //( GuildID int )
    //( GuildMark int )
    //( GuildName nvarchar(12) )
    //( CharType tinyint )
    //( Nick nvarchar(24) )
    //( Win int )
    //( Lose int )
    //( WinRate float?double?)
    //
    //call dbo.RGR_rank_grandchaser_exp_select
    //리턴값
    //( TabType tinyint )
    //( Rank int )
    //( LoginUID int )
    //( GuildID int )
    //( GuildMark int )
    //( GuildName nvarchar(12) )
    //( CharType tinyint )
    //( Nick nvarchar(24) )
    //( Exp bigint )

    //사용법
    //
    //예제) 1~100등까지 모든 탭에 대해서 리턴 : return n loop select
    //call dbo.RGR_rank_grandchaser_daily_select
    //call dbo.RGR_rank_grandchaser_weekly_select
    //call dbo.RGR_rank_grandchaser_monthly_select
    //call dbo.RGR_rank_grandchaser_exp_select
    //
    //예제) 특정 유저의 특정탭의 랭킹 정보를 LoginUID를 통해서 리턴 : return 1 loop select
    //call dbo.RGR_rank_grandchaser_daily_select
    //@iLoginUID_ = 숫자숫자
    //,   @iTabType_ = 0~7까지(전체,엘리시스,리르,...,로난 순으로) -> 입력해주지 않을 경우 기본값은 0임
    //
    //예제) 특정 유저의 특정탭의 랭킹 정보를 Nick을 통해서 리턴 : return 1 loop select
    //@strNick_ = N'닉네임'
    //,   @iTabType_ = 0~7까지(전체,엘리시스,리르,...,로난 순으로) -> 입력해주지 않을 경우 기본값은 0임
//-----------------------------------------------------------------------------------------------------
    // 080722. kkurrung.  이걸로 변경.
    //call dbo.RGR_rank_grancahser_XXXX_select_loginuid
    //( LoginUID int )
    //( TabType tinyint = 0 )

    //call dbo.RGR_rank_grancahser_XXXX_select_nick
    //( Nick nvarchar(24) )
    //( TabType tinyint = 0 )

    //call dbo.RGR_rank_grandchaser_XXXX_select_top100
    //( 인자없다능 인자없다능 인자없다능 )

    GetRankingInfo( L"{ call dbo.RGR_rank_grandchaser_daily_select_top100 }",   KRankManager::RT_DAILY_RANK );
    GetRankingInfo( L"{ call dbo.RGR_rank_grandchaser_weekly_select_top100 }",  KRankManager::RT_WEEFLY_RANK );
    GetRankingInfo( L"{ call dbo.RGR_rank_grandchaser_monthly_select_top100 }", KRankManager::RT_MONTHLY_RANK );

    { // Exp
        std::map<char, std::vector<KExpRank> > mapExpRank;
        DBJIF( m_kODBC.Query( L"{ call dbo.RGR_rank_grandchaser_exp_select_top100 }" ) );
        while( m_kODBC.Fetch() != SQL_NO_DATA )
        {
            KExpRank kRankData;
            char cTabType = -1;
            VERIFY_QUERY( m_kODBC >> cTabType
                >> kRankData.m_nRank
                >> kRankData.m_dwLoginUID
                >> kRankData.m_nGuildID
                >> kRankData.m_nGuildMark
                >> kRankData.m_strGuildName
                >> kRankData.m_cCharType
                >> kRankData.m_strNick
                >> kRankData.m_biExp );

            SiKGuildManager()->GetGuildMarkStringName( kRankData.m_nGuildID, kRankData.m_strMarkName );

            if( cTabType > -1 )
                mapExpRank[cTabType-1].push_back( kRankData );
        }

        std::map<char, std::vector<KExpRank> >::iterator mit;
        for( mit = mapExpRank.begin() ; mit != mapExpRank.end() ; ++mit )
        {
            std::sort( mit->second.begin(), mit->second.end(),
                boost::bind(&KExpRank::m_nRank, _1) < boost::bind(&KExpRank::m_nRank, _2) );
        }
        SiKRankManager()->UpdateExpRankData( mapExpRank );
    }
    std::map< int, std::map<char, std::vector<KWinRateRank> > > mapGuildRank;
    //GetGuildTotalRank( mapGuildRank ); // 구형 제거할것.
    /*SiKRankManager()->UpdateWinRankData( KRankManager::RT_GUILD_DAILY_RANK, mapGuildRank[1] );
    SiKRankManager()->UpdateWinRankData( KRankManager::RT_GUILD_WEEKLY_RANK, mapGuildRank[2] );
    SiKRankManager()->UpdateWinRankData( KRankManager::RT_GUILD_MONTHLY_RANK, mapGuildRank[3] );
    SiKRankManager()->UpdateWinRankData( KRankManager::RT_GUILD_TOTAL_RANK, mapGuildRank[4] );*/

    SiKRankManager()->BuildRankData();
}

_INT_IMPL_ON_FUNC( EVENT_DB_MY_RANK_INFO_REQ, std::set<char> )
{
    KMyRankInfo kPacket;
    std::set<char>::iterator sit;
    kPacket_.insert( -1 ); // 전체 랭킹을 얻어 내기 위해서.
    KWinRateRank kRankData;
    KExpRank kExpRankData;
    for( sit = kPacket_.begin() ; sit != kPacket_.end() ; ++sit ) {
        GetRankInfo( dwUID_, *sit, kPacket );
    }

    QUEUING_ACK_TO_USER( EVENT_MY_RANK_INFO_NOT );

    START_LOG( clog, L"자신의 랭킹 정보 가져오기. Name : " << strLogin_ )
        << BUILD_LOG( kPacket.m_mapDailyRank.size() )
        << BUILD_LOG( kPacket.m_mapWeeklyRank.size() )
        << BUILD_LOG( kPacket.m_mapMonthlyRank.size() )
        << BUILD_LOG( kPacket.m_mapExpRank.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_RANK_SEARCH_REQ )
{
    EVENT_CHECK_OUT( EVENT_RANK_SEARCH_REQ );

    SET_ERROR( ERR_UNKNOWN );
    KEVENT_RANK_SEARCH_ACK kPacket;
    kPacket.m_cRankType = kPacket_.m_cRankType;
    kPacket.m_cCharType = kPacket_.m_cCharType;
    kPacket.m_kExpRank.m_nRank = 0;
    kPacket.m_kWinRank.m_nRank = 0;
    char cTabType = -1;

    std::wstringstream stm;
    switch( kPacket_.m_cRankType )
    {
    case KRankManager::RT_DAILY_RANK:
        stm << L"{ call dbo.RGR_rank_grandchaser_daily_select_nick";
        break;
    case KRankManager::RT_WEEFLY_RANK:
        stm << L"{ call dbo.RGR_rank_grandchaser_weekly_select_nick";
        break;
    case KRankManager::RT_MONTHLY_RANK:
        stm << L"{ call dbo.RGR_rank_grandchaser_monthly_select_nick";
        break;
    case KRankManager::RT_EXP_RANK:
        stm << L"{ call dbo.RGR_rank_grandchaser_exp_select_nick";
        break;
    case KRankManager::RT_GUILD_DAILY_RANK:
    case KRankManager::RT_GUILD_WEEKLY_RANK:
    case KRankManager::RT_GUILD_MONTHLY_RANK:
    case KRankManager::RT_GUILD_TOTAL_RANK:

        GetGuildRank( kPacket_.m_cRankType, kPacket_.m_strNickName, kPacket.m_kWinRank );
        SET_ERROR( NET_OK );
        goto END_PROC; // 에러는 아니지만, 볼 일 다 봤으므로  끝으로 간다
        break;

    default:
        START_LOG( cerr, L" 랭킹 검색시 알수 없는 랭킹 타입을 보내옴.. Name : " << strLogin_ )
            << BUILD_LOGc( kPacket_.m_cRankType )
            << BUILD_LOGc( kPacket_.m_cCharType )
            << BUILD_LOG( kPacket_.m_strNickName ) << END_LOG;
        SET_ERR_GOTO( ERR_RANK_05, END_PROC );
        
    }
    stm << L" ( N'" << kPacket_.m_strNickName << L"', " << (int)(kPacket_.m_cCharType + 1) << L" ) }";

    DBJIF( m_kODBC.Query( L"%s", stm.str().c_str() ) );

    if( !m_kODBC.BeginFetch() ) {
        SET_ERR_GOTO( ERR_RANK_08, END_PROC );
    }

    if( KRankManager::RT_EXP_RANK == kPacket_.m_cRankType ) {
        VERIFY_QUERY( m_kODBC >> cTabType
            >> kPacket.m_kExpRank.m_nRank
            >> kPacket.m_kExpRank.m_dwLoginUID
            >> kPacket.m_kExpRank.m_nGuildID
            >> kPacket.m_kExpRank.m_nGuildMark
            >> kPacket.m_kExpRank.m_strGuildName
            >> kPacket.m_kExpRank.m_cCharType
            >> kPacket.m_kExpRank.m_strNick
            >> kPacket.m_kExpRank.m_biExp );
    }
    else {
        VERIFY_QUERY( m_kODBC >> cTabType
            >> kPacket.m_kWinRank.m_nRank
            >> kPacket.m_kWinRank.m_dwLoginUID
            >> kPacket.m_kWinRank.m_dwGuildUID
            >> kPacket.m_kWinRank.m_nGuildMark
            >> kPacket.m_kWinRank.m_strGuildName
            >> kPacket.m_kWinRank.m_cCharType
            >> kPacket.m_kWinRank.m_strNick
            >> kPacket.m_kWinRank.m_dwWin
            >> kPacket.m_kWinRank.m_dwLose
            >> kPacket.m_kWinRank.m_fWinRate );
    }
    m_kODBC.EndFetch();

    SET_ERROR( NET_OK );
END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    QUEUING_ACK_TO_USER( EVENT_RANK_SEARCH_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_GET_HELL_ITEM_REQ, int )
{
    EVENT_CHECK_OUT( EVENT_GET_HELL_ITEM_REQ );

    JIF( kPacket_ > 0 );
    std::vector<KItem> kPacket;

    GCITEMID dwHellItemID = SiKGCHelper()->GetHellItemID();
    if( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, dwHellItemID, kPacket_, KItem::UNLIMITED_ITEM, KItemManager::EWIT_ATTEND_REWARD, kPacket ) == false )
    {
        START_LOG( cerr, L"헬 아이템 요청시 오류.. Name : " << strLogin_ ) << END_LOG;
        return;
    }

    QUEUING_ACK_TO_USER( EVENT_GET_HELL_ITEM_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_DB_UPDATE_SELL_ITEM_LIST_REQ )
{
    SelectSellItemList();
}

_INT_IMPL_ON_FUNC( EVENT_SELL_COUNT_ITEM_REQ, KDBSellCountItemData )
{
    EVENT_CHECK_OUT( EVENT_SELL_COUNT_ITEM_REQ );
    kPacket_.m_nGP = 0;
    __int64 waitUID = 0;
    // 기본 GP등의 정보를 동기화 시킨다.

    if( !UpdateUserData( kPacket_.m_kData ) )
    {
        START_LOG( cerr, L"게임서버/DB간 사용자 정보 동기화 실패." )
            << BUILD_LOG( strLogin_ )
            << END_LOG;
        SET_ERROR( ERR_DB_COMMON_03 );
        goto sell_end;
    }

    // 판매 하려는 아이템중 변경 정보를 DB에 적용한다.
    int nDellFactor = kPacket_.m_kReqItem.m_nCount - kPacket_.m_kReqItem.m_nInitCount;
    if( nDellFactor != 0 )
    {
        if( UpdateCountItem( dwUID_, kPacket_.m_kReqItem.m_ItemUID, nDellFactor ) == false )
        {
            START_LOG( cerr, L"수량 아이템 판매시 판매 하려는 수량 아이템 동기화 실패. Name : " << strLogin_ )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
            SET_ERR_GOTO( ERR_INVEN_13, sell_end );
        }
    }
    // 차이 수량 처리 했으므로 아이템 개수 같게 만듬.
    kPacket_.m_kReqItem.m_nInitCount = kPacket_.m_kReqItem.m_nCount;

    // 수량 만큼 판매 하고.
    if ( !m_kHelper.DivideSellItem( dwUID_, kPacket_.m_kReqItem.m_ItemUID, kPacket_.m_kReqItem.m_ItemID, kPacket_.m_nSellFactor, kPacket_.m_nGP, waitUID, cCharType_ ) ) {
        // 나눠 팔기 실패.
        SET_ERR_GOTO( ERR_SELL_COUNT_ITEM_06, sell_end );
    }

    // 대기 아이템 정보가 유효 하다면 그것도 가져 온다.
    if ( waitUID != KItem::INVALID_ITEM ) {
        LIF( m_kHelper.InsertSingleWaitItemToInvenOrVDepot( dwUID_, cCharType_, waitUID, KItemManager::EWIT_ITEM_SELL, kPacket_.m_vecDurationItem ) );
    }

    // 성공적으로 팔렸으면 팔려고 했던 아이템을 갱신 한다.
    kPacket_.m_kReqItem.m_nCount     -= kPacket_.m_nSellFactor;
    kPacket_.m_kReqItem.m_nInitCount -= kPacket_.m_nSellFactor;

    SET_ERROR( NET_OK );
sell_end:

    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_SELL_COUNT_ITEM_ACK, kPacket_ );

    START_LOG( clog, L"수량 아이템 DB 작업 종료 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_nGP )
        << BUILD_LOG( kPacket_.m_nSellFactor )
        << BUILD_LOG( kPacket_.m_kReqItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kReqItem.m_ItemUID )
        << BUILD_LOG( waitUID )
        << BUILD_LOG( kPacket_.m_vecDurationItem.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_CURRENT_VIRTUAL_CASH_NOT )
{
    int nBalance = m_kHelper.GetCurrentVirtualCash( dwUID_, strLogin_ );
    _QUEUING_ACK_TO_USER( EVENT_CURRENT_VIRTUAL_CASH_NOT, nBalance );
}

_INT_IMPL_ON_FUNC( EVENT_BUY_VIRTUAL_CASH_REQ, KDB_EVENT_BUY_VIRTUAL_CASH_REQ )
{
    EVENT_CHECK_OUT( EVENT_BUY_VIRTUAL_CASH_REQ );
    KEVENT_BUY_VIRTUAL_CASH_ACK kPacket;
    kPacket.m_vecInv.clear();
    
    int nCurrentVCash = 0;
    int nPrice = 0;
    int nReservedItem = 0;
    std::vector< boost::tuple< int, int, int > > vecItemList; // ID,Factor,Type
    DWORD dwReceiveUID = 0;

    kPacket.m_dwEventUID = kPacket_.m_dwEventUID;

    // 소셜 커머스 아이템일 경우 처리
    if( true == SiKSocialCommerce()->IsRun() && 0 != kPacket_.m_dwEventUID ) {
        // 일단 판매 종료 된 상태인지 확인
        if ( false == SiKSocialCommerce()->CheckSellEnable( kPacket_.m_dwEventUID ) ) {
            SET_ERR_GOTO( ERR_BILL_15, END_PROC );
        }
        int nCurrentSellCount = 0;
        // 현재까지 판매 된 수량 가져 옴
        m_kHelper.GetSellCountByEventUID_SocialCommerce( kPacket_.m_dwEventUID, nCurrentSellCount );
        // 수량을 바탕으로 판매 상태 갱신
        SiKSocialCommerce()->UpdateSellCountAtBuyCashItem( kPacket_.m_dwEventUID, nCurrentSellCount );
        // 판매 가능한지 다시 확인
        if ( false == SiKSocialCommerce()->CheckSellEnable( kPacket_.m_dwEventUID ) ) {
            SET_ERR_GOTO( ERR_BILL_15, END_PROC );
        }
    }

    if ( false == UpdateUserData( kPacket_.m_kUserData ) ) {
        START_LOG( cerr, L"게임서버/DB간 사용자 정보 동기화 실패." )
            << BUILD_LOG( strLogin_ )
            << END_LOG;
    }

    // 현재 잔액을 알아 온다.
    nCurrentVCash = m_kHelper.GetCurrentVirtualCash( dwUID_, strLogin_ );

    if ( nCurrentVCash < 0 ) {
        SET_ERR_GOTO( ERR_BILL_07, END_PROC );
    }

    if ( false == CheckBuyLimitItem( dwUID_, kPacket_.m_kBuyInfo.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_BILL_13, END_PROC );
    }

    if( kPacket_.m_bPresent ) { // 선물일 경우..,
        boost::replace_all( kPacket_.m_strComment, L"'", L"''" );
        if( kPacket_.m_strOwnerNickName.empty() ) {
            SET_ERR_GOTO( ERR_BILL_17, END_PROC );
        }

        if( !m_kHelper.NickToUserUID( kPacket_.m_strOwnerNickName, dwReceiveUID ) ) {
            SET_ERR_GOTO( ERR_BILL_18, END_PROC );
        }

        if ( dwUID_ == dwReceiveUID ) {
            SET_ERR_GOTO( ERR_BILL_19, END_PROC );
        }
    }

    // 아이템 가격을 알아 온다.
    KBuySellItemReq& kBuyInfo = kPacket_.m_kBuyInfo;
    kBuyInfo.m_nPrice = 0;
    m_kHelper.GetPrice( kBuyInfo.m_ItemID, kBuyInfo.m_nCount, kBuyInfo.m_nPeriod, kBuyInfo.m_nPrice, kBuyInfo.m_nMoneyType );

    if ( kBuyInfo.m_nPrice < 0 ) {
        SET_ERR_GOTO( ERR_BILL_05, END_PROC );
    }

    if ( kBuyInfo.m_nPrice > nCurrentVCash ) {
        SET_ERR_GOTO( ERR_BILL_12, END_PROC );
    }

    if( kPacket_.m_bPresent ) {
        LIF( InsertPresentPost( kPacket_.m_strSendNickName, dwReceiveUID, kPacket_ ) );
    } else {
        // 가상 캐시로 캐시 아이템 구매시 GetType을 EWIT_CASH_BUY(1)이 아닌 EWIT_VCASH_BUY(3)으로 남기도록 수정한다
        // "추후에 가상 캐시 전용 가격을 설정 하지는 않을 것 같다"라고 오규운님께 답변받았다
        kBuyInfo.m_nMoneyType = KItemManager::EMT_VIRTUALCASH;

        __int64 waitItemUID = 0;
        if ( 0 != m_kHelper.BuyItemToWait_Mixed( dwUID_, strLogin_, kBuyInfo, KItemManager::EMT_VIRTUALCASH, kBuyInfo.m_nPrice, std::wstring(L""), waitItemUID ) ) {
            SET_ERR_GOTO( ERR_BILL_01, END_PROC );
        }

        m_kHelper.UpdateWaitItemToEnabled( waitItemUID );

        CommitBuyLimitItem( dwUID_, kPacket_.m_kBuyInfo.m_ItemID );
        m_kHelper.UpdateMoneyConsumptionStat( dwUID_, KItemManager::EMC_VP, kBuyInfo.m_nCount, waitItemUID );

        if ( false == m_kHelper.InsertSingleWaitItemToInvenOrVDepot( dwUID_, kPacket_.m_kBuyInfo.m_cCharType, waitItemUID, KItemManager::EMT_VIRTUALCASH, kPacket.m_vecInv ) ) {
            SET_ERR_GOTO( ERR_BILL_01, END_PROC );
        }
    }

    if ( false == m_kHelper.VirtualCashMergeWithType( dwUID_, kBuyInfo.m_ItemID, KVirtualCash::EVCMT_SUB_VCASH_ITEM_BUY, 0, kBuyInfo.m_nPrice ) ) {
        SET_ERR_GOTO( ERR_BILL_09, END_PROC );
    }

    kPacket.m_kVirtualCashInfo.m_dwDeductCashPoint = kBuyInfo.m_nPrice;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    kPacket.m_kVirtualCashInfo.m_dwCashPoint = static_cast<DWORD>( m_kHelper.GetCurrentVirtualCash( dwUID_, strLogin_ ) );

    _LOG_SUCCESS( kPacket.m_nOK == 0 || kPacket.m_nOK == NetError::ERR_BILL_11, L"가상 캐쉬 구매 로그, Name : " << strLogin_ )
        << BUILD_LOG( kPacket_.m_kBuyInfo.m_ItemID )
        << BUILD_LOG( kPacket_.m_kBuyInfo.m_nCount )
        << BUILD_LOG( kPacket.m_kVirtualCashInfo.m_dwCashPoint )
        << BUILD_LOG( kPacket.m_kVirtualCashInfo.m_dwDeductCashPoint )
        << BUILD_LOG( kPacket.m_vecInv.size() )
        << BUILD_LOG( nCurrentVCash )
        << BUILD_LOG( kBuyInfo.m_nPrice )
        << BUILD_LOG( nReservedItem )
        << BUILD_LOG( vecItemList.size() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_BUY_VIRTUAL_CASH_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_SET_ABUSE_TO_DB_REQ )
{
    //call dbo.RCR_rank_character_abuse
    //( LoginUID int )
    //
    //return 1 loop select
    //( OK int )
    //
    //OK
    //0 : 어뷰징 셋팅 성공
    //-1 : 해당 유저 정보가 존재하지 않음
    //-2 : 이미 어뷰징 유저로 셋팅되어 있음
    //-101이하 : 트랜잭션 에러

    DBJIF( m_kODBC.Query( L"{ call dbo.RCR_rank_character_abuse ( %d ) }",
        dwUID_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }
    
    LOG_CONDITION( nOK == 0, clog, cwarn )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_DB_STAT_GAME_LOADING_TIME, KEVENT_MAP_DWORD_DWORD )
{
    JIF( !kPacket_.empty() );
    //DB : Stat
    //
    //call dbo.LTS_loading_time_collect
    //( Data nvarchar(4000) ) : [Second tinyint,Count int,]n
    //
    //select 1 loop return
    //( OK int )
    //0 : 방법성공
    //-1 : 데이터 파싱과정중 에러1
    //-2 : 데이터 파싱과정중 에러2
    //-101이하 : 트랜잭션 에러
    //
    /*
    L"{ call dbo.LTGALoadingTime_merge_20110525 ( %d, %d ) }"

    { call dbo.LTGALoadingTime_merge_20110525 ( @1, @2 ) }
    @1: @iSecond_ int
    @2: @iCNT_    int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜젝션 에러
    */

    //std::wstringstream stm;
    int nOK = -99;

    KEVENT_MAP_DWORD_DWORD::iterator mit;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.LTGALoadingTime_merge_20110525 ( %d, %d ) }",
            mit->first, mit->second ), return, (&m_kODBCStat) );
        
        if ( m_kODBCStat.BeginFetch() ) {
            VERIFY_QUERY( m_kODBCStat >> nOK );
        }
        m_kODBCStat.EndFetch();
        //stm << mit->first << L"," << mit->second << L",";
    }
    //_DBJIF( m_kODBCStat.Query( L"{ call dbo.LTGALoadingTime_merge_20110525 ( N'%s' ) }",
    //    stm.str().c_str() ), return, (&m_kODBCStat) );

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery ) << END_LOG;

    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

_INT_IMPL_ON_FUNC( EVENT_COLLECTION_COMPLETE_REQ, int )
{
    KEVENT_COLLECTION_COMPLETE_ACK kPacket;
    KMissionInfo kMissionInfo;

    kPacket.m_dwMissionID = kPacket_;

    if( !SiKCollectionManager()->GetMissionInfo( kMissionInfo, (DWORD)kPacket_ ) )
    {
        SET_ERROR( ERR_TITLE_04 ); // 미션 정보 가져오기 실패
        goto END_PROC;
    }

    int nOK = CheckCollectionInfo( dwUID_, cCharType_, kPacket_ );
    switch( nOK )
    {
    case  0:    SET_ERROR( ERR_TITLE_05 );        break;
    case  1:    SET_ERROR( NET_OK );              break;
    case  2:    SET_ERROR( ERR_TITLE_01 );        break;

    default: SET_ERROR( ERR_UNKNOWN );
        START_LOG( cerr, L"컬렉션 완료 중 에러" << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        goto END_PROC;
    }

    if( !kMissionInfo.m_vecRewardItem.empty() )
    {
        std::vector<KMissionReward>::iterator vit;
        for( vit = kMissionInfo.m_vecRewardItem.begin() ; vit != kMissionInfo.m_vecRewardItem.end() ; ++vit )
        {
            if( !m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, KItemManager::EWIT_MISSION, kPacket.m_vecInv ) )
            {
                START_LOG( cerr, L"컬렉션 완료 아이템 보상시 DB 오류 발생, Name : " << strLogin_  )
                    << BUILD_LOG( kPacket_ )
                    << BUILD_LOG( dwUID_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nCount )
                    << BUILD_LOG( vit->m_nPeriod )
                    << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                SET_ERR_GOTO( ERR_TITLE_08, END_PROC );
            }
        }
    }

    /*
    L"{ call dbo.TCGATitleCollectionMain_update_20130516 ( %d, %d, %d ) }"

    { call dbo.TCGATitleCollectionMain_update_20130516 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input      int
    @02: @02iCharType_input      int
    @03: @03iMainMissionID_input int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 진행중인 칭호가 아님
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.TCGATitleCollectionMain_update_20130516 ( %d, %d, %d ) }",
        dwUID_, cCharType_, kPacket_ ) );

    nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    switch( nOK )
    {
    case  0:    SET_ERROR( NET_OK );        break;            
    case -1:    SET_ERROR( ERR_TITLE_00 );  goto END_PROC;
    case -2:    SET_ERROR( ERR_TITLE_01 );  goto END_PROC;

    default: SET_ERROR( ERR_UNKNOWN );
        START_LOG( cerr, L"컬렉션 완료 중 에러" << strLogin_ )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_ )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        goto END_PROC;
    }

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_COLLECTION_COMPLETE_ACK );
}

//-- 게임모드별 내 최고 성적을 리턴합니다.
//call dbo.MGG_mini_game_select
//( @iLoginUID_    int)
//
//return n loop select
//    GameMode    tinyint
//,   CharType    tinyint
//,   BestPoint   int
//여기서 n은 게임 모드의 수만큼이 되겠습니다.
INT_IMPL_ON_FUNC_NOPARAM( EVENT_MINIGAME_INFO_NOT )
{
    std::vector<KMiniGameRankInfo> kPacket;
    KMiniGameRankInfo kMiniGameInfo;
    DBJIF( m_kODBC.Query( L"{ call dbo.MGG_mini_game_select ( %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> kMiniGameInfo.m_cGameMode 
            >> kMiniGameInfo.m_cCharType
            >> kMiniGameInfo.m_dwBestScore );

        kPacket.push_back( kMiniGameInfo );
    }

    QUEUING_ACK_TO_USER( EVENT_MINIGAME_INFO_NOT );
}

//내 최고기록을 갱신합니다.
//call dbo.MGG_mini_game_update
//     @tiGameMode_    int
// ,   @iLoginUID_         int
// ,   @tiCharType_      tinyint
// ,   @iPoint_ [int],
//     @iPlayTime_ [int]
// return 1 loop select
// OK        int
// 0 : 변경 성공
// -1 : 플레이 한 유저가 없음
// -2 : 현재 최고 기록보다 갱신하려는 Point가 작음
// -101 이하 : 트랜젝션 에러
INT_IMPL_ON_FUNC( EVENT_DB_UPDATE_MINIGAME_SCORE )
{
    DBJIF( m_kODBC.Query( L"{ call dbo.MGG_mini_game_update ( %d, %d, %d, %d, %d ) }",
        dwUID_, static_cast<int>(kPacket_.m_kMiniGameRankInfo.m_cGameMode), 
        static_cast<int>(kPacket_.m_kMiniGameRankInfo.m_cCharType), kPacket_.m_kMiniGameRankInfo.m_dwScore, kPacket_.m_dwPlayTime ) );

    int nOK = 0;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case 0:  SET_ERROR( NET_OK );          break;
    case -1: SET_ERROR( ERR_MINIGAME_01 ); break;
    case -2: SET_ERROR( ERR_MINIGAME_02 ); break;
    default:
        break;
    }

    kPacket_.m_kMiniGameRankInfo.m_nOK = NetError::GetLastNetError();

    // 에러값 세팅해서 유저로 넘겨줘야 되겠지
   _QUEUING_ACK_TO_USER( EVENT_END_MINIGAME_ACK, kPacket_.m_kMiniGameRankInfo );

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( kPacket_.m_kMiniGameRankInfo.m_cGameMode )
        << BUILD_LOGc( kPacket_.m_kMiniGameRankInfo.m_cCharType )
        << BUILD_LOG( kPacket_.m_kMiniGameRankInfo.m_dwScore )
        << BUILD_LOG( kPacket_.m_dwPlayTime )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

//미니게임 통계입니다.
//call dbo.MGS_mini_game_statistics
//@strData_    [nvarchar](4000)
//
//@strData_ = N'GameMode,Level,HitRange,UserCount,PlayTime,GameMode,Level,HitRange,UserCount,PlayTime,...'
//
//return 1 loop select
//OK    int
//
//OK value
// 0 : 통계입력 성공
//-1 : 파싱에러
//-2 : 파싱에러
//-101이하 : 트랜젝션 에러

_INT_IMPL_ON_FUNC( EVENT_STAT_MINIGAME_INSERT, MAP_MINIGAME_STAT )
{
    JIF( !kPacket_.empty() );

    boost::wformat fmter( L"%d,%d,%d,%d,%d," ); // Level, gamemode, hit range , users , play time (minutes)
    std::wstringstream stm;
    MAP_MINIGAME_STAT::iterator mit;
    int nCount = 0;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        fmter % (int)mit->first.m_cGameMode
              % mit->first.m_nLevel
              % mit->first.m_nHitRange // 점수 범위 , 2008-07-16 현재 30 점이 한 단계
              % mit->second.first     // User Count
              % mit->second.second;   // Play time

        if( stm.str().size() + fmter.str().size() > 3500 ) // 스트링을 만들다가 용량 한계 이상이면 바로 기록하겠다는 뜻
        {
            _DBJIF( m_kODBCStat.Query( L"{ call dbo.MGS_mini_game_statistics ( N'%s' ) }",
                stm.str().c_str() ), return, (&m_kODBCStat) );
            int nOK = -99;
            if( m_kODBCStat.BeginFetch() )
            {
                VERIFY_QUERY( m_kODBCStat >> nOK );
                m_kODBCStat.EndFetch();
            }

            LOG_SUCCESS( nOK == 0 )
                << L"미니게임 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
                << BUILD_LOG( nOK )
                << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
            stm.str(L""); // 비우기.
            ++nCount;
        }
        stm << fmter.str();
    }

    if( !stm.str().empty() ) // 위의 for loop 에서 용량한계가 오지 않으면 여기서 실행
    {
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.MGS_mini_game_statistics ( N'%s' ) }",
            stm.str().c_str() ), return, (&m_kODBCStat) );
        int nOK = -99;
        if( m_kODBCStat.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBCStat >> nOK );
            m_kODBCStat.EndFetch();
        }

        LOG_SUCCESS( nOK == 0 )
            << L"미니게임 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
            << BUILD_LOG( nOK )
            << L"Last Query : " << m_kODBCStat.m_strLastQuery << dbg::endl
            << BUILD_LOG( nCount ) << END_LOG;
    }

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}



_INT_IMPL_ON_FUNC( EVENT_UNLOCK_SKILL_REQ, KDB_EVENT_UNLOCK_SKILL_REQ )
{
    EVENT_CHECK_OUT( EVENT_UNLOCK_SKILL_REQ );
    KEVENT_UNLOCK_SKILL_ACK kPacket;
    kPacket.m_cCharType = kPacket_.m_CharUnlockGroupID.first;
    kPacket.m_nUnLockGroupID = kPacket_.m_CharUnlockGroupID.second;
    LIF( AddUnLockGroupID( dwUID_, kPacket_.m_CharUnlockGroupID.first, kPacket_.m_CharUnlockGroupID.second ) );

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kItem.m_ItemUID, kPacket_.m_kItem.m_nInitCount, kPacket_.m_kItem.m_nCount ) ) {
        START_LOG( clog, L"스킬 언락 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kItem.m_nCount )
            << END_LOG;
    }

    if ( !UseCountItem( dwUID_, kPacket_.m_kItem.m_ItemUID, SiKSkillManager()->GetUnLockItemCount() ) ) {
        START_LOG( cerr, L"스킬 언락 아이템 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        kPacket.m_kItem.m_nCount = 0;
        SET_ERROR( ERR_SKILL_30 );
    }

    kPacket_.m_kItem.m_nCount -= SiKSkillManager()->GetUnLockItemCount();
    kPacket.m_kItem = kPacket_.m_kItem;

    if ( IS_CORRECT( ERR_SKILL_30) ) {
        kPacket.m_kItem.m_nCount = 0;
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_UNLOCK_SKILL_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_STAT_SPX_COUNT, MAP_SPX_STAT )
{
    MAP_SPX_STAT::iterator mit;
    int nCount = 0;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        if( UpdateStatSpxCount( mit->first.m_nModeID, mit->first.m_cCharType, mit->first.m_nLevel, mit->first.m_nSPLevel, mit->first.m_nMaxSpx, mit->second ) )
            ++nCount;
    }

    START_LOG( clog, L"SPX 통계 남기기 완료. Tota : " << kPacket_.size() << L", 성공 : " << nCount ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_CHANGE_COUPON_REQ, KEVENT_CHANGE_COUPON_ACK )
{
    EVENT_CHECK_OUT( EVENT_CHANGE_COUPON_REQ );

    if( !UpdateUserData( kPacket_.m_kData ) ) {
        SET_ERR_GOTO( ERR_COUPON_03, END_PROC ); // 필수데이터 동기화 실패.
    }

    if( kPacket_.m_kCouponItem.m_nCount == KItem::UNLIMITED_ITEM ) {
        if ( !PureEraseItem( strLogin_, dwUID_, kPacket_.m_kItem.m_dwUID, cCharType_ ) ) {
            START_LOG( cerr, L"쿠폰 전환시 아이템 제거 실패. Name : " << strLogin_ )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

            SET_ERR_GOTO( ERR_COUPON_03, END_PROC );

        }
    }

    // GP 변경된 부분 갱신.
    int nOK = -1;
    int nGP = 0;
    UpdateUserGamePoint( dwUID_, cCharType_, kPacket_.m_kData.m_dwSlotNum, kPacket_.m_nGP, nOK, nGP );
    if( nOK != 0 ) {
        START_LOG( cerr, L"GP 갱신 에러")
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;

        SET_ERR_GOTO( ERR_COUPON_04, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_CHANGE_COUPON_ACK, kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_STAT_DISCONNECT_COUNT, MAP_INT_DWORD )
{
    std::wstringstream stm;
    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    USHORT usMaxString = 3500; // SP Parameter 데이터 타입이 varchar( 4000 ) 이므로 제한 필요.
    MAP_INT_DWORD::iterator mit;
    int nOK = -99;

    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit ) {
        if( stm.str().size() == 0 ) {
            stm << L"{ call dbo.DTC_disconnect_type_collect ( '";
        }
        stm << mit->first << L","
            << mit->second << L",";
        if( stm.str().size() > usMaxString ) {
            stm << L"' ) }";
            _DBJIF( m_kODBCStat.QueryToWString( stm.str() ), return, (&m_kODBCStat) );
            if( m_kODBCStat.BeginFetch() ) {
                VERIFY_QUERY( m_kODBCStat >> nOK );
            }
            m_kODBCStat.EndFetch();
            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        }
    }
    if( stm.str().size() != 0 ) {
        stm << L"' ) }";
        _DBJIF( m_kODBCStat.QueryToWString( stm.str() ), return, (&m_kODBCStat) );
        if( m_kODBCStat.BeginFetch() ) {
            VERIFY_QUERY( m_kODBCStat >> nOK );
        }
        m_kODBCStat.EndFetch();
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBCStat.m_strLastQuery ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_GACHA_ACTION_REQ, KDB_EVENT_GACHA_ACTION_REQ )
{
    EVENT_CHECK_OUT( EVENT_GACHA_ACTION_REQ );

    std::vector< KItem >::iterator vit;
    KEVENT_GACHA_ACTION_ACK kPacket;
    kPacket.m_nVersion              = kPacket_.m_nVersion;
    kPacket.m_gachaItem.m_ItemID    = kPacket_.m_ItemID;
    kPacket.m_nLevel                = kPacket_.m_nLevel;
    kPacket.m_bRewardType           = kPacket_.m_bRewardType;

    int nItemType = -99;
    int nNeedKeyCount = INT_MAX;
    nNeedKeyCount = kPacket_.m_nNeedKeyCount;

    SET_ERROR( ERR_UNKNOWN );

    // 가챠 아이템 & 열쇠 아이템의 개수 동기화가 가능한지 우선 체크.
    if ( false == CheckUpdateCountItem( dwUID_, kPacket_.m_kGachaItem.m_ItemID, kPacket_.m_kGachaItem.m_cGradeID, 1, cCharType_ ) ) {
        START_LOG( cerr, L"가챠 아이템 개수 체크 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kGachaItem.m_ItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_GACHA_12, END_PROC );
    }

    if ( false == CheckUpdateCountItem( dwUID_, kPacket_.m_keyItem.m_ItemID, kPacket_.m_keyItem.m_cGradeID, nNeedKeyCount, cCharType_ ) ) {
        START_LOG( cerr, L"가챠 열쇠 아이템 개수 체크 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_keyItem.m_ItemUID )
            << BUILD_LOG( nNeedKeyCount ) << END_LOG;
        SET_ERR_GOTO( ERR_GACHA_13, END_PROC );
    }

    if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kPostItem.m_ItemID, kPacket_.m_kPostItem.m_nDuration, kPacket_.m_kPostItem.m_nPeriod, KItemManager::EWIT_GACHA, kPacket.m_vecReward ) ) {
        SET_ERR_GOTO( ERR_DB_COMMON_00, END_PROC );
    }

    for ( vit = kPacket.m_vecReward.begin(); vit != kPacket.m_vecReward.end(); ++vit ) {
        bool bUpdateSuccess = true;
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateLevel(dwUID_, kPacket_.m_nLevel, KItemLevelTable::IL_EQUIP_LV, *vit, cCharType_ ) );

        if ( false == bUpdateSuccess ) {
            kPacket.m_nOK = NetError::ERR_DB_COMMON_00;
        }
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_kGachaItem.m_ItemUID, kPacket_.m_kGachaItem.m_nInitCount, kPacket_.m_kGachaItem.m_nCount ) ) {
        START_LOG( clog, L"가챠 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_kGachaItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_kGachaItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_kGachaItem.m_nCount )
            << END_LOG;
    }

    if ( false == UseCountItem( dwUID_, kPacket_.m_kGachaItem.m_ItemUID, 1 ) ) {
        START_LOG( cerr, L"가챠 아이템 개수 동기화 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kGachaItem.m_ItemUID ) << END_LOG;
        SET_ERR_GOTO( ERR_GACHA_12, END_PROC );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket_.m_keyItem.m_ItemUID, kPacket_.m_keyItem.m_nInitCount, kPacket_.m_keyItem.m_nCount ) ) {
        START_LOG( clog, L"가챠 열쇠 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket_.m_keyItem.m_ItemUID )
            << BUILD_LOG( kPacket_.m_keyItem.m_nInitCount )
            << BUILD_LOG( kPacket_.m_keyItem.m_nCount )
            << END_LOG;
    }

    if ( false == UseCountItem( dwUID_, kPacket_.m_keyItem.m_ItemUID, nNeedKeyCount ) ) {
        START_LOG( cerr, L"가챠 열쇠 아이템 개수 동기화 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_keyItem.m_ItemUID )
            << BUILD_LOG( nNeedKeyCount ) << END_LOG;
        SET_ERR_GOTO( ERR_GACHA_13, END_PROC );
    }

    kPacket_.m_kGachaItem.m_nCount -= 1;
    kPacket_.m_keyItem.m_nCount -= nNeedKeyCount;

    kPacket.m_gachaItem = kPacket_.m_kGachaItem;
    kPacket.m_keyItem = kPacket_.m_keyItem;

    // 가차 아이템 로그
    int nRewardInfo = KGachaManager::GRI_FAIL_REWARD;   // true: 성공 보상, false: 꽝보상
    if ( kPacket_.m_bRewardType ) {
        nRewardInfo = KGachaManager::GRI_EQUIP_REWARD;
    }
    LIF( Insert_GachaSystem_RewardLog_ToDB( 
            kPacket_.m_nVersion,
            dwUID_,
            kPacket_.m_cCharType,
            static_cast< int >( kPacket_.m_dwCharLevel ),
            kPacket_.m_nLevel,
            kPacket_.m_kPostItem.m_ItemID,
            nRewardInfo,
            kPacket_.m_kPostItem.m_nDuration,
            nNeedKeyCount
        )
    );

END_PROC:
    if ( IS_CORRECT( ERR_GACHA_12 ) ) {
        kPacket.m_gachaItem.m_nCount = 0;
    } 
    if ( IS_CORRECT( ERR_GACHA_13 ) ) {
        kPacket.m_keyItem.m_nCount = 0;
    }
    if ( !IS_CORRECT( ERR_GACHA_12 ) && !IS_CORRECT( ERR_GACHA_13 ) ) {
        SET_ERROR( NET_OK );
    }
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_GACHA_ACTION_ACK );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPostItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kPostItem.m_nDuration )
        << BUILD_LOG( kPacket_.m_kPostItem.m_nPeriod )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nVersion )
        << BUILD_LOG( kPacket.m_vecReward.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_GACHA_SET_REWARD_REQ )
{
    EVENT_CHECK_OUT( EVENT_GACHA_SET_REWARD_REQ );

    KEVENT_GACHA_SET_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nVersion = kPacket_.m_nVersion;
    kPacket.m_gachaItemID = kPacket_.m_gachaItemID;
    kPacket.m_dwType = kPacket_.m_dwType;
    kPacket.m_nLevel = kPacket_.m_nLevel;
    std::vector< KDropItemInfo > vecItems;
    std::vector< KDropItemInfo >::iterator vit;
    std::vector< KItem >::iterator vitLevel;

    SET_ERROR( ERR_UNKNOWN );
    if ( false == SiKGachaManager()->GetGachaSetReward( kPacket_.m_nVersion, kPacket.m_gachaItemID, vecItems, kPacket.m_dwType, kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_GACHA_09, END_PROC );
    }

    int nOK = -99;
    nOK = InsertGachaSetReward( dwUID_, kPacket.m_gachaItemID, kPacket_.m_nVersion, kPacket.m_dwType );

    if ( nOK != 0 ) {
        START_LOG( cerr, L" 가차 세트 보상 기록시 오류 Name : " << strLogin_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( kPacket_.m_nVersion )
            << BUILD_LOG( kPacket.m_gachaItemID )
            << BUILD_LOG( kPacket.m_dwType ) << END_LOG;

        SET_ERR_GOTO( ERR_GACHA_10, END_PROC );
    }

    //--------- 아이템 추가.. -----------//
    for ( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit ) {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_GACHA, kPacket.m_vecReward ) );
    }

    //---------.아이템 레벨 설정---------//
    for ( vitLevel = kPacket.m_vecReward.begin(); vitLevel != kPacket.m_vecReward.end(); ++vitLevel ) {
        bool bUpdateSuccess = true;
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateLevel(dwUID_, kPacket_.m_nLevel, KItemLevelTable::IL_EQUIP_LV, *vitLevel, cCharType_ ) );

        if ( bUpdateSuccess == false ) {
            kPacket.m_nOK = NetError::ERR_GACHA_11;
        }
    }

    // 가차 아이템 로그
    int nRewardInfo = -1;
    if ( kPacket_.m_dwType == KGachaManager::GRT_EQUIP_REWARD ) {
        nRewardInfo = KGachaManager::GRI_WEAPON_REWARD;
    }
    else if ( kPacket_.m_dwType == KGachaManager::GRT_TOTAL_REWARD ) {
        nRewardInfo = KGachaManager::GRI_PET_REWARD;
    }
    LIF( Insert_GachaSystem_RewardLog_ToDB( 
            kPacket_.m_nVersion,
            dwUID_,
            kPacket_.m_cCharType,
            static_cast< int >( kPacket_.m_dwCharLevel ),
            kPacket_.m_nLevel,
            kPacket_.m_ItemID,
            nRewardInfo,
            -1,
            0
        )
    );

    SET_ERROR( NET_OK );

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_GACHA_SET_REWARD_ACK, kPacket );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nVersion )
        << BUILD_LOG( kPacket.m_gachaItemID )
        << BUILD_LOG( kPacket.m_nLevel )
        << BUILD_LOG( kPacket.m_vecReward.size() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_GACHA_LEVEL_SET_REWARD_REQ, KEVENT_GACHA_SET_REWARD_REQ )
{
    EVENT_CHECK_OUT( EVENT_GACHA_LEVEL_SET_REWARD_REQ );

    KEVENT_GACHA_SET_REWARD_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nVersion = kPacket_.m_nVersion;
    kPacket.m_gachaItemID = kPacket_.m_gachaItemID;
    kPacket.m_dwType = kPacket_.m_dwType;
    kPacket.m_nLevel = kPacket_.m_nLevel;
    std::vector< KDropItemInfo > vecItems;
    std::vector< KDropItemInfo >::iterator vit;
    std::vector< KItem >::iterator vitLevel;

    SET_ERROR( ERR_UNKNOWN );
    if ( false == SiKGachaManager()->GetGachaSetReward( kPacket_.m_nVersion, kPacket.m_gachaItemID, vecItems, kPacket.m_dwType, kPacket_.m_ItemID ) ) {
        SET_ERR_GOTO( ERR_GACHA_09, END_PROC );
    }

    int nOK = -99;
    nOK = InsertGachaSetReward( dwUID_, kPacket.m_gachaItemID, kPacket_.m_nVersion, kPacket.m_dwType, kPacket.m_nLevel );

    if ( nOK != 0 ) {
        START_LOG( cerr, L" 가차 세트 보상 기록시 오류 Name : " << strLogin_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( kPacket_.m_nVersion )
            << BUILD_LOG( kPacket.m_gachaItemID )
            << BUILD_LOG( kPacket.m_dwType ) << END_LOG;

        SET_ERR_GOTO( ERR_GACHA_10, END_PROC );
    }

    //--------- 아이템 추가.. -----------//
    for ( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit ) {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_GACHA, kPacket.m_vecReward ) );
    }

    //---------.아이템 레벨 설정---------//
    for ( vitLevel = kPacket.m_vecReward.begin(); vitLevel != kPacket.m_vecReward.end(); ++vitLevel ) {
        bool bUpdateSuccess = true;
        LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateLevel(dwUID_, kPacket_.m_nLevel, KItemLevelTable::IL_EQUIP_LV, *vitLevel, cCharType_ ) );

        if ( bUpdateSuccess == false ) {
            kPacket.m_nOK = NetError::ERR_GACHA_11;
        }
    }

    // 가차 아이템 로그
    int nRewardInfo = -1;
    if ( kPacket_.m_dwType == KGachaManager::GRT_EQUIP_REWARD ) {
        nRewardInfo = KGachaManager::GRI_WEAPON_REWARD;
    }
    else if ( kPacket_.m_dwType == KGachaManager::GRT_TOTAL_REWARD ) {
        nRewardInfo = KGachaManager::GRI_PET_REWARD;
    }
    LIF( Insert_GachaSystem_RewardLog_ToDB( 
            kPacket_.m_nVersion,
            dwUID_,
            kPacket_.m_cCharType,
            static_cast< int >( kPacket_.m_dwCharLevel ),
            kPacket_.m_nLevel,
            kPacket_.m_ItemID,
            nRewardInfo,
            -1,
            0
        )
    );

    SET_ERROR( NET_OK );

END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_GACHA_LEVEL_SET_REWARD_ACK, kPacket );

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nVersion )
        << BUILD_LOG( kPacket.m_gachaItemID )
        << BUILD_LOG( kPacket.m_nLevel )
        << BUILD_LOG( kPacket.m_vecReward.size() ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_STAT_LEAVE_GAME_COUNT, KEVENT_MAP_STAT_LEAVE_GAME )
{
    LeaveGameStat( kPacket_ );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_PERIOD_NOTICE )
{
    UpdatePeriodNotice();
}

_INT_IMPL_ON_FUNC( EVENT_UPDATE_GACHA_KEY_BUY_COUNT_REQ, PAIR_INT )
{
    PAIR_INT kPacket;
    kPacket.first = 0;
    kPacket.second = 0;

    if( kPacket_.first > 0 )
    {
        // Call Update
        UpdateCashGachaKeyBuy( dwUID_, true, kPacket_.first );
    }
    if( kPacket_.second > 0 )
    {
        UpdateCashGachaKeyBuy( dwUID_, false, kPacket_.second );
    }

    // Select Key Count .. 
    GetCashGachaKeyBuyCount( dwUID_, kPacket );

    QUEUING_ACK_TO_USER( EVENT_UPDATE_GACHA_KEY_BUY_COUNT_ACK );

    START_LOG( clog, L" 가차 업데이트... Name : " << strLogin_ )
        << BUILD_LOG( kPacket_.first )
        << BUILD_LOG( kPacket_.second )
        << BUILD_LOG( kPacket.first )
        << BUILD_LOG( kPacket.second ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_FULL_COUPLE_INFO_REQ )
{
    KCoupleInfo kPacket;
    kPacket.Clear();

    SelectCoupleInfo( strLogin_, dwUID_, kPacket );
    if( kPacket.m_dwUID > 0 && kPacket.m_dwPartnerUID > 0 && kPacket.m_tmPartnerRegDate > 0 )
    {
        SelectCoupleEquip( strLogin_, dwUID_, kPacket.m_vecMyEquip );
        SelectCoupleEquip( std::wstring(L""), kPacket.m_dwPartnerUID, kPacket.m_vecPartnerEquip );
        SelectCoupleReqDenyInfo( strLogin_, dwUID_, kPacket.m_setDenyUsers, kPacket.m_bAllDeny );
        GetWarehouseItems( strLogin_, dwUID_, kPacket.m_vecMyInven );
        GetWarehouseItems( strLogin_, kPacket.m_dwPartnerUID, kPacket.m_vecPartnerInven );
        kPacket.m_vecCoupleInven.clear();
        kPacket.m_vecCoupleInven.insert( kPacket.m_vecCoupleInven.end(), kPacket.m_vecMyInven.begin(), kPacket.m_vecMyInven.end() );
        kPacket.m_vecCoupleInven.insert( kPacket.m_vecCoupleInven.end(), kPacket.m_vecPartnerInven.begin(), kPacket.m_vecPartnerInven.end() );

    }
    else if( kPacket.m_dwUID > 0 && kPacket.m_dwPartnerUID == 0 )
    { // 140204. nodefeat. 기존에 m_tmPartnerRegDate 로 검사했으나, ctime 크래시 문제 수정(r.90951) 이후
        // 상대방이 접속하지 않고 커플 해제 하는 경우에 정상적으로 처리 되지 않기 때문에 파트너UID로 검사하도록 수정

        // 커플 해제된 상태임.

        DeleteCouple( strLogin_, dwUID_ );
        kPacket.Clear();
        std::wstring strMsg;
        ReadDivorceMsg( dwUID_, strMsg );
        _QUEUING_ACK_TO_USER( EVENT_DIVORCE_COUPLE_NOT, strMsg );

        START_LOG( cwarn,L"접속시 커플 해제됨.. Name : " << strLogin_ << L", Msg : " << strMsg ) << END_LOG;
    }

    _QUEUING_ACK_TO_USER( EVENT_FULL_COUPLE_INFO_ACK, kPacket );

}

_INT_IMPL_ON_FUNC( EVENT_MAKE_COUPLE_REQ, KMakeCoupleDBData_Req )
{
    KEVENT_MAKE_COUPLE_ACK kPacket;
    if( !MakeCouple( strLogin_, dwUID_, std::wstring(L""), kPacket_.m_dwPartnerUID ) )
    {
        goto END_PROC;
    }

    LIF( InsertWarehouseItems( strLogin_, dwUID_, kPacket_.m_kRingItem ) );
    LIF( UpdateCoupleEquip( strLogin_, dwUID_, kPacket_.m_kCoupleEquip ) );
    LIF( UpdateCoupleEquip( std::wstring(L""), kPacket_.m_dwPartnerUID, kPacket_.m_kCoupleEquip ) );
    LIF( UpdateCoupleSelectChar( strLogin_, dwUID_, 0 ) );
    LIF( UpdateCoupleSelectChar( std::wstring(L""), kPacket_.m_dwPartnerUID, 0 ) );
    LIF( PureEraseItem( strLogin_, dwUID_, kPacket_.m_kRingItem.m_dwUID, cCharType_ ) );


    kPacket.m_dwRequester   = dwUID_;
    kPacket.m_dwRequestee   = kPacket_.m_dwPartnerUID;
    kPacket.m_kRingItem     = kPacket_.m_kRingItem;
    
    SelectCoupleInfo( strLogin_, dwUID_, kPacket.m_kCoupleInfo );
    SelectCoupleEquip( strLogin_, dwUID_, kPacket.m_kCoupleInfo.m_vecMyEquip );
    SelectCoupleEquip( std::wstring(L""), kPacket.m_kCoupleInfo.m_dwPartnerUID, kPacket.m_kCoupleInfo.m_vecPartnerEquip );
    SelectCoupleReqDenyInfo( strLogin_, dwUID_, kPacket.m_kCoupleInfo.m_setDenyUsers, kPacket.m_kCoupleInfo.m_bAllDeny );
    GetWarehouseItems( strLogin_, dwUID_, kPacket.m_kCoupleInfo.m_vecMyInven );
    GetWarehouseItems( std::wstring(L""), kPacket.m_kCoupleInfo.m_dwPartnerUID, kPacket.m_kCoupleInfo.m_vecPartnerInven );
    kPacket.m_kCoupleInfo.m_vecCoupleInven.clear();
    kPacket.m_kCoupleInfo.m_vecCoupleInven.insert( kPacket.m_kCoupleInfo.m_vecCoupleInven.end(), kPacket.m_kCoupleInfo.m_vecMyInven.begin(), kPacket.m_kCoupleInfo.m_vecMyInven.end() );
    kPacket.m_kCoupleInfo.m_vecCoupleInven.insert( kPacket.m_kCoupleInfo.m_vecCoupleInven.end(), kPacket.m_kCoupleInfo.m_vecPartnerInven.begin(), kPacket.m_kCoupleInfo.m_vecPartnerInven.end() );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_MAKE_COUPLE_ACK );

}

_INT_IMPL_ON_FUNC( EVENT_COUPLE_REQ_DENY_SET, DWORD )
{
    UpdateCoupleDenyInfo( strLogin_, dwUID_, kPacket_ );
}

_INT_IMPL_ON_FUNC( EVENT_USE_COUPLE_ITEM_REQ, KCoupleItemData )
{
    std::wstring strPartner(strLogin_);
    strPartner += L"_Partner";

    if( !InsertWarehouseItems(strLogin_, dwUID_, kPacket_.m_kItem ) )
    {
        goto END_PROC;
    }

    //LIF( PureEraseItem( strLogin_, kPacket_.m_kItem.m_dwID, kPacket_.m_kItem.m_dwUID ) );

END_PROC:

    kPacket_.m_nOK = NetError::GetLastNetError();
    _QUEUING_ACK_TO_USER( EVENT_USE_COUPLE_ITEM_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_CHANGE_COUPLE_RING_REQ )
{
    KCoupleEquip kEquip;
    SRequestInfo kReqInfo;
    std::wstring strPartner(strLogin_);
    strPartner += L"_Partner";
    KEVENT_CHANGE_COUPLE_RING_ACK kPacket;

    SET_ERROR( ERR_UNKNOWN );
    // 장착 하려는 아이템의 슬롯 정보를 가져 오자.
    if( !SiKCoupleSystem()->GetCoupleItem( kPacket_.m_kItem.m_dwID, kEquip ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_21, END_PROC );
    }

    if( !SiKCoupleSystem()->GetRingRequestInfo( dwUID_, kReqInfo ) )
    {
        SET_ERR_GOTO( ERR_COUPLE_23, END_PROC );
    }

    kEquip.m_dwItemUID = kPacket_.m_kItem.m_dwUID;

    if( kPacket_.m_bInven )
    {
        LIF( InsertWarehouseItems( strLogin_, dwUID_, kPacket_.m_kItem ) );
    }

    LIF( UpdateCoupleEquip( strLogin_, dwUID_, kEquip ) );
    LIF( UpdateCoupleEquip( strLogin_, kReqInfo.m_dwRequestee, kEquip ) );
    SET_ERROR( NET_OK );

    kPacket.m_bInven = kPacket_.m_bInven;
    kPacket.m_kItem = kPacket_.m_kItem;
END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_CHANGE_COUPLE_RING_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_DIVORCE_COUPLE_REQ, KDiborceDBData )
{
    LIF( DeleteCouple( strLogin_, dwUID_ ) );
    if( !kPacket_.m_strMsg.empty() )
    {
        SaveDivorceMsg( kPacket_.m_dwPartnerUID, kPacket_.m_strMsg );
    }
}

_INT_IMPL_ON_FUNC( EVENT_COUPLEITEM_EXPIRED_NOT, std::vector<KSimpleItem> )
{
    std::vector<KSimpleItem>::const_iterator vit;
    for( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit )
    {
        DeleteWarehouseItem( strLogin_, dwUID_, *vit );
    }

    START_LOG( clog, L"커플 아이템만료 처리.. Name : " << strLogin_ )
        << BUILD_LOG( kPacket_.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_UPDATE_ATTENDANCE_REWARD_ITEM )
{
    /*
    20081125. microcat.
    { call dbo.PBGA_point_bag_select_reward }
    인자없음 ㅋㅋ

    n return ( @1, @2, @3, @4, @5 )
    @1 ; AttendanceType tinyint
    1  ;주개근
    2 ; 월개근
    @2 ; SupplyType tinyint
    1 ; 선택지급 ; 입력된 아이템 중 한개를 유저가 선택 지급
    2 ; 모두지급 ; 입력된 아이템을 해당 개근 조건이 충족되면 모두 지급
    3 ; 랜덤지급 ; 입력된 아이템 중 한개를 랜덤 지급
    @3 ; ItemID int
    @4 ; ItemType tinyint
    @5 ; Factor int
    */
    std::vector<KAttendanceReward> vecRewardItemList;  // 개근 보상 아이템 리스트
    vecRewardItemList.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.PBGA_point_bag_select_reward }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KAttendanceReward kItemUnit;

        VERIFY_QUERY( m_kODBC >> kItemUnit.m_nAttendanceType
            >> kItemUnit.m_nSupplyType
            >> kItemUnit.m_kRewardItem.m_ItemID
            >> kItemUnit.m_kRewardItem.m_nPeriod
            >> kItemUnit.m_kRewardItem.m_nCount );

        vecRewardItemList.push_back( kItemUnit );
    }

    SiKGSSimLayer()->UpdateAttendanceReward( vecRewardItemList );

    START_LOG( clog, L"개근 보상아이템 리스트 갱신")
        << BUILD_LOG( vecRewardItemList.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_WEEKLY_ATTEND_REWARD_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_WEEKLY_ATTEND_REWARD_REQ );

    SET_ERROR( ERR_UNKNOWN );

    KEVENT_WEEKLY_ATTEND_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_cMonth   = kPacket_.m_cMonth;
    kPacket.m_cWeekID  = kPacket_.m_cWeekID;
    kPacket.m_nIncGCPoint = kPacket_.m_nIncGCPoint;
    kPacket.m_nGCPoint = kPacket_.m_nGCPoint;
    kPacket.m_nFailResult = 0;

    std::vector<KItem>::iterator vit;
    int nFailResult = 0;
    CTime tmCurrent = CTime::GetCurrentTime();
    std::wstring strDate = (LPCTSTR)tmCurrent.Format( _T("%y%m%d") );

    // 주개근 석포인트 저장
    if ( kPacket.m_nIncGCPoint > 0 ) {
        if( false == UpdateUserPointBagAccTime( dwUID_, cCharType_, kPacket.m_nIncGCPoint, kPacket_.m_nAccTime, strDate ) ) {
            SET_ERR_GOTO( ERR_CALENDAR_15, END_PROC );
        }
    }
    kPacket.m_nGCPoint += kPacket.m_nIncGCPoint;

    //kPacket_.m_vecReqItem:기본보상,랜덤보상,선택보상 다 들어가있음.
    for ( vit = kPacket_.m_vecReqItem.begin() ; vit != kPacket_.m_vecReqItem.end() ; ++vit ) {
        if ( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nCount, vit->m_nPeriod, KItemManager::EWIT_ATTEND_REWARD, kPacket.m_vecInv ) ) {
            START_LOG( cerr, L"주개근 보상 아이템 얻기 DB 작업 실패. Name : " << strLogin_ )
                << BUILD_LOG( vit->m_ItemID )
                << BUILD_LOG( vit->m_nCount ) << END_LOG;
            ++nFailResult;
        }
    }
    kPacket.m_nFailResult = nFailResult; // 실패한 횟수를 넣어주자.

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"주개근출석포인트 DB저장 결과 : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nGCPoint )
        << BUILD_LOG( kPacket.m_nIncGCPoint )
        << BUILD_LOGc( kPacket.m_cMonth )
        << BUILD_LOGc( kPacket.m_cWeekID )
        << BUILD_LOG( kPacket.m_nFailResult )
        << BUILD_LOG( kPacket_.m_nAccTime )
        << BUILD_LOG( kPacket.m_vecInv.size() )
        << BUILD_LOG( kPacket_.m_vecReqItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_WEEKLY_ATTEND_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_GET_USER_DONATION_INFO_REQ )
{
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_GET_USER_DONATION_INFO_ACK kPacket;
    GetDonationPointPerUser( dwUID_, kPacket.m_nAccDonationPoint );
    SET_ERROR( NET_OK ); // 그냥.. 일관되게 했음.. ^^;;

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_GET_USER_DONATION_INFO_ACK );
}

INT_IMPL_ON_FUNC( EVENT_DONATION_POINT_REQ )
{
    EVENT_CHECK_OUT( EVENT_DONATION_POINT_REQ );
    KEVENT_DONATION_POINT_ACK kPacket;
    kPacket.m_nAccDonationPoint = kPacket_.m_nDonationPoint;

    SET_ERROR( ERR_UNKNOWN );
    // 기부 한다.
    if( !DonationGP( strLogin_, dwUID_, kPacket_.m_nDonationPoint ) )
    {
        SET_ERR_GOTO( ERR_DONATION_02, END_PROC );
    }
    // 아이템 요청이 비어 있지 않으면 아이템도 준다.
    if( !kPacket_.m_vecRewards.empty() )
    {
        std::vector<KDropItemInfo>::iterator vit;
        for( vit = kPacket_.m_vecRewards.begin(); vit != kPacket_.m_vecRewards.end() ; ++vit )
        {
            if ( !m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_DONATION_POINT, kPacket.m_vecRewardItems ) ) {
                START_LOG( cerr, L"기부 보상 아이템 요청시 오류 Name : " << strLogin_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
            }
        }
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_DONATION_POINT_ACK );
    
    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"기부 결과.. : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nAccDonationPoint )
        << BUILD_LOG( kPacket.m_vecRewardItems.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_CHECK_LUNA_NEWYEAR_REQ )
{
    bool kPacket = ( GetTodayEventCount( dwUID_, UET_LUNA_NEW_YEAR ) > 0 );
    QUEUING_ACK_TO_USER( EVENT_CHECK_LUNA_NEWYEAR_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_LUNA_NEWYEAR_REQ )
{
    SET_ERROR( ERR_UNKNOWN );

    EVENT_CHECK_OUT( EVENT_LUNA_NEWYEAR_REQ );
    KEVENT_LUNA_NEWYEAR_ACK kPacket;
    kPacket.Clear();
    std::vector<KDropItemInfo> vecRewardItems;
    std::vector<KDropItemInfo>::iterator vit;

    // 오늘 받았는지 체크 한번 한다.
    if( GetTodayEventCount( dwUID_, UET_LUNA_NEW_YEAR ) > 0 ) {
        SET_ERR_GOTO( ERR_LUNA_NEWYEAR_01, END_PROC );
    }

    // 보상 리스트를 가져 온다.
    if( !SiKLunaNewYear()->GetReward( kPacket.m_nRewardGP, vecRewardItems ) ) {
        SET_ERR_GOTO( ERR_LUNA_NEWYEAR_02,  END_PROC );
    }

    // Item을 준다.
    for( vit = vecRewardItems.begin() ; vit != vecRewardItems.end() ; ++vit ) {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_LUNA_NEWYEAR, kPacket.m_vecRewardItems ) );
    }

    LIF( SetTodayEventDoneDiff( dwUID_, UET_LUNA_NEW_YEAR ) );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_LUNA_NEWYEAR_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_CHAR_POLL_UPDATE_DB, MAP_CHAR_INT )
{
    MAP_CHAR_INT::iterator mit;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        AddServerCountData( mit->second, mit->first + CDT_POLL_ELESIS );
    }
}
INT_IMPL_ON_FUNC_NOPARAM( EVENT_DB_GACHA_COIN_COUNT_REQ )
{
    //가챠 코인수 가져오기
    //exec dbo.GSG_gacha_system_select
    //    ( LoginUID int )
    //
    //    1 return
    //    ( GachaCount int )
    DBJIF( m_kODBC.Query( L"exec dbo.GSG_gacha_system_select %d",
        dwUID_ ) );

    int kPacket = 0;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kPacket );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( kPacket >= 0 )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket ) << END_LOG;

    // 코인수가 음수가 나오면 문제가 있다
    if( kPacket < 0 )
        kPacket = 0;

    QUEUING_ACK_TO_USER( EVENT_GACHA_COIN_COUNT_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_BUY_FOR_VIP_REQ, KDB_EVENT_BUY_FOR_VIP_REQ )
{
    EVENT_CHECK_OUT( EVENT_BUY_FOR_VIP_REQ );
    KEVENT_BUY_FOR_VIP_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kVipBadge = kPacket_.m_kVIPItem;
    int nDiffCount = 0;
    __int64 biWaitUID = 0;

    //단품인지,세트인지 선별
    if ( !m_kHelper.InsertSingleItemJob_WaitUID( dwUID_, cCharType_, kPacket_.m_kPacket.m_kBuyItem.m_ItemID, kPacket_.m_kPacket.m_kBuyItem.m_nDuration, kPacket_.m_kPacket.m_kBuyItem.m_nPeriod, KItemManager::EWIT_VIP_BUY, kPacket.m_vecItem, biWaitUID ) ) {

        START_LOG( cerr, L"VIP 아이템 얻기 DB 작업 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kPacket.m_kBuyItem.m_ItemID )
            << BUILD_LOG( kPacket_.m_kPacket.m_kBuyItem.m_nDuration )
            << BUILD_LOG( kPacket_.m_kPacket.m_kBuyItem.m_nPeriod ) << END_LOG;
        SET_ERR_GOTO( ERR_VIP_ERR_02, END_PROC );
    }

    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket.m_kVipBadge.m_ItemUID, kPacket.m_kVipBadge.m_nInitCount, kPacket.m_kVipBadge.m_nCount ) ) {
        START_LOG( clog, L"리타 크리스마스 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket.m_kVipBadge.m_ItemUID )
            << BUILD_LOG( kPacket.m_kVipBadge.m_nInitCount )
            << BUILD_LOG( kPacket.m_kVipBadge.m_nCount )
            << END_LOG;
    }

    kPacket.m_kVipBadge.m_nInitCount = kPacket.m_kVipBadge.m_nCount;
    // VIP 깎고.
    // 동기화 + 차감.
    nDiffCount += kPacket_.m_kPacket.m_nItemPrice;
    if( !UseCountItem( dwUID_, kPacket_.m_kVIPItem.m_ItemUID, nDiffCount ) )
    {
        START_LOG( cerr, L"로얄VIP뱃지 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
        SET_ERR_GOTO( ERR_VIP_ERR_03, END_PROC );
    }

    // VIP개수 갱신( = 현재개수 - 지불한개수 )
    kPacket.m_kVipBadge.m_nCount = std::max<int>(0, kPacket.m_kVipBadge.m_nCount - kPacket_.m_kPacket.m_nItemPrice );
    kPacket.m_kVipBadge.m_nInitCount = kPacket.m_kVipBadge.m_nCount;

    m_kHelper.UpdateMoneyConsumptionStat( dwUID_, KItemManager::EMC_VIP, kPacket_.m_kPacket.m_kBuyItem.m_nDuration, biWaitUID );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    START_LOG( clog, L"VIP 아이템구매 DB작업 종료 : " << NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket_.m_kVIPItem.m_nCount)
        << BUILD_LOG( kPacket_.m_kPacket.m_nItemPrice )
        << BUILD_LOG( kPacket.m_kVipBadge.m_nCount )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    _QUEUING_ACK_TO_USER( EVENT_BUY_FOR_VIP_ACK, kPacket );
}



// 090520. tgkwon. ping 통계
_INT_IMPL_ON_FUNC( EVENT_STAT_PING_INSERT_DB, MAP_PING_STAT )
{
    /*  
    20090521. microcat.  
    20100511. microcat.  
    20110210. microcat.  
    20110413. mk8253.  
    */  

    /*  
    { call dbo.ICCO_inter_country_ping_collect ( N'%s', N'%s', %d, %d, %d ) }  

    { call dbo.ICCO_inter_country_ping_collect ( @1, @2, @3, @4, @5 ) }  
    @1 ; @iTypeID_                 tinyint  
    @2 ; @strSourceCountryID_      nvarchar( 2 )  
    @3 ; @strDestinationCountryID_ nvarchar( 2 )  
    @4 ; @iPingRate_               int  
    @5 ; @iPingCount_              int  

    1 return ( @1 )  
    @1 ; OK int  
    0        ; 성공  
    -101이하 ; 트랜잭션 에러  
    */

    MAP_PING_STAT ::iterator mit;
    int nCount = 0;

    for( mit = kPacket_.begin(); mit != kPacket_.end(); mit++ )
    {
        _DBJIF( m_kODBCStat.Query( L"{call dbo.ICCO_inter_country_ping_collect ( %d, N'%s', N'%s', %d, %d ) }",
        mit->first.m_nType,
        mit->first.m_wstrSendCCode.c_str(),
        mit->first.m_wstrRecvCCode.c_str(),
        mit->first.m_nPingTime,
        mit->second ), return, (&m_kODBCStat) );
        int nOK = -99;
        if( m_kODBCStat.BeginFetch() )
        {
        VERIFY_QUERY( m_kODBCStat >> nOK );
        m_kODBCStat.EndFetch();
        }

        LOG_SUCCESS( nOK == 0 )
        << L"Ping 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
        << BUILD_LOG( nOK )
        << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
    }
}


INT_IMPL_ON_FUNC( DB_EVENT_OPEN_CAPSULE_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_OPEN_CAPSULE_REQ );
    SET_ERROR( ERR_UNKNOWN );

    int nItemType = -1;
    std::vector< std::pair<DWORD,char> > vecParts;
    KEVENT_OPEN_CAPSULE_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nPriceType = kPacket_.m_nPriceType; // 화폐 타입 넘기기.
    kPacket.m_nRemain = kPacket_.m_nPrice; // GP캡슐 가격 넣어놓자.
    kPacket.m_nItemGrade = kPacket_.m_nItemGrade; // 획득 아이템 등급

    // 단품이냐,세트냐?
    if ( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kRewardItem.m_ItemID, kPacket_.m_kRewardItem.m_nDuration, kPacket_.m_kRewardItem.m_nPeriod, KItemManager::EWIT_CAPSULE, kPacket.m_vecItem ) == false ) {
        START_LOG( cerr, L"GP캡슐 아이템 얻기 DB 작업 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_kRewardItem.m_ItemID )
            << BUILD_LOG( kPacket_.m_kRewardItem.m_nDuration )
            << BUILD_LOG( kPacket_.m_kRewardItem.m_nPeriod ) << END_LOG;
        SET_ERR_GOTO( ERR_CAPSULE_07, END_PROC );
    }

    // DB에 기록 남기기
    CapsuleSystemInsertLog( dwUID_, kPacket_.m_nCharType, kPacket_.m_nItemkind, kPacket_.m_prLevelRange.first, kPacket_.m_kRewardItem.m_ItemID );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_nDuration )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_nPeriod )
        << BUILD_LOG( kPacket_.m_nPriceType )
        << BUILD_LOG( kPacket_.m_nPrice )
        << BUILD_LOG( kPacket.m_vecItem.size() )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_OPEN_CAPSULE_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_UPDATE_GAME_COUPON_FROM_DB, int )
{
    // kPacket_값
    // 1 (IT_ALL_INFO)    : 모든 정보를 준다.(서버실행시 최초1회)
    // 0 (IT_UPDATE_INFO) : 갱신된 정보만 준다.(주기적으로 실행)
    GetGameCouponInfoFromDB( kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_USE_GAME_COUPON_REQ )
{
    EVENT_CHECK_OUT( EVENT_USE_GAME_COUPON_REQ );

    std::vector<KDropItemInfo> vecItems;
    std::vector<KDropItemInfo>::iterator vit;

    KEVENT_USE_GAME_COUPON_ACK kPacket;
    kPacket.m_nOK = -99;
    DWORD dwCouponUID = -99;
    int nCouponNID = -99;
    int nItemNID = -99;

    // 0.쿠폰입력 실패오류 체크 (브라질용)
    if ( SiKGSSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_LEVELUP ) == true ) {
        if( CheckGameCouponErr( dwUID_ ) == false )
        {
            // 일일 입력 한도 횟수 초과
            SET_ERR_GOTO( ERR_GAME_COUPON_11, END_PROC );
        }
    }

    if( SiKGSSimLayer()->CheckAuthTypeFlag( KGSSimLayer::AT_FUNBOX_TH ) ) {

        if ( !CheckGameCoupon( kPacket_.m_strCouponID, kPacket_.m_strPassWD, kPacket_.m_nEventUID, nCouponNID, nItemNID ) ) {
            goto END_PROC;
        }

        // 2.게임쿠폰아이템 목록 얻어오기
        THGetGameCouponItem( nItemNID, vecItems );

        // 3.아이템 넣어주기
        for ( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit ) {
            if ( !m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_COUPON_REWARD, kPacket.m_vecItem ) ) {
                START_LOG( cerr, L"게임쿠폰 아이템넣기 DB 작업 실패. UID : " << dwUID_ << L", ItemID : " << vit->m_ItemID ) << END_LOG;
            }
        }

        // 4.사용한 게임쿠폰정보 갱신 내부에서 오류 셋팅.
        if( !UpdateGameCoupon( dwUID_, nCouponNID ) )
        {
            goto END_PROC;
        }

        // 여기까지 왔으면 모두 성공.
        SET_ERROR( NET_OK );

        goto END_PROC;
    } else {
        // 1.게임쿠폰체크
        {
            int nRet = -99;
            nRet = CheckGameCoupon( dwUID_, kPacket_.m_nEventUID, kPacket_.m_strCouponID, dwCouponUID );
            switch( nRet )
            {
            case 0: // 성공
                START_LOG( clog, L"게임쿠폰 DB체크 성공. CouponUID : " << dwCouponUID ) << END_LOG;
                break;
            case -1: // 유저 정보가 없음.
                SET_ERR_GOTO( ERR_GAME_COUPON_00, END_PROC );
                break;
            case -2: // 해당 이벤트가 없거나 이벤트 기간이 아님.
                SET_ERR_GOTO( ERR_GAME_COUPON_01, END_PROC );
                break;
            case -3: // 발행된 쿠폰 번호가 아님.
                SET_ERR_GOTO( ERR_GAME_COUPON_05, END_PROC );
                break;
            case -4: // 이미 사용된 쿠폰임
                SET_ERR_GOTO( ERR_GAME_COUPON_03, END_PROC );
                break;
            default: // 알수없는 에러
                SET_ERR_GOTO( ERR_DB_COMMON_00, END_PROC );
            }
        }

        // 2.게임쿠폰아이템 목록 얻어오기
        GetGameCouponItem( kPacket_.m_nEventUID, vecItems );

        // 3.아이템 넣어주기
        for ( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit ) {
            if ( !m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_COUPON_REWARD, kPacket.m_vecItem ) ) {
                START_LOG( cerr, L"게임쿠폰 아이템넣기 DB 작업 실패. UID : " << dwUID_ << L", ItemID : " << vit->m_ItemID ) << END_LOG;
            }
        }

        // 4.사용한 게임쿠폰정보 갱신
        // 갱신은 실패해도 이미 아이템은 들어와있음.
        // 에러로그만 출력하자.
        {
            int nRet = -99;
            nRet = UpdateGameCoupon( dwUID_, kPacket_.m_nEventUID, dwCouponUID );
            switch( nRet )
            {
            case 0: // 성공
                START_LOG( clog, L"사용한 게임쿠폰정보 갱신 성공." ) << END_LOG;
                break;
            case -1: // 유저 정보가 없음.
                SET_ERROR( ERR_GAME_COUPON_00 );
                break;
            case -2: // 해당 이벤트가 없거나 이벤트 기간이 아님.
                SET_ERROR( ERR_GAME_COUPON_01 );
                break;
            case -3: // 발행된 쿠폰 번호가 아님.
                SET_ERROR( ERR_GAME_COUPON_05 );
                break;
            case -4: // 이미 사용된 쿠폰임
                SET_ERROR( ERR_GAME_COUPON_03 );
                break;
            default: // 알수없는 에러
                SET_ERROR( ERR_DB_COMMON_00 );
            }

            _LOG_SUCCESS( nRet == 0, L"사용한 게임쿠폰정보 갱신 결과. nRet : " << nRet )
                << BUILD_LOG( NetError::GetLastNetError() )
                << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        }

        // 여기까지 왔으면 모두 성공.
        SET_ERROR( NET_OK );
    }

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L"쿠폰 사용 DB : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_nEventUID )
        << BUILD_LOG( kPacket_.m_strCouponID )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( dwCouponUID )
        << BUILD_LOG( kPacket_.m_nEventUID )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_USE_GAME_COUPON_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_GAME_COUPON_FAIL_NOT )
{
    JIF( UpdateGameCouponFailErr( dwUID_ ) == true );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_CHECK_RECOMMEND_ENABLE_REQ )
{
    //DWORD dwOrtherUID = 0;
    //int nState = 0;
    KRecommenderInfo kMyRecomInfo;
    KEVENT_CHECK_RECOMMEND_ENABLE_ACK kPacket;
    kPacket.m_nOK = -99;
    EVENT_CHECK_OUT( EVENT_CHECK_RECOMMEND_ENABLE_REQ );

    SET_ERROR( ERR_UNKNOWN );

    if( !GetMyRecomInfo( dwUID_, kMyRecomInfo, SiKRecomHelper()->GetVersion() ) )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );
    }

    if( kMyRecomInfo.m_nState != KRecomHelper::RBS_NONE )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_03, END_PROC );
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_CHECK_RECOMMEND_ENABLE_ACK );
}

INT_IMPL_ON_FUNC( DB_EVENT_RECOMMEND_USER_REQ )
{
    //DWORD dwOrtherUID = 0;
    //int nState = 0;
    KRecommenderInfo kMyRecomInfo;
    KEVENT_RECOMMEND_USER_ACK kPacket;
    kPacket.m_nOK = 99;
    kPacket.m_nRecomInfo.m_nState   = kPacket_.m_nState;
    kPacket.m_nRecomInfo.m_strNick  = kPacket_.m_strNick;

    std::vector<KRecommenderInfo> vecOrthersRecommender;
    std::vector<DWORD> vecOrthersAccepted;
    DWORD dwAcceptedCount = 0;

    EVENT_CHECK_OUT( DB_EVENT_RECOMMEND_USER_REQ );
    SET_ERROR( ERR_UNKNOWN );

    if( !GetMyRecomInfo( dwUID_, kMyRecomInfo, SiKRecomHelper()->GetVersion() ) )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );
    }

    if( kMyRecomInfo.m_nState != KRecomHelper::RBS_NONE ) // 내가 추천 할수 있는 상태 인가?
    {
        SET_ERR_GOTO( ERR_RECOMMEND_03, END_PROC );
    }

    if( kPacket_.m_nState == KRecomHelper::RUO_REFUSE )
    {
        LIF( SetMyRecomInfo( dwUID_, 0, KRecomHelper::RBS_NOT_RECOM, SiKRecomHelper()->GetVersion() ) );
        SET_ERR_GOTO( NET_OK, END_PROC );
    }

    // 다음에 추천함을 User에서 막았기 때문에
    // 이제 남은건 추천 한다 밖에 없음..
    if( kPacket_.m_nState != KRecomHelper::RUO_RECOMMEND )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_05, END_PROC );
    }

    // 대상 유저의 UID를 얻어 낸다.
    if( !NickToUID( kPacket_.m_strNick, kPacket.m_nRecomInfo.m_dwLoginUID ) )
    {
        SET_ERR_GOTO( ERR_RECOMMEND_06, END_PROC );
    }

    if( kPacket.m_nRecomInfo.m_dwLoginUID == 0 || kPacket.m_nRecomInfo.m_dwLoginUID == dwUID_ )
        SET_ERR_GOTO( ERR_RECOMMEND_06, END_PROC );

    // 대상의 추천자들이 매우 만땅이면.. 미루자..
    if( !GetOtherRecomInfo( kPacket.m_nRecomInfo.m_dwLoginUID, vecOrthersRecommender, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    if( vecOrthersRecommender.size() >= SiKRecomHelper()->GetMaxRecommReqNum() )
        SET_ERR_GOTO( ERR_RECOMMEND_12, END_PROC );

    dwAcceptedCount += SiKRecomHelper()->GetRecommendersCount( vecOrthersRecommender, KRecomHelper::RABS_ACCEPT );
    dwAcceptedCount += SiKRecomHelper()->GetRecommendersCount( vecOrthersRecommender, KRecomHelper::RABS_ATTEND );
    dwAcceptedCount += SiKRecomHelper()->GetRecommendersCount( vecOrthersRecommender, KRecomHelper::RABS_REWARDED );
    if( dwAcceptedCount >= SiKRecomHelper()->GetMaxAcceptNum() )
        SET_ERR_GOTO( ERR_RECOMMEND_12, END_PROC );

    // 이제 추천을 하자..
    if( !SetMyRecomInfo( dwUID_, kPacket.m_nRecomInfo.m_dwLoginUID, KRecomHelper::RBS_WAIT_RECOM, SiKRecomHelper()->GetVersion() ) ||
        !SetOtherRecomInfo( kPacket.m_nRecomInfo.m_dwLoginUID, dwUID_, KRecomHelper::RABS_WAIT, SiKRecomHelper()->GetVersion() ))
    {
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );
    }

    // 상대방 수락 이후 일괄 지급 받도록 레벨 달성 단계 써놓음
    UpdateUserLevelUpInfo( dwUID_, kPacket.m_nRecomInfo.m_dwLoginUID, kPacket_.m_nCurrentLevel, SiKRecomHelper()->GetVersion() );
    UpdateLevelUpRecomReward( dwUID_, kPacket.m_nRecomInfo.m_dwLoginUID, kPacket_.m_nCurrentLevel, SiKRecomHelper()->GetVersion() );
    // 상대방 지급 단계도 갱신
    UpdateRecommendSuppliedItem( kPacket.m_nRecomInfo.m_dwLoginUID, dwUID_, kPacket_.m_nCurrentLevel, SiKRecomHelper()->GetVersion() );

    UIDToNick( kPacket.m_nRecomInfo.m_dwLoginUID, kPacket.m_nRecomInfo.m_strNick );
    kPacket.m_nRecomInfo.m_nLevel = kPacket_.m_nCurrentLevel;
    
    SET_ERROR( NET_OK );
END_PROC:

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_RECOMMEND_USER_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_RECOMMEND_FULL_INFO_REQ )
{
    KEVENT_RECOMMEND_FULL_INFO_ACK kPacket;
    std::vector<DWORD> vecAttenUser;
    EVENT_CHECK_OUT( EVENT_RECOMMEND_FULL_INFO_REQ );
    SET_ERROR( ERR_UNKNOWN );
    // 나의 정보를 퍼온다.
    if( !GetMyRecomInfo( dwUID_, kPacket.m_kMyInfo, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    // 나를 추천한 사람들의 정보를 퍼온다.
    if( !GetOtherRecomInfo( dwUID_, kPacket.m_vecRecommenders, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );
    
    if( !GetRecomBonusInfo( dwUID_, kPacket.m_nRecomBonusState, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    // 나를 추천한 사람중에 첫 출석이 있는지 확인 한다.
    SiKRecomHelper()->GetRecommenders( kPacket.m_vecRecommenders, KRecomHelper::RABS_ATTEND, vecAttenUser );
    if( !vecAttenUser.empty() )
    {
        // 나를 추천한 사람중에 첫출석이 있으면 값을 셋팅한다.
        std::vector<DWORD>::iterator vit;
        for( vit = vecAttenUser.begin() ; vit != vecAttenUser.end() ; ++vit )
            SetOtherRecomInfo( dwUID_, *vit, KRecomHelper::RABS_REWARDED, SiKRecomHelper()->GetVersion() );

        // 첫 출석에 해당 하는 아이템목록을 가져 온다.
        int nMultiple = (int)vecAttenUser.size();
        std::vector<KDropItemInfo> vecReward;
        std::vector<KDropItemInfo>::const_iterator vit1;
        SiKRecomHelper()->GetFirstAttenReward( KRecomHelper::UT_RECOMMENDEE, vecReward );

        // 해당 아이템 목록을 유저에게 추가 한다.
        for( vit1 = vecReward.begin() ; vit1 != vecReward.end() ; ++vit1 )
        {
            int nDuration = (vit1->m_nDuration==KItem::UNLIMITED_ITEM?KItem::UNLIMITED_ITEM:vit1->m_nDuration*nMultiple);
            int nPeriod = (vit1->m_nPeriod==KItem::UNLIMITED_ITEM?KItem::UNLIMITED_ITEM:vit1->m_nPeriod*nMultiple);
            LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit1->m_ItemID, nDuration, nPeriod, KItemManager::EWIT_EVENT_RECOMMEND_FULL, kPacket.m_vecNormalReward ) );
        }
    }
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_RECOMMEND_FULL_INFO_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_RECOMMENDER_ACCEPT_REQ, std::vector<DWORD> )
{
    std::vector<DWORD> vecTmp;
    std::vector<DWORD>::iterator vit;
    std::vector<KRecommenderInfo> vecRecommenders;
    KRecommenderAcceptDBAck kPacket;
    kPacket.m_vecReqUserUIDs = kPacket_;

    kPacket.m_kPacket.m_nOK = -99;

    EVENT_CHECK_OUT( EVENT_RECOMMENDER_ACCEPT_REQ );
    SET_ERROR( ERR_UNKNOWN );

    // 리스트를 가져 온다.
    if( !GetOtherRecomInfo( dwUID_, vecRecommenders, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    // 요청 수가 대기자 수보다 많은가?
    SiKRecomHelper()->GetRecommenders( vecRecommenders, KRecomHelper::RABS_WAIT, vecTmp );
    if( vecTmp.size() < kPacket_.size() )
        SET_ERR_GOTO( ERR_RECOMMEND_07, END_PROC );

    // 요청수가 대기자 수안에 포함 되는가?
    std::sort( vecTmp.begin(), vecTmp.end() );
    std::sort( kPacket_.begin(), kPacket_.end() );
    if( !std::includes( vecTmp.begin(), vecTmp.end(), kPacket_.begin(), kPacket_.end() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_07, END_PROC );

    // 승인수 + 요청수가 최대 숫자보다 많은가?
    SiKRecomHelper()->GetRecommenders( vecRecommenders, KRecomHelper::RABS_ACCEPT, vecTmp );
    if( SiKRecomHelper()->GetMaxAcceptNum() < kPacket_.size() + vecTmp.size() )
        SET_ERR_GOTO( ERR_RECOMMEND_08, END_PROC );

    // 이터레이션 하면서 셋팅 해주자..
    for( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit )
    {
        SetMyRecomInfo( *vit, dwUID_, KRecomHelper::RBS_ACCEPT_RECOM, SiKRecomHelper()->GetVersion() );
        SetOtherRecomInfo( dwUID_, *vit, KRecomHelper::RABS_ACCEPT, SiKRecomHelper()->GetVersion() );
    }

    if( !GetOtherRecomInfo( dwUID_, kPacket.m_kPacket.m_vecRecommenders, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_RECOMMENDER_ACCEPT_ACK );
}
_INT_IMPL_ON_FUNC( EVENT_ATTENDANCE_BONUS_REQ, int )
{
    KEVENT_ATTENDANCE_BONUS_ACK kPacket;
    kPacket.m_nOK = -99;
    int nBonusState = 0;
    std::vector<KDropItemInfo> vecReward;
    EVENT_CHECK_OUT( EVENT_ATTENDANCE_BONUS_REQ );
    SET_ERROR( ERR_UNKNOWN );

    // 나의 기록을 가져 온가.
    if( !GetRecomBonusInfo( dwUID_, nBonusState, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    // 범위 체크 한번 더..
    if( kPacket_ <= nBonusState )
        SET_ERR_GOTO( ERR_RECOMMEND_11, END_PROC );

    // 셋팅한다.
    if( !SetRecomBonusInfo( dwUID_, kPacket_, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    kPacket.m_nRecomBonus = kPacket_;
    // 아이템 리스트 가져 온다.
    if( SiKRecomHelper()->GetRecomBonusReward( kPacket_, vecReward ) )
    {
        // 아이템넣는다.
        std::vector<KDropItemInfo>::const_iterator vit1;
        for( vit1 = vecReward.begin() ; vit1 != vecReward.end() ; ++vit1 )
            LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit1->m_ItemID, vit1->m_nDuration, vit1->m_nPeriod, KItemManager::EWIT_EVENT_ATTENDANCE_BONUS, kPacket.m_vecNormalReward ) );
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_ATTENDANCE_BONUS_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_RECOM_DAILY_ATTENDANCE_REQ, bool )
{
    // kPacket_이 true 이면 신규..
    
    KEVENT_RECOM_DAILY_ATTENDANCE_ACK kPacket;
    kPacket.m_bFirst = false;
    //DWORD dwOrtherUID;
    //int nState;
    KRecommenderInfo kMyRecomInfo;
    std::vector<KDropItemInfo> vecReward;

    EVENT_CHECK_OUT( EVENT_RECOM_DAILY_ATTENDANCE_REQ );
    SET_ERROR( ERR_UNKNOWN );

    // 내 정보 가져 오기
    JIF( GetMyRecomInfo( dwUID_, kMyRecomInfo, SiKRecomHelper()->GetVersion() ) );

    // 첫 출석 인지.
    if( kMyRecomInfo.m_nState < KRecomHelper::RBS_ATTEND )
    {
        // 첫 출석이라면...
        JIF( SetMyRecomInfo( dwUID_, kMyRecomInfo.m_dwLoginUID, KRecomHelper::RBS_ATTEND, SiKRecomHelper()->GetVersion() ) );
        LIF( SetOtherRecomInfo( kMyRecomInfo.m_dwLoginUID, dwUID_, KRecomHelper::RABS_ATTEND, SiKRecomHelper()->GetVersion() ) );
        kPacket.m_bFirst = true;
        JIF( SiKRecomHelper()->GetFirstAttenReward( (kPacket_?(KRecomHelper::UT_NEW):(KRecomHelper::UT_OLD)), vecReward ) );
    }
    else
        JIF( SiKRecomHelper()->GetDailyAttenReward( (kPacket_?(KRecomHelper::UT_NEW):(KRecomHelper::UT_OLD)), vecReward ) );

    if( !vecReward.empty() )
    {
        std::vector<KDropItemInfo>::const_iterator vit1;
        for( vit1 = vecReward.begin() ; vit1 != vecReward.end() ; ++vit1 )
            LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit1->m_ItemID, vit1->m_nDuration, vit1->m_nPeriod , KItemManager::EWIT_EVENT_RECOM_DAILY_ATTENDANCE, kPacket.m_vecNormalReward) );
    }

    QUEUING_ACK_TO_USER( EVENT_RECOM_DAILY_ATTENDANCE_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_RECOMMENDER_DENY_REQ, std::vector<DWORD> )
{
    KRecommenderAcceptDBAck kPacket;
    kPacket.m_kPacket.m_nOK = -99;
    std::vector<DWORD> vecTemp;
    std::vector<DWORD>::iterator vit;
    kPacket.m_vecReqUserUIDs = kPacket_;

    EVENT_CHECK_OUT( EVENT_RECOMMENDER_DENY_REQ );
    SET_ERROR( ERR_UNKNOWN );

    // 나를 추천한 사람들 가져 옴..
    if( !GetOtherRecomInfo( dwUID_, kPacket.m_kPacket.m_vecRecommenders, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    // 대기자들을 가져 온다.
    SiKRecomHelper()->GetRecommenders( kPacket.m_kPacket.m_vecRecommenders, KRecomHelper::RABS_WAIT, vecTemp );
    std::sort( vecTemp.begin(), vecTemp.end() );
    std::sort( kPacket_.begin(), kPacket_.end() );

    // 거절 유저가 대기자 리스트에 있는지 확인 한다.
    if( !std::includes( vecTemp.begin(), vecTemp.end(), kPacket_.begin(), kPacket_.end() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_13, END_PROC );

    // 거절 하자.
    for( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit )
    {
        if( SetOtherRecomInfo( dwUID_, *vit, KRecomHelper::RABS_DENY, SiKRecomHelper()->GetVersion() ) )
            LIF( SetMyRecomInfo( *vit, dwUID_, KRecomHelper::RBS_NONE, SiKRecomHelper()->GetVersion() ) ); // 다른 사람을 추천 할수 있게 추천 가능 상태로.
    }

    // 다시 가져 오자.
    if( !GetOtherRecomInfo( dwUID_, kPacket.m_kPacket.m_vecRecommenders, SiKRecomHelper()->GetVersion() ) )
        SET_ERR_GOTO( ERR_RECOMMEND_02, END_PROC );

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_RECOMMENDER_DENY_ACK );

}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_CHECK_GACHA_REEL_POINT_REQ )
{
    std::map< int, std::map< std::pair< DWORD, DWORD >, std::map< int, int > > > mapGachaReelPoint; // nVersion, ( prData, mapLevelReelPoint )
    std::map< int, int > mapLevelReelPoint; // nLevel, nReelPoint
    std::pair< DWORD, DWORD > prData; // nGachaItemID, nType
    std::map< std::pair< DWORD, DWORD >, std::map< int, int > >::iterator mit;
    mapGachaReelPoint.clear();
    mapLevelReelPoint.clear();

    /*
    L"{ call dbo.GGGAGameGameReel_select_20110406 ( %d ) }"

    { call dbo.GGGAGameGameReel_select_20110406 ( @1 ) }
    @1: @iLoginUID_  int

    n select ( @1, @2, @3, @4, @5 )
    @1: Version   int
    @2: ItemID    int
    @3: Type      int
    @4: Level     int
    @5: ReelPoint int
    */

    DBJIF( m_kODBC.Query( L"{ call GGGAGameGameReel_select_20110406 ( %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nVersion = -1;
        GCITEMID nItemID = 0;
        int nType = 0;
        int nLevel = 0;
        int nReelPoint = 0;

        VERIFY_QUERY( m_kODBC >> nVersion
                              >> nItemID
                              >> nType
                              >> nLevel
                              >> nReelPoint );

        prData.first = nItemID;
        prData.second = nType;
        mapLevelReelPoint[ nLevel ] = std::max< int >( 0, nReelPoint );
        mapGachaReelPoint[ nVersion ][ prData ] = mapLevelReelPoint;
    }

    _QUEUING_ACK_TO_USER( EVENT_CHECK_GACHA_REEL_POINT_ACK, mapGachaReelPoint );
}

_INT_IMPL_ON_FUNC( EVENT_GACHA_REEL_POINT_UPDATE_DB, GAGHA_REEL_POINT )
{
    /*
    L"{ call dbo.GGGAGameGameReel_merge_20110406 ( %d, %d, %d, %d, %d, %d ) }"

    { call dbo.GGGAGameGameReel_merge_20110406 ( @1, @2, @3, @4, @5, @6 ) }
    @1: @iLoginUID_  int
    @2: @iVersion_   int
    @3: @iItemID_    int
    @4: @iType_      int
    @5: @iLevel_     int
    @6: @iReelPoint_ int
    */

    typedef std::map< int, std::map< std::pair< DWORD, DWORD >, std::map< int, int > > > mapGachaReelPointType;
    typedef std::map< std::pair< DWORD, DWORD >, std::map< int, int > > mapCharTypeReelPointType;
    typedef std::map< int, int > mapLevelReelPointType;

    BOOST_FOREACH( mapGachaReelPointType::value_type& rGachaReelPoint, kPacket_ ) {
        BOOST_FOREACH( mapCharTypeReelPointType::value_type& rCharTypeReelPointType, rGachaReelPoint.second ) {
            BOOST_FOREACH( mapLevelReelPointType::value_type& rLevelReelPointType, rCharTypeReelPointType.second ) {
                DBJIF( m_kODBC.Query( L"{ call GGGAGameGameReel_merge_20110406 ( %d, %d, %d, %d, %d, %d ) }",
                    dwUID_,                                                     // LoginUID
                    rGachaReelPoint.first,                                      // Version
                    static_cast< int >( rCharTypeReelPointType.first.first ),   // ItemID
                    static_cast< int >( rCharTypeReelPointType.first.second ),  // Type
                    rLevelReelPointType.first,                                  // Level
                    rLevelReelPointType.second ) );                             // ReelPoint

                int nOK = -99;
                if ( m_kODBC.BeginFetch() ) {
                    VERIFY_QUERY( m_kODBC >> nOK );
                }
                m_kODBC.EndFetch();

                if ( nOK != 0 ) {
                    START_LOG( cerr, L"가챠 버전별 릴 포인트 저장 실패. Name : " << strLogin_ )
                        << BUILD_LOG( nOK )
                        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                }
            }
        }
    }
}

_INT_IMPL_ON_FUNC( EVENT_CHANGE_VIRTUAL_CASH_COUPON_REQ, KEVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK )
{
    EVENT_CHECK_OUT( EVENT_CHANGE_VIRTUAL_CASH_COUPON_REQ );

    kPacket_.m_nOK = NetError::NET_OK;

    if ( false == m_kHelper.VirtualCashMergeWithType( dwUID_, 0, KVirtualCash::EVCMT_ADD_COUPON, kPacket_.m_nVirtualCashPoint, 0 ) ) {
        START_LOG( cerr, L"가상캐쉬 충전 실패 ")
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( kPacket_.m_kItem.m_dwID ) << END_LOG;

        kPacket_.m_nOK = NetError::ERR_VIRTUAL_CASH_COUPON_04;
    }

    _QUEUING_ACK_TO_USER( EVENT_CHANGE_VIRTUAL_CASH_COUPON_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_SANTA_DATA )
{
    SelectSanta();
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_VIRTUAL_CASH_RESTRICT_ITEM_LIST )
{
    /*
    20090522. mk8253.
    가상캐시로구매못하는아이템

    { call dbo.VCGA_virtual_cash_not_buy_item_select }
    @1 ; iEventUID int

    n return ( @1 )
    @1 ; ItemID int
    */
    std::vector<GCITEMID> vecItemList;
    GCITEMID dwItemID;

    DBJIF( m_kODBC.Query( L"{ call dbo.VCGA_virtual_cash_not_buy_item_select }" ) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> dwItemID );

        vecItemList.push_back( dwItemID );
    } // end while

    SiKVirtualCash()->UpdateRestrictItemList( vecItemList );

}



INT_IMPL_ON_FUNC( DB_EVENT_DUNGEON_HACKING_USER_NOT )
{
    /*
    20090604. microcat.

    { call dbo.UCGA_user_cracking_insert_dungeon ( @1, @2, @3, @4, @5, @6 ) }
    @1 ; @iLoginUID _   int
    @2 ; @iGameModeID_  int
    @3 ; @iDifficulty_  int
    @4 ; @iHackingType_ int
    @5 ; @iCharType_    int
    @6 ; @iPromotion    int

    1 return ( @1 )
    @1 ; OK int
    0         ; 성공
    -1        ; 유저 정보가 없음
    -101 이하 ; 트랜잭션 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.UCGA_user_cracking_insert_dungeon ( %d, %d, %d, %d, %d, %d ) }",
        dwUID_, kPacket_.m_nGameModeID, kPacket_.m_nDifficult, kPacket_.m_nHackingType, kPacket_.m_nCharType, kPacket_.m_nPromotion ) );

    int nOK = -99;

    if( m_kODBC.BeginFetch() ) 
    {
        VERIFY_QUERY( m_kODBC >> nOK );

        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();
}

_INT_IMPL_ON_FUNC( EVENT_GET_GCCLUB_CONNECT_BONUS_REQ, std::vector<KDropItemInfo> )
{
    EVENT_CHECK_OUT( EVENT_GET_GCCLUB_CONNECT_BONUS_REQ );
    JIF( !kPacket_.empty() );

    std::vector<KItem> kPacket;

    std::vector<KDropItemInfo>::iterator vit;
    for( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit )
    {
        if( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_GCCLUB, kPacket ) == false )
        {
            START_LOG( cerr, L"GC클럽 접속시 보너스 아이템 지급 DB작업 실패." << dwUID_ ) << END_LOG;
            return;
        }
    }

    QUEUING_ACK_TO_USER( EVENT_GET_GCCLUB_CONNECT_BONUS_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_GET_GCCLUB_ATTEND_BONUS_REQ, std::vector<KDropItemInfo> )
{
    EVENT_CHECK_OUT( EVENT_GET_GCCLUB_ATTEND_BONUS_REQ );
    JIF( !kPacket_.empty() );


    std::vector<KItem> kPacket;

    std::vector<KDropItemInfo>::iterator vit;
    for( vit = kPacket_.begin() ; vit != kPacket_.end() ; ++vit )
    {
        if( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_GCCLUB, kPacket ) == false )
        {
            START_LOG( cerr, L"GC클럽 출석시 보너스 아이템 지급 DB작업 실패" << dwUID_ ) << END_LOG;
            return;
        }
    }

    QUEUING_ACK_TO_USER( EVENT_GET_GCCLUB_ATTEND_BONUS_ACK );
}
_INT_IMPL_ON_FUNC( EVENT_MISSION_PACK_REGIST_REQ, KEVENT_MISSION_PACK_REGIST_ACK )
{
    EVENT_CHECK_OUT( EVENT_MISSION_PACK_REGIST_REQ );

    // 미션 등록
    std::set<int>::iterator sit;
    for ( sit = kPacket_.m_setMissionList.begin() ; sit != kPacket_.m_setMissionList.end() ; ++sit ) {

        KMissionData kMissionData;
        int nResultRegistMission = -99;
        if ( !RegistMission( dwUID_, cCharType_, *sit, kMissionData, nResultRegistMission ) ) {
            START_LOG( cerr, L"Mission Register Fail, Name : " << strLogin_ )
                << BUILD_LOG( *sit )
                << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
        }
        kPacket_.m_vecAutoMission.push_back( kMissionData );
    }

    kPacket_.m_nOK = NetError::NET_OK;

    START_LOG( clog, L"미션 패키지 DB 등록 ")
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket_.m_vecAutoMission.size() ) << END_LOG;

    _QUEUING_ACK_TO_USER( EVENT_MISSION_PACK_REGIST_ACK, kPacket_ );
}

INT_IMPL_ON_FUNC( EVENT_RAINBOW_JACKPOT_ITEM_REQ )
{
    KEVENT_RAINBOW_JACKPOT_ITEM_ACK kPacket;
    std::vector<KDropItemInfo>::iterator vit;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_RAINBOW_JACKPOT_ITEM_REQ );

    if( !CheckJackPotItemGet( dwUID_, kPacket_.m_nRainBowID, kPacket_.m_nDailyEventID, kPacket_.m_nJackPotID ) )
        goto END_PROC;

    if( !SetJackPotItemGet( dwUID_, kPacket_.m_nRainBowID, kPacket_.m_nDailyEventID, kPacket_.m_nJackPotID ) )
        goto END_PROC;

    for ( vit = kPacket_.m_vecRewardList.begin() ; vit != kPacket_.m_vecRewardList.end() ; ++vit ) {
        LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_RAINBOW_JACKPOT_ITEM, kPacket.m_vecItem ) );
    }

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_RAINBOW_JACKPOT_ITEM_ACK );
}

INT_IMPL_ON_FUNC( EVENT_EFFECT_TEX_REPORT )
{
    KEVENT_EFFECT_TEX_REPORT::iterator mit;
    int nFailCount = 0;
    int nTotalCount = 0;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        WORD wHi = HIWORD( mit->first );
        WORD wLo = LOWORD( mit->first );
        std::set<std::string>::iterator sit;
        for( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit )
        {
            ++nTotalCount;
            if( !InsertEffectTexture( wHi, wLo, KncUtil::toWideString( *sit ) ) )
                ++nFailCount;
        }
    }
    _LOG_CONDITION( nFailCount == 0, clog, cwarn, L"텍스쳐 기록... Total : " << nTotalCount << L", Fail : " << nFailCount ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_CO_OP_ITEM_BUY_REQ, KCoopEventItem )
{
    // 해당 아이템을 유저에게 넣어 준다.
    KEVENT_CO_OP_ITEM_BUY_ACK kPacket;
    kPacket.m_kMoneyItem.m_ItemID = static_cast<GCITEMID>(kPacket_.m_nSlotID); // User로 돌아 가서 이 아이템의 가격 만큼 화폐 아이템을 제거.
    kPacket.m_nOK = -99;

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_CO_OP_ITEM_BUY_REQ );

    if( !m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_dwItemID, kPacket_.m_nCount, kPacket_.m_nPeriod, KItemManager::EWIT_EVENT_CO_OP_ITEM_BUY, kPacket.m_vecItem ) )
    {
        SET_ERR_GOTO( ERR_CO_OP_03, END_PROC );
    }

    SET_ERROR( NET_OK );
END_PROC:
    // 성공, 실패 전송
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_CO_OP_ITEM_BUY_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_GET_USER_HACKING_COUNT_REQ )
{
    int kPacket = GetUserHackingCount( dwUID_ );
    QUEUING_ACK_TO_USER( EVENT_GET_USER_HACKING_COUNT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_ITEM_TRADE_REQ )
{
    KEVENT_ITEM_TRADE_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nItemID = kPacket_.m_nItemID;
    kPacket.m_nItemCount = kPacket_.m_nItemCount;

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_ITEM_TRADE_REQ );

    // db에 해당 아이템 넣어주기
    if( !m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kRewardItem.m_ItemID, kPacket_.m_kRewardItem.m_nDuration, kPacket_.m_kRewardItem.m_nPeriod, KItemManager::EWIT_ITEM_TRADE, kPacket.m_vecItem ) ) {
        START_LOG( cerr, L"아이템 교환 DB 작업 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_nItemID )
            << BUILD_LOG( kPacket_.m_nItemCount ) << END_LOG;
        SET_ERR_GOTO( ERR_ITEM_TRADE_03, END_PROC );
    }

    // DB에 기록 남기기.
    if( false == InsertTransformItemLog( dwUID_, kPacket_.m_nItemID, kPacket_.m_nItemCount, kPacket_.m_kRewardItem ) ) {
        START_LOG( cerr, L"아이템 교환 DB 로그 남기는 작업 실패. Name : " << strLogin_ )
            << BUILD_LOG( kPacket_.m_nItemID )
            << BUILD_LOG( kPacket_.m_nItemCount )
            << BUILD_LOG( kPacket_.m_kRewardItem.m_ItemID )
            << BUILD_LOG( kPacket_.m_kRewardItem.m_nDuration )
            << BUILD_LOG( kPacket_.m_kRewardItem.m_nPeriod ) << END_LOG;
    }

    SET_ERROR( NET_OK );

END_PROC:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,               kPacket.m_nOK, 0 ); // 성공
        ERR_CASE( ERR_ITEM_TRADE_03,    kPacket.m_nOK, 4 ); // GP캡슐 아이템 얻기 DB 작업 실패
    default:
        START_LOG( cerr, L"아이템 교환중 알수 없는 오류 ..  Name : " << strLogin_ ) << END_LOG;
        kPacket.m_nOK = -99;
    }

    LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kPacket_.m_nItemID )
        << BUILD_LOG( kPacket_.m_nItemCount )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_ItemID )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_nDuration )
        << BUILD_LOG( kPacket_.m_kRewardItem.m_nPeriod )
        << BUILD_LOG( kPacket.m_vecItem.size() )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_ITEM_TRADE_ACK );
}

INT_IMPL_ON_FUNC( EVENT_MISSION_ITEM_NOT )
{
    KEVENT_MISSION_DATE_CHANGE_ACK kPacket;
    // 아이템은 어차피 하나만 넣어줄 꺼니깐..,
    // 자동 등록 미션을 수행하는 아이템이기 때문에 2개가 되어도 다른 종류의 Item이 될듯..,

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_MISSION_ITEM_NOT );

    std::vector<KMissionDailyData>::iterator vit;
    for( vit = kPacket_.m_CurrentData.m_vecMissions.begin() ; vit != kPacket_.m_CurrentData.m_vecMissions.end() ; ++vit )
    {
        if( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, (GCITEMID)vit->m_nItemID, 1, -1, KItemManager::EWIT_EVENT_MISSION_ITEM, kPacket.m_vecItemAdd ) )
        {
            // 로그는 다시 적자..,
            START_LOG( cerr, L"미션 이벤트 아이템 지급 실패 UID : " << dwUID_ )
                << BUILD_LOG( vit->m_nItemID ) << END_LOG;
        }

        InsertDailyMissionLog( dwUID_, vit->m_nEventID, vit->m_nOrderID, vit->m_nItemID );
    }

    // 지워준 아이템에 대해서 유저에게 보낼 패킷에 복사하자.
    kPacket.m_vecItemDel.swap( kPacket_.m_vecItemDel );
    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_MISSION_DATE_CHANGE_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_MY_SUBSCRIPTION_INFO_REQ )
{
    /*
    20090916. mk8253
    획득한 횟수

    {call SSGA_subscription_user_select ( @1 )}
    @1 : LoginUID int

    n return
    EventUID int
    Count    int
    */
    MAP_INT_DWORD kPacket;

    DBJIF( m_kODBC.Query( L"{ call SSGA_subscription_user_select ( %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        int nEventUID = -99;     // 응모이벤트UID
        int nMyGiftCount = 0;    // 내가 획득한 경품 개수

        VERIFY_QUERY( m_kODBC >> nEventUID
                              >> nMyGiftCount );

        kPacket[nEventUID] = (DWORD)nMyGiftCount;
        START_LOG( clog, L"Subscription EventUID : " << nEventUID << L",My Gift Count : " << nMyGiftCount ) << END_LOG;
    }

    START_LOG( clog,L"My Subscription Info. size : " << kPacket.size() ) << END_LOG;
    QUEUING_ACK_TO_USER( EVENT_MY_SUBSCRIPTION_INFO_ACK );
}

INT_IMPL_ON_FUNC( EVENT_SUBSCRIPT_REQ )
{
    EVENT_CHECK_OUT( EVENT_SUBSCRIPT_REQ );
    /*
    20090916. mk8253
    응모권 교환

    {call SSGA_subscription_exchange ( @1, @2 )}
    @1 : LoginUID int
    @2 : EventUID   int

    1 return
    OK int
    0이상     : 남은 쿠폰수
    -1        : 존재하지 않는 유저
    -2        : 발동중이지 않은 이벤트
    -3        : 응모권 아이템이 부족함
    -101 이하 : 트랜잭션 에러
    */
    KEVENT_SUBSCRIPT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nEventUID = kPacket_.m_nEventUID;
    kPacket.m_nCurrentGiftNum = kPacket_.m_kEventInfo.m_nCurrentGiftNum;
    kPacket.m_kTicketItem = kPacket_.m_kTicketItem;
    kPacket.m_nMySubscriptionGiftCout = 0L;

    int nMaxReqCount = kPacket_.m_nTicketCount / kPacket_.m_kEventInfo.m_nNeedTicketNum; // 요청된 최대 교환 횟수
    int nCurrentReqCount = 0; // 실제 처리된 응모 교환 횟수

    // 응모권 교환하기.
    for( int i = 0 ; i < nMaxReqCount ; ++i )
    {
        int nRet = -99;

        DBJIF( m_kODBC.Query( L"{ call SSGA_subscription_exchange ( %d, %d ) }",
            dwUID_, kPacket_.m_nEventUID ) );

        if( m_kODBC.BeginFetch() ) 
        {
            VERIFY_QUERY( m_kODBC >> nRet );

            LOG_SUCCESS( nRet >= 0 )
                << BUILD_LOG( nRet )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

            if( nRet < 0 )
            {
                if( nCurrentReqCount > 0 )
                    break;

                // 한번도 성공하지 못했다면 에러.
                kPacket.m_nOK = nRet;
                goto END_PROC;
            }
        }
        m_kODBC.EndFetch();

        // 남은 경품 수량.
        kPacket.m_nCurrentGiftNum = nRet;
        // 실제 응모권 교환 횟수
        ++nCurrentReqCount;
    }

    if( nCurrentReqCount <= 0 )
    {
        goto END_PROC;
    }

    // 1번이라도 교환하면 성공
    kPacket.m_nOK = 0;

    // 실제 처리된 응모 교환 횟수만큼 경품 넣어주기.
    for( int i = 0 ; i < nCurrentReqCount ; ++i )
    {
        if( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, kPacket_.m_kEventInfo.m_kRewardItem.m_ItemID, kPacket_.m_kEventInfo.m_kRewardItem.m_nDuration, kPacket_.m_kEventInfo.m_kRewardItem.m_nPeriod, KItemManager::EWIT_EVENT_SUBSCRIPT, kPacket.m_vecItem ) == false )
        {
            START_LOG( cerr, L"낙장불입..경품 얻기 DB 작업 실패. Name : " << strLogin_ )
                << BUILD_LOG( kPacket_.m_kEventInfo.m_kRewardItem.m_ItemID )
                << BUILD_LOG( kPacket_.m_kEventInfo.m_kRewardItem.m_nDuration )
                << BUILD_LOG( kPacket_.m_kEventInfo.m_kRewardItem.m_nPeriod ) << END_LOG;
        }
    }

    // 사용된 응모권 아이템 개수
    int nUsedTicketCount = kPacket_.m_kEventInfo.m_nNeedTicketNum * nCurrentReqCount;
    // 동기화전 InitCount, Count값 변동값 있는지 체크.
    if( !CheckItemDuration( dwUID_, kPacket.m_kTicketItem.m_ItemUID, kPacket.m_kTicketItem.m_nInitCount, kPacket.m_kTicketItem.m_nCount ) ) {
        START_LOG( clog, L"응모권 아이템 창고 수량 동기화. Name : " << strLogin_ ) 
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kPacket.m_kTicketItem.m_ItemUID )
            << BUILD_LOG( kPacket.m_kTicketItem.m_nInitCount )
            << BUILD_LOG( kPacket.m_kTicketItem.m_nCount )
            << END_LOG;
    }
    // 사용된 응모권 제거
    if( UseCountItem( dwUID_, kPacket.m_kTicketItem.m_ItemUID, nUsedTicketCount ) == false )
    {
        START_LOG( cerr, L"낙장불입..응모권 개수 동기화 실패. Name : " << strLogin_ ) << END_LOG;
    }

    // 응모권 개수 갱신( = 현재개수 - 지불한개수 )
    kPacket.m_kTicketItem.m_nCount -= nUsedTicketCount;
    kPacket.m_nMySubscriptionGiftCout = nCurrentReqCount;  // 실제응모횟수

END_PROC:
    LOG_SUCCESS( kPacket.m_nOK == 0 )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nEventUID )
        << BUILD_LOG( kPacket.m_nCurrentGiftNum )
        << BUILD_LOG( kPacket.m_kTicketItem.m_ItemID )
        << BUILD_LOG( kPacket.m_kTicketItem.m_ItemUID )
        << BUILD_LOG( kPacket.m_kTicketItem.m_nCount )
        << BUILD_LOG( kPacket.m_nMySubscriptionGiftCout )
        << BUILD_LOG( kPacket_.m_nTicketCount )
        << BUILD_LOG( kPacket_.m_kEventInfo.m_nNeedTicketNum )
        << BUILD_LOG( nMaxReqCount )
        << BUILD_LOG( nCurrentReqCount )
        << BUILD_LOG( nUsedTicketCount ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_SUBSCRIPT_ACK );
}

INT_IMPL_ON_FUNC( EVENT_NPC_GIFTS_REQ )
{
    EVENT_CHECK_OUT( EVENT_NPC_GIFTS_REQ );
    KEVENT_NPC_GIFTS_ACK kPacket;
    kPacket.m_nEventID = kPacket_.m_nEventID;
    kPacket.m_nOK = -99;
    kPacket.m_vecItem.clear();

    /*
    20090814. mk8253
    1일 1회 아이템 지급 로그 검색
    { call dbo.DSEV_daily_supply_select ( @1, @2, @3 ) }
    @1 ; LoginUID int
    @2 ; EventUID int
    @3 ; sdtDate  SMALLDATETIME

    1 return ( @1 )
    @1 ; nOK (받았으면:1,안받았으면:0)
    */
    CTime cCurrentTime = CTime::GetCurrentTime();
    std::wstring m_strCurrentTime = (LPCTSTR)(cCurrentTime.Format( _T("%Y-%m-%d")));
    int nOK = -99;
    std::vector< KDropItemInfo >::iterator vit;

    DBJIF( m_kODBC.Query( L"{ call dbo.DSEV_daily_supply_select ( %d, %d, N'%s' ) }",
            dwUID_, kPacket_.m_nEventID, m_strCurrentTime.c_str() ) );

    if( m_kODBC.BeginFetch() ) 
    {
        VERIFY_QUERY( m_kODBC >> nOK );

        LOG_SUCCESS( nOK == 0 || nOK == 1 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();

    if( nOK == 1 )
    {
        // 1일1회 보상 아이템이 이미 받았음.
        SET_ERR_GOTO( ERR_NPC_GIFT_02, END_PROC );
    }

    // 아이템 넣어주기.
    for( vit = kPacket_.m_vecReqItem.begin() ; vit != kPacket_.m_vecReqItem.end() ; ++vit )
    {
        if( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_NPC_GIFT, kPacket.m_vecItem ) == false )
        {
            START_LOG( cerr, L"NPC Gift 아이템넣기 DB 작업 실패. UID : " << dwUID_ ) << END_LOG;
            SET_ERR_GOTO( ERR_NPC_GIFT_03, END_PROC );
        }
    }

    /* 아이템을 받아간 유저의 로그를 기록합니다.
    20090814. mk8253
    1일 1회 아이템 지급 로그 기록
    { call dbo.DSEV_daily_supply_insert ( @1, @2 ) }
    @1 ; LoginUID int
    @2 ; EventUID int

    1 return ( @1 )
    @1 ; OK
    -1        ; 유저 정보가 없음
    -2        ; 해당하는 이벤트가 없음
    -101 이하 ; 트랜잭션 에러 
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.DSEV_daily_supply_insert ( %d, %d ) }",
        dwUID_, kPacket_.m_nEventID ) );

    if( m_kODBC.BeginFetch() ) 
    {
        int nRet;
        VERIFY_QUERY( m_kODBC >> nRet );

        LOG_SUCCESS( nRet == 0 )
            << BUILD_LOG( nRet )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();

    SET_ERROR( NET_OK );
END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_NPC_GIFTS_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_GET_USER_BOARD_GAME_INFO )
{
    EVENT_CHECK_OUT( EVENT_GET_USER_BOARD_GAME_INFO );

    KEVENT_GET_USER_BOARD_GAME_INFO kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kUserBoardInfo.Clear();

    // 초기화 시간을 가져온다.
    DWORD dwInitHour = SiKDicePlay()->GetInitHour();
    DWORD dwTotalMap = SiKDicePlay()->GetTotalMapCount();
    DWORD dwVersion = SiKDicePlay()->GetDiceEventVersion();
    // 유저의 보드게임 정보 읽어오기.
    kPacket.m_nOK = GetUserBoardInfo( dwUID_, dwInitHour, dwTotalMap, kPacket.m_kUserBoardInfo, kPacket.m_tmGiveDiceLastTime, dwVersion );

    // 초기화 여부에 대한 체크를 여기서 진행하자.
    CTime tmCurrent = CTime::GetCurrentTime();
    if( kPacket.m_nOK == 2 || dwInitHour <= (DWORD)tmCurrent.GetHour() ) {
        // 현재 날이 시간을 세팅해서 넘겨주자.
        kPacket.m_tmTodayTime = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), 
            tmCurrent.GetMonth(), 
            tmCurrent.GetDay(), 
            dwInitHour, 0, 0 ) );
    } else {
        CTimeSpan ts(1,0,0,0);
        tmCurrent -= ts;
        kPacket.m_tmTodayTime = KncUtil::TimeToInt( CTime( tmCurrent.GetYear(), 
            tmCurrent.GetMonth(), 
            tmCurrent.GetDay(), 
            dwInitHour, 0, 0 ) );
    }

    QUEUING_ACK_TO_USER( EVENT_GET_USER_BOARD_GAME_INFO );

    START_LOG( clog, L" 유저의 보드게임 정보 읽어오기. Name : " << strLogin_ )
        << BUILD_LOG( kPacket.m_kUserBoardInfo.m_dwCurrentPosition )
        << BUILD_LOG( kPacket.m_kUserBoardInfo.m_dwGiveCount )
        << BUILD_LOG( kPacket.m_kUserBoardInfo.m_dwDiceCount )
        << BUILD_LOG( kPacket.m_kUserBoardInfo.m_dwRemainTime )
        << BUILD_LOGtm( CTime( kPacket.m_tmGiveDiceLastTime ) ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_DB_DICE_PLAY_REQ )
{
    EVENT_CHECK_OUT( EVENT_DB_DICE_PLAY_REQ );

    KEVENT_DICE_PLAY_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    int nResultRegistMission = 0;
    KMissionData kMissionData;

    NetError::SetLastNetError( kPacket_.m_nOK );

    // 주사위 보상 아이템 리스트가 있을 때만 DB처리를 진행하자.
    if( !kPacket_.m_vecGoalReward.empty() )
    {
        // 주사위를 굴려서 나온 보상 아이템을 넣어주자.
        std::vector<KDropItemInfo>::iterator vit;
        for( vit = kPacket_.m_vecGoalReward.begin(); vit != kPacket_.m_vecGoalReward.end(); ++vit )
        {
            if( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_DICE_PLAY, kPacket.m_vecItem ) )
            {
                START_LOG( cerr, L"주사위 이벤트 아이템 지급 실패 Name : " << strLogin_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
            }
        }
    }

    if( !kPacket_.m_vecItemReward.empty() )
    {
        std::vector<KDropItemInfo>::iterator vit2;
        for( vit2 = kPacket_.m_vecItemReward.begin(); vit2 != kPacket_.m_vecItemReward.end(); ++vit2 )
        {
            if( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit2->m_ItemID, vit2->m_nDuration, vit2->m_nPeriod, KItemManager::EWIT_EVENT_DICE_PLAY, kPacket.m_vecItem ) )
            {
                START_LOG( cerr, L"주사위 이벤트 아이템 지급 실패 Name : " << strLogin_ )
                    << BUILD_LOG( vit2->m_ItemID )
                    << BUILD_LOG( vit2->m_nDuration )
                    << BUILD_LOG( vit2->m_nPeriod ) << END_LOG;
            }
        }
    }

    // 자동 미션 등록에 대한 처리 추가.
    if( !kPacket_.m_vecMissionList.empty() )
    {
        std::vector<DWORD>::iterator vit3;
        for( vit3 = kPacket_.m_vecMissionList.begin(); vit3 != kPacket_.m_vecMissionList.end(); ++vit3 )
        {
            if( RegistMission( dwUID_, cCharType_, *vit3, kMissionData, nResultRegistMission ) )
            {
                kPacket.m_vecMissionList.push_back( kMissionData );
            }
        }
    }

    kPacket.m_FirstDiceNum = kPacket_.m_FirstDiceNum;
    kPacket.m_SecondDiceNum = kPacket_.m_SecondDiceNum;
    kPacket.m_kMyBoardInfo = kPacket_.m_kMyBoardInfo;

    // DB에 Item넣은 로그를 남겨주자.
    // 수량, 비수량 vector들의 empty() 체크는 데이터가 없는데 굳이 
    // 프로시저 호출을 할 필요가 없기 때문이다.
    if( !kPacket.m_vecItem.empty() )
    {
        std::vector<KItem>::iterator vit4;
        for( vit4 = kPacket.m_vecItem.begin(); vit4 != kPacket.m_vecItem.end(); ++vit4 )
        {
            HalloweenItemInsertLog( dwUID_, vit4->m_ItemID, vit4->m_nCount, vit4->m_nPeriod, SiKDicePlay()->GetDiceEventVersion() );
        }
    }

    // DB에 현재 서버에서 주사위 굴린 시간을 저장하기 위해서 서버의 현재 시간값이 필요하다.
    CTime tmCurrent = CTime::GetCurrentTime();
    std::wstring strDicePlayTime;
    strDicePlayTime = (LPCTSTR)(tmCurrent.Format( _T("%Y-%m-%d %H:%M:%S"))); // 현재 시간을 DB에 저장하기 위한 Format으로 변경.
    DWORD dwTotalMapCount = SiKDicePlay()->GetTotalMapCount();
    // DB에 주사위 굴린때마다 현재값을 갱신해 주고 로그를 남겨주자.
    UpdateUserBoardInfo( dwUID_, dwTotalMapCount, kPacket_.m_kMyBoardInfo, strDicePlayTime, SiKDicePlay()->GetDiceEventVersion() );

    // 처리는 성공. 미션 보상도 있지만 더이상 등록할 수 있는 미션이 없음.
    if( IS_CORRECT( ERR_DICE_PLAY_13 ) ) goto END_PROC;

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_DICE_PLAY_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || NetError::GetLastNetError() == NetError::ERR_DICE_PLAY_13 )
        << L" 주사위 보상 아이템 DB 작업 Msg : "<< NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG(  strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_vecItem.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_DICE_ITME_BUY_CHECK_REQ )
{
    EVENT_CHECK_OUT( EVENT_DICE_ITME_BUY_CHECK_REQ );

    KEVENT_DICE_ITME_BUY_CHECK_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    if ( CheckBuyLimitItem( dwUID_, kPacket_.m_dwItemID ) ) {
        SET_ERROR( NET_OK );
    }
    else {
        SET_ERROR( ERR_DICE_PLAY_12 );
    }

    kPacket.m_nOK = NetError::GetLastNetError();
    kPacket.m_dwItemID = kPacket_.m_dwItemID;
    QUEUING_ACK_TO_USER( EVENT_DICE_ITME_BUY_CHECK_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L" 주사위 아이템 체크 DB 작업 Msg : "<< NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG(  strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwItemID )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

INT_IMPL_ON_FUNC( EVENT_DB_DONATION_ITEM_GET_REQ )
{
    KEVENT_DONATION_ITEM_GET_ACK kPacket;
    std::vector<KDropItemInfo>::iterator vitReward;

    // 우선 해당 등급의 아이템을 받은 로그가 있는지 검사하자.
    // 클라이언트에서 아이템을 지급받으면 해당 등급에 대해서 비활성화 시키지만
    // 서버에서 한번더 체크하는 과정이 필요하다.
    /*
    L"{ call dbo.CSGAContributionSystemUser_check ( %d, %d ) }"

    { call dbo.CSGAContributionSystemUser_check ( @1, @2 ) }
    @1 ; @iLoginUID_  int
    @2 ; @iType_      tinyint

    1 return ( @1 )
    @1 ; OK int
    0  ; 성공
    -1 ; 유저 정보가 없음
    -2 ; 이미 등록되어 있음
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.CSGAContributionSystemUser_check ( %d, %d ) }",
                                        dwUID_, 
                                        kPacket_.m_kItemDonationInfo.m_dwGrade ) );

    if( m_kODBC.BeginFetch() ) 
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        LOG_SUCCESS( kPacket.m_nOK == 0 )
            << BUILD_LOG( kPacket.m_nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();

    switch( kPacket.m_nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;                            // 성공
    case -1: SET_ERR_GOTO( ERR_DONATION_04, END_PROC ); break;      // 유저 정보가 없음
    case -2: SET_ERR_GOTO( ERR_DONATION_06, END_PROC ); break;      // 이미 해당 아이템 지급 받았음.
    default:
        SET_ERR_GOTO( ERR_DONATION_05, END_PROC );
        START_LOG( cerr, L"유저 기부 아이템 로그 체크시 알수 없는 에러 , Name : " << strLogin_ )
            << L" Err Code : " << kPacket.m_nOK  << END_LOG;
    }

    for( vitReward = kPacket_.m_kItemDonationInfo.m_vecRewardList.begin(); vitReward != kPacket_.m_kItemDonationInfo.m_vecRewardList.end(); ++vitReward )
    {
        // db에 해당 아이템 넣어주기
        if( false == m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vitReward->m_ItemID, vitReward->m_nDuration, vitReward->m_nPeriod, KItemManager::EWIT_EVENT_DONATION_ITEM_GET, kPacket.m_vecItemIns) )
        {
            // 로그는 다시 적자..,
            START_LOG( cerr, L"송편 기부 등급 아이템 삽입 실패. UID : " << dwUID_ )
                << BUILD_LOG( kPacket_.m_kItemDonationInfo.m_dwGrade )
                << BUILD_LOG( vitReward->m_ItemID )
                << BUILD_LOG( vitReward->m_nDuration )
                << BUILD_LOG( vitReward->m_nPeriod ) << END_LOG;
            SET_ERR_GOTO( ERR_DONATION_07, END_PROC );
        }
    }

    // 성공적으로 DB에 아이템을 넣었으면 해당 아이템을 지급해 주자.
    /*
        L"{ call dbo.CSGAContributionSystemUser_insert ( %d, %d, %d ) }"

        { call dbo.CSGAContributionSystemUser_insert ( @1, @2, @3 ) }
        @1 ; @iLoginUID_  int
        @2 ; @iType_      tinyint
        @3 ; @iGoalCount_ bigint

        1 return ( @1 )
        @1 ; OK int
        0        ; 성공
        -1       ; 유저 정보가 없음
        -2       ; 이미 등록되어 있음
        -101이하 ; 트랜잭션 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.CSGAContributionSystemUser_insert ( %d, %d, %I64d ) }",
        dwUID_, 
        kPacket_.m_kItemDonationInfo.m_dwGrade, 
        kPacket_.m_kItemDonationInfo.m_biGradeNum ) );

    if( m_kODBC.BeginFetch() ) 
    {
        VERIFY_QUERY( m_kODBC >> kPacket.m_nOK );

        LOG_SUCCESS( kPacket.m_nOK == 0 )
            << BUILD_LOG( kPacket.m_nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();

    switch( kPacket.m_nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;                            // 성공
    case -1: SET_ERR_GOTO( ERR_DONATION_04, END_PROC ); break;      // 유저 정보가 없음
    case -2: SET_ERR_GOTO( ERR_DONATION_06, END_PROC ); break;      // 이미 해당 아이템 지급 받았음.
    default:
        SET_ERR_GOTO( ERR_DONATION_05, END_PROC );
        START_LOG( cerr, L"유저 기부 아이템 로그 삽입시 알수 없는 에러 , Name : " << strLogin_ )
            << L" Err Code : " << kPacket.m_nOK  << END_LOG;
    }

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    QUEUING_ACK_TO_USER( EVENT_DONATION_ITEM_GET_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_DONATION_INFO_REQ )
{
    KEVENT_DONATION_INFO_ACK kPacket;

    // 유저가 현재 획득한 등급의 전체 등급의 리스트를 가져오자!
    /*  
    L"{ call dbo.CSGAContributionSystemUser_select ( %d ) }"  

    { call dbo.CSGAContributionSystemUser_select ( @1 ) }  
    @1 ;     @iLoginUID_ int  

    n return ( @1 )  
    @1 ; Type tinyint 
    ; 유저가 획득한 아이템 Grade 전체값 리턴.
    */
    int nGrade = 0;
    DBJIF( m_kODBC.Query( L"{ call dbo.CSGAContributionSystemUser_select ( %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC
            >> nGrade );
        // DB에서 읽어온 Grade값을 클라이언트에게 전송해줄 Packet에 넣어주자.
        kPacket.m_setSuccessGrade.insert( nGrade );
    }

    QUEUING_ACK_TO_USER( EVENT_DONATION_INFO_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_USER_BINGO_DATA_REQ )
{
    EVENT_CHECK_OUT( EVENT_USER_BINGO_DATA_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_USER_BINGO_DATA_ACK kPacket;
    int nEventID = SiKQuizBingo()->GetEventID();

    kPacket.m_nAccPlayTime = GetBingoAccTime( dwUID_, nEventID );
    GetBingoPosition( dwUID_, nEventID, kPacket.m_vecBingoData );
    GetBingoCoinCountInfo( dwUID_, nEventID, kPacket.m_nTotalBingoCoinCount, kPacket.m_nUseBingoCoinCount );
    GetBingoCorrectQuestions( dwUID_, nEventID, kPacket.m_vecCorrectQuestions );
    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_USER_BINGO_DATA_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_CHANGE_BINGO_COIN_REQ, int )
{
    EVENT_CHECK_OUT( EVENT_CHANGE_BINGO_COIN_REQ );
    SET_ERROR( ERR_UNKNOWN );
    KEVENT_CHANGE_BINGO_COIN_ACK kPacket;
    kPacket.m_nRequestCount = kPacket_;
    kPacket.m_nBingoCoinCount = 0;
    int nEventID = SiKQuizBingo()->GetEventID();

    GetBingoCoinCountInfo( dwUID_, nEventID, kPacket.m_nTotalBingoCoinCount, kPacket.m_nUseBingoCoinCount );
    SetBingoCoinCountInfo( dwUID_, nEventID, kPacket.m_nTotalBingoCoinCount + kPacket_, kPacket.m_nUseBingoCoinCount );
    kPacket.m_nTotalBingoCoinCount += kPacket_;

    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_CHANGE_BINGO_COIN_ACK );

}

_INT_IMPL_ON_FUNC( EVENT_BINGO_QUESTION_REQ, int )
{
    KEVENT_BINGO_QUESTION_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_nQuestionID = kPacket_;
    EVENT_CHECK_OUT( EVENT_BINGO_QUESTION_REQ );
    int nEventID = SiKQuizBingo()->GetEventID();
    GetBingoCoinCountInfo( dwUID_, nEventID, kPacket.m_nTotalBingoCoinCount, kPacket.m_nUseBingoCoinCount );
    SetBingoCoinCountInfo( dwUID_, nEventID, kPacket.m_nTotalBingoCoinCount , ++kPacket.m_nUseBingoCoinCount );

    SET_ERROR( NET_OK );
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_BINGO_QUESTION_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_BINGO_ANSWER_REQ, KBingoAnswerReqForDB )
{
    KBingoAnswerAckData kPacket;
    kPacket.m_bCorrect = kPacket_.m_bCorrectAnswer;
    kPacket.m_prPos = kPacket_.m_prPos;
    std::vector<KDropItemInfo > vecRewards;
    EVENT_CHECK_OUT( EVENT_BINGO_ANSWER_REQ );
    SET_ERROR( ERR_UNKNOWN );
    int nEventID = SiKQuizBingo()->GetEventID();

    // 정답을 맞추었다면.
    if ( kPacket_.m_bCorrectAnswer ) {
        LIF( SetBingoCorrectAnswer( dwUID_, nEventID, kPacket_.m_nQuestionID, kPacket_.m_prPos.first, kPacket_.m_prPos.second ) );
        std::vector<KDropItemInfo > vecRewards;
        // 빙고 보상이 있다면.
        SiKQuizBingo()->GetLineBingReward( kPacket_.m_vecLineBingo, vecRewards );

        // 정답 보상이 있다면.
        SiKQuizBingo()->GetCorrectQuizReward( vecRewards );

        if ( !vecRewards.empty() ) {
            std::vector< KDropItemInfo >::iterator vit;
            for ( vit = vecRewards.begin() ; vit != vecRewards.end() ; ++vit ) {
                LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_BINGO_ANSWER, kPacket.m_vecItem ) );
            }
        }
    }

    SET_ERROR( NET_OK );
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_BINGO_ANSWER_ACK );
}

INT_IMPL_ON_FUNC( EVENT_DB_BINGO_ACC_TIME_UPDATE )
{
    int nEventID = SiKQuizBingo()->GetEventID();
    int nTotalCount = 0;
    int nUseCount = 0;
    if ( kPacket_.m_bIncreaseCoinCount ) {
        GetBingoCoinCountInfo( dwUID_, nEventID, nTotalCount, nUseCount );
        SetBingoCoinCountInfo( dwUID_, nEventID, nTotalCount + SiKQuizBingo()->GetCoinSupplyCount(), nUseCount );
    }

    if ( kPacket_.m_bChangeDate ) {
        CTime tmYesterDay( kPacket_.m_dateYesterDay.m_sYear, kPacket_.m_dateYesterDay.m_cMonth, kPacket_.m_dateYesterDay.m_cDay, 0, 0, 0);
        SetBingoAccTime( dwUID_, nEventID, kPacket_.m_nAccTime, tmYesterDay );
        SetBingoAccTime( dwUID_, nEventID, 0 ); // 날짜가 바뀌어서..

    } else {
        SetBingoAccTime( dwUID_, nEventID, kPacket_.m_nAccTime );
    }

}

INT_IMPL_ON_FUNC( EVENT_TOURNAMENT_DONATION_REQ )
{
    KEVENT_TOURNAMENT_DONATION_ACK kPacket;
    kPacket.m_nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_TOURNAMENT_DONATION_REQ );

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    // 유저의 기부 정보를 넣어주자.
    if( !SetTodayEventDoneDiff( dwUID_, kPacket_.m_nType, tmCurrent, kPacket_.m_nItemCount ) ) {
        SET_ERR_GOTO( ERR_TOURNAMENT_DONATION_04, END_PROC );
    }
    kPacket.m_itemID = kPacket_.m_itemID;
    kPacket.m_nItemCount = kPacket_.m_nItemCount;

    SET_ERROR( NET_OK );

END_PROC:
    //정상적으로 완료 되면. 유저에게 큐잉한다.
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_TOURNAMENT_DONATION_ACK );
    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L"대회선수 기부 완료 요청 처리, Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << L" Err Code : " << NetError::GetLastNetError() << dbg::endl
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_itemID )
        << BUILD_LOG( kPacket.m_nItemCount )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

_INT_IMPL_ON_FUNC( EVENT_ITEM_BUY_CHECK_REQ, GCITEMID )
{
    EVENT_CHECK_OUT( EVENT_ITEM_BUY_CHECK_REQ );

    KEVENT_ITEM_BUY_CHECK_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_ItemID = kPacket_;

    SET_ERROR( ERR_UNKNOWN );

    // 구매 가능한가?
    if( false == CheckBuyLimitItem( dwUID_, kPacket_ ) ) {
        SET_ERR_GOTO( ERR_ITEM_BUY_CHECK_01, END_PROC );
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK || 
        NetError::GetLastNetError() == NetError::ERR_ITEM_BUY_CHECK_01, L"아이템 구매 요청 처리, Msg : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_ItemID ) << END_LOG;

    QUEUING_ACK_TO_USER( EVENT_ITEM_BUY_CHECK_ACK );
}

INT_IMPL_ON_FUNC( EVENT_WEB_GACHA_COIN_TRADE_REQ )
{
    EVENT_CHECK_OUT( EVENT_WEB_GACHA_COIN_TRADE_REQ );

    KEVENT_WEB_GACHA_COIN_TRADE_ACK kPacket;
    kPacket.m_nOK = -99;

    // DB에 해당 아이템을 넣어주자.
    /*
    { call dbo.GSGA_gacha_system_exchange_ticket ( %d, %d ) }

    { call dbo.GSGA_gacha_system_exchange_ticket ( @1, @2 ) }
    @1 ; LoginUID int
    @2 ; GachaCount int

    1 return ( @1, @2 )
    @1 ; OK int
    @2 ; GachaCoin int

    OK
    -1 ; 존재하지 않는 유저
    -2 ; 유저의 가차정보가 없음
    -101 이하 ; 트랜젝션에러
    */

    {
        // DB 웹 가챠 코인 더하는 작업
        DBJIF( m_kODBC.Query( L"{ call dbo.GSGA_gacha_system_exchange_ticket ( %d, %d ) }",
            dwUID_, kPacket_.m_nItemCount ) );

        int nOK = -99;

        if( m_kODBC.BeginFetch() ) 
        {
            VERIFY_QUERY( m_kODBC >> nOK
                         >> kPacket.m_nWebGachaCoin );
        }
        m_kODBC.EndFetch();

        switch( nOK )
        {
        case 0:  SET_ERROR( NET_OK ); break;
        case -1: SET_ERROR( ERR_WEB_GACHA_TRADE_03 ); break;
        case -2: SET_ERROR( ERR_WEB_GACHA_TRADE_04 ); break;
        default:
            SET_ERROR( ERR_WEB_GACHA_TRADE_05 );
            START_LOG( cerr, L"가챠아이템 교환 작업시 트랜젝션 오류, Name : " << strLogin_ )
                << L" Err Code : " << nOK << END_LOG;
        }
    }
    SET_ERROR( NET_OK );

    kPacket.m_nOK = NetError::GetLastNetError();
    kPacket.m_itemID = kPacket_.m_itemID;
    kPacket.m_nItemCount = kPacket_.m_nItemCount;
    QUEUING_ACK_TO_USER( EVENT_WEB_GACHA_COIN_TRADE_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L" 가챠 아이템 DB 작업 Msg : "<< NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG(  strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_itemID )
        << BUILD_LOG( kPacket.m_nItemCount )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

}

_INT_IMPL_ON_FUNC( EVENT_ADVERTISING_EXPOSURE_COUNT_NOT, MAP_AD_COUNT )
{
    MAP_AD_COUNT::const_iterator cmit;
    for( cmit = kPacket_.begin() ; cmit != kPacket_.end() ; ++cmit )
    {
        AddServerCountData( cmit->second, cmit->first );
    }
}


INT_IMPL_ON_FUNC_NOPARAM( EVENT_GWC_EVENT_INFO_REQ )
{
    KEVENT_GWC_EVENT_INFO_ACK kPacket;
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_GWC_EVENT_INFO_REQ );
    // DB에서 해당 유저의 정보를 퍼온다.
    kPacket.m_nAccCount = GetGWCEventUserInfo( dwUID_ );
    // 매니저에 해당 유저의 정보를 추가 한다.
    SiKGWCSupport()->AddUserData( dwUID_, kPacket.m_nAccCount );
    // 매니저로 부터 이벤트 아이템 정보를 가져 온다.
    kPacket.m_dwEventItemID = SiKGWCSupport()->GetEventItem();
    SET_ERROR( NET_OK );

    // 유저에거 전달 한다.
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_GWC_EVENT_INFO_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_GWC_EVENT_DONATION_REQ, int )
{
    KEVENT_GWC_EVENT_DONATION_ACK kPacket;
    kPacket.m_nOK;
    kPacket.m_nUseCount = kPacket_;
    int nBeforeCount = INT_MAX;
    int nRewardCount = SiKGWCSupport()->GetRewardCount();

    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_GWC_EVENT_DONATION_REQ );

    nBeforeCount = GetGWCEventUserInfo( dwUID_ );
    // DB 에 요청 분을 머지 한다.
    if ( !UpdateGWCDonationData( dwUID_, kPacket.m_nUseCount ) )
    {
        SET_ERR_GOTO( ERR_GWC_SUPPORT_04, END_PROC );
    }
    // 매니저에 변화량을 적용 한다.
    SiKGWCSupport()->IncreaseUserAccCount( dwUID_, kPacket.m_nUseCount );
    SiKGWCSupport()->IncreaseNationAccCount( kPacket.m_nUseCount );
    kPacket.m_nAccCount = SiKGWCSupport()->GetUserAccCount( dwUID_ );

    if ( nBeforeCount < nRewardCount && kPacket.m_nAccCount >= nRewardCount ) {
        std::vector<KDropItemInfo> vecRewards;
        SiKGWCSupport()->GetRewardItems( vecRewards );

        if ( !vecRewards.empty() ) {
            std::vector<KDropItemInfo>::iterator vit;
            for ( vit = vecRewards.begin() ; vit != vecRewards.end() ; ++vit ) {
                LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_GWC_DANATION, kPacket.m_vecInv ) );
            }
        }
    }

    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_GWC_EVENT_DONATION_ACK );
}

INT_IMPL_ON_FUNC( EVENT_NEW_YEAR_DONATION_REQ )
{
    KEVENT_NEW_YEAR_DONATION_ACK kPacket;
    kPacket.Clear();
    time_t tmDate;
    SET_ERROR( ERR_UNKNOWN );
    EVENT_CHECK_OUT( EVENT_NEW_YEAR_DONATION_REQ );

    SiKNewDonationManager()->GetBeginTime( tmDate );

    // 유저의 기부 정보를 넣어주자.
    if( !AddServerCountData( kPacket_.m_nItemCount, kPacket_.m_dwType, tmDate ) ) {
        SET_ERR_GOTO( ERR_NEW_YEAR_DONATION_04, END_PROC );
    }

    // 해당 Type의 전체 기부 총액을 가져오자.
    kPacket.m_nTotalDonation = GetTotalServerCountData( tmDate, kPacket_.m_dwType );

    kPacket.m_dwType = kPacket_.m_dwType;
    kPacket.m_dwItemID = kPacket_.m_dwItemID;
    kPacket.m_nItemCount = kPacket_.m_nItemCount;

    SET_ERROR( NET_OK );

END_PROC:
    //정상적으로 완료 되면. 유저에게 큐잉한다.
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_NEW_YEAR_DONATION_ACK );
    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L"새해 소원 성취기부 완료 요청 처리, Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << L" Err Code : " << NetError::GetLastNetError() << dbg::endl
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwType )
        << BUILD_LOG( kPacket.m_dwItemID )
        << BUILD_LOG( kPacket.m_nTotalDonation )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_RECOMMEND_LEVELUP_NOT )
{
    // 반갑다 친구야.(5th)
    // 유저의 아이템 받은 로그를 DB에 남긴다.
    UpdateUserLevelUpInfo( dwUID_, kPacket_.m_dwUserUID, kPacket_.m_nCurrentLevel, SiKRecomHelper()->GetVersion() );
    UpdateLevelUpRecomReward( dwUID_, kPacket_.m_dwUserUID, kPacket_.m_nCurrentLevel, SiKRecomHelper()->GetVersion() );
    START_LOG( clog, L"신규/복귀유저 특정 레벨 달성해서 아이템 지급. Login : "  << strLogin_ << L",Level : " << kPacket_.m_nCurrentLevel ) << END_LOG;

    // 갱신된 유저정보 전달
    std::pair<int,int> kPacket;
    if( CheckRecomUserLevelup( dwUID_, kPacket_.m_dwUserUID, kPacket, SiKRecomHelper()->GetVersion() ) ) {
        QUEUING_ACK_TO_USER( EVENT_UPDATE_RECOM_USER_INFO );
    }

    // 추천한 유저가 아이템을 받을 수 있게 DB에 데이터를 넣어두자.
    // 추천한 유저가 없을 경우에는 해당 로그를 남길 필요가 없다.
    if( kPacket_.m_dwUserUID > 0 ) {
        InsertRecommendUserLevelUp( dwUID_, kPacket_.m_dwUserUID, SiKRecomHelper()->GetVersion() );
        START_LOG( clog, L"추천한 유저 아이템 지급 받을수 있게 수정." )
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( dwUID_ ) << END_LOG;
    }
}

INT_IMPL_ON_FUNC_NOPARAM( DB_EVENT_RECOMMEND_LEVELUP_CHECK )
{
    KEVENT_RECOMMEND_LEVELUP_NOT kPacket;
    kPacket.Clear();

    // 반갑다 친구야 5th. 지급할 아이템이 있는 체크.
    // map< UserUID, std::pair<UserLevel,RewardLevel> >
    std::map<DWORD, std::pair<int,int> > mapRecomUserList;
    if( !CheckRecommendSuppliedItem( dwUID_, mapRecomUserList, SiKRecomHelper()->GetVersion() ) ) {
        START_LOG( clog, L"반갑다친구야, 추천받은 유저가 없음. LoginID : " << strLogin_ ) << END_LOG;
        return;
    }

    std::map<DWORD, std::pair<int,int> >::const_iterator cmit;
    for( cmit = mapRecomUserList.begin(); cmit != mapRecomUserList.end(); ++cmit ) {
        // 달성레벨에 따른 지급 아이템 정보 받아오기
        std::vector<KDropItemInfo> vecRewardList;
        SiKRecomHelper()->GetRecommendLevelUpReward( cmit->second, vecRewardList );

        // 아이템 지급
        std::vector< KDropItemInfo >::iterator vit;
        for( vit = vecRewardList.begin(); vit != vecRewardList.end(); ++vit ) {
            LIF( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_EVENT_RECOMMEND_LEVELUP, kPacket.m_vecItem ) );
        }

        // 아이템 지급후에 해당 아이템 지급받았다고 갱신.
        UpdateRecommendSuppliedItem( dwUID_, cmit->first, cmit->second.first, SiKRecomHelper()->GetVersion() );
        ++kPacket.m_nCount;
    }

    QUEUING_ACK_TO_USER( EVENT_RECOMMEND_LEVELUP_NOT );
}

INT_IMPL_ON_FUNC( DB_EVENT_INIT_DICE_PLAY_REQ )
{
    DWORD dwInitHour = SiKDicePlay()->GetInitHour();
    DWORD dwTotalMapCount = SiKDicePlay()->GetTotalMapCount();

    SetInitDicePlayLog( dwUID_, dwInitHour ); // 초기화 로그 기록.
    // DB에 주사위 초기화 기록을 한다.
    UpdateUserBoardInfo( dwUID_, dwTotalMapCount, kPacket_.m_kMyBoardInfo, kPacket_.m_wstrDicePlayTime, SiKDicePlay()->GetDiceEventVersion() );
}

INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_DUNGEON_STAT_NOT )
{
    // 던전 통계
    bool bRet = UpdateDungeonStat( kPacket_.m_mapDungeonStat );
    bRet = UpdateBossDungeonStat( kPacket_.m_vecBossDungeonStat );

    LOG_SUCCESS( bRet == true )
        << L"던전 통계 기록 결과 " 
        << BUILD_LOG( bRet )
        << BUILD_LOG( kPacket_.m_mapDungeonStat.size() )
        << BUILD_LOG( kPacket_.m_vecBossDungeonStat.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_CHAR_PROMOTION_UPDATE_REQ )
{
    EVENT_CHECK_OUT( DB_EVENT_CHAR_PROMOTION_UPDATE_REQ );
    KEVENT_CHAR_PROMOTION_UPDATE_ACK kPacket;
    kPacket.m_nOK = -99;
    KMissionInfo kMissionInfo;
    std::vector<KMissionReward> vecRewards;
    vecRewards.clear();
    int nResult = -99;

    SET_ERROR( ERR_UNKNOWN );

    kPacket.m_cCharType = kPacket_.m_cCharType;
    kPacket.m_cPromotionLevel = kPacket_.m_cCharPromotion;
    kPacket.m_dwMissionID = kPacket_.m_dwMissionID;
    kPacket.m_dwItemID = kPacket_.m_dwItemID;

    // 미션 정보 가져오기 실패.
    if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, kPacket_.m_dwMissionID ) ) {
        SET_ERR_GOTO( ERR_CHAR_PROMOTION_UPDATE_06, END_PROC ); 
    }

    // 여기서 DB 프로시저의 구분이 있어야 한다. 
    // 이미 등록된 미션의 경우 기존에 있는 미션 완료 프로시저를 호출.
    // 등록되지 않은 미션의 경우 미션이 완료했다는 로그를 남기는 프로시저 호출.
    if( 1 == kPacket_.m_nMissionState ) {
        // 미션 완료에 대한 DB처리.
        if( !CompleteMissionInfo( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, kMissionInfo.m_bUnique, KMissionInfo::DEL_TYPE_USER_COMPLETE, nResult ) ) {
            goto END_PROC;
        }
    } else {
        // 미션 완료에 대한 DB처리.
        if( !PromotionCompleteMission( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, nResult ) ) {
            goto END_PROC;
        }
    }

    // 전직에 대한 DB처리 진행.
    if( !UpdateCharPromotion( strLogin_, kPacket_.m_cCharType, kPacket_.m_cCharPromotion, nResult ) ) {
        goto END_PROC;
    }

    // 아이템에 대한 처리는 서버 스크립트에 저장된 보상아이템을 기준으로 보상한다.
    if( !kPacket_.m_vecRewards.empty() ) {
        std::vector<KDropItemInfo>::iterator vit;
        for( vit = kPacket_.m_vecRewards.begin(); vit != kPacket_.m_vecRewards.end(); ++vit ) {
            if( false == m_kHelper.InsertSingleItemJob( dwUID_, kPacket_.m_cCharType, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_PROMOTION_REWARD, kPacket.m_vecRewards ) )
            {
                START_LOG( cerr, L"주사위 이벤트 아이템 지급 실패 Name : " << strLogin_ )
                    << BUILD_LOG( vit->m_ItemID )
                    << BUILD_LOG( vit->m_nDuration )
                    << BUILD_LOG( vit->m_nPeriod ) << END_LOG;
            }
        }
    }

    SET_ERROR( NET_OK );
END_PROC:
    //정상적으로 완료 되면. 유저에게 큐잉한다.
    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_CHAR_PROMOTION_UPDATE_ACK );

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << L"전직 시스템 DB 요청 처리, Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << L" Err Code : " << NetError::GetLastNetError() << dbg::endl
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_dwMissionID )
        << BUILD_LOGc( kPacket.m_cCharType )
        << BUILD_LOGc( kPacket.m_cPromotionLevel )
        << BUILD_LOG( kPacket.m_vecRewards.size() ) << END_LOG;
}
