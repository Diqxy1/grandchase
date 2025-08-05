#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include "GSSimLayer.h"
#include "User.h"
#include <boost/bind.hpp>
#include "FatigueSystem.h"
#include <boost/foreach.hpp>
#include "SkillManager.h"
#include "MissionInfo.h"
#include "MissionManager.h"
#include "LoudMessage.h"
#include "Room.h"
#include "ItemManager.h"
#include "GSDBLayer.h"
#include "ResultManager.h"
#include "PeriodNotice.h"
#include "CoupleSystem.h"
#include "EnchantStrong.h"
#include "GameServer.h"
#include "GameCouponManager.h"
#include "RecomHelper.h"
#include "GCHelper.h"
#include <boost/algorithm/string.hpp> // for replace_all
#include "GachaManager.h"
#include "RankManager.h"
#include "QuizBingo.h"
#include "DicePlay.h"
#include "IPAdvantage.h"
#include "EventType.h"
#include "GCPoint.h"
#include "Gawibawibo.h"
#include "PostConfig.h"

#include "GuildManager.h"
#include "ErrandManager.h"
#define CLASS_TYPE KGSDBThread

int KGSDBThread::TranslateNicknameToLogin( IN const std::wstring& strNickName, OUT std::wstring& strName )
{
    //이미 Lock이 걸려 있음.
    int iOK = -1;

    if( !m_kODBC.Query( L"{ call dbo.up_translate_nickname_to_login ( N'%s' ) }",
        strNickName.c_str() ) )
    {
        return iOK;
    }

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> iOK );

        if( iOK == 0 )
        {
            VERIFY_QUERY( m_kODBC >> strName );
        }

        m_kODBC.EndFetch();
    }

    return iOK;
}

int KGSDBThread::GetNickName( const std::wstring& strName, std::wstring& strNickName )
{
    //이미 Lock이 걸려 있음.
    int iOK = -1;

    if( !m_kODBC.Query( L"{ call dbo.up_get_nickname ( N'%s' ) }",
        strName.c_str() ) )
    {
        return iOK;
    }

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> iOK );

        if( iOK == 0 )
        {
            VERIFY_QUERY( m_kODBC >> strNickName );
        }

        m_kODBC.EndFetch();
    }

    return iOK;
}

bool KGSDBThread::UpdateUserData( KUserData& kUserData, bool bReturn_ )
{
    START_LOG( clog, L"사용자 데이터 업데이트" )
        << BUILD_LOG( kUserData.m_strLogin.c_str() )
        << BUILD_LOG( kUserData.m_iInitGP )
        << BUILD_LOG( kUserData.m_iGamePoint )
        << BUILD_LOGc( kUserData.m_cGrade )
        << BUILD_LOG( kUserData.m_dwGuildUID )
        << BUILD_LOG( kUserData.m_dwGuildContPoint )
        << BUILD_LOG( kUserData.m_nIncGpSum )
        << BUILD_LOG( kUserData.m_nDecGpSum )
        << BUILD_LOGc( kUserData.m_cCharType )
        << END_LOG;

    if( kUserData.m_mapCharacterInfo.size() > GC_CHAR_NUM ) {
        START_LOG( cerr, L"캐릭터 수가 최대 캐릭터 수를 초과하여 로그 출력" )
            << BUILD_LOG( kUserData.m_mapCharacterInfo.size() )
            << BUILD_LOG( kUserData.m_strLogin.c_str() )
            << BUILD_LOG( kUserData.m_iInitGP )
            << BUILD_LOG( kUserData.m_iGamePoint )
            << BUILD_LOGc( kUserData.m_cGrade )
            << BUILD_LOG( kUserData.m_dwGuildUID )
            << BUILD_LOG( kUserData.m_dwGuildContPoint )
            << BUILD_LOG( kUserData.m_nIncGpSum )
            << BUILD_LOG( kUserData.m_nDecGpSum )
            << BUILD_LOGc( kUserData.m_cCharType )
            << END_LOG;
    }

    KLocker lock( m_kODBC.GetCS() );

    // GamePoint Update.
    int nOK = -1;
    int nGP = 0;

    UpdateUserGamePoint( kUserData.m_dwLoginUID, kUserData.m_cCharType, kUserData.m_dwSlotNum, kUserData.m_iGamePoint - kUserData.m_iInitGP, nOK, nGP );

    _LOG_SUCCESS( nOK == 0, L"UpdateUserData() 안에서 UpdateUserGamePoint 기록 상태 " )
        << BUILD_LOG( kUserData.m_dwLoginUID )
        << BUILD_LOGc( kUserData.m_cCharType )
        << BUILD_LOG( nGP )
        << END_LOG;

    // 실제 플레이중인 캐릭터에 대해서만 DB와 동기화를 진행한다.
    std::map<char, KCharacterInfo>::iterator mit;
    mit = kUserData.m_mapCharacterInfo.find( kUserData.m_cCharType );
    if( mit != kUserData.m_mapCharacterInfo.end() ) {
        UpdateBonusLife( kUserData.m_dwLoginUID, mit->second.m_cCharType, mit->second.m_dwSlotNum, mit->second.m_kBonusPoint );
    }

    // 포인트백 정보 저장.
    CTime tmCurrent = CTime::GetCurrentTime();
    std::wstring strDate = (LPCTSTR)tmCurrent.Format( _T("%y%m%d") );
    LIF( UpdateUserPointBagAccTime( kUserData.m_dwLoginUID, kUserData.m_cCharType, kUserData.m_nDiffGCPoint, kUserData.m_nAccTime, strDate ) );
    UpdateEquipSkillSet( kUserData.m_dwLoginUID, kUserData.m_mapCharacterInfo );

    return true;
}

bool KGSDBThread::UpdateUserData( KDB_EVENT_EXIT_GAME_NOT& kUserDataEx )
{
    //해킹 감지...
    //call dbo.sup_hacking_info_insert (LoginUID int, Count int)
    //return @iOK
    //@iOK : 0(성공), -1(유저 정보가 없음), -11이하(트랜잭션 에러)
    if( kUserDataEx.m_nHackReport > 0 ) {
        if( m_kODBCStat.Query( L"{ call dbo.sup_hacking_info_insert ( %d, %d ) }",
            kUserDataEx.m_dwUserUID, kUserDataEx.m_nHackReport ) ) {
            if ( m_kODBCStat.BeginFetch() ) {
                int nOK;
                VERIFY_QUERY( m_kODBCStat >> nOK );
                LOG_SUCCESS( nOK == 0 )
                    << L"해킹 리포트 저장 실패.." << dbg::endl
                    << BUILD_LOG( nOK )
                    << BUILD_LOG( kUserDataEx.m_dwUserUID )
                    << BUILD_LOG( kUserDataEx.m_nHackReport ) << END_LOG;
            }
            m_kODBCStat.EndFetch();
        }
    }

    if ( !UpdateUserData( kUserDataEx.m_kUserData ) ) {
        START_LOG( cerr, L"유저가 게임을 종료하기 전 DB 기록 못함." )
            << BUILD_LOG( kUserDataEx.m_kUserData.m_strLogin )
            << END_LOG;
        return false;
    }

    { // 유저 로그아웃 & 서버이동시에만 저장
        const std::map<char, KCharacterInfo>& mapCharacterInfo = kUserDataEx.m_kUserData.m_mapCharacterInfo;
        std::map<char, KCharacterInfo>::const_iterator mitChar;
        for ( mitChar =  mapCharacterInfo.begin() ; mitChar != mapCharacterInfo.end() ; ++mitChar ) {
            VerifyCharExpForStat( kUserDataEx.m_kUserData.m_strLogin, kUserDataEx.m_dwUserUID, mitChar->second );
        }        
    }

    KLocker lock( m_kODBC.GetCS() );

    //050729. microcat. users테이블에 접속여부 제거
    //    DBJIF2( m_kODBC.Query("UPDATE users WITH (ROWLOCK) SET Connecting = 0 WHERE Login = \'%s\'",
    DBJIF2( m_kODBC.Query( L"{ call dbo.up_update_disconnect_info ( N'%s' ) }",
        kUserDataEx.m_kUserData.m_strLogin.c_str() ) );

    // 장착 정보 업데이트.
    LIF( UpdateUserEquipInfo( kUserDataEx.m_kUserData.m_dwLoginUID, kUserDataEx.m_kUserData.m_mapCharacterInfo, kUserDataEx.m_kUserData.m_mapChangeWeapon, kUserDataEx.m_setChangeEquipInfo ) );

    {// 060729. kkurrung. 상하이 체크 포인트
        //{ call dbo.up_insert_check_list ( @iLoginUID, @vbList, [@bModify] ) }
        //@iLoginUID : 유저의 LoginUID
        //@vbList : 48바이트짜리 바이너리
        //@bModify : 0일 경우 insert, 1일 경우 update를 함. [기본값은 0]

        KSerializer     ks;
        KSerBuffer      kbuff;
        ks.BeginWriting( &kbuff );
        ks.Put( kUserDataEx.m_bitCheckPoint );
        ks.EndWriting();

        if( m_kODBC.QueryWithBlob( kbuff, L"{ call dbo.up_insert_check_list ( %d, ?, 1 ) }",
            kUserDataEx.m_dwUserUID ) )
        {
            int nOK;
            if( m_kODBC.BeginFetch() )
            {
                VERIFY_QUERY( m_kODBC >> nOK );
            }
            m_kODBC.EndFetch();
        }
        else
        {
            START_LOG( cerr, L"up_insert_check_list" )
                << BUILD_LOG( m_kODBC.m_szDiagMsg )
                << BUILD_LOG( (wchar_t*)m_kODBC.m_sqlState ) << END_LOG;
        }
    }

    if ( !kUserDataEx.m_vecDurationItemInv.empty() ) {
        LIF( UpdateCountItems( kUserDataEx.m_dwUserUID, kUserDataEx.m_vecDurationItemInv ) );
    }

    // 수량 아이템 기록후 펫 관련 데이터 처리한다..
    if ( !UpdatePetData( kUserDataEx.m_mapPetInfo, kUserDataEx.m_kUserData.m_mapCharacterInfo, kUserDataEx.m_dwUserUID, kUserDataEx.m_kUserData.m_strLogin ) )
    {
        START_LOG( cerr, L"게임서버/DB간 펫 정보 동기화 실패." )
            << BUILD_LOG( kUserDataEx.m_kUserData.m_strLogin )
            << END_LOG;
    }

    if ( !UpdateDepotPetData( kUserDataEx.m_mapDepotPetInfo, kUserDataEx.m_kUserData.m_mapCharacterInfo, kUserDataEx.m_dwUserUID, kUserDataEx.m_kUserData.m_strLogin ) )
    {
        START_LOG( cerr, L"게임서버/DB간 창고 펫 정보 동기화 실패." )
            << BUILD_LOG( kUserDataEx.m_kUserData.m_strLogin )
            << END_LOG;
    }

    if ( SiKGSSimLayer()->m_bRecommendEnable )
    {

        DBJIF2( m_kODBC.Query( L"{ call dbo.up_recommender_check_status ( %d ) }",
            kUserDataEx.m_dwUserUID ) );
        int nOK;
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        if( nOK == 0 )
        {
            DBJIF2( m_kODBC.Query( L"{ call dbo.up_recommender_update ( %d, %d ) }",
                kUserDataEx.m_dwUserUID, kUserDataEx.m_dwPlayTime ) );
            if( m_kODBC.BeginFetch() )
            {
                VERIFY_QUERY( m_kODBC >> nOK );
            }
            m_kODBC.EndFetch();
        }

        START_LOG( clog, L"추천 포인트 등록 UID : " << kUserDataEx.m_dwUserUID << L", Time : " <<  kUserDataEx.m_dwPlayTime << L", OK : " << nOK ) << END_LOG;
    }

    //게임 제한이 있는경우
    if( SiKFatigueSystem()->IsRun() )
    {
        //call dbo.up_LimitGamePlay_OffLine @iLoginUID int, @iPlayTime int
        //리턴값 select return @iOK
        //0(성공)
        //-11이하(트랜잭션 에러)

        int nOK = -10;
        DBJIF2( m_kODBC.Query( L"{ call dbo.up_LimitGamePlay_OffLine ( %d, %d ) }",
            kUserDataEx.m_dwUserUID, kUserDataEx.m_dwPlayTime ) );
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        if( nOK != 0 )
        {
            START_LOG( cerr, L"게임 제한 시스템 오류 : 누적 온라인 시간 저장 실패" )
                << BUILD_LOG( kUserDataEx.m_dwUserUID )
                << BUILD_LOG( kUserDataEx.m_dwPlayTime ) << END_LOG;
        }

    }

    //--유저의 달력 정보를 갱신
    if( SiKGCPoint()->IsGCPointSwitch() == true ) { // GCPoint 적용되었을 때에만 작동
        LIF( UpdatePointbagCalender( kUserDataEx.m_dwUserUID, kUserDataEx.m_kUserData.m_cCurrentChar, kUserDataEx.m_vecCalendar ) );
    }

    //  미션 정보 업데이트
    UpdateMissionData( kUserDataEx.m_dwUserUID, kUserDataEx.m_kUserData.m_cCurrentChar, kUserDataEx.m_vecMissionSlot );

    // 퀵슬롯 저장, 아이템이 유효한지 체크하는 부분
    std::map< char, KQuickSlot >::iterator mit;
    for( mit = kUserDataEx.m_mapQuickSlot.begin(); mit != kUserDataEx.m_mapQuickSlot.end(); ++mit ) {
        std::vector< KUserEquipItem > vecUserEquipItem;
        vecUserEquipItem.clear();

        std::vector< KSlotData >::iterator vit;
        for( vit = mit->second.m_vecItemSlot.begin() ; vit != mit->second.m_vecItemSlot.end() ; ++vit ) {
            if ( SiKIPAdvantage()->GetMaximumPCBangItemUID() >= vit->m_kItem.m_dwUID ) {
                continue;
            }
            UpdateEquipItem( kUserDataEx.m_kUserData.m_strLogin, kUserDataEx.m_dwUserUID, mit->first, vit->m_kItem.m_dwUID, vit->m_kItem.m_dwID, vit->m_nSlotIndex, vit->m_kItem.m_cItemType, vecUserEquipItem, KGSDBThread::ST_ITEM );
        }

        for( vit = mit->second.m_vecEmoticonSlot.begin() ; vit != mit->second.m_vecEmoticonSlot.end() ; ++vit ) {
            if( SiKIPAdvantage()->GetMaximumPCBangItemUID() >= vit->m_kItem.m_dwUID ) {
                continue;
            }

            UpdateEquipItem( kUserDataEx.m_kUserData.m_strLogin, kUserDataEx.m_dwUserUID, mit->first, vit->m_kItem.m_dwUID, vit->m_kItem.m_dwID, vit->m_nSlotIndex, vit->m_kItem.m_cItemType, vecUserEquipItem, KGSDBThread::ST_EMOTICON );
        }

        int nOK = UpdateCharQuickSlotDelete( kUserDataEx.m_dwUserUID, mit->first );

        std::vector< KUserEquipItem >::const_iterator cvit;
        for ( cvit = vecUserEquipItem.begin() ; cvit != vecUserEquipItem.end() ; ++cvit ) {
            if( nOK == 0 ) {
                UpdateCharQuickSlotMerge( kUserDataEx.m_dwUserUID, mit->first, cvit->m_nItemOrderNo, cvit->m_ItemUID, cvit->m_nItemID );
            } else {
                START_LOG( clog, L"퀵 슬롯 기록 실패3 OK : " << nOK )
                    << BUILD_LOG( kUserDataEx.m_kUserData.m_strLogin ) << END_LOG;
            }
        }
    }

    //유저 종료시 칭호 미션 기록   080526  woosh. 
    if( !kUserDataEx.m_vecCollectionSlot.empty() ) {
        UpdateCollectionData( kUserDataEx.m_dwUserUID, kUserDataEx.m_kUserData.m_cCurrentChar, kUserDataEx.m_vecCollectionSlot );
    }

    { // 스킬 학습 제거
        std::set<int>::iterator sit;
        for( sit = kUserDataEx.m_setAddReserveSkill.begin() ; sit != kUserDataEx.m_setAddReserveSkill.end() ; ++sit )
        {
            SSkillElement sSkill;
            if( !SiKSkillManager()->GetSkill( *sit, sSkill ) ) continue;
            LIF( m_kHelper.SkillTreeID_insert( kUserDataEx.m_dwUserUID, (int)sSkill.m_cCharType, (int)(unsigned char)sSkill.m_cPromotion, sSkill.m_nID ) );
        }

        for( sit = kUserDataEx.m_setDeleteReserveSkill.begin() ; sit != kUserDataEx.m_setDeleteReserveSkill.end() ; ++sit )
        {
            SSkillElement sSkill;
            if( !SiKSkillManager()->GetSkill( *sit, sSkill ) ) continue;
            LIF( m_kHelper.SkillTreeID_delete( kUserDataEx.m_dwUserUID, (int)sSkill.m_cCharType, (int)(unsigned char)sSkill.m_cPromotion, sSkill.m_nID ) );
        }
    }

    UpdateSkillSet( kUserDataEx.m_dwUserUID, kUserDataEx.m_mapSkillSetAdd, kUserDataEx.m_mapSkillSetDel );

    UpdateModeLevel( kUserDataEx.m_dwUserUID, kUserDataEx.m_kUserData.m_cCurrentChar, kUserDataEx.m_mapOldDifficulty, kUserDataEx.m_mapDifficulty );

    // 커플인 경우에만 기록 (DB요청)
    if ( kUserDataEx.m_bIsCouple ) {
        std::vector<KCoupleEquip>::iterator vit;
        for ( vit = kUserDataEx.m_vecDelCoupleEquip.begin() ; vit != kUserDataEx.m_vecDelCoupleEquip.end() ; ++vit ) {
            DeleteCoupleEquip( kUserDataEx.m_kUserData.m_strLogin.c_str(), kUserDataEx.m_dwUserUID, *vit );
        }

        for ( vit = kUserDataEx.m_vecUpdateCoupleEquip.begin() ; vit != kUserDataEx.m_vecUpdateCoupleEquip.end() ; ++vit ) {
            UpdateCoupleEquip( kUserDataEx.m_kUserData.m_strLogin.c_str(), kUserDataEx.m_dwUserUID, *vit );
        }

        UpdateCoupleSelectChar( kUserDataEx.m_kUserData.m_strLogin.c_str(), kUserDataEx.m_dwUserUID, kUserDataEx.m_cCoupleMyCharType );
    }

    // 유저의 보드게임 정보 업데이트.
    if( SiKDicePlay()->IsRun() )
    {
        // 어쩔수 없이 해당 코드 추가.
        // 일단은 초기화 유저에 대한 방어 코드.
        if( kUserDataEx.m_kUserBoardInfo.m_dwCurrentPosition == 0 && kUserDataEx.m_kUserBoardInfo.m_dwTotalCount == 0 &&
                    kUserDataEx.m_kUserBoardInfo.m_dwGiveCount == 0 && kUserDataEx.m_kUserBoardInfo.m_dwDiceCount == 0) {
                        START_LOG( cerr, L"동기화 실패로 주사위 초기화 시도중 실패, Name : " << kUserDataEx.m_kUserData.m_strLogin ) << END_LOG;
        } else {
            UpdateUserBoardInfo( kUserDataEx.m_dwUserUID, kUserDataEx.m_dwInitHour, kUserDataEx.m_kUserBoardInfo, kUserDataEx.m_strDiceLastTime, SiKDicePlay()->GetDiceEventVersion() );
        }
    }

    if ( SiKQuizBingo()->IsEventTerm() ) {
        LIF( SetBingoAccTime( kUserDataEx.m_dwUserUID, SiKQuizBingo()->GetEventID(), kUserDataEx.m_nQuizBingoAccTime ) );
    }

    UpdateLookEquipInfo( kUserDataEx.m_dwUserUID, kUserDataEx.m_mapDiffLookEquip );

    // 가위바위보 정보 저장하기
    if( SiKGawibawibo()->IsRun() ) {
        // 초기화에 대한 방어 코드 추가.
        if( kUserDataEx.m_kGawibawiboInfo.m_dwCurrentPosition == 0 && kUserDataEx.m_kGawibawiboInfo.m_dwTurnCount == 0 && 
                kUserDataEx.m_kGawibawiboInfo.m_usReceiveCount == 0 && kUserDataEx.m_kGawibawiboInfo.m_dwPlayCount == 0 && 
                    kUserDataEx.m_kGawibawiboInfo.m_dwRemainTime == 0 && kUserDataEx.m_kGawibawiboInfo.m_usVersion == 0 ) {
            START_LOG( cerr, L"동기화 실패로 가위바위보 초기화 시도중 실패, Name : " << kUserDataEx.m_kUserData.m_strLogin ) << END_LOG;
        } else {
            UpdateUserGawibawiboInfo( kUserDataEx.m_dwUserUID, kUserDataEx.m_kGawibawiboInfo );
        }
    }

    // 양말 정보 저장하기
    if ( false == kUserDataEx.m_mapSocksInfo.empty() ) {
        std::map< GCITEMID, KSocksInfo >::iterator mit;
        for ( mit = kUserDataEx.m_mapSocksInfo.begin(); mit != kUserDataEx.m_mapSocksInfo.end() ; ++mit ) {
            if ( mit->second.m_nIsHangup == KSocksInfo::HANGUP ) {
                SocksHangup( kUserDataEx.m_dwUserUID, mit->first, mit->second.m_dwRemainSec );
            }
        }
    }

    // 유저 캐릭터별 레벨구간당 레벨업시간 통계
    if ( false == kUserDataEx.m_mapUserCharLvPlayTime.empty() ) {
        MapUserCharLvPlayTime::iterator mitChar;
        for ( mitChar = kUserDataEx.m_mapUserCharLvPlayTime.begin() ; mitChar != kUserDataEx.m_mapUserCharLvPlayTime.end() ; ++mitChar ) {
            const int& nCharType = mitChar->first;

            std::map<DWORD,int>::iterator mitStat;
            for ( mitStat = mitChar->second.begin() ; mitStat != mitChar->second.end() ; ++mitStat ) {
                const DWORD& dwLv = mitStat->first;
                const int& nTimeDiff = mitStat->second;

                LIF( m_kHelper.CharacterLevelPeriod_merge_20111122( kUserDataEx.m_dwUserUID, nCharType, dwLv, nTimeDiff ) );
            }
        }
    }

    // 창고 변경 정보  - 시작
    // 아이템 인벤토리/창고  state 변경.
    if ( !kUserDataEx.m_kDepotData.m_vecInvenTypeItemInv.empty() || !kUserDataEx.m_kDepotData.m_vecDeleteDepotItem.empty() ) {
        LIF( UpdateInvenTypeItem( kUserDataEx.m_dwUserUID, kUserDataEx.m_kDepotData.m_vecInvenTypeItemInv, kUserDataEx.m_kDepotData.m_vecDeleteDepotItem, kUserDataEx.m_kUserData.m_cCurrentChar ) );
    }

    if ( !kUserDataEx.m_kDepotData.m_vecInvenTypeItemInv.empty() ) {
        LIF( MergeDepotItem( kUserDataEx.m_dwUserUID, kUserDataEx.m_kDepotData.m_vecInvenTypeItemInv ) );
    }

    if( !kUserDataEx.m_kDepotData.m_vecDeleteDepotItem.empty() ) {
        LIF( DeleteDepotItem( kUserDataEx.m_dwUserUID, kUserDataEx.m_kDepotData.m_vecDeleteDepotItem ) );
    }
    // 창고 변경 정보 - 끝.

    // 송크란 데이터 기록 - 시작 ( 종료 시에도 이 함수를 호출하니까 여기만 넣으면 된다 )
    if ( SiKSongkran()->IsRun() && true == kUserDataEx.m_kSongkranUserData.m_bLoaded ) { // 미니게임 시작 안하면 db데이터 안받아오는데 그걸 기록하면 안되니까 load여부도 체크
        
        START_LOG( clog, L"송크란 종료 DB 기록, Name : " << kUserDataEx.m_kUserData.m_strLogin ) << END_LOG; // 삭제 예정

        UpdateMultiEventUserData( kUserDataEx.m_kUserData.m_dwLoginUID, UET_SONGKRAN, SiKSongkran()->GetVersion(), 
            0, 0, 0,kUserDataEx.m_kSongkranUserData.m_dwSuppliedWaterBombDuration, kUserDataEx.m_kSongkranUserData.m_nAccTime ); // 송크란 이벤트 캐싱 데이터 DB 저장

        // 통계 기록 데이터
        UpdateSongkranWaterBombUseCount( kUserDataEx.m_kUserData.m_dwLoginUID, UET_SONGKRAN, SiKSongkran()->GetVersion(), kUserDataEx.m_kSongkranUserData.m_dwUseCount );
        UpdateSongkranWaterBombSuppliedCount( kUserDataEx.m_kUserData.m_dwLoginUID, UET_SONGKRAN, SiKSongkran()->GetVersion(), kUserDataEx.m_kSongkranUserData.m_dwSuppliedWaterBombDuration );

        START_LOG( clog, L"송크란 DB 캐싱 데이터 " ) << BUILD_LOG( kUserDataEx.m_kSongkranUserData.m_dwGradeDiff ) <<BUILD_LOG( kUserDataEx.m_kSongkranUserData.m_dwScoreDiff ) << END_LOG;
    }
    // 송크란 데이터 기록 - 끝

    // 리타의 크리스마스 이벤트 데이터 기록
    Update_RitasChristmas_UserData_ToDB( kUserDataEx.m_dwUserUID, kUserDataEx.m_kRitasChristmasEventUserData );
    UpdateCharLastLoginDate( kUserDataEx.m_kUserData.m_dwLoginUID, kUserDataEx.m_kUserData.m_cCurrentChar, kUserDataEx.m_kUserData.m_dwSlotNum );

    // 활력(피로도) 유저 데이터 기록
    Update_VitalitySystem_UserData_ToDB( kUserDataEx.m_dwUserUID, kUserDataEx.m_kUserData.m_mapCharacterInfo, kUserDataEx.m_kVitalityUpdateData );

    // 심부름왕 이벤트 데이터 기록
    if ( true == SiKErrandManager()->IsRun() ) {
        int nVersion = 0;
        SiKErrandManager()->GetVersion( nVersion );
        UpdateUserData_ErrandEvent( kUserDataEx.m_kUserData.m_dwLoginUID, nVersion, kUserDataEx.m_kErrandUserInfoData );
    }

    // 접속 선물상자 유저 데이터 기록
    Update_ConnectionGiftBox_UserData_ToDB( kUserDataEx.m_dwUserUID, static_cast< int >( kUserDataEx.m_kUserData.m_cCurrentChar ), kUserDataEx.m_kConnectionGiftBoxUpdateData );

    // 던전 개인 최고 기록 유저 데이터 기록
    Update_DungeonPersonalRecord_UserData_ToDB( kUserDataEx.m_dwUserUID, static_cast< int >( kUserDataEx.m_kUserData.m_cCurrentChar ), kUserDataEx.m_mapDungeonPersonalRecordDiff );

    START_LOG( clog, L"종료시 유저 정보 기록 완료, Name : " << kUserDataEx.m_kUserData.m_strLogin ) << END_LOG;
    return true;
}

bool KGSDBThread::UpdatePetData( const std::map<GCITEMUID, KPetInfo>& mapPetInfo_, const std::map<char, KCharacterInfo>& mapCharInfo_, const DWORD dwSenderUID_, const std::wstring& strLogin_ )
{
    int iOK = 0;

    if( mapPetInfo_.empty() )
    {
        START_LOG( clog, L"전체 펫 정보 기록 완료. 펫이 없음" ) << END_LOG;
        return true;
    }


    std::wstringstream stmExps;

    std::map<GCITEMUID, KPetInfo>::const_iterator mit;
    std::map<char, DWORD>::const_iterator mit2;
    for( mit = mapPetInfo_.begin(); mit != mapPetInfo_.end(); ++mit )
    {
        // pc방 펫은 저장하지 않는다.
        if( mit->first > SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
            //Pets : {PetUID int,PetID int, FullTime int}n -> Exp와 FullTime은 차이값을 주삼, //move Exp(See below) .070419
            if( mit->second.m_iSatiation - mit->second.m_iInitSatiation != 0 ) {
                
                //boost::wformat fmter( L"%d,%d,%d," );
                //fmter % mit->second.m_dwUID
                //    % mit->second.m_dwID
                //    % (mit->second.m_iSatiation - mit->second.m_iInitSatiation);

                //stmPets << fmter.str();

                /*
                L"{ call dbo.PIGAPetInfoFullTime_update_20110516 ( %d, %I64d, %d, %d ) }"

                { call dbo.PIGAPetInfoFullTime_update_20110516 ( @1, @2, @3, @4 ) }
                @1: @iLoginUID_ int
                @2: @iPetUID_   bitint
                @3: @iPetID_    int
                @4: @iFullTime_ int

                1 return ( @1 )
                @1 ; OK
                0       : 성공
                -1      : 유저정보가 없음
                -1      : 펫정보가 없음
                -101이하: 트랜젝션 에러
                */
                DBJIF2( m_kODBC.Query( L"{ call dbo.PIGAPetInfoFullTime_update_20110516 ( %d, %I64d, %d, %d ) }",
                    dwSenderUID_, 
                    mit->second.m_dwUID,
                    mit->second.m_dwID, 
                    mit->second.m_iSatiation - mit->second.m_iInitSatiation ) );

                iOK = -99;
                if( m_kODBC.BeginFetch() ) {
                    VERIFY_QUERY( m_kODBC >> iOK );
                }
                m_kODBC.EndFetch();

            }

            //Exps : {PetUID int, Promotion int, EXP int } PetUID,Promotion,Exp 열의 조합으로 넣어줄 것
            mit2 = mit->second.m_mapInitExp.find( mit->second.m_cPromotion );

            if( mit2 != mit->second.m_mapInitExp.end() &&
                mit->second.m_dwEXP - mit2->second != 0 )
            {
                boost::wformat fmter( L"%d,%d,%d," );
                fmter % mit->second.m_dwUID
                    % static_cast<int>(mit->second.m_cPromotion )
                    % (mit->second.m_dwEXP - mit2->second);

                stmExps << fmter.str();
                //bExps = true;
            }

            //Equips : {PetUID int, ItemUID int, ItemType int}n
            if ( !mit->second.m_vecInitEquipItem.empty() || !mit->second.m_vecEquipItem.empty() ) {
                const std::vector<KSimpleItem>& vecEquips = mit->second.m_vecEquipItem;
                std::vector<KSimpleItem> vecInitEquips = mit->second.m_vecInitEquipItem;

                std::vector<KSimpleItem>::const_iterator vitEquips;
                std::vector<KSimpleItem>::iterator vitInit;

                for ( vitEquips = vecEquips.begin() ; vitEquips != vecEquips.end() ; ++vitEquips ) {
                    // 초기장착 아이템과 비교해서 장착 해제/추가 기록
                    vitInit = std::find_if( vecInitEquips.begin(), vecInitEquips.end(),
                        boost::bind( &KSimpleItem::m_dwUID, _1 ) == vitEquips->m_dwUID );
                    if ( vitInit != vecInitEquips.end() ) {
                        vecInitEquips.erase( vitInit );
                        continue;
                    }

                    // 추가된 장착정보
                    LIF( m_kHelper.PetInfoEquip_insert( dwSenderUID_, mit->second.m_cCharType, mit->second.m_dwUID, vitEquips->m_dwUID ) );
                }

                // 제거된 장착정보
                for ( vitInit = vecInitEquips.begin() ; vitInit != vecInitEquips.end() ; ++vitInit ) {
                    LIF( m_kHelper.PetInfoEquip_delete( dwSenderUID_, mit->second.m_cCharType, mit->second.m_dwUID, vitInit->m_dwUID ) );
                }
            }
        } // End if( mit->first <= SiKIPAdvantage()->GetMaximumPCBangItemUID() )
    } // End For

    // 우선 장착하고 있는 펫을 뺀다.
    ResetCharacterPetEquip( strLogin_, dwSenderUID_ );

    std::map<char, KCharacterInfo>::const_iterator mitCharacter;
    for( mitCharacter = mapCharInfo_.begin(); mitCharacter != mapCharInfo_.end(); ++mitCharacter )
    {
        if( mitCharacter->second.m_kEquipPetInfo.m_dwUID == 0 ) continue; // 장착하고 있지 않으면..
        // PC방 PET에 대해서는 저장하지 않는다.
        if( mitCharacter->second.m_kEquipPetInfo.m_dwUID > SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
            SetCharacterPetEquip( dwSenderUID_, strLogin_, mitCharacter->second.m_kEquipPetInfo.m_dwUID, mitCharacter->first );
        }
    }

    /*
    20081024. microcat.

    { call dbo.PIGA_pet_info_update_exp ( @1, @2 ) }
    @1 ; LoginUID int
    @2 ; Data nvarchar(4000) ; [@21,@22,@23,]n
    @21 ; PetUID int
    @22 ; Promotion tinyint
    @23 ; [Exp] int

    1 return ( @1 )
    @1 ; OK int
    */
    if( !stmExps.str().empty() )
    {
        DBJIF2( m_kODBC.Query( L"{ call dbo.PIGA_pet_info_update_exp_old ( %d, N'%s' ) }",
            dwSenderUID_, stmExps.str().c_str() ) );

        iOK = -99;
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> iOK );
        }
        m_kODBC.EndFetch();

        if( iOK != 0 ) // 실패
        {
            START_LOG( cerr, L"PIGA_pet_info_update_exp 실패 " )
                << BUILD_LOG( iOK )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }

    LIF( UpdatePetCostume( strLogin_, mapPetInfo_ ) );
    switch( iOK )
    {
    case 0:  SET_ERROR( NET_OK );     break;
    case -5: SET_ERROR( ERR_PET_14 ); break;
    case -6: SET_ERROR( ERR_PET_07 ); break;
    case -7: SET_ERROR( ERR_PET_15 ); break;
    case -8: SET_ERROR( ERR_PET_16 ); break;
    case -9: SET_ERROR( ERR_PET_17 ); break;
    case -10: SET_ERROR( ERR_PET_18 ); break;
    default:
        SET_ERROR( ERR_UNKNOWN );
    }

    stmExps.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stmExps.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    return (NetError::GetLastNetError() == NetError::NET_OK);
}


