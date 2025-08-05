#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include <boost/bind.hpp>
#include "GSDBLayer.h"
#include "Room.h"
#include "ResultManager.h"
#include "Log4.h"
#include "PVPMatchManager.h"
#include "RitasChristmas.h"
#include "ItemManager.h"
#include "GuildManager.h"
#include "RankManager.h"
#include "ItemManager.h"
#include "EventType.h"
#include "SkillManager.h"
#include "PostConfig.h"
#include <boost/foreach.hpp>

#define CLASS_TYPE KGSDBThread

bool KGSDBThread::GetNewPostLetterInfo( IN const std::wstring& strLogin_, IN const DWORD dwUID_, IN const char cCharType_, IN OUT std::vector<KPostItemInfo>& vecPostItem_ )
{
    std::map<GCITEMUID,KItem> mapPostWaitItem;
    mapPostWaitItem.clear();

    std::map<POSTUID,std::vector<KItem> >::iterator mitPostItem;
    std::map<POSTUID,std::vector<KItem> > mapPostItem;
    mapPostItem.clear();

    std::vector<KPostItemInfo>::iterator vitPostInfo;
    vecPostItem_.clear();

    // 1. 새 우편 리스트 받아오기
    if ( false == m_kHelper.Post_NewPost_select( dwUID_, cCharType_, vecPostItem_ ) ) {
        START_LOG( cerr, L"새 우편 리스트 받아오기 DB처리 실패.. LoginID : " << strLogin_ ) << END_LOG;
        return false;
    }

    if ( vecPostItem_.empty() ) {
        START_LOG( cwarn, L"신규 우편이 없음.. LoginID : " << strLogin_ ) << END_LOG;
        return false;
    }

    // 2. 우편 첨부아이템 정보 받아오기
    if ( false == m_kHelper.PostItem_select( dwUID_, cCharType_, mapPostItem ) ) {
        START_LOG( cerr, L"우편 첨부아이템 정보 받아오기 실패.. LoginID : " << strLogin_ ) << END_LOG;
        return false;
    }

    // 2-1. 해당 ItemUID의 대기아이템 정보 받아오기
    if ( false == mapPostItem.empty() ) {
        if ( false == m_kHelper.WaitItem_post_select( dwUID_, cCharType_, mapPostWaitItem ) ) {
            START_LOG( cerr, L"해당 ItemUID의 대기아이템 정보 받아오기 실패.. LoginID : " << strLogin_ ) << END_LOG;
            return false;
        }
    }

    // 2-2. 첨부아이템 우편정보에 매칭시키기
    for ( vitPostInfo = vecPostItem_.begin() ; vitPostInfo != vecPostItem_.end() ; ++vitPostInfo ) {
        mitPostItem = mapPostItem.find( vitPostInfo->m_PostUID );
        if ( mitPostItem == mapPostItem.end() ) {
            START_LOG( clog, L"PostUID의 우편대기아이템 리스트가 없음.. PostUID : " << vitPostInfo->m_PostUID << L", Login : " << strLogin_ )
                << BUILD_LOG( mapPostItem.size() ) << END_LOG;
            continue;
        }

        const POSTUID& PostUID = vitPostInfo->m_PostUID;
        std::vector<KItem>& vecPostItemUID = mitPostItem->second;

        std::vector<KItem>::iterator vitItem;
        for ( vitItem = vecPostItemUID.begin() ; vitItem != vecPostItemUID.end() ; ++vitItem ) {
            // 2-2-1.우편 대기아이템 리스트에서 검색 (시스템우편)
            std::map<GCITEMUID,KItem>::iterator mitPostWaitItem;
            mitPostWaitItem = mapPostWaitItem.find( vitItem->m_ItemUID );
            if ( mitPostWaitItem != mapPostWaitItem.end() ) {
                vitPostInfo->m_vecItem.push_back( mitPostWaitItem->second );
                continue;
            }

            if( vitItem->m_ItemID != 0 ) {
                vitPostInfo->m_vecItem.push_back( *vitItem );
                continue;
            }
/*
            // 2-2-2.보낸 유저 인벤토리에서 검색 (유저우편)
            if ( m_kHelper.PostItem_detail_select( dwUID_, cCharType_, PostUID, vitPostInfo->m_cScriptType, vitItem->m_ItemUID, vitItem->m_ItemID, vitItem->m_nCount, vitItem->m_nPeriod ) ) {
                vitPostInfo->m_vecItem.push_back( *vitItem );
                continue;
            }
*/
            START_LOG( cerr, L"첨부아이템 세부정보를 찾을 수 없음.. PostUID : " << PostUID << L", Login : " << strLogin_ << L", ItemUID" << vitItem->m_ItemUID ) << END_LOG;
        }

        // 우편 상태 변경 (새 우편->읽지않음)
        int nOK = m_kHelper.Post_State_update( dwUID_, cCharType_, PostUID, KPostItemInfo::LS_NOT_READ );
        if ( nOK != 0 ) {
            START_LOG( cerr, L"우편 상태변경 실패.. PostUID : " << PostUID << L", Login : " << strLogin_ ) << END_LOG;
        }
    } 

    return true;
}

bool KGSDBThread::OlympicUserData_merge( IN const DWORD& dwUID_, IN const int& nIncreasedData_, IN time_t& tmTodayDate_, IN const int& nGameType_, IN const int& nTeamType_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012UserMedal_merge_20120712 ( %d, %d, %d, %d, %d ) }"

    { call dbo.LOEVLondonOlympic2012UserMedal_merge_20120712 ( @1, @2, @3, @4, @5 ) }
    @1: @iLoginUID_input      int
    @2: @sdtRegDateD_input    smalldatetime
    @3: @iGameType_input      int
    @4: @iTeamType_input      int
    @5: @iMedalCNT_diff_input int
        
    1 return ( @1 )
    @1: OK int

    0       : 성공
    -1      : 유저 정보가 업음
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012UserMedal_merge_20120712 ( %d, N'%s', %d, %d, %d ) }",
        dwUID_, (LPCTSTR)CTime(tmTodayDate_).Format( L"%Y%m%d" ), nGameType_, nTeamType_, nIncreasedData_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 올림픽 정보 갱신 UserUID: " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOGtm( CTime(tmTodayDate_) )
        << BUILD_LOG( nGameType_ )
        << BUILD_LOG( nTeamType_ )
        << BUILD_LOG( nIncreasedData_ )
        << END_LOG;

    return ( nOK == 0 );
}

void KGSDBThread::GetOlympicUserTimeIndex( IN const DWORD& dwUID_, OUT time_t& tmTodayDate_, OUT int& nTimeIndex_, OUT DWORD& dwGrade_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012UserIndex_select_20120712( %d ) }"

    { call dbo.LOEVLondonOlympic2012UserIndex_select_20120712( @1 ) }
    @1: @iLoginUID_input int

    1 return ( @1, @2 )
    @1: RegDateD    smalldatetime
    yyyy-mm-dd
    @2: TimeIndex   int
    @3: RewardIndex int
    */

    tmTodayDate_ = 0;
    nTimeIndex_ = -1;
    dwGrade_ = 0;

    DBJIF( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012UserIndex_select_20120712  ( %d ) }",
        dwUID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> tmTodayDate_ 
                              >> nTimeIndex_
                              >> dwGrade_ );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 TimeIndex 정보 가져오기 UserUID: " << dwUID_ )
        << BUILD_LOGtm( CTime(tmTodayDate_) )
        << BUILD_LOG( nTimeIndex_ )
        << BUILD_LOG( dwGrade_ )
        << END_LOG;
}

void KGSDBThread::GetOlympicUserAccumulateData( IN const DWORD& dwUID_, IN time_t& tmTodayDate_, OUT int& nMyAccumulateData_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012UserMedal_select_20120712( %d, N'%s' ) }"

    { call dbo.LOEVLondonOlympic2012UserMedal_select_20120712( @1, @2 ) }
    @1: @iLoginUID_input   int
    @2: @sdtRegDateD_input smalldatetime
    yyyy-mm-dd

    n return ( @1, @2, @3 )
    @1: GameType int
    @2: TeamType int
    @3: MedalCNT int
    */

    int nGameType = -1;
    int nTeamType = -1;
    int nMedalCnt = -1;

    DBJIF( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012UserMedal_select_20120712( %d, N'%s' ) }",
        dwUID_, (LPCTSTR)CTime(tmTodayDate_).Format( L"%Y%m%d" ) ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nGameType 
                              >> nTeamType
                              >> nMedalCnt );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 누적치 정보 가져오기 UserUID: " << dwUID_ )
        << BUILD_LOGtm( CTime(tmTodayDate_) )
        << BUILD_LOG( nGameType )
        << BUILD_LOG( nTeamType )
        << BUILD_LOG( nMedalCnt )
        << END_LOG;
    if( nMedalCnt == -1 ) { // 누적치 값이 없으면 초기값 세팅.
        nMyAccumulateData_ = 0;
    } else {
        nMyAccumulateData_ = nMedalCnt;
    }
}

bool KGSDBThread::UpdateOlympicUserTimeIndex( IN const DWORD& dwUID_, IN time_t& tmTodayDate_, IN int& nTimeIndex_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012UserTimeIndex_merge_20120712 ( %d, N'%s', %d ) }"

    { call dbo.LOEVLondonOlympic2012UserTimeIndex_merge_20120712 ( @1, @2, @3 ) }
    @1: @iLoginUID_input   int
    @2: @sdtRegDateD_input smalldatetime
    @3: @iTimeIndex_input  int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 업음
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012UserTimeIndex_merge_20120712 ( %d, N'%s', %d ) }",
        dwUID_, (LPCTSTR)CTime(tmTodayDate_).Format( L"%Y%m%d" ), nTimeIndex_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 올림픽 정보 갱신 UserUID: " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOGtm( CTime(tmTodayDate_) )
        << BUILD_LOG( nTimeIndex_ )
        << END_LOG;

    return true;
}

bool KGSDBThread::UpdateOlympicUserGradeData( IN const DWORD& dwUID_, IN const DWORD& dwGrade_ )
{
    /*
    L"{ call dbo.LOEVLondonOlympic2012UserRewardIndex_merge_20120712 ( %d, %d ) }"

    { call dbo.LOEVLondonOlympic2012UserRewardIndex_merge_20120712 ( @1, @2 ) }
    @1: @iLoginUID_input    int
    @2: @iRewardIndex_input int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 업음
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.LOEVLondonOlympic2012UserRewardIndex_merge_20120712 ( %d, %d ) }",
        dwUID_, dwGrade_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 올림픽 보상 등급 정보 갱신 UserUID: " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( dwGrade_ )
        << END_LOG;

    return true;
}

bool KGSDBThread::InsertDungeonLoadingInfo( IN const DWORD& dwUserUID_, IN const KUserLoadingStat& kUserLoadingStat_ )
{
    /*
    L"{ call dbo.RDGARaidDungeon_insert ( %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.RDGARaidDungeon_insert ( @1, @2, @3, @4, @5, @6, @7 ) }
    @1: @iModeID_               int --던전별
    @2: @iLoginUID_             int --방장UID
    @3: @iPartyNumber_          int --파티원수
    @4: @iLodigOutCount_        int --로딩중 튕긴수
    @5: @iAverageLodingTime_    int --평균 로딩시간
    @6: @iAverageFPS_           int --평균FPS
    @7: @iPlayOutCount_         int --플레이중 튕긴수

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 없음
    -2      : 던전 정보가 없음
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.RDGARaidDungeon_insert ( %d, %d, %d, %d, %d, %d, %d ) }",
        kUserLoadingStat_.m_nGameMode, dwUserUID_, kUserLoadingStat_.m_nPartyUserNum, kUserLoadingStat_.m_nLoadingOutUserCnt, kUserLoadingStat_.m_nAvgLoadingComplete, kUserLoadingStat_.m_nAvgFPS, kUserLoadingStat_.m_nPlayingOutUserCnt ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"던전 로딩 통계 정보" )
        << BUILD_LOG( nOK )
        << BUILD_LOG( kUserLoadingStat_.m_nGameMode )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( kUserLoadingStat_.m_nPartyUserNum )
        << BUILD_LOG( kUserLoadingStat_.m_nLoadingOutUserCnt )
        << BUILD_LOG( kUserLoadingStat_.m_nAvgLoadingComplete )
        << BUILD_LOG( kUserLoadingStat_.m_nAvgFPS )
        << BUILD_LOG( kUserLoadingStat_.m_nPlayingOutUserCnt )
        << END_LOG;

    return true;
}

bool KGSDBThread::InsertUserChangeCharCount( IN const KDB_EVENT_USER_CHANGE_CHAR_STAT& vecList_ )
{
    /*
    L"{ call dbo.CCGACharacterChangeCNT_insert_20120806 ( %d, N'%s', %d, %d, %d, %d ) }"

    { call dbo.CCGACharacterChangeCNT_insert_20120806 ( @1, @2, @3, @4, @5, @6 ) }
    @1: @iLoginUID_input         int
    @2: @dtSessionRegDateA_input datetime
    @3: @iCharType_input         int
    @4: @iLevel_input            int
    @5: @iModeID_input           int
    @6: @iCNT_input              int

    1 return ( @1 )
    @1: OK
    0       : 성공
    -101이하: 트랜젝션 에러
    */

    KDB_EVENT_USER_CHANGE_CHAR_STAT::const_iterator cvit;
    for ( cvit = vecList_.begin() ; cvit != vecList_.end() ; ++cvit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.CCGACharacterChangeCNT_insert_20120806 ( %d, N'%s', %d, %d, %d, %d ) }",
            cvit->m_dwUserUID,
            cvit->m_wstrStartDate.c_str(),
            static_cast<int>(cvit->m_cCharType),
            cvit->m_dwLevel,
            cvit->m_nModeID,
            cvit->m_dwChangeCharTypeCnt ) );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        _LOG_SUCCESS( nRet == 0, L"유저 캐릭터 변경 횟수 통계 DB기록.. Result : " << nRet )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    return true;
}

bool KGSDBThread::InsertUserModeClearInfo( IN const KDB_EVENT_USER_MODE_CLEAR_STAT& vecList_ )
{
    /*
    L"{ call dbo.MUGAModeUniqueUser_insert_20120807 ( %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.MUGAModeUniqueUser_insert_20120807 ( @1, @2, @3, @4, @5, @6, @7, @8 ) }
    @1: @iLoginUID_input   int
    @2: @iCharType_input   int
    @3: @iPromotion_input  int
    @4: @iLevel_input      int
    @5: @iModeID_input     int
    @6: @iDifficulty_input int
    @7: @iExp_input        int
    @8: @iGP_input         int

    1 return ( @1 )
    @1: OK
    0       : 성공
    -101이하: 트랜젝션 에러
    */

    KDB_EVENT_USER_MODE_CLEAR_STAT::const_iterator cvit;
    for ( cvit = vecList_.begin() ; cvit != vecList_.end() ; ++cvit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.MUGAModeUniqueUser_insert_20120807 ( %d, %d, %d, %d, %d, %d, %d, %d ) }",
            cvit->m_dwUserUID,
            static_cast<int>(cvit->m_cCharType),
            static_cast<int>(cvit->m_cCharPromotion),
            cvit->m_dwLevel,
            cvit->m_nModeID,
            cvit->m_nDifficult,
            cvit->m_nRecvExp,
            cvit->m_nAddtionalGP ) );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        _LOG_SUCCESS( nRet == 0, L"유저 모드 클리어 정보 통계 DB기록.. Result : " << nRet )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    return true;
}

bool KGSDBThread::RoomMonsterKillInfo_insert( IN const KDB_EVENT_ROOM_MONSTER_KILL_STAT& kMonKillPacket_ )
{
    /*
    L"{ call dbo.SMGAStageMonsterKillCount_insert_20120823 ( %d, %d, %d, %d, %I64d, %d, %d ) }"

    { call dbo.SMGAStageMonsterKillCount_insert_20120823 ( @1, @2, @3, @4, @5, @6, @7 ) }
    @1: @iModeID_input     int
    @2: @iDifficulty_input int
    @3: @iStageID_input    int
    @4: @iMonsterID_input  int
    @5: @iCNT_input        bigint
    @6: @iLoginUID_input   int
    @7: @iPartyCNT_input   int

    1 return ( @1 )
    @1: OK
    0       : 성공
    -101이하: 트랜젝션 에러
    */

    std::map<std::pair<int,int>, int>::const_iterator cmit;
    for ( cmit = kMonKillPacket_.m_mapStageMonKillCount.begin() ; cmit != kMonKillPacket_.m_mapStageMonKillCount.end() ; ++cmit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.SMGAStageMonsterKillCount_insert_20120823 ( %d, %d, %d, %d, %I64d, %d, %d ) }",
            kMonKillPacket_.m_nModeID,
            kMonKillPacket_.m_nDifficult,
            cmit->first.first,
            cmit->first.second,
            static_cast<__int64>( cmit->second ),
            kMonKillPacket_.m_dwUserUID,
            kMonKillPacket_.m_nPartyCount ) );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        _LOG_SUCCESS( nRet == 0, L"던전 몬스터 Kill 통계.. Result : " << nRet )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }
    return true;
}

bool KGSDBThread::CheckCrashBefore( IN const std::wstring& strIP_, IN const USHORT usPort_ )
{    // extraflag 를 가져와서 서버 크래시 발생했는지 확인하기.
    /*
    L"{ call dbo.GSGAGameServerInfo_select_20121107 ( %s, %d ) }"

    { call dbo.GSGAGameServerInfo_select_20121107 ( @1, @2 ) }
    @1: @ServerIP
    @2: @ServerPort

    1 select ( @1 )
    @1: ExtraFlag int
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGameServerInfo_select_20121107 ( N'%s', %d ) }",
        strIP_.c_str(), usPort_ ) );

    int nExtraFlag = 0;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nExtraFlag );
        m_kODBC.EndFetch();
    }
    if ( nExtraFlag == -1 ) {
        return true;
    }

    return false;
}

void KGSDBThread::GetUserCharLevelEventData( IN const DWORD& dwUID_, IN const DWORD& dwVersion_, OUT std::set<char>& setCharLevelEvent_ )
{
    /*
    L"{ call dbo.CIGACharacterInfoTimestamp_select_20120910( %d, %d ) }"

    { call dbo.CIGACharacterInfoTimestamp_select_20120910( @1, @2 ) }
    @1: @iLoginUID_input int
    @2: @iVersion_input  int

    n return ( @1 )
    @1: CharType int
    */

    setCharLevelEvent_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.CIGACharacterInfoTimestamp_select_20120910( %d, %d ) }", dwUID_, dwVersion_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nCharType;
        VERIFY_QUERY( m_kODBC >> nCharType );
        setCharLevelEvent_.insert( (char)nCharType );
    }

    START_LOG( clog, L"캐릭터 특정 레벨 보상 이벤트 정보 가져오기 " )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( dwVersion_ )
        << BUILD_LOG( setCharLevelEvent_.size() )
        << END_LOG;
}

