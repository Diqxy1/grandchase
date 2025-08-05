#include <WinSock2.h>
#include "GSDBThread.h"
#include <dbg/dbg.hpp>
#include "GSSimLayer.h"
#include "GSDBLayer.h"
#include "GameServer.h"
#include "Room.h"
#include "GuildManager.h"
#include <boost/bind.hpp>
#include "GCHelper.h"
#include "EnchantStrong.h"
#include "support_User.h"
#include "ItemManager.h"
#include "ItemBreakUp.h"
#include "ServerBenefit.h"
#include "LoginOutStat.h"
#include "GCPoint.h"
#include "ResultManager.h"
#include "IPAdvantage.h"
#include "SkillManager.h"
#include "PostConfig.h"

#define CLASS_TYPE KGSDBThread

_INT_IMPL_ON_FUNC( EVENT_SELL_ITEM_REQ, KDB_EVENT_SELL_ITEM_INFO )
{
    EVENT_CHECK_OUT( EVENT_SELL_ITEM_REQ );

    KEVENT_SELL_ITEM_ACK kPacket;
    kPacket.m_vecSellItem = kPacket_.m_vecSellItem;
    kPacket.m_nGP = 0;
    kPacket.m_nOK = -99;
    int nDiffCount = 0;
    __int64 waitUID = KItem::INVALID_ITEM;
    std::vector<KItem>::iterator vit;
    SET_ERROR( ERR_UNKNOWN );

    if( !UpdateUserData( kPacket_.m_kData ) )
    {
        START_LOG( cerr, L"게임서버/DB간 사용자 정보 동기화 실패." )
            << BUILD_LOG( strLogin_ )
            << END_LOG;
        SET_ERR_GOTO( ERR_DB_COMMON_03, sell_end );
    }

    for( vit = kPacket_.m_vecSellItem.begin(); vit != kPacket_.m_vecSellItem.end(); ++vit ) {
        if ( vit->m_nPeriod >= 0 ) { // 기간제.
            LIF( PureEraseItem( strLogin_, dwUID_, vit->m_ItemUID, cCharType_ ) );
        }
        else {
            //판매 하려는 아이템의 수량이 변경 되었으면 적용 하자.
            nDiffCount = vit->m_nCount - vit->m_nInitCount;
            if ( nDiffCount != 0 ) {
                if ( !UpdateCountItem( dwUID_, vit->m_ItemUID, nDiffCount ) ) {
                    SET_ERR_GOTO( ERR_SELL_ITEM_04, sell_end );
                }
            }

            if ( !m_kHelper.SellItem( dwUID_, vit->m_ItemID, vit->m_ItemUID, kPacket.m_nGP, waitUID, cCharType_ ) ) {
                SET_ERR_GOTO( ERR_SELL_ITEM_05, sell_end );
            }

            if ( waitUID != KItem::INVALID_ITEM ) {
                LIF( m_kHelper.InsertSingleWaitItemToInvenOrVDepot( dwUID_, cCharType_, waitUID, KItemManager::EWIT_ITEM_SELL, kPacket.m_vecItem ) );
            }
        }
    }

    SET_ERROR( NET_OK );
sell_end:

    kPacket.m_nOK = NetError::GetLastNetError();
    QUEUING_ACK_TO_USER( EVENT_SELL_ITEM_ACK );
    _LOG_SUCCESS( kPacket.m_nOK == NetError::NET_OK, L" 아이템 판매 Msg : " << NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( kPacket.m_nGP )
        << BUILD_LOG( kPacket.m_vecSellItem.size() )
        << BUILD_LOG( kPacket.m_vecItem.size() ) << END_LOG;
}

INT_IMPL_ON_FUNC( DB_EVENT_SERVER_INFO_REQ )
{
    // 오픈테섭의 경우 체크섬 확인과 동접정보 갱신은 본섭에서 처리한다.
    KODBC* pkODBC = SiKGSDBLayer()->GetODBC( "__odbcMain" );   // 오픈테섭인 경우 본섭으로 작업해야 함

    if( pkODBC == NULL )    pkODBC = &m_kODBC;                  // 이외의 경우엔 기본 db로 연결한다.

    if( kPacket_.m_bFirstDBWork )
    {
        {// locking scope
            KLocker lock( pkODBC->GetCS() );

            //DBJIF( pkODBC->Query( L"SELECT CheckSum FROM dbo.ClientChecksum(NOLOCK) WHERE ProtocolVer = %d",
            DBJIF( pkODBC->Query( L"{ call dbo.sp_executesql ( "
                L"N'select [CheckSum] from dbo.ClientCheckSum with(nolock) where ProtocolVer = @1', "
                L"N'@1 int', %d ) }",
                kPacket_.m_nProtocolVer ) );

            KSerializer ks;
            KSerBuffer  buff;
            DWORD       dwChecksum = 0x00;

            ks.BeginReading( &buff );

            while( pkODBC->Fetch() !=  SQL_NO_DATA )
            {
                VERIFY_QUERY( *pkODBC >> buff );
                ks.Get( dwChecksum );
                SiKGameServer()->AddChecksum( dwChecksum );
            }
        }

        //이벤트가 있는지 확인한다.
        UpdateTimeEvent();

        //061211. kkurrung. 모드 카운트 이름.

        //--서버에서 알고 있는 모드 정보를 db에 기록(1회) 최초 서버 부팅시 실행해주면 좋겠습니다.
        //call dbo.set_desc_mode_count 'Data nvarchar(4000)'
        //Data인자는 다음과 같이 보내주셔야 합니다.
        //ModeCode,ModeName,ModeCode,ModeName,ModeCode,ModeName,...,ModeCode,ModeName
        //생각해보니까 trim을 하지 않았습니다 -_-;
        //빈칸을 넣지 않도록 해주세요 ㅎㅎ
        //ModeCode는 해당 모드의 고유 식별 번호이고, ModeName은 해당 모드의 이름입니다.

        //if( !SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) ) //사내 테섭이 아닌경우
        RegistGameMode( pkODBC );
        // 070201. kkurrung. 전체 메세지
        UpdateLoudMsg();

        // 070423. kkurrung. 달개근 정보(직접 호출)
        if( SiKGCPoint()->IsGCPointSwitch() == true ) 
        {
            ON_DB_EVENT_UPDATE_ATTENDANCE_REWARD_ITEM( std::wstring( L""), 0L, 0 );
            ON_DB_EVENT_UPDATE_LUCKYDAY_LIST( std::wstring( L"" ), 0L, 0 );
        }

        // 채팅 이벤트 가져오기..
        ON_EVENT_GET_CHATTING_EVENT_MSG_REQ( std::wstring(L""), 0L, 0 );

        //  통계 필드 이름 등록..
        StatisticsInit();

        // 용사의 섬 랭킹 업데이트
        UpdateIndigoRank();

        // 수량 아이템 판매 정보.
        SelectSellItemList();

        // 단일 GP 아이템..
        //UpdateSingleItemList();
        //UpdateSetItemList();

        UpdateItemEquipLevelList();
        //GetFullGuildInfo(); // 길드 전체정보 퍼오기. 제거예정.

        {
            std::set<GCITEMID> setExceptList;
            m_kHelper.GetItemBreakupExceptionList( setExceptList );
            SiKItemManager()->m_kItemBreakup.SetExceptListFromDB( setExceptList );
        }

        //if( CheckCrashBefore( kPacket_.m_strIP, kPacket_.m_usPort ) == true ) {
        //    SiKGameServer()->SetServerCrashBefore();
        //}

    } // first DB Work end if  

    LIF( GetServerName( *pkODBC, kPacket_.m_strIP, kPacket_.m_usPort ) );
    LIF( GetServerSort( *pkODBC, kPacket_.m_strIP, kPacket_.m_usPort ) );

    int nServerID = SiKGameServer()->GetServerID();

    JIF( UpdateCCU( *pkODBC, kPacket_.m_nProtocolVer, kPacket_.m_strIP, kPacket_.m_usPort, kPacket_.m_mapChannellingUserNum[KUser::USER_CT_TOTAL], nServerID ) );

    // 일단 채널링 유저수에 대한 보완 코드 추가.
    // 종료 시점에 0으로 초기화 하는 부분에서 예외상황 발생해서 해당 부분 추가.
    if( kPacket_.m_mapChannellingUserNum[KUser::USER_CT_TOTAL] == 0 ) {
        kPacket_.m_mapChannellingUserNum.clear();
    }

    int nChannellingUserAllCount = 0;
    for ( int i = KUser::USER_CT_PUBLISHER_2 ; i < KUser::USER_CT_MAX ; ++i ) {
        nChannellingUserAllCount += kPacket_.m_mapChannellingUserNum[i];
    }

    kPacket_.m_mapChannellingUserNum[KUser::USER_CT_PUBLISHER_1] = kPacket_.m_mapChannellingUserNum[KUser::USER_CT_TOTAL] - nChannellingUserAllCount;

    // 전체서버, 넷마블, 투니랜드 순으로 호출.
    for ( int i = KUser::USER_CT_TOTAL ; i < KUser::USER_CT_MAX ; ++i ) {
        JIF( m_kHelper.GetCommonSP().UpdateChannelCCU( KLoginOutStat::CCU, i, nServerID, kPacket_.m_mapChannellingUserNum[i] ) );
    }

    std::vector<KServerInfo> vecServerInfo;
    JIF( GetServerList( *pkODBC, kPacket_.m_nProtocolVer, vecServerInfo ) );
    SiKGameServer()->SetServerList( vecServerInfo );
    SiKGameServer()->SetServerIP();
    {
        std::vector<KServerInfo>::iterator vit;
        vit = std::find_if( vecServerInfo.begin(), vecServerInfo.end(), boost::bind( &KServerInfo::m_dwUID, _1) == (DWORD)(nServerID) );
        if ( vit != vecServerInfo.end() ) {
            SiKServerBenefit()->SetLevelRange( vit->m_prLvRange );
            SiKGameServer()->SetMaxUserNum( vit->m_iMaxUserNum );
        }
        else {
            SiKServerBenefit()->SetLevelRange( std::pair<int,int>(-1,-1) );
        }
    }

    // 080722. 메신저 주소 얻어오기(Tick으로 주기적으로 받고있음)
    if( GetMsgServerList( vecServerInfo ) )
    {
        SiKGameServer()->SetMsgServerList( vecServerInfo );
    }

    if( kPacket_.m_hSyncEvent != NULL ) // 060404. florist. 메인 스레드에 작업의 종료를 알려야 하는 경우.
    {
        SetEvent( kPacket_.m_hSyncEvent );
    }

    // TCP 서버 정보 가져오기.
    std::vector<KSimpleServerInfo> vecUseTCPServerInfo;
    std::vector<KSimpleServerInfo> vecNoUseTCPServerInfo;
    GetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );
    SiKGameServer()->SetTCPServerList( vecUseTCPServerInfo, vecNoUseTCPServerInfo );
}