bool KGSDBThread::UpdateDepotPetData( const std::map<GCITEMUID, KPetInfo>& mapPetInfo_, const std::map<char, KCharacterInfo>& mapCharInfo_, const DWORD dwSenderUID_, const std::wstring& strLogin_ )
{
    int iOK = 0;

    if( mapPetInfo_.empty() )
    {
        START_LOG( clog, L"전체 펫 정보 기록 완료. 펫이 없음" )
            << END_LOG;
        return true;
    }

    std::wstringstream stmExps;

    std::map<GCITEMUID, KPetInfo>::const_iterator mit;
    std::map<char, DWORD>::const_iterator mit2;
    for( mit = mapPetInfo_.begin(); mit != mapPetInfo_.end(); ++mit )
    {
        // pc방 펫은 저장하지 않는다.
        if( mit->first > SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
            //Pets : {PetUID int,PetID int, FullTime int}n -> Exp와 FullTime은 차이값을 주삼, //move Exp(See below) .070419
            if( mit->second.m_iSatiation - mit->second.m_iInitSatiation != 0 ) {
                
                //boost::wformat fmter( L"%d,%d,%d," );
                //fmter % mit->second.m_dwUID
                //    % mit->second.m_dwID
                //    % (mit->second.m_iSatiation - mit->second.m_iInitSatiation);

                //stmPets << fmter.str();

                /*
                L"{ call dbo.PIGAPetInfoFullTime_update_20110516 ( %d, %I64d, %d, %d ) }"

                { call dbo.PIGAPetInfoFullTime_update_20110516 ( @1, @2, @3, @4 ) }
                @1: @iLoginUID_ int
                @2: @iPetUID_   bitint
                @3: @iPetID_    int
                @4: @iFullTime_ int

                1 return ( @1 )
                @1 ; OK
                0       : 성공
                -1      : 유저정보가 없음
                -1      : 펫정보가 없음
                -101이하: 트랜젝션 에러
                */
                DBJIF2( m_kODBC.Query( L"{ call dbo.PIGAPetInfoFullTime_update_20110516 ( %d, %I64d, %d, %d ) }",
                    dwSenderUID_, 
                    mit->second.m_dwUID,
                    mit->second.m_dwID, 
                    mit->second.m_iSatiation - mit->second.m_iInitSatiation ) );

                iOK = -99;
                if( m_kODBC.BeginFetch() ) {
                    VERIFY_QUERY( m_kODBC >> iOK );
                }
                m_kODBC.EndFetch();

            }

            //Exps : {PetUID int, Promotion int, EXP int } PetUID,Promotion,Exp 열의 조합으로 넣어줄 것
            mit2 = mit->second.m_mapInitExp.find( mit->second.m_cPromotion );

            if( mit2 != mit->second.m_mapInitExp.end() &&
                mit->second.m_dwEXP - mit2->second != 0 )
            {
                boost::wformat fmter( L"%d,%d,%d," );
                fmter % mit->second.m_dwUID
                    % static_cast<int>(mit->second.m_cPromotion )
                    % (mit->second.m_dwEXP - mit2->second);

                stmExps << fmter.str();
                //bExps = true;
            }

            //Equips : {PetUID int, ItemUID int, ItemType int}n
            if ( !mit->second.m_vecInitEquipItem.empty() || !mit->second.m_vecEquipItem.empty() ) {
                const std::vector<KSimpleItem>& vecEquips = mit->second.m_vecEquipItem;
                std::vector<KSimpleItem> vecInitEquips = mit->second.m_vecInitEquipItem;

                std::vector<KSimpleItem>::const_iterator vitEquips;
                std::vector<KSimpleItem>::iterator vitInit;

                for ( vitEquips = vecEquips.begin() ; vitEquips != vecEquips.end() ; ++vitEquips ) {
                    // 초기장착 아이템과 비교해서 장착 해제/추가 기록
                    vitInit = std::find_if( vecInitEquips.begin(), vecInitEquips.end(),
                        boost::bind( &KSimpleItem::m_dwUID, _1 ) == vitEquips->m_dwUID );
                    if ( vitInit != vecInitEquips.end() ) {
                        vecInitEquips.erase( vitInit );
                        continue;
                    }

                    // 추가된 장착정보
                    LIF( m_kHelper.PetInfoEquip_insert( dwSenderUID_, mit->second.m_cCharType, mit->second.m_dwUID, vitEquips->m_dwUID ) );
                }

                // 제거된 장착정보
                for ( vitInit = vecInitEquips.begin() ; vitInit != vecInitEquips.end() ; ++vitInit ) {
                    LIF( m_kHelper.PetInfoEquip_delete( dwSenderUID_, mit->second.m_cCharType, mit->second.m_dwUID, vitInit->m_dwUID ) );
                }
            }
        } // End if( mit->first <= SiKIPAdvantage()->GetMaximumPCBangItemUID() )
    } // End For

    //// 우선 장착하고 있는 펫을 뺀다.
    //ResetCharacterPetEquip( strLogin_, dwSenderUID_ );

    //std::map<char, KCharacterInfo>::const_iterator mitCharacter;
    //for( mitCharacter = mapCharInfo_.begin(); mitCharacter != mapCharInfo_.end(); ++mitCharacter )
    //{
    //    if( mitCharacter->second.m_dwEquipPetUID == 0 ) continue; // 장착하고 있지 않으면..
    //    // PC방 PET에 대해서는 저장하지 않는다.
    //    if( mitCharacter->second.m_dwEquipPetUID > SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
    //        SetCharacterPetEquip( dwSenderUID_, strLogin_, mitCharacter->second.m_dwEquipPetUID, mitCharacter->first );
    //    }
    //}

    /*
    20081024. microcat.

    { call dbo.PIGA_pet_info_update_exp ( @1, @2 ) }
    @1 ; LoginUID int
    @2 ; Data nvarchar(4000) ; [@21,@22,@23,]n
    @21 ; PetUID int
    @22 ; Promotion tinyint
    @23 ; [Exp] int

    1 return ( @1 )
    @1 ; OK int
    */
    if( !stmExps.str().empty() )
    {
        DBJIF2( m_kODBC.Query( L"{ call dbo.PIGA_pet_info_update_exp_old ( %d, N'%s' ) }",
            dwSenderUID_, stmExps.str().c_str() ) );

        iOK = -99;
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> iOK );
        }
        m_kODBC.EndFetch();

        if( iOK != 0 ) // 실패
        {
            START_LOG( cerr, L"PIGA_pet_info_update_exp 실패 " )
                << BUILD_LOG( iOK )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }

    LIF( UpdatePetCostume( strLogin_, mapPetInfo_ ) );
    switch( iOK )
    {
    case 0:  SET_ERROR( NET_OK );     break;
    case -5: SET_ERROR( ERR_PET_14 ); break;
    case -6: SET_ERROR( ERR_PET_07 ); break;
    case -7: SET_ERROR( ERR_PET_15 ); break;
    case -8: SET_ERROR( ERR_PET_16 ); break;
    case -9: SET_ERROR( ERR_PET_17 ); break;
    case -10: SET_ERROR( ERR_PET_18 ); break;
    default:
        SET_ERROR( ERR_UNKNOWN );
    }

    stmExps.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stmExps.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    return (NetError::GetLastNetError() == NetError::NET_OK);
}

bool KGSDBThread::RegistMission( IN const DWORD dwUID_, IN const char& cCharType_, IN const GCITEMID dwItemID_, OUT KMissionData& kData_, OUT int& nRet_ )
{
    KMissionInfo kMissionInfo;
    // 미션 등록을 
    std::wstringstream stmInvalidRegist;
    std::wstringstream stmInvalidComplete;
    std::wstringstream stmSubMission;
    stmInvalidRegist.str( L"" );
    stmInvalidComplete.str( L"" );
    stmSubMission.str( L"" );
    std::vector<int> vecCountSubMission;

    SET_ERROR( ERR_MISSION_21 );

    // 왠만큼 User에서 체크 했다 믿고 가자...
    // 해당 미션의 정보를 가져온다.
    if( !SiKMissionManager()->GetMissionInfo( kMissionInfo, dwItemID_ ) ) {
        SET_ERROR( ERR_MISSION_17 );
        return false;
    }

    // 해당 미션으로 부터 미션 데이터를 추출한다.
    if( !SiKMissionManager()->GetMissionData( kData_, kMissionInfo.m_dwMissionID, false ) ) {
        SET_ERROR( ERR_MISSION_17 );
        return false;
    }

    std::vector<DWORD>::iterator vit;
    if( !kMissionInfo.m_vecInvalidMission.empty() ) {
        for( vit = kMissionInfo.m_vecInvalidMission.begin(); vit != kMissionInfo.m_vecInvalidMission.end(); ++vit ) {
            LIF( CheckPlayMainMission( dwUID_, cCharType_, *vit ) );
        }
    }

    if( !kMissionInfo.m_vecInvalidComplete.empty() ) {
        for( vit = kMissionInfo.m_vecInvalidComplete.begin(); vit != kMissionInfo.m_vecInvalidComplete.end(); ++vit ) {
            if ( false == CheckCompleteMainMission( dwUID_, cCharType_, *vit ) ) {
                SET_ERROR( ERR_MISSION_02 );
                return false;
            }
        }
    }

    if( !SiKMissionManager()->GetCountSubMissionList( vecCountSubMission, kMissionInfo.m_dwMissionID ) ) {
        SET_ERROR( ERR_MISSION_20 );
        return false;
    }

    int nOK = InsertMainMission( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, kMissionInfo.m_nPeriod );

    std::vector<int>::iterator vitCount;
    if( !vecCountSubMission.empty() ) {
        for( vitCount = vecCountSubMission.begin(); vitCount != vecCountSubMission.end(); ++vitCount ) {
            LIF( InsertSubMission( dwUID_, cCharType_, kMissionInfo.m_dwMissionID, *vitCount ) );
        }
    }

    if( nOK != 0 ) {
        LOG_SUCCESS( nOK == -1 || nOK == -2 || nOK == -3 || nOK == -4 )
            << L"미션 등록 DB 쿼리 실패. Name : " << dwUID_ << dbg::endl
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << BUILD_LOG( dwItemID_ ) << END_LOG;

        switch( nOK )
        {
        case 0: SET_ERROR( NET_OK ); break;
        case -1: SET_ERROR( ERR_MISSION_00 ); break; // 유저 정보 없음
        case -2: SET_ERROR( ERR_MISSION_28 ); break; // 캐릭터 정보 없음
        case -3: SET_ERROR( ERR_MISSION_01 ); break; // 이미 진행중인 미션
        case -4: SET_ERROR( ERR_MISSION_02 ); break; // 이미 완료된 미션
        default:
            SET_ERROR( ERR_MISSION_21 );
        }

        nRet_ = NetError::GetLastNetError(); // 실패 이유를 알 수 있도록

        return false;
    }
    return true;
}

bool KGSDBThread::RemoveMission( IN const DWORD dwUID_, IN const char& cCharType_, IN const GCITEMID dwItemID_, IN const int nDeleteType_ )
{
    /*
    L"{ call dbo.MSGAMissionSystemMain_delete_20130527 ( %d, %d, %d, %d ) }"

    { call dbo.MSGAMissionSystemMain_delete_20130527 ( @01, @02, @03, @04 ) }
    @01iLoginUID_input int
   ,@02iCharType_input int
   ,@03iMainID_input   int
   ,@04iStatus_input   int
        2: 유저 삭제
        3: 완료

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 미션 정보 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.MSGAMissionSystemMain_delete_20130527 ( %d, %d, %d, %d ) }",
        dwUID_, cCharType_, dwItemID_, nDeleteType_ ) );

    if( m_kODBC.BeginFetch() ) {
        int nOK;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();

        LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

        switch( nOK )
        {
        case 0:  SET_ERROR( NET_OK ); break;
        case -1: SET_ERROR( ERR_MISSION_00 ); break;
        default:
            SET_ERROR( ERR_MISSION_21 );
            START_LOG( cerr, L"미션 제거시 트랜젝션 오류, Name : " << dwUID_ )
                << L" Err Code : " << nOK << END_LOG;
        }
        if( nOK == 0 )
            return true;
        else {
            START_LOG( cerr, L"미션 제거 실패, UserUID : " << dwUID_ )
                << BUILD_LOG( m_kODBC.m_strLastQuery )
                << END_LOG;
            return false;
        }
    }

    return false;
}

void KGSDBThread::GetEventCharacter( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ )
{
    //이벤트의 적용 캐릭터 리스트
    //
    //{ call dbo.EPGA_event_perfect_character_select ( %d ) }
    //@1 ; iEventUID int
    //
    //n return ( @1, @2 )
    //@1 ; CharType int
    //@2 ; Promotion int
    std::map<int, std::set<int> >   mapCharTypePromotion;

    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_character_select ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        int nCharType = -1;
        int nPromotion = -1;

        VERIFY_QUERY( m_kODBC >> nCharType >> nPromotion );
        mapCharTypePromotion[nCharType].insert( nPromotion );
    }

    sTimeEvent_.m_mapCharTypePromotion.swap( mapCharTypePromotion );

    START_LOG( clog, L"Get Character Promotion Event from DB. size : " << sTimeEvent_.m_mapCharTypePromotion.size() ) << END_LOG;
}

void KGSDBThread::GetEventEquipItem( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ )
{
    //이벤트 장착 아이템 리스트
    //
    //{ call dbo.EPGA_event_perfect_item_select ( %d ) }
    //@1 ; iEventUID int
    //
    //n return ( @1 )
    //@1 ; Type int
    //@2 ; ItemID int
    //@3 ; Chartype int
    std::set<DWORD> setEquipA;
    std::set<DWORD> setEquipB;

    int nType = -1;
    GCITEMID nItemID = -1;
    int nCharType = -1;
    int nCharTypeA = -1;
    int nCharTypeB = -1;

    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_item_select ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> nType >> nItemID >> nCharType );

        if( nType == 0 )
        {
            nCharTypeA = nCharType;
            setEquipA.insert( nItemID );
        }
        else if( nType == 1 )
        {
            nCharTypeB = nCharType;
            setEquipB.insert( nItemID );
        }
    }

    if( !setEquipA.empty() )
    {
        sTimeEvent_.m_mapEquipItemIDA.insert( std::make_pair( (char)(nCharTypeA), setEquipA ) );
    }
    if( !setEquipB.empty() )
    {
        sTimeEvent_.m_mapEquipItemIDB.insert( std::make_pair( (char)(nCharTypeB), setEquipB ) );
    }
}

void KGSDBThread::GetEventLevel( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ )
{
    //이벤트 적용 레벨 리스트
    //
    //{ call dbo.EPGA_event_perfect_level_select ( %d ) }
    //@1 ; iEventUID int
    //
    //n return ( @1, @2 )
    //@1 ; StartLevel int
    //@2 ; EndLevel int
    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_level_select ( %d ) }", dwUID_ ) );
    if( m_kODBC.BeginFetch() )
    {             
        VERIFY_QUERY( m_kODBC >> sTimeEvent_.m_sLevelBonus.m_iBeginLv >> sTimeEvent_.m_sLevelBonus.m_iEndLv );
        m_kODBC.EndFetch();
    }
}

void KGSDBThread::GetEventModeList( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ )
{
    //이벤트 적용 모드 리스트
    //
    //{ call dbo.EPGA_event_perfect_mode_select ( %d ) }
    //@1 ; iEventUID int
    //
    //n return ( @1, @2 )
    //@1 ; ModeID int
    //@2 ; Type int
    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_mode_select ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        int nModeID = -1;;
        int nType = -1;

        VERIFY_QUERY( m_kODBC >> nModeID >> nType );
        _JIF( nModeID >= 0, m_kODBC.EndFetch(); return (void)0 );
        sTimeEvent_.m_mapModeList[nModeID].insert( nType );
    }

    START_LOG( clog, L"Get Mode Event from DB. size : " << sTimeEvent_.m_mapModeList.size() ) << END_LOG;
}

void KGSDBThread::GetEventParty( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ )
{
    //이벤트 파티원 보상
    //
    //{ call dbo.EPGA_event_perfect_party_select ( %d ) }
    //@1 ; iEventUID int
    //
    //n return ( @1, @2, @3 )
    //@1 ; Party int
    //@2 ; GPRatio int
    //@3 ; EXPRatio int
    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_party_select ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> sTimeEvent_.m_nPartyNum >> sTimeEvent_.m_fPartyExpBonus >> sTimeEvent_.m_fPartyGpBonus );
    }
}

void KGSDBThread::GetEventServer( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ )
{
    //이벤트 서버 리스트
    //
    //{ call dbo.EPGA_event_perfect_server_select ( %d ) }
    //@1 ; iEventUID int
    //
    //n return ( @1 )
    //@1 ; ServerPart int
    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_server_select ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        int nServerNum = -1;
        VERIFY_QUERY( m_kODBC >> nServerNum );
        sTimeEvent_.m_setServerNum.insert( nServerNum );
    }

    START_LOG( clog, L"Get Server Event from DB. size : " << sTimeEvent_.m_setServerNum.size() ) << END_LOG;
}

void KGSDBThread::GetEventStrengthRatio( IN const DWORD dwUID_, OUT sTimeEvent& sTimeEvent_ )
{
    /*
    20110830. mk8253.강화율 상승 이벤트

    { call dbo.EPGA_event_perfect_StrengthRatio_select ( @1 ) }
    @1 ; iEventUID int

    1 return ( @1 )
    @1 ; StrengthRatio float
    */
    // 작업필요
    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_StrengthRatio_select ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC >> sTimeEvent_.m_fStrengthRatio );
    }
}

void KGSDBThread::GetEventPet( IN const DWORD dwUID_, OUT std::vector<KPetBonusInfo>& vecPetBonus_ )
{
    /*
    20090305. mk8253.
    펫 경험치 이벤트
    { call dbo.EPGA_event_perfect_pet_select ( @1 ) }
    @1 ; iEventUID int

    n return ( @1, @2, @3 )
    @1 ; PetID int
    @2 ; PetPromotion int
    @3 ; PetExpRatio float
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_pet_select ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        KPetBonusInfo kPetBonus;
        kPetBonus.m_nPetID = -1;
        kPetBonus.m_nPromotion = -1;
        kPetBonus.m_fExpBonusRatio = 0.f;

        VERIFY_QUERY( m_kODBC >> kPetBonus.m_nPetID >> kPetBonus.m_nPromotion >> kPetBonus.m_fExpBonusRatio );

        vecPetBonus_.push_back( kPetBonus );
    }

    START_LOG( clog, L"Get Pet Event from DB.. size : " << vecPetBonus_.size() ) << END_LOG;
}

void KGSDBThread::UpdateTimeEvent()
{
    std::vector< sTimeEvent >::iterator vit;
    std::vector< sTimeEvent > vecTimeEvent;
    std::vector<KPetBonusInfo> vecPetBonus;
    std::set<int> setFreeHellMode;
    std::set<int> setPerfectEventMode;

    vecTimeEvent.clear();
    vecPetBonus.clear();
    setFreeHellMode.clear();
    setPerfectEventMode.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.EPGA_event_perfect_select }" ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        sTimeEvent timeEvent;

        VERIFY_QUERY( m_kODBC
            >> timeEvent.m_nEventUID
            >> timeEvent.m_nEventID
            >> timeEvent.m_bApplyType
            >> timeEvent.m_fActiveRate
            >> timeEvent.m_fGpRatio
            >> timeEvent.m_fExpRatio );

        vecTimeEvent.push_back( timeEvent );
    }

    if( !vecTimeEvent.empty() )
    {
        for( vit = vecTimeEvent.begin() ; vit != vecTimeEvent.end() ; ++vit )
        {
            GetEventCharacter( vit->m_nEventUID, *vit );
            GetEventEquipItem( vit->m_nEventUID, *vit );
            GetEventLevel( vit->m_nEventUID, *vit );
            GetEventModeList( vit->m_nEventUID, *vit );
            GetEventParty( vit->m_nEventUID, *vit );
            GetEventServer( vit->m_nEventUID, *vit );
            GetEventStrengthRatio( vit->m_nEventUID, *vit );
            GetEventPet( vit->m_nEventUID, vecPetBonus );
            // EventID에 종속적인 이벤트 예외 처리.
            GetFreeHellModeEvent( *vit, setFreeHellMode );
            GetPerfectClearBonusEvent( *vit, setPerfectEventMode );
            GetEventDungeonDropRatio( vit->m_nEventUID, *vit );
        }
    }

    SiKTimeEventManager()->UpdateTimeEvent( vecTimeEvent );
    SiKPetHelper()->SetPetExpBonusInfo( vecPetBonus );
    SiKGCHelper()->SetHellItemFreeMode( setFreeHellMode );
    SiKResultManager()->SetPerfectEventGameMode( setPerfectEventMode );

    START_LOG( clog, L"시간 제한 이벤트 얻어냄 : " << vecTimeEvent.size() ) << END_LOG;
}

void KGSDBThread::UpdateLoudMsg()
{

    // 070201. kkurrung. ��ü �޼���
    //Message
    std::map< std::pair< int, DWORD >, std::wstring > mapMessageTable;
    DBJIF(m_kODBC.Query(L"{ call dbo.MMGAMessageMessage_select_20110830 }"));
    while (m_kODBC.Fetch() != SQL_NO_DATA) {
        DWORD dwKey;
        std::wstring strMsg;
        int nLanguageID;

        VERIFY_QUERY(m_kODBC >> nLanguageID >> dwKey >> strMsg);

        mapMessageTable.insert(std::make_pair(std::make_pair(nLanguageID, dwKey), strMsg));
    }

    //( ItemID, Offset, Name, MsgID )
    std::vector< boost::tuple<DWORD, char, std::wstring, DWORD, int> >  vecNameTable;
    DBJIF(m_kODBC.Query(L"{ call dbo.MMGAMessageMessageName_select_20110830 }"));
    while (m_kODBC.Fetch() != SQL_NO_DATA)
    {
        DWORD dwID;
        char cOffset;
        std::wstring strName;
        int nLanguageID;
        DWORD dwMessageKey;

        VERIFY_QUERY(m_kODBC >> dwID >> cOffset >> strName >> nLanguageID >> dwMessageKey);
        vecNameTable.push_back(boost::make_tuple(dwID, cOffset, strName, dwMessageKey, nLanguageID));
    }

    SiKLoudMessage()->UpdateMsgNames(mapMessageTable, vecNameTable);
}

void KGSDBThread::UpdateCalendar( IN const DWORD& dwUID_, IN const char& cCharType_, OUT std::vector<KDailyInfo>& vecCalendar_ )
{
    vecCalendar_.clear();
    /*
    L"{ call dbo.PBGAPointBagCalender_select_20130523 ( %d, %d ) }"

    { call dbo.PBGAPointBagCalender_select_20130523 ( @01, @02 ) }
    @01iLoginUID_input int
   ,@02iCharType_input int

    n return ( @01, @02, @03 )
    @01: RegDateD smalldatetime
    @02: LineNo   int
    @03: [Type]   int
    0x01:세모
    0x02:네모
    0x04:동그라미
    0x08:주
    0x10:월
    0x20:럭키
    */
    if( !m_kODBC.Query( L"{ call dbo.PBGAPointBagCalender_select_20130523 ( %d, %d ) }",
        dwUID_, cCharType_ ) )
    {
        START_LOG( cerr, L"DB에서 달력 가져오는 부분 실패" )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( (&m_kODBC)->m_szDiagMsg )
            << BUILD_LOG( (wchar_t*)((&m_kODBC)->m_sqlState) )
            << BUILD_LOG( (&m_kODBC)->m_strLastQuery ) << END_LOG; 

        SET_ERROR( ERR_CALENDAR_09 ); // 달력 정보 로드 실패
        return;
    }

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        time_t tmTime;
        char cWeekNum; //주차
        char cSealType;
        VERIFY_QUERY( m_kODBC >> tmTime >> cWeekNum >> cSealType );

        CTime tmDate( tmTime );
        KDailyInfo kDailyInfo;
        kDailyInfo.m_kDate.m_sYear      = tmDate.GetYear();
        kDailyInfo.m_kDate.m_cMonth     = tmDate.GetMonth();
        kDailyInfo.m_kDate.m_cDay       = tmDate.GetDay();
        kDailyInfo.m_kDate.m_cWeekDay   = tmDate.GetDayOfWeek() -1;
        kDailyInfo.m_cSealType          = cSealType;
        kDailyInfo.m_cWeekID            = cWeekNum;

        vecCalendar_.push_back( kDailyInfo );
    }
    START_LOG( clog, L"DB에서 얻어온 달력 벡터의 크기는..." )
        << BUILD_LOG( vecCalendar_.size() ) << END_LOG; // 로그에 사용자 아이디도 뿌려야 할텐데... // 070501. woosh.
}