bool KGSDBThread::InsertUserCharLevelEventData( IN const DWORD& dwUID_, IN const DWORD& dwVersion_, IN const char& cCharType_ )
{
    /*
    L"{ call dbo.CIGACharacterInfoTimestamp_insert_20120910 ( %d, %d, %d ) }"

    { call dbo.CIGACharacterInfoTimestamp_insert_20120910 ( @1, @2, @3 ) }
    @1: @iLoginUID_input int
    @2: @iVersion_input  int
    @3: @iCharType_input int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 업음
    -2      : 이미 등록되었음
    -101이하: 트랜잭션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CIGACharacterInfoTimestamp_insert_20120910 ( %d, %d, %d ) }",
        dwUID_,
        dwVersion_,
        cCharType_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"유저 캐릭터 레벨 이벤트 정보 통계 DB기록.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::InsertItemComposeRewardLog( IN const DWORD& dwUID_, IN const DWORD& dwVersion_, IN std::vector<GCITEMID>& vecList_, IN const GCITEMID& dwItemID_ )
{
    /*
    L"{ call dbo.ICGAItemCoposition_insert_20120919 ( %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.ICGAItemCoposition_insert_20120919 ( @1, @2, @3, @4, @5, @6, @7 ) }
    @1: @iLoginUID_input        int
    @2: @iVersion_input         int
    @3: @iRewardItemID_input    int
    @4: @iMaterialItemID1_input int
    @5: @iMaterialItemID2_input int
    @6: @iMaterialItemID3_input int
    @7: @iMaterialItemID4_input int

    1 return ( @1 )
    @1: OK
    0       : 성공
    -101이하: 트랜젝션 에러
    */

    std::vector<GCITEMID> vecInfo;
    vecInfo = vecList_;
    vecInfo.resize( 4 );

    DBJIF2( m_kODBC.Query( L"{ call dbo.ICGAItemCoposition_insert_20120919 ( %d, %d, %d, %d, %d, %d, %d ) }",
        dwUID_,
        dwVersion_,
        dwItemID_,
        vecInfo[0],
        vecInfo[1],
        vecInfo[2],
        vecInfo[3] ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"아이템 합성 이벤트 정보 통계 DB기록.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::InsertSkillMissionInfo( IN const std::map<DWORD, std::pair<int,DWORD>>& mapDBUpdateSkillMissionInfo_ )
{
    /*
    L"{ call dbo.SMGASkillMissionCheck_insert ( %d, %d, %d ) }"

    { call dbo.SMGASkillMissionCheck_insert ( @1, @2, @3 ) }
    @1: @iSkillID_      int
    @2: @iMissionID_    int
    @3: @iType_         int    0:GP, 1:케쉬
        
    1 return ( @1 )
    @1: OK  int
        0       : 성공/데이터 존재
        -1      : 데이터 존재
        -101이하: 트랜젝션 에러
    */

    std::map<DWORD, std::pair<int,DWORD>>::const_iterator cmit;
    for( cmit = mapDBUpdateSkillMissionInfo_.begin(); cmit != mapDBUpdateSkillMissionInfo_.end(); ++cmit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.SMGASkillMissionCheck_insert ( %d, %d, %d ) }",
            cmit->second.first,
            cmit->first,
            cmit->second.second ) );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        _LOG_SUCCESS( nRet == 0, L"스킬 미션 정보 DB기록.. Result : " << nRet )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    return true;
}

bool KGSDBThread::UpdateGachaLotteryLog( IN const DWORD& dwUID_, IN const DWORD& dwVersion_, IN const char& cCharType_, IN const GCITEMID& dwRewardItemID_,  IN const DWORD& dwRewardType_, IN const int& nItemRemainCount_ )
{
    /*
    L"{ call dbo.GLGAGachaLottery_insert_20120828 ( %d, %d, %d, %d, %d, %d ) }"

    { call dbo.GLGAGachaLottery_insert_20120828 ( @1, @2, @3, @4, @5, @6 ) }
    @1: @iLoginUID_input         int
    @2: @iVersion_input          int
    @3: @iCharType_input         int
    @4: @iItemID_input           int
    @5: @iRewardType_input       int
    @6: @iRemainLotteryCNT_input int

    1 return ( @1 )
    @1: OK
    0       : 성공
    -101이하: 트랜젝션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.GLGAGachaLottery_insert_20120828 ( %d, %d, %d, %d, %d, %d ) }",
        dwUID_,
        dwVersion_,
        static_cast<int>(cCharType_),
        dwRewardItemID_,
        dwRewardType_,
        nItemRemainCount_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"가챠 복권 로그." << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return true;
}

void KGSDBThread::RegistGameMode( IN KODBC* pkODBC_ )
{
    if ( pkODBC_ == NULL ) {
        return;
    }

    std::wstringstream stm;
    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    USHORT usMaxString = 3500; // SP Parameter 데이터 타입이 varchar( 4000 ) 이므로 제한 필요.

    for( int nMode = 0 ; nMode < NUM_GC_GAME_MODE ; ++nMode ) {
        int iModeDiff = nMode;
        iModeDiff = iModeDiff << 16;

        if( stm.str().size() == 0 ) {
            stm << L"{ call dbo.up_set_desc_mode_count ( '";
        }

        if( KRoom::IsDifficultyMode( nMode ) ) {
            int nMaxDifficulty = KRoom::GetMaxDifficulty( nMode );
            for ( int nDifficult = 0 ; nDifficult < nMaxDifficulty ; ++nDifficult ) {
                stm << (iModeDiff + nDifficult) << L"," << KRoom::ms_szModeName[nMode] << L"(" << (nDifficult + 1) << L")" << L"," ;
            }

            int nChampionDifficulty = GC_GMD_CHAMPION_BEGIN;
            if ( SiKResultManager()->IsExistDifficulty( nMode, nChampionDifficulty ) ) {
                stm << (iModeDiff + GC_GMD_CHAMPION_BEGIN) << L"," << KRoom::ms_szModeName[nMode] << L"(" << (GC_GMD_CHAMPION_BEGIN + 1) << L")" << L"," ;
            }

            int nBreakDifficulty = GC_GMD_BREAK_BEGIN;
            if (SiKResultManager()->IsExistDifficulty(nMode, nBreakDifficulty)) {
                stm << (iModeDiff + GC_GMD_CHAMPION_BEGIN) << L"," << KRoom::ms_szModeName[nMode] << L"(" << (GC_GMD_BREAK_BEGIN + 1) << L")" << L",";
            }
        }
        else {
            stm << iModeDiff << L"," << KRoom::ms_szModeName[nMode] << L",";
        }

        if( stm.str().size() > usMaxString ) {
            stm << L"' ) }";
            KLocker lock( pkODBC_->GetCS() );
            DBJIF( m_kODBC.QueryToWString( stm.str() ) );

            int nOK = -1;
            if( pkODBC_->BeginFetch() ) {
                VERIFY_QUERY( (*pkODBC_) >> nOK );
            }
            pkODBC_->EndFetch();

            _LOG_SUCCESS( nOK == 0, L"게임 모드 이름 등록 완료. OK : " << nOK )
                << BUILD_LOG( pkODBC_->m_strLastQuery ) << END_LOG;

            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        }
    }

    if( stm.str().size() != 0 ) {
        stm << L"' ) }";
        KLocker lock( pkODBC_->GetCS() );

        DBJIF( m_kODBC.QueryToWString( stm.str() ) );

        int nOK = -1;
        if( pkODBC_->BeginFetch() ) {
            VERIFY_QUERY( (*pkODBC_) >> nOK );
        }
        pkODBC_->EndFetch();

        _LOG_SUCCESS( nOK == 0, L"게임 모드 이름 등록 완료. OK : " << nOK )
            << BUILD_LOG( pkODBC_->m_strLastQuery ) << END_LOG;

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }
}

bool KGSDBThread::InsertMonsterTypeStatInfo( IN const KMonsterTypeStat& kMonsterTypeStat_ )
{
    /*
    L"{ call dbo.DMGADungeonMonsterState_insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.DMGADungeonMonsterState_insert ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 ) }
    @1 : @iModeID_input          int    --던전의 ID 
    @2 : @iLoginUID_input        int    --방장 UID
    @3 : @iDifficulty_input      int    --난이도( 0 ~ 255 ) 
    @4 : @iParty_input           int    --참여 파티원 수 ( 1 ~ 6 )
    @5 : @iWinLose_input         int    --모드 클리어 여부 ( 실패 - 0, 성공 - 1 )
    @6 : @iMonID_input           int    --몬스터 ID
    @7 : @iMonStateNum_input     int    --몬스터 상태값(각 몬스터에 정의된 상태값의 번호)
    @8 : @iMonStateCount_input   int    --몬스터 상태가 실행된 횟 수
    @9 : @iCharKillCount_input   int    --던전 한번 플레이 하는 동안 파티원 캐릭터들이 죽은 수의 총 합 
    @10: @iPlayTime_input        int    --던전 플레이 시간

    1 return ( @1 )
    @1: OK
    0       : 성공
    -101이하: 트랜젝션 에러
    */
    std::map< std::pair<int,int>, int >::const_iterator mit;
    for( mit = kMonsterTypeStat_.m_mapMonsterTypeCount.begin(); mit != kMonsterTypeStat_.m_mapMonsterTypeCount.end(); ++mit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.DMGADungeonMonsterState_insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
            kMonsterTypeStat_.m_nModeID, 
            kMonsterTypeStat_.m_dwUserUID, 
            kMonsterTypeStat_.m_nDifficult, 
            kMonsterTypeStat_.m_nTotalPartyNum, 
            ( kMonsterTypeStat_.m_bClear ? 1 : 0 ),
            mit->first.first,
            mit->first.second,
            mit->second,
            kMonsterTypeStat_.m_nTotalDeathCount,
            kMonsterTypeStat_.m_dwPlayTime ) );

        int nOK = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        _LOG_SUCCESS( nOK == 0, L"몬스터 타입 사용 카운트 정보 DB기록.. Result : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    return true;
}

void KGSDBThread::GetPVPELOInfo( IN const DWORD& dwUserUID_, IN const int& nSeasonYear_, IN const int& nSeasonVersion_, IN const char& cCharType_, IN OUT KCharacterInfo& kCharacterInfo_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPoint_select_20121010( %d, %d, %d, %d ) }"

    { call dbo.PPGAPvPRatingPoint_select_20121010( @1, @2, @3, @4 ) }
    @1: @iLoginUID_input int
    @2: @iYear_input     int
    @3: @iSeason_input   int
    @4: @iCharType_input int

    1 return ( @1, @2, @3, @4, @5, @6, @7 )
    @1: RatingPoint   int
    @2: Win           int
    @3: Lose          int
    @4: MachingCNT    int
    @5: LastWin1Lose0 int
    @6: ConstantK     int
    @7: Grade         int
    */

    KELOUserData kELOUserData;
    kELOUserData.m_nRatingPoint = -1; // 해당 초기값을 통해서 DB에 값이 있는지 없는지 체크.

    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPoint_select_20121010( %d, %d, %d, %d ) }",
        dwUserUID_, nSeasonYear_, nSeasonVersion_, (int)cCharType_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kELOUserData.m_nRatingPoint 
                              >> kELOUserData.m_nELOWin
                              >> kELOUserData.m_nELOLose
                              >> kELOUserData.m_nMatchTotalCount
                              >> kELOUserData.m_nLastWinLose
                              >> kELOUserData.m_nConstantK
                              >> kELOUserData.m_ucGrade );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"캐릭터 ELO 정보 가져오기 UserUID: " << dwUserUID_ )
        << BUILD_LOG( nSeasonYear_ )
        << BUILD_LOG( nSeasonVersion_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    if( kELOUserData.m_nRatingPoint == -1 ) { // 누적치 값이 없으면 초기값 세팅.
        SiKPVPMatchManager()->GetPlacementTestRP( kCharacterInfo_.m_dwLevel, kELOUserData.m_nRatingPoint );
    }
    kELOUserData.m_nInitRatingPoint = kELOUserData.m_nRatingPoint;
    kELOUserData.m_nInitELOWin = kELOUserData.m_nELOWin;
    kELOUserData.m_nInitELOLose = kELOUserData.m_nELOLose;
    kELOUserData.m_nInitMatchTotalCount = kELOUserData.m_nMatchTotalCount;
    SiKPVPMatchManager()->CalcELOType( kELOUserData.m_nMatchTotalCount, kELOUserData.m_nELOType );
    kELOUserData.m_nPlacementTestPlayCount = SiKPVPMatchManager()->GetPlacementTestPlayCount();
    kCharacterInfo_.m_kELOUserData = kELOUserData;
}

bool KGSDBThread::UpdatePVPELOInfo( IN const DWORD& dwUID_, IN const int& nSeasonYear_, IN const int& nSeasonVersion_, IN const char& cCharType_, IN const int& nRatingPointDiff_, IN const int& nWinDiff_, IN const int& nLoseDiff_, IN const int& nMatchingCntDiff_, IN const int& nLastWinLose_, IN const int& nConstantK_, IN const UCHAR& ucGrade_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPoint_merge_20121010 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.PPGAPvPRatingPoint_merge_20121010 ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11 ) }
    @1 : @iLoginUID_input         int
    @2 : @iYear_input             int
    @3 : @iSeason_input           int
    @4 : @iCharType_input         int
    @5 : @iRatingPoint_diff_input int
    @6 : @iWin_diff_input         int
    @7 : @iLose_diff_input        int
    @8 : @iMachingCNT_diff_input  int
    @9 : @iLastWin1Lose0_input    int
    @10: @iConstantK_input        int
    @11: @iGrade_input            int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 업음
    -101이하: 트랜잭션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPoint_merge_20121010 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
        dwUID_,
        nSeasonYear_,
        nSeasonVersion_,
        (int)cCharType_,
        nRatingPointDiff_,
        nWinDiff_,
        nLoseDiff_,
        nMatchingCntDiff_,
        nLastWinLose_,
        nConstantK_,
        ucGrade_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"캐릭터 대전 매칭 플레이 DB기록.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

void KGSDBThread::Character_ELOData_select( IN const DWORD& dwUserUID_, IN const int& nSeasonYear_, IN const int& nSeasonVersion_, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo_ )
{
    std::map<char, KCharacterInfo>::iterator mit;
    for( mit = mapCharacterInfo_.begin(); mit != mapCharacterInfo_.end(); ++mit ) {
        GetPVPELOInfo( dwUserUID_, nSeasonYear_, nSeasonVersion_, mit->second.m_cCharType, mit->second );
    }
}

bool KGSDBThread::InsertDelMatchInfo( IN const KDB_EVENT_DEL_MATCH_REQ& kDBPacket_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointFail_insert_20121115 ( %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.PPGAPvPRatingPointFail_insert_20121115 ( @1, @2, @3, @4, @5, @6, @7, @8 ) }
    @1: @iLoginUID_input      int
    @2: @iYear_input          int
    @3: @iSeason_input        int
    @4: @iModeID_input        int
    @5: @iWaitingSecond_input int
    @6: @iResult_input        int
    @7: @iCharType_input      int
    @8: @iCurrentRP_input     int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 업음
    -101이하: 트랜잭션 에러
    */

    // 매칭 취소에 대한 통계.
    DBJIF2( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointFail_insert_20121115 ( %d, %d, %d, %d, %d, %d, %d, %d ) }",
        kDBPacket_.m_dwUserUID,
        kDBPacket_.m_nSeasonYear,
        kDBPacket_.m_nSeasonVersion,
        kDBPacket_.m_nModeInfo,
        kDBPacket_.m_dwDurationTime,
        kDBPacket_.m_nDelType,
        (int)kDBPacket_.m_cCharType,
        kDBPacket_.m_nRP ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"매칭 취소한 유저의 정보 DB기록.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::InsertPVPELOStatInfo( IN const KDB_EVENT_MATCH_ELO_RESULT_REQ& kDBPacket_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointSuccess_insert_20121115 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.PPGAPvPRatingPointSuccess_insert_20121115 ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10, @11, @12, @13, @14 ) }
    @1 : @iLoginUID_input         int
    @2 : @iYear_input             int
    @3 : @iSeason_input           int
    @4 : @iModeID_input           int
    @5 : @iWaitingSecond_input    int
    @6 : @iResult_input           int
    @7 : @iCharType_input         int
    @8 : @iCurrentRP_input        int
    @9 : @iReceiveRP_input        int
    @10: @iLevel_input            int
    @11: @iMyTeamTotalRP_input    int
    @12: @iOtherTeamTotalRP_input int
    @13: @iLoopCNT_input          int
    @14: @iPartyCNT_input         int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보가 업음
    -101이하: 트랜잭션 에러
    */

    // 정상적인 대전 매칭에 대한 통계.
    DBJIF2( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointSuccess_insert_20121115 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
        kDBPacket_.m_dwUserUID,
        kDBPacket_.m_nSeasonYear,
        kDBPacket_.m_nSeasonVersion,
        kDBPacket_.m_nModeInfo,
        kDBPacket_.m_dwMatchWaitTime,
        kDBPacket_.m_nWinLose,
        kDBPacket_.m_cCharType,
        kDBPacket_.m_nTotalRP,
        kDBPacket_.m_nReceiveRP,
        kDBPacket_.m_dwLevel,
        kDBPacket_.m_nMyTeamRP,
        kDBPacket_.m_nOtherTeamRP,
        kDBPacket_.m_nLoopCount, 
        kDBPacket_.m_nTotalPartyNum ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"대전매칭 정상적인 플레이한 캐릭터 정보 DB기록.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::Get_RitasChristmas_StartCoin_FromDB( IN int nVersion_, IN DWORD dwUID_, OUT int& nStartCoinCount_, OUT time_t& tmStartCoinReceived_ )
{
    /*
    크리스마스이벤트코인 검색
    L"{ call dbo.CEEVChristmasEventStartCoin_select ( %d, %d ) }"

    { call dbo.CEEVChristmasEventStartCoin_select ( @1, @2 ) }
    @1: @iVersion_      int
    @2: @iLoginUID_     int

    1 return ( @1, @2 )
    @1: RegdateD    smalldatetime
    @2: Coin        int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventStartCoin_select ( %d, %d ) }", nVersion_, dwUID_ ) );

    int nStartCoinCount = 0;
    time_t tmStartCoinReceived = 0;

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC
            >> tmStartCoinReceived
            >> nStartCoinCount );
    }

    tmStartCoinReceived_ = tmStartCoinReceived;
    nStartCoinCount_ = nStartCoinCount;

    START_LOG( clog, L"시작 코인 정보 가져오기: " )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( tmStartCoinReceived_ )
        << BUILD_LOG( nStartCoinCount_ )
        << END_LOG;

    return true;
}

bool KGSDBThread::Get_RitasChristmas_ContinueCoin_FromDB( IN int nVersion_, IN DWORD dwUID_, OUT int& nContinueCoinCount_ )
{
    /*
    크리스마스이벤트 재시작 코인 검색
    L"{ call dbo.CEEVChristmasEventRetryCoin_select ( %d, %d ) }"

    { call dbo.CEEVChristmasEventRetryCoin_select ( @1, @2 ) }
    @1: @iVersion_      int
    @2: @iLoginUID_     int

    1 return ( @1 )
    @1: RetryCoin       int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventRetryCoin_select ( %d, %d ) }", nVersion_, dwUID_ ) );

    int nContinueCoinCount = 0;

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC
            >> nContinueCoinCount );
    }

    nContinueCoinCount_ = nContinueCoinCount;

    START_LOG( clog, L"재도전 코인 정보 가져오기: " )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nContinueCoinCount_ )
        << END_LOG;

    return true;
}

bool KGSDBThread::Get_RitasChristmas_PremiumRewardCount_FromDB( IN int nVersion_, IN DWORD dwUID_, OUT int& nPremiumRewardCount_ )
{
    /*
    크리스마스이벤트 프리미엄 박스 검색
    L"{ call dbo.CEEVChristmasEventBox_select ( %d, %d ) }"

    { call dbo.CEEVChristmasEventBox_select ( @1, @2 ) }
    @1: @iVersion_      int
    @2: @iLoginUID_     int

    1 return ( @1 )
    @1: Box       int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventBox_select ( %d, %d ) }", nVersion_, dwUID_ ) );

    int nPremiumRewardCount = 0;

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC
            >> nPremiumRewardCount );
    }

    nPremiumRewardCount_ = nPremiumRewardCount;

    START_LOG( clog, L"프리미엄 상자 정보 가져오기: " )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nPremiumRewardCount_ )
        << END_LOG;

    return true;
}

bool KGSDBThread::Get_RitasChristmas_TodayMaxPoint_FromDB( IN int nVersion_, IN DWORD dwUID_, OUT int& nTodayMaxPoint_ )
{
    /*
    크리스마스이벤트 점수 검색
    L"{ call dbo.CEEVChristmasEventScore_select ( %d, %d ) }"

    { call dbo.CEEVChristmasEventScore_select ( @1, @2 ) }
    @1: @iVersion_      int
    @2: @iLoginUID_     int


    1 return ( @1 )
    @1: Score       int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventScore_select ( %d, %d ) }", nVersion_, dwUID_ ) );

    int nTodayMaxPoint = 0;

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC
            >> nTodayMaxPoint );
    }

    nTodayMaxPoint_ = nTodayMaxPoint;

    START_LOG( clog, L"오늘 최고 점수 가져오기: " )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nTodayMaxPoint_ )
        << END_LOG;

    return true;
}

bool KGSDBThread::Update_RitasChristmas_NewRecord_ToDB( IN int nVersion_, IN DWORD dwUID_, IN const int nPointDiff_ )
{
    /*
    크리스마스이벤트 점수 갱신
    L"{ call dbo.CEEVChristmasEventScore_update ( %d, %d, %d ) }"

    { call dbo.CEEVChristmasEventScore_update ( @1, @2, @3 ) }
    @1: @iVersion_          int
    @2: @iLoginUID_         int
    @3: @iScore_diff_       int

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -901이하: 트랜젝션에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventScore_update ( %d, %d, %d ) }",
        nVersion_,
        dwUID_,
        nPointDiff_ ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"오늘 최고 점수 DB기록.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::Get_RitasChristmas_RankInfo_FromDB( IN DWORD dwUID_, OUT std::vector< KRitasChristmasRankInfo >& vecRankInfo_ )
{
    /*
    크리스마스이벤트 랭킹 검색
    L"{ call dbo.CEEVChristmasEventRank_select ( %d ) }"

    { call dbo.CEEVChristmasEventRank_select ( @1 ) }
    @1: @iLoginUID_     int


    n return ( @1, @2, @3, @4 )
    @1: Loginuid        int
    @2: RLoginuid       int
    @3: RNickName       nvarchar(48)
    @4: Score           int
    */

    vecRankInfo_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventRank_select ( %d ) }", dwUID_ ) );

    DWORD dwLoginUID = 0;
    DWORD dwRLoginUID = 0;
    std::wstring wsNickName;
    int nMaxPoint = 0;

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC
            >> dwLoginUID
            >> dwRLoginUID
            >> wsNickName
            >> nMaxPoint );

        KRitasChristmasRankInfo kRitasChristmasRankInfo;
        kRitasChristmasRankInfo.m_wsNickName = wsNickName;
        kRitasChristmasRankInfo.m_nMaxPoint = nMaxPoint;

        vecRankInfo_.push_back( kRitasChristmasRankInfo );
    }

    // DB에서 10위까지의 데이터를 넘겨주지만 정렬은 되어있지 않다. 그래서 내림차순 정렬함
    std::sort( vecRankInfo_.begin(), vecRankInfo_.end(),
        boost::bind( &KRitasChristmasRankInfo::m_nMaxPoint, _1 ) > boost::bind( &KRitasChristmasRankInfo::m_nMaxPoint, _2 ) );

    START_LOG( clog, L"어제 랭킹 가져오기: " )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( vecRankInfo_.size() )
        << END_LOG;

    return true;
}

bool KGSDBThread::Update_RitasChristmas_UserData_ToDB( IN DWORD dwLoginUID_, IN KRitasChristmasEventUserData& kRitasChristmasUserData_ )
{
    if ( false == SiKRitasChristmas()->IsEventTerm() ) {
        return false;
    }

    int nVersion = SiKRitasChristmas()->GetVersion();

    // 유저 데이터 초기화 조심해서 처리
    // 어떻게 조심하지? 0같은 초기값인지 확인해야하나

    // 시작 코인 수, 시작 코인을 마지막으로 받은 시간
    int nStartCoinDiff = kRitasChristmasUserData_.m_nStartCoinCount - kRitasChristmasUserData_.m_nInitialStartCoinCount;
    Update_RitasChristmas_StartCoin_ToDB( nVersion, dwLoginUID_, nStartCoinDiff, kRitasChristmasUserData_.m_tmStartCoinReceived );

    // 재도전 코인 수는 충전할 때 기록하고 소모할 때 기록한다. 여기서 안함
    //Update_RitasChristmas_ContinueCoin_ToDB( nVersion, dwLoginUID_, nDiff_ );

    // 프리미엄 상자 수
    int nPremiumRewardDiff = kRitasChristmasUserData_.m_nPremiumRewardCount - kRitasChristmasUserData_.m_nInitialPremiumRewardCount;
    Update_RitasChristmas_PremiumRewardCount_ToDB( nVersion, dwLoginUID_, nPremiumRewardDiff );

    // 오늘 내 최고 점수는 최고 점수를 얻었을 때마다 기록하도록 구현되어 있다. 여기서 안함
    //Update_RitasChristmas_TodayMaxPoint_ToDB( nVersion, dwLoginUID_, nDiff_ );

    return true;
}

