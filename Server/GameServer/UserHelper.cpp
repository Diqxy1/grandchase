#include "User.h"
#include <dbg/dbg.hpp>
#include "Channel.h"
#include "UserFSM.h"
#include "GSDBLayer.h"
#include "GSSimLayer.h"
#include "PetHelper.h"
#include "FatigueSystem.h"
#include "IndigoUtil.h"
#include <boost/random.hpp>
#include "GuildManager.h"
#include "SHCheckPoint.h"
#include "LogManager.h"
#include "ResultManager.h"
#include <boost/bind.hpp>
#include "Square.h"
#include "ResultManager.h"
#include "EnchantStrong.h"
#include "support_User.h"
#include "CollectionManager.h"
#include "SkillManager.h"
#include "PreHackingCheckManager.h"
#include "DicePlay.h"
#include "IPAdvantage.h"
#include "GCHelper.h"
#include "Worker.h"
#include "CoupleSystem.h"
#include "GCPoint.h"
#include "AgentConnector.h"
#include "AgitHelper.h"
#include "LevelResetEvent.h"
#include "DungeonManager.h"
#include "ResultReward.h"
#include "HeroDungeonManager.h"
#include "RecomHelper.h"
#include "CYOUBilling/CYOUBilling.h"
#include "Escort.h"
#include "CnConnector.h"
#include <boost/lexical_cast.hpp>
#include "CharacterGraduation.h"
#include <boost/foreach.hpp>
#include <algorithm>
#include "InfinityDungeon.h"
#include "GameDrop.h"
#include "Rainbow.h"
#include "ServerBenefit.h"
#include "StageReward.h"

#define CLASS_TYPE  KUser

void KUser::GetInfoSimple( OUT KUserInfo& kInfo_ )
{
    kInfo_.m_iWin       = 0;
    kInfo_.m_iLose      = 0;
    kInfo_.m_iGuildMark = -1;

    kInfo_.m_strLogin       = m_strName;
    kInfo_.m_strNickName    = m_strNickName;
    kInfo_.m_strNickColor   = m_strNickColor;
    kInfo_.m_dwUserUID      = GetUID();
    kInfo_.m_bMale          = m_bMale;
    kInfo_.m_usRoomID       = 0xFFFF;
    kInfo_.m_cKorGrade      = m_cGrade;

    const KCharacterInfo* pkCharacter = GetCurrentCharacter();
    if ( pkCharacter != NULL ) { 
        kInfo_.m_iWin  = pkCharacter->m_iWin;
        kInfo_.m_iLose = pkCharacter->m_iLose;
        kInfo_.m_cCharType = pkCharacter->m_cCharType;
        kInfo_.m_strCharNickName = pkCharacter->m_strCharName;
    }
}

bool KUser::GetInfo( OUT KUserInfo& kInfo_ )
{
    //유저리스트에 사용될 유저정보 설정

    GetInfoSimple( kInfo_ );

    // channel: 1, playing: 2, room: 3, square: 4
    switch( GetStateID() )
    {
    case KUserFSM::STATE_CHANNEL:
    case KUserFSM::STATE_CHANNELLOBBY:
        _JIF( m_pkChannel, return false );
        kInfo_.m_cState    = 1;
        break;
    case KUserFSM::STATE_ROOM:
        //case KUserFSM::STATE_JOIN:
        _JIF( m_pkRoom, return false );

        kInfo_.m_cState    = 3;
        kInfo_.m_usRoomID = m_pkRoom->m_usRoomID;
        break;
    case KUserFSM::STATE_PLAYING:
        _JIF( m_pkRoom, return false );

        kInfo_.m_cState    = 2;
        kInfo_.m_usRoomID = m_pkRoom->m_usRoomID;
        break;
    case KUserFSM::STATE_SQUARE_JOIN:
    case KUserFSM::STATE_SQUARE:
        _JIF( m_spSquare, return false );

        kInfo_.m_cState    = 4;
        break;
    case KUserFSM::STATE_MINIGAME:
    case KUserFSM::STATE_MINIGAME_PLAYING:
        _JIF( m_spSquare, return false );
        kInfo_.m_cState    = 5; // 
        break;
    default :
        _JIF( m_pkChannel, return false );

        START_LOG( cwarn, L"채널 내에 존재하는 유저의 상태가 이상함. Name : " << m_strName )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( m_pkChannel )
            << END_LOG;
        kInfo_.m_cState    = 0;
        return false;
    }

    return true;
}

bool KUser::GetInviteInfo( OUT KInviteUserInfo& kInfo_, IN bool bAdminCall_ /*=false*/ )
{
    if( GetStateID() != KUserFSM::STATE_CHANNEL &&
        GetStateID() != KUserFSM::STATE_SQUARE )
        return false;

    // 운영자 요청이 아니고, 내가 초대거부상태이면 리턴 false.
    if( !bAdminCall_ && m_bInviteDeny ) return false;

    kInfo_.m_cGrade         = m_cGrade;
    kInfo_.m_dwUserUID      = GetUID();
    kInfo_.m_strNickName    = m_strNickName;
    kInfo_.m_cBestCharGrade = GetBestCharGrade(); // 대만 홍콩 크레이드. ㅜㅜ

    const KCharacterInfo* pkCharacter = GetCurrentCharacter();
    if ( pkCharacter != NULL ) {
        kInfo_.m_cCharType = pkCharacter->m_cCharType;
        kInfo_.m_strCharNickName = pkCharacter->m_strCharName;
    }

    return true;
}

///@ return 업데이트 여부.
bool KUser::DBUpdateUserData()
{
    if( m_bGMuser ) return true; // 경험치 기록을 막기 위해서.

    if ( ::GetTickCount() - GetTick(EXP_DB_UPDATE) > SiKGSSimLayer()->GetExpDBUpdateTick() ) {
        SendExpUpdateToDBNotice();
        SetTick( EXP_DB_UPDATE );
    }

    if ( ::GetTickCount() - GetTick(DB_UPDATE) > SiKGSSimLayer()->GetDBUpdateUserDataTick() ) {
        KUserData   kReq;
        GetDBUpdateData( kReq );

        QUEUEING_EVENT_TO_DB( DB_EVENT_UPDATE_USER_DATA_REQ, kReq);

        if ( SiKGSSimLayer()->CheckTypeFlag( ST_INDIGO ) ) { 
            KDB_EVENT_UPDATE_INDIGO_REQ kReqIndigo;
            kReqIndigo.m_dwUserUID    = m_dwUID;
            kReqIndigo.m_iRPChange    = m_iRP.GetChangeValue();
            kReqIndigo.m_iWinChange   = m_iIndigoWin.GetChangeValue();
            kReqIndigo.m_iLoseChange  = m_iIndigoLose.GetChangeValue();
            if( kReqIndigo.m_iRPChange != 0 || kReqIndigo.m_iWinChange != 0 || kReqIndigo.m_iLoseChange != 0 )
                QUEUEING_EVENT_TO_DB( DB_EVENT_UPDATE_INDIGO_REQ, kReqIndigo );
        }

        // 창고 데이터 업데이트 
        KDepotUpdateData kDepotUpdateData;
        GetDBDepotUpdateData( kDepotUpdateData );
        QUEUEING_EVENT_TO_DB( DB_EVENT_UPDATE_DEPOT_DATA_REQ, kDepotUpdateData );
        
        SetTick( DB_UPDATE );
        return true;
    }
    return false;
}

void KUser::GetDBUpdateData( OUT KUserData& kUserData_ )
{
    kUserData_.m_strLogin           = GetName();
    kUserData_.m_dwLoginUID         = m_dwUID;
    kUserData_.m_iInitGP            = GetInitGP();
    kUserData_.m_iGamePoint         = GetGP();
    kUserData_.m_dwGuildUID         = m_kGuildUserInfo.m_dwGuildUID;
    kUserData_.m_dwGuildContPoint   = m_kGuildUserInfo.m_dwContributePoint;
    kUserData_.m_mapCharacterInfo   = m_mapCharacterInfo;
    kUserData_.m_nDiffGCPoint       = m_kGCPoint.GetGCPoint() - m_kGCPoint.GetInitGCPoint();
    kUserData_.m_nAccTime           = m_kCalendar.GetAccTime();
    kUserData_.m_cGrade             = m_cGrade;
    kUserData_.m_mapChangeWeapon    = m_mapChangeWeapon;
    kUserData_.m_nIncGpSum          = m_nIncGpSum;
    kUserData_.m_nDecGpSum          = m_nDecGpSum;
    kUserData_.m_cCurrentChar       = m_cCurrentCharacter;

    // 060107. florist. DB thread로 보낸 캐싱 데이터는 항상 성공한다고 가정한다.
    SetInitGP( GetGP() );
    m_kGCPoint.SetInitGCPoint( m_kGCPoint.GetGCPoint() );
    m_nIncGpSum = 0;
    m_nDecGpSum = 0;
    kUserData_.m_cCharType          = m_cCurrentCharacter;

    GetCharSlotNum( kUserData_.m_dwSlotNum );

    // 071216. kkurrung. 보너스 포인트 관련.
    std::map< char, KCacheData<int> >::const_iterator mit;
    mit = m_mapBaseBonus.find( kUserData_.m_cCharType );
    if( mit != m_mapBaseBonus.end() ) {
        int nDiffBouns = m_mapBaseBonus[kUserData_.m_cCharType].GetValue() - m_mapBaseBonus[kUserData_.m_cCharType].GetInitValue();
        if( 0 > nDiffBouns + m_mapBaseBonus[kUserData_.m_cCharType].GetInitValue() ) {
            nDiffBouns = 0;
        }

        kUserData_.m_mapCharacterInfo[kUserData_.m_cCharType].m_kBonusPoint.m_nBaseBonus = nDiffBouns;
        m_mapBaseBonus[kUserData_.m_cCharType].SetAllValue( m_mapBaseBonus[kUserData_.m_cCharType] );
    }

    std::map< char, KCacheData<int> >::const_iterator mitSpecial;
    mitSpecial = m_mapSpecialBonus.find( kUserData_.m_cCharType );
    if( mitSpecial != m_mapSpecialBonus.end() ) {
        int nDiffBouns = m_mapSpecialBonus[kUserData_.m_cCharType].GetValue() - m_mapSpecialBonus[kUserData_.m_cCharType].GetInitValue();
        if( 0 > nDiffBouns + m_mapSpecialBonus[kUserData_.m_cCharType].GetInitValue() ) {
            nDiffBouns = 0;
        }

        kUserData_.m_mapCharacterInfo[kUserData_.m_cCharType].m_kBonusPoint.m_nSpecialBonus = nDiffBouns;
        m_mapSpecialBonus[kUserData_.m_cCharType].SetAllValue( m_mapSpecialBonus[kUserData_.m_cCharType] );
    }

    SetTick( DB_UPDATE );
}

void KUser::GetDBUpdateExpData( OUT KEVENT_EXP_UPDATE_TO_DB_NOT& mapData_ )
{
    mapData_ = m_mapCharacterInfo;
    CharacterCachingDataSync();
}