INT_IMPL_ON_FUNC( DB_EVENT_WRITE_MODE_COUNT_NOT )
{
//     if( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) )
//     {
//         START_LOG( clog, L"사내 테스트 서버에서는 모드 카운트를 남기지 않는다." ) << END_LOG;
//         return;
//     }
    //--매시간마다 모드 정보를 db에 기록(정해신 시간마다)
    //call dbo.up_update_user_mode_count 'Data nvarchar(4000)'
    //여기도 마찬가지로 Data인자에다가 넣어주시면 되는데 위에꺼랑은 조금 다릅니다.
    //ModeCode,ModeCount,ModeCode,ModeCount,...,ModeCode,ModeCount
    //ModeName대신에 ModeCount를 입력해주시면 깔끔하겠습니다.

    KODBC* pkODBC = SiKGSDBLayer()->GetODBC( "__odbcMain" );    // 오픈테섭인 경우 본섭으로 작업해야 함
    if( pkODBC == NULL )    pkODBC = &m_kODBC;                  // 이외의 경우엔 기본 db로 연결한다.

    // 챔피언 난이도
    int nChampionDifficulty = GC_GMD_CHAMPION_BEGIN;
    int nBreakDifficulty = GC_GMD_BREAK_BEGIN;

    //std::wstringstream stmQuery;
    std::wstringstream stmLog;
    {// 로그/ 쿼리 문자열 생성
        for ( int iMode = 0 ; iMode < NUM_GC_GAME_MODE ; ++iMode ) {
            int iID = iMode << 16;

            if ( kPacket_.m_avecModeCount[iMode].empty() ) {
                continue;
            }

            if ( KRoom::IsDifficultyMode(iMode) ) {
                int nMaxDifficulty = KRoom::GetMaxDifficulty(iMode);
                bool bExistChampionDifficulty = SiKResultManager()->IsExistDifficulty(iMode, nChampionDifficulty);
                bool bExistBreakDifficulty = SiKResultManager()->IsExistDifficulty(iMode, nBreakDifficulty);

                for ( int nDifficult = GC_GMD_NORMAL_BEGIN ; nDifficult < nMaxDifficulty ; ++nDifficult ) {

                    int nCurrentDifficult = nDifficult;
					if( bExistChampionDifficulty && nDifficult == nMaxDifficulty-1 )
					{
						nCurrentDifficult = GC_GMD_CHAMPION_BEGIN;
					}
					if( bExistBreakDifficulty && nDifficult == nMaxDifficulty-1 )
					{
						nCurrentDifficult = GC_GMD_BREAK_BEGIN;
					}

                    m_kHelper.UpdateUserModeCount( iID + nCurrentDifficult, kPacket_.m_avecModeCount[iMode][nDifficult], *pkODBC );

                    stmLog  << L"," << KRoom::ms_szModeName[iMode] << L"(" << nCurrentDifficult << L") : " << kPacket_.m_avecModeCount[iMode][nDifficult] << std::endl;
                }
            }
            else {
                m_kHelper.UpdateUserModeCount( iID, kPacket_.m_avecModeCount[iMode][0], *pkODBC );

                stmLog  << L"," << KRoom::ms_szModeName[iMode] << L" : " << kPacket_.m_avecModeCount[iMode][0] << std::endl;
            }// end if ... else
        }// end for

    }// 로그/ 쿼리 문자열 생성

    //int nOK = -1;
    //{// locking scope
    //    KLocker lock( pkODBC->GetCS() );

    //    DBJIF( pkODBC->Query( L"{ call dbo.up_update_user_mode_count ( N'%s' ) }",
    //        stmQuery.str().c_str() ) );
    //    if( pkODBC->BeginFetch() )
    //    {
    //        VERIFY_QUERY( (*pkODBC) >> nOK );
    //    }
    //    pkODBC->EndFetch();
    //}
    START_LOG( clog, L"게임 모드 DB 기록 완료 " << stmLog.str() ) << END_LOG;

    //stmQuery.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stmQuery.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    stmLog.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    stmLog.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

_INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_GUILD_POINT_REQ, KGuildGameResult )
{
    KGuildGameResult kPacket;
    int nResult = -99;
    nResult = UpdateGuildPoint( kPacket_.m_dwUserUID,
        kPacket_.m_dwGuildUID,
        kPacket_.m_dwPoint,
        kPacket_.m_nWin, 
        kPacket_.m_nLose );

    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_dwPoint = kPacket_.m_dwPoint;
    
    
    switch ( nResult ) {
        case 0: // 성공
            SET_ERROR( NET_OK );
            kPacket.m_dwPoint = kPacket_.m_dwPoint;
            break;
        case -1: // 길드 정보 없음
            SET_ERROR( ERR_GUILD_UPDATE_SCORE_00 );
            break;
        case -2: // 길드 멤버 아님
            SET_ERROR( ERR_GUILD_UPDATE_SCORE_01 );
            break;
        default:
            SET_ERROR( ERR_DB_COMMON_00 ); // 트랜잭션 에러, 아닐 수도 있을라나
    }

    if( nResult != 0 ) {
        START_LOG( cerr, L"길드전 포인트, 승패 변경 실패 , result : " << nResult ) << END_LOG;
    }

    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( kPacket.m_dwUserUID )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_dwPoint )
        << BUILD_LOG( kPacket_.m_nWin )
        << BUILD_LOG( kPacket_.m_nLose ) << END_LOG;

    QUEUEING_ACK_TO_GUILD_MANAGER( DB_EVENT_UPDATE_GUILD_POINT_ACK );
}