void KGSDBThread::GetReserveMonthlyList( IN const DWORD& dwUID_, IN const char& cCharType_, OUT std::vector<KSimpleDate>& vecRewardList_ )
{
    vecRewardList_.clear();
    /*
    월보상 미지급 리스트 재전달.
    L"{ call dbo.PBGAPointBagMonthReward_select_20130523 ( %d, %d ) }"

    { call dbo.PBGAPointBagMonthReward_select_20130523 ( @01, @02 ) }
    @01: @01iLoginUID_input int
    @02: @02iCharType_input int

    n return ( @01, @02 )
    @01: RegDateM smalldatetime
    @02: ItemID   int

    ItemID가 -1일 경우 지급받은 아이템이 없음. 나머지 ItemID는 유효하다고 판단함.
    */

    std::vector<KSimpleDate> kPacket;
    DBJIF( m_kODBC.Query( L"{ call dbo.PBGAPointBagMonthReward_select_20130523 ( %d, %d ) }", dwUID_, cCharType_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        time_t tmDate = time_t(NULL);
        int nRecieveItem = -1;

        VERIFY_QUERY( m_kODBC >> tmDate >> nRecieveItem );

        if ( nRecieveItem == -1 ) {
            CTime Date( tmDate );
            KSimpleDate simpleDate( Date.GetYear(), Date.GetMonth(), Date.GetDay(), 0 );
            vecRewardList_.push_back( simpleDate );
        }
    }
}

void KGSDBThread::StatisticsInit()
{
    // Mode...
    {
        /*
        L"{ call dbo.SPTSeasonTwo_merge_20110622 ( %d, %d, N'%s' ) }"

        { call dbo.SPTSeasonTwo_merge_20110622 ( @1, @2, @3 ) }
        @1: @iModeID_     smallint
        @2: @iDifficluty_ tinyint
        @3: @strModeName_ nvarchar( 20 )
            
        1 return ( @1 )
        @1: OK int
            0       : 성공
            -101이하: 트랜젝션 에러
        */
        std::wstringstream stm;
        for( int nMode = 0 ; nMode < NUM_GC_GAME_MODE ; ++nMode ) {
            if( KRoom::IsDifficultyMode( nMode ) ) {
                for( int nDifficult = 0 ; nDifficult < KRoom::GetMaxDifficulty( nMode ) ; ++nDifficult ) {
                    //stm << nMode << L"," << nDifficult << L"," << KRoom::ms_szModeName[nMode] << L"(" << (nDifficult + 1) << L")" << L"," ;
                    _DBJIF( m_kODBCStat.Query( L"{ call dbo.SPTSeasonTwo_merge_20110622 ( %d, %d, N'%s' ) }",
                        nMode, nDifficult, KRoom::ms_szModeName[nMode] ), return, (&m_kODBCStat) );

                    int nOK = 99;
                    if( m_kODBCStat.BeginFetch() ) {
                        VERIFY_QUERY( m_kODBCStat >> nOK );
                        m_kODBCStat.EndFetch();
                    }

                    LOG_SUCCESS( nOK == 0 )
                        << L"초기 게임 모드 이름 등록 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
                        << BUILD_LOG( nOK )
                        << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
                }

                int nChampionDifficulty = GC_GMD_CHAMPION_BEGIN;
                if ( SiKResultManager()->IsExistDifficulty( nMode, nChampionDifficulty ) ) {
                    _DBJIF( m_kODBCStat.Query( L"{ call dbo.SPTSeasonTwo_merge_20110622 ( %d, %d, N'%s' ) }",
                        nMode, nChampionDifficulty, KRoom::ms_szModeName[nMode] ), return, (&m_kODBCStat) );

                    int nOK = 99;
                    if( m_kODBCStat.BeginFetch() ) {
                        VERIFY_QUERY( m_kODBCStat >> nOK );
                        m_kODBCStat.EndFetch();
                    }

                    LOG_SUCCESS( nOK == 0 )
                        << L"초기 게임 모드 이름 등록 (챔피언)" << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
                        << BUILD_LOG( nOK )
                        << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
                }

                int nBreakDifficulty = GC_GMD_BREAK_BEGIN;
                if (SiKResultManager()->IsExistDifficulty(nMode, nBreakDifficulty)) {
                    _DBJIF(m_kODBCStat.Query(L"{ call dbo.SPTSeasonTwo_merge_20110622 ( %d, %d, N'%s' ) }",
                        nMode, nBreakDifficulty, KRoom::ms_szModeName[nMode]), return, (&m_kODBCStat));

                    int nOK = 99;
                    if (m_kODBCStat.BeginFetch()) {
                        VERIFY_QUERY(m_kODBCStat >> nOK);
                        m_kODBCStat.EndFetch();
                    }

                    LOG_SUCCESS(nOK == 0)
                        << L"초기 게임 모드 이름 등록 (챔피언)" << (nOK == 0 ? L"성공" : L"실패") << dbg::endl
                        << BUILD_LOG(nOK)
                        << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
                }
            }
            else {
                //stm << nMode << L",0," << KRoom::ms_szModeName[nMode] << L",";
                _DBJIF( m_kODBCStat.Query( L"{ call dbo.SPTSeasonTwo_merge_20110622 ( %d, %d, N'%s' ) }",
                    nMode, 0, KRoom::ms_szModeName[nMode] ), return, (&m_kODBCStat) );

                int nOK = 99;
                if( m_kODBCStat.BeginFetch() ) {
                    VERIFY_QUERY( m_kODBCStat >> nOK );
                    m_kODBCStat.EndFetch();
                }

                LOG_SUCCESS( nOK == 0 )
                    << L"초기 게임 모드 이름 등록 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
                    << BUILD_LOG( nOK )
                    << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
            }
        }
    }

    // mapID
    {
        //call dbo.spt_season_two_map_update
        //( Data nvarchar(4000) )
        //
        //Data = [MapID smallint, MapName nvarchar(20),]
        //
        //return 1 loop select
        //( OK int )
        //
        //0 : 성공
        //-1 : 데이터의 짝이 맞지 않음
        //-2 : Data에 중복된 MapID가 있음
        //-101이하 : 트랜잭션 에러

        std::wstringstream stm;
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        USHORT usMaxString = 3500; // SP Parameter 데이터 타입이 varchar( 4000 ) 이므로 제한 필요.
        int nOK = 99;
        for( int i = 0 ; i < KRoom::GetMapNum() ; ++i ) {
            if( stm.str().size() == 0 ) {
                stm << L"{ call dbo.spt_season_two_map_update ( '";
            }
            stm << KRoom::ms_naMaps[i] << L"," << KRoom::ms_szMapName[i] << L",";
            if( stm.str().size() > usMaxString ) {
                stm << L"' ) }";
                _DBJIF( m_kODBCStat.QueryToWString( stm.str() ), return, (&m_kODBCStat) );

                if( m_kODBCStat.BeginFetch() ) {
                    VERIFY_QUERY( m_kODBCStat >> nOK );
                    m_kODBCStat.EndFetch();
                }
                stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
                stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
            }
        }
        if( stm.str().size() != 0 ) {
            stm << L"' ) }";
            _DBJIF( m_kODBCStat.QueryToWString( stm.str() ), return, (&m_kODBCStat) );

            if( m_kODBCStat.BeginFetch() ) {
                VERIFY_QUERY( m_kODBCStat >> nOK );
                m_kODBCStat.EndFetch();
            }
            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        }

        LOG_SUCCESS( nOK == 0 )
            << L"초기 맵 이름 등록 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
            << BUILD_LOG( nOK )
            << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;
    }
}

void KGSDBThread::SelectSellItemList()
{
    /*
    { call dbo.DSG_divide_sell_item_list_select }

    { call dbo.DSG_divide_sell_item_list_select }

    n return ( @1, @2, @3, @4, @5 )
    @1 ; ItemID int
    @2 ; Type tinyint ; 보상타입 ; 1(GP), 2(잼), 3(크리스탈)
    @3 ; Price int ; 해당 아이템 구매 가격
    @4 ; Duration int ; 해당 아이템 구매시 지급 수량
    @5 ; Factor int ; 판매시 나눠지는 비율
    */
    std::map<GCITEMID, sSellItemInfo> mapSellItemInfo;
    DBJIF( m_kODBC.Query( L"{ call dbo.DSG_divide_sell_item_list_select }" ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        sSellItemInfo sItem;
        sItem.m_dwItemID = 0;
        VERIFY_QUERY( m_kODBC   >> sItem.m_dwItemID
            >> sItem.m_cSellMoneyType
            >> sItem.m_nBuyPrice
            >> sItem.m_nBuyCount
            >> sItem.m_nDivideFactor );
        if( sItem.m_dwItemID == 0 ) continue; // Exception 일으켰을경우

        // 클라이언트에서 사용하는 머니 타입으로 변환.
        switch( sItem.m_cSellMoneyType )
        {
        case 1 : sItem.m_cSellMoneyType = KItemManager::EMT_GAMEPOINT; break; // GP
        case 2 : sItem.m_cSellMoneyType = KItemManager::EMT_GEM; break; // Gem
        case 3 : sItem.m_cSellMoneyType = KItemManager::EMT_CRYSTAL; break; // Crystal
        }
        mapSellItemInfo.insert( std::make_pair( sItem.m_dwItemID, sItem ) );
    }

    SiKItemManager()->UpdateSellItemInfo( mapSellItemInfo );
}

bool KGSDBThread::GetMsgServerList( OUT std::vector<KServerInfo>& vecServerList_ )
{
    vecServerList_.clear();
    // 메시지 서버 리스트를 얻어온다.
    //{ call dbo.MSG_MsgServer_Enable }
    KODBC* pkODBCMsg = SiKGSDBLayer()->GetODBC( "__odbcMsgServer" );   // 메신저용

    _JIF( pkODBCMsg, return false );

    KLocker Lock( pkODBCMsg->GetCS() );

    _DBJIF( pkODBCMsg->Query( L"{ call dbo.MSG_MsgServer_Enable }" ), return false, pkODBCMsg );

    while( pkODBCMsg->Fetch() != SQL_NO_DATA )
    {
        int         iUID;
        KServerInfo kServerInfo;

        VERIFY_QUERY( (*pkODBCMsg) >> iUID
            >> kServerInfo.m_iUserNum
            >> kServerInfo.m_strIP
            >> kServerInfo.m_usPort
            >> kServerInfo.m_strConnIP );

        kServerInfo.m_strName = (boost::wformat( L"MsgServer_%02d" ) % iUID).str();
        vecServerList_.push_back( kServerInfo );
    }

    return true;
}

void KGSDBThread::GetTrainedSkill( IN DWORD dwUID_, 
                                  IN std::map<char, KCharacterInfo>& mapCharacterInfo_, 
                                  OUT KFullSPInfo& kFullSPInfo_, 
                                  IN const std::set<char>& setOldSkillCharacter_ )
{
    { // Pre Set
        kFullSPInfo_.m_mapTrainedSkill.clear();
        std::map<char, KCharacterInfo>::iterator mit;
        for( mit = mapCharacterInfo_.begin() ; mit != mapCharacterInfo_.end() ; ++mit )
        {
            for( char cCount = -1 ; cCount <= mit->second.m_cPromotion ; ++cCount )
            {
                KSPInfo kSpInfo;
                kSpInfo.m_cCharType = mit->second.m_cCharType;
                kSpInfo.m_cPromotion = cCount;
                std::pair<char,char> prKey( kSpInfo.m_cCharType, kSpInfo.m_cPromotion );
                kFullSPInfo_.m_mapTrainedSkill.insert( std::make_pair(prKey,kSpInfo) ); // 보유 캐릭터로 미리 데이터 구성.
            }
        }
    }
    //--유저가 보유하고 있는 스킬 정보
    /*
    L"{ call dbo.STGASkillTreeID_select_20120604 ( %d, N'%s' ) }"

    { call dbo.STGASkillTreeID_select_20120604 ( @1, @2 ) }
    @1: @iLoginUID_  int
    @2: @strCharType nvarchar( 49 ) = N'0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15'

    n return ( @1, @2, @3 )
    @1: CharType  tinyint
    @2: Promotion tinyint
    @3: SkillID   int
    */

    std::vector< boost::tuple<char, char, int> > vecRemove;

    std::wstringstream stm;
    stm << L"{ call dbo.STGASkillTreeID_select_20120604 ( ";
    stm << dwUID_ << L",";

    std::set<char>::const_iterator sit;
    for ( sit = setOldSkillCharacter_.begin(); sit != setOldSkillCharacter_.end(); ++sit ) {
        if ( sit != setOldSkillCharacter_.begin() ) {
            stm << L",";
        }
        stm << static_cast<int>(*sit);        
    }
    stm << L") }";

    DBJIF( m_kODBC.QueryToWString( stm.str() ) );

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    //DBJIF( m_kODBC.Query( L"{ call dbo.STGASkillTreeID_select_20120604 ( %d, N'%s' ) }",
        //dwUID_, strNewSkillCharacter_.c_str() ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        std::pair<char,char> prKey;
        int  nSkillID = -1;
        unsigned char ucPromotion;
        VERIFY_QUERY( m_kODBC >> prKey.first >> ucPromotion >> nSkillID );

        prKey.second = ucPromotion;
        if( nSkillID < 0 ) continue;

        SSkillElement sSkill;
        if( !SiKSkillManager()->GetSkill( nSkillID, sSkill ) ) // 스크립트에 스킬이 없으면 제거.
        {
            vecRemove.push_back( boost::make_tuple( prKey.first, prKey.second, nSkillID ));
            continue;
        }

        std::map<char, KCharacterInfo>::iterator mitChar;
        mitChar = mapCharacterInfo_.find( sSkill.m_cCharType );
        if( mitChar == mapCharacterInfo_.end() ) // 스킬 캐릭터가 내 캐릭터에 없는 캐릭터이면
        {
            vecRemove.push_back( boost::make_tuple( prKey.first, prKey.second, nSkillID ));
            continue;
        }

        if( sSkill.m_nCharLv > (int)mitChar->second.m_dwLevel ) // 스킬 레벨에 캐릭터 레벨이 미달
        {
            vecRemove.push_back( boost::make_tuple( prKey.first, prKey.second, nSkillID ));
            continue;
        }

        std::map<std::pair<char,char>, KSPInfo >::iterator mit;
        mit = kFullSPInfo_.m_mapTrainedSkill.find( prKey );
        if( mit == kFullSPInfo_.m_mapTrainedSkill.end() ) // 보유하지 않은 (캐릭터, 전직)의 스킬일 경우 제거
        {
            vecRemove.push_back( boost::make_tuple( prKey.first, prKey.second, nSkillID ));
            continue;
        }
        mit->second.m_vecSkills.push_back( nSkillID ); // pre set 스킬 데이터에 스킬 번호 넣기. 유효한 스킬이다.
    }

    if ( !vecRemove.empty() ) { // 무효한 스킬을 지운다.
        std::vector< boost::tuple<char, char, int> >::iterator vit;
        for ( vit = vecRemove.begin() ; vit != vecRemove.end() ; ++vit ) {
            LIF( m_kHelper.SkillTreeID_delete( dwUID_, (int)(vit->get<0>()), (int)(unsigned char)(vit->get<1>()), (int)(vit->get<2>()) ) );
        }
    }
}

void KGSDBThread::GetSkillSet( IN DWORD dwUID_, IN std::map<char, KCharacterInfo>& mapCharacterInfo_, OUT KFullSPInfo& kFullSPInfo_, IN const std::set<char>& setOldSkillCharacter_ )
{
    SiKSkillManager()->InitSkillSets( mapCharacterInfo_, kFullSPInfo_.m_mapSkillSets );

    /*
    L"{ call dbo.STGASkillTreeSet_select_20120604 ( %d, N'%s' ) }"

    { call dbo.STGASkillTreeSet_select_20120604 ( @1, @2 ) }
    @1: @iLoginUID_  int
    @2: @strCharType nvarchar( 49 ) = N'0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15'

    n return ( @1, @2, @3, @4, @5, @6 )
    @1: CharType  tinyint
    @2: Promotion tinyint
    @3: SetID     tinyint
    @4: SlotIndex int
    @5: SlotID    int
    @6: SkillID   int
    */

    std::vector< boost::tuple<char, char, int, int, int, int > > vecRemove; // [ char type, promotion, setid, SlotIndex, Slotid, skillID ]

    std::wstringstream stm;
    stm << L"{ call dbo.STGASkillTreeSet_select_20120604 ( ";
    stm << dwUID_ << L",";

    std::set<char>::const_iterator sit;
    for ( sit = setOldSkillCharacter_.begin(); sit != setOldSkillCharacter_.end(); ++sit ) {
        if ( sit != setOldSkillCharacter_.begin() ) {
            stm << L",";
        }
        stm << static_cast<int>(*sit);        
    }
    stm << L") }";

    DBJIF( m_kODBC.QueryToWString( stm.str() ) );

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    //DBJIF( m_kODBC.Query( L"{ call dbo.STGASkillTreeSet_select_20120604 ( %d, N'%s' ) }",
    //    dwUID_, strNewSkillCharacter_.c_str() ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        char cCharType;
        char cPromotion;
        int  nSetID;
        KSkillSlot kSlot;

        VERIFY_QUERY( m_kODBC >> cCharType >> cPromotion >> nSetID >> kSlot.m_nSlotIndex >> kSlot.m_nSlotID >> kSlot.m_nSkillID );
        if( nSetID > 1 || nSetID < 0 ) {
            vecRemove.push_back( boost::make_tuple( cCharType, cPromotion, nSetID, kSlot.m_nSlotIndex, kSlot.m_nSlotID, kSlot.m_nSkillID ) );
            continue;
        }
        std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;
        std::pair<char,char> prKey( cCharType, cPromotion );
        mit = kFullSPInfo_.m_mapSkillSets.find( prKey );
        if( mit == kFullSPInfo_.m_mapSkillSets.end() ) {
            vecRemove.push_back( boost::make_tuple( cCharType, cPromotion, nSetID, kSlot.m_nSlotIndex, kSlot.m_nSlotID, kSlot.m_nSkillID ) );
            continue;
        }

        std::map<char, KCharacterInfo>::iterator mitChar;
        mitChar = mapCharacterInfo_.find( cCharType );
        if( mitChar == mapCharacterInfo_.end() ) { // 캐릭터가 존재 하지 않을경우
            vecRemove.push_back( boost::make_tuple( cCharType, cPromotion, nSetID, kSlot.m_nSlotIndex, kSlot.m_nSlotID, kSlot.m_nSkillID ) );
            continue;
        }

        if( mitChar->second.m_cPromotion < cPromotion || cPromotion < -1 ) { // 전직이 맞지 않을경우
            vecRemove.push_back( boost::make_tuple( cCharType, cPromotion, nSetID, kSlot.m_nSlotIndex, kSlot.m_nSlotID, kSlot.m_nSkillID ) );
            continue;
        }

        // 배우지 않은 스킬일 경우
        if( CheckSkillInfo( cCharType, cPromotion, kSlot.m_nSkillID, kFullSPInfo_.m_mapTrainedSkill ) == false ) { // 전직 & 공용 에서 찾는다.
            vecRemove.push_back( boost::make_tuple( cCharType, cPromotion, nSetID, kSlot.m_nSlotIndex, kSlot.m_nSlotID, kSlot.m_nSkillID ) );
            continue;
        }

        mit->second[nSetID].m_vecSkills.push_back( kSlot );
    }
    if( !vecRemove.empty() ) {
        std::vector< boost::tuple<char, char, int, int, int, int > >::iterator vit;
        for( vit = vecRemove.begin() ; vit != vecRemove.end() ; ++vit ) {
            LIF( UpdateDeleteSkillTreeSet( dwUID_, (int)vit->get<0>(), (int)vit->get<1>(), vit->get<2>(), vit->get<3>(), vit->get<4>(), vit->get<5>() ) );
        }
    }
}

void KGSDBThread::GetEquipSkillSet( IN DWORD dwUID_, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo_, 
                                   IN const std::set<char>& setOldSkillCharacter_ )
{
    /*
    L"{ call dbo.STGASkillTreeSetEquip_select_20120604 ( %d, N'%s' ) }"

    { call dbo.STGASkillTreeSetEquip_select_20120604 ( @1, @2 ) }
    @1: @iLoginUID_  int
    @2: @strCharType nvarchar( 49 ) = N'0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15'

    n return ( @1, @2, @3 )
    @1: CharType  tinyint
    @2: Promotion tinyint
    @3: SetID     tinyint
    */

    std::wstringstream stm;
    stm << L"{ call dbo.STGASkillTreeSetEquip_select_20120604 ( ";
    stm << dwUID_ << L",";

    std::set<char>::const_iterator sit;
    for ( sit = setOldSkillCharacter_.begin(); sit != setOldSkillCharacter_.end(); ++sit ) {
        if ( sit != setOldSkillCharacter_.begin() ) {
            stm << L",";
        }
        stm << static_cast<int>(*sit);        
    }
    stm << L") }";

    DBJIF( m_kODBC.QueryToWString( stm.str() ) );

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    //DBJIF( m_kODBC.Query( L"{ call dbo.STGASkillTreeSetEquip_select_20120604 ( %d, N'%s' ) }",
    //    dwUID_, strNewSkillCharacter_.c_str() ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        char cCharType;
        UCHAR ucPromotion;
        int  nSetID;

        VERIFY_QUERY( m_kODBC >> cCharType >> ucPromotion >> nSetID );

        char cPromotion = static_cast<char>(ucPromotion);

        std::map<char, KCharacterInfo>::iterator mit;
        mit = mapCharacterInfo_.find( cCharType );
        if( mit == mapCharacterInfo_.end() ) continue;
        if( mit->second.m_cPromotion < cPromotion ) continue;

        mit->second.m_mapEquipSkillSet[ cPromotion ] = nSetID;
    }
}

void KGSDBThread::UpdateEquipSkillSet( DWORD dwUID_, std::map<char, KCharacterInfo>& mapCharacterInfo_ )
{
    /*
    스킬 장착 세트
    L"{ call dbo.STGASkillTreeSetEquip_merge_20120111 ( %d, %d, %d, %d, %d, %d, %d, .. , %d, %d, %d ) }"

    { call dbo.STGASkillTreeSetEquip_merge_20120111 ( @a1, @b000, @c000, @d000, @b001, @c001, @d001, .. , @b199, @c199, @d199 ) }
    @a1   : @LoginUID int
    @b000 : CharType  tinyint = null
    @c000 : Promotion tinyint = null
    @d000 : SetID     tinyint = null
    @b001 : CharType  tinyint = null
    @c001 : Promotion tinyint = null
    @d001 : SetID     tinyint = null
    ..
    @b199 : CharType  tinyint = null
    @c199 : Promotion tinyint = null
    @d199 : SetID     tinyint = null

    1 return ( @1 )
    @1: OK         int
    0       : 성공
    -1      : 유저 없음
    -101이하: 트랜젝션 에러
    */
    const USHORT usMaxCnt = 150;
    std::wstringstream stm;
    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    DWORD dwCnt = 0;
    int nOK = -99;

    std::map<char, KCharacterInfo>::iterator mit;
    for( mit = mapCharacterInfo_.begin() ; mit != mapCharacterInfo_.end() ; ++mit ) {
        if( stm.str().size() == 0 ) {
            stm << L"{ call dbo.STGASkillTreeSetEquip_merge_20120111 ( ";
            stm << static_cast<int>(dwUID_);
        }

        std::map<char, int>::iterator mit2;
        for( mit2 = mit->second.m_mapEquipSkillSet.begin() ; mit2 != mit->second.m_mapEquipSkillSet.end() ; ++mit2 ) {
            stm << L"," << static_cast<UCHAR>(mit->first);
            stm << L"," << static_cast<UCHAR>(mit2->first);
            stm << L"," << static_cast<UCHAR>(mit2->second);
            dwCnt += 3;
        }

        if( dwCnt > usMaxCnt ) {
            stm << L") }";
            DBJIF( m_kODBC.QueryToWString( stm.str() ) );
            if( m_kODBC.BeginFetch() ) {
                VERIFY_QUERY( m_kODBC >> nOK );
                m_kODBC.EndFetch();
            }
            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
            dwCnt = 0;
        }
    }
    if( stm.str().size() != 0 ) {
        stm << L") }";
        DBJIF( m_kODBC.QueryToWString( stm.str() ) );
        if( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();
        }
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }

    _LOG_SUCCESS( nOK == 0, L" 스킬 셋트 장착 정보 업데이트 결과. nOK : " << nOK << L", UserUID : " << dwUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

void KGSDBThread::UpdateSkillSet( IN DWORD dwUID, std::map<std::pair<char,char>, IN std::vector<KSPSetInfo> >& mapSkillSetAdd,
                                 IN std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapSkillSetDel )
{
    std::vector< boost::tuple<char,char,int,int,int,int> > vecAddList;
    std::vector< boost::tuple<char,char,int,int,int,int> > vecDelList;
    std::vector< boost::tuple<char,char,int,int,int,int> >::iterator vitAdd;
    std::vector< boost::tuple<char,char,int,int,int,int> >::iterator vitDel;

    std::map<std::pair<char,char>, IN std::vector<KSPSetInfo> >::iterator mit;
    for( mit = mapSkillSetDel.begin() ; mit != mapSkillSetDel.end() ; ++mit )
    {
        std::vector<KSPSetInfo>::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            std::vector<KSkillSlot>::iterator vit2;
            for( vit2 = vit->m_vecSkills.begin() ; vit2 != vit->m_vecSkills.end() ; ++vit2 )
            {
                vecDelList.push_back( boost::make_tuple(mit->first.first,mit->first.second,vit->m_nSPSetID,vit2->m_nSlotIndex,vit2->m_nSlotID,vit2->m_nSkillID) );
            }
        }
    }

    for( mit = mapSkillSetAdd.begin() ; mit != mapSkillSetAdd.end() ; ++mit )
    {
        std::vector<KSPSetInfo>::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            std::vector<KSkillSlot>::iterator vit2;
            for( vit2 = vit->m_vecSkills.begin() ; vit2 != vit->m_vecSkills.end() ; ++vit2 )
            {
                vecAddList.push_back( boost::make_tuple(mit->first.first,mit->first.second,vit->m_nSPSetID,vit2->m_nSlotIndex,vit2->m_nSlotID,vit2->m_nSkillID) );
            }
        }
    }

    for( vitAdd = vecAddList.begin() ; vitAdd != vecAddList.end() ; )
    {
        bool bDel = false;
        for( vitDel = vecDelList.begin() ; vitDel != vecDelList.end() ; )
        {
            if( vitAdd->get<0>() == vitDel->get<0>() &&
                vitAdd->get<1>() == vitDel->get<1>() &&
                vitAdd->get<2>() == vitDel->get<2>() &&
                vitAdd->get<3>() == vitDel->get<3>() &&
                vitAdd->get<4>() == vitDel->get<4>() &&
                vitAdd->get<5>() == vitDel->get<5>() )
            {
                vitDel = vecDelList.erase( vitDel );
                bDel = true;
                continue;
            }
            ++vitDel;
        }

        if( bDel )
        {
            vitAdd = vecAddList.erase( vitAdd );
            continue;
        }
        ++vitAdd;
    }

    if( !vecAddList.empty() ) {
        for( vitAdd = vecAddList.begin() ; vitAdd != vecAddList.end() ; ++vitAdd ) {
            LIF( UpdateAddSkillTreeSet( dwUID, (int)vitAdd->get<0>(), (int)vitAdd->get<1>(), vitAdd->get<2>(), vitAdd->get<3>(), vitAdd->get<4>(), vitAdd->get<5>() ) );
        }
    }

    if( !vecDelList.empty() ) {
        for( vitDel = vecDelList.begin() ; vitDel != vecDelList.end() ; ++vitDel ) {
            LIF( UpdateDeleteSkillTreeSet( dwUID, (int)vitDel->get<0>(), (int)vitDel->get<1>(), vitDel->get<2>(), vitDel->get<3>(), vitDel->get<4>(), vitDel->get<5>() ) );
        }
    }
}

bool KGSDBThread::CreateNewChar( IN const DWORD dwUID_, IN const std::wstring& strLogin_, IN char cCharType_, OUT KCharacterInfo& kCharacterInfo_ )
{
    /*
    L"{ call dbo.CIGACharacterInfo_insert_1_20100629 ( %d, %d ) }"

    { call dbo.CIGACharacterInfo_insert_1_20100629 ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iCharType_ tinyint

    1 return ( @1 )
    @1 ; OK int
    0        ; 캐릭터 생성 성공
    -1       ; 계정이 존재하지 않음
    -2       ; 캐릭터가 이미 존재함
    -101이하 ; 트랜잭션 에러
    */
    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CIGACharacterInfo_insert_1_20100629 ( %d, %d ) }",
        dwUID_, (int)cCharType_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    if( 0 == nOK ) {
        GetCharacterInfo_1( dwUID_, cCharType_, kCharacterInfo_ );
    }
    else
    {
        START_LOG(cerr, L"캐릭터 생성 실패, UID : " << dwUID_)
            << BUILD_LOG( nOK )
            << BUILD_LOG( strLogin_ )
            << BUILD_LOGc( cCharType_ )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << END_LOG;
    }

    // 신규 캐릭터의 기본적인 스킬레벨에 대한 세팅.
    SiKSkillManager()->SetNewCharSkillInfo( kCharacterInfo_.m_cCharType, kCharacterInfo_.m_dwLevel, kCharacterInfo_.m_kSkillInfo.m_nLvSPPoint, kCharacterInfo_.m_kSkillInfo.m_nMaxSPPoint );
    // 스킬 각성

    return ( nOK == 0 );
}

void KGSDBThread::GetUnLockGroupID( IN DWORD dwUID_, OUT std::map<char, std::set<int> >& mapUnLockGroupID_, IN const std::set<char>& setOldSkillCharacter_ )
{
    /*
    { call dbo.STGASkillTreeGroup_select_20120604 ( %d ) }

    { call dbo.STGASkillTreeGroup_select_20120604 ( @1 ) }
    @1 ; LoginUID int

    n return ( @1, @2 )
    @1 ; CharType tinyint
    @2 ; GroupID int
    */

    mapUnLockGroupID_.clear();

    std::wstringstream stm;
    stm << L"{ call dbo.STGASkillTreeGroup_select_20120604 ( ";
    stm << dwUID_ << L",";

    std::set<char>::const_iterator sit;
    for ( sit = setOldSkillCharacter_.begin(); sit != setOldSkillCharacter_.end(); ++sit ) {
        if ( sit != setOldSkillCharacter_.begin() ) {
            stm << L",";
        }
        stm << static_cast<int>(*sit);        
    }
    stm << L") }";

    DBJIF( m_kODBC.QueryToWString( stm.str() ) );

    stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    //DBJIF( m_kODBC.Query( L"{ call dbo.STGASkillTreeGroup_select_20120604 ( %d, N'%s' ) }",
    //    dwUID_, strNewSkillCharacter_.c_str() ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        char cCharType = -1;
        int nUnLockID = -1;
        VERIFY_QUERY( m_kODBC >> cCharType >> nUnLockID );
        if( nUnLockID != -1 )
            mapUnLockGroupID_[cCharType].insert( nUnLockID );
    }

    START_LOG( clog, L" UID : " << dwUID_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( mapUnLockGroupID_.size() ) << END_LOG;
}

bool KGSDBThread::AddUnLockGroupID( DWORD dwUID_, char cCharType_, int nUnLockGroupID_ )
{
    /*
    { call dbo.STGASkillTreeGroup_insert_20101220 ( @1, @2, @3 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iCharType_ tinyint
    @3 ; @iGroupID   int

    1 select ( @1 )
    @1  ; OK int
    0        ; 성공
    -1       ; 이미 스킬트리그룹이존재함
    -101이하 ; 트랜잭션 에러
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.STGASkillTreeGroup_insert_20101220 ( %d, %d, %d ) }",
        dwUID_, cCharType_, nUnLockGroupID_ ) );
    while( m_kODBC.BeginFetch() )
    {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();

        switch( nOK )
        {
        case  0 : SET_ERROR( NET_OK ); break;
        case -1 : SET_ERROR( ERR_SKILL_24 ); break;  // 이미 스킬트리그룹이 존재함
        default:
            SET_ERROR( ERR_UNKNOWN );
            START_LOG( cerr, L" UnLockGroupID 추가중 오류, UID : " << dwUID_ ) 
                << BUILD_LOG( nUnLockGroupID_ )
                << BUILD_LOG( nOK ) << END_LOG;
        }
        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    return ( NetError::GetLastNetError() == NetError::NET_OK );
}

bool KGSDBThread::UpdateCountItems( IN DWORD const dwUserUID_, IN const std::vector<KItem>& vecDuration_ )
{

    DWORD dwCount = 0;
    DWORD dwFailCount = 0;
    std::vector<KItem>::const_iterator vit;
    for( vit = vecDuration_.begin() ; vit != vecDuration_.end() ; ++vit )
    {
        if ( vit->m_bReserveNewUID == true ) { // 아직 uid 할당 전 아이템을 updatecountitem 하면 원본 아이템이 변경되므로 주의.
            continue;
        }
        int iDiffDurationItem = vit->m_nCount - vit->m_nInitCount;

        if( iDiffDurationItem == 0 ) continue;

        if ( !UpdateCountItem( dwUserUID_, vit->m_ItemUID, iDiffDurationItem ) ) {
            ++dwFailCount;
        }

        ++dwCount;
    }

    _LOG_SUCCESS( dwFailCount == 0, L"수량 아이템 동기화 " << (dwFailCount == 0?L"성공":L"실패") )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( dwCount )
        << BUILD_LOG( dwFailCount ) << END_LOG;
    return true;
}

void KGSDBThread::GetGachaSetObtained( IN std::vector< int >& vecVersions_, IN DWORD dwUID_, OUT std::map< int, std::set< std::pair< DWORD, DWORD > > >& mapGachaSetObtained_ )
{
    mapGachaSetObtained_.clear();

    /*  
    20090724. microcat.  
    20110209. mk8253.  

    L"{ call dbo.GGGA_game_game_user_select ( %d, %d, %d ) }"  

    { call dbo.GGGA_game_game_user_select ( @1, @2, @3 ) }  
    @1 ; LoginUID int  
    @2 ; Version  int  

    n return ( @1 )  
    @1 ; ItemID int  
    @2 ; Type   tinyint  
    */  

    BOOST_FOREACH( int& nVersion, vecVersions_ ) {
        std::set< std::pair< DWORD, DWORD > > setGachaSetObtained;
        setGachaSetObtained.clear();

        DBJIF( m_kODBC.Query( L"{ call dbo.GGGA_game_game_user_select ( %d, %d ) }",
            dwUID_, nVersion ) );
        while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
            DWORD dwGachaID = 0;
            DWORD dwType = 0;
            std::pair< DWORD, DWORD > prData;

            VERIFY_QUERY( m_kODBC >> dwGachaID >> dwType );
            prData.first = dwGachaID;
            prData.second = dwType;

            setGachaSetObtained.insert( prData );
        }
        m_kODBC.EndFetch();

        mapGachaSetObtained_[ nVersion ] = setGachaSetObtained;
    }
}

void KGSDBThread::GetGachaSetObtained( IN std::vector< int >& vecVersions_, IN DWORD dwUID_, OUT std::map< int, std::map< std::pair< DWORD, int >, std::set< DWORD > > >& mapGachaSetObtainedLevel_ )
{
    mapGachaSetObtainedLevel_.clear();

    /*
    L"{ call dbo.GGGAGameGameUser_select ( %d, %d ) }"
    가차 획득 완료 로그
    
    { call dbo.GGGAGameGameUser_select ( @1, @2 ) }
    @1 ; LoginUID int
    @2 ; Version  int

    n return ( @1, @2, @3 )
    @1 ; ItemID    int
    @2 ; Type      tinyint
    @3 ; ItemLevel tinyint
    */

    BOOST_FOREACH( int& nVersion, vecVersions_ ) {
        std::map< std::pair< DWORD, int >, std::set< DWORD > > mapGachaSetObtained;
        mapGachaSetObtained.clear();

        DBJIF( m_kODBC.Query( L"{ call dbo.GGGAGameGameUser_select ( %d, %d ) }",
            dwUID_, nVersion ) );
        while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
            DWORD dwGachaID = 0;
            DWORD dwType = 0;
            int nItemLevel = 0;
            std::pair< DWORD, int > prData;

            VERIFY_QUERY( m_kODBC >> dwGachaID >> dwType >> nItemLevel );
            prData.first = dwGachaID;
            prData.second = nItemLevel;

            if ( mapGachaSetObtained[ prData ].insert( dwType ).second == false ) {
                START_LOG( cerr, L"가챠 획득 완료 데이터에서 타입 중복 있음" )
                    << BUILD_LOG( dwGachaID )
                    << BUILD_LOG( nItemLevel )
                    << BUILD_LOG( dwType )
                    << END_LOG;
            }
        }
        m_kODBC.EndFetch();

        mapGachaSetObtainedLevel_[ nVersion ] = mapGachaSetObtained;
    }
}

void KGSDBThread::LeaveGameStat( KEVENT_MAP_STAT_LEAVE_GAME& kPacket_ )
{

    //{ call dbo.ACC_abandonment_count_collect ( @1, @2, @3, @4, @5, @6, @7 ) }
    //@1 : ModeID tinyint
    //@2 : Difficulty tinyint
    //@3 : PlayTime int
    //@4 : LiftCount tinyint
    //@5 : CharType tinyint
    //@6 : Level tinyint
    //@7 : Count int
    //
    //1 return ( @1 )
    //@1 : OK int
    //
    //OK
    //0 : 성공
    //-101이하 : 트랜잭션 에러

    KEVENT_MAP_STAT_LEAVE_GAME::iterator mit;
    for( mit = kPacket_.begin() ; mit != kPacket_.end() ; ++mit )
    {
        _DBJIF( m_kODBCStat.Query( L"{ call dbo.ACC_abandonment_count_collect ( %d, %d, %d, %d, %d, %d, %d ) }",
            mit->first.m_nGameMode,
            mit->first.m_nDifficulty,
            mit->first.m_nPlayTime,
            (int)mit->first.m_cLeftLife,
            (int)mit->first.m_cCharType,
            mit->first.m_ucLv,
            mit->second ), return, (&m_kODBCStat) );

        m_kODBCStat.EndFetch();
    }
}