void KUser::GetDBUpdateData( OUT KDB_EVENT_EXIT_GAME_NOT& kNot_ )
{
    GetDBUpdateData( kNot_.m_kUserData );
    kNot_.m_vecMissionSlot = m_vecMissionSlot;

    m_kInventory.GetDiffDurationItemAndUpdate( kNot_.m_vecDurationItemInv );

    kNot_.m_strIP               = GetIPStr();
    //kNot_.m_strWantedServerName 은 기본적으로 empty. 이름이 설정되지 않으면 서버이동이 아닌것으로 판단.
    kNot_.m_dwUserUID           = m_dwUID;
    kNot_.m_dwAuthType          = m_dwAuthType; // 서버간 이동시 개쉬인경우 처리하게 된다. 개쉬가 로긴서버로 가면 더이상 사용하지 않음.
    kNot_.m_bitCheckPoint       = m_bitCheckPoint;
    kNot_.m_dwPlayTime          = static_cast<DWORD>( GetElapsedTimeAfterAuth() );
    kNot_.m_nHackReport         = m_nHackReport;
    m_nHackReport               = 0; // 초기화..
    
    if( SiKGCPoint()->IsGCPointSwitch() == true ) // 쥐씨 포인트 시스템 사용하면
        m_kCalendar.GetCalendarDBInfo( kNot_.m_vecCalendar );

    kNot_.m_mapQuickSlot        = m_mapQuickSlot;
    kNot_.m_vecCollectionSlot   = m_vecCollectionSlot; //  080529  woosh. 컬렉션(칭호)
    kNot_.m_mapSkillSetDel      = m_mapInitSkillSets;
    kNot_.m_mapSkillSetAdd      = m_mapSkillSets;
    DumpSkillSet( m_mapInitSkillSets );
    DumpSkillSet( m_mapSkillSets );
    kNot_.m_setAddReserveSkill      = m_setAddReserveSkill;
    kNot_.m_setDeleteReserveSkill   = m_setDeleteReserveSkill;
    GetUserDifficultInfo( kNot_.m_mapDifficulty ); // 현재 플레이중인 던전 클리어 정보만 저장.
    kNot_.m_mapOldDifficulty        = m_mapOldDifficulty;

    GetPetCacheData( kNot_.m_mapPetInfo );
    GetDepotPetCacheData( kNot_.m_mapDepotPetInfo );

    // 커플 상태일 경우에만 데이터 기록
    kNot_.m_bIsCouple = ( m_kCoupleInfo.m_dwCoupleState == KCoupleSystem::CS_COUPLE ? true : false );
    if ( kNot_.m_bIsCouple ) {
        GetCoupleChangeEquip( kNot_.m_vecDelCoupleEquip, kNot_.m_vecUpdateCoupleEquip );
        kNot_.m_cCoupleMyCharType = m_kCoupleInfo.m_cCharType;
    }

    // 091103 tgkwon. 유저의 보드게임 정보 저장 
    if( SiKDicePlay()->IsRun() ) UpdateUserBoardInfo( kNot_.m_dwInitHour, kNot_.m_kUserBoardInfo, kNot_.m_strDiceLastTime );

    kNot_.m_nQuizBingoAccTime = m_kQuizBingo.GetAccTime();

    ExtractDiffLookEquip( kNot_.m_mapDiffLookEquip );
    kNot_.m_kGawibawiboInfo = m_kGawibawiboInfo;
    kNot_.m_mapSocksInfo = m_mapSocksInfo;
    GetCharLvPlayTime( kNot_.m_mapUserCharLvPlayTime );
    GetCharEquipInfo( kNot_.m_setChangeEquipInfo );

    m_KSongkranUserData.GetSongkranCachingData( kNot_.m_kSongkranUserData ); // 송크란 캐싱 데이터 얻기

    m_kRitasChristmasUserData.GetRitasChristmasEventUserData( kNot_.m_kRitasChristmasEventUserData );

    m_kVitalityUserData.GetVitalityUserData( kNot_.m_kVitalityUpdateData );

    if ( -1 != static_cast< int >( m_cCurrentCharacter ) ) { // 마지막 플레이한 캐릭터 정보가 있는 경우에만 기록하도록 한다
        GetConnectionGiftBoxUpdateData( kNot_.m_kConnectionGiftBoxUpdateData ); // 접속 선물상자 유저 데이터 얻기
        kNot_.m_mapDungeonPersonalRecordDiff = m_mapDungeonPersonalRecordDiff[ m_cCurrentCharacter ]; // 저장용 던전 개인 최고 기록 유저 데이터 얻기
    }
}

void KUser::GetDBUpdatePetData( KPetData& kPetData_ )
{
    GetPetCacheData( kPetData_.m_mapPetInfo );
    kPetData_.m_mapCharacterInfo   = m_mapCharacterInfo;
}

void KUser::GetDBDepotUpdateData( OUT KDepotUpdateData& kDepotData_ )
{
    m_kInventory.GetDiffInvenTypeItem( kDepotData_.m_vecInvenTypeItemInv, kDepotData_.m_vecDeleteDepotItem, 
        kDepotData_.m_vecInvenCountItem, kDepotData_.m_vecDeleteDepotCountItem ); // 인벤타입 변경( 인벤토리, 창고 )
}

///@ return 강제 변환이 성공적으로 이루어 졌을 때 true.
bool KUser::ForceTranslateState( int nWantedState_, bool bDestroy_ /*= false*/ )
{
    // 유저 상태의 강제 전환은, 채널 선택창 - 채널 - 게임방 - 경기중 상태에서의 변환만을 허용한다.
    // 종료과정의 상태는 강제변환할 수 없다.
    // 또한 높은 상태(오른쪽)에서 낮은 상태(왼쪽)로의 강제 변환만을 허용한다.
    // 채널로비 <- 채널 <- 게임방 <- 게임중. :: 요렇게.

    // parameter가 유효한 범위 내의 값인지 확인
    if ( nWantedState_ < 1 || nWantedState_ >= KUserFSM::STATE_SENTINEL ) { 
        SET_ERROR( ERR_STATE_00 );
        return false;
    }

    // 상태가 똑같다면 transition 할 필요 없다.
    if ( nWantedState_ == GetStateID() ) { 
        SET_ERROR( NET_OK );
        return true;
    }

    switch( nWantedState_ ) {
        //////////////////////////////////////////////////////////////////////////
    case KUserFSM::STATE_CHANNELLOBBY:
        if ( GetStateID() == KUserFSM::STATE_PLAYING ) { 
            if ( m_pkRoom ) { 
                if( bDestroy_ ) {
                    LeaveGameProcess();
                }
                m_pkRoom->LeaveGame( *this );
            }
            else {
                goto TransitionFailed;
            }
        }

        if ( GetStateID() == KUserFSM::STATE_ROOM /*|| GetStateID() == KUserFSM::STATE_JOIN*/ ) { 
            if( m_pkRoom ) {
                m_pkRoom->Leave( *this, KRoom::LEAVE_SERVER_FIND_ERR ); //leave room
            }
            else { 
                goto TransitionFailed;
            }
        }

        if ( GetStateID() == KUserFSM::STATE_CHANNEL ) { 
            if( m_pkChannel )
                m_pkChannel->Leave( *this );    //leave channel
            else goto TransitionFailed;
        }

        if( GetStateID() == KUserFSM::STATE_SQUARE_JOIN ||
            GetStateID() == KUserFSM::STATE_SQUARE) {
            if ( m_spSquare ) { 
                LIF( LeaveSquare() );
            }
            else { 
                goto TransitionFailed;
            }
        }

        if ( GetStateID() == KUserFSM::STATE_AGIT_JOIN || GetStateID() == KUserFSM::STATE_AGIT ) {
            if ( m_dwStayAgitUID != 0 ) {
                LeaveAgit();
                StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
            }
            else { 
                goto TransitionFailed;
            }
        }

        if( GetStateID() == KUserFSM::STATE_MINIGAME_PLAYING || 
            GetStateID() == KUserFSM::STATE_MINIGAME ) {
            KEVENT_ENTER_SQUARE_ACK kPacket;
            if ( m_spSquare ) { 
                m_spSquare->BackToSquareFromMiniGame( GetThisPtr() , kPacket );
                LIF( LeaveSquare() );
            }
            else goto TransitionFailed;
        }

        if( GetStateID() == KUserFSM::STATE_GUILD_PLAYING || 
            GetStateID() == KUserFSM::STATE_GUILD_ROOM ) {
            // 미구현
        }


        if( GetStateID() == KUserFSM::STATE_CHANNELLOBBY )
            break;
        else goto TransitionFailed;
        break;
        //////////////////////////////////////////////////////////////////////////
    case KUserFSM::STATE_CHANNEL:
        if ( GetStateID() == KUserFSM::STATE_PLAYING ) { 
            if ( m_pkRoom ) { 
                if( bDestroy_ ) LeaveGameProcess();
                m_pkRoom->LeaveGame( *this );
            }
            else goto TransitionFailed;
        }
        if ( GetStateID() == KUserFSM::STATE_ROOM /*|| GetStateID() == KUserFSM::STATE_JOIN*/ ) { 
            if( m_pkRoom )
                m_pkRoom->Leave( *this, KRoom::LEAVE_SERVER_FIND_ERR ); //leave room
            else goto TransitionFailed;
        }
        else goto TransitionFailed;
        break;
        //////////////////////////////////////////////////////////////////////////
    case KUserFSM::STATE_ROOM:  // 050711. 작업일 현재 room으로의 상태 전환은 사용하지 않고 있다.
        if ( GetStateID() == KUserFSM::STATE_PLAYING ) { 
            if ( m_pkRoom != NULL ) { 
                if( bDestroy_ ) LeaveGameProcess();
                m_pkRoom->LeaveGame( *this );
            }
            else goto TransitionFailed;
        }
        else goto TransitionFailed;
    }

    if ( GetStateID() != nWantedState_ ) { 
        SET_ERROR( ERR_STATE_02 );
        return false;
    }

    // 강제 상태 변경 성공.
    SET_ERROR( NET_OK );
    return true;

TransitionFailed:
    START_LOG( cerr, L"강제 상태 변경 실패. Name : " << m_strName )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( GetStateIDString() )
        << BUILD_LOG( m_spFSM->GetStateIDString( nWantedState_ ) )
        << END_LOG;

    SET_ERROR( ERR_STATE_01 );
    return false;
}

void KUser::ProcessPetReward( IN const KGameResultIn& kResultIn_, OUT KGameResultOut& kResultOut_ )
{
    kResultOut_.m_vecPetInfo.clear();

    std::vector< KCharacterExpInfo >::const_iterator vit;
    DWORD dwCharNum = kResultIn_.m_vecCharExp.size();
    for( vit = kResultIn_.m_vecCharExp.begin() ; vit != kResultIn_.m_vecCharExp.end() ; ++vit )
    {
        const KCharacterInfo* pkCharacterInfo = NULL;
        pkCharacterInfo = GetCharacterInfo( vit->m_cCharType );
        if( pkCharacterInfo == NULL ) {
            continue;
        }

        KPetInfo* pkPetInfo = NULL;
        if ( GetPetInfo( pkCharacterInfo->m_kEquipPetInfo.m_dwUID, &pkPetInfo ) ) { 
            // 플레이한 시간(분)을 구함
            int iPlayTimeMin = static_cast<int>( m_pkRoom->GetLatestPlayTime() ) / 60000;
            int iDecrese = SiKPetHelper()->GetDecreseSatiation( iPlayTimeMin );

            // 경험치를 주기 전에 만복도에 따른 패널티 값을 받아온다.
            float fExpPanalty = SiKPetHelper()->GetExpPanalty( pkPetInfo->m_iSatiation );
            float fPromotionPanalty = SiKPetHelper()->GetPromotionPanalty( pkPetInfo->m_cPromotion );

            // 펫 만복도 감소 ( 음수 단위라 그냥 더함 ) // 아래쪽에 영향을 주기 때문에 iDecrese값을 변경한다. 차후 수정.
            if( (pkPetInfo->m_iSatiation + iDecrese) < 0 )
                iDecrese -= (pkPetInfo->m_iSatiation + iDecrese);

            pkPetInfo->m_iSatiation += iDecrese;

            // 도움 아이템중 가장 효과 좋은것을 적용한다.
            float fExpAssist = 1.0f; // 초기 값.
            std::vector<KSimpleItem>::const_iterator vitEquip;
            for( vitEquip = pkPetInfo->m_vecEquipItem.begin(); vitEquip != pkPetInfo->m_vecEquipItem.end(); ++vitEquip )
            {
                if( !SiKPetHelper()->GetPetExpAssistFactor( vitEquip->m_dwID, fExpAssist ) ) // 입력한 기본 팩터보다 큰것을 꺼내옴
                    continue;
            }

            // 최종 Exp Factor 계산
            float fExpFactor = 1.0f;
            fExpFactor = fExpPanalty * fExpAssist * fPromotionPanalty;

            START_LOG( clog, L"펫 최종 경험치 Factor " )
                << BUILD_LOG( fExpFactor )
                << BUILD_LOG( fExpPanalty )
                << BUILD_LOG( fExpAssist )
                << BUILD_LOG( fPromotionPanalty ) << END_LOG;

            // 펫 경험치 주기
            DWORD dwExp = 0;

            //Short 게임 이면 경험치는 주지 않는다.
            if ( SiKGSSimLayer()->CheckTypeFlag( ST_INTERNAL_TESOP ) || false == m_pkRoom->IsShortGame() ) {
                dwExp = static_cast< DWORD >( SiKPetHelper()->GetRewardExpByLv( pkPetInfo->m_iLevel, kResultIn_.m_bWin, fExpFactor, pkPetInfo->m_dwID, pkPetInfo->m_cPromotion, IsPCBangUser() ) );
            }

            pkPetInfo->m_dwEXP += ( dwExp / dwCharNum );

            int iPrevLv = pkPetInfo->m_iLevel;
            // 레벨 셋팅
            pkPetInfo->m_iLevel = SiKPetHelper()->GetPetLevel( pkPetInfo->m_dwEXP );

            START_LOG( clog, L"보상받은 펫의 정보")
                << BUILD_LOG( m_strName )
                << BUILD_LOG( pkPetInfo->m_dwUID )
                << BUILD_LOG( pkPetInfo->m_dwID )
                << BUILD_LOG( pkPetInfo->m_dwEXP )
                << BUILD_LOG( (dwExp / dwCharNum) )
                << BUILD_LOG( pkPetInfo->m_iSatiation )
                << BUILD_LOG( pkPetInfo->m_iLevel ) << END_LOG;

            // 결과로 보낼 펫 정보 셋팅
            kResultOut_.m_vecPetInfo.push_back( std::make_pair( vit->m_cCharType, *pkPetInfo ) );

            // IndoorCharacterInfo Update
            m_pkRoom->UpdatePetInfo( m_strName, pkCharacterInfo->m_cCharType, *pkPetInfo );
        }
    }

}