_INT_IMPL_ON_FUNC( DB_EVENT_UPDATE_GUILD_BATTLE_POINT_REQ, KGuildGameResult )
{
    KGuildGameResult kPacket;
    int nResult = -99;
    nResult = UpdateGuildBattlePoint( kPacket_.m_dwGuildUID,
        kPacket_.m_nBPointDiff );

    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_nBPointDiff = kPacket_.m_nBPointDiff;

    switch ( nResult ) {
        case 0: // 성공
            SET_ERROR( NET_OK );
            kPacket.m_dwPoint = kPacket_.m_dwPoint;
            break;
        case -1: // 길드 정보 없음
            SET_ERROR( ERR_GUILD_UPDATE_SCORE_00 );
            break;
        default:
            SET_ERROR( ERR_DB_COMMON_00 ); // 트랜잭션 에러
    }

    if( nResult != 0 ) {
        START_LOG( cerr, L"길드전 포인트, 승패 변경 실패 , result : " << nResult ) << END_LOG;
    }

    kPacket.m_nOK = NetError::GetLastNetError();

    LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_nBPointDiff ) << END_LOG;

    QUEUEING_ACK_TO_GUILD_MANAGER( DB_EVENT_UPDATE_GUILD_POINT_ACK );
}

// #DB_EVENT_INSERT_DROP_ITEM_REQ
_INT_IMPL_ON_FUNC( DB_EVENT_INSERT_DROP_ITEM_REQ, KInsertDropItem )
{
    /////////////////////미국 이벤트
    int nGCMedalCount = 0;
    /////////////////////미국 이벤트

    // 전달해주는 인벤토리는 절대 클리어 하면 안됨..
    // 이유는 더블 크리스탈 카드 사용하면 미리 넣어두기 때문임..
    std::map<DWORD, DROP_ITEMS>::iterator mit;

    for ( mit = kPacket_.m_mapEachUserDrops.begin() ; mit != kPacket_.m_mapEachUserDrops.end() ; ++mit ) {
        // 드랍 목록에 있는 유저가 ACK에 있는지 확인
        std::vector<KGameResultOut>::iterator vitResult;
        vitResult = std::find_if( kPacket_.m_kEndGameAck.m_vecGameResult.begin(), kPacket_.m_kEndGameAck.m_vecGameResult.end(),
                boost::bind( &KGameResultOut::m_dwUID, _1 ) == mit->first );
        
        if ( vitResult == kPacket_.m_kEndGameAck.m_vecGameResult.end() ) {
            continue;
        }

        DROP_ITEMS vecDropList;
        DROP_ITEMS::iterator vit;

        for ( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit ) {

            DROP_ITEMS::iterator vit2;
            vit2 = std::find_if( vecDropList.begin(), vecDropList.end(),
                boost::bind( &KDropItemInfo::m_ItemID, _1 ) == vit->m_ItemID );

            /////////////////////미국 이벤트
            if ( vit->m_ItemID == KUser::GC_MEDAL_ID ) {
                nGCMedalCount += vit->m_nDuration;
            }
            /////////////////////미국 이벤트

            vitResult->m_mapObtainCount[vit->m_ItemID] += 1;
            if( vit2 != vecDropList.end() ) {
                if ( vit2->m_nDuration < 0 && vit->m_nDuration < 0 &&
                    vit2->m_nPeriod > 0 && vit->m_nPeriod > 0 ) {

                    vit2->m_nPeriod += vit->m_nPeriod;
                    continue;
                }

                if ( vit2->m_nPeriod < 0 && vit->m_nPeriod < 0 &&
                    vit2->m_nDuration > 0 && vit->m_nDuration > 0 ) {

                    vit2->m_nDuration += vit->m_nDuration;
                    continue;
                }
            }

            vecDropList.push_back( *vit );
        }

        if ( vecDropList.empty() && vitResult->m_mapResultReward.empty() && vitResult->m_vecStageReward.empty() ) {
            continue;
        }

        if ( kPacket_.m_kEndGameAck.m_nRewardType == KEndGameAck::STAGE_REWARD ) {
            if( vitResult->m_vecStageReward.empty() )
            {
                START_LOG( cerr, L"vitResult->m_vecStageReward.empty()" )
                    << BUILD_LOG( vitResult->m_strLogin )
                    << END_LOG;
            }
            else
            {
                for ( auto element : vitResult->m_vecStageReward ) 
                {
                    vitResult->m_mapObtainCount[ element.m_ItemID ] += 1;
                    vecDropList.push_back( element );
                }
            }
        }

        DWORD dwUID = 0;
        if ( !LoginToUID( vitResult->m_strLogin, dwUID ) ) {
            START_LOG( cerr, L"Login을 Login UID로 변환 실패.. Name : " << vitResult->m_strLogin ) << END_LOG;
            continue;
        }

        for ( vit = vecDropList.begin() ; vit != vecDropList.end() ; ++vit ) {
            if ( SiKGCHelper()->ProcessSnowCoin( vit->m_ItemID, vit->m_nDuration, vitResult->m_vecInv ) ) {
                InsertSnowCoin( dwUID, vit->m_nDuration );
                continue;
            }
            LIF( m_kHelper.InsertSingleItemJob( dwUID, vitResult->m_ucCharType, vit->m_ItemID, vit->m_nDuration, vit->m_nPeriod, KItemManager::EWIT_GAME_DROP, vitResult->m_vecInv ) );
        }

        std::map<int,KDropItemInfo>::iterator mitResultReward;
        std::vector<KItem>::iterator vitResultReward1;
        std::vector<KItem>::iterator vitResultReward2;
        std::vector<KItem> vecItemInfo;
        vecItemInfo.clear();

        // 결과 보상 시스템 문제 확인 로그.
        if( vitResult->m_mapResultReward.empty() && kPacket_.m_kEndGameAck.m_nRewardType == KEndGameAck::RESULT_REWARD ) {
            START_LOG( cerr, L"vitResult->m_mapResultReward.empty()" ) 
                << BUILD_LOG( vitResult->m_strLogin )
                << END_LOG;
        }

        if ( !vitResult->m_mapResultReward.empty() && kPacket_.m_kEndGameAck.m_nRewardType == KEndGameAck::RESULT_REWARD ) 
        {
            for( mitResultReward = vitResult->m_mapResultReward.begin(); mitResultReward != vitResult->m_mapResultReward.end(); ++mitResultReward ) {
                LIF( m_kHelper.InsertSingleItemJob( dwUID, vitResult->m_ucCharType, mitResultReward->second.m_ItemID, mitResultReward->second.m_nDuration, mitResultReward->second.m_nPeriod, KItemManager::EWIT_GAME_RESULT_REWARD, vecItemInfo, mitResultReward->second.m_cGradeID, mitResultReward->second.m_cEquipLevel ) );
            }
            for ( vitResultReward1 = vecItemInfo.begin(); vitResultReward1 != vecItemInfo.end(); ++vitResultReward1 ) {
                if( vitResultReward1->m_sEquipLevel != 0 ) {
                    bool bUpdateSuccess = true;
                    LIF( bUpdateSuccess = bUpdateSuccess && m_kHelper.UpdateLevel(vitResult->m_dwUID, vitResultReward1->m_sEquipLevel, KItemLevelTable::IL_EQUIP_LV, *vitResultReward1, vitResult->m_ucCharType ) );
                }
            }
            for( vitResultReward2 = vecItemInfo.begin(); vitResultReward2 != vecItemInfo.end(); ++vitResultReward2 ) {
                vitResult->m_vecInv.push_back( *vitResultReward2 );
            }
        }

        START_LOG( clog, L"게임 종료후 Item Drop, Name : " << vitResult->m_strLogin )
            << BUILD_LOG( vitResult->m_vecInv.size() ) 
            << BUILD_LOG( vitResult->m_mapObtainCount.size() ) << END_LOG;
    }

    _QUEUING_ACK_TO_USER( DB_EVENT_INSERT_DROP_ITEM_ACK, kPacket_.m_kEndGameAck );

    /////////////////////미국 이벤트
    if ( SiKGSSimLayer()->m_bUSEvent && nGCMedalCount > 0 ) {
        DBJIF( m_kODBC.Query( L"exec dbo.GMW_grandchase_medal %d", nGCMedalCount ) );

        int nOK = -99;
        if ( m_kODBC.BeginFetch() ) {
            VERIFY_QUERY( m_kODBC >> nOK );
        }
        m_kODBC.EndFetch();

        LOG_SUCCESS( nOK == 0 )
            << BUILD_LOG( nOK )
            << BUILD_LOG( m_kODBC.m_strLastQuery ) << END_LOG;
    }
    /////////////////////미국 이벤트

}