bool KGSDBThread::NickToUID( IN const std::wstring& strNick_, OUT DWORD& dwUID_ )
{
    //닉네임 -> UID
    //{ call dbo.ULGA_user_loginuid_select_nick ( @1 ) }
    //@1 ; @strNickname_ [nvarchar](24),
    //
    //1 return ( @1 )
    //@1 ; LoginUID int
    //데이터 없음 ; 해당 유저가 존재하지 않음
    std::wstring strNick = boost::replace_all_copy( strNick_, L"'", L"''" );

    dwUID_ = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.ULGA_user_loginuid_select_nick ( N'%s' ) }",
        strNick.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> dwUID_ );
        m_kODBC.EndFetch();
    }

    if( dwUID_ == 0 )
    {
        START_LOG( clog, L"NickName To UID-Failed( " << strNick_ << L" )" ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"Nickname To UID-Success( " << strNick_ << L" ) : " << dwUID_ ) << END_LOG;
    return true;
}

bool KGSDBThread::LoginToUID( IN const std::wstring& strLogin_, OUT DWORD& dwUID_ )
{
    //로그인 -> UID
    //{ call dbo.ULGA_user_loginuid_select_login ( @1 ) }
    //@1 ; @strLogin_ [nvarchar](20),

    //    1 return ( @1 )
    //    @1 ; LoginUID int
    //    데이터 없음 ; 해당 유저가 존재하지 않음

    dwUID_ = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.ULGA_user_loginuid_select_login ( N'%s' ) }",
        strLogin_.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> dwUID_ );
        m_kODBC.EndFetch();
    }

    if( dwUID_ == 0 )
    {
        START_LOG( clog, L"LoginID To UID-Failed( " << strLogin_ << L" )" ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"LoginID To UID-Success( " << strLogin_ << L" ) : " << dwUID_ ) << END_LOG;
    return true;
}


void KGSDBThread::UpdatePeriodNotice()
{
    std::vector<int> vecNoticeIDList;
    std::map<int, KPeriodNoticeInfo> mapNoticeInfo;
    vecNoticeIDList.clear();
    mapNoticeInfo.clear();
    /*
    L"{ call dbo.GNGAGameNotice_select_20130709 }"

    { call dbo.GNGAGameNotice_select_20130709 }

    n select ( @01, @02, @03 )
    @01iNo        int
    @02strMessage nvarchar( 256 )
    @03iIntervalM int
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.GNGAGameNotice_select_20130709 }" ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KPeriodNoticeInfo kPeriodNoticeInfo;
        VERIFY_QUERY( m_kODBC
            >> kPeriodNoticeInfo.m_nUID
            >> kPeriodNoticeInfo.m_wstrNotice
            >> kPeriodNoticeInfo.m_nPeriod );

        if( kPeriodNoticeInfo.m_nPeriod >= SiKPeriodNotice()->GetMinPeriod() && kPeriodNoticeInfo.m_wstrNotice.length() < 410 ) {
            mapNoticeInfo[kPeriodNoticeInfo.m_nUID] = kPeriodNoticeInfo;
            vecNoticeIDList.push_back( kPeriodNoticeInfo.m_nUID );
        }
    }

    // 기존 uid 에서 변동사항이 있는가
    if( SiKPeriodNotice()->CompareNoticeUID( vecNoticeIDList ) == false ) {
        START_LOG( clog, L"Period Notice UID 변동없음. size : " << vecNoticeIDList.size() ) << END_LOG;
        return;
    }

    if( !mapNoticeInfo.empty() ) {
        // 그렇다면  periodNotice 의 메세지를 바꿔준다.
        SiKPeriodNotice()->UpdatePeriodNotice( mapNoticeInfo );
    } else {
        SiKPeriodNotice()->ClearPeriodNotice();
    }

    START_LOG( clog, L"Period Notice 갱신 완료. Notice Size : " << mapNoticeInfo.size()  ) << END_LOG;
}

void KGSDBThread::UpdateCashGachaKeyBuy( DWORD dwUID_, bool bCash_, int nDiffCount_ )
{
    //{ call dbo.GGGA_game_game_update_buycount ( @1, @2, @3 ) }
    //@1 ; LoginUID int
    //@2 ; Type tinyint
    //@3 ; BuyCount tinyint ; 차이값을 줄것
    // 
    //1 return ( @1 )
    //@1 ; OK int
    // 
    //OK
    //0 ; 성공
    //-1 ; 유저 정보가 존재하지 않음
    //-2 ; GG정보가 존재하지 않음(GGGA_game_game_select_buycount를 선호출 하지 않았음)
    //-101이하 ; 트랜잭션 에러

    int nType = 1;
    if( !bCash_ )
        nType = 2;

    DBJIF( m_kODBC.Query( L"{ call dbo.GGGA_game_game_update_buycount ( %d, %d, %d ) }",
        dwUID_, nType, nDiffCount_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nType )
        << BUILD_LOG( nDiffCount_ )
        << BUILD_LOG( nOK ) << END_LOG;
}

void KGSDBThread::GetCashGachaKeyBuyCount( DWORD dwUID_, PAIR_INT& prData )
{
    //{ call dbo.GGGA_game_game_select_buycount ( @1 ) }
    //@1 ; LoginUID int
    //
    //1 return ( @1 )
    //@1 ; BuyCount tinyint ; 구매제한 횟수가 255회를 넘지 않을 것으로 판단함
    //
    //@1은 껍데기만 올 수도 있음
    //이 경우는 유저 정보가 없는 경우임

    prData.first = 255;
    prData.second = 255;

    DBJIF( m_kODBC.Query( L"{ call dbo.GGGA_game_game_select_buycount ( %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        int nType = -1;
        int nData = 0;
        VERIFY_QUERY( m_kODBC >> nType >> nData );
        switch( nType )
        {

        case 1: // 캐시
            prData.first = nData;
            break;
        case 2: // GP
            prData.second = nData;
            break;
        default:
            START_LOG( cerr, L"가차 키 가져 오는데 알수 없는 타입 UID : " << dwUID_ )
                << BUILD_LOG( nType ) << END_LOG;
        }
    }
}

void KGSDBThread::UpdateDataReportCollet( IN const PAIR_INT prData_ )
{
    /*
    20081024. microcat.

    { call dbo.DRGA_data_report_collect ( %d, %d ) }

    { call dbo.DRGA_data_report_collect ( @1, @2 ) }
    @1 ; Type tinyint
    @2 ; DRCount int

    1 return ( @1 )
    @1 ; OK int
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.DRGA_data_report_collect ( %d, %d ) }",
        prData_.first, prData_.second ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( prData_.first )   //Type(Index)
        << BUILD_LOG( prData_.second )  //Count
        << BUILD_LOG( nOK ) << END_LOG;
}

bool KGSDBThread::UpdateStatSpxCount( int nGameMode_, char cCharType_, int nLv_, int nSpLv_, int nMaxSpx_, DWORD dwCount_ )
{
    //{ call dbo.STGA_skill_tree_collect ( @1, @2, @3, @4, @5, @6 ) }
    //@1 ; GameMode tinyint
    //@2 ; CharType tinyint
    //@3 ; Level tinyint
    //@4 ; SPLv tinyint
    //@5 ; SPExp int
    //@6 ; STCount int
    //
    //1 return ( @1 )
    //@1 ; OK int
    //
    //OK
    //0 ; 성공
    //-101이하 ; 트랜잭션 에러

    _DBJIF( m_kODBCStat.Query( L"{ call dbo.STGA_skill_tree_collect ( %d, %d, %d, %d, %d, %d ) }",
        nGameMode_, cCharType_, nLv_, nSpLv_, nMaxSpx_, dwCount_ ), return false, (&m_kODBCStat) );
    int nOK = -99;
    if( m_kODBCStat.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBCStat >> nOK );
        m_kODBCStat.EndFetch();
    }

    LOG_SUCCESS( nOK == 0 )
        << L"SPX 통계 기록중 " << (nOK == 0 ? L"성공":L"실패" ) << dbg::endl
        << BUILD_LOG( nOK )
        << L"Last Query : " << m_kODBCStat.m_strLastQuery << END_LOG;

    return ( nOK == 0 );

}

void KGSDBThread::GetModeLevel( IN const DWORD& dwUID_, IN const char& cCharType_, OUT std::map<int,KDungeonUserInfo>& mapDifficulty_ )
{
    mapDifficulty_.clear();

    /*
    L"{ call dbo.DCGADungeonClear_select_20130524 ( %d, %d ) }"

    { call dbo.DCGADungeonClear_select_20130524 ( @01, @02 ) }
    @01iLoginUID_input int
    ,   @02iCharType_input int

    n return ( @01, @02 )
    @01: DungeonID  int
    @02: Difficulty int
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.DCGADungeonClear_select_20130524 ( %d, %d ) }",
        dwUID_, cCharType_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        int nMode = 0;
        int nDifficulty = 0;

        VERIFY_QUERY( m_kODBC >> nMode >> nDifficulty );

        KDungeonUserInfo kInfo;
        mapDifficulty_[nMode].m_kModeDifficulty.Set( nDifficulty );

        START_LOG( clog, L"던전 클리어 난이도 정보.. UserUID : " << dwUID_ << L", ModeID : " << nMode << L", Difficulty : " << nDifficulty ) << END_LOG;
    }
    m_kODBC.EndFetch();
}

void KGSDBThread::GetOldModeLevel( IN const DWORD& dwUID_, OUT std::map<int,KDungeonUserInfo>& mapDifficulty_ )
{
    //{ call dbo.UGTE_user_gameinfo_select ( @1 ) }
    //@1 ; LoginUID int
    //
    //1 return ( @1 )
    //@1 ; ModeLevel varbinary(200)
    mapDifficulty_.clear();

    DBJIF( m_kODBC.Query( L"{ call dbo.UGTE_user_gameinfo_select ( %d ) }",
        dwUID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        KSerializer ks;
        KSerBuffer kbuff;
        std::map<char,KncBitStream> mapDifficulty;
        mapDifficulty.clear();

        VERIFY_QUERY( m_kODBC >> kbuff );
        m_kODBC.EndFetch();

        ks.BeginReading( &kbuff );
        ks.Get( mapDifficulty );
        ks.EndWriting();

        std::map<char,KncBitStream>::iterator mit;
        for ( mit = mapDifficulty.begin() ; mit != mapDifficulty.end(); ++mit ) {
            mapDifficulty_[ static_cast<int>(mit->first) ].m_kModeDifficulty = mit->second;
        }
    }
    START_LOG( cwarn, L"옛날 버전 모드 레벨 읽어 들임. UID : " << dwUID_ )
        << BUILD_LOG( mapDifficulty_.size() ) << END_LOG;
}

void KGSDBThread::UpdateModeLevel( IN const DWORD& dwUID_, IN const char& cCharType_, IN OUT std::map<char, KncBitStream>& mapOldDifficulty_, IN OUT std::map<int, KDungeonUserInfo >& mapNewDifficulty_ )
{
    std::map<char,KncBitStream> mapNewDifficulty;
    mapNewDifficulty.clear();

    std::map<int,KDungeonUserInfo>::iterator mitOrg;
    for ( mitOrg = mapNewDifficulty_.begin() ; mitOrg != mapNewDifficulty_.end() ; ++mitOrg ) {
        mapNewDifficulty[ static_cast<char>(mitOrg->first) ] = mitOrg->second.m_kModeDifficulty;

        // 던전 클리어정보 갱신
        // 해당 정보가 초기값 이외의 정보가 있을때만 DB에 갱신 요청을 한다.
        if( CheckUpdateClearData( mitOrg->second.m_kClearData ) ) {
            LIF( m_kHelper.UserGameClear2_merge_20111213( dwUID_, cCharType_, mitOrg->first, mitOrg->second.m_kClearData ) );
        }

        // 던전별 컨트롤 레코드 데이터 DB 갱신 (컨트롤 피드백)
        std::map< int, KUsersDungeonRecordData >::iterator mitDifficultyRecord;
        for ( mitDifficultyRecord = mitOrg->second.m_kClearData.m_mapBestRecord.begin(); mitDifficultyRecord != mitOrg->second.m_kClearData.m_mapBestRecord.end(); ++mitDifficultyRecord ) {
            // DB에서 얻은 값에서 변화가 있는 데이터만 기록
            if ( true == mitDifficultyRecord->second.m_bClearRankChanged || true == mitDifficultyRecord->second.m_bClearTimeChanged ) {
                UpdateUserControlRecordData( dwUID_, cCharType_, mitOrg->first, mitDifficultyRecord->first, mitDifficultyRecord->second );
            }
        }
    }

    std::vector<std::pair<int,int> > vecDiffModeLevel;
    std::map<char,KncBitStream>::iterator mit;
    for ( mit = mapNewDifficulty.begin() ; mit != mapNewDifficulty.end() ; ++mit ) {
        const int& nModeID = mit->first;
        DWORD dwSize = mit->second.GetBitSize();

        std::map<char, KncBitStream>::iterator mit2;
        mit2 = mapOldDifficulty_.find( nModeID );

        for( DWORD dwIndex = 0 ; dwIndex < dwSize ; ++dwIndex )
        {
            if( mit->second.Get( dwIndex ) == false ) 
                continue;

            if( mit2 == mapOldDifficulty_.end() ||
                (mit2 != mapOldDifficulty_.end() && mit2->second.Get(dwIndex) == false ) )
            {
                vecDiffModeLevel.push_back( std::pair<int,int>(nModeID,(int)dwIndex) );
                continue;
            }
        }
    }

    int nCount = 0;
    std::vector<std::pair<int,int> >::iterator vit;
    for ( vit = vecDiffModeLevel.begin() ; vit != vecDiffModeLevel.end() ; ++vit ) {
        int nOK = InsertDungeonClearInfo( dwUID_, cCharType_, vit->first, vit->second );

        if ( nOK == 0 ) ++nCount;
        _LOG_CONDITION( nOK == 0, clog, cwarn, L"Mode Level Update.. Ret : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    START_LOG( clog, L"Mode Level Update .. UID : " << dwUID_ )
        << BUILD_LOG( vecDiffModeLevel.size() )
        << BUILD_LOG( mapNewDifficulty.size() )
        << BUILD_LOG( mapOldDifficulty_.size() )
        << BUILD_LOG( mapNewDifficulty_.size() )
        << BUILD_LOG( nCount )
        << END_LOG;
}

void KGSDBThread::SelectCoupleInfo( IN const std::wstring& strLogin_, IN const DWORD dwUID_, OUT KCoupleInfo& kCoupleInfo_ )
{
    std::wstring sTmpClr;
    kCoupleInfo_.Clear();
    //{ call dbo.CSGA_couple_system_select ( @1 ) }
    //@1 ; LoginUID int
    // 
    //1 return ( @1, @2, @3, @4 )
    //@1 ; LoginUID int ; 내꺼
    //@2 ; CharType tinyint ; 내꺼 선택된 캐릭터
    //@3 ; RegDateA datetime ; 내꺼 커플이 된 시각
    //@4 ; Nick nvarchar(24) ; 빈데이터임 패스
    //union all
    //1 return ( @1, @2, @3, @4 )
    //@1 ; LoginUIDE int ; 커플꺼
    //@2 ; CharTypeE tinyint ; 커플꺼 선택된 캐릭터
    //@3 ; RegDateA datetime ; 커플꺼 커플이 된 시각
    //@4 ; Nick nvarchar(24) ; 커플꺼 닉네임
    //
    //첫번째줄과 두번째줄이 리턴되는 경우 ; 커플 상태임
    //첫번째줄만 리턴되는 경우 ; 상대편이 커플 해제한 상태임
    //리턴되는 것이 없는 경우 ; 커플 상태 아님

    DBJIF( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_select_color ( %d ) }",
        dwUID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kCoupleInfo_.m_dwUID );
        VERIFY_QUERY( m_kODBC >> kCoupleInfo_.m_cCharType );
        VERIFY_QUERY( m_kODBC >> kCoupleInfo_.m_tmRegDate );

        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> kCoupleInfo_.m_dwPartnerUID );
            VERIFY_QUERY( m_kODBC >> kCoupleInfo_.m_cPartnerCharType );
            VERIFY_QUERY( m_kODBC >> kCoupleInfo_.m_tmPartnerRegDate );
            VERIFY_QUERY( m_kODBC >> kCoupleInfo_.m_strPartnerNick );
            VERIFY_QUERY (m_kODBC >> kCoupleInfo_.m_strPartnerNickColor );
        }
    }
    m_kODBC.EndFetch();


    START_LOG( clog, L"커플 정보 가져 옴, Name : " << strLogin_ )
        << BUILD_LOGc( kCoupleInfo_.m_cCharType )
        << BUILD_LOG( kCoupleInfo_.m_tmRegDate )
        << BUILD_LOG( kCoupleInfo_.m_dwPartnerUID )
        << BUILD_LOGc( kCoupleInfo_.m_cPartnerCharType )
        << BUILD_LOG( kCoupleInfo_.m_strPartnerNick )
        << BUILD_LOG( kCoupleInfo_.m_strPartnerNickColor )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

}

void KGSDBThread::SelectCoupleEquip( IN const std::wstring& strLogin_, IN const DWORD dwUID_,
                                     OUT std::vector<KCoupleEquip>& vecEquip_ )
{

    //{ call dbo.CSGA_couple_system_select_equip ( @1 ) }
    //@1 ; LoginUID int
    //
    //n return ( @1, @2, @3, @4 )
    //@1 ; CharType smallint ; -1(유저), 0부터 엘리시스...
    //@2 ; Type tinyint ; 커플 아이템 장착 위치
    //@3 ; ItemUID int
    //@4 ; ItemID int

    DBJIF( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_select_equip ( %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KCoupleEquip equipInfo;
        equipInfo.m_cEquipSlot = -1;
        equipInfo.m_dwItemID = 0;
        equipInfo.m_dwItemUID = 0;
        char cCharType;
        VERIFY_QUERY( m_kODBC >> cCharType
            >> equipInfo.m_cEquipSlot
            >> equipInfo.m_dwItemUID
            >> equipInfo.m_dwItemID );
        vecEquip_.push_back( equipInfo );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"커플 장착 정보 가져 옴, Name : " << strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( vecEquip_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

}

void KGSDBThread::SelectCoupleReqDenyInfo( IN const std::wstring& strLogin_, IN const DWORD dwUID_,
                                           OUT std::set<DWORD>& setDinyUserList_, OUT bool& bAllDiny_ )
{
    setDinyUserList_.clear();
    bAllDiny_   = false;

    //{ call dbo.CSGA_couple_system_select_check ( @1 ) }
    //@1 ; LoginUID int
    //
    //n return ( @1 )
    //@1 ; LoginUIDE int ; 내가 커플 신청을 차단한 유저. -1은 전체임

    DBJIF( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_select_check ( %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        int nUID = 0;
        VERIFY_QUERY( m_kODBC >> nUID );
        if( nUID == -1 )
        {
            bAllDiny_ = true;
            break;
        }
        setDinyUserList_.insert( (DWORD)(nUID) );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"커플 신청 거부 정보 가져 옴, Name : " << strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( setDinyUserList_.size() )
        << BUILD_LOG( bAllDiny_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

bool KGSDBThread::MakeCouple( IN const std::wstring& strLogin_, IN const DWORD dwUID_,
                              IN const std::wstring& strPartnerLogin_, IN const DWORD dwPartnerUID_ )
{
    //{ call dbo.CSGA_couple_system_insert ( @1, @2 ) }
    //@1 ; LoginUID int ; 내꺼
    //@2 ; LoginUIDE int ; 커플꺼
    //
    //1 return ( @1 )
    //@1 ; OK int
    //
    //OK
    //0 ; 성공
    //-1 ; 내꺼 유저 정보가 존재하지 않음
    //-2 ; 커플꺼 유저 정보가 존재하지 않음
    //-3 ; 내가 이미 커플 상태에 있음
    //-4 ; 커플꺼가 이미 커플 상태에 있음
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_insert ( %d, %d ) }",
        dwUID_, dwPartnerUID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;
    case -1: SET_ERROR( ERR_COUPLE_00 ); break;
    case -2: SET_ERROR( ERR_COUPLE_01 ); break;
    case -3: SET_ERROR( ERR_COUPLE_02 ); break;
    case -4: SET_ERROR( ERR_COUPLE_03 ); break;
    default:
        SET_ERROR( ERR_DB_COMMON_00 );
    }

    START_LOG( clog, L"커플 맺기. My Name : " << strLogin_ << L", Your Name : " << strPartnerLogin_ )
        << L" Err Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( dwPartnerUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

bool KGSDBThread::UpdateCoupleEquip( IN const std::wstring& strLogin_, IN const DWORD dwUID_,
                                     IN const KCoupleEquip& coupleEquip_ )
{
    //{ call dbo.CSGA_couple_system_merge_equip ( @1, @2, @3, @4, @5 ) }
    //@1 ; LoginUID int ; 내꺼
    //@2 ; CharType smallint ; -1(유저), 0부터 엘리시스...
    //@3 ; Type tinyint ; 장착 아이템의 위치
    //@4 ; ItemUID bigint
    //@5 ; ItemID int
    // 
    //1 return ( @1 )
    //@1 ; OK int
    // 
    //OK
    //0 ; 성공
    //-1 ; 내꺼 유저 정보가 존재하지 않음
    //-2 ; 내 커플 정보가 존재하지 않음
    //-101이하 ; 트랜잭션 에러
    char cCharType = 0;
    if( SiKCoupleSystem()->IsRingItem( coupleEquip_.m_dwItemID ) )
        cCharType = -1;


    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_merge_equip ( %d, %d, %d, %I64d, %d ) }",
        dwUID_,
        cCharType,
        coupleEquip_.m_cEquipSlot,
        coupleEquip_.m_dwItemUID,
        coupleEquip_.m_dwItemID ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;
    case -1: SET_ERROR( ERR_COUPLE_00 ); break;
    case -2: SET_ERROR( ERR_COUPLE_01 ); break;
    default:
        SET_ERROR( ERR_DB_COMMON_00 );
    }

    START_LOG( clog, L"커플 장착 변경. My Name : " << strLogin_ )
        << L" Err Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( coupleEquip_.m_cEquipSlot )
        << BUILD_LOG( coupleEquip_.m_dwItemID )
        << BUILD_LOG( coupleEquip_.m_dwItemUID )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

bool KGSDBThread::DeleteCoupleEquip( IN const std::wstring& strLogin_, IN const DWORD dwUID_,
                                     IN const KCoupleEquip& coupleEquip_ )
{
    //{ call dbo.CSGA_couple_system_delete_equip ( @1, @2, @3 ) }
    //@1 ; LoginUID int ; 내꺼
    //@2 ; CharType smallint ; -1(유저), 0부터 엘리시스...
    //@3 ; Type tinyint ; 장착 아이템의 위치
    // 
    //1 return ( @1 )
    //@1 ; OK int
    // 
    //OK
    //0 ; 성공
    //-1 ; 내꺼 유저 정보가 존재하지 않음
    //-2 ; 내 커플 정보가 존재하지 않음
    //-3 ; 내 장착 정보가 존재하지 않음
    //-101이하 ; 트랜잭션 에러
    char cCharType = 0;
    if( SiKCoupleSystem()->IsRingItem( coupleEquip_.m_dwItemID ) )
        cCharType = -1;

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_delete_equip ( %d, %d, %d ) }",
        dwUID_, cCharType, coupleEquip_.m_cEquipSlot ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;
    case -1: SET_ERROR( ERR_COUPLE_00 ); break;
    case -2: SET_ERROR( ERR_COUPLE_01 ); break;
    case -3: SET_ERROR( ERR_COUPLE_04 ); break;
    default:
        SET_ERROR( ERR_DB_COMMON_00 );
    }

    START_LOG( clog, L"커플 장착 해제. My Name : " << strLogin_ )
        << L" Err Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( coupleEquip_.m_cEquipSlot )
        << BUILD_LOGc( coupleEquip_.m_dwItemID )
        << BUILD_LOGc( coupleEquip_.m_dwItemUID )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;


    return (nOK == 0);
}

bool KGSDBThread::UpdateCoupleSelectChar( IN const std::wstring& strLogin_, IN const DWORD dwUID_, IN const char cCharType_ )
{
    //{ call dbo.CSGA_couple_system_merge_character ( @1, @2 ) }
    //@1 ; LoginUID int ; 내꺼
    //@2 ; CharType tinyint ; 선택된 캐릭터
    //
    //1 return ( @1 )
    //@1 ; OK int
    //
    //OK
    //0 ; 성공
    //-1 ; 내꺼 유저 정보가 존재하지 않음
    //-2 ; 내 커플 정보가 존재하지 않음
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_merge_character ( %d, %d ) }",
        dwUID_, cCharType_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;
    case -1: SET_ERROR( ERR_COUPLE_00 ); break;
    case -2: SET_ERROR( ERR_COUPLE_01 ); break;
    default:
        SET_ERROR( ERR_DB_COMMON_00 );
    }

    START_LOG( clog, L"커플 선택 캐릭 업데이트. My Name : " << strLogin_ )
        << L" Err Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;


    return (nOK == 0);
}

bool KGSDBThread::UpdateCoupleDenyInfo( IN const std::wstring& strLogin_, IN const DWORD dwUID_, IN const DWORD dwDinyUID_ )
{
    // dwDinyUID_ == 0 이면 모두 막기.

    //{ call dbo.CSGA_couple_system_merge_check ( @1, @2 ) }
    //@1 ; LoginUID int ; 내꺼
    //@2 ; LoginUIDE int ; 차단될 유저의 UID. -1를 넣어주면 전체 유저로 약속.
    //
    //1 return ( @1 )
    //@1 ; OK int
    //
    //OK
    //0 ; 성공
    //-1 ; 내꺼 유저 정보가 존재하지 않음
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_merge_check ( %d, %d ) }",
        dwUID_, (dwDinyUID_== 0 ? (int)(-1) : (DWORD)(dwDinyUID_) ) ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;
    case -1: SET_ERROR( ERR_COUPLE_00 ); break;
    default:
        SET_ERROR( ERR_DB_COMMON_00 );
    }

    _LOG_SUCCESS( nOK == 0, L"커플 요청 거절 업데이트. My Name : " << strLogin_ )
        << L" Err Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwUID_ )
        << BUILD_LOGc( dwDinyUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;


    return (nOK == 0);

}

bool KGSDBThread::DeleteCouple( IN const std::wstring& strLogin_, IN const DWORD dwUID_ )
{
    //{ call dbo.CSGA_couple_system_delete ( @1 ) }
    //@1 ; LoginUID int ; 내꺼
    //
    //1 return ( @1 )
    //@1 ; OK int
    //
    //OK
    //0 ; 성공
    //-1 ; 내꺼 유저 정보가 존재하지 않음
    //-2 ; 내 커플 정보가 존재하지 않음
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_delete ( %d ) }",
        dwUID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;
    case -1: SET_ERROR( ERR_COUPLE_00 ); break;
    case -2: SET_ERROR( ERR_COUPLE_01 ); break;
    default:
        SET_ERROR( ERR_DB_COMMON_00 );
    }

    START_LOG( clog, L"커플 끊기. My Name : " << strLogin_ )
        << L" Err Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

void KGSDBThread::GetWarehouseItems( IN const std::wstring& strLogin_, IN const DWORD dwUID_, OUT std::vector<KSimpleItem>& vecItems_ )
{
    //{ call dbo.CSGA_couple_system_select_warehouse ( @1 ) }
    //@1 ; LoginUID int
    //
    //n return ( @1 )
    //@1 ; ItemUID int
    //@1 ; ItemID int


    DBJIF( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_select_warehouse ( %d ) }",
        dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KSimpleItem kItem;
        kItem.m_dwID = 0;
        VERIFY_QUERY( m_kODBC >> kItem.m_dwUID >> kItem.m_dwID );
        if( kItem.m_dwID <= 0 )
        {
            // LOG
            break;
        }
        
        vecItems_.push_back( kItem );
    }

    START_LOG( clog, L"커플 옷장 정보 가져 오기. My Name : " << strLogin_ )
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( vecItems_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

}

bool KGSDBThread::InsertWarehouseItems( IN const std::wstring& strLogin_, IN const DWORD dwUID_, IN const KSimpleItem& kItem_ )
{
    //{ call dbo.CSGA_couple_system_insert_warehouse ( @1, @2, @3 ) }
    //@1 ; LoginUID int ; 내꺼
    //@2 ; ItemUID bigint
    //@2 ; ItemID int
    // 
    //1 return ( @1 )
    //@1 ; OK int
    // 
    //OK
    //0 ; 성공
    //-1 ; 내꺼 유저 정보가 존재하지 않음
    //-2 ; 내 커플 정보가 존재하지 않음
    //-3 ; 이미 창고에 존재 동일한 아이템이 있음
    //-101이하 ; 트랜잭션 에러


    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_insert_warehouse ( %d, %I64d, %d ) }",
        dwUID_, kItem_.m_dwUID, kItem_.m_dwID ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    switch( nOK )
    {
    case 0:  SET_ERROR( NET_OK ); break;
    case -1: SET_ERROR( ERR_COUPLE_00 ); break;
    case -2: SET_ERROR( ERR_COUPLE_01 ); break;
    case -3: SET_ERROR( ERR_COUPLE_06 ); break;
    default:
        SET_ERROR( ERR_DB_COMMON_00 );
    }

    START_LOG( clog, L"옷장에 넣기. My Name : " << strLogin_ )
        << L" Err Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwUID_ )
        << BUILD_LOG( kItem_.m_dwUID )
        << BUILD_LOG( kItem_.m_dwID )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;


    return (nOK == 0);
}

void KGSDBThread::SaveDivorceMsg( DWORD dwReaderUID_, IN std::wstring& strMsg_ )
{
    //{ call dbo.CSGA_couple_system_merge_message ( @1, @2 ) }
    //@1 ; LoginUID int
    //@2 ; Message nvarchar(256)
    //
    //1 return ( @1 )
    //@1 ; OK int
    //
    //OK
    //0 ; 성공
    //-1 ; 내꺼 유저 정보가 존재하지 않음
    //-2 ; 내 커플 정보가 존재하지 않음
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_merge_message ( %d, N'%s' ) }",
        dwReaderUID_, strMsg_.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    START_LOG( clog, L"커플 해제 메세지 저장. My Name : " << dwReaderUID_ )
        << L" Err Msg : " << NetError::GetLastNetErrMsg() << dbg::endl
        << BUILD_LOG( dwReaderUID_ )
        << BUILD_LOG( strMsg_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

}

void KGSDBThread::ReadDivorceMsg( DWORD dwReaderUID_, OUT std::wstring& strMsg_ )
{
    //{ call dbo.CSGA_couple_system_select_message ( @1 ) }
    //@1 ; LoginUID int
    //
    //1 return ( @1 )
    //@1 ; Message nvarchar(256)

    DBJIF( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_select_message ( %d ) }",
        dwReaderUID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strMsg_ );
        m_kODBC.EndFetch();
    }

    START_LOG( clog, L"커플 해제 메세지 : " << strMsg_ )
        << BUILD_LOG( dwReaderUID_ ) << END_LOG;
}

bool KGSDBThread::PureEraseItem( IN const std::wstring& strLogin_, IN const DWORD dwLoginUID_, IN const GCITEMUID IntemUID_, IN const char cCharType_ )
{
    /*
    L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItem_DelState_update_20131005 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input int
    @02: @02iItemUID_input  bigint
    @04: @03iDelstate_input tinyint
        
    1 return ( @01 )
    @01: OK
        0        : 성공
        -1       : 메인 아이템 정보 없음
        -801~-899: 무시가능 에러
        -901~-999: 치명적인 에러
        -1000    : 사용 중지된 sp인데 사용하려 함
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItem_DelState_update_20131005 ( %d, %I64d, %d ) }", dwLoginUID_, IntemUID_, KItemManager::EDEL_DEL_USER ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

    if( nOK != 0 )
    {
        START_LOG( cerr, L"아이템 삭제 실패. Name : " << strLogin_ )
            << BUILD_LOG( dwLoginUID_ )
            << BUILD_LOGc( nOK )
            << BUILD_LOG( IntemUID_ )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    return (nOK == 0);
}

void KGSDBThread::DeleteWarehouseItem( IN const std::wstring& strLogin_, IN const DWORD dwUID_, IN const KSimpleItem& kItem_ )
{
        //{ call dbo.CSGA_couple_system_delete_warehouse ( @1, @2, @3 ) }
        //@1 ; LoginUID int ; 내꺼
        //@2 ; ItemUID bigint
        //@3 ; itemID int
        //
        //1 return ( @1 )
        //@1 ; OK int
        //
        //OK
        //0 ; 성공
        //-1 ; 내꺼 유저 정보가 존재하지 않음
        //-2 ; 내 커플 정보가 존재하지 않음
        //-3 ; 내 창고 정보가 존재하지 않음
        //-101이하 ; 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.CSGA_couple_system_delete_warehouse ( %d, %I64d, %d ) }",
        dwUID_,
        kItem_.m_dwUID,
        kItem_.m_dwID ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    if( nOK != 0 )
    {
        START_LOG( cerr, L"커플 인벤 아이템 삭제 실패. Name : " << strLogin_ )
            << BUILD_LOGc( nOK )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kItem_.m_dwID )
            << BUILD_LOG( kItem_.m_dwUID )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
}

void KGSDBThread::GetPetCostume( IN const std::wstring& strLogin_, IN OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_, IN const std::vector<KItem>& vecInven_ )
{
    if( mapPetInfo_.empty() ) return;

    //{ call dbo.PIGA_pet_info_select_costume ( @1 ) }
    //@1 ; PetUID bigint
    //
    //1 return ( @1, @2 )
    //@1 ; CostumeUID int
    //@2 ; CostumeID int
    //0         ; 기본 코스튬
    //빈 데이터 ; 기본 코스튬
    //기타      ; 해당 코스튬 아이템 ID

    int nCount = 0;
    std::map<GCITEMUID, KPetInfo>::iterator mit;
    for( mit = mapPetInfo_.begin() ; mit != mapPetInfo_.end() ; ++mit )
    {
        mit->second.m_kCostume.m_dwID = 0;
        mit->second.m_kCostume.m_dwUID = 0;
        mit->second.m_kCostume.m_cItemType = -1;
        _DBJIF( m_kODBC.Query( L"{ call dbo.PIGA_pet_info_select_costume ( %I64d ) }",
            mit->second.m_dwUID ), continue, (&m_kODBC) );
        if( m_kODBC.BeginFetch() )
        {
            VERIFY_QUERY( m_kODBC >> mit->second.m_kCostume.m_dwUID >> mit->second.m_kCostume.m_dwID );
            ++nCount;
        }
        m_kODBC.EndFetch();

        // 인벤에 존재 여부 확인.
        if( mit->second.m_kCostume.m_dwUID != 0 && mit->second.m_kCostume.m_dwID != 0 &&
            ( !SiKPetHelper()->IsPetCostume( mit->second.m_dwID, mit->second.m_cPromotion, mit->second.m_kCostume.m_dwID ) ||
            std::find_if( vecInven_.begin(), vecInven_.end(), boost::bind( &KItem::m_ItemUID, _1 ) == mit->second.m_kCostume.m_dwUID ) == vecInven_.end() ) )
        {
            START_LOG( clog, L"펫 코스튬이 인벤토리에 존재하지 않아서 제거.. Name : " << strLogin_
                << L", ID : " << mit->second.m_kCostume.m_dwID
                << L", UID : " << mit->second.m_kCostume.m_dwUID ) << END_LOG;
            mit->second.m_kCostume.m_dwID = 0;
            mit->second.m_kCostume.m_dwUID = 0;
            mit->second.m_kCostume.m_cItemType = -1;
        }
    }

    START_LOG( clog, L" 펫 코스튬 읽기.. Name : " << strLogin_ )
        << BUILD_LOG( mapPetInfo_.size() )
        << BUILD_LOG( nCount ) << END_LOG;

    return;
}

bool KGSDBThread::UpdatePetCostume( IN const std::wstring& strLogin_, IN const std::map<GCITEMUID, KPetInfo>& mapPetInfo_ )
{
    if( mapPetInfo_.empty() ) return true;

    //{ call dbo.PIGA_pet_info_merge_costume ( @1, @2, @3 ) }
    //@1 ; PetUID bigint
    //@2 ; CostumeUID_ bigint
    //@3 ; CostumeID_ int
    //0         ; 기본 코스튬
    //빈 데이터 ; 기본 코스튬
    //기타      ; 해당 코스튬 아이템 ID
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-101이하 ; 트랜잭션 에러


    int nFailCoount = 0;
    std::map<GCITEMUID, KPetInfo>::const_iterator mit;
    for( mit = mapPetInfo_.begin() ; mit != mapPetInfo_.end() ; ++mit )
    {
        // PC방 PET에 대해서는 저장하지 않는다.
        if( mit->second.m_dwUID > SiKIPAdvantage()->GetMaximumPCBangItemUID() ) {
            _DBJIF( m_kODBC.Query( L"{ call dbo.PIGA_pet_info_merge_costume ( %I64d, %I64d, %d ) }",
                mit->second.m_dwUID, mit->second.m_kCostume.m_dwUID, mit->second.m_kCostume.m_dwID ), continue, (&m_kODBC) );

            int nOK = -99;
            if( m_kODBC.BeginFetch() )
            {
                VERIFY_QUERY( m_kODBC >> nOK );

                if( nOK != 0 )
                {
                    ++nFailCoount;
                    START_LOG( cerr, L"펫 코스튬 업데이트시 오류 Name : " << strLogin_ )
                        << BUILD_LOG( nOK )
                        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
                }
            }
            m_kODBC.EndFetch();
        }
    }

    START_LOG( clog, L" 펫 코스튬 업데이트.. Name : " << strLogin_ )
        << BUILD_LOG( mapPetInfo_.size() )
        << BUILD_LOG( nFailCoount ) << END_LOG;

    return (nFailCoount == 0);
}

void KGSDBThread::GetCharacterPetEquip( IN const DWORD& dwUserUID_, IN OUT std::map<char, KCharacterInfo>& mapCharacterInfo_ )
{
    /*
    L"{ call dbo.UIGAUserItemPet_select_all_character_20130510 ( %d ) }"

    { call dbo.UIGAUserItemPet_select_all_character_20130510 ( @01 ) }
    @01: @01iLoginUID_input int

    n return ( @01, @02, @03, @04, @05, @06 )
    @01: CharType  tinyint
    @02: PetUID    bigint
    @03: PetID     int
    @04: PetName   nvarchar( 20 )
    @05: [Exp]     int
    @06: Promotion tinyint
    */

    std::map<GCITEMUID, KPetInfo>::const_iterator mitPet;
    std::map<char, KCharacterInfo>::iterator mitChar;

    DBJIF( m_kODBC.Query( L"{ call dbo.UIGAUserItemPet_select_all_character_20130510 ( %d ) }", dwUserUID_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        GCITEMUID dwPetUID = 0;
        char cCharType = -1;
        GCITEMID dwPetID = 0;
        std::wstring wstrPetName;
        int nEXP = 0;
        int nPromotion = 0;
        VERIFY_QUERY( m_kODBC >> cCharType >> dwPetUID >> dwPetID >> wstrPetName >> nEXP >> nPromotion );
        mitChar = mapCharacterInfo_.find( cCharType );
        if( mitChar != mapCharacterInfo_.end() ) {
            mitChar->second.m_kEquipPetInfo.m_cCharType = cCharType;
            mitChar->second.m_kEquipPetInfo.m_dwUID = dwPetUID;
            mitChar->second.m_kEquipPetInfo.m_dwID = dwPetID;
            mitChar->second.m_kEquipPetInfo.m_strName = wstrPetName;
            mitChar->second.m_kEquipPetInfo.m_dwEXP = nEXP;
            mitChar->second.m_kEquipPetInfo.m_iLevel = SiKPetHelper()->GetPetLevel( nEXP );
            mitChar->second.m_kEquipPetInfo.m_cPromotion = nPromotion;
        }
    }
}

void KGSDBThread::ResetCharacterPetEquip( IN const std::wstring& strLogin_, IN const DWORD dwUserUID_ )
{
    /*
    캐릭터의 펫 장착 정보 모두 삭제
    L"{ call dbo.PIGAPetInfoCharacter_update_20120103 ( %d ) }"

    { call dbo.PIGAPetInfoCharacter_update_20120103 ( @1 ) }
    @1: @iLoginUID_ int

    1 return ( @1 )
    @1: OK  int
    0       : 성공
    -1      : 펫을 장착한 캐릭터가 없음
    -101이하: 트랜젝션 에러
    */

    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.PIGAPetInfoCharacter_update_20120103 ( %d ) }", dwUserUID_ ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();
    
    _LOG_SUCCESS( nOK == 0, L" 캐릭터가 차고 있는 펫 초기화. .Name : " << strLogin_ << L", OK : " <<  nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

void KGSDBThread::SetCharacterPetEquip( IN const DWORD& dwUserUID_, IN const std::wstring& strLogin_, IN const GCITEMUID dwPetUID_, IN const char cCharType_ )
{
    /*
    캐릭터의 펫 장착 정보 설정
    L"{ call dbo.PIGAPetInfoCharacter_merge_20120103 ( %d, %I64d, %d ) }"

    { call dbo.PIGAPetInfoCharacter_merge_20120103 ( @1, @2, @3 ) }
    @1: @iLoginUID_  int
    @2: @iPetUID_    bigint
    @3: @iCharType_  tinyint

    1 return ( @1 )
    @1: OK  int
    0       : 성공
    -1      : 펫 정보가 없음
    -101이하: 트랜젝션 에러
    */

    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.PIGAPetInfoCharacter_merge_20120103 ( %d, %I64d, %d ) }",
        dwUserUID_, dwPetUID_, static_cast<UCHAR>(cCharType_) ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L" 캐릭터가 차고 있는 펫 설정.. Name : " << strLogin_ << L", OK : " <<  nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

bool KGSDBThread::DonationGP( IN const std::wstring& strLogin_, IN const DWORD dwUID_, IN const int nGP_ )
{
    //{ call dbo.CGGA_contribution_gp_merge ( @1, @2 ) }
    //@1 ; LoginUID int
    //@2 ; ContributionGP int 변화량.
    //
    //1 return ( @1 )
    //@1 ; OK int
    //
    //OK
    //0 ; 성공
    //-1 ; 유저가 존재하지 않음
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CGGA_contribution_gp_merge ( %d, %d ) }",
        dwUID_, nGP_ ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return (nOK == 0);

}

void KGSDBThread::GetDonationPointPerUser( IN const DWORD dwUID_, OUT int& nDonationGP_ )
{
    //{ call dbo.CGGA_contribution_gp_select_user ( @1 ) }
    //@1 ; LoginUID int
    //
    //1 return ( @1 )
    //@1 ; Contribution int
    nDonationGP_ = 0;
    DBJIF( m_kODBC.Query( L"{ call dbo.CGGA_contribution_gp_select_user ( %d ) }", dwUID_ ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nDonationGP_ );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L" 유저의 누적 기부 금액 가져옴. UID : " << dwUID_ << L", Point : " << nDonationGP_ ) << END_LOG;
}

void KGSDBThread::InsertSnowCoin( IN const DWORD& dwUID_, IN const int nFactor_ )
{
    /*
    mk8253. 20110413.
    웹연동 아이템 쌓기

    { call dbo.OWWEOccupationWar_insert ( @1, @2 ) }
    @1 ; @iLoginUID_ [int]
    @2 ; @iCount_    [int]

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -1       ; 유저 정보가 없음
    -2       ; Count가 1보다 작음
    -101이하 ; 트랜젝션 에러
    */
    int nOK = -99;

    if( SiKGSSimLayer()->CheckAuthTypeFlag( KGSSimLayer::AT_NETMARBLE ) ) {
        DBJIF( m_kODBC.Query( L"{ call dbo.OWWEOccupationWar_insert ( %d, %d ) }", dwUID_, nFactor_ ) );
    }
    else {
        DBJIF( m_kODBC.Query( L"{ call dbo.REWERouletteEvent_insert ( %d, %d ) }", dwUID_, nFactor_ ) );
    }

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"Ret : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

bool KGSDBThread::UpdateCountItem( IN const DWORD dwUID_, IN const GCITEMUID ItemUID, IN const int nDellFactor_ )
{
    //수량아이템 개수 변경 획득 및 사용, 삭제
    /*
    L"{ call dbo.UIGAUserItemDuration_merge_20131028 ( %d, %I64d, %d ) }"

    { call dbo.UIGAUserItemDuration_merge_20131028 ( @01, @02, @03 ) }
        @01iLoginUID_input      int
    ,   @02iItemUID_input       bigint
    ,   @03iDuration_diff_input int
        
    1 return ( @01, @02 )
        @01iOK       int
            0        : 성공
            -1       : 유저 정보 없음
            -2       : 메인 아이템 정보 없음
            -801~-899: 무시가능 에러
            -901~-999: 치명적인 에러
            -1000    : 사용 중지된 sp인데 사용하려 함
        @02iDuration int
    */
    //!!주의 : nDellFactor_ 가 음수이면 수량 아이템을 소모합니다. 양수이면 수량 증가합니다.
    int nRet = -99;
    int nCount = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.UIGAUserItemDuration_merge_20131028 ( %d, %I64d, %d ) }",
        dwUID_, ItemUID, nDellFactor_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nRet >> nCount );
        m_kODBC.EndFetch();
    }

    LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nRet ) );

    _LOG_SUCCESS( (0 == nRet), L"Count Item Update, OK : " << nRet << L", count : " << nCount << L", Query : " << m_kODBC.m_strLastQuery ) << END_LOG;

    return ( 0 == nRet );
}