bool KUser::AdjustExpGP( IN const int& nPlayType_, 
                        IN const KGameInfo& kGameInfo_,
                        IN OUT KEndGameReq& kReq_,
                        OUT KEndGameAck& kAck_ )
{
    _JIF( m_pkRoom, return false );

    bool bRet = true;
    float fChatBoostExp = 0.f;
    float fChatBoostGp = 0.f;
    float fStageClearRatio = 1.f;
    __int64 biMaxExp = 0;
    kAck_.m_vecGameResult.clear();
    float fNationBoostExp = 0.f;
    float fNationBoostGp = 0.f;
    kAck_.m_setHackingUserList.clear();

    GetChatExpGP( kAck_.m_bRewardChatEvent, fChatBoostExp, fChatBoostGp );
    m_pkRoom->UpdateQuickSlotData(); // 퀵 슬롯 장착 정보를 업데이트 한다.
    biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() );
    CalcStageClearRatio( kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty, kReq_.m_setClearStageCount.size(), fStageClearRatio );
    GetNationRewardExpGP( kAck_.m_bNationReward, fNationBoostExp, fNationBoostGp );

    std::vector<KGameResultIn>::iterator vitResultIn;
    for( vitResultIn = kReq_.m_vecGameResult.begin() ; vitResultIn != kReq_.m_vecGameResult.end() ; ++vitResultIn )
    {
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vitResultIn->m_strLogin ) );

        if ( !spUser || !spUser->m_pkRoom ) {
            continue; // 서버에 존재하지 않으면 짤없다...^^... 생각 해보니 그러하네... ㅋㅋ
        } //if( !spUser->m_pkRoom ) continue;  // 080129. kkurrun. 시간 차에 의해 이미 룸을 떠났을수 있음..

        spUser->CheckExpirationItem();

        KPremiumInfo kPremiumInfo;
        spUser->CheckInventoryPremiumItem( kPremiumInfo );
        m_pkRoom->UpdatePremiumData( spUser->m_strName, kPremiumInfo );

        KGameResultOut kResultOut;
        kResultOut.Init();
        kResultOut.m_strLogin = spUser->m_strName;
        kResultOut.m_bWin = vitResultIn->m_bWin;
        kResultOut.m_dwUID = spUser->GetUID();
        kResultOut.m_ucCharType = spUser->GetCurrentCharType();
        spUser->GetUserDifficultInfo( kResultOut.m_mapDifficulty );
        vitResultIn->m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, fChatBoostExp);
        vitResultIn->m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, fChatBoostGp);
        if( kAck_.m_bNationReward ) {
            vitResultIn->m_mapExpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, fNationBoostExp);
            vitResultIn->m_mapGpBoost[KCharacterExpInfoOut::BT_EVENT] += std::max<float>(0.f, fNationBoostGp);
        }

        // Short Game인 경우 진사람의 경우는 패배 처리를 하고, 이긴 경우는 아무 처리도 하지 않겠다. 왜?
        // ShortGameCheck -> true == 이상없다는 뜻, IsShortGame == true -> 쇼트게임 으로 바꾸는게 더 명확합니까?
        if ( ShortGameCheck( spUser, vitResultIn->m_bWin, bRet, kResultOut, kAck_ ) == false ) {
            START_LOG( cerr, L"Short Game.." ) << END_LOG;
            continue;
        }

        // 아래 HackingCheck 는 true 가  해킹없음.
        if ( HackingCheck( *vitResultIn, kReq_, kAck_, spUser, kGameInfo_, kReq_.m_vecGameResult, kResultOut, bRet ) == false ) {
            START_LOG( cerr, L"Hacking Game.." ) << END_LOG;
            continue;
        }

        AdjustBoostInfo( *vitResultIn ); // 경험치/스킬 경험치 정보를 캐릭터 안으로 밀어 넣는다.
        CalcGPBoostInfo( *vitResultIn, fStageClearRatio );

        spUser->ProcessPetReward( *vitResultIn, kResultOut );

        // 캐릭터 경험치를 계산한다.
        CalcCharacterBoostInfo( kGameInfo_.m_iGameMode, spUser, vitResultIn->m_vecCharExp, fStageClearRatio );// 계산 결과를 구한다.
        spUser->AdjustResultInAndOut( kGameInfo_, *vitResultIn, biMaxExp, kResultOut );
        spUser->UpdateGrade(); // 등급 갱신.
        m_pkRoom->UpdateCharacterInfo( spUser->m_strName, spUser->GetGP() ); //GP는 위에서 구해놓았음.

        //  091102  woosh. 용섬 결과 
        UpdateIndigoInfo( *vitResultIn, kGameInfo_.m_iGameMode, vitResultIn->m_bWin, spUser, kResultOut );

        //  091102  woosh. 길드 관련 포인트 수정
        UpdateGuildPoint( *vitResultIn, kGameInfo_.m_iGameMode, spUser, kResultOut );

        m_pkRoom->SetGuildBattlePoint( spUser->GetGuildUID(), spUser->GetGuildBattlePoint() );

        // 자동 매칭 결과 계산 처리.( EndGame, LeaveGame )
        UpdateELOInfo( nPlayType_, *vitResultIn, kGameInfo_.m_iGameMode, vitResultIn->m_bWin, spUser, kResultOut );

        kAck_.m_vecGameResult.push_back( kResultOut );
    }

    std::vector<KGameResultIn>::iterator vitResultInClear;
    std::vector<KGameResultOut>::iterator vitResultOut;
    std::set<DWORD>::iterator sit;
    for( vitResultInClear = kReq_.m_vecGameResult.begin() ; vitResultInClear != kReq_.m_vecGameResult.end() ; ++vitResultInClear ) {
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vitResultInClear->m_strLogin ) );

        if ( !spUser || !spUser->m_pkRoom ) {
            continue;
        }

        vitResultOut = std::find_if( kAck_.m_vecGameResult.begin(), kAck_.m_vecGameResult.end(), 
                    boost::bind( &KGameResultOut::m_dwUID, _1 ) == spUser->m_dwUID );

        if( vitResultOut == kAck_.m_vecGameResult.end() ) {
            continue;
        }
        bool bHackUser = false;
        sit = kAck_.m_setHackingUserList.find( spUser->m_dwUID );
        if( sit != kAck_.m_setHackingUserList.end() ) {
            bHackUser = true;
        }

        if ( false == bHackUser ) {
            // 비해킹유저는 영웅던전, 일반던전 모두 클리어 기록함
            UpdateDifficultyGameMode( *vitResultInClear, kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty, spUser, *vitResultOut );
            UpdateHeroDungeonPlay( kGameInfo_.m_iGameMode, spUser, *vitResultInClear );
        }
        else {
            if ( true == SiKHeroDungeonManager()->IsHeroDungeon( kGameInfo_.m_iGameMode ) ) {
                // 해킹유저는 영웅던전일 경우에만 클리어 기록함
                UpdateDifficultyGameMode( *vitResultInClear, kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty, spUser, *vitResultOut );
                UpdateHeroDungeonPlay( kGameInfo_.m_iGameMode, spUser, *vitResultInClear );
            }
        }
    }

    return bRet;
}

bool KUser::HackingCheck( IN const KGameResultIn& vitResultIn_, IN const KEndGameReq& kReq_, OUT KEndGameAck& kAck_, IN KUserPtr spUser_, IN const KGameInfo& kGameInfo_
                         , IN const std::vector<KGameResultIn>& vecGameResult_, IN KGameResultOut& kResultOut_, OUT bool& bRet_ )
{
    // 해킹체크(운영자 제외)
    if ( IsAdmin() ) {
        return true;
    }

    std::set<int> setHackType; // 해킹체크결과
    int nGameMode = kGameInfo_.m_iGameMode;
    int nDifficult = kGameInfo_.m_nDifficulty;
    int nCharType = -1;
    int nCharJob = -1;

    if ( !vitResultIn_.m_vecCharExp.empty() ) {
        nCharType = (int)vitResultIn_.m_vecCharExp.begin()->m_cCharType;
        nCharJob  = (int)vitResultIn_.m_vecCharExp.begin()->m_cCharJob;
    }

    const KCharacterInfo* pkCharInfo = spUser_->GetCharacterInfo( nCharType );
    _JIF( pkCharInfo != NULL, return false );
    char cLevel = SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

    // 공격력 해킹
    {
        int nType = SiKPreHackingCheckManager()->CheckMaxAtkLimit( cLevel, vitResultIn_.m_kHactkData.m_vecAtkData );
        if ( nType != KPreHackingCheckManager::GHT_NORMAL ) {
            setHackType.insert( nType );
        }
    }

    // 공격력증가율 해킹
    {
        int nType = SiKPreHackingCheckManager()->CheckMaxIncreaseAtkRate( vitResultIn_.m_kHactkData.m_vecIncAPRatio );
        if ( nType != KPreHackingCheckManager::GHT_NORMAL ) { 
            setHackType.insert( nType );
        }
    }

    // 던전모드시
    if ( KRoom::IsDungeonMode( nGameMode ) == true ) { 
        bool bPartyUserCheck = SiKPreHackingCheckManager()->CheckOption( KPreHackingCheckManager::GHT_CLIENT_PARTY_USER, KPreHackingCheckManager::OPT_CHECK_ENABLE ); // 파티원 체크여부
        bool bIsHostUser = ( spUser_ == m_pkRoom->m_spHostUser ? true : false ); // 방장인지 여부

        // 올킬 해킹(방장이 범인)
        if ( SiKPreHackingCheckManager()->CheckOption( KPreHackingCheckManager::GHT_CLIENT_ALL_KILL, KPreHackingCheckManager::OPT_CHECK_ENABLE ) == true ) { 
            if ( vitResultIn_.m_kHactkData.m_bUseAllKill == true ) { 
                if ( bIsHostUser == true ) { 
                    setHackType.insert( KPreHackingCheckManager::GHT_CLIENT_ALL_KILL ); // 방장이 해커
                }
                else if ( bPartyUserCheck == true ) { 
                    setHackType.insert( KPreHackingCheckManager::GHT_CLIENT_PARTY_USER ); // 파티원은 따로 체크
                }
            }
        }

        // 몬스터 HP 해킹(방장이 범인)
        if ( SiKPreHackingCheckManager()->CheckOption( KPreHackingCheckManager::GHT_CLIENT_MON_HP, KPreHackingCheckManager::OPT_CHECK_ENABLE ) == true ) { 
            if ( vitResultIn_.m_kHactkData.m_bMonsterHPHack == true ) { 
                if ( bIsHostUser == true ) { 
                    setHackType.insert( KPreHackingCheckManager::GHT_CLIENT_MON_HP ); // 방장이 해커
                }
                else if ( bPartyUserCheck == true ) { 
                    setHackType.insert( KPreHackingCheckManager::GHT_CLIENT_PARTY_USER ); // 파티원은 따로 체크
                }
            }
        }

        // 클리어시 체크
        if ( vitResultIn_.m_bWin == true ) { 
            // 최소스테이지수 검사
            {
                int nType = SiKPreHackingCheckManager()->CheckMinStageCheck( nGameMode, nDifficult, kReq_.m_setClearStageCount.size() );
                if ( nType != KPreHackingCheckManager::GHT_NORMAL ) { 
                    setHackType.insert( nType );
                }
            }

            // 클리어시간 검사
            {
                int nType = SiKPreHackingCheckManager()->CheckClearTimeCheck( nGameMode, nDifficult, kReq_.m_dwElapsedSec );
                if ( nType != KPreHackingCheckManager::GHT_NORMAL ) { 
                    setHackType.insert( nType );
                }
            }
        }
    }

    LOG_CONDITION( setHackType.empty(), clog, cwarn )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( nCharType )
        << BUILD_LOG( nCharJob )
        << BUILD_LOG( nGameMode )
        << BUILD_LOG( nDifficult )
        << BUILD_LOG( kReq_.m_setClearStageCount.size() )
        << BUILD_LOG( kReq_.m_dwElapsedSec )
        << BUILD_LOG( vitResultIn_.m_kHactkData.m_bUseAllKill )
        << BUILD_LOG( vitResultIn_.m_kHactkData.m_bMonsterHPHack ) << END_LOG;

    if ( setHackType.empty() ) { 
        return true;
    }

    // 여기서부터 해킹유저 처리
    bool bNoticeMsgToClient = false; // 클라이언트 공지여부
    bool bInitialize = false; // 보상 초기화 여부

    for( std::set<int>::const_iterator csit = setHackType.begin() ; csit != setHackType.end() ; ++csit )
    {
        // 파일로그 남김.
        if ( SiKPreHackingCheckManager()->CheckOption( *csit, KPreHackingCheckManager::OPT_WRITE_FILE ) == true ) { 
            std::wstringstream stm;
            stm << KNC_TIME_STRING
                << L"|" << *csit                             // 해킹 분류
                << L"|" << kGameInfo_.m_iGameMode            // 게임모드
                << L"|" << kGameInfo_.m_nDifficulty     // 난이도
                << L"|" << kReq_.m_setClearStageCount.size() // 클리어한 스테이지수
                << L"|" << kReq_.m_dwElapsedSec              // 클리어 타임
                << L"|" << m_pkRoom->GetRoomID()             // 방번호
                << L"|" << vitResultIn_.m_strLogin
                << L"|" << nCharType                         // 캐릭터타입
                << L"|" << nCharJob;                         // 직업

            stm << L"|";
            std::vector<KEquipItemInfo> vecEquipInfo;
            if ( GetEquipItem( nCharType, vecEquipInfo ) == true ) { 
                std::vector<KEquipItemInfo>::iterator vitEquip;
                for( vitEquip = vecEquipInfo.begin() ; vitEquip != vecEquipInfo.end() ; ++vitEquip )
                {
                    // 장착정보
                    stm << vitEquip->m_dwID << L"/"
                        << vitEquip->m_dwUID << L"/"
                        << (int)vitEquip->m_cEnchantLevel << L",";
                }
            }

            stm << L"|";
            for( std::vector<KGameResultIn>::const_iterator cvit = vecGameResult_.begin() ; cvit != vecGameResult_.end() ; ++cvit )
            {
                if( vitResultIn_.m_strLogin == cvit->m_strLogin ) continue;

                KUserPtr spUserTemp( SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( cvit->m_strLogin ) );
                if( !spUserTemp ) continue;
                if( !spUserTemp->m_pkRoom ) continue;

                // 같이 플레이한 파티원
                stm << L" , " << cvit->m_strLogin;
            }

            stm << L"|";
            for( std::vector<float>::const_iterator cvit = vitResultIn_.m_kHactkData.m_vecAtkData.begin() ; cvit != vitResultIn_.m_kHactkData.m_vecAtkData.end() ; ++cvit )
            {
                if ( *cvit > SiKPreHackingCheckManager()->GetMaxAtkLimit( cLevel ) ) { 
                    stm << *cvit << L","; // 능력치 제한 넘은것 표시
                }
            }

            stm << L"|";
            for( std::vector<float>::const_iterator cvit = vitResultIn_.m_kHactkData.m_vecIncAPRatio.begin() ; cvit != vitResultIn_.m_kHactkData.m_vecIncAPRatio.end() ; ++cvit )
            {
                if ( *cvit > SiKPreHackingCheckManager()->GetMaxIncAPRatio() ) { 
                    stm << *cvit << L","; // 공격력증가 비율 제한 넘은것 표시
                }
            }

            TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_LT_DUNGEON_HACK, GetName(), GetUID(), 0, stm.str() );

            stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
            stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
        }

        // DB 기록
        if ( SiKPreHackingCheckManager()->CheckOption( *csit, KPreHackingCheckManager::OPT_WRITE_DB ) == true ) { 
            KDB_EVENT_DUNGEON_HACKING_USER_NOT kNot;
            kNot.m_nGameModeID  = kGameInfo_.m_iGameMode;
            kNot.m_nDifficult   = kGameInfo_.m_nDifficulty;
            kNot.m_nHackingType = *csit;
            kNot.m_nCharType    = nCharType;
            kNot.m_nPromotion   = nCharJob;

            QUEUEING_EVENT_TO_DB( DB_EVENT_DUNGEON_HACKING_USER_NOT, kNot );

            // 누적 해킹횟수 갱신(DB에 기록되는 것만)
            ++spUser_->m_nHackingCount;
        }

        // 클라이언트 알림
        if ( SiKPreHackingCheckManager()->CheckOption( *csit, KPreHackingCheckManager::OPT_NOTICE_MSG ) == true ) { 
            bNoticeMsgToClient = true;
        }

        // 결과 초기화
        if ( SiKPreHackingCheckManager()->CheckOption( *csit, KPreHackingCheckManager::OPT_INITIALIZE ) == true ) { 
            bInitialize = true;
        }
    }

    if ( bNoticeMsgToClient == true ) { 
        // 해당 유저에게 알림.
        KEVENT_DUNGEON_HACKING_USER_NOT KData;
        KData.m_setHackType.swap( setHackType );
        KData.m_nHackCount = spUser_->m_nHackingCount;

        spUser_->SendPacket( KUserEvent::EVENT_DUNGEON_HACKING_USER_NOT, KData );
    }

    if ( bInitialize == true ) { 
        spUser_->GetDefaultResultOut( kResultOut_ );
        kAck_.m_vecGameResult.push_back( kResultOut_ );
        bRet_ = false;
        kAck_.m_setHackingUserList.insert( spUser_->m_dwUID );
        return false;
    }

    return true;

}