bool KGSDBThread::Update_RitasChristmas_StartCoin_ToDB( IN int nVersion_, IN DWORD dwUID_, IN int nStartCoinDiff_, IN time_t tmStartCoinReceived_ )
{
    /*
    크리스마스이벤트코인 갱신
    L"{ call dbo.CEEVChristmasEventStartCoin_update ( %d, %d, N'%s', %d ) }"

    { call dbo.CEEVChristmasEventStartCoin_update ( @1, @2, @3, @4 ) }
    @1: @iVersion_          int
    @2: @iLoginUID_         int
    @3: @sdtReceiveDateB_   smalldatetime   --마지막으로코인받은시간
    @4: @iCoin_diff_        int
        
    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -901이하: 트랜젝션에러
    */

    CTime tmTime( tmStartCoinReceived_ );

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventStartCoin_update ( %d, %d, N'%s', %d ) }",
        nVersion_,
        dwUID_,
        (LPCTSTR)tmTime.Format( L"%Y%m%d %H:%M:%S" ),
        nStartCoinDiff_) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"시작 코인 DB기록.. Result : " << nRet )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nStartCoinDiff_ )
        << BUILD_LOGtm( CTime( tmStartCoinReceived_ ) )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::Update_RitasChristmas_PremiumRewardCount_ToDB( IN int nVersion_, IN DWORD dwUID_, IN int nPremiumRewardDiff_ )
{
    /*
    크리스마스 이벤트 프리미엄 박스 갱신
    L"{ call dbo.CEEVChristmasEventBox_update ( %d, %d, %d ) }"

    { call dbo.CEEVChristmasEventBox_update ( @1, @2, @3 ) }
    @1: @iVersion_          int
    @2: @iLoginUID_         int
    @3: @iBox_diff_         int
        
    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -901이하: 트랜젝션에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventBox_update ( %d, %d, %d ) }",
        nVersion_,
        dwUID_,
        nPremiumRewardDiff_) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"프리미엄 상자 DB기록.. Result : " << nRet )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nPremiumRewardDiff_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

bool KGSDBThread::Update_RitasChristmas_ContinueCoin_ToDB( IN int nVersion_, IN DWORD dwUID_, IN int nContinueCoinCount_ )
{
    /*
    크리스마스 이벤트 재도전 코인 갱신
    L"{ call dbo.CEEVChristmasEventRetryCoin_update ( %d, %d, %d ) }"

    { call dbo.CEEVChristmasEventRetryCoin_update ( @1, @2, @3 ) }
    @1: @iVersion_          int
    @2: @iLoginUID_         int
    @3: @iRetryCoin_diff_   int
        
    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -901이하: 트랜젝션에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEEVChristmasEventRetryCoin_update ( %d, %d, %d ) }",
        nVersion_,
        dwUID_,
        nContinueCoinCount_) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"재도전 코인 DB기록.. Result : " << nRet )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nContinueCoinCount_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return true;
}

void KGSDBThread::DeleteRemoveStrengthItem( IN const std::wstring& strLogin_, IN const char cCharType_, IN const DWORD& dwUID_, IN std::vector<GCITEMUID>& vecRemoveItem_ )
{
    std::vector<GCITEMUID>::iterator vit;
    for( vit = vecRemoveItem_.begin(); vit != vecRemoveItem_.end(); ++vit ) {
        LIF( SetTypeEraseItem( strLogin_, dwUID_, *vit, KItemManager::EDEL_DEL_STRENGTH_CONNECTION, cCharType_ ) );
    }
}

void KGSDBThread::GetRPMatchRank()
{
    std::map<char, std::vector<KMatchRank>> mapMatchRank;
    mapMatchRank.clear();
    /*
    L"{ call dbo.PPGAPvPRatingPointRankMachingTop_select_20121218 }"

    { call dbo.PPGAPvPRatingPointRankMachingTop_select_20121218 }

    n return ( @1, @2, @3, @4, @5, @6, @7, @8, @9 )
    @1: TabType   int
    -1   : 전체 랭킹
    0부터: 캐릭터별 랭킹
    @2: Rank      int
    @3: GuildID   int
    @4: GuildMark int
    @5: CharType  int
    @6: Nickname  nvarchar( 24 )
    @7: Win       int
    @8: Lose      int
    @9: WinRate   float xx.yyy
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankMachingTop_select_20121218 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KMatchRank kRankData;
        kRankData.m_cTabType = -2;
        VERIFY_QUERY( m_kODBC >> kRankData.m_cTabType
            >> kRankData.m_nRank
            >> kRankData.m_dwGuildUID
            >> kRankData.m_nGuildMark
            >> kRankData.m_cCharType
            >> kRankData.m_strNick
            >> kRankData.m_dwELOWin
            >> kRankData.m_dwELOLose
            >> kRankData.m_fWinRate );
        kRankData.m_cRankType = KMatchRank::MRT_RP;
        SiKGuildManager()->GetGuildMarkStringName( kRankData.m_dwGuildUID, kRankData.m_strMarkName );

        if( kRankData.m_cTabType > -2 ) {
            mapMatchRank[kRankData.m_cTabType].push_back( kRankData );
        }
    }
    std::map<char, std::vector<KMatchRank> >::iterator mit;
    for( mit = mapMatchRank.begin() ; mit != mapMatchRank.end() ; ++mit ) {
        std::sort( mit->second.begin(), mit->second.end(),
            boost::bind(&KMatchRank::m_nRank, _1) < boost::bind(&KMatchRank::m_nRank, _2) );
    }

    SiKRankManager()->UpdateMatchRankData( KMatchRank::MRT_RP, mapMatchRank );
}

void KGSDBThread::GetWinRecordMatchRank()
{
    std::map<char, std::vector<KMatchRank>> mapMatchRank;
    mapMatchRank.clear();
    /*
    L"{ call dbo.PPGAPvPRatingPointRankWinTop_select_20121218 }"

    { call dbo.PPGAPvPRatingPointRankWinTop_select_20121218 }

    n return ( @1, @2, @3, @4, @5, @6, @7, @8, @9 )
    @1: TabType   int
    -1   : 전체 랭킹
    0부터: 캐릭터별 랭킹
    @2: Rank      int
    @3: GuildID   int
    @4: GuildMark int
    @5: CharType  int
    @6: Nickname  nvarchar( 24 )
    @7: Win       int
    @8: Lose      int
    @9: WinRate   float xx.yyy
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankWinTop_select_20121218 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KMatchRank kRankData;
        kRankData.m_cTabType = -2;
        VERIFY_QUERY( m_kODBC >> kRankData.m_cTabType
            >> kRankData.m_nRank
            >> kRankData.m_dwGuildUID
            >> kRankData.m_nGuildMark
            >> kRankData.m_cCharType
            >> kRankData.m_strNick
            >> kRankData.m_dwELOWin
            >> kRankData.m_dwELOLose
            >> kRankData.m_fWinRate );
        kRankData.m_cRankType = KMatchRank::MRT_WINRECORD;
        SiKGuildManager()->GetGuildMarkStringName( kRankData.m_dwGuildUID, kRankData.m_strMarkName );

        if( kRankData.m_cTabType > -2 ) {
            mapMatchRank[kRankData.m_cTabType].push_back( kRankData );
        }
    }
    std::map<char, std::vector<KMatchRank> >::iterator mit;
    for( mit = mapMatchRank.begin() ; mit != mapMatchRank.end() ; ++mit ) {
        std::sort( mit->second.begin(), mit->second.end(),
            boost::bind(&KMatchRank::m_nRank, _1) < boost::bind(&KMatchRank::m_nRank, _2) );
    }

    SiKRankManager()->UpdateMatchRankData( KMatchRank::MRT_WINRECORD, mapMatchRank );
}


void KGSDBThread::GetRPMatchRankHOF()
{
    std::map<char, std::vector<KMatchRank>> mapMatchRank;
    mapMatchRank.clear();
    /*
    L"{ call dbo.PPGAPvPRatingPointRankMachingTopHOF_select_20121218 }"

    { call dbo.PPGAPvPRatingPointRankMachingTopHOF_select_20121218 }

    n return ( @1, @2, @3, @4, @5, @6, @7, @8, @9 )
    @1: TabType   int
    -1   : 전체 랭킹
    0부터: 캐릭터별 랭킹
    @2: Rank      int
    @3: GuildID   int
    @4: GuildMark int
    @5: CharType  int
    @6: Nickname  nvarchar( 24 )
    @7: Win       int
    @8: Lose      int
    @9: WinRate   float xx.yyy
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankMachingTopHOF_select_20121218 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KMatchRank kRankData;
        kRankData.m_cTabType = -2;
        VERIFY_QUERY( m_kODBC >> kRankData.m_cTabType
            >> kRankData.m_nRank
            >> kRankData.m_dwGuildUID
            >> kRankData.m_nGuildMark
            >> kRankData.m_cCharType
            >> kRankData.m_strNick
            >> kRankData.m_dwELOWin
            >> kRankData.m_dwELOLose
            >> kRankData.m_fWinRate );
        kRankData.m_cRankType = KMatchRank::MRT_BEFORE_RP;
        SiKGuildManager()->GetGuildMarkStringName( kRankData.m_dwGuildUID, kRankData.m_strMarkName );

        if( kRankData.m_cTabType > -2 ) {
            mapMatchRank[kRankData.m_cTabType].push_back( kRankData );
        }
    }
    std::map<char, std::vector<KMatchRank> >::iterator mit;
    for( mit = mapMatchRank.begin() ; mit != mapMatchRank.end() ; ++mit ) {
        std::sort( mit->second.begin(), mit->second.end(),
            boost::bind(&KMatchRank::m_nRank, _1) < boost::bind(&KMatchRank::m_nRank, _2) );
    }

    SiKRankManager()->UpdateMatchRankData( KMatchRank::MRT_BEFORE_RP, mapMatchRank );
}

void KGSDBThread::GetWinRecordMatchRankHOF()
{
    std::map<char, std::vector<KMatchRank>> mapMatchRank;
    mapMatchRank.clear();
    /*
    L"{ call dbo.PPGAPvPRatingPointRankWinTopHOF_select_20121218 }"

    { call dbo.PPGAPvPRatingPointRankWinTopHOF_select_20121218 }

    n return ( @1, @2, @3, @4, @5, @6, @7, @8, @9 )
    @1: TabType   int
    -1   : 전체 랭킹
    0부터: 캐릭터별 랭킹
    @2: Rank      int
    @3: GuildID   int
    @4: GuildMark int
    @5: CharType  int
    @6: Nickname  nvarchar( 24 )
    @7: Win       int
    @8: Lose      int
    @9: WinRate   float xx.yyy
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankWinTopHOF_select_20121218 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KMatchRank kRankData;
        kRankData.m_cTabType = -2;
        VERIFY_QUERY( m_kODBC >> kRankData.m_cTabType
            >> kRankData.m_nRank
            >> kRankData.m_dwGuildUID
            >> kRankData.m_nGuildMark
            >> kRankData.m_cCharType
            >> kRankData.m_strNick
            >> kRankData.m_dwELOWin
            >> kRankData.m_dwELOLose
            >> kRankData.m_fWinRate );
        kRankData.m_cRankType = KMatchRank::MRT_BEFORE_WINRECORD;
        SiKGuildManager()->GetGuildMarkStringName( kRankData.m_dwGuildUID, kRankData.m_strMarkName );

        if( kRankData.m_cTabType > -2 ) {
            mapMatchRank[kRankData.m_cTabType].push_back( kRankData );
        }
    }
    std::map<char, std::vector<KMatchRank> >::iterator mit;
    for( mit = mapMatchRank.begin() ; mit != mapMatchRank.end() ; ++mit ) {
        std::sort( mit->second.begin(), mit->second.end(),
            boost::bind(&KMatchRank::m_nRank, _1) < boost::bind(&KMatchRank::m_nRank, _2) );
    }

    SiKRankManager()->UpdateMatchRankData( KMatchRank::MRT_BEFORE_WINRECORD, mapMatchRank );
}

void KGSDBThread::GetExpMatchRank()
{
    std::map<char, std::vector<KExpRank>> mapMatchRank;
    mapMatchRank.clear();
    /*
    L"{ call dbo.PPGAPvPRatingPointRankExpTop_select_20121218 }"

    { call dbo.PPGAPvPRatingPointRankExpTop_select_20121218 }

    n return ( @1, @2, @3, @4, @5, @6, @7 )
    @1: TabType   int
    -1   : 전체 랭킹
    0부터: 캐릭터별 랭킹
    @2: Rank      int
    @3: GuildID   int
    @4: GuildMark int
    @5: CharType  int
    @6: Nickname  nvarchar( 24 )
    @7: Exp       bigint
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankExpTop_select_20121218 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KExpRank kRankData;
        char cTabType = -2;
        VERIFY_QUERY( m_kODBC >> cTabType
            >> kRankData.m_nRank
            >> kRankData.m_nGuildID
            >> kRankData.m_nGuildMark
            >> kRankData.m_cCharType
            >> kRankData.m_strNick
            >> kRankData.m_biExp );
        SiKGuildManager()->GetGuildMarkStringName( kRankData.m_nGuildID, kRankData.m_strMarkName );

        if( cTabType > -2 ) {
            mapMatchRank[cTabType].push_back( kRankData );
        }
    }
    std::map<char, std::vector<KExpRank> >::iterator mit;
    for( mit = mapMatchRank.begin() ; mit != mapMatchRank.end() ; ++mit ) {
        std::sort( mit->second.begin(), mit->second.end(),
            boost::bind(&KExpRank::m_nRank, _1) < boost::bind(&KExpRank::m_nRank, _2) );
    }

    SiKRankManager()->UpdateMatchExpRankData( mapMatchRank );
}

void KGSDBThread::GetMatchRankInfo( IN const DWORD& dwUserUID_, IN const int& nTabType_, OUT KMyMatchRankInfo& kMyRankInfo_ )
{
    KMatchRank kRPMatchRank;
    KMatchRank kWinRecordMatchRank;
    KExpRank kExpRank;
    GetUserRPMatchRank( dwUserUID_, nTabType_, kRPMatchRank );
    GetUserWinRecordMatchRank( dwUserUID_, nTabType_, kWinRecordMatchRank );
    GetUserExpMatchRank( dwUserUID_, nTabType_, kExpRank );

    if( kRPMatchRank.m_nRank != 0 ) {
        kMyRankInfo_.m_mapRPMatchRank[nTabType_] = kRPMatchRank;
    }
    if( kWinRecordMatchRank.m_nRank != 0 ) {
        kMyRankInfo_.m_mapWinRecordRank[nTabType_] = kWinRecordMatchRank;
    }
    if( kExpRank.m_nRank != 0 ) {
        kMyRankInfo_.m_mapExpRank[nTabType_] = kExpRank;
    }
}

void KGSDBThread::GetUserRPMatchRank( IN const DWORD& dwUserUID_, IN const int& nTabType_, OUT KMatchRank& kMyRPRankInfo_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointRankMachingLoginUID_select_20121218( %d, %d ) }"

    { call dbo.PPGAPvPRatingPointRankMachingLoginUID_select_20121218( @1, @2 ) }
    @1: @iLoginUID_input int
    @2: @iTabType_input  int

    1 return ( @1, @2, @3, @4, @5, @6, @7, @8 )
    @1: Rank      int
    @2: GuildID   int
    @3: GuildMark int
    @4: CharType  int
    @5: Nickname  nvarchar( 24 )
    @6: Win       int
    @7: Lose      int
    @8: WinRate   float xx.yyy
    */

    KMatchRank kMatchRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankMachingLoginUID_select_20121218 ( %d, %d ) }", dwUserUID_, nTabType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kMatchRank.m_nRank
                              >> kMatchRank.m_dwGuildUID
                              >> kMatchRank.m_nGuildMark
                              >> kMatchRank.m_cCharType 
                              >> kMatchRank.m_strNick 
                              >> kMatchRank.m_dwELOWin
                              >> kMatchRank.m_dwELOLose
                              >> kMatchRank.m_fWinRate );
    }
    m_kODBC.EndFetch();

    SiKGuildManager()->GetGuildMarkStringName( kMatchRank.m_dwGuildUID, kMatchRank.m_strMarkName );

    kMyRPRankInfo_ = kMatchRank;
}

void KGSDBThread::GetUserWinRecordMatchRank( IN const DWORD& dwUserUID_, IN const int& nTabType_, OUT KMatchRank& kMyWinRecordRankInfo_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointRankWinLoginUID_select_20121218( %d, %d ) }"

    { call dbo.PPGAPvPRatingPointRankWinLoginUID_select_20121218( @1, @2 ) }
    @1: @iLoginUID_input int
    @2: @iTabType_input  int

    1 return ( @1, @2, @3, @4, @5, @6, @7, @8 )
    @1: Rank      int
    @2: GuildID   int
    @3: GuildMark int
    @4: CharType  int
    @5: Nickname  nvarchar( 24 )
    @6: Win       int
    @7: Lose      int
    @8: WinRate   float xx.yyy
    */

    KMatchRank kMatchRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankWinLoginUID_select_20121218 ( %d, %d ) }", dwUserUID_, nTabType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kMatchRank.m_nRank
                              >> kMatchRank.m_dwGuildUID
                              >> kMatchRank.m_nGuildMark
                              >> kMatchRank.m_cCharType 
                              >> kMatchRank.m_strNick 
                              >> kMatchRank.m_dwELOWin
                              >> kMatchRank.m_dwELOLose
                              >> kMatchRank.m_fWinRate );
    }
    m_kODBC.EndFetch();

    SiKGuildManager()->GetGuildMarkStringName( kMatchRank.m_dwGuildUID, kMatchRank.m_strMarkName );

    kMyWinRecordRankInfo_ = kMatchRank;
}

void KGSDBThread::GetUserExpMatchRank( IN const DWORD& dwUserUID_, IN const int& nTabType_, OUT KExpRank& kMyExpRankInfo_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointRankExpLoginUID_select_20121218( %d, %d ) }"

    { call dbo.PPGAPvPRatingPointRankExpLoginUID_select_20121218( @1, @2 ) }
    @1: @iLoginUID_input int
    @2: @iTabType_input  int

    1 return ( @1, @2, @3, @4, @5, @6 )
    @1: Rank      int
    @2: GuildID   int
    @3: GuildMark int
    @4: CharType  int
    @5: Nickname  nvarchar( 24 )
    @6: Exp       bigint
    */

    KExpRank kExpRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankExpLoginUID_select_20121218 ( %d, %d ) }", dwUserUID_, nTabType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kExpRank.m_nRank
                              >> kExpRank.m_nGuildID
                              >> kExpRank.m_nGuildMark
                              >> kExpRank.m_cCharType 
                              >> kExpRank.m_strNick 
                              >> kExpRank.m_biExp );
    }
    m_kODBC.EndFetch();

    SiKGuildManager()->GetGuildMarkStringName( kExpRank.m_nGuildID, kExpRank.m_strMarkName );
    kMyExpRankInfo_ = kExpRank;
}

void KGSDBThread::FindRPRankUseNickName( IN const char& cCharType_, IN const std::wstring& strNickName_, OUT KMatchRank& kMatchRank_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointRankMachingNickname_select_20121218( N'%s', %d ) }"

    { call dbo.PPGAPvPRatingPointRankMachingNickname_select_20121218( @1, @2 ) }
    @1: @strNickname_input nvarchar( 24 )
    @2: @iTabType_input    int

    1 return ( @1, @2, @3, @4, @5, @6, @7, @8 )
    @1: Rank      int
    @2: GuildID   int
    @3: GuildMark int
    @4: CharType  int
    @5: Nickname  nvarchar( 24 )
    @6: Win       int
    @7: Lose      int
    @8: WinRate   float xx.yyy
    */

    KMatchRank kMatchRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankMachingNickname_select_20121218 ( N'%s', %d ) }", strNickName_.c_str(), cCharType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kMatchRank.m_nRank
            >> kMatchRank.m_dwGuildUID
            >> kMatchRank.m_nGuildMark
            >> kMatchRank.m_cCharType 
            >> kMatchRank.m_strNick 
            >> kMatchRank.m_dwELOWin
            >> kMatchRank.m_dwELOLose
            >> kMatchRank.m_fWinRate );
    }
    m_kODBC.EndFetch();

    SiKGuildManager()->GetGuildMarkStringName( kMatchRank.m_dwGuildUID, kMatchRank.m_strMarkName );

    kMatchRank_ = kMatchRank;
}