bool KGSDBThread::UpdatePVExpInfo(IN const DWORD dwUID_)
{
    int nOK = -99;

    DBJIF2(m_kODBC.Query(L"{ call dbo.LUMA_PvE_Exp_Add ( %d ) }", dwUID_));

    if (m_kODBC.BeginFetch()) {
        VERIFY_QUERY(m_kODBC >> nOK);
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS(nOK == 0, L"Ret : " << nOK)
        << BUILD_LOG(m_kODBC.m_strLastQuery) << END_LOG;

    return (0 == nOK);
}

bool KGSDBThread::UpdateCharacterInfo( IN const std::wstring strLogin_, IN const DWORD dwUID_, IN const std::map<char, KCharacterInfo>& mapCharacterInfo_ )
{

    //{ call dbo.CGGA_character_gameinfo_update ( @1, @2, @3, @4, @5 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iCharType_ tinyint
    //0 ; 엘리시스
    //1 ; 리르
    //2 ; 아르메
    //3 ; 라스
    //4 ; 라이언
    //5 ; 로난
    //6 ; 에이미
    //7 ; 진
    //@3 ; @iExpS4_ bigint  ; 차이값만 주쇼
    //@4 ; @iWin_ int     ; 차이값만 주쇼
    //@5 ; @iLose_ int    ; 차이값만 주쇼
    //@6 ; @iExp bigint ; 차이값만 주쇼
    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-1       ; 유저 정보가 없음
    //-2       ; 캐릭터가 없음
    //-101이하 ; 트랜잭션 에러

    __int64 biExpRewardLimit = SiKResultManager()->GetExpRewardLimit();

    std::map<char, KCharacterInfo>::const_iterator mit;
    bool bRet = true;
    for( mit = mapCharacterInfo_.begin() ; mit != mapCharacterInfo_.end() ; mit++ )
    {
        __int64 biAdditionalExp = mit->second.m_biExp - mit->second.m_biInitExp;
        // 경험치 롤백에 대한 추가 코드.
        if( biAdditionalExp < 0 ) {
            START_LOG( cerr, L"CharExp Minus Value. Name : " << strLogin_ )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOGc( mit->second.m_cCharType )
                << BUILD_LOG( mit->second.m_biExp )
                << BUILD_LOG( mit->second.m_biInitExp )
                << BUILD_LOG( biAdditionalExp ) << END_LOG;

            biAdditionalExp = 0;
        }

        // 과다 경험치에 대한 추가코드
        __int64 biExpRewardLimit = SiKResultManager()->GetExpRewardLimit();
        if ( biAdditionalExp > biExpRewardLimit ) {
            START_LOG( cerr, L"CharExp Over Value. Name : " << strLogin_ )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOGc( mit->second.m_cCharType )
                << BUILD_LOG( mit->second.m_biExp )
                << BUILD_LOG( mit->second.m_biInitExp )
                << BUILD_LOG( biAdditionalExp ) << END_LOG;

            if ( SiKResultManager()->IsClippingExpRewardLimit() ) {
                biAdditionalExp = biExpRewardLimit;
            }
        }

        int nWind = mit->second.m_iWin - mit->second.m_iInitWin;
        int nLose = mit->second.m_iLose - mit->second.m_iInitLose;
        __int64 biTotalExp = mit->second.m_biTotalExp - mit->second.m_biInitTotalExp;
        int nOK = -99;
        if( biAdditionalExp != 0 || nWind != 0 || nLose != 0 || biTotalExp != 0 ) { // DB 부하를 줄이기 위해서 값의 변경이 있을때만 해당 sp 호출.
            DBJIF2( m_kODBC.Query( L"{ call dbo.CGGA_character_gameinfo_update (  %d, %d, %I64d, %d, %d, %I64d ) }",
                dwUID_,
                (int)mit->second.m_cCharType,
                biAdditionalExp,
                nWind,
                nLose,
                biTotalExp ) );

            if( m_kODBC.BeginFetch() )
            {
                VERIFY_QUERY( m_kODBC >> nOK );
                m_kODBC.EndFetch();
                if( nOK != 0 ) bRet = false;
            }
        } else {
            nOK = 0;
        }

        _LOG_SUCCESS( nOK == 0, L"캐릭터 기록 Name : " << strLogin_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery )
            << BUILD_LOGc( mit->second.m_cCharType )
            << BUILD_LOG( mit->second.m_biExp )
            << BUILD_LOG( mit->second.m_biInitExp )
            << BUILD_LOG( mit->second.m_dwLevel ) << END_LOG;
    }

    return bRet;
}

int KGSDBThread::GetTodayEventCount( IN const DWORD dwUID_, IN const int nEventType_ )
{
    //{ call dbo.SDGA_server_data_select_integer_user ( @1, @2, @3 ) }
    //@1 ; @iLoginUID_ [int]
    //@2 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
    //@3 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
    //
    //1 return ( @1 )
    //@1 ; Data int

    int nRes = 0;
    DBJIF3( m_kODBC.Query( L"{ call dbo.SDGA_server_data_select_integer_user (  %d, N'%s', %d ) }",
        dwUID_, (LPCTSTR)CTime::GetCurrentTime().Format( L"%Y%m%d" ), nEventType_ ), return -1 );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nRes );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, m_kODBC.m_strLastQuery )
        << BUILD_LOG( nRes ) << END_LOG;
    return nRes;
}

int KGSDBThread::GetTodayEventCount( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_ )
{
    //{ call dbo.SDGA_server_data_select_integer_user ( @1, @2, @3 ) }
    //@1 ; @iLoginUID_ [int]
    //@2 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
    //@3 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
    //
    //1 return ( @1 )
    //@1 ; Data int

    int nRes = 0;
    DBJIF3( m_kODBC.Query( L"{ call dbo.SDGA_server_data_select_integer_user (  %d, N'%s', %d ) }",
        dwUID_, (LPCTSTR)CTime(tmDate_).Format( L"%Y%m%d" ), nEventType_ ), return -1 );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nRes );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, m_kODBC.m_strLastQuery )
        << BUILD_LOG( nRes ) << END_LOG;
    return nRes;
}

bool KGSDBThread::SetTodayEventDoneDiff( IN const DWORD dwUID_, IN const int nEventType_ )
{
    /*
    L"{ call dbo.SDGAServerDataIntegerUser_merge_diff ( %d, N'%s', %d, %d ) }"

    { call dbo.SDGAServerDataIntegerUser_merge_diff ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_   int
    @2 ; @sdtRegDateD_ smalldatetime ; 일단위까지만 인식함
    @3 ; @iType_       tinyint       ; 서버에서 원하는 숫자별로 분류됨
    @4 ; @iDataDiff_   int           ; 차이값을 주쇼

    1 return ( @1 )
    @1 ; OK int
    0        ; 성공
    -1       ; 유저 정보가 없음
    -101이하 ; 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.SDGAServerDataIntegerUser_merge_diff (  %d, N'%s', %d, 1 ) }",
        dwUID_, (LPCTSTR)CTime::GetCurrentTime().Format( L"%Y%m%d" ), nEventType_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"일일 이벤트??? 셋팅 할때 오류.., UID : " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;


    return (nOK == 0);
}

bool KGSDBThread::SetTodayEventDoneDiff( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_ )
{
/*
    L"{ call dbo.SDGAServerDataIntegerUser_merge_diff ( %d, N'%s', %d, %d ) }"

    { call dbo.SDGAServerDataIntegerUser_merge_diff ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_   int
    @2 ; @sdtRegDateD_ smalldatetime ; 일단위까지만 인식함
    @3 ; @iType_       tinyint       ; 서버에서 원하는 숫자별로 분류됨
    @4 ; @iDataDiff_   int           ; 차이값을 주쇼

    1 return ( @1 )
    @1 ; OK int
    0        ; 성공
    -1       ; 유저 정보가 없음
    -101이하 ; 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.SDGAServerDataIntegerUser_merge_diff (  %d, N'%s', %d, %d ) }",
        dwUID_, (LPCTSTR)CTime(tmDate_).Format( L"%Y%m%d" ), nEventType_, nData_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"일일 이벤트??? 셋팅 할때 오류.., UID : " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;


    return (nOK == 0);
}

bool KGSDBThread::SetTodayEventDone( IN const DWORD dwUID_, IN const int nEventType_, IN const time_t tmDate_, IN const int nData_ )
{
    /*
    L"{ call dbo.SDGAServerDataIntegerUser_merge ( %d, N'%s', %d, %d ) }"

    { call dbo.SDGAServerDataIntegerUser_merge ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUID_   int
    @2 ; @sdtRegDateD_ smalldatetime
    @3 ; @iType_       tinyint
    @4 ; @iData_       int

    1 return ( @1 )
    @1 ; OK int
    0        ; 성공
    -1       ; 유저 정보가 없음
    -101이하 ; 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.SDGAServerDataIntegerUser_merge (  %d, N'%s', %d, %d ) }",
        dwUID_, (LPCTSTR)CTime(tmDate_).Format( L"%Y%m%d" ), nEventType_, nData_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0, L"일일 이벤트??? 셋팅 할때 오류.., UID : " << dwUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;


    return (nOK == 0);
}

void KGSDBThread::WaitingItemMergeDB( IN const DWORD dwUID_ )
{

    //20081220. microcat.
    //
    //{ call dbo.IWGA_item_waitingitem_check ( @1 ) }
    //@1 ; LoginUID int
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0  ; 아이템을 받아야 됨
    //-1 ; 유저 정보가 없음
    //-2 ; 대기 아이템이 없음
    int iOK = -99;

    DBJIF( m_kODBC.Query( L"{ call dbo.IWGA_item_waitingitem_check ( %d ) }", dwUID_ ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> iOK );
        m_kODBC.EndFetch();
    }

    if( iOK != 0 ) return;

    //20081220. microcat.

    //{ call dbo.IWGA_item_waitingitem_merge ( @1 ) }
    //@1 ; LoginUID int

    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 아이템을 받아야 됨
    //-1       ; 유저 정보가 없음
    //-101이하 ; 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.IWGA_item_waitingitem_merge ( %d ) }", dwUID_ ) );
    if( m_kODBC.BeginFetch() )
    {
        iOK = -99;
        VERIFY_QUERY( m_kODBC >> iOK );
        m_kODBC.EndFetch();
        if( iOK != 0 )
        {
            START_LOG( cerr, L"IWGA_item_waitingitem_merge 실패" )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        }
    }
}

void KGSDBThread::GetSantaComment( IN const DWORD dwUID_, IN const char& cCharType_, OUT std::vector<KSantaInfo>& vecGiftList_, IN const int nLanguageCode_ )
{
    /*
    L"{ call dbo.SSGASantaSystemEvent_select_20130521 ( %d, %d, %d ) }"

    { call dbo.SSGASantaSystemEvent_select_20130521 ( @01, @02, @03 ) }
    @01: @01iLoginUID_input         int
    @02: @02iCharType_input         int
    @03: @03iServerLanguageID_input int

    n return ( @1, @2, @3, @4 )
    @1: EventUID int
    @2: UserType tinyint
    @3: Comment  nvarchar( 100 )
    @4: ItemID   int
    */

    vecGiftList_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.SSGASantaSystemEvent_select_20130521 ( %d, %d, %d ) }", dwUID_, cCharType_, nLanguageCode_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA ) {
        KSantaInfo kData;

        VERIFY_QUERY( m_kODBC >> kData.m_dwEventUID
                              >> kData.m_dwUserType
                              >> kData.m_wstrComment
                              >> kData.m_ItemID );

        vecGiftList_.push_back( kData );
    }

    START_LOG( clog, L"산타 아이템 정보 얻어오기 완료. Size:" << vecGiftList_.size() )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