// 유저가 보상을 받을 '조건' 이 되는지 검사
bool KUser::CheckConditionDropItem( KUserPtr spUser, const KDropItemDetail& vecDropItem_ , const KDropItemInfo& kDropItemUnit_, const KEndGameReq& kReq_ )
{
    //{{ 061101. kkurrung. 게임 제한이 적용되어 있으면 다음처럼 수정.
    //static boost::mt19937 rng;
    //static boost::uniform_int<DWORD> uint32( 1, UINT_MAX );
    //static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);
    
    // 게임 제한 시스템
    //if ( SiKFatigueSystem()->IsRun() ) { 

        //int fPaneltyRatio = SiKFatigueSystem()->GetRewardRatio( spUser->m_nAccOnlineTime + spUser->GetElapsedTimeAfterAuth(), spUser->m_strName );

        //if (  die() / (float)(UINT_MAX) >= fPaneltyRatio ) { 
        //    START_LOG( clog, L"게임 제한으로 인해서 보상이 지급되지 않음: " << spUser->GetName() )
        //        << BUILD_LOG( fPaneltyRatio ) << END_LOG;
        //    return false;
        //}
    //}
    //}} 061101. kkurrung. 

    std::vector<KGameResultIn>::const_iterator gameResultIter;

    // spUser 가 결과 패킷안에 있는지 찾는 부분이군요. 빨리 이해되지 않는 코드였음;
    gameResultIter = kReq_.m_vecGameResult.begin();
    while( gameResultIter != kReq_.m_vecGameResult.end() )
    {
        if ( spUser->GetName() == gameResultIter->m_strLogin ) { 
            break;
        }
        gameResultIter++;
    }
    
    if ( gameResultIter == kReq_.m_vecGameResult.end() ) { 
        START_LOG( cerr, L"사용자 Login이 EndGameReq 패킷 안에 없음." )
            << BUILD_LOG( spUser->GetName() )
            << END_LOG;

        return false;
    }

    _JIF( m_pkRoom, return false );

    if ( SiKFatigueSystem()->IsRun() ) { 
        if ( m_pkRoom->GetFatigueRatio( spUser->GetUID() ) < 100 ) {
            return false;
        }
    }

    if( !m_pkRoom->IsUserJoined( spUser->GetName() ) ) //게임에 참여하지 않으면 안준다.
        return false;

    if ( vecDropItem_.m_iDropReceiver == KDropItemDetail::DR_HOST ) { 
        if ( spUser != m_pkRoom->m_spHostUser ) { 
            return false;
        }
    }

    if ( vecDropItem_.m_iDropReceiver == KDropItemDetail::DR_GCCLUB ) { 
        if ( spUser->IsPremiumUser() == false ) { 
            return false;
        }
    }

    if ( vecDropItem_.m_iDropReceiver == KDropItemDetail::DR_NOT_GCCLUB ) { 
        if ( spUser->IsPremiumUser() == true ) { 
            return false;
        }
    }

    // 특정 아이템을 가지고 있을 때 드랍 받음.
    if ( vecDropItem_.m_iConditionItem > 0 ) { 
        if ( !spUser->CheckIfItemUsed( vecDropItem_.m_iConditionItem ) ) { 
            return false;
        }
    }

    switch( vecDropItem_.m_iDropType )
    {
    case KDropItemDetail::DT_NORMAL:
        if ( gameResultIter->m_bWin ) {  // 이기면 준다.
            std::vector<DWORD> vecTitleList;
            if ( SiKCollectionManager()->IsTitleItem( kDropItemUnit_.m_ItemID ) ) {  // 칭호 아이템이면 참
                if ( spUser->m_kInventory.FindItemByItemID( kDropItemUnit_.m_ItemID ) == NULL ) {  // 칭호 아이템이 내 인벤토리에 없다면 드랍한다
                    return true;
                    // 넣어주셈
                }
                else {// 내가 이미 칭호 아이템을 가지고 있다면  드랍하지 않도록 한다.
                    START_LOG( cwarn, L"이미 있는 칭호를 드랍받으려고 한다. 안준다.")
                        << BUILD_LOG( m_strName )
                        << BUILD_LOG( kDropItemUnit_.m_ItemID ) << END_LOG;
                    return false;
                }
            } // 인벤토리에 칭호 아이템이 없어야 드랍해줄 수 있다.
                                    
            return true;
        }
        else {
            return false;
        }


    case KDropItemDetail::DT_ADMIN_EVENT:
        if( spUser->CheckIfAllItemExist( kDropItemUnit_.m_ItemID, -1 ) )
            return false;
        else
            return true;

    case KDropItemDetail::DT_ALLWAYS: // 이기든 지든 무조건 주는(게임에 참여한 사람은)
        return true;

    case KDropItemDetail::DT_LOSE: // 패배시 지급
        if ( false == gameResultIter->m_bWin ) {
            return true;
        }
        return false;

    default:
        return false;
    }
}

bool KUser::CheckConditionItemDropByLevel( KUserPtr spUser, const KDropItemDetail& vecDropItem_ )
{
    int iLevel = 0;

    const KCharacterInfo* pkCharacterInfo = spUser->GetCurrentCharacter();
    if( !pkCharacterInfo )
        return false;

    iLevel = SiKResultManager()->Exp_2_Level( pkCharacterInfo->m_biExp );
    if ( vecDropItem_.m_iBeginLv == -1 ) { 
        return true;
    }
    else if ( vecDropItem_.m_iBeginLv <= iLevel && vecDropItem_.m_iEndLv >= iLevel ) { 
        START_LOG( clog, L"레벨별 드랍 기능 작동")
            << BUILD_LOG( spUser->GetName() )
            << BUILD_LOG( iLevel )
            << BUILD_LOG( vecDropItem_.m_iBeginLv )
            << BUILD_LOG( vecDropItem_.m_iEndLv )
            << END_LOG;
        return true;
    }

    return false;
}