void KGSDBThread::FindWinRecordRankUseNickName( IN const char& cCharType_, IN const std::wstring& strNickName_, OUT KMatchRank& kMatchRank_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointRankWinNickname_select_20121218( N'%s', %d ) }"

    { call dbo.PPGAPvPRatingPointRankWinNickname_select_20121218( @1, @2 ) }
    @1: @strNickname_input nvarchar( 24 )
    @2: @iTabType_input    int

    1 return ( @1, @2, @3, @4, @5, @6, @7, @8 )
    @1: Rank      int
    @2: GuildID   int
    @3: GuildMark int
    @4: CharType  int
    @5: Nickname  nvarchar( 24 )
    @6: Win       int
    @7: Lose      int
    @8: WinRate   float xx.yyy
    */

    KMatchRank kMatchRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankWinNickname_select_20121218 ( N'%s', %d ) }", strNickName_.c_str(), cCharType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kMatchRank.m_nRank
            >> kMatchRank.m_dwGuildUID
            >> kMatchRank.m_nGuildMark
            >> kMatchRank.m_cCharType 
            >> kMatchRank.m_strNick 
            >> kMatchRank.m_dwELOWin
            >> kMatchRank.m_dwELOLose
            >> kMatchRank.m_fWinRate );
    }
    m_kODBC.EndFetch();

    SiKGuildManager()->GetGuildMarkStringName( kMatchRank.m_dwGuildUID, kMatchRank.m_strMarkName );

    kMatchRank_ = kMatchRank;
}

void KGSDBThread::FindExpRankUseNickName( IN const char& cCharType_, IN const std::wstring& strNickName_, OUT KExpRank& kExpRank_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointRankExpNickname_select_20121218( N'%s', %d ) }"

    { call dbo.PPGAPvPRatingPointRankExpNickname_select_20121218( @1, @2 ) }
    @1: @strNickname_input nvarchar( 24 )
    @2: @iTabType_input    int

    1 return ( @1, @2, @3, @4, @5, @6 )
    @1: Rank      int
    @2: GuildID   int
    @3: GuildMark int
    @4: CharType  int
    @5: Nickname  nvarchar( 24 )
    @6: Exp       bigint
    */

    KExpRank kExpRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankExpNickname_select_20121218 ( N'%s', %d ) }", strNickName_.c_str(), cCharType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kExpRank.m_nRank
            >> kExpRank.m_nGuildID
            >> kExpRank.m_nGuildMark
            >> kExpRank.m_cCharType 
            >> kExpRank.m_strNick 
            >> kExpRank.m_biExp );
    }
    m_kODBC.EndFetch();

    SiKGuildManager()->GetGuildMarkStringName( kExpRank.m_nGuildID, kExpRank.m_strMarkName );
    kExpRank_ = kExpRank;
}

void KGSDBThread::FindRPRankUseNickNameHOF( IN const char& cCharType_, IN const std::wstring& strNickName_, OUT KMatchRank& kMatchRank_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointRankMachingNicknameHOF_select_20121218( N'%s', %d ) }"

    { call dbo.PPGAPvPRatingPointRankMachingNicknameHOF_select_20121218( @1, @2 ) }
    @1: @strNickname_input nvarchar( 24 )
    @2: @iTabType_input    int

    1 return ( @1, @2, @3, @4, @5, @6, @7, @8 )
    @1: Rank      int
    @2: GuildID   int
    @3: GuildMark int
    @4: CharType  int
    @5: Nickname  nvarchar( 24 )
    @6: Win       int
    @7: Lose      int
    @8: WinRate   float xx.yyy
    */

    KMatchRank kMatchRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankMachingNicknameHOF_select_20121218 ( N'%s', %d ) }", strNickName_.c_str(), cCharType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kMatchRank.m_nRank
            >> kMatchRank.m_dwGuildUID
            >> kMatchRank.m_nGuildMark
            >> kMatchRank.m_cCharType 
            >> kMatchRank.m_strNick 
            >> kMatchRank.m_dwELOWin
            >> kMatchRank.m_dwELOLose
            >> kMatchRank.m_fWinRate );
    }
    m_kODBC.EndFetch();

    SiKGuildManager()->GetGuildMarkStringName( kMatchRank.m_dwGuildUID, kMatchRank.m_strMarkName );

    kMatchRank_ = kMatchRank;
}

void KGSDBThread::FindWinRecordRankUseNickNameHOF( IN const char& cCharType_, IN const std::wstring& strNickName_, OUT KMatchRank& kMatchRank_ )
{
    /*
    L"{ call dbo.PPGAPvPRatingPointRankWinNicknameHOF_select_20121218( N'%s', %d ) }"

    { call dbo.PPGAPvPRatingPointRankWinNicknameHOF_select_20121218( @1, @2 ) }
    @1: @strNickname_input nvarchar( 24 )
    @2: @iTabType_input    int

    1 return ( @1, @2, @3, @4, @5, @6, @7, @8 )
    @1: Rank      int
    @2: GuildID   int
    @3: GuildMark int
    @4: CharType  int
    @5: Nickname  nvarchar( 24 )
    @6: Win       int
    @7: Lose      int
    @8: WinRate   float xx.yyy
    */

    KMatchRank kMatchRank;
    DBJIF( m_kODBC.Query( L"{ call dbo.PPGAPvPRatingPointRankWinNicknameHOF_select_20121218 ( N'%s', %d ) }", strNickName_.c_str(), cCharType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kMatchRank.m_nRank
            >> kMatchRank.m_dwGuildUID
            >> kMatchRank.m_nGuildMark
            >> kMatchRank.m_cCharType 
            >> kMatchRank.m_strNick 
            >> kMatchRank.m_dwELOWin
            >> kMatchRank.m_dwELOLose
            >> kMatchRank.m_fWinRate );
    }
    m_kODBC.EndFetch();

    SiKGuildManager()->GetGuildMarkStringName( kMatchRank.m_dwGuildUID, kMatchRank.m_strMarkName );

    kMatchRank_ = kMatchRank;
}

bool KGSDBThread::Insert_GachaSystem_RewardLog_ToDB( IN const int nVersion_, IN const DWORD dwLoginUID_, IN const char cSelectedCharType_, IN const int nSelectedCharLevel_, IN const int nSelectedItemLevel_, IN const GCITEMID dwRewardItemID_, IN const int nRewardInfo_, IN const int nRewardItemCount_, IN const int nDecreaseCount_ )
{
    /*
    가차 아이템 로그

    { call dbo.GSGAGachaSystemItem_insert ( @1, @2, @3, @4, @5, @6, @7, @8, @9 ) }
    { call dbo.GSGAGachaSystemItem_insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d ) }
    @1 : @iVersion_      int
    @2 : @iLoginUID_     int
    @3 : @iCharType_     int  //가차 페이지에서 선택된 캐릭터
    @4 : @iItemLevel_    int  //가차 페이지에서 선택된 레벨
    @5 : @iCharLevel_    int  //캐릭터의 현재 레벨
    @6 : @iItemID_       int  //획득 아이템 ID
    @7 : @iRewardKind_   int  //꽝 0, 장비 1, 무기 2, 펫 3
    @8 : @iFactor_       int
    @9 : @iUsedCount_    int

    1 return ( @1 )
    @1: OK
        0       : 성공
        -101이하: 트랜젝션 에러
    */

    int nOK = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.GSGAGachaSystemItem_insert ( %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
        nVersion_,                                      // 획득 가차 버전
        dwLoginUID_,                                    // 획득한 유저 UID
        static_cast< int >( cSelectedCharType_ ),       // 현재 선택중인 캐릭터 타입
        nSelectedItemLevel_,                            // 현재 선택중인 아이템 레벨
        nSelectedCharLevel_,                            // 현재 선택중인 캐릭터 레벨
        dwRewardItemID_,                                // 획득한 보상 아이템 ID
        nRewardInfo_,                                   // 보상 타입. GachaManager의 GACHA_REWARD_INFO 정보
        nRewardItemCount_,                              // 획득한 보상 아이템 수량
        nDecreaseCount_                                 // 사용한 주문서 수량
        ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"가챠 보상 아이템 획득 로그 UserUID: " << dwLoginUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwLoginUID_ )
        << BUILD_LOGc( cSelectedCharType_ )
        << BUILD_LOG( nSelectedCharLevel_ )
        << BUILD_LOG( nSelectedItemLevel_ )
        << BUILD_LOG( dwRewardItemID_ )
        << BUILD_LOG( nRewardInfo_ )
        << BUILD_LOG( nRewardItemCount_ )
        << BUILD_LOG( nDecreaseCount_ )
        << END_LOG;

    return true;
}

void KGSDBThread::GetUserInfo_AccAttendance( IN const DWORD& dwUserUID_, IN const int& nVersion_, OUT int& nAccumulateCount_, OUT time_t& tmLastAttendanceTime_ )
{
    /*
    발렌타인이벤트( 화령의축제) 유저별출석정보조회

    L"{ call dbo.FSGAFlowerSpiritEventAttendance_Select ( %d, %d ) }"

    { call dbo.FSGAFlowerSpiritEventAttendance_Select ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iVersion_ int

    n return ( @1, @2 )
    @1 ; AccumulatedCount   int  // 현재 총 누적 출석 수
  @2 ; LastAttendanceDate smalldatetime // 마지막 출석 날짜 ( yyyy-mm-dd )
*/

    DBJIF( m_kODBC.Query( L"{ call dbo.FSGAFlowerSpiritEventAttendance_Select ( %d, %d ) }", dwUserUID_, nVersion_ ) );

    // db에 데이터가 존재하지 않으면 update함수에서 출석일자를 0, 마지막 출석 날짜를 어제로 기록한다
    CTime tmToday = CTime::GetCurrentTime();
    time_t tmYesterday = KncUtil::TimeToInt( tmToday - CTimeSpan( 1, 0, 0, 0 ) );
    nAccumulateCount_ = 0;
    tmLastAttendanceTime_ = tmYesterday;

    // 현재 총 출석 누적수, 마지막 출석 날짜 얻는다
    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nAccumulateCount_ 
            >> tmLastAttendanceTime_ );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"누적 출석 누적일수/마지막출석일 얻기 UserUID : " << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( nAccumulateCount_ )
        << BUILD_LOGtm( CTime(tmLastAttendanceTime_) )
        << END_LOG;
}