void KGSDBThread::AddServerCountData( int nData_, int nType_ )
{
    //{ call dbo.SDGA_server_data_merge_integer ( @1, @2, @3 ) }
    //@1 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
    //@2 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
    //@3 ; @iData_ [int]                  ; 차이값을 주쇼

    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-101이하 ; 트랜잭션 에러
    DBJIF( m_kODBC.Query( L"{ call dbo.SDGA_server_data_merge_integer ( N'%s', %d, %d ) }",
        (LPCTSTR)CTime::GetCurrentTime().Format( L"%Y%m%d" ), nType_, nData_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

void KGSDBThread::UpdateMissionData( IN const DWORD dwUID_, IN const char& cCharType_, IN const std::vector<KMissionData>& vecMissionData_ )
{
    /*
    L"{ call dbo.MSGAMissionSystemSub_update_20130527 ( %d, %d, %d, %d, %d ) }"

    { call dbo.MSGAMissionSystemSub_update_20130527 ( @01, @02, @03, @04, @05 ) }
    @01iLoginUID_input int
    ,   @02iCharType_input int
    ,   @03iMainID_input   int
    ,   @04iSubID_input    int
    ,   @05iCNT_diff_input int

    1 return ( @01 )
    @01: OK int
    0        : 성공
    -1       : 서브미션 정보 없음
    -801~-899: 무시가능 에러
    -901~-999: 치명적인 에러
    */

    std::vector<KMissionData>::const_iterator vit;
    for( vit = vecMissionData_.begin() ; vit != vecMissionData_.end() ; ++vit ) {
        std::map<int,int>::const_iterator mit;
        for( mit = vit->m_mapCompletionInfo.begin() ; mit != vit->m_mapCompletionInfo.end() ; ++mit ) {
            if( vit->m_nInitSubCnt >= mit->second ) { // 미션 롤백에 대한 보정 처리 추가.
                continue;
            }
            DBJIF( m_kODBC.Query( L"{ call dbo.MSGAMissionSystemSub_update_20130527 ( %d, %d, %d, %d, %d ) }",
                dwUID_, cCharType_, vit->m_dwMissionID, mit->first, mit->second - vit->m_nInitSubCnt ) );
            if( m_kODBC.BeginFetch() ) {
                int nOK = -99;
                VERIFY_QUERY( m_kODBC >> nOK );
                m_kODBC.EndFetch();

                LIF( m_kHelper.IgnorableErrorProcessingAfterCallSP( nOK ) );

                if( nOK != 0 ) {
                    START_LOG( cerr, L"Mission Update 실패 .. OK : " << nOK << L" Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
                } // nOK 값 체크
            } // BeginFetch
        } // m_mapCompletionInfo for문
    } // vecMissionData
}

bool KGSDBThread::GetServerList( IN KODBC& kODBC_, IN const int nProtocol_, OUT std::vector<KServerInfo>& vecServerList_ )
{
    vecServerList_.clear();
    KLocker lock( kODBC_.GetCS() );
     /*
    L"{ call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( %d ) }"

    { call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( @1 ) }
    @1 ; @iProtocolVersion_ int

    n select ( @1, @2, @3, @4, @5, @6, @7 )
        @1  ; ServerPart smallint
        @2  ; ServerName nvarchar( 20 )
        @3  ; ServerIP   varchar ( 25 ) // 인증용 ip.
        @4  ; ServerPort int
        @5  ; UserNum    int
        @6  ; MaxNum     int
        @7  ; NO         int
        @8  ; ServerIP2  varchar ( 25 ) // 전달용 ip.
    */

    DBJIF2( kODBC_.Query( L"{ call dbo.CUGAConcurrentUser_select_All_by_ProtocolVersion ( %d ) }",
        nProtocol_ ) );

    while( kODBC_.Fetch() != SQL_NO_DATA )
    {
        KServerInfo kServerInfo;
        kServerInfo.m_nUserProtocolVer = nProtocol_;

        VERIFY_QUERY( kODBC_ >> kServerInfo.m_iServerPart 
            >> kServerInfo.m_strName
            >> kServerInfo.m_strIP
            >> kServerInfo.m_usPort
            >> kServerInfo.m_iUserNum
            >> kServerInfo.m_iMaxUserNum
            >> kServerInfo.m_dwUID
            >> kServerInfo.m_strConnIP );

        vecServerList_.push_back( kServerInfo );
    }

    kODBC_.EndFetch();

    std::vector<KServerInfo>::iterator vit;
    DBJIF2( kODBC_.Query( L"{ call dbo.SIGAServerInfo_select }" ) );
    while( kODBC_.Fetch() != SQL_NO_DATA )
    {
        DWORD dwUID;
        std::pair<int,int> prRange;
        VERIFY_QUERY( kODBC_ >> dwUID
            >> prRange.first
            >> prRange.second );
        vit = std::find_if( vecServerList_.begin(), vecServerList_.end(),
            boost::bind( &KServerInfo::m_dwUID, _1 ) == dwUID );
        if ( vit != vecServerList_.end() ) {
            vit->m_prLvRange = prRange;
        }
    }
    kODBC_.EndFetch();

    return true;
}

bool KGSDBThread::GetServerName( IN KODBC& kODBC_, IN const std::wstring& strIP_, IN const int nPort_ )
{
    KLocker lock( kODBC_.GetCS() );
/*
    _DBJIF( kODBC_.Query( L"{ call dbo.sp_executesql "
        L"( N'select ServerName from dbo.ConnectStatusDB with(nolock) where ServerIP = @1 and ServerPort = @2', "
        L"N'@1 nvarchar(15), @2 int', N'%s', %d ) }",
        strIP_.c_str(), nPort_ ), return false, (&kODBC_) );
*/
    /*
     L"{ call dbo.CUGAConcurrentUser_select_ServerID_ServerName_by_IP_Port ( N'%s', %d ) }"

     { call dbo.CUGAConcurrentUser_select_ServerID_ServerName_by_IP_Port ( @1, @2 ) }
     @1 ; @strIP_ nvarchar( 15 )
     @2 ; @iPort_ smallint

     1 select ( @1 )
     @1 ; ServerID   int
     @2 ; ServerName nvarchar( 20 )
     */
    DBJIF2( kODBC_.Query( L"{ call dbo.CUGAConcurrentUser_select_ServerID_ServerName_by_IP_Port ( N'%s', %d ) }",
        strIP_.c_str(), nPort_ ) );

    std::wstring strServerName;
    int nServerID = 0;
    if( kODBC_.BeginFetch() ) {
        VERIFY_QUERY( kODBC_ >>  nServerID
                             >>  strServerName );
    }
    kODBC_.EndFetch();
    SiKGameServer()->SetServerID( nServerID );
    if( SiKGameServer()->SetServerName( strServerName ) ) { // 이름 변경시 센터 서버 재접속 하게 수정.
        SiKGameServer()->ReConnectCenterServer();
    }
    return true;
}

bool KGSDBThread::UpdateCCU( IN KODBC& kODBC_, IN const int nProtocol_, IN const std::wstring& strIP_, IN const int nPort_, IN const int nCCU_, IN const int nServerID_ )
{
    KLocker lock( kODBC_.GetCS() );
/*
    _DBJIF( kODBC_.Query( L"{ call dbo.sp_executesql "
        L"( N'update a with (updlock) set a.UserNum = @1, a.ExtraFlag = @2 from dbo.ConnectStatusDB as a with (updlock) where ServerIP = @3 and ServerPort = @4', "
        L"N'@1 int, @2 int, @3 nvarchar(15), @4 int', %d, %d, N'%s', %d ) }",
        nCCU_, nProtocol_, strIP_.c_str(), nPort_ ), return false, (&kODBC_) );
*/
    /*
    L"{ call dbo.CUGAConcurrentUser_update_CU_and_ProtocolVersion_by_ServerID ( %d, %d, %d ) }"

    { call dbo.CUGAConcurrentUser_update_CU_and_ProtocolVersion_by_ServerID ( @1, @2, @3 ) }
    @1 ; @iServerID_        int
    @2 ; @iCU_              int
    @3 ; @iProtocolVersion_ int

    1 select ( @1 )
    @1  ; OK int
    0        ; 성공
    -1       ; 해당서버ID가유효하지않음
    -101이하; 트랜잭션에러
    */
    DBJIF2( kODBC_.Query( L"{ call dbo.CUGAConcurrentUser_update_CU_and_ProtocolVersion_by_ServerID ( %d, %d, %d ) }",
        nServerID_, nCCU_, nProtocol_ ) );

    if( kODBC_.BeginFetch() )
    {
        int nOK = -99;
        VERIFY_QUERY( kODBC_ >> nOK );
        kODBC_.EndFetch();
        if( nOK != 0 )
        {
            START_LOG( cerr, L"서버 동접 갱신 실패 .. OK : " << nOK << L" Query : " << kODBC_.m_strLastQuery ) << END_LOG;
        }
    }

    return true;
}


bool KGSDBThread::CheckGameCoupon( IN const std::wstring& strSerialNum_, IN const std::wstring& strPassWD_, IN const int nEventUID_,
                                   OUT int& nCouponNID_, OUT int& nItemNID_ )
{
    //{ call dbo.GCGA_game_coupon_check ( @1, @2, @3, @4, @5 ) }
    //@1 ; 쿠폰번호          nvarchar( 20 )
    //@2 ; 패스워드          nvarchar( 20 )
    //@3 ; EventNID          int
    //@4 ; 패스워드 체크여부 bit
    //@5 ; 이벤트 체크여부   bit
    //
    //1 return ( @1, @2, @3 )
    //@1 ; OK        int
    //0  ; 성공
    //-1 ; 쿠폰이 존재하지 않음
    //-2 ; 쿠폰 체크 조건이 다름(패스워드)
    //-3 ; 쿠폰 체크 조건이 다름(이벤트)
    //-4 ; 쿠폰이 이미 사용되었음
    //-5 ; 패스워드가 틀림
    //-6 ; 쿠폰과 이벤트 정보가 연결되어 있지 않음
    //-7 ; 쿠폰과 아이템 정보가 연결되어 있지 않음
    //@2 ; 쿠폰NID   int
    //쿠폰번호를 대체하는 숫자값
    //@3 ; 아이템NID int
    //쿠폰과 연결된 아이템 목록을 정하는 숫자값
    nCouponNID_ = -1;
    nItemNID_ = -1;
    int nOK = -99;
    SET_ERROR( ERR_UNKNOWN );
    DBJIF2( m_kODBC.Query( L"{ call dbo.GCGA_game_coupon_check ( N'%s', N'%s', %d, %d, %d ) }",
                            strSerialNum_.c_str(), strPassWD_.c_str(), nEventUID_,
                            (strPassWD_.empty()?0:1), (nEventUID_ > 0?1:0) ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK >> nCouponNID_ >> nItemNID_ );
    }
    m_kODBC.EndFetch();
 
    if( nOK == 0 )
    {
        SET_ERROR( NET_OK );
        return true;
    }
 
    switch( nOK ) 
    {
    case -1: SET_ERROR( ERR_GAME_COUPON_05 ); break; // 쿠폰이 존재하지 않음
    case -2: SET_ERROR( ERR_GAME_COUPON_06 ); break; // 쿠폰 체크 조건이 다름(패스워드)
    case -3: SET_ERROR( ERR_GAME_COUPON_07 ); break; // 쿠폰 체크 조건이 다름(이벤트)
    case -4: SET_ERROR( ERR_GAME_COUPON_03 ); break; // 쿠폰이 이미 사용되었음
    case -5: SET_ERROR( ERR_GAME_COUPON_08 ); break; // 패스워드가 틀림
    case -6: SET_ERROR( ERR_GAME_COUPON_09 ); break; // 쿠폰과 이벤트 정보가 연결되어 있지 않음
    case -7: SET_ERROR( ERR_GAME_COUPON_10 ); break; // 쿠폰과 아이템 정보가 연결되어 있지 않음
    }
    START_LOG( cwarn, L"쿠폰 확인 실패.. Err : " << nOK << L", Last Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
    return false;
}

void KGSDBThread::THGetGameCouponItem( IN const int nItemNID_, OUT std::vector<KDropItemInfo>& vecCouponInfo_ )
{
    //{ call dbo.GCGA_game_coupon_select_itemlist ( @1 ) }
    //@1 ; 아이템NID int
    //
    //n return ( @1, @2 )
    //@1 ; ItemID int
    //@2 ; Factor int

    vecCouponInfo_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.GCGA_game_coupon_select_itemlist ( %d ) }", nItemNID_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KDropItemInfo kData;

        VERIFY_QUERY( m_kODBC >> kData.m_ItemID
            >> kData.m_nDuration
            >> kData.m_nPeriod );

        vecCouponInfo_.push_back( kData );
    }

    START_LOG( clog, L"게임쿠폰아이템 목록 얻어오기 완료. Size:" << vecCouponInfo_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

bool KGSDBThread::UpdateGameCoupon( IN const DWORD dwUID_, IN const int nCouponNID_ )
{

    //{ call dbo.GCGA_game_coupon_update ( @1, @2 ) }
    //@1 ; 유저NID   int
    //@2 ; 쿠폰NID   int
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-1       ; 유저정보가 존재하지 않음
    //-2       ; 아이템 지급이 가능한 상태가 아님
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    SET_ERROR( ERR_UNKNOWN );

    DBJIF2( m_kODBC.Query( L"{ call dbo.GCGA_game_coupon_update ( %d, %d ) }",
        dwUID_, nCouponNID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    if( nOK == 0 )
    {
        SET_ERROR( NET_OK );
        return true;
    }

    switch( nOK )
    {
    case -1: SET_ERROR( ERR_GAME_COUPON_00 ); break; // 유저정보가 존재하지 않음
    case -2: SET_ERROR( ERR_GAME_COUPON_06 ); break; // 아이템 지급이 가능한 상태가 아님
    default:
        SET_ERROR( ERR_DB_COMMON_00 ); // 트랜잭션 오류

    }
    START_LOG( cerr, L"사용한 게임쿠폰정보 갱신 DB실패.. OK : " << nOK << L" Query : " << m_kODBC.m_strLastQuery ) << END_LOG;
    return false;
}

bool KGSDBThread::UIDToNick( IN const DWORD dwUID_, OUT std::wstring& strNick_ )
{
    //{ call dbo.UNGA_user_nickname_select_loginuid ( @1 ) }
    //@1 ; @iLoginUID_ [int],
    //
    //1 return ( @1 )
    //@1 ; Nickname nvarchar(24)
    //데이터 없음 ; 해당 유저의 닉네임이 존재하지 않음
    strNick_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.UNGA_user_nickname_select_loginuid ( %d ) }",
        dwUID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> strNick_ );
        m_kODBC.EndFetch();
    }

    if( strNick_.empty() )
    {
        START_LOG( clog, L"UID to Nick Name Failed. ( " << dwUID_ << L" )" ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"UID to Nick Name-Success( " << dwUID_ << L" ) : " << strNick_ ) << END_LOG;

    return true;
}

// 09.04.09. kkurrung. 친구 추천(5th)
bool KGSDBThread::GetMyRecomInfo( IN const DWORD dwUID_, OUT KRecommenderInfo& kMyRecomInfo_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFEV_friend_friend_select_B_20110920 ( %d, %d ) }"

    { call dbo.FFEV_friend_friend_select_B_20110920 ( @1, @2 ) }
    @1 ; @iLoginUIDB_ [int], ; 신규/휴면 유저
    @2 ; @iVersion_   [tinyint] ; 버전

    1 return ( @1, @2, @3 )
    @1 ; LoginUIDA int ; 기존 유저
    @2 ; RegDateD smalldatetime
    @3 ; Status tinyint
    1 ; 아직 추천인 없음
    2 ; 추천 하지 않음
    3 ; 추천 승인 대기중
    4 ; 추천 거절됨
    5 ; 추천 승인됨
    6 ; 첫출석 완료

    데이터 없음 ; 추천 대상이라면 Status 1과 동일하게 취급
    */
    kMyRecomInfo_.Clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.FFEV_friend_friend_select_B_20110920 ( %d, %d ) }", dwUID_, nVersion_ ) );

    if( m_kODBC.BeginFetch() )
    {
        int nUID = -1;
        VERIFY_QUERY( m_kODBC >> nUID >> kMyRecomInfo_.m_tmBeginDate >> kMyRecomInfo_.m_nState );
        m_kODBC.EndFetch();
        if( nUID < 0 )
            kMyRecomInfo_.m_dwLoginUID = 0;
        else
            kMyRecomInfo_.m_dwLoginUID = (DWORD)nUID;

        CTime tmEnd(CTime::GetCurrentTime());
        if( kMyRecomInfo_.m_nState >= KRecomHelper::RBS_ACCEPT_RECOM )
        {
            tmEnd = CTime(kMyRecomInfo_.m_tmBeginDate);
            tmEnd += CTimeSpan( SiKRecomHelper()->GetAdjustPeriod(),0,0,0 );
        }
        kMyRecomInfo_.m_tmEndDate = KncUtil::TimeToInt( tmEnd );

    }

    if( kMyRecomInfo_.m_dwLoginUID > 0 )
        UIDToNick( kMyRecomInfo_.m_dwLoginUID, kMyRecomInfo_.m_strNick );

    START_LOG( clog, L"GetMyRecomInfo" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( kMyRecomInfo_.m_dwLoginUID )
        << BUILD_LOG( kMyRecomInfo_.m_nState )
        << BUILD_LOG( kMyRecomInfo_.m_strNick )
        << BUILD_LOG( kMyRecomInfo_.m_tmBeginDate )
        << BUILD_LOG( kMyRecomInfo_.m_tmEndDate ) << END_LOG;

    return true;
}

bool KGSDBThread::SetMyRecomInfo( IN const DWORD dwUID_, OUT const DWORD dwOtherUID_, OUT const int nState_, IN const int nVersion_ )
{
/*
L"{ call dbo.FFEV_friend_friend_merge_B_20110920 ( %d, %d, %d, %d ) }"

{ call dbo.FFEV_friend_friend_merge_B_20110920 ( @1, @2, @3, @4 ) }
@1 ; @iLoginUIDB_ [int], ; 신규/휴면 유저
@2 ; @iLoginUIDA_ [int], ; 기존 유저
@3 ; @iStatus_ [tinyint],
    1 ; 아직 추천인 없음
    2 ; 추천 하지 않음
    3 ; 추천 승인 대기중
    4 ; 추천 거절됨
    5 ; 추천 승인됨
    6 ; 첫출석 완료
@4 ; @iVersion_   [tinyint] ; 버전

1 return ( @1 )
@1 ; OK int
-1       ; 신규/휴면 유저 정보가 없음
-2       ; 기존 유저 정보가 없음
-101이하 ; 트랜잭션 에러
*/

    int nOK = -99;
    int nUID;

    if( dwOtherUID_ == 0 )
        nUID = -1;
    else
        nUID = (int)dwOtherUID_;

    DBJIF2( m_kODBC.Query( L"{ call dbo.FFEV_friend_friend_merge_B_20110920 ( %d, %d, %d, %d ) }",
        dwUID_, nUID, nState_, nVersion_ ) );

    if( m_kODBC.BeginFetch() )
    {
        int nUID = -1;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L" OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

bool KGSDBThread::GetOtherRecomInfo( IN const DWORD dwUID_, OUT std::vector<KRecommenderInfo>& vecRecommender_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFEV_friend_friend_select_AB_20110920 ( %d, %d ) }"

    { call dbo.FFEV_friend_friend_select_AB_20110920 ( @1, @2 ) }
    @1 ; @iLoginUIDA_ [int], ; 기존 유저
    @2 ; @iVersion_   [tinyint] ; 버전

    n return ( @1, @2, @3 )
    @1 ; LoginUIDB int ; 신규/휴면 유저
    @2 ; RegDateD smalldatetime
    @3 ; Status tinyint
    1 ; 추천 승인 대기중
    2 ; 추천 거절됨 - 디스플레이 하지 않음
    3 ; 추천 승인됨
    4 ; 첫출석 완료 - 아이템을 받을 수 있는 상태
    5 ; 아이템을 받았음

    데이터 없음 ; 추천 기록이 없음
    */

    vecRecommender_.clear();
    DBJIF2( m_kODBC.Query( L"{ call dbo.FFEV_friend_friend_select_AB_20110920 ( %d, %d ) }", dwUID_, nVersion_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KRecommenderInfo kData;
        VERIFY_QUERY( m_kODBC >> kData.m_dwLoginUID >> kData.m_tmBeginDate >> kData.m_nState );

        // 거절된 목록은 가져 오지도 말자.
        if( kData.m_nState == KRecomHelper::RABS_DENY ) continue;

        CTime tmEnd(CTime::GetCurrentTime());
        if( kData.m_nState >= KRecomHelper::RABS_ACCEPT )
        { 
            tmEnd = CTime(kData.m_tmBeginDate);
            tmEnd += CTimeSpan( SiKRecomHelper()->GetAdjustPeriod(),0,0,0 );
        }

         kData.m_tmEndDate = KncUtil::TimeToInt( tmEnd );

        //연계 기간이 지나도 보너스 처리 때문에 가지고 있어야 한다.
        vecRecommender_.push_back( kData );
    }

    std::vector<KRecommenderInfo>::iterator vit;
    for( vit = vecRecommender_.begin() ; vit != vecRecommender_.end() ; ++vit )
        UIDToNick( vit->m_dwLoginUID, vit->m_strNick );

    return true;
}

bool KGSDBThread::SetOtherRecomInfo( IN const DWORD dwRecommendeeUID_, IN const DWORD dwRecommenderUID_, IN const int nState_, IN const int nVersion_ )
{

    /*
    L"{ call dbo.FFEV_friend_friend_merge_AB_20110920 ( %d, %d, %d, %d ) }"

    { call dbo.FFEV_friend_friend_merge_AB_20110920 ( @1, @2, @3, @4 ) }
    @1 ; @iLoginUIDA_ [int], ; 기존 유저
    @2 ; @iLoginUIDB_ [int], ; 신규/휴면 유저
    @3 ; @iStatus_ [tinyint],
        1 ; 추천 승인 대기중
        2 ; 추천 거절됨 - 디스플레이 하지 않음
        3 ; 추천 승인됨
        4 ; 첫출석 완료 - 아이템을 받을 수 있는 상태
        5 ; 아이템을 받았음
    @4 ; @iVersion_   [tinyint] ; 버전

    1 return ( @1 )
    @1 ; OK int
    -1       ; 기존 유저 정보가 없음
    -2       ; 신규/휴면 유저 정보가 없음
    -101이하 ; 트랜잭션 에러
    */


    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.FFEV_friend_friend_merge_AB_20110920 ( %d, %d, %d, %d ) }",
        dwRecommendeeUID_, dwRecommenderUID_, nState_, nVersion_ ) );

    if( m_kODBC.BeginFetch() )
    {
        int nUID = -1;
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L" OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

bool KGSDBThread::GetRecomBonusInfo( IN const DWORD dwUID_, OUT int& nState_, IN const int nVersion_ )
{
/*
L"{ call dbo.FFEV_friend_friend_select_A_20110920 ( %d, %d ) }"

{ call dbo.FFEV_friend_friend_select_A_20110920 ( @1, @2 ) }
@1 ; @iLoginUIDA_ [int], ; 기존 유저
@2 ; @iVersion_   [tinyint] ; 버전

1 return ( @1 )
@1 ; Status tinyint ; 3, 6, 9, 12 체크이기 때문에 서버에서 알아서 결정

데이터 없음 ; 3, 6, 9, 12 체크 아이템을 받은 기록이 없음
*/
    nState_ = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.FFEV_friend_friend_select_A_20110920 ( %d, %d ) }", dwUID_, nVersion_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nState_ );
        m_kODBC.EndFetch();
    }

    return true;
}

bool KGSDBThread::SetRecomBonusInfo( IN const DWORD dwUID_, IN const int nState_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFEV_friend_friend_merge_A_20110920 ( %d, %d, %d ) }"

    { call dbo.FFEV_friend_friend_merge_A_20110920 ( @1, @2, @3 ) }
    @1 ; @iLoginUIDA_ [int] ; 기존 유저
    @2 ; @iStatus_    [tinyint] ; 3, 6, 9, 12 체크이기 때문에 서버에서 알아서 결정
    @3 ; @iVersion_   [tinyint] ; 버전

    1 return ( @1 )
    @1 ; OK int
    -1       ; 기존 유저 정보가 없음
    -101이하 ; 트랜잭션 에러
    */

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.FFEV_friend_friend_merge_A_20110920 ( %d, %d, %d ) }", dwUID_, nState_, nVersion_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, m_kODBC.m_strLastQuery ) << END_LOG;

    return (nOK == 0);
}

void KGSDBThread::InitialiseRecomInfo( IN const DWORD dwUID_, IN const int nSessionInfo_, IN const time_t tmFirstLogin_,
                                       IN const time_t tmLastLogin_, OUT KRecommenderInfo& kMyRecomInfo_ )
{
    kMyRecomInfo_.Clear();
    // 최대 이벤트 기간인가?
    if( !SiKRecomHelper()->IsLimitEventTerm() ) return;
    // 이벤트 기간이고. 신규 혹은 복귀 유저 인가?
    if( SiKRecomHelper()->IsEventTerm() &&
        ( nSessionInfo_ == KUser::SI_NEW_ACCOUNT || SiKRecomHelper()->IsReturnUser( tmLastLogin_) ) )
    {
        SetMyRecomInfo( dwUID_, 0, KRecomHelper::RBS_NONE, SiKRecomHelper()->GetVersion() );
    }
    GetMyRecomInfo( dwUID_, kMyRecomInfo_, SiKRecomHelper()->GetVersion() );
}

bool KGSDBThread::GetServerSort( IN KODBC& kODBC_, IN const std::wstring& strIP_, IN const int nPort_ )
{
    KLocker lock( kODBC_.GetCS() );
/*
    _DBJIF( kODBC_.Query( L"{ call dbo.sp_executesql "
                          L"( N'select ServerPart from dbo.ConnectStatusDB with(nolock) where ServerIP = @1 and ServerPort = @2', "
                          L"N'@1 nvarchar(15), @2 int', N'%s', %d ) }",
                          strIP_.c_str(), nPort_ ), return false, (&kODBC_) );
*/
/*
    서버 소트 정보 가져오기 (adhoc -> stored procedure)
        L"{ call dbo. SIGAServerConnectInfo_select ( N''%s'', %d ) }"

    { call dbo. SIGAServerConnectInfo_select ( @1, @2 ) }
        @1 ; @strServerIP_ varchar ( 25 ),
        @2 ; @iServerPort_ int    

        1 return ( @1 )
        @1 ; ServerPart int
*/
    DBJIF2( kODBC_.Query( L"{ call dbo.SIGAServerConnectInfo_select ( N'%s', %d ) }",
        strIP_.c_str(), nPort_ ) );

    if( kODBC_.BeginFetch() ) {
        int nSort;
        VERIFY_QUERY( kODBC_ >> nSort );
        SiKTimeEventManager()->SetServerSort( nSort );
    }
    kODBC_.EndFetch();

    return true;
}

bool KGSDBThread::CheckSkillInfo( IN const char cCharType_, const char cPromotion_, const int nSkillID_, const std::map<std::pair<char,char>, KSPInfo>& mapTrainedSkill_ )
{
    std::map<std::pair<char,char>, KSPInfo>::const_iterator cmitPromotion;
    std::vector<int>::const_iterator cvitPromotion;
    std::map<std::pair<char,char>, KSPInfo>::const_iterator cmitTotal;
    std::vector<int>::const_iterator cvitTotal;
    std::pair<char,char> prPromotion = std::make_pair( cCharType_, cPromotion_ ); // 캐릭터 전직.
    std::pair<char,char> prTotal = std::make_pair( cCharType_, -1 ); // 공용 정보.

    // Key[CharType,Job]
    cmitPromotion = mapTrainedSkill_.find( prPromotion );
    cmitTotal = mapTrainedSkill_.find( prTotal );
    if( cmitPromotion == mapTrainedSkill_.end() && cmitTotal == mapTrainedSkill_.end() ) {
        START_LOG( cwarn, L"배운 스킬중에 해당하는 Key를 찾을 수 없음. CharType:" << (int)cCharType_ << L",Promotion:" << (int)cPromotion_ << L",SkillID:" << nSkillID_ ) << END_LOG;
        return false;
    }

    cvitPromotion = std::find( cmitPromotion->second.m_vecSkills.begin(), cmitPromotion->second.m_vecSkills.end(), nSkillID_ );
    cvitTotal = std::find( cmitTotal->second.m_vecSkills.begin(), cmitTotal->second.m_vecSkills.end(), nSkillID_ );
    if( cvitPromotion == cmitPromotion->second.m_vecSkills.end() && cvitTotal == cmitTotal->second.m_vecSkills.end() ) {
        START_LOG( cwarn, L"배운 스킬중에 없는 스킬을 장착하고 있음. CharType:" << (int)cCharType_ << L",Promotion:" << (int)cPromotion_ << L",SkillID:" << nSkillID_ ) << END_LOG;
        return false;
    }

    return true;
}

void KGSDBThread::GetFreeHellModeEvent( IN const sTimeEvent& sTimeEvent_, OUT std::set<int>& setMode_ )
{
    std::map< int, std::set<int> >::const_iterator cmit;

    if( sTimeEvent_.m_nEventID != sTimeEvent::EM_FREE_HELL_MODE &&
        sTimeEvent_.m_nEventID != sTimeEvent::EM_CONTINENT_EVENT ) return;

    for( cmit = sTimeEvent_.m_mapModeList.begin() ; cmit != sTimeEvent_.m_mapModeList.end() ; ++cmit )
    {
        setMode_.insert( cmit->first );
    }

    START_LOG( clog, L"Get Free Hell Mode. size : " << setMode_.size() ) << END_LOG;
}

void KGSDBThread::GetPerfectClearBonusEvent( IN const sTimeEvent& sTimeEvent_, OUT std::set<int>& setMode_ )
{
    std::map< int, std::set<int> >::const_iterator cmit;

    if( sTimeEvent_.m_nEventID != sTimeEvent::EM_PERFECT_DAY ) return;

    for( cmit = sTimeEvent_.m_mapModeList.begin() ; cmit != sTimeEvent_.m_mapModeList.end() ; ++cmit )
    {
        setMode_.insert( cmit->first );
    }

    START_LOG( clog, L"Get Perfect Event. Mode size : " << setMode_.size() ) << END_LOG;
}

//void KGSDBThread::InitialiseRecomInfo( IN const DWORD dwUID_, IN const int nSessionInfo_,
//                                       IN const time_t tmLastLogin_, OUT KRecommenderInfo& kMyRecomInfo_ )
//{
//    kMyRecomInfo_.Clear();
//    // 최대 이벤트 기간인가?
//    if( !SiKRecomHelper()->IsLimitEventTerm() ) return;
//    // 이벤트 기간이고. 신규 혹은 복귀 유저 인가?
//    if( SiKRecomHelper()->IsEventTerm() &&
//        ( nSessionInfo_ == KUser::SI_NEW_ACCOUNT || SiKRecomHelper()->IsReturnUser( tmLastLogin_) ) )
//    {
//        SetMyRecomInfo( dwUID_, 0, KRecomHelper::RBS_NONE );
//    }
//    GetMyRecomInfo( dwUID_, kMyRecomInfo_ );
//}


void KGSDBThread::SelectSanta() // 주기적 호출 ( OUT sSanta& sData )
{
/*
mk8253. 20090325
진행중인 산타 이벤트를 리턴합니다.

{ call dbo.SAEV_santa_select }
인자 없음

n return ( @1, @2, @3, @4 )
@1 : EventUID int
@2 : EventName nvarchar(40)
@3 : StartDate smalldatetime
@3 : EndDate smalldatetime
*/
    std::vector<sSanta> vecSanta;
    std::vector<sSanta>::iterator vit;
    vecSanta.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.SAEV_santa_select }" ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        sSanta sSantaData;
        VERIFY_QUERY( m_kODBC >> sSantaData.m_nEventUID
            >> sSantaData.m_strEventName
            >> sSantaData.m_tmStartDateTime
            >> sSantaData.m_tmEndDateTime );

        vecSanta.push_back( sSantaData );
    }

    for( vit = vecSanta.begin(); vit != vecSanta.end(); ++vit )
    {
        GetSantaDormantDate( vit->m_nEventUID, *vit );
        GetSantaUserType( vit->m_nEventUID, *vit );
        SelectSantaItem( vit->m_nEventUID, vit->m_vecItem );
    }

    SiKSanta()->UpdateSantaList( vecSanta );
}

void KGSDBThread::GetSantaDormantDate( IN int nEventUID_, OUT sSanta& sData )
{
    /*
    mk8253. 20090325
    진행중인 산타 이벤트의 휴면유저 기준일 리턴합니다.

    { call dbo.SAEV_santa_limit_select ( @1 ) }
    @1 : iEventUID int

    1 return ( @1 )
    @1 : LimitDate smalldatetime
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.SAEV_santa_limit_select ( %d )}", nEventUID_ ) );
    if( m_kODBC.BeginFetch() )
    {
        time_t tmDate = time_t(NULL);
        VERIFY_QUERY( m_kODBC >> sData.m_tmDormantDate );
        m_kODBC.EndFetch();
        
        //CTime Date(tmDate);
    }
}

void KGSDBThread::GetSantaUserType( IN int nEventUID_, IN OUT sSanta& sData_ )
{
    /*
    mk8253. 20090325
    진행중인 산타 이벤트의 유저타입을 리턴합니다.

    { call dbo.SAEV_santa_type_select ( @1 ) }
    @1 : iEventUID int

    n return ( @1, @2, @3, @4, @5, @6 )
    @1 : UserType int 1 : 신규, 2 : 휴면, 4 : 활성 ,8 ; 전체
    @2 : CharType int
    @3 : StartLevel int
    @4 : EndLevel int
    @5 : ItemID int
    @6 : Comment nvarchar(50)
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.SAEV_santa_type_select ( %d )}", nEventUID_ ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> sData_.m_nUserType 
            >> sData_.m_nCharType >> sData_.m_nBeginLv >> sData_.m_nEndLv 
            >> sData_.m_nItemID >> sData_.m_strComment );
        m_kODBC.EndFetch();
    }
}

void KGSDBThread::SelectSantaItem( IN int nEventUID_, OUT DROP_ITEMS& vecItem_ )
{
    /*
    mk8253. 20090325
    진행중인산타이벤트의지급아이템을리턴합니다.

    { call dbo.SAEV_santa_item_select ( @1 ) }
    @1 : iEventUID int

    n return ( @1, @2 )
    @1 : ItemID int
    @2 : Count int
    @2 : Period int
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.SAEV_santa_item_select ( %d ) }", nEventUID_ ) );
    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        //sSanta sSantaData;
        KDropItemInfo kData;
        VERIFY_QUERY( m_kODBC >> kData.m_ItemID
            >> kData.m_nDuration
            >> kData.m_nPeriod );
        vecItem_.push_back(kData);
    }
}

int KGSDBThread::CheckSantaRewardRecord( IN DWORD dwUserUID_, IN sSanta& sData_ )
{
    /*
    mk8253. 20090325
    유저가 보상 받은 기록이 있는지 검사

    { call dbo.SAEV_santa_user_select ( @1, @2 ) }
    @1 : iLoginUID int
    @2 : iEventUID int

    1 return ( @1 )
    @1 : OK int

    OK
    -1 : 유저정보가 없음
    -2 : 해당하는 이벤트가 없음
    -3 : 이미 이벤트를 받아간 유저임
    */
    int nOK = -99;

    _DBJIF( m_kODBC.Query( L"{ call dbo.SAEV_santa_user_select ( %d, %d )}", 
        dwUserUID_, sData_.m_nEventUID )
        ,return nOK, (&m_kODBC) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    return nOK;
}

bool KGSDBThread::InsertSantaUser( IN DWORD dwUserUID_, IN sSanta& sData_ )
{
    /*
    mk8253. 20090325
    보상 받은 유저를 입력합니다.

    { call dbo.SAEV_santa_user_insert ( @1, @2, @3, @4, @5, @6 ) }
    @1 : iLoginUID int
    @2 : iEventUID int
    @3 : iUserType int
    @4 : iCharType [int]
    @5 : iStartLevel int
    @6 : iEndLevel int

    1 return ( @1 )
    @1 : OK int

    OK: -101 트랜젝션 에러
    */
    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.SAEV_santa_user_insert ( %d, %d, %d, %d, %d, %d )}", 
        dwUserUID_, sData_.m_nEventUID, sData_.m_nUserType, sData_.m_nCharType, sData_.m_nBeginLv, sData_.m_nEndLv ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    _LOG_SUCCESS( nOK == 0, L"산타 유저 DB기록 하기" )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if( nOK == 0 )
        return true;


    return false;
}

void KGSDBThread::ApplySanta( IN DWORD dwUserUID_, IN const char cCharType_, OUT std::vector<KSantaInfo>& vecGiftList_ )
{
    std::vector<KSantaInfo>::iterator vit;
    for ( vit = vecGiftList_.begin() ; vit != vecGiftList_.end() ; ++vit ) {
        std::vector<KDropItemInfo> vecData;
        GetSantaItemInfo( vit->m_dwEventUID, vit->m_dwUserType, vecData );

        std::vector<KDropItemInfo>::iterator vitItem;
        for( vitItem = vecData.begin(); vitItem != vecData.end(); ++vitItem ) {
            if( vitItem->m_ItemID == 0 ) {
                START_LOG( clog, L"유저의 산타 아이템 가져오기 실패" )
                    << BUILD_LOG( dwUserUID_ )
                    << BUILD_LOG( vit->m_dwEventUID )
                    << BUILD_LOG( vit->m_dwUserType ) << END_LOG;
                continue;
            }

            std::vector<KItem> vecItems;
            m_kHelper.InsertSingleItemJob( dwUserUID_, cCharType_, vitItem->m_ItemID, vitItem->m_nDuration, vitItem->m_nPeriod, KItemManager::EWIT_PACKAGE, vecItems, vitItem->m_cGradeID, -1, vitItem->m_cEnchantLevel );
        }

        int nOK = UpdateSantaInfo( vit->m_dwEventUID, dwUserUID_, cCharType_ );
        if( nOK != 0 ) {
            START_LOG( clog, L"유저의 산타 아이템 지급 갱신 실패." )
                << BUILD_LOG( nOK )
                << BUILD_LOG( dwUserUID_ )
                << BUILD_LOG( vit->m_dwEventUID )
                << BUILD_LOG( vit->m_dwUserType ) << END_LOG;
        }
    }

    START_LOG( clog, L"산타 설정.. UserUID : " << dwUserUID_ << L", size : " << vecGiftList_.size() ) << END_LOG;
}