bool KUser::DivideEachShareOfDropItem( IN const KEndGameReq& kReq_,
                                      IN OUT KInsertDropItem& kInsertDropItem_,
                                      IN OUT bool& bShDrop_,
                                      OUT bool& bResultDrop_ )
{
    _JIF( m_pkChannel, return false );
    _JIF( m_pkRoom, return false );

    std::vector<KDropItemDetail> vecDropItemDetail;
    KDropMode kDropMode;

    bool bDropMode = m_pkRoom->GetDropMode( kReq_.m_iQuestEndingStage, 0, kDropMode );
    bool bDrop = m_pkRoom->DropItem( kReq_.m_iQuestEndingStage, 0 , vecDropItemDetail ); // 채널 그레이드 확인
    int nPartyNum = kReq_.m_vecGameResult.size(); // 파티원수 Keep.

    std::vector<KGameResultIn>::const_iterator vit;
    for( vit = kReq_.m_vecGameResult.begin(); vit != kReq_.m_vecGameResult.end(); vit++ )
    {
        _JIF( !vit->m_vecCharExp.empty(), return false );

        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin ) );

        if ( !spUser ) { 
            continue;
        }

        const KInDoorUserInfo* pkStartingInfo = m_pkRoom->FindInDoorUserInfo( vit->m_strLogin );

        if( !pkStartingInfo ) continue;

        if ( bDropMode && kDropMode.m_iDropMethod == KDropMode::DM_PER_USER ) { 
            if( m_pkRoom->DropItem( kReq_.m_iQuestEndingStage, 0, vecDropItemDetail ) )
                bDrop = true;

        }

        int iJob = -1;
        iJob = pkStartingInfo->m_cCharIndex + (GC_CHAR_NUM * pkStartingInfo->m_cPromotion);

        if( m_pkRoom->m_iGameMode == GC_GM_INDIGO_TEAM ||
            m_pkRoom->m_iGameMode == GC_GM_TAG_SURVIVAL ||
            m_pkRoom->m_iGameMode == GC_GM_TAG_TEAM )
        {
            const KCharacterInfo* pkCharacterInfo =  GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
            if ( !vit->m_vecCharExp.empty() && pkCharacterInfo != NULL ) { 
                iJob = pkCharacterInfo->m_cCharType + (GC_CHAR_NUM * pkCharacterInfo->m_cCurrentPromotion );
            }
        }

        std::vector<KDropItemDetail>::const_iterator dropItemIter;
        std::map<int, int>::const_iterator itemKindIter;
        DROP_ITEMS vecItem;

        //{{ 060609. kkurrung.
        KEVENT_SHANGHAI_DROP_REQ shReq;
        std::vector<KGameResultOut>::iterator vitResultOut;
        vitResultOut = std::find_if( kInsertDropItem_.m_kEndGameAck.m_vecGameResult.begin(), kInsertDropItem_.m_kEndGameAck.m_vecGameResult.end(),
                boost::bind( &KGameResultOut::m_strLogin, _1 ) == vit->m_strLogin );

        if ( vitResultOut != kInsertDropItem_.m_kEndGameAck.m_vecGameResult.end() ) { 
            std::vector< std::pair<char,KCharacterExpInfoOut> >::const_iterator vitExp;
            for( vitExp = vitResultOut->m_vecExps.begin() ; vitExp != vitResultOut->m_vecExps.end() ; ++ vitExp )
            {
                if ( spUser->IsCharExist( vitExp->first ) ) {

                    KSHCheckPoint::SHChecData shData;
                    shData.m_cCurrentCharID     = vitExp->first;
                    shData.m_cOldLevel          = SiKResultManager()->Exp_2_Level( vitExp->second.m_biOldExp );
                    shData.m_cLevel             = SiKResultManager()->Exp_2_Level( vitExp->second.m_biExp );
                    shData.m_strLogin           = spUser->m_strName;

                    START_LOG( clog, L"SHANGHAI_DROPCHECK" )
                        << BUILD_LOGc( shData.m_cCurrentCharID )
                        << BUILD_LOGc( shData.m_cOldLevel )
                        << BUILD_LOGc( shData.m_cLevel ) << END_LOG;

                    if( m_tmFirstLoginTime > SiKGSSimLayer()->GetSHAdhustTime() && 
                        SiKSHCheckPoint()->DropCheck( vecItem, shReq.m_vecDropPostItems, shReq.m_vecAutoMission, shData, KUser::USER_CT_TOTAL ) )
                    {
                        START_LOG( clog, L"SHANGHAI_DROP Item : " << vecItem.size() << L", Mission : " << shReq.m_vecAutoMission.size()  ) << END_LOG;
                        bDrop = true;
                        
                        if ( !vecItem.empty() )
                            bShDrop_ = true;
                    }

                    if( spUser->IsPCBangUser() )
                    {
                        int nCount = SiKIPAdvantage()->AddLevelUpReward( shData.m_cOldLevel, shData.m_cLevel, vecItem );
                        START_LOG( clog, L"PC방 유저가 레벨업 해서 아이템 받음.. Name : " << spUser->GetName() <<  L", Count : " << nCount ) << END_LOG;
                        if( nCount > 0 ) bDrop = true;
                    }
                    // 반갑다 친구야 레벨업시 아이템 지급 여부 체크.
                    if( ( spUser->IsNewRecomUser() || spUser->IsReturnRecomUser() ) && m_MyRecomInfo.m_nState >= KRecomHelper::RBS_ACCEPT_RECOM ) {
                        std::pair<int,int> prRecomInfo = spUser->GetNewRecomUserInfo();
                        int nCount = spUser->RecomLevelUpCheck( shData.m_cOldLevel, shData.m_cLevel, prRecomInfo.second, vecItem );
                        START_LOG( clog, L"반갑다 친구야 신규/복귀 유저가 레벨업 해서 아이템 받음.. Name : " << spUser->GetName() <<  L", Count : " << nCount )
                            << BUILD_LOGc( shData.m_cOldLevel )
                            << BUILD_LOGc( shData.m_cLevel )
                            << BUILD_LOG( prRecomInfo.first )
                            << BUILD_LOG( prRecomInfo.second ) << END_LOG;
                        if( nCount > 0 ) bDrop = true;
                    }

                    DWORD dwChannelType = 0;
                    dwChannelType = spUser->GetUserChannelType();
                    // 유저 채널 타입에 따른 레벨업 보상 이벤트 있는지 체크.
                    if( SiKSHCheckPoint()->DropCheck( vecItem, shReq.m_vecDropPostItems, shReq.m_vecAutoMission, shData, dwChannelType ) )  {
                        bool bRet = true;
                        START_LOG( clog, L" 채널링 유저 레벨업 해서 아이템 지급 성공." )
                            << BUILD_LOGc( shData.m_cOldLevel )
                            << BUILD_LOGc( shData.m_cLevel ) << END_LOG;
                        if ( bRet ) { bDrop = true; }
                    }
                    
					//  레벨 초기화 이벤트 달성 여부 체크
                    if ( SiKLevelResetEvent()->IsRun() && shData.m_cOldLevel < shData.m_cLevel )
                    {

                        std::vector<KDropItemInfo> vecReward;
                        bool bCashType = false;

                        bool bReward = spUser->CheckRewardLevel( shData.m_cCurrentCharID, shData.m_cLevel, bCashType, vecReward );

                        if( bReward ) 
                        {
                            KDB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ kDBPacket;
                            kDBPacket.m_dwVersion = SiKLevelResetEvent()->GetVersion();
                            kDBPacket.m_cCharType = shData.m_cCurrentCharID;
                            kDBPacket.m_vecItemInfo.swap( vecReward );

                            //  보상 지급 요청 및 데이터 셋팅    
                            _QUEUEING_EVENT_TO_DB( DB_EVENT_LEVEL_RESET_EVENT_REWARD_REQ, kDBPacket, spUser );
                        }                        
                	}
					
					
                    if( ( shData.m_cOldLevel < shData.m_cLevel ) && SiKGCHelper()->IsCharLevelEvent() ) { // 레벨업을 했을 경우에..,
                        KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;
                        if( SiKGCHelper()->GetCharLevelRewardItem( vitExp->first, shData.m_cLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                            kDBPacket.m_setCharInfo.insert( vitExp->first );
                            _QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket, spUser );
                        }
                    }

                    // 최종 보상 아이템 넣기.
                    int nRewardType = 0;
                    if( false == SiKDungeonManager()->GetDungeonRewardType( m_pkRoom->m_iGameMode, nRewardType ) ) {
                        continue;
                    }

                    if ( nRewardType == KEndGameAck::RESULT_REWARD && true == vit->m_bWin ) {
                        KDropItemInfo kDefaultItem;
                         if( !SiKResultReward()->GetDefaultRewardInfo( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, vitExp->first, shData.m_cLevel, kDefaultItem ) ) {
                            START_LOG( cerr, L"결과 보상을 정상적으로 받지 못함 " )
                                << BUILD_LOG( vit->m_strLogin )
                                << BUILD_LOG( vit->m_dwUID )
                                << BUILD_LOGc( vitExp->first )
                                << END_LOG;
                         } else {
                             vitResultOut->m_mapResultReward[KGameResultOut::DEFAULT_REWARD] = kDefaultItem;
                             bDrop = true;
                             bResultDrop_ = true;
                         }
                         KDropItemInfo kAddItem;
                         if( SiKResultReward()->GetAddRewardInfo( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, vitExp->first,  shData.m_cLevel, spUser->IsPCBangUser(), m_pkRoom->GetLatestPlayTime(), nPartyNum, kAddItem ) ) 
                         {
                             vitResultOut->m_mapResultReward[KGameResultOut::ADDITION_REWARD] = kAddItem;
                             bDrop = true;
                             bResultDrop_ = true;
                         }
                    }

                    if ( nRewardType == KEndGameAck::STAGE_REWARD && true == vit->m_bWin ) 
                    {
                        std::vector<KDropItemInfo> kDefaultItemVector;
                        kDefaultItemVector.clear();
                        if ( !SiKStageReward()->GetStageRewardInfo( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, m_pkRoom->m_iStageClearcount - 2, kDefaultItemVector ) )
                        {
                            START_LOG( cerr, L"스테이지 보상 불러오기 오류 " )
                                << BUILD_LOG( vit->m_strLogin )
                                << BUILD_LOGc( m_pkRoom->m_iStageClearcount - 2 )
                                << END_LOG;
                        }
                        else
                        {
                            if ( !kDefaultItemVector.empty() )
                            {
                                vitResultOut->m_vecStageReward.swap( kDefaultItemVector );
                                bDrop = true;
                                bResultDrop_ = true;
                            }
                            else
                            {
                                START_LOG( cerr, L"스테이지 보상 불러오기 오류 " )
                                    << BUILD_LOG( vit->m_strLogin )
                                    << BUILD_LOGc( m_pkRoom->m_iStageClearcount - 2 )
                                    << END_LOG;
                            }
                        }
                    }
                }
            }
        }

        shReq.m_cCharType = spUser->GetCurrentCharType();
        spUser->CheckLevelUpDrop( shReq );

        for( dropItemIter = vecDropItemDetail.begin(); dropItemIter != vecDropItemDetail.end(); dropItemIter++ )
        {
            itemKindIter = dropItemIter->m_mapItemList.find( iJob );
            // 051026. jseop. 해당 직업에 대한 획득 아이템이 존재해야하고, Item ID 가 0보다 커야 한다.
            // 051219. florist. 특정 직업에 아이템이 안 떨어지길 원하면 id를 0으로 처리한다.
            if ( itemKindIter != dropItemIter->m_mapItemList.end() && itemKindIter->second > 0 ) { 
                KDropItemInfo kItemUnit;
                kItemUnit.m_ItemID = (GCITEMID)itemKindIter->second;
                kItemUnit.m_nDuration = dropItemIter->m_iNum;
                kItemUnit.m_nPeriod = dropItemIter->m_iPeriod;

                if ( !CheckConditionDropItem( spUser, *dropItemIter, kItemUnit, kReq_ ) ) { 
                    continue;
                }

                if ( !CheckConditionItemDropByLevel( spUser, *dropItemIter ) ) { 
                    continue;
                }

                AdditionalDropProcess( spUser, kItemUnit, kInsertDropItem_.m_kEndGameAck );

                vecItem.push_back( kItemUnit );
            }
            else
            {
                START_LOG( clog, L"해당 직업에 대한 획득 아이템이 없음." )
                    << BUILD_LOG( iJob )
                    << BUILD_LOG( itemKindIter->second )
                    << BUILD_LOG( dropItemIter->m_mapItemList.size() )
                    << END_LOG;

                continue;
            }
        }

        if ( !vecItem.empty() || bResultDrop_ ) { 
            kInsertDropItem_.m_mapEachUserDrops[vit->m_dwUID] = vecItem;
        }
    }
    return bDrop;
}

void KUser::AdditionalDropProcess( KUserPtr spUser, OUT KDropItemInfo& kDropItemUnit, KEndGameAck& kAck )
{
    JIF( spUser.get() );

    std::vector<KGameResultOut>::iterator gameResultIter;

    gameResultIter = std::find_if( kAck.m_vecGameResult.begin(), kAck.m_vecGameResult.end(),
        boost::bind( &KGameResultOut::m_strLogin, _1 ) == spUser->GetName() );

    if ( gameResultIter == kAck.m_vecGameResult.end() ) { 
        START_LOG( cerr, L"사용자 Login이 EndGameAck 패킷 안에 없음." )
            << BUILD_LOG( spUser->GetName() )
            << END_LOG;

        return;
    }
}

void KUser::SetAdult( IN bool bAdult_ )
{
    m_bAdult = bAdult_;
}

bool KUser::PCBangDungeonItemDrop( IN const KEndGameReq& kReq_,
                                  IN OUT KInsertDropItem& kInsertDropItem_ )
{
    _JIF( m_pkChannel, return false );
    _JIF( m_pkRoom, return false );
    bool bDrop = false;

    std::vector<KGameResultIn>::const_iterator vit;
    for( vit = kReq_.m_vecGameResult.begin(); vit != kReq_.m_vecGameResult.end(); vit++ )
    {
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID ) );

        if ( !spUser ) { 
            continue;
        }

        DROP_ITEMS vecItem;

        if( spUser->IsPCBangUser() && vit->m_bWin && m_pkRoom->IsDungeonMode( m_pkRoom->m_iGameMode ) )
        {
            int nCount = SiKIPAdvantage()->AddDungeonClearReward( m_pkRoom->m_iGameMode, vecItem );
            START_LOG( clog, L"PC방 유저가 던전 클리어 해서 아이템 받음.. Name : " << spUser->GetName() <<  L", Count : " << nCount ) << END_LOG;
        }


        if ( !vecItem.empty() ) { 
            kInsertDropItem_.m_mapEachUserDrops[vit->m_dwUID] = vecItem;
            bDrop = true;
        }
    }

    return bDrop;
}

void KUser::UpdateCYOUHeartbeat()
{
    if ( m_kTimerCYOUHeartbeat.CheckTime() ) {
        KCYOUAuthUserInfo kPacket;
        //kPacket.m_strServerIP = strServerIP_;

        kPacket.m_strLogin = m_strBillingAccount; // KncUtil::toNarrowString( GetName() );
        kPacket.m_dwUID = GetUID();
        KCharacterInfo* pkCharacterInfo = GetCurrentCharacter();
        if ( pkCharacterInfo == NULL ) { return; }

        kPacket.m_usCharacterLevel = static_cast<USHORT>( pkCharacterInfo->m_dwLevel );

        LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_CYOU_USER_HEART_BEAT_REQ, kPacket ) );
    }
}