bool KGSDBThread::UpdateAttendance_AccAttendance( IN const DWORD& dwUserUID_, IN const int& nVersion_, IN const int& nDiff_, IN time_t& tmLastAttendanceTime_ )
{
    /*
    발렌타인이벤트( 화령의축제) 유저별출석정보갱신

    L"{ call dbo.FSGAFlowerSpiritEventAttendance_merge ( %d, %d, %d,  N'%s' ) }"

    { call dbo.FSGAFlowerSpiritEventAttendance_merge ( @1, @2, @3, @4 ) }
    @1: @iLoginUID_              int
    @2: @iVersion_               int
    @3: @iAccumulatedCount_Diff_ int  //누적출석Diff값( 아마서버에서DB로+1 값만줄듯)
    @4: @sdtAttendanceDate_      smalldatetime   //마지막으로출석한날짜( 날짜형식: yyyy-mm-dd  시,분,초는서버에서DB로 00:00으로 고정하여 호출 )

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -2      : 중복출석( 이미출석을찍음) 
        -101이하: 트랜젝션에러
*/

    DBJIF2( m_kODBC.Query( L"{ call dbo.FSGAFlowerSpiritEventAttendance_merge ( %d, %d, %d, N'%s' ) }",
        dwUserUID_, nVersion_, nDiff_, (LPCTSTR)CTime(tmLastAttendanceTime_).Format( L"%Y%m%d" ) ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"누적 출석 마지막 출석일 갱신 UserUID : " << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( nDiff_ )
        << BUILD_LOGtm( CTime(tmLastAttendanceTime_) )
        << END_LOG;

    return ( nOK == 0 );
}

void KGSDBThread::GetUserRewardStateList_AccAttendance( IN const DWORD& dwUserUID_, IN const int& nVersion_, OUT std::vector<int>& vecRewardCompleteDay_ )
{
    /*
    발렌타인이벤트( 화령의축제) 유저별출석보상정보조회

    L"{ call dbo.FSGAFlowerSpiritEventReward_Select ( %d, %d ) }"

    { call dbo.FSGAFlowerSpiritEventReward_Select ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iVersion_ int

    n return ( @1, @2 )
    @1 ; AccumulatedCount   int
*/
    vecRewardCompleteDay_.clear();

    // 보상을 이미 받은 날짜들을 얻어 온다
    DBJIF( m_kODBC.Query( L"{ call dbo.FSGAFlowerSpiritEventReward_Select( %d, %d ) }", dwUserUID_, nVersion_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nRewardCompleteDay=0;
        VERIFY_QUERY( m_kODBC >> nRewardCompleteDay );
        vecRewardCompleteDay_.push_back( nRewardCompleteDay );
    }

    START_LOG( clog, L"누적 출석 보상을 이미 얻은 날짜 리스트 얻기" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( vecRewardCompleteDay_.size() )
        << END_LOG;
}

// DB에 보상 지급 상태 변경 함수
int KGSDBThread::UpdateUserRewardStateList_AccAttendance( IN const DWORD& dwUserUID_, IN const int& nVersion_, IN int& nRewardGetDay_, IN GCITEMID& dwItemID_ )
{
/*
    발렌타인이벤트( 화령의축제) 유저별출석보상정보입력

    L"{ call dbo.FSGAFlowerSpiritEventReward_insert ( %d, %d, %d,  N'%s' ) }"

    { call dbo.FSGAFlowerSpiritEventReward_insert ( @1, @2, @3, @4 ) }
    @1: @iLoginUID_          int
    @2: @iVersion_           int
    @3: @iAccumulatedCount_  int  //총누적출석값
    @4: @iRewardItemID       int  //보상으로받은ItemID
        
    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가없음
        -2      : 중복보상에러( 해당출석에동일한아이템을이미받았음) 
        -101이하: 트랜젝션에러
*/

    DBJIF2( m_kODBC.Query( L"{ call dbo.FSGAFlowerSpiritEventReward_insert ( %d, %d, %d, %d ) }",
        dwUserUID_, nVersion_, nRewardGetDay_, dwItemID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"누적 출석 보상 지급 완료 날짜 갱신 UserUID: " << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( nRewardGetDay_ )
        << BUILD_LOG( dwItemID_ )
        << END_LOG;

    return nOK;
}

int KGSDBThread::InsertPvPBalancePromotion( IN const DWORD& dwUID_, IN const char& cCharType_, IN const char& cPromotion_, IN const DWORD& dwLevel_, IN const int& nModeID_, IN const int& nPlayTime_, IN const int& nResult_, IN const int& nLifeCount_ )
{
    /*
    유저전직별대전정보저장
    L"{ call dbo.PBGAPvpBalancePromotion_insert ( %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.PBGAPvpBalancePromotion_insert ( @1, @2, @3, @4, @5, @6, @7, @8 ) }
    @1 ; @iLoginUID_       int
    @2 ; @iCharType_       int  // 케릭터번호
    @3 ; @iPromotion_      int  // 케릭터전직
    @4 ; @iLevel_          int  // 케릭터레벨
    @5 ; @iModeID_         int  // 던전번호
    @6 ; @iPlayTime_       int  // 플레이타임
    @7 ; @iResult_         int  // 승0 , 패1
    @8 ; @iLifeCount_      int  // 남은목숨수

    1 return ( @1, @2 )
    @1: OK      int
    0       : 성공
    -101이하: 트랜젝션에러
    @2: UID     int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPvpBalancePromotion_insert ( %d, %d, %d, %d, %d, %d, %d, %d ) }",
        dwUID_, cCharType_, cPromotion_, dwLevel_, nModeID_, nPlayTime_, nResult_, nLifeCount_ ) );

    int nOK = -99;
    int nUID = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> nUID );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 전직별 대전 정보 저장 UserUID: " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nUID )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOGc( cPromotion_ )
        << BUILD_LOG( dwLevel_ )
        << BUILD_LOG( nModeID_ )
        << BUILD_LOG( nPlayTime_ )
        << BUILD_LOG( nResult_ )
        << BUILD_LOG( nLifeCount_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return nUID;
}

void KGSDBThread::InsertPvPBalanceSkillUseCount( IN const DWORD& dwUID_, IN const int& nUID_, IN const std::map<int,int>& mapSkillUseCount_ )
{
    /*
    대전- 스킬사용카운트저장
    L"{ call dbo.PBGAPvpBalanceSkillUseCount_insert ( %d, %d, %d, %d ) }"

    { call dbo.PBGAPvpBalanceSkillUseCount_insert ( @1, @2, @3, @4 ) }
        @1 ; @iLoginUID_       int
        @2 ; @iUID_            int
        @3 ; @iSkillID_        int  // 스킬번호
        @4 ; @iSkillUseCount_  int  // 스킬사용카운트

    1 return ( @1 )
    @1: OK      int
        0       : 성공
        -101이하: 트랜젝션에러
    */

    std::map<int,int>::const_iterator cmit;
    for( cmit = mapSkillUseCount_.begin(); cmit != mapSkillUseCount_.end(); ++cmit ) {
        DBJIF( m_kODBC.Query( L"{ call dbo.PBGAPvpBalanceSkillUseCount_insert ( %d, %d, %d, %d ) }",
            dwUID_, nUID_, cmit->first, cmit->second ) );

        int nOK = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        START_LOG( clog, L"PvP 스킬 사용 카운트 저장 UserUID: " << dwUID_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( nUID_ )
            << BUILD_LOG( cmit->first )
            << BUILD_LOG( cmit->second )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }
}

void KGSDBThread::InsertPvPBalanceEquipSkillSlot( IN const DWORD& dwUID_, IN const int& nUID_, IN const std::map<int,int>& mapSkillSlotInfo_ )
{
    /*
    대전- 스킬슬롯에장착된스킬정보저장
    L"{ call dbo.PBGAPvpBalanceEqiupSlotSkill_insert ( %d, %d, %d, %d ) }"

    { call dbo.PBGAPvpBalanceEqiupSlotSkill_insert ( @1, @2, @3, @4 ) }
        @1 ; @iLoginUID_       int
        @2 ; @iUID_            int
        @3 ; @iIndexID_        int  // 슬롯인덱스번호
        @4 ; @iSkillID_        int  // 창작중인스킬번호

    1 return ( @1 )
    @1: OK      int
        0       : 성공
        -101이하: 트랜젝션에러
    */

    std::map<int,int>::const_iterator cmit;
    for( cmit = mapSkillSlotInfo_.begin(); cmit != mapSkillSlotInfo_.end(); ++cmit ) {
        DBJIF( m_kODBC.Query( L"{ call dbo.PBGAPvpBalanceEqiupSlotSkill_insert ( %d, %d, %d, %d ) }",
            dwUID_, nUID_, cmit->first, cmit->second ) );

        int nOK = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        START_LOG( clog, L"PvP 스킬슬롯에 장착된 스킬 정보 저장 UserUID: " << dwUID_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( nUID_ )
            << BUILD_LOG( cmit->first )
            << BUILD_LOG( cmit->second )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }
}

void KGSDBThread::GetPetDefaultInfo( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_ )
{
    /*
    L"{ call dbo.UIGAUserItemPet_select_20130510 ( %d, %d ) }"

    { call dbo.UIGAUserItemPet_select_20130510 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02, @03, @04, @05 )
    @01: PetUID    bigint
    @02: PetID     int
    @03: PetName   nvarchar( 20 )
    @04: Promotion tinyint
    @05: FullTime  int
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemPet_select_20130510 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KPetInfo kPetInfo;
        kPetInfo.Init();
        kPetInfo.m_vecInitEquipItem.clear();
        kPetInfo.m_vecEquipItem.clear();

        VERIFY_QUERY( m_kODBC
            >> kPetInfo.m_dwUID         // 펫 UID
            >> kPetInfo.m_dwID          // 펫 아이템 아이디
            >> kPetInfo.m_strName       // 펫 이름
            >> kPetInfo.m_cPromotion    // 진화
            >> kPetInfo.m_iSatiation ); // 만복도

        kPetInfo.m_cCharType = cCharType_; // 현재 파라메터로 넘긴 CharType 세팅.

        if( mapPetInfo_.find( kPetInfo.m_dwUID) == mapPetInfo_.end() ) {
            // 초기 상태 설정.
            kPetInfo.m_iInitSatiation   = kPetInfo.m_iSatiation;
            kPetInfo.m_nHatchingID =  SiKPetHelper()->GetNextID( kPetInfo.m_dwID );

            // 펫의 레벨 세팅
            mapPetInfo_.insert( std::make_pair( kPetInfo.m_dwUID, kPetInfo ) );
        }
        else {
            START_LOG( cerr, L"펫 정보 가져오는중 중복 데이터 있음." << L" UserID : " << dwUserUID_ 
                << L", Pet UID : " << kPetInfo.m_dwUID ) << END_LOG;
        }
    }
}

void KGSDBThread::GetEquipPetCostume( IN const GCITEMUID& PetUID_, OUT GCITEMUID& CostumeUID_, OUT GCITEMID& CostumeID_ )
{
    /*
    { call dbo.PIGA_pet_info_select_costume ( @1 ) }
    @1 ; PetUID bigint

    1 return ( @1, @2 )
    @1 ; CostumeUID int
    @2 ; CostumeID int
    0         ; 기본 코스튬
    빈 데이터 ; 기본 코스튬
    기타      ; 해당 코스튬 아이템 ID
    */
    CostumeUID_ = 0;
    CostumeID_ = 0;

    DBJIF( m_kODBC.Query( L"{ call dbo.PIGA_pet_info_select_costume ( %I64d ) }", PetUID_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> CostumeUID_ >> CostumeID_ );
    }
    m_kODBC.EndFetch();
};
void KGSDBThread::GetCharEquipPetCostume( IN const DWORD& dwUserUID_, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo_ )
{
    std::map<char, KCharacterInfo>::iterator mit;
    for( mit = mapCharacterInfo_.begin(); mit != mapCharacterInfo_.end(); ++mit ) {
        if( mit->second.m_kEquipPetInfo.m_dwUID != 0 ) {
            GetEquipPetCostume( mit->second.m_kEquipPetInfo.m_dwUID, mit->second.m_kEquipPetInfo.m_kCostume.m_dwUID, mit->second.m_kEquipPetInfo.m_kCostume.m_dwID );
        }
    }
}

void KGSDBThread::GetCharPetInfo( IN const std::wstring strLogin_, IN const DWORD dwUID_, IN const char& cCharType_, OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_, IN OUT std::vector<KItem>& vecInv_ )
{
    // 펫 기본 정보 가져오기.
    GetPetDefaultInfo( dwUID_, cCharType_, mapPetInfo_ );
    // 펫 전직별 경험치 가져오기
    GetPetExp( dwUID_, cCharType_, mapPetInfo_ );

    GetPetInfo( dwUID_, cCharType_, vecInv_, mapPetInfo_ );
    // 펫 코스튬 가져 오기..
    GetPetCostume( strLogin_, mapPetInfo_, vecInv_ );
}

void KGSDBThread::UpdateEquipItem( IN const std::wstring& strLogin_, IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const GCITEMUID& ItemUID_, IN const GCITEMID& ItemID_, IN const int& nSlotIndex_, IN const char& cItemType_, IN OUT std::vector< KUserEquipItem >& vecUserEquipItem_, IN int nType_ )
{
    /*
    L"{ call dbo.UIGAUserItem_select_1_ItemUID_20130515 ( %d, %d, %I64d, %d ) }"

    { call dbo.UIGAUserItem_select_1_ItemUID_20130515 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iItemUID_input  bigint
    @04: @04iItemID_input   int

    1 return ( @01 )
    @01: ItemUID bigint
    */

    GCITEMUID biItemUID = 0;
    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItem_select_1_ItemUID_20130515 ( %d, %d, %I64d, %d ) }",
        dwUserUID_, cCharType_, ItemUID_, ItemID_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> biItemUID );
    }
    m_kODBC.EndFetch();

    if( biItemUID != 0 ) {
        KUserEquipItem kUserEquipItem;
        if( nType_ == KGSDBThread::ST_EMOTICON ) {
            kUserEquipItem.m_nItemOrderNo = nSlotIndex_+5;
        } else {
            kUserEquipItem.m_nItemOrderNo = nSlotIndex_;
        }
        kUserEquipItem.m_ItemUID     = ItemUID_;
        kUserEquipItem.m_nItemID      = ItemID_;
        kUserEquipItem.m_nItemType    = (int)cItemType_;
        vecUserEquipItem_.push_back( kUserEquipItem );
    }
    else {
        START_LOG( clog, L"퀵 슬롯 기록 실패" )
            << BUILD_LOG( strLogin_ )
            << BUILD_LOG( nType_ )
            << BUILD_LOG( nSlotIndex_ )
            << BUILD_LOG( ItemUID_ )
            << BUILD_LOG( ItemID_ )
            << BUILD_LOGc( cItemType_ ) << END_LOG;
    }
}
void KGSDBThread::UpdateUserCollection( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const int& nMainMissionID_, IN const int& nSubMissionID_, IN const int& nDiff_ )
{
    /*
    { call dbo.TCGATitleCollectionSub_merge_20130516 ( %d, %d, %d, %d, %d ) }

    { call dbo.TCGATitleCollectionSub_merge_20130516 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input      int
    @02: @02iCharType_input      int
    @03: @03iMainMissionID_input int
    @04: @04iSubMissionID_input  int
    @05: @05iCNT_diff_input      int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 진행중인 칭호가 아님
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.TCGATitleCollectionSub_merge_20130516 ( %d, %d, %d, %d, %d ) }",
        dwUserUID_, cCharType_, nMainMissionID_, nSubMissionID_, nDiff_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"칭호 정보 갱신 OK : " << nOK )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( nMainMissionID_ )
        << BUILD_LOG( nSubMissionID_ )
        << BUILD_LOG( nDiff_ ) << END_LOG;
}
void KGSDBThread::UpdateCharLastLoginDate( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const DWORD& dwSlotNum_ )
{
    /*
    L"{ call dbo.CIGACharacterInfo_update_LastLoginDateA_20130521 ( %d, %d, %d ) }"

    { call dbo.CIGACharacterInfo_update_LastLoginDateA_20130521 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03iSlotNo_input   int

    1 return ( @1 )
    @1: OK int
    0        : 캐릭터 생성 성공
    -1       : 캐릭터가 존재하지 않음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_update_LastLoginDateA_20130521 ( %d, %d, %d ) }",
        dwUserUID_, cCharType_, dwSlotNum_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"캐릭터 변경할 때 정보 갱신 OK : " << nOK )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( dwSlotNum_ ) << END_LOG;
}
void KGSDBThread::GetUserData_ErrandEvent( IN const DWORD& dwUserUID_, IN const int& nVersion_, OUT KErrandUserInfoDataStruct& kData_ )
{
/*
    어린이날 유저 게임 데이터 조회

    L"{ call dbo.EEGAErandEvent_select ( %d, %d ) }"

    { call dbo.EEGAErandEvent_select ( @1, @2 ) }
    @1: @iVersion_   int
    @2: @iLoginUID_  int

    1 return ( @1, @2, @3, @4 )
    @1 ; RemainSecond         int  // 남은 초 ( 초 단위 )
    @2 ; Playable             int  // 게임 플레이 가능 여부
    @3 ; TotalPlaycount       int  // 현재까지 누적 플레이 횟수
    @4 ; TotalComplimentCount int  // 현재까지 누적 칭찬 횟수
*/

    DBJIF( m_kODBC.Query( L"{ call dbo.EEGAErandEvent_select ( %d, %d ) }", nVersion_, dwUserUID_ ) );

    if( m_kODBC.BeginFetch() ) {
        int nPlayable = 0;

        VERIFY_QUERY( m_kODBC
            >> kData_.m_nRemainTime
            >> nPlayable
            >> kData_.m_nTotalPlayCount
            >> kData_.m_nTotalSuccessCount );

        if ( 0 == nPlayable ) {
            kData_.m_bPlayable = false;
        }
        else {
            kData_.m_bPlayable = true;
        }
        
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"심부름왕 유저 데이터 얻기" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( kData_.m_nRemainTime )
        << BUILD_LOG( kData_.m_bPlayable )
        << BUILD_LOG( kData_.m_nTotalPlayCount )
        << BUILD_LOG( kData_.m_nTotalSuccessCount )
        << END_LOG;
}

int KGSDBThread::UpdateUserData_ErrandEvent( IN const DWORD& dwUserUID_, IN const int& nVersion_, IN const KErrandUserInfoDataStruct& kData_ )
{
/*
    어린이날 유저 게임 데이터 저장

    L"{ call dbo.EEGAErandEvent_merge ( %d, %d, %d, %d, %d, %d ) }"

    { call dbo.EEGAErandEvent_merge ( @1, @2, @3, @4, @5, @6 ) }
    @1: @iVersion_              int
    @2: @iLoginUID_             int
    @3: @iRemainSecond_         int
    @4: @iPlayable_             int
    @5: @iTotalPlaycount_       int
    @6: @iTotalComplimentCount_ int

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가 없음
        -2      : 요청한 누적 플레이 횟수, 누적 칭찬 횟수가 이전 횟수 보다 작음
        -101이하: 트랜젝션 에러
*/

    DBJIF2( m_kODBC.Query( L"{ call dbo.EEGAErandEvent_merge ( %d, %d, %d, %d, %d, %d ) }",
        nVersion_, dwUserUID_, kData_.m_nRemainTime, static_cast<int>( kData_.m_bPlayable ), kData_.m_nTotalPlayCount, kData_.m_nTotalSuccessCount ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"심부름왕 유저 정보 갱신" << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( kData_.m_nRemainTime )
        << BUILD_LOG( kData_.m_bPlayable )
        << BUILD_LOG( kData_.m_nTotalPlayCount )
        << BUILD_LOG( kData_.m_nTotalSuccessCount )
        << END_LOG;

    return nOK;
}

void KGSDBThread::GetUserReceivedSpecialRewardInfo_ErrandEvent( IN const DWORD& dwUserUID_, IN const int& nVersion_, OUT std::vector<PAIR_INT>& vecRecvedSpecialReward_ )
{
/*
    어린이날 유저 보상 데이터 조회

    L"{ call dbo.EEGAErandEventRewardInfo_select ( %d, %d ) }"

    { call dbo.EEGAErandEventRewardInfo_select ( @1, @2 ) }
    @1: @iVersion_   int
    @2: @iLoginUID_  int

    n return ( @1, @2 )
    @1 ; ItemID  int  // 보상 받은 ItemID
    @2 ; Count   int  // 보상 받은 Count
    @3 ; LastUpdateDate smalldatetime // 마지막 갱신 시간
*/
    vecRecvedSpecialReward_.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.EEGAErandEventRewardInfo_select ( %d, %d ) }", nVersion_, dwUserUID_ ) );

    // 세번째 받는 값은 안씀
    time_t tmReceivedTime = 0;
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        PAIR_INT prData;

        VERIFY_QUERY( m_kODBC
            >> prData.first
            >> prData.second
            >> tmReceivedTime );

        vecRecvedSpecialReward_.push_back( prData );
    }

    START_LOG( clog, L"심부름왕 유저 스페셜 보상 받은 목록 얻기" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( vecRecvedSpecialReward_.size() )
        << END_LOG;
}

int KGSDBThread::UpdateUserReceivedSpecialRewardInfo_ErrandEvent( IN const DWORD& dwUserUID_, IN const int& nVersion_, IN const GCITEMID& dwItemID_, IN const int& nCount_ )
{
/*
    어린이날 유저 보상 데이터 저장

    L"{ call dbo.EEGAErandEventRewardInfo_merge ( %d, %d, %d, %d ) }"

    { call dbo.EEGAErandEventRewardInfo_merge ( @1, @2, @3, @4 ) }
    @1: @iVersion_      int
    @2: @iLoginUID_     int
    @3: @iItemID_       int
    @4: @iCount_        int

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가 없음
        -2      : 요청한 누적 보상 Count 가 이전 Count 보다 작거나 같음 ( Count갱신 요청 무시 )
        -101이하: 트랜젝션 에러
*/


    DBJIF2( m_kODBC.Query( L"{ call dbo.EEGAErandEventRewardInfo_merge ( %d, %d, %d, %d ) }",
        nVersion_, dwUserUID_, dwItemID_, nCount_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"심부름왕 스페셜 보상 아이템별 받은 개수 갱신" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nVersion_ )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( dwItemID_ )
        << BUILD_LOG( nCount_ )
        << END_LOG;

    return nOK;
}

bool KGSDBThread::InsertKairoRewardInfo( IN const DWORD& dwUserUID_, IN const GCITEMID& ReqItemID_, IN const DWORD& dwType_, IN KDropItemInfo& kDropItemInfo_ )
{
    /*
    유저별 카이로 보상 아이템 기록 저장
    L"{ call dbo.KRMOKairoReward_insert_20130522 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.KRMOKairoReward_insert_20130522 ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 ) }
        @1  ; @LoginUID_       int            //유저고유번호
        @2  ; @BoxItemID_      int            //보물상자 ItemID
        @3  ; @RewardType_     int            //보상 타입 - 1 : 일반 아이템, 2: 강화석 박힌 아이템
        @4  ; @RewardItemID_   int            //보상 아이템 ItemID
        @5  ; @Duration_       int            //보상 아이템 수량
        @6  ; @Period_         int            //보상 아이템 기간
        @7  ; @GradeID_        int            //보상 아이템 등급
        @8  ; @EnchantLevel_   int            //보상 아이템 강화레벨
        @9  ; @EquipType_      int            //보상 아이템 타입 - 1 : 헬멧, 2 : 자켓, 3 : 바지, 4 : 장갑, 5 : 신발, 6 : 망토, 7 : 무기
        @10 ; @EquipLevel_     int            //보상 아이템 장착 레벨

    1 return ( @1 )
    @1: OK      int
        0       : 성공
        -101이하: 트랜젝션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.KRMOKairoReward_insert_20130522 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
        dwUserUID_, ReqItemID_, dwType_, kDropItemInfo_.m_ItemID, kDropItemInfo_.m_nDuration, kDropItemInfo_.m_nPeriod, kDropItemInfo_.m_cGradeID, kDropItemInfo_.m_cEnchantLevel, kDropItemInfo_.m_cEquipType, kDropItemInfo_.m_cEquipLevel ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"Kairo 보물상자 오픈에 대한 세부 사항 UserUID : " << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nOK )
        << BUILD_LOG( ReqItemID_ )
        << BUILD_LOG( dwType_ )
        << BUILD_LOG( kDropItemInfo_.m_ItemID )
        << END_LOG;

    return ( nOK == 0 );
}

bool KGSDBThread::InsertReturnUserInfo( IN const int& nEventType_, IN const int& nVersion_, IN const DWORD& dwUserUID_ )
{
    /*
    { call dbo.CBGAComeBackUserCheck_insert ( @1, @2, @3 ) }

    { call dbo.CBGAComeBackUserCheck_insert ( %d, %d, %d ) }
    @1 ; @iEventID_  int
    @2 ; @iVersion_  int
    @3 ; @iLoginUID_ int

    1 return ( @1 )
    @1: OK
    0       : 성공
    -1      : 유저데이터없음.
    -101이하: 트랜젝션에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBGAComeBackUserCheck_insert ( %d, %d, %d ) }",
        nEventType_, nVersion_, dwUserUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"복귀 유저 정보 갱신 UserUID : " << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nEventType_ )
        << BUILD_LOG( nVersion_ )
        << END_LOG;

    return ( nOK == 0 );
}

bool KGSDBThread::CheckReturnUser( IN const int& nEventType_, IN const int& nVersion_, IN const DWORD& dwUserUID_ )
{
    /*
    L"{ call dbo.CBGAComeBackUserCheck_select ( %d, %d, %d ) }"

    { call dbo.CBGAComeBackUserCheck_select ( @1, @2, @3 ) }
    @1 ; @iEventID_  int
    @2 ; @iVersion_  int
    @3 ; @iLoginUID_ int

    1 return ( @1 )
    @1 ;  @iOK int   //데이터있으면: 1, 없으면: 0
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CBGAComeBackUserCheck_select ( %d, %d, %d ) }",
        nEventType_, nVersion_, dwUserUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"복귀 유저 정보 체크 UserUID : " << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nEventType_ )
        << BUILD_LOG( nVersion_ )
        << END_LOG;

    return ( nOK == 1 );
}

bool KGSDBThread::UpdateCharLookItemInvenSize( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const DWORD& dwSlotNum_, IN const int& nExtendSize_ )
{
    /*
    L"{ call dbo.CIGACharacterInfo_update_CoordiSize_20130806 ( %d, %d, %d, %d ) }"

    { call dbo.CIGACharacterInfo_update_CoordiSize_20130806 ( @01, @02, @03, @04 ) }
    @01iLoginUID_input        int
   ,@02iCharType_input        int
   ,@03iSlotNo_input          int
   ,@04iCoordiSize_diff_input int

    1 return ( @01 )
    @01: OK        int
    0        : 캐릭터 생성 성공
    -1       : 캐릭터가 존재하지 않음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_update_CoordiSize_20130806 ( %d, %d, %d, %d ) }",
        dwUserUID_, cCharType_, dwSlotNum_, nExtendSize_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"코디 인벤 사이즈 갱신 UserUID : " << dwUserUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( nOK )
        << BUILD_LOG( cCharType_ )
        << BUILD_LOG( dwSlotNum_ )
        << BUILD_LOG( nExtendSize_ )
        << END_LOG;

    return ( nOK == 0 );
}

void KGSDBThread::GetSystemGuideUserData( IN const DWORD& dwUserUID_, OUT std::map< int, KGuideCompleteInfo >& mapComplete_ )
{
/*
    L"{ call dbo.SGGASystemGuide_select ( %d ) }"

    { call dbo.SGGASystemGuide_select ( @1 ) }
    @1 ; @iLoginUID_ int


    1 return ( @1, @2, @3 )
    @1 ; Type         int
    @2 ; ProvideFlag  int
    @3 ; Result       int
*/

    mapComplete_.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.SGGASystemGuide_select ( %d ) }", dwUserUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KGuideCompleteInfo kCompleteInfo;
        int nGuideType = -1;
        VERIFY_QUERY( m_kODBC
            >> nGuideType
            >> kCompleteInfo.m_nProvideFlag
            >> kCompleteInfo.m_nCompleteFlag );

        if( !mapComplete_.insert( std::make_pair( nGuideType, kCompleteInfo ) ).second ) {
            START_LOG( cerr, L"시스템 가이드 완료 목록 중에 중복 되는 가이드 타입이 있음 nGuideType : " << nGuideType ) << END_LOG;
        }
    }

    START_LOG( clog, L"시스템 가이드 DB데이터 얻음" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( mapComplete_.size() )
        << END_LOG;
}

int KGSDBThread::UpdateSystemGuideUserData( IN const DWORD& dwUserUID_, IN int& nGuideType_, IN int& nProvideFlag_, IN int& nCompleteFlag_ )
{
/*
    L"{ call dbo.SGGASystemGuide_merge ( %d, %d, %d, %d ) }"

    { call dbo.SGGASystemGuide_merge ( @1, @2, @3, @4 ) }
    @1: @iLoginUID_     int
    @2: @iType_         int
    @3: @iProvideFlag_  int
    @4: @iResult_       int   //0:진행중  1:성공

    1 return ( @1 )
    @1: OK       int
    0       : 성공
    -1      : 유저가 없음
    -801    ; 로그 기록 실패
    -901    ; 가이드 시스템 정보 갱신 실패
    -902    ; 가이드 시스템 정보 등록 실패
*/


    DBJIF2( m_kODBC.Query( L"{ call dbo.SGGASystemGuide_merge ( %d, %d, %d, %d ) }",
        dwUserUID_, nGuideType_, nProvideFlag_, nCompleteFlag_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_CONDITION( nOK == 0, clog, cerr, L"시스템 가이드 DB데이터 갱신 nOK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

void KGSDBThread::GetUserTutorialEnable( IN const DWORD& dwUID_, IN const int& nSessionInfo_, IN const time_t& tmFirstLoginTime_, IN const int& nEventType_, OUT bool& bTutorialEnable_ )
{
    if( nSessionInfo_ == 0 ) {
        bTutorialEnable_ = true;
        SetTodayEventDoneDiff( dwUID_, nEventType_, tmFirstLoginTime_, 1 );
        return;
    }

    int nData = GetTodayEventCount( dwUID_, nEventType_, tmFirstLoginTime_ );
    if( nData == 1 ) {
        bTutorialEnable_ = true;
    }
    else {
        bTutorialEnable_ = false;
    }

    START_LOG( clog, L"신규 유저 튜토리얼 정보 전달." )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nSessionInfo_ )
        << BUILD_LOG( nEventType_ )
        << BUILD_LOG( bTutorialEnable_ )
        << END_LOG;
}

void KGSDBThread::GetAttributeMigrationInfo( IN const DWORD& dwUID_, OUT std::map<GCITEMUID, DWORD>& mapAttributeMigrationItemInfo_ )
{
    mapAttributeMigrationItemInfo_.clear();
    if( SiKItemManager()->m_kAttributeDecider.CheckMigrationAttributeEventEnable() ) {
        if( SiKItemManager()->m_kAttributeDecider.IsEventTerm() || SiKItemManager()->m_kAttributeDecider.CheckUserCountCheckEnable() ) {
            LIF( m_kHelper.GetUserAttributeMigrationInfo( dwUID_, mapAttributeMigrationItemInfo_ ) );
            }
        }
}

void KGSDBThread::MonsterCardMigration( IN const DWORD& dwUserUID_ )
{
    // 몬스터 카드 마이그레이션 작업.
    /*
    L"{ call dbo.UIGAUserItemSocket_migration_20130716 ( %d ) }"

    { call dbo.UIGAUserItemSocket_migration_20130716 ( @01 ) }
    @01iLoginUID_input int

    1 return ( @01 )
    @01iOK int
    0        : 성공
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemSocket_migration_20130716 ( %d ) }",
        dwUserUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"몬스터 카드 마이그레이션 작업" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( dwUserUID_ )
        << END_LOG;
}

bool KGSDBThread::InsertDungeonLeaveInfo( IN const KEVENT_STAT_DUNGEON_LEAVE_NOT& kDBPacket_ )
{
    /*
    L"{ call dbo.CCGACCBData00_insert_20130426 ( %d, %d, %d, %d, N'%s', N'%s', %d ) }"

    { call dbo.CCGACCBData00_insert_20130426 ( @01, @02, @03, @04, @05, @06, @07 ) }
    @01: @01iModeID_input       int
    @02: @02iDifficulty_input   int
    @03: @03iLoginUID_input     int
    @04: @04iCharType_input     int
    @05: @05sdtStartDateA_input datetime
    @06: @06sdtEndDateA_input   datetime
    @07: @07iStatus_input       int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CCGACCBData00_insert_20130426 ( %d, %d, %d, %d, N'%s', N'%s', %d ) }",
        kDBPacket_.m_nGameMode,
        kDBPacket_.m_nDifficult,
        kDBPacket_.m_dwUserUID,
        kDBPacket_.m_cCharType,
        kDBPacket_.m_wstrGameStartDate.c_str(),
        kDBPacket_.m_wstrGameEndDate.c_str(),
        kDBPacket_.m_nType ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( nRet == 0, L"던전 게임플레이 정상종료,나간유저 통계 DB기록.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( 0 == nRet );
}

bool KGSDBThread::InsertUserPlayTime( IN const KEVENT_STAT_USER_PLAY_TIME_NOT& kDBPacket_ )
{
    /*
    L"{ call dbo.CCGACCBData04_merge_20130426 ( %d, %d, %d, %d ) }"

    { call dbo.CCGACCBData04_merge_20130426 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input      int
    @02: @02iPVETime_diff_input  int
    @03: @03iPVPTime_diff_input  int
    @04: @04iElsetime_diff_input int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CCGACCBData04_merge_20130426 ( %d, %d, %d, %d ) }",
        kDBPacket_.m_dwUserUID,
        kDBPacket_.m_dwPVEPlayTime,
        kDBPacket_.m_dwPVPPlayTime,
        kDBPacket_.m_dwUserDurationTime ) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( nRet == 0, L"유저 던전,대전 playTime DB기록.. Result : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( 0 == nRet );
}

bool KGSDBThread::InsertLevelGPInfo( IN const KEVENT_STAT_LEVEL_GP_INFO_NOT& kDBPacket_ )
{
    /*
    L"{ call dbo.CCGACCBData05_merge_20130426 ( %d, %I64d, %I64d ) }"

    { call dbo.CCGACCBData05_merge_20130426 ( @01, @02, @03 ) }
    @01: @01iLevel_input               int
    @02: @02iGamePointPlus_diff_input  int
    @03: @03iGamePointMinus_diff_input int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    int nRet = -99;
    std::map<int, std::pair<__int64, __int64>>::const_iterator cmit;
    for( cmit = kDBPacket_.begin(); cmit!= kDBPacket_.end(); ++cmit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.CCGACCBData05_merge_20130426 ( %d, %I64d, %I64d ) }",
            cmit->first,
            cmit->second.first,
            cmit->second.second ) );

        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

        _LOG_SUCCESS( nRet == 0, L"레벨별 GP 증가 감소 정보 DB기록.. Result : " << nRet )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    return ( 0 == nRet );
}
void KGSDBThread::GetUserSlotCnt( IN const DWORD& dwUserUID_, OUT int& nTotalSlotCnt_ )
{
    /*
    L"{ call dbo.UIGAUserInfoSlotCNT_select_20130325 ( %d ) }"

    { call dbo.UIGAUserInfoSlotCNT_select_20130325 ( @1 ) }
    @1: @01iLoginUID_input int

    1 return ( @1 )
    @1: SlotCNT int
    */

    int nSlotCnt;
    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserInfoSlotCNT_select_20130325 ( %d ) }",
        dwUserUID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nSlotCnt );
    }
    m_kODBC.EndFetch();

    nTotalSlotCnt_ = nSlotCnt;

    START_LOG( clog, L"유저 슬롯 전체 갯수 가져오기 UserUID: " << dwUserUID_ )
        << BUILD_LOG( nSlotCnt )
        << END_LOG;
}

bool KGSDBThread::UpdateUserSlotCnt( IN const DWORD& dwUserUID_, OUT int& nAddSlotCnt_ )
{
    /*
    L"{ call dbo.UIGAUserInfoSlotCNT_merge_20130325 ( %d, %d ) }"

    { call dbo.UIGAUserInfoSlotCNT_merge_20130325 ( @01, @02 ) }
    @01: @01iLoginUID_input     int
    @02: @02iSlotCNT_diff_input int

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -1      : 유저 정보 없음
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserInfoSlotCNT_merge_20130325 ( %d, %d ) }",
        dwUserUID_, nAddSlotCnt_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"유저 슬롯 갯수 갱신 UserUID: " << dwUserUID_ )
        << BUILD_LOG( nOK )
        << END_LOG;

    return true;
}

int KGSDBThread::InsertSlotCharInfo(IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const DWORD& dwSlotNumber_, OUT KCharacterInfo& kCharacterInfo_)
{
    int nOK = -99;
    DBJIF2(m_kODBC.Query(L"{ call dbo.CIGACharacterInfo_insert_20130326 ( %d, %d, %d ) }",
        dwUserUID_, cCharType_, dwSlotNumber_));

    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nOK);
    }
    m_kODBC.EndFetch();

    if (0 == nOK) {
        LIF(GetCharacterInfo_1(dwUserUID_, cCharType_, kCharacterInfo_));
    }
    else {
        START_LOG(cerr, L"슬롯에 캐릭터 생성 실패 UserUID: " << dwUserUID_)
            << BUILD_LOG(nOK)
            << BUILD_LOG(dwUserUID_)
            << BUILD_LOGc(cCharType_)
            << BUILD_LOG(dwSlotNumber_)
            << BUILD_LOG(m_kODBC.m_strLastQuery)
            << END_LOG;
    }
    kCharacterInfo_.m_dwSlotNum = dwSlotNumber_;

    SiKSkillManager()->SetNewCharSkillInfo(kCharacterInfo_.m_cCharType, kCharacterInfo_.m_dwLevel, kCharacterInfo_.m_kSkillInfo.m_nLvSPPoint, kCharacterInfo_.m_kSkillInfo.m_nMaxSPPoint);

    return nOK;
}

int KGSDBThread::UpdateCharNickName( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const DWORD& dwSlotNum_, IN const std::wstring& wstrNickName_ )
{
    /*
    L"{ call dbo.CNGACharacterNickname_merge_20130327 ( %d, %d, %d, N'%s' ) }"

    { call dbo.CNGACharacterNickname_merge_20130327 ( @01, @02, @03, @04 ) }
    @01: @01iLoginUID_input   int
    @02: @02iCharType_input   int
    @03: @03iSlotNo_input     int
    @04: @04strNickname_input nvarchar( 24 )

    1 return ( @01 )
    @01: OK int
    0       : 성공
    -1      : 글로벌 닉네임 입력중 에러
    -2      : 유저 정보 없음
    -3      : 이미 존재하는 로컬 닉네임
    -4      : 닉네임, 캐릭터 타입, 슬롯번호 오류
    -5      : 닉네임, 캐릭터 타입, 슬롯번호 오류
    -6      : 닉네임, 캐릭터 타입, 슬롯번호 오류
    -101이하: 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CNGACharacterNickname_merge_20130327 ( %d, %d, %d, N'%s' ) }",
        dwUserUID_, cCharType_, dwSlotNum_, wstrNickName_.c_str() ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"캐릭터 닉네임 갱신 UserUID: " << dwUserUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( dwSlotNum_ )
        << BUILD_LOG( wstrNickName_ )
        << BUILD_LOG( nOK )
        << END_LOG;

    _LOG_SUCCESS( nOK == 0, L"캐릭터 닉네임 갱신 nOK: " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

void KGSDBThread::GetCharNickName( IN const DWORD& dwUserUID_, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo_ )
{
    /*
    L"{ call dbo.CNGACharacterNickname_select_20130327 ( %d ) }"

    { call dbo.CNGACharacterNickname_select_20130327 ( @01 ) }
    @01: @01iLoginUID_input int

    n return ( @01, @02 )
    @01: CharType int
    @02: Nickname nvarchar( 24 )
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.CNGACharacterNickname_select_20130327 ( %d ) }",
        dwUserUID_ ) );

    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        char cCharType = -1;
        std::wstring wstrNickName;
        std::map<char, KCharacterInfo>::iterator mit;

        VERIFY_QUERY( m_kODBC
            >> cCharType
            >> wstrNickName );

        mit = mapCharacterInfo_.find( cCharType );
        if( mit != mapCharacterInfo_.end() ) {
            mit->second.m_strCharName = wstrNickName;
        }

        START_LOG( clog, L"캐릭터 닉네임 정보 가져오기 UserUID: " << dwUserUID_ )
            << BUILD_LOGc( cCharType )
            << BUILD_LOG( wstrNickName )
            << END_LOG;
    }
}
int KGSDBThread::UpdateCharQuickSlotDelete( IN const DWORD& dwUserUID_, IN const char& cCharType_ )
{
    /*
    L"{ call dbo.GQGAGameQuickslot_delete_20130523 ( %d, %d, %d ) }"

    { call dbo.GQGAGameQuickslot_delete_20130523 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03OrderNo_input   int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 삭제될 데이터가 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.GQGAGameQuickslot_delete_20130523 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"캐릭터 장착 정보 삭제 OK : " << nOK )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOGc( cCharType_ ) 
        << END_LOG;

    return nOK;
}

void KGSDBThread::UpdateCharQuickSlotMerge( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN const int& nItemOrderNo_, IN const GCITEMUID& ItemUID_, IN const GCITEMID& ItemID_ )
{
    /*
    L"{ call dbo.GQGAGameQuickslot_merge_20130523 ( %d, %d, %d, %I64d, %d ) }"

    { call dbo.GQGAGameQuickslot_merge_20130523 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int
    @03: @03OrderNo_input   int
    @04: @04iItemUID_input  bigint
    @05: @05iItemID_input   int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 캐릭터 정보 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.GQGAGameQuickslot_merge_20130523 ( %d, %d, %d, %I64d, %d ) }",
        dwUserUID_, cCharType_, nItemOrderNo_, ItemUID_, ItemID_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"캐릭터 장착 정보 갱신 OK : " << nOK )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( nItemOrderNo_ )
        << BUILD_LOG( ItemUID_ )
        << BUILD_LOG( ItemID_ )
        << END_LOG;
}

void KGSDBThread::SelectCharQuickSlotInfo( IN const DWORD& dwUserUID_, IN const char& cCharType_, IN OUT std::vector< KSlotData >& vecItemSlot_, IN OUT std::vector< KSlotData >& vecEmoticonSlot_ )
{
    /*
    L"{ call dbo.GQGAGameQuickslot_select_20130523 ( %d, %d ) }"

    { call dbo.GQGAGameQuickslot_select_20130523 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02, @03 )
    @01: OrderNo int
    @02: ItemUID bigint
    @03: ItemID  int
    */

    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.GQGAGameQuickslot_select_20130523 ( %d, %d ) }",
        dwUserUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KSlotData slotData;
        VERIFY_QUERY( m_kODBC >> slotData.m_nSlotIndex
            >> slotData.m_kItem.m_dwUID
            >> slotData.m_kItem.m_dwID );

        if( slotData.m_nSlotIndex < 5 ) {
            vecItemSlot_.push_back( slotData );
        }
        else {
            slotData.m_nSlotIndex -= 5;
            vecEmoticonSlot_.push_back( slotData );
        }
    }
}

bool KGSDBThread::UpdateUserPointBagAccTime( IN const DWORD& dwUID_, IN const char& cCharType_, IN const int& nDiffGCPoint_, IN const int& nAccTime_, IN std::wstring& wstrDate_ )
{
    /*
    L"{ call dbo.PBGAPointBag_update_20130523 ( %d, %d, %d, %d, N'%s' ) }"

    { call dbo.PBGAPointBag_update_20130523 ( @01, @02, @03, @04, @05 ) }
    @01: @01iLoginUID_input   int
    @02: @02iCharType_input   int
    @03: @03iPoint_diff_input int
    @04: @04iPlayTime_input   int
    @05: @05sdtRegDateD_input smalldatetime

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 포인트 정보가 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPointBag_update_20130523 ( %d, %d, %d, %d, N'%s' ) }",
        dwUID_, cCharType_, nDiffGCPoint_, nAccTime_, wstrDate_.c_str() ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"포인트백 저장 결과 : " << nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( nDiffGCPoint_ )
        << BUILD_LOG( nAccTime_ )
        << BUILD_LOG( wstrDate_ ) << END_LOG;

    return ( nOK == 0 );
}

int KGSDBThread::CheckPointBagItemInfo( IN const DWORD& dwUID_, IN const char& cCharType_, IN const GCITEMID& ItemID_ )
{
    /*
    L"{ call dbo.PBGAPointBagItem_check_20130523 ( %d, %d, %d ) }"

    { call dbo.PBGAPointBagItem_check_20130523 ( @01, @02, @04 ) }
        @01iLoginUID_input int
    ,   @02iCharType_input int
    ,   @03iItemID_input   int

    1 return ( @01 )
    @01: OK int
    0 : 구매 가능
    -1: 유저 정보 없음
    -2: 캐릭터 정보 없음
    -3: GoodsInfoList 아이템 정보 없음
    -4: 출석포인트 정보 없음
    -5: GPointBagItemList 아이템 정보 없음
    -6: 판매중 아님
    -7: 포인트 부족
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPointBagItem_check_20130523 ( %d, %d, %d ) }",
        dwUID_, cCharType_, ItemID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"포인트백 아이템 체크 결과 : " << nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( ItemID_ ) << END_LOG;

    return nOK;
}

bool KGSDBThread::GetPointBagItemInfo( IN const GCITEMID& ItemID_, OUT int& nPeriod_, OUT int& nCount_, OUT int& nPrice_ )
{
    /*
    L"{ call dbo.PBGAPointBagItem_select_20130523 ( %d ) }"

    { call dbo.PBGAPointBagItem_select_20130523 ( @01 ) }
    @01iItemID_input int

    n return ( @01, @02, @03 )
    @01: Period int
    @02: CNT    int
    @03: Price  int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.PBGAPointBagItem_select_20130523 ( %d ) }",
        ItemID_ ) );

    // DB에서 ok값을 주지 않아서 서버 임시 초기값으로 체크.
    int nPeriod = -99;
    int nCount = -99;
    int nPrice = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nPeriod >> nCount >> nPrice );
        m_kODBC.EndFetch();
    }

    nPeriod_ = nPeriod;
    nCount_ = nCount;
    nPrice_ = nPrice;

    START_LOG( clog, L"포인트백 아이템 정보 가져오기 " )
        << BUILD_LOG( ItemID_ )
        << BUILD_LOG( nPeriod )
        << BUILD_LOG( nCount )
        << BUILD_LOG( nPrice )
        << END_LOG;

    if( nPeriod == -99 && nCount == -99 && nPrice == -99 ) {
        return false;
    } else {
        return true;
    }
}

bool KGSDBThread::CheckPlayMainMission( IN const DWORD& dwUID_, IN const char& cCharType_, IN const DWORD& dwMainMissionID_ )
{
    /*
    L"{ call dbo.MSGAMissionSystemMain_select_1_20130527 ( %d, %d, %d ) }"

    { call dbo.MSGAMissionSystemMain_select_1_20130527 ( @01, @02, @03 ) }
    @01iLoginUID_input int
    ,   @02iCharType_input int
    ,   @03iMainID_input   int

    n return ( @01 )
    @01: MainID int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.MSGAMissionSystemMain_select_1_20130527 ( %d, %d, %d ) }",
        dwUID_, cCharType_, dwMainMissionID_ ) );

    int nMissionID = 0;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nMissionID );
        m_kODBC.EndFetch();
    }

    START_LOG( clog, L"진행중인 미션 체크 ")
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( dwMainMissionID_ )
        << END_LOG;

    return ( nMissionID != 0 );
}

bool KGSDBThread::CheckCompleteMainMission( IN const DWORD& dwUID_, IN const char& cCharType_, IN const DWORD& dwMainMissionID_ )
{
    /*
    L"{ call dbo.MSGAMissionSystemTag_select_1_20130527 ( %d, %d, %d ) }"

    { call dbo.MSGAMissionSystemTag_select_1_20130527 ( @01, @02, @03 ) }
    @01iLoginUID_input int
    ,   @02iCharType_input int
    ,   @03iMainID_input   int

    n return ( @01 )
    @01: MainID int
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.MSGAMissionSystemTag_select_1_20130527 ( %d, %d, %d ) }",
        dwUID_, cCharType_, dwMainMissionID_ ) );

    int nMissionID = 0;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nMissionID );
        m_kODBC.EndFetch();
    }

    // 140123. nodefeat. InvalidCompleteMission테이블에 등록한 조건체크 정상동작 하지 않아서 추가 함
    bool bAlreadyCompleteMission = false;
    if ( nMissionID != dwMainMissionID_ ) {
        bAlreadyCompleteMission = true;
    }

    START_LOG( clog, L"완료한 미션 체크 ")
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( dwMainMissionID_ )
        << END_LOG;

    return bAlreadyCompleteMission;
}

int KGSDBThread::InsertMainMission( IN const DWORD& dwUID_, IN const char& cCharType_, IN const DWORD& dwMainMissionID_, IN const int& nLimitDay_ )
{
    /*
    L"{ call dbo.MSGAMissionSystemMain_insert_20130527 ( %d, %d, %d, %d ) }"

    { call dbo.MSGAMissionSystemMain_insert_20130527 ( @01, @02, @03, @04 ) }
    @01iLoginUID_input int
    ,   @02iCharType_input int
    ,   @03iMainID_input   int
    ,   @04iLimitDay_input int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 캐릭터 정보 없음
    -3       : 이미 진행중인 미션
    -4       : 이미 완료된 미션
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.MSGAMissionSystemMain_insert_20130527 ( %d, %d, %d, %d ) }",
        dwUID_, cCharType_, dwMainMissionID_, nLimitDay_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"Main Mission 정보 넣기 ok : " << nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( dwMainMissionID_ )
        << BUILD_LOG( nLimitDay_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return nOK;
}

bool KGSDBThread::InsertSubMission( IN const DWORD& dwUID_, IN const char& cCharType_, IN const DWORD& dwMainMissionID_, IN const int& nSubMissionID_ )
{
    /*
    L"{ call dbo.MSGAMissionSystemSub_insert_20130527 ( %d, %d, %d, %d, %d ) }"

    { call dbo.MSGAMissionSystemSub_insert_20130527 ( @01, @02, @03, @04, @05 ) }
    @01iLoginUID_input int
    ,   @02iCharType_input int
    ,   @03iMainID_input   int
    ,   @04iSubID_input    int
    ,   @05iCNT_input      int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 캐릭터 정보 없음
    -3       : 원본 미션이 없음
    -4       : 이미 등록된 서브미션
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    // 5번째 파라메터에 0값을 넣어준 이유는 DB에서 추후 미션 개편을 고려해야 개발된 부분이다.
    // 현재 서버로직에서는 해당 부분 작업이 되어 있지 않기 때문에 초기값 0으로 세팅해 주면 된다.
    DBJIF2( m_kODBC.Query( L"{ call dbo.MSGAMissionSystemSub_insert_20130527 ( %d, %d, %d, %d, %d ) }",
        dwUID_, cCharType_, dwMainMissionID_, nSubMissionID_, 0 ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"Sub Mission 정보 넣기 ok : " << nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( dwMainMissionID_ )
        << BUILD_LOG( nSubMissionID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return ( nOK == 0 );
}

int KGSDBThread::InsertDungeonClearInfo( IN const DWORD& dwUID_, IN const char& cCharType_, IN const int& nDungeonID_, IN const int& nDifficult_ )
{
    /*
    { call dbo.DCGADungeonClear_insert_20130524 ( %d, %d, %d, %d ) }

    { call dbo.DCGADungeonClear_insert_20130524 ( @01, @02, @03, @04 ) }
    @01iLoginUID_input   int
    ,   @02iCharType_input   int
    ,   @03DungeonID_input   int
    ,   @04iDifficulty_input int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 캐릭터 정보 없음
    -3       : 이미 데이터가 존재함
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.DCGADungeonClear_insert_20130524 ( %d, %d, %d, %d ) }",
        dwUID_, cCharType_, nDungeonID_, nDifficult_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    START_LOG( clog, L"던전 클리어 정보 넣기 ok : " << nOK )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( nDungeonID_ )
        << BUILD_LOG( nDifficult_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return nOK;
}


int KGSDBThread::UpdateCharInvenCapacity( IN const DWORD& dwUID_, IN const char& cCharType_, IN const DWORD& dwSlowNum_, IN const int& nInvenCapacity_ )
{
    /*
    L"{ call dbo.CIGACharacterInfo_update_InvenSize_20130627 ( %d, %d, %d, %d ) }"

    { call dbo.CIGACharacterInfo_update_InvenSize_20130627 ( @01, @02, @03, @04 ) }
    @01iLoginUID_input       int
    ,   @02iCharType_input       int
    ,   @03iSlotNo_input         int
    ,   @04iInvenSize_diff_input int

    1 return ( @01 )
    @01: OK        int
    0        : 캐릭터 생성 성공
    -1       : 캐릭터가 존재하지 않음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_update_InvenSize_20130627 ( %d, %d, %d, %d ) }",
        dwUID_, (int)cCharType_, (int)dwSlowNum_, nInvenCapacity_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_CONDITION( nOK == 0, clog, cerr, L"캐릭터 인벤토리 사이즈 DB데이터 갱신 nOK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

int KGSDBThread::CheckCollectionInfo( IN const DWORD& dwUID_, IN const char& cCharType_, IN const int& nMainMissionID_ )
{
    /*
    L"{ call dbo.TCGATitleCollectionMain_select_Status_20130629 ( %d, %d, %d ) }"

    { call dbo.TCGATitleCollectionMain_select_Status_20130629 ( @01, @02, @03 ) }
    @01iLoginUID_input      int
    ,   @02iCharType_input      int
    ,   @03iMainMissionID_input int

    1 select ( @01 )
    @01: Status int
    0: 칭호 미등록 상태
    1: 칭호 획득 가능
    2: 칭호 획득 완료
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.TCGATitleCollectionMain_select_Status_20130629 ( %d, %d, %d ) }",
        dwUID_, (int)cCharType_, nMainMissionID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_CONDITION( nOK == 0, clog, cerr, L"칭호 미션 완료가능한지 체크 nOK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nOK;
}

bool KGSDBThread::SetTodayCharEventDoneDiff( IN const DWORD dwUID_, IN const char& cCharType_, IN const int nEventType_ )
{
    /*
    L"{ call dbo.SDGAServerDataIntCharacter_merge_20130630 ( %d, %d, N'%s', %d, %d ) }"

    { call dbo.SDGAServerDataIntCharacter_merge_20130630 ( @01, @02, @03, @04, @5 ) }
    @01iLoginUID_input   int
   ,@02iCharType_input   int
   ,@03sdtRegDateD_input smalldatetime
   ,@04iType_input       int
   ,@05iData_diff_input  int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 유저 정보 없음
    -2       : 캐릭터 정보 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.SDGAServerDataIntCharacter_merge_20130630 ( %d, %d, N'%s', %d, 1 ) }",
        dwUID_, cCharType_, (LPCTSTR)CTime::GetCurrentTime().Format( L"%Y%m%d" ), nEventType_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"캐릭터 일일 이벤트 셋팅 오류. UID: " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( nEventType_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

int KGSDBThread::GetTodayCharEventCount( IN const DWORD dwUID_, IN const char& cCharType_, IN const int nEventType_ )
{
    /*
    L"{ call dbo.SDGAServerDataIntCharacter_select_20130630 ( %d, %d, N'%s', %d ) }"

    { call dbo.SDGAServerDataIntCharacter_select_20130630 ( @01, @02, @03, @04 ) }
    @01iLoginUID_input   int
   ,@02iCharType_input   int
   ,@03sdtRegDateD_input smalldatetime
   ,@04iType_input       int

    1 select ( @01 )
    @01iData int
    */

    int nRes = 0;
    DBJIF3( m_kODBC.Query( L"{ call dbo.SDGAServerDataIntCharacter_select_20130630 ( %d, %d, N'%s', %d ) }",
        dwUID_, cCharType_, (LPCTSTR)CTime::GetCurrentTime().Format( L"%Y%m%d" ), nEventType_ ), return -1 );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRes );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"캐릭터 일일 이벤트 조회. UID: " << dwUID_ )
        << BUILD_LOG( nRes )
        << BUILD_LOG( nEventType_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return nRes;
}

bool KGSDBThread::SetJumpingCharInfo( IN const std::wstring& strLogin_, IN const DWORD& dwUID_, IN const char& cCharType_, IN KCharacterInfo& kCharacterInfo_ )
{
    int nWin = 0;
    int nLose = 0;
    int nOK = -99;
    __int64 biDiffExp = kCharacterInfo_.m_biExp - kCharacterInfo_.m_biInitExp;
    __int64 biTotalExp = kCharacterInfo_.m_biTotalExp - kCharacterInfo_.m_biInitTotalExp;

    DBJIF2( m_kODBC.Query( L"{ call dbo.CGGA_character_gameinfo_update (  %d, %d, %I64d, %d, %d, %I64d ) }",
        dwUID_,
        (int)cCharType_,
        biDiffExp,
        nWin,
        nLose,
        biTotalExp ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"캐릭터 기록 Name : " << strLogin_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( biDiffExp )
        << BUILD_LOG( biTotalExp )
        << END_LOG;

    return true;
}

void KGSDBThread::SetJumpingCharLevelPromotion( IN const DWORD& dwLevel_, IN const char& cPromotion_, IN OUT KCharacterInfo& kCharacterInfo_ )
{
    __int64 biExp = SiKResultManager()->GetExpByLv( dwLevel_ - 1 );
    kCharacterInfo_.m_biTotalExp = biExp;
    kCharacterInfo_.m_biExp = biExp;
    kCharacterInfo_.m_dwLevel = dwLevel_;
    kCharacterInfo_.m_cPromotion = cPromotion_;
}

void KGSDBThread::SetJumpingCharInitLevel( IN OUT KCharacterInfo& kCharacterInfo_ )
{
    kCharacterInfo_.m_biInitTotalExp = kCharacterInfo_.m_biTotalExp;
    kCharacterInfo_.m_biInitExp = kCharacterInfo_.m_biExp;
}

bool KGSDBThread::InsertJumpingCharLog( IN const DWORD& dwUID_, IN const int& nType_, IN const char cCharType_, IN const DWORD dwBeforeLevel_, IN const DWORD dwCurrentLevel_ )
{
    /*
    L"{ call dbo.JCSLJumpingCharacter_insert_20130812 ( %d, %d, %d, %d, %d ) }"

    { call dbo.JCSLJumpingCharacter_insert_20130812 ( @01, @02, @03, @04, @05 ) }
        @01iLoginUID_input    int
    ,   @02iCharType_input    int
    ,   @03iLevelBefore_input int
    ,   @04iLevelAfter_input  int
    ,   @05iCreatetype_input  int
        0: 캐릭터 생성 후 점핑
        1: 기존 캐릭터 점핑
        2: 아이템 보상

    1 return ( @01 )
    @01iOK int
        0        : 성공
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.JCSLJumpingCharacter_insert_20130812 ( %d, %d, %d, %d, %d ) }",
        dwUID_, cCharType_, dwBeforeLevel_, dwCurrentLevel_, nType_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_CONDITION( nOK == 0, clog, cerr, L"점핑 캐릭터 로그 데이터 기록 nOK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

void KGSDBThread::GetEventDungeonDropRatio( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ )
{
    /*
    { call dbo.EPDBEventPerfectDungeonDrop_select_20130812 ( %d ) }

    { call dbo.EPDBEventPerfectDungeonDrop_select_20130812 ( @01 ) }
    @01iEventUID_input int

    n return ( @01, @02 )
    @01iDungeonID   int
    @02iDropPercent int : 0~100 정수로 출력(디봇에서 해당 값으로 입력함)
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.EPDBEventPerfectDungeonDrop_select_20130812 ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        int nModeID = -1;
        int nDropRatio = -1;
        VERIFY_QUERY( m_kODBC >> nModeID >> nDropRatio );
        sTimeEvent_.m_mapModeList[nModeID].insert( nDropRatio );
        sTimeEvent_.m_nDropRatio = nDropRatio;
    }
}

bool KGSDBThread::InsertUserRelaySendCount( IN const DWORD& dwUID_, IN KEVENT_USER_RELAY_SEND_COUNT_REQ& kPacket_ )
{
    /*
    L"{ call dbo.TRGATcpRelayServerSendCount_insert ( %d, %d, %d, %d, %d, %d ) }"

    { call dbo.TRGATcpRelayServerSendCount_insert ( @1, @2, @3, @4, @5, @6 ) }
    @iLoginUID_     int
    ,   @iModeID_       int
    ,   @iCharType_     int
    ,   @iPlayTime_     int
    ,   @iType_         int  //0: TCP Relay Send Count, 1: UDP Relay Send Count 
    ,   @iCount_        int

    1 return ( @1 )
    @1: iOK       int
    0       ; 성공
    -801    ; 로그기록실패
    */

    std::map<int,int>::iterator mit;
    int nOK = -99;
    for( mit = kPacket_.m_mapRelaySendCount.begin(); mit != kPacket_.m_mapRelaySendCount.end(); ++mit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.TRGATcpRelayServerSendCount_insert ( %d, %d, %d, %d, %d, %d ) }",
            dwUID_, kPacket_.m_nModeID, kPacket_.m_cCharType, kPacket_.m_dwPlayTime, mit->first, mit->second ) );

        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        _LOG_CONDITION( nOK == 0, clog, cerr, L"릴레이 서버 Send 로그 데이터 기록 nOK : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    return (nOK == 0);
}

bool KGSDBThread::InsertProcessCPUMemoryInfo( IN const DWORD& dwServerGroup_, IN const DWORD& dwServerUniqueNum_, IN const int& nCPUUsage_, IN const KProcessMemoryInfo& kProcessMemoryInfo_ )
{
    /*
    L"{ call dbo.SRGASystemResourceCPURAM_insert_20130221 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.SRGASystemResourceCPURAM_insert_20130221 ( @01, @02, @03, @04, @05, @06, @07, @08, @09, @10, @11 ) }
    @01: @01iServerType_input    int
    0: 센터서버
    1: 게임서버
    2: 메신저라우터서버
    3: 메신저서버
    4: 에이전트서버
    5: 매칭서버
    @02: @02iServerNo_input      int
    @03: @03iCPUPercent_input    int
    CPU 사용율
    @04: @04iMemoryPercent_input int
    메모리 사용율
    @05: @05iPhyTotalSize_input  int
    물리 메모리 총 사이즈
    @06: @06iPhyAvailSize_input  int
    물리 메모리 가용 사이즈
    @07: @07iPagTotalSize_input  int
    페이징 파일 총 사이즈
    @08: @08iPagAvailSize_input  int
    페이징 파일 가용 사이즈
    @09: @09iVirTotalSize_input  int
    가상 메모리 총 사이즈
    @10: @10iVirAvailSize_input  int
    가상 메모리 가용 사이즈
    @11: @11iEVAvailSize_input   int
    확장 가상 메모리 가용 사이즈

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜잭션 에러
    */
    int nOK = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.SRGASystemResourceCPURAM_insert_20130221 ( %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
        dwServerGroup_,
        dwServerUniqueNum_,
        nCPUUsage_,
        kProcessMemoryInfo_.m_dwMemoryLoad,
        kProcessMemoryInfo_.m_dwTotalPhys,
        kProcessMemoryInfo_.m_dwAvailPhys,
        kProcessMemoryInfo_.m_dwTotalPageFile,
        kProcessMemoryInfo_.m_dwAvailPageFile,
        kProcessMemoryInfo_.m_dwTotalVirtual,
        kProcessMemoryInfo_.m_dwAvailVirtual,
        kProcessMemoryInfo_.m_dwAvailExtendedVirtual
        ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"시스템 메모리 CPU 사용량 기록" ) 
        << BUILD_LOG( nOK )
        << BUILD_LOG( dwServerGroup_ )
        << BUILD_LOG( nCPUUsage_ )
        << BUILD_LOG( dwServerUniqueNum_ )
        << BUILD_LOG( kProcessMemoryInfo_.m_dwMemoryLoad )
        << BUILD_LOG( kProcessMemoryInfo_.m_dwTotalPhys )
        << BUILD_LOG( kProcessMemoryInfo_.m_dwAvailPhys )
        << BUILD_LOG( kProcessMemoryInfo_.m_dwTotalPageFile )
        << BUILD_LOG( kProcessMemoryInfo_.m_dwAvailPageFile )
        << BUILD_LOG( kProcessMemoryInfo_.m_dwTotalVirtual )
        << BUILD_LOG( kProcessMemoryInfo_.m_dwAvailVirtual )
        << BUILD_LOG( kProcessMemoryInfo_.m_dwAvailExtendedVirtual )
        << END_LOG;

    return true;
}

bool KGSDBThread::InsertSystemHDDInfo( IN const DWORD& dwServerGroup_, IN const DWORD& dwServerUniqueNum_, IN const std::vector<KSystemHDDInfo>& vecSystemHDDInfo_ )
{
    /*
    L"{ call dbo.SRGASystemResourceHDD_insert_20130221 ( %d, %d, N'%s', %d, %d ) }"

    { call dbo.SRGASystemResourceHDD_insert_20130221 ( @01, @02, @03, @04, @05 ) }
    @01: @01iServerType_input        int
    0: 센터서버
    1: 게임서버
    2: 메신저라우터서버
    3: 메신저서버
    4: 에이전트서버
    5: 매칭서버
    @02: @02iServerNo_input          int
    @03: @03strPartitionString_input nvarchar( 50 )
    파티션 이름
    @04: @04iTotalSize_input         int
    파티션 총 사이즈
    @05: @05iAvailSize_input         int
    파티션 가용 사이즈

    1 return ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜잭션 에러
    */

    std::vector<KSystemHDDInfo>::const_iterator cvit;
    int nOK = -99;
    for( cvit = vecSystemHDDInfo_.begin(); cvit != vecSystemHDDInfo_.end(); ++cvit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.SRGASystemResourceHDD_insert_20130221 ( %d, %d, N'%s', %d, %d ) }",
            dwServerGroup_,
            dwServerUniqueNum_,
            cvit->m_wstrDiskName.c_str(),
            cvit->m_dwDiskTotalSize,
            cvit->m_dwDiskFreeSize
            ) );

        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        START_LOG( clog, L"시스템 메모리 사용량 기록" ) 
            << BUILD_LOG( nOK )
            << BUILD_LOG( dwServerGroup_ )
            << BUILD_LOG( dwServerUniqueNum_ )
            << BUILD_LOG( cvit->m_wstrDiskName )
            << BUILD_LOG( cvit->m_dwDiskTotalSize )
            << BUILD_LOG( cvit->m_dwDiskFreeSize )
            << END_LOG;
    }

    return true;
}

bool KGSDBThread::InsertSystemNetworkInfo( IN const DWORD& dwServerGroup_, IN const DWORD& dwServerUniqueNum_, IN const std::map<std::wstring, __int64>& mapNetworkInfo_ )
{
    /*
    L"{ call dbo.SRGASystemResourceNetwork_insert_20130903 ( %d, %d, N'%s', %I64d ) }"

    { call dbo.SRGASystemResourceNetwork_insert_20130903 ( @01, @02, @03, @04 ) }
    @01iServerType_input int
        0: 센터서버
        1: 게임서버
       2: 메신저라우터서버
        3: 메신저서버
        4: 에이전트서버
        5: 매칭서버
    ,   @02iServerNo_input   int
    ,   @03strString_input   nvarchar( 100 )
        네트워크 이름(100자 이내)
    ,   @04iValue_input      bigint
        바이트

    1 return ( @01 )
    @01iOK int
        0        : 성공
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
    */

    std::map<std::wstring, __int64>::const_iterator cmit;
    int nOK = -99;
    for( cmit = mapNetworkInfo_.begin(); cmit != mapNetworkInfo_.end(); ++cmit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.SRGASystemResourceNetwork_insert_20130903 ( %d, %d, N'%s', %I64d ) }",
            dwServerGroup_,
            dwServerUniqueNum_,
            cmit->first.c_str(),
            cmit->second
            ) );

        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

        START_LOG( clog, L"시스템 메모리 사용량 기록" ) 
            << BUILD_LOG( nOK )
            << BUILD_LOG( dwServerGroup_ )
            << BUILD_LOG( dwServerUniqueNum_ )
            << BUILD_LOG( cmit->first )
            << BUILD_LOG( cmit->second )
            << END_LOG;
    }

    return true;
}