_INT_IMPL_ON_FUNC( EVENT_QUERY_NORMAL_ITEM_LIST_REQ, KPacket2Name )
{
    KItemInv kPacket;
    kPacket.m_vecItemInv.clear();

    KLocker lock( m_kODBC.GetCS() );

    if( kPacket_.m_bIsNickname )
    {
        int  nOK = TranslateNicknameToLogin( kPacket_.m_strName, kPacket_.m_strName );
        if( nOK != 0 )  goto quit;
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_select_normal_item ( N'%s' ) }",
        kPacket_.m_strName.c_str() ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KItemInfoForOperator   kii;
        VERIFY_QUERY( m_kODBC
            >> kii.m_strGoodsName
            >> kii.m_strOwnerLogin
            >> kii.m_strBuyerLogin
            >> kii.m_dwGoodsID
            >> kii.m_dwGoodsUID
            >> kii.m_tRegDate
            >> kii.m_tStartDate
            >> kii.m_tEndDate
            >> kii.m_iPeriod );
        kii.m_tDelDate = static_cast<time_t>(-1);
        kPacket.m_vecItemInv.push_back( kii );
    }

quit:
    QUEUING_ACK_TO_USER( EVENT_QUERY_NORMAL_ITEM_LIST_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_QUERY_DURATION_ITEM_LIST_REQ, KPacket2Name )
{
    KDurationInv kPacket;
    kPacket.m_vecDurationItemInv.clear();

    KLocker lock( m_kODBC.GetCS() );

    if( kPacket_.m_bIsNickname )
    {
        int  nOK = TranslateNicknameToLogin( kPacket_.m_strName, kPacket_.m_strName );
        if( nOK != 0 )  goto quit;
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_select_duration_item ( N'%s' ) }",
        kPacket_.m_strName.c_str() ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KDurationItemInfoForOperator   kdii;
        VERIFY_QUERY( m_kODBC
            >> kdii.m_strGoodsName
            >> kdii.m_strOwnerLogin
            >> kdii.m_strBuyerLogin
            >> kdii.m_dwGoodsID
            >> kdii.m_dwGoodsUID
            >> kdii.m_iDurationNum
            >> kdii.m_tRegDate );
        kdii.m_tDelDate = static_cast<time_t>(-1);
        kPacket.m_vecDurationItemInv.push_back( kdii );

    }