void KUser::CYOULogout( IN const char cCharType_ )
{
    if ( false == SiKGSSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_CYOU ) ) {
        return;
    }

    if ( m_bCYOULogined == false ) {
        START_LOG( clog, L"창유 A3 로그인 되지 않은 상태라서 A4 로그아웃 보내지 않음. Name : " << GetName() )
            << END_LOG;
        return;
    }
    KCYOUAuthUserInfo kPacket;
    kPacket.m_strLogin = m_strBillingAccount;
    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( cCharType_ );
    if ( pkCharacterInfo == NULL ) { return; }

    kPacket.m_usCharacterLevel = static_cast<USHORT>( pkCharacterInfo->m_dwLevel );
    if ( ( GetDisconnReason() == DRW_DUPLICATE_CONNECTION ) || ( GetDisconnReason() == DRW_DUPLICATE_CONNECTION_CYOU ) ) {
        kPacket.m_cLogoutType = 0x03;
    }
    else {
        kPacket.m_cLogoutType = 0x01;
    }

    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_CYOU_USER_LOGOUT, kPacket ) );
}

void KUser::CYOULogin( IN const char cCharType_ )
{
    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( cCharType_ );
    if ( pkCharacterInfo == NULL ) { return; }

    KCYOUAuthUserInfo kData;
    kData.m_dwUID = GetUID();
    kData.m_strLogin = m_strBillingAccount;
    kData.m_Character = GetCurrentCharType();
    kData.m_usCharacterLevel = static_cast<USHORT>(pkCharacterInfo->m_dwLevel);

    std::wstring wstrNick(L"");
    GetCharNickNameInfo(kData.m_Character, wstrNick ); 
    kData.m_strCharNickname = KncUtil::toNarrowString( wstrNick );
    
    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_CYOU_USER_LOGIN_REQ, kData ) ); // 센터 서버에 등록 한다.
}

void KUser::InitFatigue()
{
    if( SiKFatigueSystem()->IsRun() )
    {
        QUEUEING_ID_TO_DB( EVENT_ACCUMULRATION_TIME_NOT );
    }
}

void KUser::CYOUGetCurrentPoint( IN const int& nServerID_ )
{
    KCYOUCashData kPacket;
    kPacket.m_strLogin = m_strBillingAccount;//KncUtil::toNarrowString( GetName() );
    kPacket.m_strUserIP = GetIPStr();

    std::wstring wstrNick(L"");
    GetCharNickNameInfo(GetCurrentCharType(), wstrNick ); 
    kPacket.m_strCharNickname = KncUtil::toNarrowString( wstrNick );
    
    kPacket.m_usServerID = nServerID_;
    kPacket.m_dwUserUID = GetUID();

    LIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_CYOU_GET_CURRENT_CASH_REQ, kPacket ) );
}

void KUser::CYOUSendBuyLog( IN KEVENT_BUY_FOR_CASH_ACK& kPacket_ )
{
    KCYOUCashData kPacket;
    kPacket.m_strLogin = m_strBillingAccount;//KncUtil::toNarrowString( GetName() );
    kPacket.m_strUserIP = GetIPStr();
    
    std::wstring wstrNick(L"");
    GetCharNickNameInfo(GetCurrentCharType(), wstrNick ); 
    kPacket.m_strCharNickname = KncUtil::toNarrowString( wstrNick );

    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( GetCurrentCharType() );
    if ( pkCharacterInfo == NULL ) { return; }
    kPacket.m_usCharacterLevel = static_cast<USHORT>( pkCharacterInfo->m_dwLevel );

    kPacket.m_dwUserUID = GetUID();
    kPacket.m_nCashPoint = kPacket_.m_cashInfo.m_dwDecreaseCashPoint;
    
    DWORD dwBuyCount = 1; // 구매 횟수이다.  아이템의 수량이 아님. 현재는 아이템 구매 1회에 한개씩만 살 수 있기 때문에 1이라고 보낸다. 
    kPacket.m_strItemInfo = boost::lexical_cast<std::string>( kPacket_.m_kBuyInfo.m_ItemID ) + ","
        + boost::lexical_cast<std::string>( dwBuyCount ) + ","
        + boost::lexical_cast<std::string>( kPacket_.m_kBuyInfo.m_nPrice );

    SiKCnConnector()->SendPacket( KCenterEvent::ECN_CYOU_BUY_LOG_REQ, kPacket );
}

void KUser::ApplyExpToCharacter( IN const int nCharType_, IN const int nAddExp_ )
{
    KCharacterInfo* pkCharInfo = GetCharacterInfo( nCharType_ );
    if ( NULL == pkCharInfo ) {
        START_LOG( cerr, L"서버에 유저 캐릭터 정보가 없음. LoginID: " << GetName() ) << END_LOG;
        return;
    }

    // 이미 만렙이면 처리 안함
    if ( pkCharInfo->m_dwLevel >= SiKResultManager()->GetMaxLevel() ) {
        return;
    }

    __int64 biMaxExp = SiKResultManager()->GetExpByLv( SiKResultManager()->GetMaxLevel() );

    pkCharInfo->m_biExp += static_cast< __int64 >( nAddExp_ );
    pkCharInfo->m_biExp = std::min< __int64 >( pkCharInfo->m_biExp, biMaxExp );
    pkCharInfo->m_biExp = std::max< __int64 >( pkCharInfo->m_biExp, USER_MIN_EXP );

    pkCharInfo->m_biTotalExp += static_cast< __int64 >( nAddExp_ );
    pkCharInfo->m_biTotalExp = std::min< __int64 >( pkCharInfo->m_biTotalExp, _I64_MAX );
    pkCharInfo->m_biTotalExp = std::max< __int64 >( pkCharInfo->m_biTotalExp, USER_MIN_EXP );

    // 레벨 계산
    DWORD dwOldLevel = pkCharInfo->m_dwLevel;
    pkCharInfo->m_dwLevel = SiKResultManager()->Exp_2_Level( pkCharInfo->m_biExp );

    // 캐릭터의 Levelup을 달성했을 경우에 체크하자
    if ( dwOldLevel < pkCharInfo->m_dwLevel ) {
        // SPX 계산
        SiKSkillManager()->GetSkillPointInfo( nCharType_, pkCharInfo->m_cPromotion, pkCharInfo->m_dwLevel, pkCharInfo->m_kSkillInfo.m_nLvSPPoint );

        // 렙업 드랍 체크(상하이)
        char cCharType = static_cast< char >( nCharType_ );
        LevelUpDrop( GetThisPtr(), cCharType, static_cast< char >( dwOldLevel ), static_cast< char >( pkCharInfo->m_dwLevel ) );
        KEVENT_SHANGHAI_DROP_REQ kDBPacket;
        kDBPacket.m_cCharType = pkCharInfo->m_cCharType;
        CheckLevelUpDrop( kDBPacket );

        // 전직 미션 가이드 정보 체크
        CheckJobMissionGuide( GetUID(), pkCharInfo->m_cCharType, static_cast< int >( pkCharInfo->m_dwLevel ) );

        // 봉인 캐릭터 정보 갱신
        UpdateUserSealedCharInfo( GetThisPtr(), pkCharInfo->m_cCharType, static_cast< int >( pkCharInfo->m_dwLevel ) );

        // 유저 레벨업 정보 저장(룸에 있는 경우에만)
        if ( NULL != m_pkRoom ) {
            std::map< DWORD, std::pair< int, DWORD > > mapLevelUpUser; // 유저의 레벨업 정보 [UserUID, <CharType, Level>]
            mapLevelUpUser.clear();
            m_pkRoom->GetLevelUpUser( mapLevelUpUser );

            SetUserLevelUpInfo( GetUID(), nCharType_, pkCharInfo->m_dwLevel, mapLevelUpUser );

            // 유저의 레벨업 정보 갱신
            m_pkRoom->SetLevelUpUser( mapLevelUpUser );
        }

        // 통계용
        UpdateCharLvPlayTime( nCharType_, dwOldLevel );

        // 레벨업 이벤트
        if ( SiKGCHelper()->IsCharLevelEvent() ) {
            KDB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ kDBPacket;

            if( SiKGCHelper()->GetCharLevelRewardItem( pkCharInfo->m_cCharType, pkCharInfo->m_dwLevel, m_setCharLevelEvent, kDBPacket.m_vecRewardInfo ) ) {
                kDBPacket.m_setCharInfo.insert( pkCharInfo->m_cCharType );
                _QUEUEING_EVENT_TO_DB( DB_EVENT_CHAR_LEVEL_REWARD_EVENT_REQ, kDBPacket, GetThisPtr() );
            }
        }
    }
}

void KUser::CheckCharacterGraduate( IN const int nSendType_ )
{
    KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT kPacket;
    KDB_EVENT_GRADUATE_CHARACTER_USER_INFO_REQ kDBPacket;

    bool bResult = false;
    switch ( nSendType_ ) {
        case KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT::EGCUI_ON_CONNECT:
            kPacket.m_nSendType = nSendType_;

            bResult = CheckCharacterGraduate_OnConnect( kPacket );

            SEND_PACKET( EVENT_GRADUATE_CHARACTER_USER_INFO_NOT );

            break;
        case KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT::EGCUI_ON_GRADUATE:
            bResult = CheckCharacterGraduate_OnDungeonClear( kDBPacket );

            // 졸업한 경우 DB처리를 한다(클리어 모드 기록, 보상 아이템 지급)
            if ( true == bResult ) {
                QUEUEING_EVENT_TO_DB( DB_EVENT_GRADUATE_CHARACTER_USER_INFO_REQ, kDBPacket );
            }

            break;
        default:
            START_LOG( cerr, L"정의되지 않은 전송타입!") << END_LOG;
            break;
    }

    START_LOG( clog, L"캐릭터 졸업 확인. nSendType: " << nSendType_ << ", bResult: " << bResult ) << END_LOG;
}

bool KUser::CheckCharacterGraduate_OnConnect( OUT KEVENT_GRADUATE_CHARACTER_USER_INFO_NOT& kPacket_ )
{
    // 접속시에는 졸업하지 못한 캐릭터 정보만 계산하여 항상 전달한다
    GetNonGraduateCharacters( kPacket_.m_setNonGraduateCharacters );

    m_setNonGraduateCharacters = kPacket_.m_setNonGraduateCharacters;

    return true;
}

bool KUser::GetNonGraduateCharacters( OUT std::set< int >& setNonGraduateCharacters_ )
{
    setNonGraduateCharacters_.clear();

    std::map< int, KCharacterGraduation::KGraduation > mapGraduation;
    SiKCharacterGraduation()->GetGraduation( mapGraduation );

    typedef std::map< int, KCharacterGraduation::KGraduation > mapGraduationType;
    BOOST_FOREACH( mapGraduationType::value_type& rGraduation, mapGraduation ) {
        setNonGraduateCharacters_.insert( rGraduation.first );

        if ( true == CheckGraduationCondition( rGraduation.second.m_mapConditionModes ) ) {
            setNonGraduateCharacters_.erase( rGraduation.first );
        }
    }

    if ( true == setNonGraduateCharacters_.empty() ) {
        return false;
    }

    return true;
}

bool KUser::CheckGraduationCondition( IN const std::map< int, std::set< int > >& mapConditionModes_ )
{
    bool bIsGraduate = true;

    typedef std::map< int, std::set< int > > mapConditionModesType;
    BOOST_FOREACH( const mapConditionModesType::value_type& rConditionMode, mapConditionModes_ ) {
        std::map< int, KDungeonUserInfo >::iterator mitDifficulty = m_mapDifficulty[GetCurrentCharType()].find( rConditionMode.first );
        if ( mitDifficulty == m_mapDifficulty[GetCurrentCharType()].end() ) {
            bIsGraduate = false;

            break;
        }

        BOOST_FOREACH( const std::set< int >::value_type& rConditionDifficulties, rConditionMode.second ) {
            if ( false == mitDifficulty->second.m_kModeDifficulty.Get( rConditionDifficulties ) ) {
                bIsGraduate = false;

                break;
            }
        }

        if ( false == bIsGraduate ) {
            break;
        }
    }

    return bIsGraduate;
}