void KGSDBThread::GetVirtualDepotItemInfo( IN const DWORD& dwUID_, OUT std::vector<KItem>& vecVDepotItemList_ )
{
    /*
    L"{ call dbo.UIGAUserItemTempWarehouse_select_20130905 ( %d ) }"

    { call dbo.UIGAUserItemTempWarehouse_select_20130905 ( @01 ) }
    @01iLoginUID_input int

    n return ( @01, @02, @03, @04, @05 )
    @01iItemUID  bigint
    @02iItemID   int
    @03iGradeID  tinyint
    @04iPeriod   int
    @05iDuration int
    */

    vecVDepotItemList_.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouse_select_20130905( %d ) }", dwUID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KItem kItem;
        int nGrade = 0;
        VERIFY_QUERY( m_kODBC
            >> kItem.m_ItemUID
            >> kItem.m_ItemID
            >> nGrade
            >> kItem.m_nPeriod
            >> kItem.m_nCount
            );
        
        kItem.m_cGradeID = static_cast<char>( nGrade );
        kItem.m_cInvenType = KItem::VIRTUAL_DEPOT;

        // 코디 아이템 정보 설정
        if ( SiKLookEquipment()->IsLookItemID( kItem.m_ItemID ) ) {
            kItem.AddItemType(KItem::TYPE_LOOK);
        }

        vecVDepotItemList_.push_back( kItem );
    }

    // 아이템 속성값 조회
    m_kHelper.VirtualDepotItemAttribute_select( dwUID_, vecVDepotItemList_ );
    // 아이템 소켓 정보 조회
    m_kHelper.VirtualDepotItemSocket_select( dwUID_, vecVDepotItemList_ );
    // 아이템 레벨 정보 조회
    m_kHelper.VirtualDepotLevel_select( dwUID_, vecVDepotItemList_ );
    // 아이템 강화 정보 조회
    m_kHelper.VirtualDepotStrengthInfo_select( dwUID_, vecVDepotItemList_ );
    // 아이템 날짜 정보 조회
    m_kHelper.VirtualDepotItemReceiveDate_select( dwUID_, vecVDepotItemList_ );
    // 아이템 코디 합성 정보 조회
    m_kHelper.VirtualDepotItemCooriCompose_select( dwUID_, vecVDepotItemList_ );

    START_LOG( clog, L"가상창고 아이템 가져오기" )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( vecVDepotItemList_.size() )
        << END_LOG;
}