void KGSDBThread::ApplySantaPost( IN DWORD dwUserUID_, IN const char& cCharType_, OUT std::vector<KSantaInfo>& vecGiftList_ )
{
    USHORT usTitleLength = SiKPostConfig()->GetTitleLength();
    USHORT usMsgLength = SiKPostConfig()->GetMessageLength();

    std::vector<KSantaInfo>::iterator vit;
    for ( vit = vecGiftList_.begin() ; vit != vecGiftList_.end() ; ++vit ) {
        std::vector<KDropItemInfo> vecData;
        GetSantaItemInfo( vit->m_dwEventUID, vit->m_dwUserType, vecData );

        // 우편 메세지 만들기
        KPostItemInfo kInfo;
        kInfo.m_dwToUserUID = dwUserUID_;
        kInfo.m_cScriptType = KPostItemInfo::LT_SYSTEM;
        kInfo.m_nGamePoint = 0;
        kInfo.m_vecItem.clear();
        kInfo.m_strFromNickName = SiKPostConfig()->GetPostString(L"str_00");
        //boost::replace_all( vit->m_wstrComment, L"'", L"''" );
        kInfo.m_strTitle = vit->m_wstrComment.substr( 0, std::min<USHORT>(vit->m_wstrComment.size(), usTitleLength ) );
        //kInfo.m_strMessage = vit->m_wstrComment.substr( 0, std::min<USHORT>(vit->m_wstrComment.size(), usMsgLength ) );
        kInfo.m_strMessage = vit->m_wstrComment;
        kInfo.m_cCharType = cCharType_;
        boost::replace_all( kInfo.m_strTitle, L"'", L"''" );
        boost::replace_all( kInfo.m_strMessage, L"'", L"''" );

        std::vector<KDropItemInfo>::iterator vitItem;
        for( vitItem = vecData.begin(); vitItem != vecData.end(); ++vitItem ) {
            if( vitItem->m_ItemID == 0 ) {
                START_LOG( clog, L"유저의 산타 아이템 가져오기 실패" )
                    << BUILD_LOG( dwUserUID_ )
                    << BUILD_LOG( vit->m_dwEventUID )
                    << BUILD_LOG( vit->m_dwUserType )
                    << END_LOG;
                continue;
            }

            KItem kItem;
            kItem.m_ItemID = vitItem->m_ItemID;
            kItem.m_nCount = vitItem->m_nDuration;
            kItem.m_nPeriod = vitItem->m_nPeriod;
            kItem.m_cEnchantLevel = vitItem->m_cEnchantLevel;
            kItem.m_cGradeID = vitItem->m_cGradeID;

            kInfo.m_vecItem.push_back( kItem );
        }

        // 우편 산타 지급
        LIF( InsertSystemPost( kInfo ) );

        int nOK = UpdateSantaInfo( vit->m_dwEventUID, dwUserUID_, cCharType_ );
        _LOG_CONDITION( nOK == 0, clog, cwarn, L"유저의 산타 아이템 지급 갱신 실패." )
                << BUILD_LOG( nOK )
                << BUILD_LOG( dwUserUID_ )
                << BUILD_LOGc( cCharType_ )
                << BUILD_LOG( vit->m_dwEventUID )
                << BUILD_LOG( vit->m_dwUserType ) << END_LOG;
    }

    START_LOG( clog, L"산타 우편 설정.. UserUID : " << dwUserUID_ << L", size : " << vecGiftList_.size() ) << END_LOG;
    vecGiftList_.clear(); // 시스템 메일로 보냈으니 초기화
}

bool KGSDBThread::CheckJackPotItemGet( IN const DWORD dwUID_, IN const int nRainBowID_, IN const int nEventID_, IN const int nJackPotID_ )
{
    //L"{ call dbo.REEV_rainbow_event_check_user ( %d, %d, %d, %d ) }"

    //{ call dbo.REEV_rainbow_event_check_user ( @1, @2, @3, @4 ) }
    //@1 ; @iLoginUID_  int
    //@2 ; @iEventUID_  int
    //@3 ; @iEventID_   tinyint
    //@4 ; @iJackpotID_ tinyint

    //1 return ( @1 )
    //@1 ; OK int
    //0  ; 성공
    //-1 ; 유저 정보가 없음
    //-2 ; 등록된 이벤트가 없음
    //-3 ; 등록된 이벤트가 없음
    //-4 ; 등록된 이벤트가 없음
    //-5 ; 이미 아이템을 받아갔음

    SET_ERROR( ERR_RAIN_BOW_04 );
    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.REEV_rainbow_event_check_user ( %d, %d, %d, %d ) }",
        dwUID_, nRainBowID_, nEventID_, nJackPotID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, m_kODBC.m_strLastQuery << L", OK : " << nOK ) << END_LOG;

    if( nOK != 0 )
    {
        switch ( nOK )
        {
        case -1 : SET_ERROR( ERR_RAIN_BOW_01 ); break;
        case -2 : SET_ERROR( ERR_RAIN_BOW_03 ); break;
        case -3 : SET_ERROR( ERR_RAIN_BOW_03 ); break;
        case -4 : SET_ERROR( ERR_RAIN_BOW_03 ); break;
        case -5 : SET_ERROR( ERR_RAIN_BOW_02 ); break;
        }
        START_LOG( cwarn, m_kODBC.m_strLastQuery )
            << BUILD_LOG( nOK ) << END_LOG;
        return false;
    }

    SET_ERROR( NET_OK );
    return true;
}

bool KGSDBThread::SetJackPotItemGet( IN const DWORD dwUID_, IN const int nRainBowID_, IN const int nEventID_, IN const int nJackPotID_ )
{
    //{ call dbo.REEV_rainbow_event_insert_user ( @1, @2, @3, @4 ) }
    //@1 ; @iLoginUID_  int
    //@2 ; @iEventUID_  int
    //@3 ; @iEventID_   tinyint
    //@4 ; @iJackpotID_ tinyint
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-1       ; 이미 아이템을 받아갔음
    //-101이하 ; 트랜잭션 에러

    SET_ERROR( ERR_RAIN_BOW_04 );
    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.REEV_rainbow_event_insert_user ( %d, %d, %d, %d ) }",
        dwUID_, nRainBowID_, nEventID_, nJackPotID_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, m_kODBC.m_strLastQuery << L", OK : " << nOK ) << END_LOG;

    if( nOK != 0 )
    {
        switch ( nOK )
        {
        case -1 : SET_ERROR( ERR_RAIN_BOW_02 ); break;
        }
        START_LOG( cwarn, m_kODBC.m_strLastQuery )
            << BUILD_LOG( nOK ) << END_LOG;
        return false;
    }

    SET_ERROR( NET_OK );
    return true;
}

bool KGSDBThread::InsertEffectTexture( int nType_, int nIndex_, std::wstring& strFile_ )
{
    //{ call dbo.CECL_client_effect_insert ( @1, @2, @3 ) }
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-1       ; 이미 데이터가 존재함
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.CECL_client_effect_insert ( %d, %d, N'%s' ) }",
        nType_, nIndex_, strFile_.c_str() ) );
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    return (nOK == 0);
}

int KGSDBThread::GetUserHackingCount( IN const DWORD dwUID_ )
{
    /*
    20090723. microcat.

    { call dbo.UCGA_user_cracking_select_dungeon ( @1 ) }
    @1 ; 유저UID int

    1 return ( @1 )
    @1 ; 해킹누적카운트 int
    */
    int nRet = 0;
    _DBJIF( m_kODBC.Query( L"{ call dbo.UCGA_user_cracking_select_dungeon ( %d ) }",
        dwUID_ ),
        return nRet, (&m_kODBC));

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nRet );
    }
    m_kODBC.EndFetch();

    return nRet;
}

void KGSDBThread::CapsuleSystemInsertLog( IN const DWORD dwUID_, IN const char cCharType_, IN const int nItemKind_, IN const int nLevel_, IN const GCITEMID iteID_ )
{
    /*
    20090224. microcat.
    L"{ call dbo.CSGA_capsule_system_insert ( %d, %d, %d, %d, %d ) }"

    { call dbo.CSGA_capsule_system_insert ( @1, @2, @3, @4, @5 ) }
    @1 ; @iLoginUID_ [int],
    @2 ; @iCharType_ [tinyint],
    @3 ; @iKind_ [tinyint],
    @4 ; @iLevel_ [tinyint],
    @4 ; @iItemID_ [int],

    1 return ( @1 )
    @1 ; OK int
    0         ; 성공
    -101 이하 ; 트랜잭션 에러
    */

    // DB에 기록 남기기.
    DBJIF( m_kODBC.Query( L"{ call dbo.CSGA_capsule_system_insert ( %d, %d, %d, %d, %d ) }",
        dwUID_, (int)cCharType_, nItemKind_, nLevel_, iteID_ ) );

    int nOK = -99;

    if( m_kODBC.BeginFetch() ) 
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    if( nOK != 0 )
    {
        START_LOG( cerr, L"GP캡슐 아이템 DB기록 작업 실패. UserUID : " << dwUID_ )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
}

// 유저의 보드게임 정보 가져오기.
int KGSDBThread::GetUserBoardInfo( IN const DWORD dwUID_, IN const DWORD dwInitHour_, IN const DWORD dwTotalMapCount_, OUT KUserBoardInfo& kUserBoard_, OUT time_t& tDiceLastTime_, IN DWORD dwVersion_ )
{
    // DB에서 유저의 보드게임 정보 가져오기 작업.
    /*
    mk8253. 20100116.
    유저주사위정보

    L"{ call dbo.DPGADicePlay_select ( %d, %d, %d, %d ) }"

    { call dbo.DPGADicePlay_select ( @1, @2. @3, @4 ) }
    @1 ; @iType_          [tinyint](  여러 Type이 있을수 있으니깐.., 할로윈1, 대만이번이벤트2 .., )
    @2 ; @iLoginUID_      [int]
    @4 ; @iTotalPosition_ [tinyint]( 말칸이 몇칸인지 계산.., )

    1 return ( @1, @2, @3, @4, @5, @6 )
    @1 ; TurnCount; 전체바퀴수
    @2 ; Position ; 현재위치
    @3 ; ReceiveCount ; 오늘주사위받은수
    @4 ; GetCount ; 주사위 굴릴수 있는 횟수
    @5 ; RestSec; 남은시간초
    @6 ; LastGetDate; 마지막으로주사위받은시간
    */

    DBJIF2( m_kODBC.Query( L"{ call dbo.DPGADicePlay_select ( %d, %d, %d ) }",
        dwVersion_ ,dwUID_, dwTotalMapCount_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> kUserBoard_.m_dwTotalCount
            >> kUserBoard_.m_dwCurrentPosition
            >> kUserBoard_.m_dwGiveCount
            >> kUserBoard_.m_dwDiceCount
            >> kUserBoard_.m_dwRemainTime
            >> tDiceLastTime_ );
        m_kODBC.EndFetch();
    }

    // 주사위 무료 지급 횟수에 대한 보정처리.
    CheckFreeReceiveCount( kUserBoard_.m_dwGiveCount );

    // 초기화 여부도 함께 체크.
    /*
    mk8253. 20100116.
    오늘초기화기록이있는지체크

    L"{ call dbo.DPGADicePlayInit_check( %d, %d ) }"

    { call dbo.DPGADicePlayInit_check ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iSdtHour_  int

    1 return ( @1 )
    @1 ; Data
    */
    int nData = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.DPGADicePlayInit_check ( %d, %d ) }",
        dwUID_, dwInitHour_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nData );
        m_kODBC.EndFetch();
    }

    // 주사위 초기화를 진행하기 전에 해당 사항에 대해서 넣어주자.
    if( 0 == nData ) { // 0 : 초기화 성공 ( 날짜가 바뀜)
//        SetInitDicePlayLog( dwUID_, dwInitHour_ );
        nData = 2;
    }
    return nData;
}

void KGSDBThread::HalloweenItemInsertLog( IN DWORD dwUID_, IN GCITEMID dwItemID_, IN int nDuration_, IN int nPeriod_, IN DWORD dwVersion_ )
{
    /*
    mk8253. 20091124
    아이템 획득 로그

    L"{ call dbo.DPGADicePlay_item_insert ( %d, %d, %d, %d, %d ) }"

    { call dbo.DPGADicePlay_item_insert ( @1, @2, @3, @4, @5 ) }
    @1 ; @iType_     tinyint
    @2 ; @iLoginUID_ int  
    @3 ; @iItemID_   int  
    @4 ; @iDuration_ int  
    @5 ; @iPeriod_   int  

    1 return ( @1 )  
    @1 ; OK
    0        ; 성공
    -1       ; 유저 정보가 없음
    -101이하 ; 트랜젝션 에러
    */
    
    DBJIF( m_kODBC.Query( L"{ call dbo.DPGADicePlay_item_insert ( %d, %d, %d, %d, %d ) }",
        dwVersion_, dwUID_, dwItemID_, nDuration_, nPeriod_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() ) 
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

}

bool KGSDBThread::CheckBuyLimitItem( IN const DWORD dwUID_, IN const GCITEMID dwItemID_ )
{
    //{ call dbo.ILGAItemLimitUser_check ( @1, @2 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iItemID_   int
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0 ; 구매 승인(해당 아이템은 최대 구매횟수 제한 있음)
    //-1 ; 구매 승인(해당 아이템은 최대 구매횟수 제한 없음)
    //-2 ; 구매 불가(제한 횟수를 초과하였음)

    DBJIF2( m_kODBC.Query( L"{ call dbo.ILGAItemLimitUser_check ( %d, %d )  }", dwUID_, dwItemID_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch()  )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0 || nOK == -1 || nOK == -2, L"구매 횟수 제한 체크. UID :" << dwUID_ << L", Item ID : " << dwItemID_ <<L", OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return ( nOK != -2 );
}

void KGSDBThread::CommitBuyLimitItem( IN const DWORD dwUID_, IN const GCITEMID dwItemID_ )
{
    //{ call dbo.ILGAItemLimitUser_merge ( @1, @2 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iItemID_   int
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0       ; 성공(해당 아이템에 대한 구매횟수 카운팅)
    //-1       ; 성공(해당 아이템은 최대 구매횟수 제한 없음)
    //-101이하 ; 트랜잭션 에러

    DBJIF( m_kODBC.Query( L"{ call dbo.ILGAItemLimitUser_merge ( %d, %d )  }", dwUID_, dwItemID_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch()  )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    _LOG_SUCCESS( nOK == 0 || nOK == -1, L"구매 횟수 제한 적용. UID :" << dwUID_ << L", Item ID : " << dwItemID_ <<L", OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
}

void KGSDBThread::UpdateUserBoardInfo( IN DWORD dwUID_, IN DWORD dwTotalMapCount_, IN KUserBoardInfo kUserBoardInfo_, IN std::wstring strDiceLastTime_, IN DWORD dwVersion_ )
{
    // 유저의 마지막 보드게임 정보 저장.

    /*
    mk8253. 20091008
    주사위정보변경

    L"{ call dbo.DPGADicePlay_update ( %d, %d, %d, %d, %d, %d, %d, N'%s', %d ) }"

    { call dbo.DPGADicePlay_update ( @1, @2, @3, @4, @5, @6, @7, @8, @9 ) }
    @1 ; @iType_          [tinyint]
    @2 ; @iLoginUID_      [int]
    @3 ; @iTurnCount_     [tinyint]
    @4 ; @iPosition_      [tinyint]
    @5 ; @iReceiveCount_  [smallint]
    @6 ; @iGetCount_      [smallint]
    @7 ; @iRestSec_       [smallint]
    @8 ; @sdtLastGetDate_ [smalldatetime]
    @9 ; @iTotalPosition_ [tinyint]

    1 return ( @1 )  
    @1 ; OK
    0        ; 성공
    -1       ; 유저정보가없음
    -101이하; 트랜젝션에러
    */
    DBJIF( m_kODBC.Query( L"{ call dbo.DPGADicePlay_update ( %d, %d, %d, %d, %d, %d, %d, N'%s', %d ) }",
        dwVersion_,
        dwUID_,
        kUserBoardInfo_.m_dwTotalCount,
        kUserBoardInfo_.m_dwCurrentPosition,
        kUserBoardInfo_.m_dwGiveCount,
        kUserBoardInfo_.m_dwDiceCount,
        kUserBoardInfo_.m_dwRemainTime,
        strDiceLastTime_.c_str(),
        dwTotalMapCount_
        ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    // 어차피 로그성 데이터이므로 에러 처리를 진행하진 않고
    // 실패한 부분에 대해서 로그는 남기자.
    if( nOK != 0 )
    {
        START_LOG( cerr, L"할로윈 보드게임 주사위 굴린 로그 남기기 실패. " )
            << BUILD_LOG( nOK )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( kUserBoardInfo_.m_dwTotalCount )
            << BUILD_LOG( kUserBoardInfo_.m_dwCurrentPosition )
            << BUILD_LOG( kUserBoardInfo_.m_dwGiveCount )
            << BUILD_LOG( kUserBoardInfo_.m_dwDiceCount )
            << BUILD_LOG( strDiceLastTime_.c_str() )
            << BUILD_LOG( dwTotalMapCount_ )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
}

void KGSDBThread::InsertIPAdvantageDailyItem( IN const DWORD dwUID_, IN const char cCharType_, IN OUT std::vector<KItem>& vecItem_ )
{
    if( GetTodayCharEventCount( dwUID_, cCharType_, UET_IP_ADVANTAGE ) > 0 ) {
        return;
    }
    LIF( SetTodayCharEventDoneDiff( dwUID_, cCharType_, UET_IP_ADVANTAGE ) );

    std::vector<KDropItemInfo> vecItems;
    SiKIPAdvantage()->GetDailyConnectionItem( vecItems );

    if( vecItems.empty() ) return;

    vecItem_.reserve( vecItems.size() );

    std::vector<KDropItemInfo>::iterator vit;
    for( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit ) {
        m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_IP_REWARD, vecItem_ );
    }
}

void KGSDBThread::InsertDailyMissionLog( IN const DWORD dwUID_, IN const int nEventID_, IN const int nOrderID_, IN const GCITEMID nItemID_ )
{

    // 아이템을 넣어준 해당 로그를 기록하자~!!
    /*
    L"{ call dbo.DQGADailyQuest_insert_log ( %d, %d, %d, %d ) }"

    { call dbo.DQGADailyQuest_insert_log ( @1, @2, @3, @4 ) }
    @1 ; LoginUID int
    @1 ; EventNID int
    @1 ; OrderID  tinyint
    @1 ; ItemID   int

    1 return ( @1 )
    @1 ; OK int
    0       ; 성공
    -1       ; 유저 정보가 존재하지 않음
    -2       ; 등록된 이벤트가 아님
    -3       ; 등록된 세부이벤트가 아님
    -101이하 ; 트랜잭션 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.DQGADailyQuest_insert_log ( %d, %d, %d, %d ) }",
        dwUID_, nEventID_, nOrderID_, nItemID_ ) );

    if( m_kODBC.BeginFetch() ) 
    {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );

        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();
}

void KGSDBThread::GetBuyWebAndGiftItems( IN const DWORD dwUID_, OUT std::vector<KGiftInfo>& vecGiftInfo_ )
{
    //{ call dbo.up_query_cash_item ( @1, @2 ) }
    //@1 ; LoginUID int
    //@2 ; Gift     bit
    //0 ; 자가구매분만 리턴
    //1 ; 전체 리턴

    //n return ( @1, @2, @3, @4, @5, @6, @7, @8, @9, @10 )
    //@1  ; ItemType  int
    //@2  ; ItemUID   int
    //@3  ; ItemID    int
    //@4  ; RegDate   smalldatetime
    //@5  ; StartDate smalldatetime
    //@6  ; EndDate   smalldatetime
    //@7  ; Factor    int
    //@8  ; GiftItem  bit
    //@9  ; BuyerNick nvarchar( 24 )
    //@10 ; Comment   nvarchar( 80 )

    vecGiftInfo_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.up_query_cash_item ( %d, 1 ) }", dwUID_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KItemUnit kItemUnit;
        KGiftInfo kInfo;
        bool bGift = false;

        VERIFY_QUERY( m_kODBC 
            >> kItemUnit.m_iItemType
            >> kItemUnit.m_iUID
            >> kInfo.m_dwItemID
            >> kItemUnit.m_tRegDate
            >> kItemUnit.m_tStartDate
            >> kItemUnit.m_tEndDate
            >> kItemUnit.m_iDuration
            >> bGift
            >> kInfo.m_strBuyerNick
            >> kInfo.m_strMsg );

        if( bGift )
            vecGiftInfo_.push_back( kInfo );
    }
    m_kODBC.EndFetch();
}

void KGSDBThread::AddInvenExternalItems( DWORD dwUID_, IN const char cCharType_ )
{

    //{ call dbo.IEGA_item_external_select ( @1 ) }
    //@1 ; 유저UID   int

    //n return ( @1, @2, @3 )
    //@1 ; UID    int
    //개별 아이템을 식별하기 위한 UID
    //@2 ; ItemID int
    //@3 ; Factor int

    std::vector< boost::tuple<int,GCITEMID,int,int> > vecItems;

    DBJIF( m_kODBC.Query( L"{ call dbo.IEGA_item_external_select ( %d ) }", dwUID_ ) );
    while( m_kODBC.Fetch() !=  SQL_NO_DATA )
    {
        boost::tuple<int,GCITEMID,int,int> tpItem;
        VERIFY_QUERY( m_kODBC >> tpItem.get<0>()
            >> tpItem.get<1>()
            >> tpItem.get<2>()
            >> tpItem.get<3>()
            );
        vecItems.push_back( tpItem );
    }
    m_kODBC.EndFetch();

    if( vecItems.empty() ) return;

    //{ call dbo.o ( @1, @2 ) }
    //@1 ; 유저UID int
    //@2 ; UID     int
    //개별 아이템을 식별하기 위한 UID
    //
    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-1       ; 유저정보가 없음
    //-2       ; 해당 아이템 정보가 없거나 이미 받아갔음
    //-101이하 ; 트랜잭션 에러
    int nCount = 0;

    std::vector< boost::tuple<int,GCITEMID,int,int> >::iterator vit;
    for( vit = vecItems.begin() ; vit != vecItems.end() ; ++vit )
    {
        std::vector<KItem> vecItems;
        if( m_kHelper.InsertSingleItemJob( dwUID_, cCharType_, vit->get<1>(), vit->get<2>(), vit->get<3>(), KItemManager::EWIT_EXTERNAL, vecItems ) )
        {
            int nOK = -99;
            DBJIF( m_kODBC.Query( L"{ call dbo.IEGA_item_external_update ( %d, %d ) }", dwUID_, vit->get<0>() ) );
            if( m_kODBC.BeginFetch() )
            {
                VERIFY_QUERY( m_kODBC >> nOK );
            }
            m_kODBC.EndFetch();
            _LOG_SUCCESS( nOK == 0 , L"UID : " << dwUID_
                << L"OK : " << nOK
                << L" [" << vit->get<0>()
                << L"," << vit->get<1>()
                << L"," << vit->get<2>()
                << L"," << vit->get<3>()
                << L"]" ) << END_LOG;
            ++nCount;
        }
    }

    START_LOG( clog, L"External Item Insert .. UID : " << dwUID_ << L", Count : " << vecItems.size() << L", Success : " << nCount ) << END_LOG;
}

bool KGSDBThread::SetBingoCorrectAnswer( IN const DWORD dwUID_, IN const int nEventID_, IN const int nQuestionID_, IN const int nXPos_, IN const int nYPos_ )
{


    //{ call dbo.QBGAQuizBingoUser_insert ( @1, @2, @3, @4, @5 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iNID_      int
    //@3 ; @iNID1_     tinyint
    //@4 ; @iX_        tinyint
    //@5 ; @iY_        tinyint
    //
    //1 select ( @1 )
    //@1  ; OK int
    //0        ; 성공
    //-1       ; 등록된 퀴즈빙고가 아님
    //-2       ; 등록된 퀴즈내용이 아님
    //-3       ; 이미 빙고를 찍었음
    //-4       ; 이미 푼 퀴즈임
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoUser_insert ( %d, %d, %d, %d, %d ) }",
        dwUID_, nEventID_, nQuestionID_, nXPos_, nYPos_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }
    _LOG_SUCCESS( nOK == 0, L" OK : " << nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    return (nOK == 0);
}

void KGSDBThread::GetBingoPosition( IN const DWORD dwUID_, IN const int nEventID_, OUT std::vector<std::pair<int,int> >& vecBingoPos_ )
{

    //{ call dbo.QBGAQuizBingoUser1_select ( @1, @2 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iNID_      int
    //
    //n select ( @1, @2, @3, @4 )
    //@1  ; LoginUID int
    //@2  ; NID      int
    //@3  ; X        tinyint
    //@4  ; Y        tinyint

    vecBingoPos_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoUser1_select ( %d, %d ) }",
        dwUID_, nEventID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        DWORD dwUID;
        int nEventID;
        std::pair<int,int> prPos(-1,-1);
        VERIFY_QUERY( m_kODBC >> dwUID
            >> nEventID
            >> prPos.first
            >> prPos.second );
        if ( dwUID != dwUID_ || nEventID != nEventID_ ) {
            START_LOG( cerr, L"Miss Match Event ID OR UID, : " << dwUID << L", " << dwUID_ << L", " << nEventID << L", " <<  nEventID_ )
                << BUILD_LOG( m_kODBC.m_strLastQuery )
                << BUILD_LOG( m_kODBC.m_strBeforeLastQuery ) << END_LOG;
            continue;
        }

        vecBingoPos_.push_back( prPos );
    }
}

void KGSDBThread::GetBingoCorrectQuestions( IN const DWORD dwUID_, IN const int nEventID_, OUT std::vector<int>& vecCorrectQuestions_ )
{
    //{ call dbo.QBGAQuizBingoUser2_select ( @1, @2 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iNID_      int
    //
    //n select ( @1, @2, @3 )
    //@1  ; LoginUID int
    //@2  ; NID      int
    //@3  ; NID1     tinyint

    vecCorrectQuestions_.clear();
    DBJIF( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoUser2_select ( %d, %d ) }",
        dwUID_, nEventID_ ) );

    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {
        DWORD dwUID;
        int nEventID;
        int nQuestionID;
        VERIFY_QUERY( m_kODBC >> dwUID
            >> nEventID
            >> nQuestionID );
        if ( dwUID != dwUID_ || nEventID != nEventID_ ) {
            START_LOG( cerr, L"Miss Match Event ID OR UID, : " << dwUID << L", " << dwUID_ << L", " << nEventID << L", " <<  nEventID_ )
                << BUILD_LOG( m_kODBC.m_strLastQuery )
                << BUILD_LOG( m_kODBC.m_strBeforeLastQuery ) << END_LOG;
            continue;
        }

        vecCorrectQuestions_.push_back( nQuestionID );
    }
}

int KGSDBThread::GetBingoAccTime( IN const DWORD dwUID_, IN const int nEventID_ )
{

    //{ call dbo.QBGAQuizBingoUser3_select ( @1, @2, @3 ) }
    //@1 ; @iLoginUID_   int
    //@2 ; @iNID_        int
    //@3 ; @sdtRegDateD_ smalldatetime
    //N'20091117' 형태로 전송되어야 함
    //
    //1 select ( @1, @2, @3, @4 )
    //@1  ; LoginUID int
    //@2  ; NID      int
    //@3  ; RegDateD smalldatetime
    //@4  ; PlayTime int
    int nAccTime = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoUser3_select ( %d, %d, N'%s' ) }",
        dwUID_, nEventID_, (LPCTSTR)CTime::GetCurrentTime().Format( _T("%Y%m%d") ) ) );

    if ( m_kODBC.BeginFetch() ) {
        DWORD dwUID;
        int nEventID;
        time_t tmDate;
        VERIFY_QUERY( m_kODBC >> dwUID
            >> nEventID
            >> tmDate
            >> nAccTime );
        m_kODBC.EndFetch();
    }

    return nAccTime;
}

bool KGSDBThread::SetBingoAccTime( IN const DWORD dwUID_, IN const int nEventID_, IN int nAccTime_ )
{
    // 빙고 플레이 타임에 대한 보완 코드.
    // 23시 59분에서 00시 1분 사이에 서버이동이나 로그아웃 하는 유저들의 빙고 플레이 타임을
    // 강제로 0으로 초기화 하는 코드를 넣어주자.
    CTime tmCurrent = CTime::GetCurrentTime();
    if( tmCurrent.GetHour() == 0 && tmCurrent.GetMinute() < 3 ) {
        nAccTime_ = 0;
    }
    return SetBingoAccTime( dwUID_, nEventID_, nAccTime_, tmCurrent );
}

bool KGSDBThread::SetBingoAccTime( IN const DWORD dwUID_, IN const int nEventID_, IN const int nAccTime_, IN const CTime& tmDate_ )
{
    //{ call dbo.QBGAQuizBingoUser3_merge ( @1, @2, @3, @4 ) }
    //@1 ; @iLoginUID_   int
    //@2 ; @iNID_        int
    //@3 ; @sdtRegDateD_ smalldatetime
    //N'20091117' 형태로 전송되어야 함
    //@4 ; @iPlayTime_   int
    //
    //1 select ( @1 )
    //@1  ; OK int
    //0        ; 성공
    //-1       ; 등록된 퀴즈빙고가 아님
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoUser3_merge ( %d, %d, N'%s', %d ) }",
        dwUID_, nEventID_, (LPCTSTR)tmDate_.Format( _T("%Y%m%d") ), nAccTime_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    return (nOK == 0);
}

void KGSDBThread::GetBingoCoinCountInfo( IN const DWORD dwUID_, IN const int nEventID_, OUT int& nTotalCount_, OUT int& nUseCount_ )
{
    //{ call dbo.QBGAQuizBingoUser4_select ( @1, @2 ) }
    //@1 ; @iLoginUID_   int
    //@2 ; @iNID_        int
    //
    //1 select ( @1, @2, @3, @4 )
    //@1  ; LoginUID   int
    //@2  ; NID        int
    //@3  ; BonusCount int
    //@4  ; UseCount   int
    nTotalCount_ = 0;
    nUseCount_ = 0;

    DBJIF( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoUser4_select ( %d, %d ) }",
        dwUID_, nEventID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        DWORD dwUID;
        int nEventID;
        VERIFY_QUERY( m_kODBC >> dwUID
            >> nEventID
            >> nTotalCount_
            >> nUseCount_ );
        m_kODBC.EndFetch();
    }
}

bool KGSDBThread::SetBingoCoinCountInfo( IN const DWORD dwUID_, IN const int nEventID_, IN const int nTotalCount_, IN const int nUseCount_ )
{
    //{ call dbo.QBGAQuizBingoUser4_merge ( @1, @2, @3, @4 ) }
    //@1 ; @iLoginUID_   int
    //@2 ; @iNID_        int
    //@3 ; @iBonusCount_ int
    //@4 ; @iUseCount_   int
    //
    //1 select ( @1 )
    //@1  ; OK int
    //0        ; 성공
    //-1       ; 등록된 퀴즈빙고가 아님
    //-101이하 ; 트랜잭션 에러

    int nOK = -99;
    DBJIF2( m_kODBC.Query( L"{ call dbo.QBGAQuizBingoUser4_merge ( %d, %d, %d, %d ) }",
        dwUID_, nEventID_, nTotalCount_, nUseCount_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }

    return (nOK == 0);
}

void KGSDBThread::CompactionRewards( IN OUT std::vector<KDropItemInfo>& vecRewards_ )
{
    std::vector<KDropItemInfo> vecRewards;

    std::vector<KDropItemInfo>::iterator vit;
    for ( vit = vecRewards_.begin() ; vit != vecRewards_.end() ; ++ vit ) {

        // 영구 아이템
        if ( vit->m_nPeriod < 0 && vit->m_nDuration < 0 ) {
            vecRewards.push_back( *vit );
            continue;
        }

        bool bMerge = false;
        std::vector<KDropItemInfo>::iterator vit1;
        for ( vit1 = vecRewards.begin() ; vit1 != vecRewards.end() ; ++vit1 ) {
            if ( vit1->m_ItemID != vit->m_ItemID ) continue;
            // 수량이 음수 이고, 기간이 양수 이면 합치기
            if ( vit1->m_nDuration < 0 && vit->m_nDuration < 0 &&
                vit1->m_nPeriod > 0 && vit->m_nPeriod > 0 ) {

                vit1->m_nPeriod += vit->m_nPeriod;
                bMerge = true;
                break;
            }

            if ( vit1->m_nPeriod < 0 && vit->m_nPeriod < 0 &&
                vit1->m_nDuration > 0 && vit->m_nDuration > 0 ) {

                vit1->m_nDuration += vit->m_nDuration;
                bMerge = true;
                break;
            }
        }

        if ( !bMerge ) {
            vecRewards.push_back( *vit );
        }
    }
    vecRewards_.swap( vecRewards );
}

bool KGSDBThread::GetUserEquipInfo( IN const DWORD dwUID_, IN const std::vector<KItem>& vecInven_, IN OUT std::map<char,KCharacterInfo>& mapCharacterInfo_, IN std::vector<std::pair<GCITEMUID, GCITEMID>>& vecDesignCoordi_ )
{
    /*  
    L"{ call dbo.CEGACharacterEquipitem_select ( %d ) }"  

    { call dbo.CEGACharacterEquipitem_select ( @01 ) }  
    @01: @01iLoginUID_input int  

    n select ( @01, @02, @03 )  
    @01: CharType tinyint  
    @02: ItemUID  int  
    @03: ItemID   int  
    */  

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEGACharacterEquipitem_select ( %d ) }", dwUID_ ) );
    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {

        // TODO : 아이템 타입 처리.
        char cCharType = -1;
        KEquipItemInfo equipElement;
        VERIFY_QUERY( m_kODBC >> cCharType
            >> equipElement.m_dwUID
            >> equipElement.m_dwID );

        // 장착 아이템이 내가 보유 하고 있는 아이템인지 확인.
        std::vector<KItem>::const_iterator vit;
        vit = std::find_if( vecInven_.begin(), vecInven_.end(),
            boost::bind( &KItem::m_ItemUID, _1 ) == equipElement.m_dwUID );

        // 보유 하고 있지 않으면 장착 하지 않음..
        if ( vit != vecInven_.end() ) {
            equipElement.m_dwID = vit->m_ItemID;
            equipElement.m_vecAttribute = vit->m_vecAttribute;
            equipElement.m_vecSocket = vit->m_vecSocket;
            equipElement.m_cGradeID = vit->m_cGradeID;
            equipElement.m_nEquipLevel = static_cast<int>( vit->m_sEquipLevel );
            equipElement.m_cEnchantLevel = vit->m_cEnchantLevel;
            equipElement.m_cEnchantEquipGradeID = vit->m_cEnchantEquipGradeID;
            equipElement.m_DesignCoordiID = vit->m_DesignCoordiID;
        } else {
            std::vector<std::pair<GCITEMUID, GCITEMID>>::iterator vitDesignCoordi;
            vitDesignCoordi = std::find_if( vecDesignCoordi_.begin(), vecDesignCoordi_.end(),
                boost::bind( &std::pair<GCITEMUID, GCITEMID>::first, _1) == equipElement.m_dwUID );
            if( vitDesignCoordi != vecDesignCoordi_.end() ) {
                equipElement.m_DesignCoordiID = vitDesignCoordi->second;
            }
        }

        std::map<char, KCharacterInfo>::iterator mit;
        mit = mapCharacterInfo_.find(cCharType);
        if( mit == mapCharacterInfo_.end() ) {
            START_LOG( cwarn, L"장착 장비에 맞는 캐릭터 타입 없음, UID : " << dwUID_ )
                << BUILD_LOGc( cCharType )
                << BUILD_LOG( equipElement.m_dwUID )
                << BUILD_LOG( equipElement.m_dwID )
                << BUILD_LOGc( equipElement.m_cGradeID )
                << BUILD_LOGc( equipElement.m_cEnchantLevel )
                << BUILD_LOG( equipElement.m_DesignCoordiID )
                << BUILD_LOG( equipElement.m_vecAttribute.size() )
                << BUILD_LOG( equipElement.m_vecSocket.size() ) << END_LOG;

            continue;
        }

        mit->second.m_vecEquipItems.push_back( equipElement );
    }
    return true;
}

bool KGSDBThread::GetUserEquipItem( IN const DWORD dwUID_, IN const std::vector<KItem>& vecInven_, IN OUT std::vector<KEquipItemInfo>& vecEquipItem_ )
{
    //{ call dbo.CEGACharacterEquipitem_select ( @1 ) }
    //@1 ; @iLoginUID_ int
    //
    //n select ( @1, @2, @3 )
    //@1  ; CharType tinyint
    //@3  ; ItemUID  int

    DBJIF2( m_kODBC.Query( L"{ call dbo.CEGACharacterEquipitem_select ( %d ) }", dwUID_ ) );
    while ( m_kODBC.Fetch() != SQL_NO_DATA ) {

        // TODO : 아이템 타입 처리.
        char cCharType = -1;
        KEquipItemInfo equipElement;
        VERIFY_QUERY( m_kODBC >> cCharType
            >> equipElement.m_dwUID
            >> equipElement.m_dwID );

        // 장착 아이템이 내가 보유 하고 있는 아이템인지 확인.
        std::vector<KItem>::const_iterator vit;
        vit = std::find_if( vecInven_.begin(), vecInven_.end(),
        boost::bind( &KItem::m_ItemUID, _1 ) == equipElement.m_dwUID );

        // 보유 하고 있지 않으면 장착 하지 않음..
        if ( vit == vecInven_.end() ) {
            continue;
        }
        equipElement.m_dwID = vit->m_ItemID;
        equipElement.m_vecAttribute = vit->m_vecAttribute;
        equipElement.m_vecSocket = vit->m_vecSocket;
        equipElement.m_cGradeID = vit->m_cGradeID;
        equipElement.m_nEquipLevel = static_cast<int>( vit->m_sEquipLevel );
        equipElement.m_cEnchantLevel = vit->m_cEnchantLevel;
        equipElement.m_cEnchantEquipGradeID = vit->m_cEnchantEquipGradeID;

        vecEquipItem_.push_back( equipElement );
    }
    return true;
}

bool KGSDBThread::UpdateUserEquipInfo( IN const DWORD dwUID_, IN const std::map<char, KCharacterInfo>& mapCharacterInfo_, IN const std::map<char,std::pair<bool,GCITEMUID> >& mapChangeWeapon_, IN const std::set<DWORD>& setChangeEquipInfo_ )
{
    /*
    L"{ call dbo.CEGACharacterEquipitemDeleted_update_20120115 ( %d, %d ) }"

    { call dbo.CEGACharacterEquipitemDeleted_update_20120115 ( @1, @2 ) }
    @1: @iLoginUID_ int
    @2: @iCharType_ tinyint

    1 select ( @1 )
    @1: OK int
    0       : 성공
    -101이하: 트랜잭션 에러
    */

    std::set<DWORD>::const_iterator csit;
    for( csit = setChangeEquipInfo_.begin(); csit != setChangeEquipInfo_.end(); ++csit ) {
        DBJIF2( m_kODBC.Query( L"{ call dbo.CEGACharacterEquipitemDeleted_update_20120115 ( %d, %d ) }", dwUID_, *csit ) );
        if ( m_kODBC.BeginFetch() ) {
            int nOK = -99;
            VERIFY_QUERY( m_kODBC >> nOK );
            m_kODBC.EndFetch();

            _LOG_SUCCESS( nOK == 0, L"장착 정보 지우기 실패.. OK : " << nOK )
                << BUILD_LOG( dwUID_ )
                << BUILD_LOG( *csit )
                << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

            if( nOK != 0 )
                return false;
        }
    }

    std::map<char, KCharacterInfo>::const_iterator mit;
    for ( mit = mapCharacterInfo_.begin() ; mit != mapCharacterInfo_.end() ; ++mit ) {
        csit = setChangeEquipInfo_.find( static_cast<DWORD>(mit->first) ); // 변경되지 않은 캐릭터는 장착 정보 갱신 하지 않음.
        if( csit == setChangeEquipInfo_.end() ) {
            continue;
        }

        if ( mit->second.m_vecEquipItems.size() > KUser::MAX_EQUIP_SIZE ) {
            // 사이즈가 과도하면 해당 캐릭터는 업데이트 하지 않겠음..
            START_LOG( cerr, L"장착 아이템 크기가 이상함. UID : " << dwUID_ )
                << BUILD_LOG( KUser::MAX_EQUIP_SIZE )
                << BUILD_LOGc( mit->second.m_cCharType )
                << BUILD_LOG( mit->second.m_vecEquipItems.size() )
                << END_LOG;
            continue;
        } // end if

        /*
        캐릭터 장착 정보
        L"{ call dbo.CEGACharacterEquipItem_merge_20120110 ( %d, %d, %I64d, .. , %I64d ) }"

        { call dbo.CEGACharacterEquipItem_merge_20120110 ( @a, @b, @00, @01, .. , @99 ) }
        @a : @LoginUID  int
        @b : @CharType  tinyint
        @00: @ItemUID00 bigint = null
        @01: @ItemUID01 bigint = null
        ..
        @99: @ItemUID99 bigint = null

        1 return ( @1 )
        @1: OK         int
        0       : 성공
        -1      : 유저 없음
        -2      : 캐릭터 없음
        -101이하: 트랜젝션 에러
        */
        const USHORT usMaxCnt = 80;
        std::wstringstream stm;
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        DWORD dwCnt = 0;

        std::vector<KEquipItemInfo>::const_iterator vit;
        for ( vit = mit->second.m_vecEquipItems.begin() ; vit != mit->second.m_vecEquipItems.end() ; ++vit ) {
            if( stm.str().size() == 0 ) {
                stm << L"{ call dbo.CEGACharacterEquipItem_merge_20120110 ( ";
                stm << static_cast<int>(dwUID_) << L",";
                stm << static_cast<int>(mit->first);
            }

            if( vit->m_dwUID > 0 ) {
                stm << L"," << static_cast<__int64>(vit->m_dwUID);
                ++dwCnt;
            }

            if( dwCnt > usMaxCnt ) {
                stm << L") }";
                DBJIF2( m_kODBC.QueryToWString( stm.str() ) );

                if ( m_kODBC.BeginFetch() ) {
                    int nOK = -99;
                    VERIFY_QUERY( m_kODBC >> nOK );
                    m_kODBC.EndFetch();

                    if ( nOK != 0 )
                        START_LOG( cerr, L"장착 실패 OK : " << nOK << L", UID : " << dwUID_ << L", CharType : " << (int)mit->first ) << END_LOG;
                } // end if
                stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
                stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
                dwCnt = 0;
            }
        }

        if( stm.str().size() != 0 ) {
            stm << L") }";
            DBJIF2( m_kODBC.QueryToWString( stm.str() ) );

            if ( m_kODBC.BeginFetch() ) {
                int nOK = -99;
                VERIFY_QUERY( m_kODBC >> nOK );
                m_kODBC.EndFetch();

                if ( nOK != 0 )
                    START_LOG( cerr, L"장착 실패 OK : " << nOK << L", UID : " << dwUID_ << L", CharType : " << (int)mit->first ) << END_LOG;
            } // end if
            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        }
    }

    // 체인지웨폰 정보 기록
    std::map<char,std::pair<bool,GCITEMUID> >::const_iterator cmitChangeWeapon;
    for ( cmitChangeWeapon = mapChangeWeapon_.begin() ; cmitChangeWeapon != mapChangeWeapon_.end() ; ++cmitChangeWeapon ) {
        csit = setChangeEquipInfo_.find( static_cast<DWORD>(cmitChangeWeapon->first) ); // 변경되지 않은 캐릭터는 장착 정보 갱신 하지 않음.
        if( csit == setChangeEquipInfo_.end() ) {
            continue;
        }

        LIF( m_kHelper.UpdateChangeWeaponInfoToDB( dwUID_, cmitChangeWeapon->first, cmitChangeWeapon->second.first, cmitChangeWeapon->second.second ) );
    }

    return true;
}

void KGSDBThread::CheckEventInfoEnable( IN const DWORD dwUID_, OUT std::set<DWORD>& setStringList_ )
{
    setStringList_.clear();

    if( GetTodayEventCount( dwUID_, UET_EVENT_INFO_CHACK ) > 0 ) return;
    LIF( SetTodayEventDoneDiff( dwUID_, UET_EVENT_INFO_CHACK ) );

    std::set<DWORD> setStringList;
    setStringList.clear();
    SiKGCHelper()->GetEventStringList( setStringList );
    setStringList_.swap( setStringList );
}

bool KGSDBThread::CheckRecomUserLevelup( IN const DWORD dwUID_, IN const DWORD dwRecomUserUID_, OUT std::pair<int,int>& prRecomRewardInfo_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFGAFriendFriendLevelItem_select_B_20110920 ( %d, %d, %d ) }"

    { call dbo.FFGAFriendFriendLevelItem_select_B_20110920 ( @1, @2, @3 ) }
    @1 ; @추천받은님_ int
    @2 ; @추천한님_   int
    @3 ; @iVersion_   tinyint ; 버전

    1 select ( @1, @2 )
    @1 ; 추천한님의레벨달성단계   tinyint
    @2 ; 추천한님의아이템지급단계 tinyint
    */

    prRecomRewardInfo_.first = -1; // 추천한님의레벨달성단계
    prRecomRewardInfo_.second = -1; // 추천한님의아이템지급단계

    DBJIF2( m_kODBC.Query( L"{ call dbo.FFGAFriendFriendLevelItem_select_B_20110920 ( %d, %d, %d ) }",
        dwRecomUserUID_, dwUID_, nVersion_ ) );

    if( m_kODBC.BeginFetch() ) 
    {
        VERIFY_QUERY( m_kODBC >> prRecomRewardInfo_.first >> prRecomRewardInfo_.second );
    }
    m_kODBC.EndFetch();

    START_LOG( clog, L"CheckRecomUserLevelup" )
        << BUILD_LOG( m_kODBC.m_strLastQuery )
        << BUILD_LOG( prRecomRewardInfo_.first )
        << BUILD_LOG( prRecomRewardInfo_.second ) << END_LOG;

    return true;
}

void KGSDBThread::UpdateUserLevelUpInfo( IN const DWORD dwUID_, IN const DWORD dwRecomUserUID_, IN const int nLevel_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFGAFriendFriendLevelItem_merge_B_20110920 ( %d, %d, %d, %d ) }"

    { call dbo.FFGAFriendFriendLevelItem_merge_B_20110920 ( @1, @2, @3, @4 ) }
    @1 ; @추천받은님_             int
    @2 ; @추천한님_               int
    @3 ; @추천한님의레벨달성단계_ tinyint
    @4 ; @iVersion_               tinyint ; 버전

    1 select ( @1 )
    @1 ; OK int
    0        ; 성공
    -1       ; 추천받은님 정보가 없음
    -2       ; 추천한님 정보가 없음
    -3       ; 레벨 아이템 단계 정보가 없음
    -101이하 ; 트랜잭션 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.FFGAFriendFriendLevelItem_merge_B_20110920 ( %d, %d, %d, %d ) }",
        dwRecomUserUID_, dwUID_, nLevel_, nVersion_ ) );

    if( m_kODBC.BeginFetch() ) 
    {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );

        _LOG_SUCCESS( nOK == 0, L"Result : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();
}

void KGSDBThread::UpdateLevelUpRecomReward( IN const DWORD dwUID_, IN const DWORD dwRecomUserUID_, IN const int nLevel_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFGAFriendFriendLevelItem_update_B_20110920 ( %d, %d, %d, %d ) }"

    { call dbo.FFGAFriendFriendLevelItem_update_B_20110920 ( @1, @2, @3, @4 ) }
    @1 ; @추천받은님_               int
    @2 ; @추천한님_                 int
    @3 ; @추천한님의아이템지급단계_ tinyint
    @4 ; @iVersion_   tinyint ; 버전

    1 select ( @1 )
    @1 ; OK int
    0        ; 성공
    -1       ; 추천받은님 정보가 없음
    -2       ; 추천한님 정보가 없음
    -3       ; 레벨 아이템 단계 정보가 없음
    -101이하 ; 트랜잭션 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.FFGAFriendFriendLevelItem_update_B_20110920 ( %d, %d, %d, %d ) }",
        dwRecomUserUID_, dwUID_, nLevel_, nVersion_ ) );

    if( m_kODBC.BeginFetch() ) 
    {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );

        _LOG_SUCCESS( nOK == 0, L"Result : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();
}