bool KUser::CheckCharacterGraduate_OnDungeonClear( OUT KDB_EVENT_GRADUATE_CHARACTER_USER_INFO_REQ& kDBPacket_ )
{
    bool bIsGraduate = false;

    std::set< int > setGraduateCharacters;
    setGraduateCharacters.clear();
    std::set< int > setClearModes;
    setClearModes.clear();
    std::vector< KItem > vecRewards;
    vecRewards.clear();
    std::map< int, KDungeonUserInfo > mapDifficulty;
    mapDifficulty = m_mapDifficulty[GetCurrentCharType()];

    std::set< int > setNonGraduateCharacters;
    GetNonGraduateCharacters( setNonGraduateCharacters );

    // 미졸업한 캐릭터들의 리스트에 변동이 있을 경우에만 처리한다
    if ( m_setNonGraduateCharacters != setNonGraduateCharacters ) {
        // 이번에 졸업한 캐릭터들을 계산한다
        /* 기존 리스트에서 빠진 캐릭터 번호가 졸업한 캐릭터이지만
           혹시모를 스크립트 변경에 대처하기 위해
           { 기존 미졸업 캐릭터 집합 } - { 새로 계산한 미졸업 캐릭터 집합 } 이 아니라
           diff( { 기존 미졸업 캐릭터 집합 }, { 새로 계산한 미졸업 캐릭터 집합 } ) 으로 계산한다 */
        std::set_difference( m_setNonGraduateCharacters.begin(), m_setNonGraduateCharacters.end(),
            setNonGraduateCharacters.begin(), setNonGraduateCharacters.end(),
            std::inserter( setGraduateCharacters, setGraduateCharacters.begin() ) );

        std::map< int, KCharacterGraduation::KGraduation > mapGraduation;
        SiKCharacterGraduation()->GetGraduation( mapGraduation );

        // 이번에 졸업한 캐릭터들의 자동 클리어 해줘야 할 모드와 보상 정보를 가져온다
        BOOST_FOREACH( std::set< int >::value_type rCharType, setGraduateCharacters ) {
            std::map< int, KCharacterGraduation::KGraduation >::iterator mitGraduation;
            mitGraduation = mapGraduation.find( rCharType );

            setClearModes.insert( mitGraduation->second.m_setClearModes.begin(), mitGraduation->second.m_setClearModes.end() );
            vecRewards.insert( vecRewards.end(), mitGraduation->second.m_vecRewards.begin(), mitGraduation->second.m_vecRewards.end() );
        }

        // 자동 클리어 모드를 유저 객체에 세팅한다
        BOOST_FOREACH( std::set< int >::value_type rClearMode, setClearModes ) {
            mapDifficulty[ rClearMode ].m_kModeDifficulty.Set( 0 );   // 기본 난이도(0)만 클리어 처리해준다
        }

        // DB 패킷을 세팅한다. 큐잉은 CheckCharacterGraduate()에서 한다
        kDBPacket_.m_mapOldDifficulty           = m_mapOldDifficulty;
        kDBPacket_.m_mapDifficulty              = mapDifficulty;
        kDBPacket_.m_setGraduateCharacters      = setGraduateCharacters;
        kDBPacket_.m_setNonGraduateCharacters   = setNonGraduateCharacters;
        kDBPacket_.m_vecRewards                 = vecRewards;

        bIsGraduate = true;
    }

    return bIsGraduate;
}

int KUser::GetGP()
{
    std::map<char, KCharacterInfo>::iterator mit;
    mit = m_mapCharacterInfo.find( m_cCurrentCharacter );
    if( mit != m_mapCharacterInfo.end() ) {
        return mit->second.m_nGamePoint;
    }
    return 0;
}

int KUser::GetInitGP()
{
    std::map<char, KCharacterInfo>::iterator mit;
    mit = m_mapCharacterInfo.find( m_cCurrentCharacter );
    if( mit != m_mapCharacterInfo.end() ) {
        return mit->second.m_nInitGP;
    }
    return 0;
}

int KUser::GetGP( IN const char cCharType_ )
{
    std::map<char, KCharacterInfo>::iterator mit;
    mit = m_mapCharacterInfo.find( cCharType_ );
    if( mit != m_mapCharacterInfo.end() ) {
        return mit->second.m_nGamePoint;
    }
    return 0;
}

int KUser::GetInitGP( IN const char cCharType_ )
{
    std::map<char, KCharacterInfo>::iterator mit;
    mit = m_mapCharacterInfo.find( cCharType_ );
    if( mit != m_mapCharacterInfo.end() ) {
        return mit->second.m_nInitGP;
    }
    return 0;
}

void KUser::GetDepotInfo()
{
    QUEUEING_EVENT_TO_DB( EVENT_DEPOT_CHAR_TAB_INFO_REQ, m_mapDepotInfo ); // 서버 접속 후  디비 호출은 1회만 일어나야 함.

    START_LOG( clog, L"창고 캐릭터,탭 해당 정보 불러오기 , User : " << GetName() )
        << BUILD_LOG( m_mapDepotInfo.size() )
        << END_LOG;
}

DWORD KUser::GetTongDonationUserAmount( IN const time_t tmVersion_, IN const DWORD dwNPCType_ )
{
    return m_mapTongDonationUserAmount[ tmVersion_ ][ dwNPCType_ ];
}

void KUser::SetTongDonationUserAmount( IN const time_t tmVersion_, IN const DWORD dwNPCType_, IN const DWORD dwAmount_ )
{
    m_mapTongDonationUserAmount[ tmVersion_ ][ dwNPCType_ ] = dwAmount_;
}

bool KUser::IsInfinityDungeonMonsterHack( IN const DWORD dwSummonGap_, IN const DWORD dwSummonMonsterCount_ )
{
    if ( false == SiKPreHackingCheckManager()->IsInfinityDungeonMonsterHackErrDisconnect() ) {
        return false;
    }

    if ( false == SiKPreHackingCheckManager()->IsInfinityDungeonMonsterLimitOver( dwSummonGap_, dwSummonMonsterCount_ ) ) {
        return false;
    }

    START_LOG( cerr, L"무한던전 해킹감지됨" )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( dwSummonGap_ )
        << BUILD_LOG( dwSummonMonsterCount_ )
        << END_LOG;

    return true;
}

void KUser::MakeIDSummonedMonsterInfo( IN KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ& kPacket_, IN DWORD dwIDMonsterLastUID_, OUT std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, OUT DWORD& dwIDMonsterLastUIDAfter_ )
{
    lstIDSummonedMonsters_.clear();
    dwIDMonsterLastUIDAfter_ = 0;

    BOOST_FOREACH( PAIR_INT& rMonsterInfo, kPacket_.m_vecSummonMonsters ) {
        KIDMonsterInfo kIDMonsterInfo;
        kIDMonsterInfo.m_nRound             = kPacket_.m_dwRoundSequence;
        kIDMonsterInfo.m_nMonID             = rMonsterInfo.first;
        kIDMonsterInfo.m_nMonLv             = rMonsterInfo.second;
        kIDMonsterInfo.m_nMonType           = kPacket_.m_nSummonType;

        kIDMonsterInfo.m_nMonUID            = dwIDMonsterLastUID_++;

        lstIDSummonedMonsters_.push_back( kIDMonsterInfo );

        // 통계용으로 타입별 소환된 몬스터 수를 기록해둔다. DB기록은 endgame, 모든 방원이 leavegame 하는 시점에 게임 플레이당 한번씩만 한다
        if ( NULL != m_pkRoom ) {
            m_pkRoom->IncreaseIDSummonedMonstersForStats( kPacket_.m_nSummonType );
        }
    }

    dwIDMonsterLastUIDAfter_ = dwIDMonsterLastUID_;
}

void KUser::MakeIDReservedDropInfo( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, IN std::vector< KInfinityDungeon::KIDRewardInfo >& vecIDRewardInfo_, IN DWORD nPartyNum_, IN DWORD dwIDReservedDropLastUID_, OUT std::list< KGameDropElement >& lstIDReservedDrops_, OUT DWORD& dwIDReservedDropLastUIDAfter_ )
{
    BOOST_FOREACH( KIDMonsterInfo& rIDSummonedMonster, lstIDSummonedMonsters_ ) {
        std::vector< KInfinityDungeon::KIDRewardInfo >::iterator vitIDRewardInfo;
        vitIDRewardInfo = std::find_if( vecIDRewardInfo_.begin(), vecIDRewardInfo_.end(),
            boost::bind( &KInfinityDungeon::KIDRewardInfo::m_nMonsterID, _1 ) == rIDSummonedMonster.m_nMonID );

        if ( vecIDRewardInfo_.end() == vitIDRewardInfo ) {
            continue;
        }

        BOOST_FOREACH( Kairo& rKairo, vitIDRewardInfo->m_vecKairoDrops ) {
            KDropItemInfo kDropItemInfo;
            if ( false == GetIDReward( rKairo, kDropItemInfo ) ) {
                continue;
            }

            KGameDropElement kGameDropElement;
            kGameDropElement.m_dwUserUID        = 0;
            kGameDropElement.m_nStageCountType  = -1;
            kGameDropElement.m_cRewardType      = KGameDropElement::RT_ITEM;
            kGameDropElement.m_nStrongLV        = 0;

            kGameDropElement.m_nTriggerID       = rIDSummonedMonster.m_nMonUID;
            kGameDropElement.m_nMonID           = rIDSummonedMonster.m_nMonID;

            kGameDropElement.m_dwItemID         = kDropItemInfo.m_ItemID;
            kGameDropElement.m_nCount           = kDropItemInfo.m_nDuration;
            kGameDropElement.m_nPeriod          = kDropItemInfo.m_nPeriod;
            kGameDropElement.m_fRatio           = 100.f;

            int nDropCount = nPartyNum_;  // 기본적으로 파티원 수 만큼 드랍 아이템을 생성한다
            if ( KInfinityDungeon::EDT_SOLO == vitIDRewardInfo->m_nDropType ) {
                nDropCount = 1;
            }

            for ( int i = 0; i < nDropCount; ++i ) {
                kGameDropElement.m_dwUID        = dwIDReservedDropLastUID_++;

                lstIDReservedDrops_.push_back( kGameDropElement );
            }
        }
    }

    dwIDReservedDropLastUIDAfter_ = dwIDReservedDropLastUID_;
}

void KUser::MakeIDReservedGPDropInfo( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, IN DWORD dwIDReservedDropLastUID_, OUT std::list< KGameDropElement >& lstIDReservedGPDrops_, OUT DWORD& dwIDReservedDropLastUIDAfter_ )
{
    lstIDReservedGPDrops_.clear();

    BOOST_FOREACH( KIDMonsterInfo& rIDSummonedMonster, lstIDSummonedMonsters_ ) {
        sGPDropInfo kInfo;
        SiKGameDrop()->MakeGPDropInfo( kInfo );
        float fGP = SiKGameDrop()->GetMonGp( rIDSummonedMonster.m_nMonID, rIDSummonedMonster.m_nMonLv );
        int nGP = static_cast< int >( fGP );

        if ( 0 >= nGP ) {
            continue;
        }

        for ( int i = 0; i < kInfo.m_nGoldCount; ++i ) {
            KGameDropElement kGameDropElement;
            kGameDropElement.m_dwUserUID        = GetUID(); // 클라이언트의 정상동작을 위해 방장의 유저UID를 임시로 넣어서 전달함(드랍처리에서 사용하지는 않음)
            kGameDropElement.m_nStageCountType  = -1;
            kGameDropElement.m_cRewardType      = KGameDropElement::RT_GP;
            kGameDropElement.m_nStrongLV        = 0;
            kGameDropElement.m_nTriggerID       = rIDSummonedMonster.m_nMonUID;
            kGameDropElement.m_nMonID           = rIDSummonedMonster.m_nMonID;
            kGameDropElement.m_dwItemID         = kInfo.m_dwGoldID;
            kGameDropElement.m_nCount           = nGP;
            kGameDropElement.m_nPeriod          = -1;
            kGameDropElement.m_fRatio           = kInfo.m_fGoldRatio;
            kGameDropElement.m_dwUID            = dwIDReservedDropLastUID_++;

            lstIDReservedGPDrops_.push_back( kGameDropElement );
        }

        for ( int i = 0; i < kInfo.m_nSilverCount; ++i ) {
            KGameDropElement kGameDropElement;
            kGameDropElement.m_dwUserUID        = GetUID(); // 클라이언트의 정상동작을 위해 방장의 유저UID를 임시로 넣어서 전달함(드랍처리에서 사용하지는 않음)
            kGameDropElement.m_nStageCountType  = -1;
            kGameDropElement.m_cRewardType      = KGameDropElement::RT_GP;
            kGameDropElement.m_nStrongLV        = 0;
            kGameDropElement.m_nTriggerID       = rIDSummonedMonster.m_nMonUID;
            kGameDropElement.m_nMonID           = rIDSummonedMonster.m_nMonID;
            kGameDropElement.m_dwItemID         = kInfo.m_dwSilverID;
            kGameDropElement.m_nCount           = nGP;
            kGameDropElement.m_nPeriod          = -1;
            kGameDropElement.m_fRatio           = kInfo.m_fSilverRatio;
            kGameDropElement.m_dwUID            = dwIDReservedDropLastUID_++;

            lstIDReservedGPDrops_.push_back( kGameDropElement );
        }
    }

    dwIDReservedDropLastUIDAfter_ = dwIDReservedDropLastUID_;
}

bool KUser::GetIDReward( IN Kairo& kKairo_, OUT KDropItemInfo& kAddItem_ )
{
    if ( false == kKairo_.Do( kAddItem_ ) ) {
        return false;
    }

    if ( 0 == kAddItem_.m_ItemID ) {
        return false;
    }

    return true;
}