quit:
    QUEUING_ACK_TO_USER( EVENT_QUERY_DURATION_ITEM_LIST_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_REQ, KPacket2Name )
{
    KItemInv kPacket;
    kPacket.m_vecItemInv.clear();

    KLocker lock( m_kODBC.GetCS() );

    if( kPacket_.m_bIsNickname )
    {
        int  nOK = TranslateNicknameToLogin( kPacket_.m_strName, kPacket_.m_strName );
        if( nOK != 0 )  goto quit;
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_select_del_normal_item ( N'%s' ) }",
        kPacket_.m_strName.c_str() ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KItemInfoForOperator   kii;
        VERIFY_QUERY( m_kODBC
            >> kii.m_strGoodsName
            >> kii.m_strOwnerLogin
            >> kii.m_strBuyerLogin
            >> kii.m_dwGoodsID
            >> kii.m_dwGoodsUID
            >> kii.m_tRegDate
            >> kii.m_tStartDate
            >> kii.m_tEndDate
            >> kii.m_iPeriod
            >> kii.m_tDelDate );
        kPacket.m_vecItemInv.push_back( kii );
    }

quit:
    QUEUING_ACK_TO_USER( EVENT_QUERY_SALED_NORMAL_ITEM_LIST_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_QUERY_SALED_DURATION_ITEM_LIST_REQ, KPacket2Name )
{
    KDurationInv kPacket;
    kPacket.m_vecDurationItemInv.clear();

    KLocker lock( m_kODBC.GetCS() );

    if( kPacket_.m_bIsNickname )
    {
        int nOK = TranslateNicknameToLogin( kPacket_.m_strName, kPacket_.m_strName );
        if( nOK != 0 )  goto quit;
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_select_del_duration_item ( N'%s' ) }",
        kPacket_.m_strName.c_str() ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KDurationItemInfoForOperator   kdii;
        VERIFY_QUERY( m_kODBC
            >> kdii.m_strGoodsName
            >> kdii.m_strOwnerLogin
            >> kdii.m_strBuyerLogin
            >> kdii.m_dwGoodsID
            >> kdii.m_dwGoodsUID
            >> kdii.m_tRegDate
            >> kdii.m_tDelDate );
        kdii.m_iDurationNum = -1;
        kPacket.m_vecDurationItemInv.push_back( kdii );
    }