int KGSDBThread::UpdateCountItemAtVirtualDepot( IN const DWORD& dwUID_, OUT const GCITEMUID& ItemUID_, IN const int& nDiff_, IN const int& nDelstate_ )
{
    /*
    L"{ call dbo.UIGAUserItemTempWarehouseCNT_update_20130917 ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItemTempWarehouseCNT_update_20130917 ( @01, @02, @03 ) }
        @01iLoginUID_input int
    ,   @02iItemUID_input  bigint
    ,   @03iCNT_diff_input int
        
    1 return ( @1, @2 )
        @1iOK  int
            0        : 성공
            -1       : 임시 창고 아이템이 아니거나 관련 아이템이 없음
            -2       : 수량 정보가 없거나 (보유개수-차감개수)가 0보다 작음
            -801~-899: 무시가능 에러
            -901~-999: 치명적인 에러
        @2iCNT int
            @01iOK 값이 0일 경우 유효함
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouseCNT_update_20130917 ( %d, %I64d, %d ) }",
        dwUID_, ItemUID_, nDiff_ ) );

    int nRet = -99;
    int nCnt = 1;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet >> nCnt );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

    // 가상창고에 남은 수량이 없으면 delstate 변경 (가상창고에서 제거)
    if ( 0 >= nCnt ) {
        int nOK = -99;
        DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemTempWarehouse_DelState_update_20131002 ( %d, %I64d, %d ) }",
            dwUID_, ItemUID_, nDelstate_ ) ); // nDelstate_는 여기서밖에 안쓰는데 ItemManager.h include시키기 그래서 걍 매개변수로 받음

        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

        _LOG_SUCCESS( nOK == 0, L"가상창고 수량아이템 delstate 변경.. Result : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << BUILD_LOG( nOK )
            << BUILD_LOG( nDelstate_ )
            << END_LOG;
    }

    START_LOG( clog, L"가상창고 수량 아이템 수량 갱신" )
        << BUILD_LOG( nRet )
        << BUILD_LOGc( dwUID_ )
        << BUILD_LOG( ItemUID_ )
        << BUILD_LOGc( nDiff_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << END_LOG;

    return nRet;
}

void KGSDBThread::GetTCPServerList( OUT std::vector<KSimpleServerInfo>& vecUseTCPServerList_, OUT std::vector<KSimpleServerInfo>& vecNoUseTCPServerList_ )
{
    vecUseTCPServerList_.clear();
    vecNoUseTCPServerList_.clear();

    /*
    L"{ call dbo.SMGAServerManagement_select_20130926 }"

    { call dbo.SMGAServerManagement_select_20130926 }

    n select ( @01, @02, @03, @04, @05, @06 )
        @01iUID    int            서버의 고유 번호
    ,   @02strIP   nvarchar( 15 ) 서버의 IP
    ,   @03iPort   int            서버의 Port
    ,   @04iType   int            서버의 종류
    ,   @05iStatus int            서버의 상태( 꺼질때 : 0, 켜질때 : 1 )
    ,   @06iCNT    int            접속 유저 수
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.SMGAServerManagement_select_20130926 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KSimpleServerInfo kSimpleServerInfo;

        VERIFY_QUERY( m_kODBC >> kSimpleServerInfo.m_dwUID 
            >> kSimpleServerInfo.m_strIP
            >> kSimpleServerInfo.m_usPort
            >> kSimpleServerInfo.m_cServerType
            >> kSimpleServerInfo.m_cServerStatus
            >> kSimpleServerInfo.m_nCurrentConn );

        if( kSimpleServerInfo.m_cServerStatus == KSimpleServerInfo::SS_CLOSE ) {
            vecNoUseTCPServerList_.push_back( kSimpleServerInfo );
        }
        else {
            vecUseTCPServerList_.push_back( kSimpleServerInfo );
        }
    }
    m_kODBC.EndFetch();
}

//2.	보부상 이벤트 상태 정보 변경
/*
    발렌타인 보부상 이벤트 상태정보 변경

    L"{ call dbo.SCGASocialCommerceEventInfo_SuccessFlag_update ( %d, %d ) }"

    { call dbo.SCGASocialCommerceEventInfo_SuccessFlag_update ( @1, @2) }
    @1: @iEventUID_    int
    @2: @iSuccessFlag_ int  -- 목표 달성 성공 여부 ( -1 : 실패, 0 : 진행중, 1 : 성공, 2 : 보상 지급까지 완료 )

    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 해당 이벤트가 존재하지 않음
        -101이하: 트랜젝션 에러
*/

void KGSDBThread::GetAllSellCount_SocialCommerce( OUT std::vector<PAIR_DWORD_INT>& vecPrBuyCount_ )
{
/*
    발렌타인 보부상 모든 이벤트 누적 판매 개수 조회

    L"{ call dbo.SCGASocialCommerceEventSellCount_All_Select }"

    { call dbo.SCGASocialCommerceEventSellCount_All_Select }

    1 return ( @1 )
    @1 ; EventUID    int
    @2 ; SellCount   int    --누적 판매 개수
*/
    vecPrBuyCount_.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventSellCount_All_Select }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        DWORD dwEventUID = 0;
        int nCount = 0;

        VERIFY_QUERY( m_kODBC >> dwEventUID
            >> nCount );

        vecPrBuyCount_.push_back( std::make_pair( dwEventUID, nCount ) );

        START_LOG( clog, L"DB 판매 개수 얻기")
            << BUILD_LOG( dwEventUID )
            << BUILD_LOG( nCount )
            << END_LOG;
    }

    START_LOG( cerr, L"소셜 커머스 아이템별 현재 판매 개수 얻기 : " << vecPrBuyCount_.size() ) << END_LOG;
}

int KGSDBThread::UpdateSellCount_SocialCommerce( IN const DWORD& dwUserUID_, IN const DWORD& dwEventUID_, IN const int& nSellCountDiff_ )
{
/*
    발렌타인 보부상 이벤트별 누적 판매 개수 갱신

    L"{ call dbo.SCGASocialCommerceEventSellCount_merge ( %d, %d, %d ) }"

    { call dbo.SCGASocialCommerceEventSellCount_merge ( @1, @2, @3 ) }
    @1: @iEventUID_       int
    @2: @iLoginUID_       int
    @3: @iSellCount_diff_ int
    
    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 해당 이벤트 설정이 존재하지 않음
        -101이하: 트랜젝션 에러
*/

    DBJIF2( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventSellCount_merge ( %d, %d, %d ) }",
        dwEventUID_,
        dwUserUID_,
        nSellCountDiff_) );

    int nRet = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nRet == 0, L"소셜커머스 구매 수량 DB에 갱신.. Result : " << nRet )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( dwEventUID_ )
        << BUILD_LOG( nSellCountDiff_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) 
        << END_LOG;

    return nRet;
}