void KGSDBThread::InsertRecommendUserLevelUp( IN const DWORD dwUID_, IN const DWORD dwRecomUID_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFEVFriendFriendSupplied_insert_20110920 ( %d, %d, %d ) }"
    추천한 유저가 처음 15레벨을 달성할 경우

    { call dbo.FFEVFriendFriendSupplied_insert_20110920 ( @1, @2, @3 ) }
    @1 ; @iLoginUID_  int 추천받은 유저
    @2 ; @iLoginUIDE_ int 추천한 유저
    @3 ; @iVersion_   tinyint 버전

    1 return ( @1 )
    @1 ; OK
    0        ; 성공
    -1       ; 추천받은 유저 정보가 없음
    -2       ; 추천한 유저 정보가 없음
    -101이하 ; 트랜젝션 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.FFEVFriendFriendSupplied_insert_20110920 ( %d, %d, %d ) }",
        dwRecomUID_, dwUID_, nVersion_ ) );

    if( m_kODBC.BeginFetch() ) 
    {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );

        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();
}

bool KGSDBThread::CheckRecommendSuppliedItem( IN const DWORD dwUID_, OUT std::map<DWORD, std::pair<int,int> >& mapRecomUserList_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFGAFriendFriendLevelItem_select_A_20110920 ( %d, %d ) }"

    { call dbo.FFGAFriendFriendLevelItem_select_A_20110920 ( @1, @2 ) }
    @1 ; @추천받은님_ int
    @2 ; @iVersion_   tinyint ; 버전

    n select ( @1, @2, @3 )
    @1 ; 추천한님                   int
    @2 ; 추천한님의레벨달성단계     tinyint
    @3 ; 추천받은님의아이템지급단계 tinyint
    */

    mapRecomUserList_.clear();

    DBJIF2( m_kODBC.Query( L"{ call dbo.FFGAFriendFriendLevelItem_select_A_20110920 ( %d, %d ) }",
        dwUID_, nVersion_ ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        DWORD dwUserUID;
        std::pair<int,int> prRecomInfo;

        VERIFY_QUERY( m_kODBC >> dwUserUID
                              >> prRecomInfo.first
                              >> prRecomInfo.second );

        mapRecomUserList_[ dwUserUID ] = prRecomInfo;
        START_LOG( clog, L"Recom User UID : " << dwUserUID << L",RecomLv : " << prRecomInfo.first << L",RewardLv : " << prRecomInfo.second ) << END_LOG;
    }

    START_LOG( clog, L"Check Recom Info. size : " << mapRecomUserList_.size() )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    return ( !mapRecomUserList_.empty() );
}

void KGSDBThread::UpdateRecommendSuppliedItem( IN const DWORD dwUID_, IN const DWORD dwRecommendUID_, IN const int nRewardLevel_, IN const int nVersion_ )
{
    /*
    L"{ call dbo.FFGAFriendFriendLevelItem_update_A_20110920 ( %d, %d, %d, %d ) }"

    { call dbo.FFGAFriendFriendLevelItem_update_A_20110920 ( @1, @2, @3, @4 ) }
    @1 ; @추천받은님_                 int
    @2 ; @추천한님_                   int
    @3 ; @추천받은님의아이템지급단계_ tinyint
    @4 ; @iVersion_   tinyint ; 버전

    1 select ( @1 )
    @1 ; OK int
    0        ; 성공
    -1       ; 추천받은님 정보가 없음
    -2       ; 추천한님 정보가 없음
    -3       ; 레벨 아이템 단계 정보가 없음
    -101이하 ; 트랜잭션 에러
    */

    DBJIF( m_kODBC.Query( L"{ call dbo.FFGAFriendFriendLevelItem_update_A_20110920 ( %d, %d, %d, %d ) }",
        dwUID_, dwRecommendUID_, nRewardLevel_, nVersion_ ) );

    if( m_kODBC.BeginFetch() ) 
    {
        int nOK = -99;
        VERIFY_QUERY( m_kODBC >> nOK );

        _LOG_SUCCESS( nOK == 0, L"Resut : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    m_kODBC.EndFetch();
}

// void KGSDBThread::GetGuildTotalRank( OUT std::map< int, std::map< char, std::vector<KWinRateRank> >>& mapGuildRank_ )
// {
//     //게임내 랭킹 정보 리턴
//     //일일/주간/월간/전체 상위 100위
//     //
//     //길드 랭킹은 매일 6시에 갱신됩니다
//     /*
//     L"{ call dbo.RGGARankGuild_select_top100 }"
//     { call dbo.RGGARankGuild_select_top100 }
//     100 return ( @1, @2, @3, @4, @5, @6, @7, @8 )
//     @1 ; Type  tinyint
//     1 ; 일일랭킹
//     2 ; 주간랭킹
//     3 ; 월간랭킹
//     4 ; 전체랭킹
//     @2 ; Rank  int
//     @3 ; NID   int
//     @4 ; Name  nvarchar( 12 )
//     @5 ; Point bigint
//     @6 ; Win   int
//     @7 ; Lose  int
//     @8 ; Ratio float
//     */
// 
// 
//     KWinRateRank kRank;
//     char cCharType = -1;
//     int nRankType;
//     std::map< char, std::vector<KWinRateRank> > mapRank;
//     DBJIF( m_kODBC.Query( L"{ call dbo.RGGARankGuild_select_top100 }" ) );
// 
//     while( m_kODBC.Fetch() != SQL_NO_DATA )
//     {
//         nRankType = -1;
//         VERIFY_QUERY( m_kODBC >> nRankType              >> kRank.m_nRank        >> kRank.m_dwGuildUID
//                               >> kRank.m_strGuildName   >> kRank.m_dwGuildPoint  >> kRank.m_dwWin
//                               >> kRank.m_dwLose         >> kRank.m_fWinRate );
// 
//         // 기존 랭크에서 사용하던 것 초기화.
//         kRank.m_cCharType = cCharType;
//         kRank.m_dwLoginUID = 0;
//         kRank.m_nGuildMark = 0;
//         kRank.m_strNick = L"";
// 
//         if( nRankType > -1 )
//         {
//             mapRank[cCharType].push_back( kRank );
//             mapGuildRank_.insert( std::map< int, std::map< char, std::vector<KWinRateRank> > >::value_type( nRankType, mapRank ) );
//         }
//     }
//     m_kODBC.EndFetch();
// }

void KGSDBThread::GetRankingInfo( const std::wstring& strQuery, char eRankType )
{
    std::map<char, std::vector<KWinRateRank> > mapWinRank;
    DBJIF( m_kODBC.Query( strQuery.c_str() ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KWinRateRank kRankData;
        char cTabType = -1;
        VERIFY_QUERY( m_kODBC >> cTabType
            >> kRankData.m_nRank
            >> kRankData.m_dwLoginUID
            >> kRankData.m_dwGuildUID
            >> kRankData.m_nGuildMark
            >> kRankData.m_strGuildName
            >> kRankData.m_cCharType
            >> kRankData.m_strNick
            >> kRankData.m_dwWin
            >> kRankData.m_dwLose
            >> kRankData.m_fWinRate );
        kRankData.m_dwGuildPoint = 0;
        SiKGuildManager()->GetGuildMarkStringName( kRankData.m_dwGuildUID, kRankData.m_strMarkName );

        if( cTabType > -1 )
            mapWinRank[cTabType-1].push_back( kRankData );
    }
    std::map<char, std::vector<KWinRateRank> >::iterator mit;
    for( mit = mapWinRank.begin() ; mit != mapWinRank.end() ; ++mit )
    {
        std::sort( mit->second.begin(), mit->second.end(),
            boost::bind(&KWinRateRank::m_nRank, _1) < boost::bind(&KWinRateRank::m_nRank, _2) );
    }

    SiKRankManager()->UpdateWinRankData( eRankType, mapWinRank );
}


void KGSDBThread::GetGuildRank( int nType_, IN std::wstring& strName_, OUT KWinRateRank& kRank_ )
{
//게임내 랭킹 정보 리턴
//자기 길드 순위 및 길드 순위 검색시

/*
L"{ call dbo.GSGAGuildSystemRank_select_1_guild ( %d, N'%s' ) }"

{ call dbo.GSGAGuildSystemRank_select_1_guild ( @1, @2 ) }
@1 ; @iType_   tinyint
서버 ; 디비 ; 종류
0    ; 1    ; GPoint일일랭킹
1    ; 2    ; GPoint주간랭킹
2    ; 3    ; GPoint월간랭킹
3    ; 4    ; GPoint전체랭킹
4    ; 5    ; BattlePoint랭킹
@2 ; @strName_ nvarchar( 12 )

1 return ( @1, @2, @3, @4, @5, @6, @7 )
@1 ; Rank   int
@2 ; NID    int
@3 ; Name   nvarchar( 12 )
@4 ; GPoint bigint
@5 ; Win    int
@6 ; Lose   int
@7 ; Ratio  float
*/
    if( nType_ < 0 ) {
        return;
    }
    DBJIF( m_kODBC.Query( L"{ call dbo.GSGAGuildSystemRank_select_1_guild ( %d, N'%s' ) }", nType_, strName_.c_str() ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> kRank_.m_nRank 
                              >> kRank_.m_dwGuildUID 
                              >> kRank_.m_strGuildName 
                              >> kRank_.m_dwGuildPoint 
                              >> kRank_.m_dwWin 
                              >> kRank_.m_dwLose 
                              >> kRank_.m_fWinRate );
    }
    m_kODBC.EndFetch();

}

bool KGSDBThread::InsertTransformItemLog( IN const DWORD dwUID_, IN const GCITEMID dwItemID_, IN const int nItemCount_, IN const KDropItemInfo kReward_ )
{
    /*
    20090810. mk8253.
    L"{ call dbo.ITGA_item_transform_insert ( %d, %d, %d, %d, %d ) }"

    { call dbo.ITGA_item_transform_insert ( @1, @2, @3, @4, @5 ) }
    @1 ; iLoginUID_ int  => 계정
    @2 ; iOItemID_ int    => Original ItemID
    @3 ; iOCount_ int    => Original Item Count
    @4 ; iTItemID_ int    => Transfrom ItemID
    @5 ; iTCount_ int    => Transfrom Item Count

    1 return ( @1 )
    @1 ; OK   int
    0         ; 성공
    -101 이하 ; 트랜잭션 에러
    */

    // DB에 기록 남기기.
    DBJIF2( m_kODBC.Query( L"{ call dbo.ITGA_item_transform_insert ( %d, %d, %d, %d, %d, %d ) }",
        dwUID_, dwItemID_, nItemCount_, kReward_.m_ItemID, kReward_.m_nDuration, kReward_.m_nPeriod ) );

    int nOK = -99;

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    if( nOK != 0 ) {
        START_LOG( cerr, L"아이템 교환 DB기록 작업 실패." )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
        return false;
    }

    return true;
}

int KGSDBThread::GetGWCEventUserInfo( IN const DWORD dwUID_ )
{
    //L"{ call dbo.WCGAWorldChampionship2009_select_user ( %d ) }"
    //
    //{ call dbo.WCGAWorldChampionship2009_select_user ( @1 ) }
    //@1 ; @iLoginUID_ int
    //
    //1 select ( @1 )
    //@1 ; Data int

    int nAccCount = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.WCGAWorldChampionship2009_select_user ( %d ) }", dwUID_ ) );

    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nAccCount );
        m_kODBC.EndFetch();
    } // end if

    return nAccCount;
}

bool KGSDBThread::UpdateGWCDonationData( IN const DWORD dwUID_, IN const int nCount_ )
{
    //{ call dbo.WCGAWorldChampionship2009_merge_user ( @1, @2 ) }
    //@1 ; @iLoginUID_ int
    //@2 ; @iData_     int
    //
    //1 select ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-1       ; 유저 정보가 없음
    //-101이하 ; 트랜잭션 에러

    DBJIF2( m_kODBC.Query( L"{ call dbo.WCGAWorldChampionship2009_merge_user ( %d, %d ) }",
        dwUID_, nCount_ ) );

    int nOK = -99;
    if ( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
        m_kODBC.EndFetch();
    }// end if

    if ( nOK != 0 ) {
        START_LOG( cerr, L"GWC 기부, UID : " << dwUID_ << L", OK : " << nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }

    return (nOK == 0);
}

bool KGSDBThread::AddServerCountData( IN int nData_, IN int nType_, IN time_t tmDate_ )
{
    //{ call dbo.SDGA_server_data_merge_integer ( @1, @2, @3 ) }
    //@1 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
    //@2 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨
    //@3 ; @iData_ [int]                  ; 차이값을 주쇼

    //1 return ( @1 )
    //@1 ; OK int
    //0        ; 성공
    //-101이하 ; 트랜잭션 에러
    CTime tmTime( tmDate_ );
    DBJIF2( m_kODBC.Query( L"{ call dbo.SDGA_server_data_merge_integer ( N'%s', %d, %d ) }",
        (LPCTSTR)tmTime.Format( L"%Y%m%d" ), nType_, nData_ ) );

    int nOK = -99;
    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    LOG_SUCCESS( nOK == 0 )
        << BUILD_LOG( nOK )
        << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;

    if( nOK == 0 ) return true;
    return false;
}

int KGSDBThread::GetTotalServerCountData( IN time_t tmDate_, IN const DWORD dwType_ )
{
    /*  
    20090116. microcat.  

    L"{ call dbo.SDGA_server_data_select_integer ( N'%s', %d ) }"  

    { call dbo.SDGA_server_data_select_integer ( @1, @2 ) }  
    @1 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함  
    @2 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨  

    1 return ( @1 )  
    @1 ; Data int  
    */  
    CTime tmTime( tmDate_ );
    int nOK = 0;
    DBJIF2( m_kODBC.Query( L"{ call dbo.SDGA_server_data_select_integer ( N'%s', %d ) }",
        (LPCTSTR)tmTime.Format( L"%Y%m%d" ), dwType_ ) );

    if( m_kODBC.BeginFetch() )
    {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();

    return nOK;
}


void KGSDBThread::SetInitDicePlayLog( IN const DWORD dwUID_, IN const DWORD dwInitHour_ )
{
    /*
    mk8253. 20100116.
    오늘초기화기록추가

    L"{ call dbo.DPGADicePlayInit_insert( %d, %d ) }"

    { call dbo.DPGADicePlayInit_insert ( @1, @2 ) }
    @1 ; @iLoginUID_ int
    @2 ; @iSdtHour_  int
    */
    int nOK = -99;
    DBJIF( m_kODBC.Query( L"{ call dbo.DPGADicePlayInit_insert ( %d, %d ) }",
        dwUID_, dwInitHour_ ) );

    if( m_kODBC.BeginFetch() ) {
        VERIFY_QUERY( m_kODBC >> nOK );
    }
    m_kODBC.EndFetch();
    if( nOK != 0 ) {
        START_LOG( cerr, L"보드게임 주사위 초기화 기록 남기기 실패. " )
            << BUILD_LOG( nOK )
            << BUILD_LOG( dwUID_ )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
}

bool KGSDBThread::UpdateDungeonStat( IN std::map<KDungeonStat,int>& mapData_ )
{
    // 던전 게임 결과 통계
    /*
    { call dbo.DSGADungeonStat_merge ( @1, @2, @3, @4, @5, @6, @7, @8, @9 ) }
    @1 ; @iDungeonID_     tinyint   던전번호
    @2 ; @iStageID_       tinyint   해당 던전의 스테이지 번호
    @3 ; @iLevel_         tinyint   이 sp를 호출한 유저님이 선택한 캐릭터의 레벨
    @4 ; @iPlayTime_      tinyint   해당 던전에서의 플레이타임(분)
    @5 ; @iLifeCNT_       tinyint   남은 목숨수
    @6 ; @iRegenCNT_      tinyint   수호의 반지 또는 부활 보너스 사용 횟수
    @7 ; @iClearStageCNT_ tinyint   이 sp를 호출하기 전까지 스테이지 클리어수
    @8 ; @iResult_        tinyint   0 ; 던전 클리어 / 0보다 큰 양수 ; 이탈의 이유(클라이언트측에서 결정)
    @9 ; @iCNT_           int     = 1

    1 select ( @1 )
    @1  ; OK int
    0        ; 성공
    -101이하 ; 트랜잭션 에러
    */
    int nRet = -99;
    std::map<KDungeonStat,int>::const_iterator cmit;

    for ( cmit = mapData_.begin() ; cmit != mapData_.end() ; ++cmit ) {
        DBJIF2( m_kODBCStat.Query( L"{ call dbo.DSGADungeonStat_merge ( %d, %d, %d, %d, %d, %d, %d, %d, %d ) }",
            cmit->first.m_nGameMode,
            cmit->first.m_nStageID,
            cmit->first.m_nCharLevel,
            cmit->first.m_nPlayTime,
            cmit->first.m_nLifeCount,
            cmit->first.m_nRegenCount,
            cmit->first.m_nClearStageCount,
            cmit->first.m_nResult,
            cmit->second ) );

        if ( m_kODBCStat.BeginFetch() ) {
            VERIFY_QUERY( m_kODBCStat >> nRet );
            m_kODBCStat.EndFetch();
        }// end if

        _LOG_SUCCESS( nRet == 0, L"nRet : " << nRet )
            << BUILD_LOG( m_kODBCStat.m_strLastQuery ) << END_LOG;
    }

    return true;
}