bool KUser::AddIDSummonedMonsters( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, OUT std::list< KIDMonsterInfo >& lstIDSummonedMonstersAfter_ )
{
    lstIDSummonedMonstersAfter_.insert( lstIDSummonedMonstersAfter_.end(), lstIDSummonedMonsters_.begin(), lstIDSummonedMonsters_.end() );

    return true;
}

bool KUser::AddIDReservedDrops( IN std::list< KGameDropElement >& lstIDReservedDrops_, OUT std::list< KGameDropElement >& lstIDReservedDropsAfter_ )
{
    lstIDReservedDropsAfter_.insert( lstIDReservedDropsAfter_.end(), lstIDReservedDrops_.begin(), lstIDReservedDrops_.end() );

    return true;
}

bool KUser::AddIDReservedGPDrops( IN std::list< KGameDropElement >& lstIDReservedGPDrops_, OUT std::list< KGameDropElement >& lstIDReservedGPDropsAfter_ )
{
    lstIDReservedGPDropsAfter_.insert( lstIDReservedGPDropsAfter_.end(), lstIDReservedGPDrops_.begin(), lstIDReservedGPDrops_.end() );

    return true;
}

bool KUser::SetInfinityDungeonMonsterSummonBroadPacket( IN std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, IN std::list< KGameDropElement >& lstIDReservedDrops_, IN std::list< KGameDropElement >& lstIDReservedGPDrops_, OUT KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_BROAD& kPacket_ )
{
    kPacket_.m_vecSummonedMonsters.clear();
    kPacket_.m_vecMonsterDrop.clear();
    kPacket_.m_vecGpDrop.clear();

    kPacket_.m_vecSummonedMonsters.insert( kPacket_.m_vecSummonedMonsters.begin(), lstIDSummonedMonsters_.begin(), lstIDSummonedMonsters_.end() );
    kPacket_.m_vecMonsterDrop.insert( kPacket_.m_vecMonsterDrop.begin(), lstIDReservedDrops_.begin(), lstIDReservedDrops_.end() );
    kPacket_.m_vecGpDrop.insert( kPacket_.m_vecGpDrop.begin(), lstIDReservedGPDrops_.begin(), lstIDReservedGPDrops_.end() );

    return true;
}

bool KUser::DoKillInfinityDungeonMonster( IN const int nMonsterUID_, IN OUT std::list< KIDMonsterInfo >& lstIDSummonedMonsters_, OUT KIDMonsterInfo& kIDMonsterInfo_ )
{
    std::list< KIDMonsterInfo >::iterator lit;
    lit = std::find_if( lstIDSummonedMonsters_.begin(), lstIDSummonedMonsters_.end(),
        boost::bind( &KIDMonsterInfo::m_nMonUID, _1 ) == nMonsterUID_ );

    if ( lstIDSummonedMonsters_.end() == lit ) {
        return false;
    }

    kIDMonsterInfo_ = *lit;

    lstIDSummonedMonsters_.erase( lit );

    return true;
}

bool KUser::DoGetInfinityDungeonItem( IN const DWORD dwDropUID_, IN std::list< KGameDropElement >& lstIDReservedDrops_, OUT KDropItemInfo& kDropItemInfo_, OUT DWORD& dwReservedUserUID_ )
{
    dwReservedUserUID_ = 0;

    std::list< KGameDropElement >::iterator lit;
    lit = std::find_if( lstIDReservedDrops_.begin(), lstIDReservedDrops_.end(),
        boost::bind( &KGameDropElement::m_dwUID, _1 ) == dwDropUID_ );

    if ( lstIDReservedDrops_.end() == lit ) {
        return false;
    }

    kDropItemInfo_.m_ItemID     = lit->m_dwItemID;
    kDropItemInfo_.m_nDuration  = lit->m_nCount;
    kDropItemInfo_.m_nPeriod    = lit->m_nPeriod;

    lstIDReservedDrops_.erase( lit );

    if ( KGameDropElement::RT_CHARDROP_ITEM == lit->m_cRewardType ) {
        // 획득할 유저가 정해져 있는 아이템인 경우 UserUID 정보를 저장해둔다
        dwReservedUserUID_ = lit->m_dwUserUID;
    }

    return true;
}

bool KUser::DecideWhoGetDropItem( IN GCITEMID dwItemID_, IN std::vector< DWORD >& vecUserUIDs_, IN OUT DWORD& dwRecvUserUID_, OUT char& cRecvUserCharType_, OUT std::map< DWORD, int >& mapUserDice_ )
{
    cRecvUserCharType_ = -1;
    mapUserDice_.clear();

    if ( true == vecUserUIDs_.empty() ) {
        return false;
    }

    // 아이템을 추가로 획득 가능한 유저들을 추려낸다
    std::vector< PAIR_DWORD_CHAR > vecNewUserUIDs;
    GetAffordableInvenUsers( dwItemID_, vecUserUIDs_, vecNewUserUIDs );

    if ( true == vecNewUserUIDs.empty() ) {
        START_LOG( clog, L"아이템을 추가로 획득 가능한 유저가 없음. ItemID: " << dwItemID_ ) << END_LOG;

        return false;
    }

    // 참고: 받을 유저의 UID정보는 DoGetInfinityDungeonItem() 함수에서 세팅한다
    if ( 0 != dwRecvUserUID_ ) {
        // 획득할 유저가 정해져 있는 아이템인 경우 처리
        std::vector< PAIR_DWORD_CHAR >::iterator vit;
        vit = std::find_if( vecNewUserUIDs.begin(), vecNewUserUIDs.end(),
            boost::bind( &PAIR_DWORD_CHAR::first, _1 ) == dwRecvUserUID_ );

        if ( vecNewUserUIDs.end() != vit ) {                                        // 지정되어 있는 유저가 현재 아이템을 받을 수 있다면
            PAIR_DWORD_CHAR prRecvUser( dwRecvUserUID_, vit->second );              // 해당 유저의 정보를 저장해두고(UserUID, CharType)
            vecNewUserUIDs.erase( vit );                                            // 방원 리스트에서 잠시 삭제했다가
            std::random_shuffle( vecNewUserUIDs.begin(), vecNewUserUIDs.end() );    // 나머지 방원들의 순서를 섞은 뒤
            vecNewUserUIDs.push_back( prRecvUser );                                 // 벡터의 마지막에 다시 넣어줌
        }
        else {
            std::random_shuffle( vecNewUserUIDs.begin(), vecNewUserUIDs.end() );    // 지정된 유저가 현재 아이템을 받을 수 없는 상황이라면 나머지 방원 중에 지급한다
        }
    }
    else {
        // 획득할 유저가 정해져 있지 않은 아이템인 경우 처리
        std::random_shuffle( vecNewUserUIDs.begin(), vecNewUserUIDs.end() );
    }

    // 주사위 수를 아이템을 추가로 획득 가능한 유저 수 만큼 만든다
    std::set< int > setDice;
    setDice.clear();
    int nNewUserUIDs = vecNewUserUIDs.size();
    for ( int i = 0; i < nNewUserUIDs; ++i ) {
        setDice.insert( ( rand() % 100 ) + 1 );
    }

    // 주사위 수를 정렬된 순서대로 넣는다
    std::set< int >::iterator sit = setDice.begin();
    BOOST_FOREACH( PAIR_DWORD_CHAR& rprUserUIDs, vecNewUserUIDs ) {
        mapUserDice_[ rprUserUIDs.first ] = *sit;
        ++sit;
    }

    // 즉 벡터의 마지막 유저가 아이템을 획득하게 된다
    dwRecvUserUID_ = vecNewUserUIDs.rbegin()->first;
    cRecvUserCharType_ = vecNewUserUIDs.rbegin()->second;

    return true;
}

bool KUser::GetAffordableInvenUsers( IN GCITEMID dwItemID_, IN std::vector< DWORD >& vecUserUIDs_, OUT std::vector< PAIR_DWORD_CHAR >& vecNewUserUIDs_ )
{
    vecNewUserUIDs_.clear(); // ( UserUID, CharType )

    BOOST_FOREACH( DWORD& rUserUID, vecUserUIDs_ ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >( rUserUID );
        if ( NULL == spUser ) {
            START_LOG( clog, L"서버에서 유저를 찾을 수 없어 아이템 지급하지 않음. UserUID: " << rUserUID ) << END_LOG;

            continue;
        }

        if ( false == spUser->CheckItemInvenLimit( dwItemID_ ) ) {
            START_LOG( clog, L"최대 인벤 사이즈를 초과해서 더이상 아이템 지급하지 않음. ItemID: " << dwItemID_ )
                << BUILD_LOG( spUser->GetUID() )
                << BUILD_LOG( spUser->GetName() )
                << BUILD_LOG( spUser->GetTrueInvenCapacity() )
                << BUILD_LOG( spUser->m_kInventory.GetTotalEnableSize() ) << END_LOG;

            continue;
        }

        vecNewUserUIDs_.push_back( std::make_pair( rUserUID, spUser->GetCurrentCharType() ) );
    }

    return true;
}

bool KUser::AddStatInfinityDungeon( IN const int nEndType_ )
{
    if ( NULL == m_pkRoom ) {
        return false;
    }

    if ( false == SiKInfinityDungeon()->IsInfinityDungeon( m_pkRoom->m_iGameMode ) ) {
        return false;
    }

    KDB_EVENT_INFINITY_DUNGEON_STAT_NOT kDBPacket;
    kDBPacket.m_nModeID = m_pkRoom->m_iGameMode;
    kDBPacket.m_nDifficulty = m_pkRoom->m_nDifficulty;
    kDBPacket.m_nEndState = nEndType_;

    std::map< int, int > mapIDSummonedMonstersForStats;
    m_pkRoom->GetIDSummonedMonstersForStats( mapIDSummonedMonstersForStats );
    kDBPacket.m_nSummonedNormal = mapIDSummonedMonstersForStats[ KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ::EST_NORMAL ];
    kDBPacket.m_nSummonedIntervalBoss = mapIDSummonedMonstersForStats[ KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ::EST_BOSS_INTERVAL ];;
    kDBPacket.m_nSummonedRankBoss = mapIDSummonedMonstersForStats[ KEVENT_INFINITY_DUNGEON_MONSTER_SUMMON_REQ::EST_BOSS_RANK ];;

    m_pkRoom->GetResultExp( kDBPacket.m_mapGainExp );
    m_pkRoom->GetIDPlayTime( kDBPacket.m_mapPlayTime );
    kDBPacket.m_nStartParty = kDBPacket.m_mapPlayTime.size();

    QUEUEING_EVENT_TO_DB( DB_EVENT_INFINITY_DUNGEON_STAT_NOT, kDBPacket );

    START_LOG( clog, L"무한던전 통계 기록 요청" ) << END_LOG;

    return true;
}

float KUser::GetDropEventScale( IN const KGameInfo& kGameInfo_ )
{
    float fDropEventScale = 0.f;

    // 레인보우 이벤트 보너스
    KRainBowData kRainBowData;
    if ( SiKRainbow()->GetEventByID( KRainbow::RE_DUNGEON_DROP, kRainBowData ) ) {
        fDropEventScale = kRainBowData.m_fPrimaryRatio;
    }

    if ( kGameInfo_.m_nPCBangUserNum > 0 ) {
        fDropEventScale += SiKIPAdvantage()->GetDungeonDropBonus( KIPAdvantage::PCBANG_PUBLISHER );
    }

    // D-Bot Event Perfect 보너스
    fDropEventScale += SiKTimeEventManager()->GetDungeonDropRatioBonus( kGameInfo_.m_iGameMode );

    // 추천 레벨 서버 보너스
    std::vector<DWORD>::const_iterator vit;
    for ( vit = kGameInfo_.m_vecUserUIDs.begin() ; vit != kGameInfo_.m_vecUserUIDs.end() ; ++vit ) {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID< KUser >( *vit );
        if ( NULL == spUser ) {
            continue;
        }

        const KCharacterInfo* pkChar = spUser->GetCurrentCharacter();
        if ( NULL != pkChar && SiKServerBenefit()->IsCorrectLvRange( static_cast< const int >( pkChar->m_dwLevel ) ) ) {
            fDropEventScale += SiKServerBenefit()->GetDropBenefit();
        }
    }

    return fDropEventScale;
}

bool KUser::BindIDReservedDropToUsers( IN std::list< KGameDropElement >& lstIDReservedDrops_, IN std::vector< DWORD >& vecUserUIDs_ )
{
    DWORD dwUserSize = vecUserUIDs_.size();

    BOOST_FOREACH( KGameDropElement& rGameDropElement, lstIDReservedDrops_ ) {
        if ( 0 == rGameDropElement.m_dwUserUID ) {
            rGameDropElement.m_dwUserUID = vecUserUIDs_[ SiKGameDrop()->GetRandomNum() % dwUserSize ];
        }
        else {
            rGameDropElement.m_cRewardType = KGameDropElement::RT_CHARDROP_ITEM;
        }
    }

    return true;
}