quit:
    QUEUING_ACK_TO_USER( EVENT_QUERY_SALED_DURATION_ITEM_LIST_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_GP_REQ, KAddItemInfo )
{
    KPacketAck kPacket;

    KLocker lock( m_kODBC.GetCS() );

    //050810. microcat. 따로 작업하지 않음.

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_insert_gp_normal_item ( N'%s', N'%s', %d ) }",
        strLogin_.c_str(), kPacket_.m_strName.c_str(), kPacket_.m_dwGoodsID ) );
    if( m_kODBC.BeginFetch() )
    {
        kPacket.m_cOK = -1;

        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );
        m_kODBC.EndFetch();
    }

    QUEUING_ACK_TO_USER( EVENT_ADD_NORMAL_ITEM_GP_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_ADD_DURATION_ITEM_GP_REQ, KAddItemInfo )
{
    KPacketAck kPacket;

    KLocker lock( m_kODBC.GetCS() );

    //050810. microcat. 따로 작업하지 않음.

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_insert_gp_duration_item ( N'%s', N'%s', %d ) }",
        strLogin_.c_str(), kPacket_.m_strName.c_str(), kPacket_.m_dwGoodsID ) );
    if( m_kODBC.BeginFetch() )
    {
        kPacket.m_cOK = -1;

        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );
        m_kODBC.EndFetch();
    }

    QUEUING_ACK_TO_USER( EVENT_ADD_DURATION_ITEM_GP_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_ADD_NORMAL_ITEM_CASH_REQ, KAddItemInfo )
{
    KPacketAck kPacket;

    KLocker lock( m_kODBC.GetCS() );

    //050810. microcat. 따로 작업하지 않음.

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_insert_cash_normal_item ( N'%s', N'%s', %d ) }",
        strLogin_.c_str(), kPacket_.m_strName.c_str(), kPacket_.m_dwGoodsID ) );
    if( m_kODBC.BeginFetch() )
    {
        kPacket.m_cOK = -1;

        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );
        m_kODBC.EndFetch();
    }

    QUEUING_ACK_TO_USER( EVENT_ADD_NORMAL_ITEM_CASH_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_ADD_DURATION_ITEM_CASH_REQ, KAddItemInfo )
{
    KPacketAck kPacket;

    KLocker lock( m_kODBC.GetCS() );

    //050810. microcat. 따로 작업하지 않음.

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_insert_cash_duration_item ( N'%s', N'%s', %d ) }",
        strLogin_.c_str(), kPacket_.m_strName.c_str(), kPacket_.m_dwGoodsID ) );
    if( m_kODBC.BeginFetch() )
    {
        kPacket.m_cOK = -1;

        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );
        m_kODBC.EndFetch();
    }

    QUEUING_ACK_TO_USER( EVENT_ADD_DURATION_ITEM_CASH_ACK );
}

INT_IMPL_ON_FUNC( EVENT_REMOVE_ITEM_REQ )
{
    KPacketAck kPacket;

    KLocker lock( m_kODBC.GetCS() );

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_del_item ( %d, %d ) }", kPacket_.m_dwGoodsUID, dwUID_));

    if( m_kODBC.BeginFetch() )
    {
        kPacket.m_cOK = -1;

        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );
        m_kODBC.EndFetch();
    }

    QUEUING_ACK_TO_USER( EVENT_REMOVE_ITEM_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_QUERY_USER_INFO_REQ, KPacket2Name )
{
    KEVENT_QUERY_USER_INFO_ACK kPacket;

    KLocker lock( m_kODBC.GetCS() );

    if( kPacket_.m_bIsNickname )
    {
        int nOK = TranslateNicknameToLogin( kPacket_.m_strName, kPacket_.m_strName );
        if( nOK != 0 )  goto quit;
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_select_user_data ( N'%s' ) }",
        kPacket_.m_strName.c_str() ) );
    if( m_kODBC.BeginFetch() )
    {
        kPacket.m_cOK = -1;

        VERIFY_QUERY( m_kODBC >> kPacket.m_cOK );

        if( kPacket.m_cOK == 0 )
        {
            VERIFY_QUERY( m_kODBC
                >> kPacket.m_strName
                >> kPacket.m_firstLoginTime
                >> kPacket.m_lastLoginTime
                >> kPacket.m_iGP
                >> kPacket.m_iGrade
                >> kPacket.m_iGuildid
                >> kPacket.m_iLevel0
                >> kPacket.m_iLevel1
                >> kPacket.m_iLevel2 );

        }

        m_kODBC.EndFetch();
    }

quit:
    QUEUING_ACK_TO_USER( EVENT_QUERY_USER_INFO_ACK );
}

_INT_IMPL_ON_FUNC( EVENT_CHANGE_STRONG_ITEM_REQ, KPacket2Name )
{
    KEVENT_CHANGE_STRONG_ITEM_ACK kPacket;
    kPacket.m_vecStrongItemInv.clear();

    KLocker lock( m_kODBC.GetCS() );

    if( kPacket_.m_bIsNickname )
    {
        int nOK = TranslateNicknameToLogin( kPacket_.m_strName, kPacket_.m_strName );
        if( nOK != 0 )  goto quit;
    }

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_select_change_strong_item_list ( N'%s' ) }",
        kPacket_.m_strName.c_str() ) );

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        KStrongItemInv   ksii;
        VERIFY_QUERY( m_kODBC
            >> ksii.m_strName
            >> ksii.m_dwBeforeGoodsID
            >> ksii.m_dwNextGoodsID
            >> ksii.m_tRegDate );
        kPacket.m_vecStrongItemInv.push_back( ksii );
    }

quit:
    QUEUING_ACK_TO_USER( EVENT_CHANGE_STRONG_ITEM_ACK );
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_QUERY_ITEM_INFO_REQ )
{
    KEVENT_QUERY_ITEM_INFO_ACK kPacket;
    kPacket.m_vecDefaultItem.clear();

    KLocker lock( m_kODBC.GetCS() );

    DBJIF( m_kODBC.Query( L"{ call dbo.up_admin_select_iteminfo_data_list }" ) );

    KItemDefaultInfo   kidi;

    while( m_kODBC.Fetch() != SQL_NO_DATA )
    {
        VERIFY_QUERY( m_kODBC
            >> kidi.m_dwGoodsID
            >> kidi.m_strGoodsName
            >> kidi.m_bCash
            >> kidi.m_iStrongLevel
            >> kidi.m_iDurationNum
            >> kidi.m_iCharType
            >> kidi.m_bShowWeb
            >> kidi.m_iGem
            >> kidi.m_iCrystal );
        kPacket.m_vecDefaultItem.push_back( kidi );

    }

    QUEUING_ACK_TO_USER( EVENT_QUERY_ITEM_INFO_ACK );
}