bool KGSDBThread::InsertGroupPurchaseUser_SocialCommerce( IN const DWORD& dwEventUID_, IN const DWORD& dwUserUID_ )
{
/*
    발렌타인 보부상 이벤트 공동구매 유저 리스트 입력

    L"{ call dbo.SCGASocialCommerceEventGroupPurchaseList_Insert ( %d, %d ) }"

    { call dbo.SCGASocialCommerceEventGroupPurchaseList_Insert ( @1, @2 ) }
    @1: @iEventUID_  int
    @2: @iLoginUID_  int
        
    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저가 없음
        -101이하: 트랜젝션 에러
*/


    DBJIF2( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventGroupPurchaseList_Insert ( %d, %d ) }",
        dwEventUID_,
        dwUserUID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"소셜커머스 공동구매 신청 유저 DB 기록.. Result : " << nOK )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( dwEventUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) 
        << END_LOG;

    return ( nOK == 0 );
}

void KGSDBThread::UpdateBuyLog_SocialCommerce( IN const DWORD& dwEventUID_, IN const int& nType_, IN const DWORD& dwUserUID_, IN const GCITEMID& dwItemID_ )
{
/*
    발렌타인 보부상 이벤트 아이템 구매 로그 기록

    L"{ call dbo.SCGASocialCommerceEventBuyItemList_insert ( %d, %d, %d, %d ) }"

    { call dbo.SCGASocialCommerceEventBuyItemList_insert ( @1, @2, @3, @4 ) }
    @1: @iEventUID_   int
    @2: @iEventType_  int
    @3: @iLoginUID_   int
    @4: @iItemID_     int
        
    return 값 없음
    
*/

    DBJIF( m_kODBC.Query( L"{ call dbo.SCGASocialCommerceEventBuyItemList_insert ( %d, %d, %d, %d ) }",
        dwEventUID_, nType_, dwUserUID_, dwItemID_) );
}

bool KGSDBThread::InsertUserUDPPortInfo( IN const KDB_EVENT_USER_UDP_PORT_INFO_REQ& vecList_ )
{
    /*
    L"{ call dbo.CUSLClientUDPHolePunching_insert_20131028 ( %d, %d, N'%s', %d, %d ) }"

    { call dbo.CUSLClientUDPHolePunching_insert_20131028 ( @01, @02, @03, @04, @05 ) }
    @01iLoginUID_input int
   ,@02iModeID_input   int
   ,@03strIP_input     nvarchar( 15 )
   ,@04iPort_input     int
   ,@05iPartyCNT_input int

    1 select ( @01 )
    @01iOK int
    0        : 성공
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    -1000    : 사용 중지된 sp인데 사용하려 함
    */

    bool bSuccess = true;
    KDB_EVENT_USER_UDP_PORT_INFO_REQ::const_iterator cvit;
    for ( cvit = vecList_.begin() ; cvit != vecList_.end() ; ++cvit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.CUSLClientUDPHolePunching_insert_20131028 ( %d, %d, N'%s', %d, %d ) }",
            cvit->m_dwUserUID,
            cvit->m_nModeID,
            cvit->m_strIP.c_str(),
            cvit->m_usUDPUsePort,
            cvit->m_nPartyUserNum ) );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

        _LOG_SUCCESS( nRet == 0, L"유저 UDP 포트 정보 통계 DB기록.. Result : " << nRet )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

        if ( 0 != nRet ) {
            bSuccess = false;
        }
    }

    return bSuccess;
}


void KGSDBThread::GetUserControlRecordData( IN const DWORD dwUID_, IN const char& cCharType_, IN OUT std::map<int,KDungeonUserInfo>& mapList_ )
{
    /*
    L"{ call dbo.CFGAControlFeedbackSystem_select ( %d, %d ) }"
     
    { call dbo.CFGAControlFeedbackSystem_select ( @1, @2 ) }
    @1 ; @iLoginUID_    int
    @2 ; @iCharType_    tinyint
     
    n return ( @1, @2, @3, @4 )
    @1 ; ModeID         int     //던전번호
    @2 ; Difficult      int     //던전난이도
    @3 ; BestRank       int     //베스트클리어랭크//F = 0, E = 1, D = 2, C = 3, B = 4, A = 5, S = 6, SS = 7
    @4 ; BestPlayTime   int     //최단시간클리어타임(초)
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.CFGAControlFeedbackSystem_select ( %d, %d ) }", dwUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KUsersDungeonRecordData kRecord;
        int nModeID = 0;
        int nDifficult = 0;

        VERIFY_QUERY( m_kODBC >> nModeID
            >> nDifficult
            >> kRecord.m_nPrevBestClearRank
            >> kRecord.m_dwPrevBestClearTime );

        kRecord.m_nBestClearRank = kRecord.m_nPrevBestClearRank;
        kRecord.m_dwBestClearTime = kRecord.m_dwPrevBestClearTime;



        // find로 집어넣는 이유는 혹시나 클리어데이터랑 일치 하지 않아서 꼬일까봐
        std::map<int,KDungeonUserInfo>::iterator mitMode;
        mitMode = mapList_.find( nModeID );
        if ( mapList_.end() != mitMode ) {
            // mitMode는 클리어 한 ModeID 정보를 찾은 상태. 해당 모드에 레코드 데이터를 난이도별로 집어 넣는다
            mitMode->second.m_kClearData.m_mapBestRecord.insert( std::make_pair( nDifficult, kRecord ) );
        }
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"DB에서 던전/난이도별 유저 컨트롤 레코드 데이터 얻음 : " << mapList_.size() ) << END_LOG;
}

bool KGSDBThread::UpdateUserControlRecordData( IN const DWORD dwUID_, IN const char& cCharType_, IN const int& nModeID_, IN const SHORT shDifficulty_,
                                              IN KUsersDungeonRecordData& kRecordData_ )
{
    /*
    L"{ call dbo.CFGAControlFeedbackSystem_merge ( %d, %d, %d, %d, %d, %d ) }"
     
    { call dbo.CFGAControlFeedbackSystem_merge ( @1, @2, @3, @4, @5, @6 ) }
    @1 ; @iLoginUID_     int
    @2 ; @iCharType_     tinyint
    @3 ; @iModeID_       int     //던전번호
    @4 ; @iDifficult_    int     //던전난이도
    @5 ; @iBestRank_     int     //베스트클리어랭크//F = 0, E = 1, D = 2, C = 3, B = 4, A = 5, S = 6, SS = 7
    @6 ; @iBestPlayTime_ int     //최단시간클리어타임(초)
     
    1 return ( @1 )
    @1: OK       int
        0       : 성공
        -1      : 유저/케릭터없음
        -801    ; 로그기록실패
        -901    ; 데이터등록실패
        -902    ; 서버에서전달해준BestRank 값이더낮음.
        -903    ; 서버에서전달해준BestPlayTime 값이더높음.
        -904    ; 데이터갱신실패
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.CFGAControlFeedbackSystem_merge ( %d, %d, %d, %d, %d, %d ) }", 
        dwUID_, cCharType_, nModeID_, shDifficulty_, kRecordData_.m_nBestClearRank, kRecordData_.m_dwBestClearTime ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if ( 0 == nOK ) {
        kRecordData_.m_bClearRankChanged = false;
        kRecordData_.m_bClearTimeChanged = false;
    }

    _LOG_SUCCESS( nOK == 0, L"던전(난이도별) 최고 기록 저장결과.. " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )  << END_LOG;

    return (nOK == 0);
}

bool KGSDBThread::CheckItemDuration( IN const DWORD& dwUID_, IN const GCITEMUID& ItemUID_, IN const int& nInitCount_, IN const int& nCount_ )
{
    int nDiffCount = 0;
    if( nInitCount_ == nCount_ ) {
        return true;
    }

    nDiffCount = nCount_ - nInitCount_;
    if( nDiffCount > 0 ) {
        LIF( AddCountItem( dwUID_, ItemUID_, nDiffCount ) );
    } else {
        LIF( UseCountItem( dwUID_, ItemUID_, -nDiffCount ) );
    }
    return false;
}

bool KGSDBThread::InsertPresentPost( IN const std::wstring strSendNickName_, IN const DWORD dwReceiveUID_, IN const KDB_EVENT_BUY_VIRTUAL_CASH_REQ& kPacket_ )
{
    USHORT usTitleLength = SiKPostConfig()->GetTitleLength();
    USHORT usMsgLength = SiKPostConfig()->GetMessageLength();

    // 우편 메세지 만들기
    KPostItemInfo kInfo;
    kInfo.m_dwFromUserUID = kPacket_.m_kUserData.m_dwLoginUID;
    kInfo.m_dwToUserUID = dwReceiveUID_;
    kInfo.m_cScriptType = KPostItemInfo::LT_USER;
    kInfo.m_nGamePoint = 0;
    kInfo.m_vecItem.clear();

    kInfo.m_strFromNickName = strSendNickName_;
    SiKPostConfig()->GetPostPresentString(L"str_05", strSendNickName_, kInfo.m_strTitle );
    kInfo.m_strMessage = kPacket_.m_strComment;
    kInfo.m_cCharType = -1;

    KItem kItem;
    kItem.m_ItemID = kPacket_.m_kBuyInfo.m_ItemID;
    kItem.m_nCount = kPacket_.m_kBuyInfo.m_nCount;
    kItem.m_nPeriod = kPacket_.m_kBuyInfo.m_nPeriod;
    kItem.m_cEnchantLevel = 0;
    kItem.m_cGradeID = -1;
    kInfo.m_vecItem.push_back( kItem );

    // 우편 지급
    if ( false == InsertSystemPost( kInfo ) ) {
        START_LOG( cerr, L"선물하기 우편 DB처리 실패.. LoginID : " << dwReceiveUID_ << " ItemID : " << kPacket_.m_kBuyInfo.m_ItemID ) 
            << END_LOG;
        return false;
    }

    return true;
}

void KGSDBThread::GetFinishedMissionList( IN const DWORD dwUID_, IN const char& cCharType_, OUT std::vector< DWORD >& vecFinishedMission_ )
{
    /*
    L"{ call dbo.MSGAMissionSystemTagHuntingGround2013121201_select_20131212 ( %d, %d ) }"

    { call dbo.MSGAMissionSystemTagHuntingGround2013121201_select_20131212 ( @01, @02 ) }
        @01iLoginUID_input int
    ,   @02iCharType_input int

    n return ( @01 )
    @01: MainID int
    */

    vecFinishedMission_.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.MSGAMissionSystemTagHuntingGround2013121201_select_20131212 ( %d, %d ) }",
        dwUID_, cCharType_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        DWORD dwMissionID = 0;

        VERIFY_QUERY( m_kODBC >> dwMissionID );

        if ( 0 != dwMissionID ) {
            vecFinishedMission_.push_back( dwMissionID );
        }
    }

    START_LOG( clog, L"DB에서 완료 한 미션 목록 얻기 결과 : " << vecFinishedMission_.size() ) << END_LOG;
}

bool KGSDBThread::InsertFinishedMission( IN const DWORD dwUID_, IN const char& cCharType_, IN const DWORD& dwMissionID_ )
{
    /*
    L"{ call dbo.MSGAMissionSystemTagHuntingGround2013121201_insert_20131212 ( %d, %d, %d ) }"

    { call dbo.MSGAMissionSystemTagHuntingGround2013121201_insert_20131212 ( @01, @02, @03 ) }
        @01iLoginUID_input int
    ,   @02iCharType_input int
    ,   @03iMainID_input   int

    1 return ( @01 )
        @01iOK int
            0        : 성공
            -1       : 유저 정보 없음
           -2       : 캐릭터 정보 없음
            -3       : 이미 완료된 미션
            -801~-899: 무시가능 에러
            -901~-999: 치명적인 에러
    */
    DBJIF2( m_kODBC.Query( L"{ call dbo.MSGAMissionSystemTagHuntingGround2013121201_insert_20131212 ( %d, %d, %d ) }", 
        dwUID_, cCharType_, dwMissionID_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    _LOG_SUCCESS( nOK == 0, L"미션 클리어 시 일반/에픽 미션 클리어 정보 DB 기록 결과 : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery )  << END_LOG;

    return (nOK == 0);
}

bool KGSDBThread::Insert_PvPPlayCharactersCount_ToDB( IN const KEVENT_STAT_PVP_PLAY_CHARACTER_COUNT_NOT& kPacket_ )
{
    /*
    L"{ call dbo.MCGAMatchUpCharType_insert ( N'%s', %d, %d, %d, %d, %d, %d, %d, %d ) }"

    { call dbo.MCGAMatchUpCharType_insert ( @1, @2, @3, @4, @5, @6, @7, @8, @9 ) }
    @1 ;  RegDateA     datetime    --던전 시작 시간
    @2 ;  ModeID       int         --모드id
    @3 ;  Difficult    int         --모드 난이도
    @4 ;  CharType1    int
    @5 ;  CharType2    int
    @6 ;  CharType3    int = null
    @7 ;  CharType4    int = null
    @8 ;  CharType5    int = null
    @9 ;  CharType6    int = null

    1 select ( @1 )
    @1  ; OK int
        0        ; 성공
        -901     ; 로그 기록 실패
    */

    bool bDBWriteSuccess = true;
    BOOST_FOREACH( const KStatPvPPlayCharacterInfo& rStat, kPacket_ ) {
        if ( 2 > rStat.m_vecCharTypes.size() ) { // 플레이 인원 수가 2명 이상인 경우에만 DB에 기록한다
            continue;
        }

        std::wstring strDate( static_cast< LPCTSTR >( CTime( rStat.m_tmStartTime ).Format( _T( "%Y%m%d %H:%M:%S" ) ) ) );

        std::wstringstream stm;
        stm.clear();
        stm.str( L"" );

        stm << L"{ call dbo.MCGAMatchUpCharType_insert ( ";
        stm << L"N'" << strDate.c_str() << L"', ";
        stm << rStat.m_nModeID << L", ";
        stm << L"0";

        std::vector< int >::const_iterator cvit( rStat.m_vecCharTypes.begin() );
        for ( ; rStat.m_vecCharTypes.end() != cvit; ++cvit ) {
            stm << L", " << *cvit;
        }

        stm << L" ) }";

        DBJIF2( m_kODBC.QueryToWString( stm.str() ) );

        stm.clear();
        stm.str( L"" );

        int nRet = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nRet );
            m_kODBC.EndFetch();
        }

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

        if ( 0 != nRet ) {
            bDBWriteSuccess = false;
            START_LOG( cerr, L"대전 플레이 캐릭터 타입 통계 기록 DB 저장 실패" )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        }

    }

    _LOG_SUCCESS( true == bDBWriteSuccess, L"대전 플레이 캐릭터 타입 통계 기록" )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return bDBWriteSuccess;
}

bool KGSDBThread::UpdateGachaPonLog( IN const DWORD& dwUID_, IN const char& cCharType_, IN const DWORD& dwMachineNumber, IN const DWORD& MainRewardItem, IN const DWORD& dwRewardItemID, IN const int& nCapsule, IN const int& nCapsulesInMachine )
{
    DBJIF2(m_kODBC.Query(L"{ call dbo.GPGAGachaPonInsert ( %d, %d, %d, %d, %d, %d, %d ) }",
        dwUID_,
        static_cast<int>(cCharType_),
        dwMachineNumber,
        MainRewardItem,
        dwRewardItemID,
        nCapsule,
        nCapsulesInMachine));

    int nRet = -99;
    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nRet);
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS(nRet == 0, L"Gacha Pon Log insert result." << nRet)
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    return true;
}

bool KGSDBThread::GetPetGlyph( IN const DWORD dwLoginUID, IN const char cCharType, OUT std::vector<KPetGlyphCharData>& vecPetGlyph )
{
    START_LOG(clog, L"KGSDBThread::GetPetGlyph")
        << BUILD_LOG(dwLoginUID)
        << BUILD_LOGc(cCharType)
        << END_LOG;

    DBJIF2(m_kODBC.Query(L"{ call dbo.PGGAPetGlyphCharacterSelect ( %d ) }", dwLoginUID));

    while (m_kODBC.Fetch() != SQL_NO_DATA)
    {
        KPetGlyphCharData _data;
        VERIFY_QUERY(m_kODBC
            >> _data.m_dwUID
            >> _data.m_dwID
            >> _data.m_cType);

        vecPetGlyph.push_back(_data);
    }

    return true;
}

bool KGSDBThread::EquipPetGlyph( IN const DWORD dwLoginUID, IN const char cCharType, IN const GCITEMUID dwItemUID, IN const GCITEMID dwItemID, IN const int iGlyphType )
{
    START_LOG(clog, L"KGSDBThread::EquipPetGlyph")
        << BUILD_LOG(dwLoginUID)
        << BUILD_LOGc(cCharType)
        << BUILD_LOG(dwItemUID)
        << BUILD_LOG(dwItemID)
        << BUILD_LOG(iGlyphType)
        << END_LOG;

    if (dwItemUID == 0 || dwItemID == 0)
    {
        return false;
    }

    DBJIF2(m_kODBC.Query(L"{ call dbo.PGGAPetGlyphEquip ( %d, %I64d, %d, %d ) }", dwLoginUID, dwItemUID, dwItemID, iGlyphType));

    int nRet = -99;
    if (m_kODBC.BeginFetch())
    {
        VERIFY_QUERY(m_kODBC >> nRet);
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS(nRet == 0, L"Pet Glyph Equip Result: " << nRet)
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    return nRet == 0;
}

bool KGSDBThread::RemovePetGlyph( IN const DWORD dwLoginUID, IN const char cCharType, IN const GCITEMUID dwItemUID )
{
    if (dwItemUID == 0)
    {
        return false;
    }

    DBJIF2(m_kODBC.Query(L"{ call dbo.PGGAPetGlyphRemove ( %d, %I64d ) }",
        dwLoginUID,
        dwItemUID));

    int nRet = -99;
    if (m_kODBC.BeginFetch())
    {
        VERIFY_QUERY(m_kODBC >> nRet);
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS(nRet == 0, L"Pet Glyph Remove Result: " << nRet)
        << BUILD_LOG(m_kODBC.m_strLastQuery)
        << END_LOG;

    return nRet == 0;
}

bool KGSDBThread::GetCoordiViewerCharSlotItems(IN const int iLoginUID, IN const int iCharType, OUT std::map< int, std::vector< GCITEMID > >& m_mapSlotInfo)
{
    GCITEMID TempItem;
    std::vector< GCITEMID > TempItemVector;
    TempItemVector.clear();

    for (int i = 0; i < 3; i++)
    {
        DBJIF2(m_kODBC.Query(L"{ call dbo.LUMACoordiviewer_Get_Slot_Equip ( %d, %d, %d ) }", iLoginUID, i, iCharType));
        while (m_kODBC.Fetch() != SQL_NO_DATA)
        {
            VERIFY_QUERY(m_kODBC >> TempItem);
            TempItemVector.push_back(TempItem);
        }

        m_mapSlotInfo[i] = TempItemVector;
        TempItemVector.clear();
    }

    START_LOG(clog, L"Coordiview Slot Equip retreieved! UID: " << iLoginUID << L", CharType: " << iCharType << L", Map size: " << m_mapSlotInfo.size()) << BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

    return true;
}

bool KGSDBThread::SetCoordiViewerCharSlotItems(IN const int iLoginUID, IN const int iSlotID, IN const int iCharType, IN std::vector< GCITEMID >& m_vecItemList, OUT int iOK)
{
    std::wstringstream stm;
    stm.clear();
    stm.str(L"");
    for (auto& element : m_vecItemList)
    {
        if (stm.str().size() == 0)
        {
            stm << L"{ call dbo.LUMACoordiViewer_Slot_Merge ( ";
            stm << static_cast<int>(iLoginUID) << L",";
            stm << static_cast<int>(iCharType) << L",";
            stm << static_cast<int>(iSlotID);
        }

        if (element > 0)
        {
            stm << L"," << static_cast<__int64>(element);
        }
    }

    if (stm.str().size() != 0)
    {
        stm << L") }";
        DBJIF2(m_kODBC.QueryToWString(stm.str()));

        if (m_kODBC.BeginFetch())
        {
            int iOK = -99;
            VERIFY_QUERY(m_kODBC >> iOK);
            m_kODBC.EndFetch();

            if (iOK != 0)
                START_LOG(clog, L"Fail to insert Coordiview slot! OK : " << iOK << L", UID : " << iLoginUID << L", CharType : " << iCharType << L", SlotID : " << iSlotID << L", ItemVector Size : " << m_vecItemList.size() << END_LOG);
        }
        stm.clear();
        stm.str(L"");
    }
    return true;
}

void KGSDBThread::UpdateBillboardLog(IN const DWORD& dwUID_, IN const std::wstring& strNickname_, IN const std::wstring& strMessage_)
{
    DBJIF(m_kODBC.Query(L"{ call dbo.UIMOUserItemBillboardUsage_insert ( %d, N'%s', N'%s' ) }",
        dwUID_, strNickname_.data(), strMessage_.data()));
}


bool KGSDBThread::UpdateUserHeroDungeonTicket( IN const DWORD dwUserUID_, IN const char cCharType_, IN const int nModeID_ )
{
    int nRet = -99;

    DBJIF2( m_kODBC.Query( L"{ call dbo.DCGADungeonClearHeroTicket ( %d, %d, %d ) }",
        dwUserUID_, cCharType_, nModeID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nRet );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( nRet == 0, L"Set User HeroDungeon Ticket.. OK : " << nRet )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( 0 == nRet );
}