INT_IMPL_ON_FUNC(DB_EVENT_CHANGE_CHARACTER_INFO_REQ)
{
    EVENT_CHECK_OUT(DB_EVENT_CHANGE_CHARACTER_INFO_REQ);
    KEVENT_CHANGE_CHARACTER_INFO_ACK kPacket;
    std::vector<std::pair<GCITEMUID, GCITEMID>> vecDesignCoordi;

    //////////////////////////////////////////////////////////////////////////////////////////
    // ĳ���� ������ ����
    if (!UpdateUserData(kPacket_.m_kUserData)) {
        START_LOG(cerr, L"ĳ���� ����� �⺻ ������ ���� ����.")
            << BUILD_LOG(kPacket_.m_kUserData.m_strLogin)
            << END_LOG;
    }

    std::vector<KItem>::iterator vitPCItem;
    vitPCItem = kPacket_.m_vecCountItemInv.begin();
    while (vitPCItem != kPacket_.m_vecCountItemInv.end()) {
        if (vitPCItem->m_ItemUID <= SiKIPAdvantage()->GetMaximumPCBangItemUID()) {
            vitPCItem = kPacket_.m_vecCountItemInv.erase(vitPCItem);
            continue;
        }
        vitPCItem++;
    }

    if (!kPacket_.m_vecCountItemInv.empty()) {
        LIF(UpdateCountItems(dwUID_, kPacket_.m_vecCountItemInv));
    }

    if (kPacket_.m_cPrevChar != -1) { // ������ �÷��� ����� �ִ� ĳ���Ͱ� ���� ��쿡 ���ؼ��� DB�� �����Ѵ�.
        // ���� ������ ����� �� ���� ������ ó���Ѵ�..
        if (!UpdatePetData(kPacket_.m_mapPetInfo, kPacket_.m_mapCharacterInfo, dwUID_, strLogin_)) {
            START_LOG(cerr, L"���Ӽ���/DB�� �� ���� ����ȭ ����.")
                << BUILD_LOG(strLogin_)
                << END_LOG;
        }

        if (!UpdateDepotPetData(kPacket_.m_mapDepotPetInfo, kPacket_.m_mapCharacterInfo, dwUID_, strLogin_)) {
            START_LOG(cerr, L"���Ӽ���/DB�� â�� �� ���� ����ȭ ����.")
                << BUILD_LOG(strLogin_)
                << END_LOG;
        }

        UpdateCharLastLoginDate(dwUID_, kPacket_.m_cPrevChar, kPacket_.m_dwSlotNum);
        // ���� ���� ������Ʈ.
        //LIF( UpdateUserEquipInfo( kPacket_.m_kUserData.m_dwLoginUID, kPacket_.m_kUserData.m_mapCharacterInfo, kPacket_.m_kUserData.m_mapChangeWeapon, kPacket_.m_setChangeEquipInfo ) );
        //--������ �޷� ������ ����
        if (SiKGCPoint()->IsGCPointSwitch() == true) { // GCPoint ����Ǿ��� ������ �۵�
            LIF(UpdatePointbagCalender(dwUID_, kPacket_.m_kUserData.m_cCurrentChar, kPacket_.m_vecCalendar));
        }
        //  �̼� ���� ������Ʈ
        UpdateMissionData(dwUID_, kPacket_.m_cPrevChar, kPacket_.m_vecMissionSlot);

        //���� ����� Īȣ �̼� ���   080526  woosh. 
        if (!kPacket_.m_vecCollectionSlot.empty()) {
            UpdateCollectionData(dwUID_, kPacket_.m_kUserData.m_cCurrentChar, kPacket_.m_vecCollectionSlot);
        }

        { // ��ų �н� ����
            std::set<int>::iterator sit;
            for (sit = kPacket_.m_setAddReserveSkill.begin(); sit != kPacket_.m_setAddReserveSkill.end(); ++sit)
            {
                SSkillElement sSkill;
                if (!SiKSkillManager()->GetSkill(*sit, sSkill)) continue;
                LIF(m_kHelper.SkillTreeID_insert(dwUID_, (int)sSkill.m_cCharType, (int)(unsigned char)sSkill.m_cPromotion, sSkill.m_nID));
            }

            for (sit = kPacket_.m_setDeleteReserveSkill.begin(); sit != kPacket_.m_setDeleteReserveSkill.end(); ++sit)
            {
                SSkillElement sSkill;
                if (!SiKSkillManager()->GetSkill(*sit, sSkill)) continue;
                LIF(m_kHelper.SkillTreeID_delete(dwUID_, (int)sSkill.m_cCharType, (int)(unsigned char)sSkill.m_cPromotion, sSkill.m_nID));
            }
        }

        UpdateSkillSet(dwUID_, kPacket_.m_mapSkillSetAdd, kPacket_.m_mapSkillSetDel);
        UpdateModeLevel(dwUID_, kPacket_.m_kUserData.m_cCurrentChar, kPacket_.m_mapOldDifficulty, kPacket_.m_mapDifficulty);
        UpdateLookEquipInfo(dwUID_, kPacket_.m_mapDiffLookEquip);

        // ���� �������� ���� ������ ���
        Update_ConnectionGiftBox_UserData_ToDB(dwUID_, static_cast<int>(kPacket_.m_cPrevChar), kPacket_.m_kConnectionGiftBoxUpdateData);

        // ���� ���� �ְ� ��� ���� ������ ���
        Update_DungeonPersonalRecord_UserData_ToDB(dwUID_, static_cast<int>(kPacket_.m_cPrevChar), kPacket_.m_mapDungeonPersonalRecordDiff);
    }
    // ���� �÷����ϴ� ĳ���� ���� ���� ��
    //////////////////////////////////////////////////////////////////////////////////////////
    // ���� ������ ĳ���� ������ ��������.
    if (SiKIPAdvantage()->IsPCBangUser(kPacket_.m_cPCBangType)) {
        // DB�� �ִ� �������� ������ �������� ���� ����. ���߿� ������ ��ó�� ������.
        InsertIPAdvantageDailyItem(dwUID_, kPacket_.m_cNextChar, std::vector<KItem>()); // �̾ȿ��� ���� �޾Ҵ��� ���� üũ��..
    }

    LIF(m_kHelper.InsertFullWaitItemToInven(dwUID_, kPacket_.m_cNextChar, std::vector<KItem>()));
    GetItemListFromDB(dwUID_, kPacket_.m_cNextChar, kPacket.m_vecInv, vecDesignCoordi);
    AdjustLookItemInfo(dwUID_, kPacket.m_vecInv, kPacket_.m_cNextChar);
    // PC�� ������ ���� ��������.
    // ���⼭ �Ǿ��� ��¥ �������� �־� �ش�.
    if (SiKIPAdvantage()->IsPCBangUser(kPacket_.m_cPCBangType)) {
        std::map<char, KCharacterInfo>::iterator mit;
        for (mit = kPacket_.m_kUserData.m_mapCharacterInfo.begin(); mit != kPacket_.m_kUserData.m_mapCharacterInfo.end(); ++mit) {
            if (mit->second.m_cCharType == kPacket_.m_cNextChar) { // ���� ������ ĳ������ �����۸� �����´�.
                SiKIPAdvantage()->GetConnectionItem(mit->second.m_cCharType, (int)mit->second.m_dwLevel, kPacket.m_vecInv, KIPAdvantage::PCBANG_PUBLISHER);
            }
        }
    }

    // �� ���� ��������.
    GetCharPetInfo(strLogin_, dwUID_, kPacket_.m_cNextChar, kPacket.m_mapPetInfo, kPacket.m_vecInv);
    // ���⼭ PC�� �� �������� �־��ش�.
    // 1. User�� Pet������ ��� �ִ� std::map<DWORD, PetInfo>�� ������ �־��ش�.
    // 2. User�� Inven�� �� ������, �� ���� ������, �� �ڽ�Ƭ ������ �־��ش�.
    if (SiKIPAdvantage()->IsPCBangUser(kPacket_.m_cPCBangType)) {
        SiKIPAdvantage()->GetPCBangPetItem(kPacket_.m_cNextChar, kPacket.m_mapPetInfo, kPacket.m_vecInv, KIPAdvantage::PCBANG_PUBLISHER);
    }
    // Īȣ ���� ��������.
    GetMainCollectionInfo(dwUID_, kPacket_.m_cNextChar, kPacket.m_vecCollection);
    GetSubCollectionInfo(dwUID_, kPacket_.m_cNextChar, kPacket.m_vecCollection);
    // �̼� ���� ��������.
    GetMainMissionInfo(dwUID_, kPacket_.m_cNextChar, kPacket.m_vecMissionSlot); // �̼� ���� ��������.
    GetSubMissionInfo(dwUID_, kPacket_.m_cNextChar, kPacket.m_vecMissionSlot);

    // ���� Ŭ���� ���� �޾ƿ���
    GetModeLevel(dwUID_, kPacket_.m_cNextChar, kPacket.m_mapDifficulty);
    if (kPacket.m_mapDifficulty.empty()) {
        GetOldModeLevel(dwUID_, kPacket.m_mapDifficulty);
        kPacket.m_bReadOldModwLevel = true;
    }
    LIF(m_kHelper.UserGameClear2_select_20111213(dwUID_, kPacket_.m_cNextChar, kPacket.m_mapDifficulty));
    // ���� ��Ʈ�� ���ڵ� ���� �޾ƿ��� (��Ʈ�� �ǵ��)
    GetUserControlRecordData(dwUID_, kPacket_.m_cNextChar, kPacket.m_mapDifficulty);

    // ĳ���� ���� ���� ���� ����.
    //LIF( GetUserEquipItem( dwUID_, kPacket.m_vecInv, kPacket.m_vecEquipItems ) );

    // ���� �������� ���� ������ ��������
    kPacket.m_kConnectionGiftBoxUpdateData = kPacket_.m_kConnectionGiftBoxUpdateData;
    Get_ConnectionGiftBox_UserData_FromDB(dwUID_, static_cast<int>(kPacket_.m_cNextChar), kPacket.m_kConnectionGiftBoxUpdateData);

    {
        //QUEUEING_ID_TO_DB( EVENT_NEW_POST_LETTER_INFO_REQ );
        std::vector<KPostItemInfo> vecPostItem;
        vecPostItem.clear();

        if (true == GetNewPostLetterInfo(strLogin_, dwUID_, cCharType_, vecPostItem))
        {
            _QUEUING_ACK_TO_USER(EVENT_NEW_POST_LETTER_INFO_ACK, vecPostItem);
        }
    }

    kPacket.m_cCharacter = kPacket_.m_cNextChar;

    START_LOG(clog, L"������ ���� ���")
        << BUILD_LOG(kPacket.m_vecInv.size()) << END_LOG;

    {
        if (!GetPetGlyph(dwUID_, kPacket.m_cCharacter, kPacket.m_vecPetGlyphCharData))
        {
            kPacket.m_vecPetGlyphCharData.clear();
            kPacket.m_dwPetGlyphSize = 0;
        }
        else
        {
            kPacket.m_dwPetGlyphSize = kPacket.m_vecPetGlyphCharData.size();
        }
    }

    if (kPacket_.m_bInPvP)
    {
        QUEUING_ACK_TO_USER(EVENT_IN_ROOM_CHANGE_CHARACTER_INFO_ACK);
    }
    else
    {
        QUEUING_ACK_TO_USER(EVENT_CHANGE_CHARACTER_INFO_ACK);
    }

    Get_DungeonPersonalRecord_UserData(dwUID_, kPacket_.m_cNextChar);
}