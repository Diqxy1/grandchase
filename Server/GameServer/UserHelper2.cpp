#include "User.h"
#include <dbg/dbg.hpp>
#include <boost/algorithm/string.hpp>
#include "UserFSM.h"
#include "Room.h"
#include "GSSimLayer.h"
#include "Guild.h"
#include "FailRate.h"
#include "Channel.h"
#include "GameServer.h"
#include "MissionManager.h"
#include "PetHelper.h"
#include "SlangFilter.h"
#include "ChannelDrop.h"
#include "ResultManager.h"
#include "TimeEventManager.h"
#include <boost/bind.hpp>
#include "RequierInfo.h"
#include "GCHelper.h"
#include "IPAdvantage.h"
#include "StatisticsManager.h"
#include "MapStatistics.h"
#include "CPLStatistics.h"
#include "PlayTimeStat.h"
#include "Square.h"
#include "SquareManager.h"
#include "EnchantStrong.h"
#include "StatGameLoading.h"
#include "CollectionManager.h"
#include "StatDeathMatch.h"
#include "support_User.h"
#include "LogManager.h"
#include "IndigoUtil.h"
#include "ChannelManager.h"
#include "CnConnector.h"
#include "StatMiniGame.h"
#include "SkillManager.h"
#include "StatSpx.h"
#include "StatLeaveGame.h"
#include "PeriodNotice.h"
#include "RecomHelper.h"
#include "PreHackingCheckManager.h"
#include "Rainbow.h"
#include "DicePlay.h"
#include "IPAdvantage.h"
#include "ServerBenefit.h"
#include "EventMonsterManager.h"
#include "StatCharacter.h"
#include "GCPoint.h"
#include "StatCharacterEquip.h"
#include "Escort.h"
#include "PVPMatchManager.h"
#include "VitalityManager.h"
#include "ConnectionGiftBox.h"
#include <boost/foreach.hpp>
#include "StatPvPPlayCharacter.h"

void KUser::GetInfo( OUT KDetailUserInfo& kInfo_ )
{
    kInfo_.m_strLogin           = m_strName;
    kInfo_.m_strNickName        = m_strNickName;
    kInfo_.m_bMale              = m_bMale;
    GetGuildUserInfo( kInfo_.m_kGuildUserInfo );
    kInfo_.m_cKorGrade          = m_cGrade;

    const KCharacterInfo* pkCharacterInfo = GetBestCharacter();
    if( pkCharacterInfo )
    {
        kInfo_.m_iWin               = pkCharacterInfo->m_iWin;
        kInfo_.m_iLose              = pkCharacterInfo->m_iLose;
        kInfo_.m_cCurrentCharType   = pkCharacterInfo->m_cCharType;
        kInfo_.m_biCurrentCharExp   = pkCharacterInfo->m_biExp;
        kInfo_.m_vecCurCharEquip    = pkCharacterInfo->m_vecEquipItems;
    }
    else
    {
        START_LOG( cerr, L"캐릭터 인덱스 이상." ) << END_LOG;
    }

    switch( GetStateID() )  // godsend 사용자 상태 추가 [6/4/2004]
    {
    case KUserFSM::STATE_CHANNEL:
    case KUserFSM::STATE_CHANNELLOBBY:  // Channel 에 있는 경우 : L        
        kInfo_.m_cLiveState   = -1;
        break;
    case KUserFSM::STATE_ROOM:          // Room 에 있는 경우 : wait.        
        //case KUserFSM::STATE_JOIN:          // Room 에 있는 경우 : wait.        
        JIF( m_pkRoom );

        kInfo_.m_cLiveState   = static_cast<char>( m_pkRoom->m_usRoomID );
        break;
    case KUserFSM::STATE_PLAYING:       // 경기 중인 경우        
        kInfo_.m_cLiveState   = -2;
        break;
    default:
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( GetStateIDString() )
            << END_LOG;
    }
}

void KUser::PreStartIndigo( OUT KInDoorUserInfo& kInDoorUserInfo_ )
{
    //태그 매치(인디고)인경우 비어 있는 태그 슬롯에 0,1,2 순서로 채워 넣는다.

    //캐릭터를 모은다
    std::set<char> setCharacters; //자동 정렬된다.
    for( char ch = 0 ; ch < GC_CHAR_NUM ; ++ch )
    {
        setCharacters.insert( ch );
    }

    // 이미 등록된 캐릭터를 지우고
    int i = 0;
    for( i = 0 ; i < 3 ; ++i )
    {
        std::set<char>::iterator sit;
        sit = setCharacters.find( kInDoorUserInfo_.m_acTagModeInfo[i][0] );
        if( sit != setCharacters.end() )
        {
            setCharacters.erase( sit );
        }
    }

    // 빈곳에 순서대로 집어 넣는다.
    for( i = 0 ; i < 3 ; ++i )
    {
        if( kInDoorUserInfo_.m_acTagModeInfo[i][0] < 0 )
        {
            kInDoorUserInfo_.m_acTagModeInfo[i][0] = *(setCharacters.begin());
            setCharacters.erase( setCharacters.begin() );
            kInDoorUserInfo_.m_acTagModeInfo[i][1] = 0;
        }
    }
}

// 작명 센스.. ㅡㅡ; 혹시나 핵킹에 의해서 문제가 생길까봐서.
void KUser::MakeCorrectInDoorInfo( IN OUT KInDoorUserInfo& kInDoorUserInfo_ )
{
    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo(kInDoorUserInfo_.m_cCharIndex);
    if (pkCharacterInfo != NULL) {
        kInDoorUserInfo_.m_cPromotion = pkCharacterInfo->m_cCurrentPromotion;
        kInDoorUserInfo_.m_dwLevel = pkCharacterInfo->m_dwLevel;
        kInDoorUserInfo_.m_strCharNickName = pkCharacterInfo->m_strCharName;
    }
    else {
        kInDoorUserInfo_.m_cCharIndex = 0;
        kInDoorUserInfo_.m_cPromotion = 0;
    }

    kInDoorUserInfo_.m_strLogin = m_strName;
    kInDoorUserInfo_.m_dwUserUID = GetUID();
    kInDoorUserInfo_.m_strNickName = m_strNickName;
    kInDoorUserInfo_.m_strNickColor = m_strNickColor;
    kInDoorUserInfo_.m_iPlayerIndex = 0;

    kInDoorUserInfo_.m_vecFontVector = m_vecFontVector;
    kInDoorUserInfo_.m_iPvExp = m_iPvExp;

    kInDoorUserInfo_.m_bIsObserver = false;
    if (IsObserver())
    {
        kInDoorUserInfo_.m_bIsObserver = true;
    }

    kInDoorUserInfo_.m_nState = GC_RUS_WAIT;
    kInDoorUserInfo_.m_bMale = m_bMale;
    kInDoorUserInfo_.m_cAuthLevel = m_cAuthLevel;
    kInDoorUserInfo_.m_iGP = GetGP();
    kInDoorUserInfo_.m_cPCBangType = GetPCBangType();
    kInDoorUserInfo_.m_cUserBenfitType = GetUserBenfitType();

    kInDoorUserInfo_.m_bIsHost = false;
    kInDoorUserInfo_.m_bIsLive = true;

    KNGuildInfo guildInfo;
    if (GetGuildInfo(guildInfo) && m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_JOINER)
    {

        kInDoorUserInfo_.m_dwGuildUID = guildInfo.m_dwUID;
        kInDoorUserInfo_.m_strMarkName = guildInfo.m_strFileName;
        kInDoorUserInfo_.m_strGuildName = guildInfo.m_strName;
        kInDoorUserInfo_.m_iContributionPoint = m_kGuildUserInfo.m_dwContributePoint;
    }
    else
    {
        kInDoorUserInfo_.m_dwGuildUID = 0;
        kInDoorUserInfo_.m_strMarkName = std::wstring(L"");
        kInDoorUserInfo_.m_strGuildName = std::wstring(L"");
        kInDoorUserInfo_.m_iContributionPoint = 0;
    }

    GetInDoorCharInfo(kInDoorUserInfo_.m_vecCharInfo);
    CheckInventoryPremiumItem(kInDoorUserInfo_.m_kPremiumInfo);
    GetCharQuickSlotInfo(kInDoorUserInfo_.m_kQuickSlot);

    kInDoorUserInfo_.m_cAuthLevel = m_cAuthLevel;
    kInDoorUserInfo_.m_mapMonthlyRank = m_kRankInfo.m_mapMonthlyRank;
    kInDoorUserInfo_.m_dwCoupleUID = 0;

    if (IsCouple())
    {
        kInDoorUserInfo_.m_strPartnerNick = m_kCoupleInfo.m_strPartnerNick;
        kInDoorUserInfo_.m_dwCoupleUID = GetPartnerUID();
        GetCoupleEquip(kInDoorUserInfo_.m_vecCoupleEquip);
    }

    kInDoorUserInfo_.m_wstrCCode = m_wstrCCode;
    kInDoorUserInfo_.m_cGrade = m_cGrade;
    GetUserDifficultInfo(kInDoorUserInfo_.m_mapDifficulty);

    kInDoorUserInfo_.m_kCoupleInfo = m_kCoupleInfo;
    kInDoorUserInfo_.m_kRecommenderInfo = m_MyRecomInfo;
    kInDoorUserInfo_.m_bNewUserAfterEventDate = IsNewUserAfterEventDate();
    kInDoorUserInfo_.m_nAccOnlineTime = m_nAccOnlineTime + GetElapsedTimeAfterAuth();
    kInDoorUserInfo_.m_mapFairyTreeFruitBuff = m_mapFairyTreeFruitBuff;

    std::set<GCITEMID> setInvenBuffItemID;
    CheckInventoryBuffItem(setInvenBuffItemID);
    kInDoorUserInfo_.m_setInvenBuffItemID = setInvenBuffItemID;
}

void KUser::CheckLoadingComplete()
{
    if ( GetLoadingComplete() ) {
        return;
    }

    if ( ( ::GetTickCount() - GetTick( GAME_START_TICK ) ) > SiKGSSimLayer()->m_dwGameStartWaitTime ) {
        KStatGameLoadingPtr spStat = GET_STAT( KStatGameLoading, SI_STAT_GAMELOADING );
        if ( spStat ) {
            spStat->AddStat( ::GetTickCount() - GetTick( GAME_START_TICK ) );
        }
        
        m_bRecvLoadingComplete = true;
        SetStageLoadComplete( true );

        JIF( m_pkRoom );
        m_pkRoom->CheckLoadingCompleteUser( GetName(), GetUID() ); // 이 방식에도 예외는 있는데
        SEND_ID( EVENT_LOADING_TIME_OVER_NOT ); // 로딩 시간 초과시 클라이언트로 알려줌
    }
}

void KUser::CheckStageLoadComplete()
{
    if ( GetStageLoadComplete() ) {
        return;
    }

    if ( m_pkRoom && m_pkRoom->GetStageLoadSwitch() == false ) {
        return;
    }

    if ( m_pkRoom && m_pkRoom->IsPvPMode( m_pkRoom->m_iGameMode ) ) {
        return;
    }

    if ( ( ::GetTickCount() - GetTick( STAGE_LOAD_TICK ) ) > SiKGSSimLayer()->m_dwStageLoadLimitTime ) {

        SetStageLoadComplete( true );
        
        JIF( m_pkRoom );
        m_pkRoom->FailStageLoadComplete( GetName(), GetUID() ); // 이 방식에도 예외는 있는데
        SEND_ID( EVENT_LOADING_TIME_OVER_NOT ); // 로딩 시간 초과시 클라이언트로 알려줌
    }
}

bool KUser::CheckIfItemUsed( int iItemID )
{
    std::vector<int>::iterator iter;
    bool bRet = false;

    iter = m_vecUsedDurationItem.begin();
    while( iter != m_vecUsedDurationItem.end() )
    {
        if( iItemID == ( *iter ) )
        {
            bRet = true;
            break;
        }
        ++iter;
    }

    return bRet;
}

bool KUser::CheckIfAllItemExist( GCITEMID ItemID_, ... )
{
    va_list marker;
    GCITEMID ItemID = ItemID_;

    va_start( marker, ItemID_ );   /* Initialize variable arguments. */
    while( ItemID != -1 )
    {
        // 일반 아이템에서 먼저 찾기
        KItemPtr pkItem;
        pkItem = m_kInventory.FindItemByItemID( ItemID );
        if (  pkItem == NULL ) {
                return false;
        }

        if ( pkItem->m_nCount == 0 ) {
            return false;
        }

        ItemID = va_arg( marker, int );
    }
    va_end( marker );   /* Reset variable arguments. */

    return true;
}

bool KUser::CheckIfDurItemExist( GCITEMID dwItemID_, int nCount_, bool bDirectUse_ )
{
    KItemPtr pkItem;
    pkItem = m_kInventory.FindDurationItemByItemID( dwItemID_ );

    // 해당 아이템의 아이디가 존재하지 않으면 리턴.
    if( pkItem == NULL ) return false;

    if( pkItem->m_nCount < nCount_ ) // 작은 경우에만 실패. 정확히 요구량만을 가진 경우도 통과된다.
        return false;

    if( bDirectUse_ )
    {
        pkItem->m_nCount = std::max<int>( 0, pkItem->m_nCount - nCount_ );
        std::vector<KItem> kPacket;
        kPacket.push_back( *pkItem );

        if ( pkItem->m_nCount <= 0 ) {
            QUEUEING_EVENT_TO_DB( EVENT_UPDATE_ITEM_COUNT_NOT, kPacket );
        }
        else {
            SEND_PACKET( EVENT_UPDATE_ITEM_COUNT_NOT );
        }
    }

    return true;
}

UINT KUser::GetDurItemCount( IN const GCITEMID itemID_ )
{
    KItemPtr pkItem = m_kInventory.FindItemByItemID( itemID_ );
    // 해당 아이템의 아이디가 존재하지 않으면 리턴.
    if( pkItem == NULL ) return 0;

    return pkItem->m_nCount;
}

void KUser::CheckExpirationItem()
{
    std::vector<KItem> kPacket;
    m_kInventory.GetExpiredItemList( kPacket );

    if ( kPacket.empty() ) {
        START_LOG( clog, L"만료된 기간제 아이템 없음. (name:" << m_strName << L") 현재 아이템수 : " << m_kInventory.GetTotalItemNum() ) << END_LOG;
        return;
    }

    START_LOG( clog, L"" << kPacket.size() << L"개의 아이템 기한 만료 판단. Name : " << m_strName ) << END_LOG;

    QUEUEING_EVENT_TO_DB( DB_EVENT_CHECK_EXPIRATION_ITEM_REQ, kPacket );
}

int KUser::GetElapsedTimeAfterAuth() const
{
    CTimeSpan cSpan = CTime::GetCurrentTime() - m_cConnectionTime;
    return static_cast<int>( cSpan.GetTotalMinutes() );
}

void KUser::SetUdpPort( IN const SOCKADDR_IN& sender_, IN const KUdpToServer& kData_ )
{
    unsigned short kPacket;
    kPacket = ntohs(sender_.sin_port);
    SetUDPUsePort( kPacket );

    SEND_PACKET( EVENT_UDP_PORT_NOT );
}

KCharacterInfo* KUser::GetCharacterInfo( IN int iIndex )
{
    std::map<char, KCharacterInfo>::iterator mit;

    mit = m_mapCharacterInfo.find( iIndex );
    if( mit == m_mapCharacterInfo.end() )
    {
        return NULL;
    }
    else
    {
        return &mit->second;
    }
}

KCharacterInfo* KUser::GetBestCharacter()
{
    if( m_mapCharacterInfo.empty() ) return NULL;

    std::vector<KCharacterInfo*> vecCharPtr;

    std::map<char, KCharacterInfo>::iterator mit;

    for( mit = m_mapCharacterInfo.begin(); mit != m_mapCharacterInfo.end(); ++mit )
    {
        vecCharPtr.push_back( &mit->second );
    }

    std::sort( vecCharPtr.begin(), vecCharPtr.end(), 
        boost::bind( &KCharacterInfo::m_biExp, _1 ) >
        boost::bind( &KCharacterInfo::m_biExp, _2 ) );

    if( vecCharPtr.empty() ) return NULL;

    return *vecCharPtr.begin();
}

KCharacterInfo* KUser::GetCurrentCharacter()
{
    return GetCharacterInfo( m_cCurrentCharacter );
}

char KUser::GetCurrentCharType()
{
    return m_cCurrentCharacter;
}

char KUser::GetCurrentPromotion()
{
    KCharacterInfo* pkCharacterInfo = GetCurrentCharacter();

    if( pkCharacterInfo == NULL ) return 0;

    return pkCharacterInfo->m_cCurrentPromotion;
}

bool KUser::SetCurrentChar( char cCurrentChar_ )
{
    if( m_mapCharacterInfo.find( cCurrentChar_ ) != m_mapCharacterInfo.end() )
    {
        m_cCurrentCharacter = cCurrentChar_;
        return true;
    }

    return false;
}

bool KUser::IsCharExist( char cCharID )
{
    return ( m_mapCharacterInfo.find( cCharID ) != m_mapCharacterInfo.end() );
}

bool KUser::CheckExceedRefCount( int nCount_ )
{
    if( KSession::CheckExceedRefCount( nCount_ ) ) return true;

    // 초과된 refcount가 있을 경우 로그를 출력하고 추가작업을 진행한다.

    START_LOG_WITH_NAME( cerr )
        << BUILD_LOG( m_pkChannel )
        << BUILD_LOG( m_pkRoom )
        << BUILD_LOG( m_spSquare.get() )
        << BUILD_LOG( GetStateIDString() )
        << BUILD_LOG( GetRefCount() )
        << BUILD_LOG( GetElapsedTimeAfterAuth() )
        << BUILD_LOG( GetUID() )
        << BUILD_LOG( GetSockObj() )
        << BUILD_LOG( GetSockObj()->GetKey() )
        << END_LOG;

    // 아직 채널과 방 포인터가 유효하다면 다시한번 이곳들에 대해서 퇴장처리를 진행해본다.

    if( m_pkRoom )
        m_pkRoom->Leave( *this, KRoom::LEAVE_SERVER_FIND_ERR ); //leave room

    LIF( KSession::CheckExceedRefCount( nCount_ ) );

    if( m_pkChannel )
        m_pkChannel->Leave( *this );    //leave channel

    // 일반 채널에서 나가는 경우 자동으로 채널로비로 입장하게 된다.
    // 한번 퇴장처리 후에도 아직까지 포인터가 살아있다면 한번더 퇴장처리.

    if( m_pkChannel )
        m_pkChannel->Leave( *this );

    SiKChannelManager()->DeleteUserChannel( GetName() );


    LIF( KSession::CheckExceedRefCount( nCount_ ) );

    if( m_spSquare )
        m_spSquare->LeaveSquare( GetUID() );

    LIF( KSession::CheckExceedRefCount( nCount_ ) );

    return false;
}

bool KUser::AdditionalDrop( IN const KEndGameReq& kReq_, OUT std::map<DWORD, DROP_ITEMS>& mapEachUserDrops_ )
{
    //게임결과를 이터레이션 한다.
    std::vector<KGameResultIn>::const_iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        _JIF( !vit->m_vecCharExp.empty(), return false );
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin ) );

        if( !spUser ) // 이미 로그아웃한 유저라면 스킵.
            continue;

        std::vector<KDropItemInfo> vecdrops;
        vecdrops.clear();

        if( !spUser->m_vecMissionSlot.empty() ) // 미션, 아래는 서브미션 조건 체크 & 보상, 카운트 증가
        {
            SiKMissionManager()->DropSubMission( *spUser, *m_pkRoom, *vit, kReq_, vecdrops );

            START_LOG( clog, spUser->GetName() <<L"유저 : "<< vecdrops.size() << L"개 드랍(서브미션)" ) << END_LOG;
        }

        if( !spUser->m_vecCollectionSlot.empty() ) // 컬렉션
        {
            SiKCollectionManager()->DropSubMission( *spUser, *m_pkRoom, *vit, kReq_, vecdrops );
            START_LOG( clog, spUser->GetName() <<L"유저 : "<< vecdrops.size() << L"개 드랍(컬렉션 서브미션)" ) << END_LOG;
        }

        /////////////////////호위 이벤트 보상 아이템 지급(Escort)
        if ( kReq_.m_bIsActiveEscortEvent && vit->m_bWin ) {
            KCharacterInfo* pkCharInfo = spUser->GetCurrentCharacter();
            if ( !pkCharInfo ) {
                START_LOG( cwarn, L"유저 캐릭터 정보가 없음. LoginID : " << spUser->GetName() ) << END_LOG;
            }
            else {
                CheckEscortDrop( kReq_.m_bIsEscortMonsterAlive, kReq_.m_dwEscortMonsterID, m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, pkCharInfo->m_dwLevel, vecdrops );
            }
        }

        /////////////////////호위 이벤트

        if( !vecdrops.empty() )
        {
            std::map<DWORD, DROP_ITEMS>::iterator mit;
            mit = mapEachUserDrops_.find( vit->m_dwUID );
            if( mit != mapEachUserDrops_.end() )
            {
                mit->second.insert( mit->second.end(), vecdrops.begin(), vecdrops.end() );
            }
            else
            {
                mapEachUserDrops_[ vit->m_dwUID ] = vecdrops;
            }
        }
    }

    return !mapEachUserDrops_.empty();
}

bool KUser::SetIndigoInfo( IN const KIndigoData& kIndigoData_ )
{
    if( kIndigoData_.m_cOK != 0 )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOGc( kIndigoData_.m_cOK )
            << END_LOG;

        return false;
    }

    m_iIndigoWin.SetAllValue( kIndigoData_.m_iWin );
    m_iIndigoLose.SetAllValue( kIndigoData_.m_iLose );
    m_iRP.SetAllValue( kIndigoData_.m_iRP );

    return true;
}

bool KUser::GetPetInfo( IN GCITEMUID dwUID_, OUT KPetInfo** pkPetInfo_ )
{
    *(pkPetInfo_) = NULL;

    if( m_mapPetInfo.empty() )
        return false;

    std::map<GCITEMUID, KPetInfo>::iterator mit;
    mit = m_mapPetInfo.find( dwUID_ );

    if( mit == m_mapPetInfo.end() )
        return false;

    (*pkPetInfo_) = &(mit->second);

    if( (*pkPetInfo_)->m_iLevel < 0 )
    {
        (*pkPetInfo_)->m_iLevel = SiKPetHelper()->GetPetLevel( (*pkPetInfo_)->m_dwEXP );
    }

    return ( NULL != *(pkPetInfo_) );
}

bool KUser::GetBestPet( OUT KPetInfo* pkPetInfo_ )
{
    pkPetInfo_ = NULL;
    if( m_mapPetInfo.empty() )
        return false;

    std::map<GCITEMUID, KPetInfo>::iterator mitBest; // pkPetInfo_ 로 대체 가능 하다.
    std::map<GCITEMUID, KPetInfo>::iterator mit;
    mitBest = m_mapPetInfo.begin();

    for( mit = m_mapPetInfo.begin() ; mit != m_mapPetInfo.end() ; ++mit )
    {
        if( mitBest->second.m_dwEXP < mit->second.m_dwEXP )
            mitBest = mit;
    }

    pkPetInfo_ = &(mitBest->second);
    return true;
}

bool KUser::EquipPet( IN GCITEMUID dwPetUID_, IN char cCharType_ )
{
    //펫을 찾아서 해당 캐릭터에게 셋팅한다.
    std::map<GCITEMUID, KPetInfo>::iterator mit;
    mit = m_mapPetInfo.find( dwPetUID_ );

    if( mit == m_mapPetInfo.end() )
        return false;

    KCharacterInfo* pkCharacter = NULL;
    pkCharacter = GetCharacterInfo( cCharType_ );
    if( pkCharacter == NULL )
        return false;

    pkCharacter->m_kEquipPetInfo.m_dwUID = dwPetUID_;
    return true;
}

void KUser::UnEquipAllPet()
{
    // 모든 펫을 해제한다.
    std::map<char, KCharacterInfo>::iterator mit;
    for ( mit = m_mapCharacterInfo.begin(); mit != m_mapCharacterInfo.end(); ++mit ) {
        KPetInfo* pkPetInfo = NULL;
        GetPetInfo( mit->second.m_kEquipPetInfo.m_dwUID, &pkPetInfo );
        if ( pkPetInfo ) {
            pkPetInfo->m_vecEquipItem.clear();
        }
        mit->second.m_kEquipPetInfo.Init();
    }
}

bool KUser::GetPetOwner( IN GCITEMUID dwPetUID_, OUT std::vector<char>& vecCharType_ )
{
    vecCharType_.clear();
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {
        if ( cmit->second.m_kEquipPetInfo.m_dwUID == dwPetUID_ ) {
            vecCharType_.push_back( cmit->second.m_cCharType );
        }
    }
    return (!vecCharType_.empty());
}

bool KUser::DeletePet( IN GCITEMUID dwPetUID_ )
{
    std::map<GCITEMUID, KPetInfo>::iterator mit;
    mit = m_mapPetInfo.find( dwPetUID_ );
    if( mit == m_mapPetInfo.end() )
        return false;

    m_mapPetInfo.erase( mit );

    START_LOG( clog, L"펫을 삭제 한다.." )
        << BUILD_LOG( m_strName )
        << BUILD_LOG( dwPetUID_ )
        << BUILD_LOG( m_mapPetInfo.size() ) << END_LOG;

    return true;
}

bool KUser::ReleasePetEquipElement( DWORD dwUID_, char cItemType_ )
{
    bool bRet = false;
    std::map<GCITEMUID, KPetInfo>::iterator mit;
    for( mit = m_mapPetInfo.begin(); mit != m_mapPetInfo.end(); ++mit )
    {
        // 펫별로 다 검사
        KPetInfo& kPetInfo = mit->second;
        std::vector<KSimpleItem>::iterator vit;
        for( vit = kPetInfo.m_vecEquipItem.begin() ; vit != kPetInfo.m_vecEquipItem.end() ; ++vit )
        {
            if( vit->m_cItemType == cItemType_ &&
                vit->m_dwUID == dwUID_ )
            {
                kPetInfo.m_vecEquipItem.erase( vit );
                bRet = true;
                break;
            }
        }
    }
    return bRet;
}

void KUser::GetPetCacheData( OUT std::map<GCITEMUID, KPetInfo>& mapPetInfo_ )
{
    mapPetInfo_.clear();

    std::map< GCITEMUID, KPetInfo >::iterator mit;
    for( mit = m_mapPetInfo.begin() ; mit != m_mapPetInfo.end() ; ++mit )
    {
        mapPetInfo_.insert( std::make_pair( mit->first, mit->second ) );
        std::map<char,DWORD>::iterator mit2;
        mit2 = mit->second.m_mapInitExp.find( mit->second.m_cPromotion );
        if( mit2 != mit->second.m_mapInitExp.end() )
        {
            mit2->second = mit->second.m_dwEXP;
        }
        else
        {
            START_LOG( cerr, L" 펫 경험치 업데이트중 현재 진화 상태의 초기 Exp가 없음. Name : " << m_strName )
                << BUILD_LOG( mit->second.m_dwID )
                << BUILD_LOG( mit->second.m_dwUID )
                << BUILD_LOG( mit->second.m_strName )
                << BUILD_LOGc( mit->second.m_cPromotion )
                << BUILD_LOG( mit->second.m_dwEXP )
                << BUILD_LOG( mit->second.m_mapInitExp.size() ) << END_LOG;
        }

        mit->second.m_iInitSatiation    = mit->second.m_iSatiation;
    }
}

void KUser::GetDepotPetCacheData( OUT std::map<GCITEMUID, KPetInfo>& mapDepotPetInfo_ )
{
    mapDepotPetInfo_.clear();

    std::map< GCITEMUID, KPetInfo >::iterator mit;
    for ( mit = m_mapDepotPetInfo.begin(); mit != m_mapDepotPetInfo.end(); ++mit ) {

        mapDepotPetInfo_.insert( std::make_pair( mit->first, mit->second ) );

        std::map<char,DWORD>::iterator mit2;
        mit2 = mit->second.m_mapInitExp.find( mit->second.m_cPromotion );

        if ( mit2 != mit->second.m_mapInitExp.end() ) {
            mit2->second = mit->second.m_dwEXP;
        }
        else {
            START_LOG( cerr, L" 펫 경험치 업데이트중 현재 진화 상태의 초기 Exp가 없음. Name : " << m_strName )
                << BUILD_LOG( mit->second.m_dwID )
                << BUILD_LOG( mit->second.m_dwUID )
                << BUILD_LOG( mit->second.m_strName )
                << BUILD_LOGc( mit->second.m_cPromotion )
                << BUILD_LOG( mit->second.m_dwEXP )
                << BUILD_LOG( mit->second.m_mapInitExp.size() ) << END_LOG;
        }

        mit->second.m_iInitSatiation    = mit->second.m_iSatiation;
    }
}

void KUser::ReleasePetFromChar( IN GCITEMUID dwPetUID_ )
{
    std::map<char, KCharacterInfo>::iterator mit;
    for ( mit = m_mapCharacterInfo.begin(); mit != m_mapCharacterInfo.end(); ++mit ) {
        if ( mit->second.m_kEquipPetInfo.m_dwUID == dwPetUID_ ) {
            mit->second.m_kEquipPetInfo.Init();
        }
    }
}

void KUser::ReleasePetCostume( IN GCITEMUID dwItemUID_ )
{
    std::map< GCITEMUID, KPetInfo >::iterator mit;
    for( mit = m_mapPetInfo.begin() ; mit != m_mapPetInfo.end() ; ++mit )
    {
        if( mit->second.m_kCostume.m_dwUID == dwItemUID_ )
        {
            mit->second.m_kCostume.m_dwID       = 0;
            mit->second.m_kCostume.m_dwUID      = 0;
            mit->second.m_kCostume.m_cItemType  = -1;
        }
    }
}

bool KUser::CheckGameMode( int nGameMode_ )
{
    _JIF( m_pkChannel, return false );
    _JIF( m_pkChannel != KChannel::ms_spChannelLobby.get(), return false );

    std::vector<int> vecModeList;
    m_pkChannel->GetModeList( vecModeList );

    if( vecModeList.empty() )
        return true;

    if( std::find( vecModeList.begin(), vecModeList.end(), nGameMode_ ) == vecModeList.end() )
    {
        START_LOG( cerr, L"등록되어있지 않은 게임 모드입니다 GameMode : " << nGameMode_ ) << END_LOG;
        return false;
    }
    return true;
}

void KUser::UpdateAccTime()
{
    if ( SiKGCPoint()->IsGCPointSwitch() == false ) {// Not using GCPoint -> return
        return;
    }
    if ( m_bGMuser ) {
        return;
    }
    
    if ( ::GetTickCount() - GetTick( UPDATE_ACC_TICK ) > SiKGCPoint()->GetAccUpdateTime() ) {
        int ret = m_kCalendar.Update();
        SetTick( UPDATE_ACC_TICK );
        if( ret ==  KCalendar::UR_GETFAIL ) {
            QUEUEING_ID_TO_DB( EVENT_LOAD_POINTSYSTEM_INFO_REQ ); // 달력 정보 못가져 왔을 경우 다시 가져오기.
        }
        //달력을 새로 가져와야 하는지 확인하고
        if ( ret == KCalendar::UR_RELOAD ) {
            std::vector<KDailyInfo> vecDailyInfo;
            m_kCalendar.GetCalendarDBInfo( vecDailyInfo );
            QUEUEING_EVENT_TO_DB( EVENT_LOAD_NEW_CALENDAR_NOT, vecDailyInfo );
        }

        if ( ret == KCalendar::UR_RELOAD || ret == KCalendar::UR_NEXTDAY ) {
            CheckBonusRefill();
            UpdateGachaKeyBuyCount( 0, 0 ); // Key 구매 카운트 갱신 요청.
            if ( IsCouple() ) {
                ClearCoupleDenyInfo();
            }
            m_bLunaNewYearEventDone = false;
            SendNpcGiftsNot();
            SEND_ID( EVENT_INIT_TODAYS_POPUP_NOT );
        }

        //누적 시간을 전달 한다. 오늘의 정보 보내준다
        KEVENT_ACC_TIME_NOT kPacket;
        kPacket.m_nAccTime = m_kCalendar.GetAccTime();
        m_kCalendar.GetToday( kPacket.m_kToday );
        SEND_PACKET( EVENT_ACC_TIME_NOT );

        START_LOG( clog, L"Acc Time Update.. : " << m_strName )
            << BUILD_LOG( kPacket.m_nAccTime )
            << BUILD_LOG( kPacket.m_kToday.m_sYear )
            << BUILD_LOGc( kPacket.m_kToday.m_cMonth )
            << BUILD_LOGc( kPacket.m_kToday.m_cDay ) << END_LOG;

        if ( SiKDicePlay()->IsRun() ) {
            KUserBoardInfo kUserBoardInfo;
            kUserBoardInfo.Clear();
            GetUserBoardInfo( kUserBoardInfo );
            CheckUserBoardInfo( kUserBoardInfo ); // 주사위 이벤트 유저의 정보 계산.
        }

        bool bChangeDate = ( ret == KCalendar::UR_RELOAD || ret == KCalendar::UR_NEXTDAY );

        // 0623. nodefeat.
        // 일단 서버 날짜 지나면 db에 lastlogin 갱신하도록...
        if ( bChangeDate ) {
            if ( false == m_bFirstLoginToday ) { // 자정이 지나면 첫접속으로 간주한다.
                m_bFirstLoginToday = true;
            }
            QUEUEING_ID_TO_DB( EVENT_LOGIN_TIME_UPDATE_REQ );
        }

        if ( SiKQuizBingo()->IsEventTerm() ) {
            KEVENT_BINGO_COIN_CHARGE_NOT kNotPacket;
            if ( m_kQuizBingo.UpdateAccTime( GetUID(), m_strName, GetCurrentCharType(),bChangeDate, kPacket.m_kToday, kNotPacket ) ) {
                SEND_DATA( EVENT_BINGO_COIN_CHARGE_NOT, kNotPacket );
                START_LOG( clog, L"시간이 되어서 코인 충전 패킷을 보냄. Name : " << m_strName )
                    << BUILD_LOG( GetUID() )
                    << BUILD_LOG( kNotPacket.m_bFinishToday )
                    << BUILD_LOG( kNotPacket.m_nAccTime )
                    << BUILD_LOG( kNotPacket.m_nCoinCount ) << END_LOG;
            }
        }

        if ( SiKSongkran()->IsRun() && true == m_KSongkranUserData.IsLoaded() ) { // 송크란 이벤트 물풍선 지급
            KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT kNotPacket;
            if ( m_KSongkranUserData.UpdateAccTime( GetUID(), m_strName, bChangeDate, kPacket.m_kToday, kNotPacket ) ) {
                if ( KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT::PERIOD_SUPPLY == kNotPacket.m_nOK ) {  // 주기 지급
                    START_LOG( clog, L"시간이 되어서 물풍 충전 패킷을 보냄. Name : " << m_strName )
                        << BUILD_LOG( GetUID() )
                        << BUILD_LOG( kNotPacket.m_nOK )
                        << BUILD_LOG( kNotPacket.m_dwWaterBombDuration )
                        << END_LOG;
                }
                else if ( KEVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT::PERIOD_SUPPLY_LAST == kNotPacket.m_nOK ) { // 금일 지급 종료 ( 지급을 하면서 마지막이라고 알림 )
                    START_LOG( clog, L"물풍선 금일 지급 종료 메세지 전송.(종료) Name : " << m_strName )
                        << BUILD_LOG( GetUID() )
                        << BUILD_LOG( kNotPacket.m_nOK )
                        << BUILD_LOG( kNotPacket.m_dwWaterBombDuration )
                        << END_LOG;
                }
                SendSongkranWaterBombNot( m_KSongkranUserData.GetSuppliedWaterBombDuration(), SiKSongkran()->GetWaterBombSupplyDuration() );
                SEND_DATA( EVENT_SONGKRAN_WATERBOMB_SUPPLY_NOT, kNotPacket );
            }
        }

        if ( SiKTimeDrop()->IsEventTerm() ) {
            // 내부 처리함
            m_kTimeDrop.CheckTimeDrop( GetUID(), GetName(), GetCurrentCharType(), IsPCBangUser(), bChangeDate );
        }

        CheckRitasChristmasStartCoinSupply();   // 리타의 크리스마스 이벤트 시작코인 지급 확인

        if ( true == bChangeDate ) {
            SEND_ID( EVENT_HERO_DUNGEON_RESET_NOT );
            START_LOG( clog, L"영웅던전 초기화 알림. LoginID : " << GetName() ) << END_LOG;

            QUEUEING_ID_TO_DB( EVENT_TODAYS_SEED_DROP_COUNT_REQ );
            START_LOG( clog, L"날짜변경후, 일일 획득한 씨앗개수 DB에 요청. LoginID : " << GetName() ) << END_LOG;
        }

        UpdateGawibawiboPoint(); // 가위바위보 포인트 누적
        InitGawibawiboData( ret );
        SendUserConnectNoticeToAgentMaster();

        CheckConnectionGiftBoxAccTime(); // 접속 선물상자 누적시간 증가 및 보상 획득가능 여부 확인
    } // if ( ::GetTickCount() - GetTick( UPDATE_ACC_TICK ) >SiKGCPoint()->GetAccUpdateTime() ) {

    UpdateUserAuthLevel();
}

bool KUser::ChkEquipExist( IN std::vector<KEquipItemInfo>& vecEquip_ )
{
    std::vector<KEquipItemInfo>::iterator vit;
    for( vit = vecEquip_.begin() ; vit != vecEquip_.end() ; ++vit )
    {
        if ( m_kInventory.FindItem( vit->m_dwID, vit->m_dwUID ) == NULL ) {
            START_LOG( cerr, L"장착 하려는 아이템이 내 인벤토리에없음. Name : " << GetName() )
                << BUILD_LOG( vit->m_dwID )
                << BUILD_LOG( vit->m_dwUID ) << END_LOG;
            return false;
        }
    }
    return true;
}

int KUser::CheckValidationReleaseReq( IN OUT std::map<char,KChangeEquipment>& mapEquipReq_ )
{
    std::map<char,KChangeEquipment>::iterator mit;
    int nCount = 0;
    for( mit = mapEquipReq_.begin() ; mit != mapEquipReq_.end() ; ++mit )
    {
        const KCharacterInfo* pkChar = GetCharacterInfo( mit->first );
        if( !pkChar ) continue;

        std::vector< KEquipItemInfo >::iterator vit;
        for( vit = mit->second.m_vecReleaseItems.begin() ; vit != mit->second.m_vecReleaseItems.end() ; )
        {
            std::vector<KEquipItemInfo>::const_iterator cvitEquip;
            cvitEquip = std::find_if( pkChar->m_vecEquipItems.begin(), pkChar->m_vecEquipItems.end(),
                boost::bind( &KEquipItemInfo::m_dwID, _1 ) == vit->m_dwID );
            if( cvitEquip == pkChar->m_vecEquipItems.end() )
            {
                vit = mit->second.m_vecReleaseItems.erase( vit );
                ++nCount;
                continue;
            }
            ++vit;
        }
    }
    return nCount;
}

int KUser::CheckValidationEquipReq( IN OUT std::map<char,KChangeEquipment>& mapEquipReq_ )
{
    std::map<char,KChangeEquipment>::iterator mit;
    int nCount = 0;
    for( mit = mapEquipReq_.begin() ; mit != mapEquipReq_.end() ; ++mit )
    {
        std::vector< KEquipItemInfo >::iterator vit;
        for( vit = mit->second.m_vecEquipItems.begin() ; vit != mit->second.m_vecEquipItems.end() ; )
        {
            KItemPtr kItem = m_kInventory.FindItem( vit->m_dwID, vit->m_dwUID );
            if ( kItem == NULL ) {
                vit = mit->second.m_vecEquipItems.erase( vit );
                ++nCount;
                continue;
            }

            vit->m_vecAttribute = kItem->m_vecAttribute;
            vit->m_vecSocket = kItem->m_vecSocket;
            vit->m_cGradeID = kItem->m_cGradeID;
            vit->m_nEquipLevel = static_cast<int>( kItem->m_sEquipLevel );
            vit->m_cEnchantLevel = kItem->m_cEnchantLevel;
            vit->m_cEnchantEquipGradeID = kItem->m_cEnchantEquipGradeID;
            vit->m_DesignCoordiID = kItem->m_DesignCoordiID;
            ++vit;
        }
    }
    return nCount;
}

bool KUser::CopyEquipReq(  IN std::map<char,KChangeEquipment>& mapEquipReq_,
                      OUT std::map<char,std::vector<KEquipItemInfo> >& mapEquipCopy_ )
{
    // 여기서는 내 인벤토리 확인은 않하고. 바로 제거와 
    
    //장착 정보를 캐릭터 정보에서 복사 해온다
    std::map<char,KCharacterInfo>::const_iterator cmitChar;
    for ( cmitChar = m_mapCharacterInfo.begin() ; cmitChar != m_mapCharacterInfo.end() ; ++cmitChar ) {
        if ( cmitChar->first != GetCurrentCharType() ) { // 캐릭터 인벤토리 분리 작업으로 현재 캐릭터 외에는 체크하지 않기 위해서
            continue;
        }
        mapEquipCopy_[cmitChar->first] = cmitChar->second.m_vecEquipItems;
    }
 
    // 해제 해야할 아이템을 해제 시킨다.
    std::map<char,KChangeEquipment>::iterator mitReq; 
    for( mitReq = mapEquipReq_.begin() ; mitReq != mapEquipReq_.end() ; ++mitReq )
    {
        if ( mitReq->first != GetCurrentCharType() ) { // 캐릭터 인벤토리 분리 작업으로 현재 캐릭터 외에는 체크하지 않기 위해서
            continue;
        }
        std::map<char,std::vector<KEquipItemInfo> >::iterator mitCharCopy;
        mitCharCopy = mapEquipCopy_.find( mitReq->first );
        if( mitCharCopy == mapEquipCopy_.end() )
        {
            START_LOG( cerr, L"없는 캐릭터의 장착 해제 요청, Name : " << m_strName )
                << BUILD_LOGc( mitReq->first ) << END_LOG;
            return false;
        }
        //mitCharCopy 사본, mitReq->second.m_vecRelease 요청본
        // 사본에서 요청본을 제거한다.
        std::vector<KEquipItemInfo>::iterator vitReqRel;
        for( vitReqRel = mitReq->second.m_vecReleaseItems.begin() ; vitReqRel != mitReq->second.m_vecReleaseItems.end() ; ++vitReqRel )
        {
            std::vector<KEquipItemInfo>::iterator vitReqEle;
            vitReqEle = std::find_if( mitCharCopy->second.begin(), mitCharCopy->second.end(),
                boost::bind( &KEquipItemInfo::m_dwID, _1 ) == vitReqRel->m_dwID );
            if( vitReqEle == mitCharCopy->second.end() )
            {
                START_LOG( cerr, L"해제하려는 아이템이 내 캐릭터 정보에 없음, Name :" << m_strName )
                    << BUILD_LOGc( mitReq->first )
                    << BUILD_LOG( vitReqRel->m_dwID )
                    << BUILD_LOG( vitReqRel->m_dwUID ) << END_LOG;
                return false;
            }
            mitCharCopy->second.erase( vitReqEle );
        }

        //mitCharCopy 사본, mitReq->second.m_vecEquip 요청본
        // 사본에서 요청본을 추가 한다.
        std::vector<KEquipItemInfo>::iterator vitReqEqui;
        for( vitReqEqui = mitReq->second.m_vecEquipItems.begin() ; vitReqEqui != mitReq->second.m_vecEquipItems.end() ; ++vitReqEqui )
        {
            std::vector<KEquipItemInfo>::iterator vitReqEle;
            vitReqEle = std::find_if( mitCharCopy->second.begin(), mitCharCopy->second.end(),
                boost::bind( &KEquipItemInfo::m_dwID, _1 ) == vitReqEqui->m_dwID );

            if( vitReqEle != mitCharCopy->second.end() )
            {
                START_LOG( cwarn, L"장착하려는 아이템이 내 캐릭터가 이미 장착하고 있음, Name : " << m_strName )
                    << BUILD_LOGc( mitReq->first )
                    << BUILD_LOG( vitReqEqui->m_dwID )
                    << BUILD_LOG( vitReqEqui->m_dwUID ) << END_LOG;
                return false;
            }

            mitCharCopy->second.push_back( *vitReqEqui );
        }
    }
    return true;
}

bool KUser::ApplyEquipItem( IN std::map<char,std::vector<KEquipItemInfo> >& mapEquipCopy_, IN std::map<char,KChangeEquipment>& mapEquipReq_ )
{
    std::map<char,std::vector<KEquipItemInfo> >::iterator mit;

    for( mit = mapEquipCopy_.begin() ; mit != mapEquipCopy_.end() ; ++mit )
    {
        if ( mit->first != GetCurrentCharType() ) { // 캐릭터 인벤토리 분리 작업으로 현재 캐릭터 외에는 체크하지 않기 위해서
            continue;
        }
        //해당 캐릭터의 전직 상태를 알아 와야 한다.
        const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( mit->first );
        _JIF( pkCharacterInfo, return false );

        // 무기를 내 전직 레벨에 맞냐? 무기를 여러개 차고 있냐?
        char cCurrentJobLV;
        SiKGameServer()->ChkValidWeapon( mit->second, mit->first, pkCharacterInfo->m_cPromotion, cCurrentJobLV );
        if( NetError::GetLastNetError() != NetError::NET_OK )
        {
            START_LOG( cerr, L"무기 장착 전직 에러 , m_strName : " << m_strName )
                << BUILD_LOG( NetError::GetLastNetError() )
                << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
            //로그를 남기고 리턴한다..
            return false;
        }

        // 장착 중인 아이템이 내 인벤토리에 있냐?
        if( !ChkEquipExist( mit->second ) )
        {
            START_LOG( cerr, L"장착중인 아이템이 내 인벤토리에 없음, Name : " << m_strName )
                << BUILD_LOG( mit->second.size() ) << END_LOG;
            return false;
        }

        // 현재 전직 선택된 전직 레벨을 요청 패킷에 간단히 셋팅.. 
        std::map<char,KChangeEquipment>::iterator mitReq;
        mitReq = mapEquipReq_.find( mit->first );
        if( mitReq == mapEquipReq_.end() )
        {
            START_LOG( cerr, L"선택된 전직이 내 전직 레벨에 없음, Name : " << m_strName )
                << BUILD_LOGc( mit->first ) << END_LOG;
            continue;
        }

        mitReq->second.m_cCurrentJobLV = cCurrentJobLV;
    }

    // 펫 관련 변경 사항.. 체크
    std::map<char, KChangeEquipment>::iterator mitReq;
    for( mitReq = mapEquipReq_.begin() ; mitReq != mapEquipReq_.end() ; ++mitReq )
    {
        if ( mitReq->first != GetCurrentCharType() ) { // 캐릭터 인벤토리 분리 작업으로 현재 캐릭터 외에는 체크하지 않기 위해서
            continue;
        }

        _JIF( IsCharExist( mitReq->first ), return false );

        if( mitReq->second.m_equipPetInfo.m_dwUID == 0 )
        {
            mitReq->second.m_equipPetInfo.Init();
            continue;
        }

        if( mitReq->second.m_equipPetInfo.m_cCharType != -1 && mitReq->second.m_equipPetInfo.m_cCharType != mitReq->first ) {
            START_LOG( cerr, L"해당 캐릭터의 종속된 펫이 아님, Name : " << m_strName )
                << BUILD_LOGc( mitReq->first ) << END_LOG;
            return false;
        }

        KPetInfo* pkPetInfo = NULL;
        if( !GetPetInfo( mitReq->second.m_equipPetInfo.m_dwUID, &pkPetInfo ) )
        {
            START_LOG( cerr, L"장착 해제 적용에서 펫정보 얻기 에러, Name : " << m_strName )
                << BUILD_LOGc( mitReq->first )
                << BUILD_LOG( mitReq->second.m_equipPetInfo.m_dwUID ) << END_LOG;                
            return false;
        }

        {
            // 보내온 펫의 장착 요청 정보가 올바른지 확인한다.
            std::vector<KSimpleItem>::iterator vit;
            for( vit = mitReq->second.m_equipPetInfo.m_vecEquipItem.begin() ; vit != mitReq->second.m_equipPetInfo.m_vecEquipItem.end() ;  )
            {
                if ( m_kInventory.FindItem( vit->m_dwID, vit->m_dwUID ) == NULL ) {
                    START_LOG( cwarn, L"펫이 장착하고 있는 아이템이 내 인벤토리에 없음, Name : " << m_strName )
                        << BUILD_LOGc( vit->m_cItemType )
                        << BUILD_LOG( vit->m_dwUID )
                        << BUILD_LOG( vit->m_dwID ) << END_LOG;
                    vit = mitReq->second.m_equipPetInfo.m_vecEquipItem.erase( vit );
                    continue;
                }
                ++vit;
            }
        }
        // 펫 코스튬..
        if( SiKPetHelper()->IsPetCostume( pkPetInfo->m_dwID, pkPetInfo->m_cPromotion, mitReq->second.m_equipPetInfo.m_kCostume.m_dwID ) &&
            m_kInventory.FindItemByItemUID( mitReq->second.m_equipPetInfo.m_kCostume.m_dwUID ) != NULL )
        {
            pkPetInfo->m_kCostume = mitReq->second.m_equipPetInfo.m_kCostume;
            START_LOG( clog, L"펫 코스튬 장착 성공 " )
                << BUILD_LOG( pkPetInfo->m_dwID )
                << BUILD_LOGc( pkPetInfo->m_cPromotion )
                << BUILD_LOG( mitReq->second.m_equipPetInfo.m_kCostume.m_dwID )
                << BUILD_LOG( mitReq->second.m_equipPetInfo.m_kCostume.m_dwUID ) << END_LOG;
        }
        else if( mitReq->second.m_equipPetInfo.m_kCostume.m_dwID == 0 )
        {
            pkPetInfo->m_kCostume = mitReq->second.m_equipPetInfo.m_kCostume;
            START_LOG( clog, L"펫 코스튬 해제" )
                << BUILD_LOG( pkPetInfo->m_dwID )
                << BUILD_LOGc( pkPetInfo->m_cPromotion )
                << BUILD_LOG( mitReq->second.m_equipPetInfo.m_kCostume.m_dwID )
                << BUILD_LOG( mitReq->second.m_equipPetInfo.m_kCostume.m_dwUID ) << END_LOG;
        }
        else
        {
            START_LOG( cerr, L"펫 코스튬 장착 실패 " )
                << BUILD_LOG( pkPetInfo->m_dwID )
                << BUILD_LOGc( pkPetInfo->m_cPromotion )
                << BUILD_LOG( mitReq->second.m_equipPetInfo.m_kCostume.m_dwID )
                << BUILD_LOG( mitReq->second.m_equipPetInfo.m_kCostume.m_dwUID ) << END_LOG;
        }

        mitReq->second.m_equipPetInfo.m_dwUID           = pkPetInfo->m_dwUID;
        mitReq->second.m_equipPetInfo.m_dwID            = pkPetInfo->m_dwID;
        mitReq->second.m_equipPetInfo.m_strName         = pkPetInfo->m_strName;
        mitReq->second.m_equipPetInfo.m_mapInitExp      = pkPetInfo->m_mapInitExp;
        mitReq->second.m_equipPetInfo.m_dwEXP           = pkPetInfo->m_dwEXP;
        mitReq->second.m_equipPetInfo.m_iLevel          = pkPetInfo->m_iLevel;
        mitReq->second.m_equipPetInfo.m_cPromotion      = pkPetInfo->m_cPromotion;
        mitReq->second.m_equipPetInfo.m_nHatchingID     = pkPetInfo->m_nHatchingID;
        mitReq->second.m_equipPetInfo.m_iInitSatiation  = pkPetInfo->m_iInitSatiation;
        mitReq->second.m_equipPetInfo.m_iSatiation      = pkPetInfo->m_iSatiation;
        mitReq->second.m_equipPetInfo.m_kCostume        = pkPetInfo->m_kCostume;
    }

    //적용해보자..
    for( mitReq = mapEquipReq_.begin() ; mitReq != mapEquipReq_.end() ; ++mitReq )
    {
        if ( mitReq->first != GetCurrentCharType() ) { // 캐릭터 인벤토리 분리 작업으로 현재 캐릭터 외에는 체크하지 않기 위해서
            continue;
        }

        KCharacterInfo* pkCharacterInfo = GetCharacterInfo( mitReq->first );
        if( pkCharacterInfo == NULL )
        {
            START_LOG( cerr, L" 캐릭터의 장착 정보를 적용하는 시점에서 해당 캐릭터가 존재하지 않음. Name : " << m_strName 
                << L", Char Type : " << (int)(mitReq->first) ) << END_LOG;
            continue;
        }

        // mit는 제일 위에 선언 되어 있음..
        mit = mapEquipCopy_.find( mitReq->first );
        if( mit == mapEquipCopy_.end() )
        {
            START_LOG( cerr, L"장착 정보 사본 적용시에 해당 캐릭터 정보가 사본에 없음.. Name : " << m_strName 
                << L", Char Type : " << (int)(mitReq->first) ) << END_LOG;
            continue;
        }
        pkCharacterInfo->m_vecEquipItems = mit->second;
        pkCharacterInfo->m_cCurrentPromotion = mitReq->second.m_cCurrentJobLV;

        KPetInfo* pkPetInfo = NULL;
        if( mitReq->second.m_equipPetInfo.m_dwUID != 0 &&
            GetPetInfo( mitReq->second.m_equipPetInfo.m_dwUID, &pkPetInfo ) )
        {
            *pkPetInfo = mitReq->second.m_equipPetInfo;
            pkCharacterInfo->m_kEquipPetInfo = mitReq->second.m_equipPetInfo;
        }
        else
        {
            pkCharacterInfo->m_kEquipPetInfo.Init();
        }
    }

    return true;
}

const KItem* KUser::FindItemByID( IN const std::vector<KItem>& vecItem_, GCITEMID dwItemID_ )
{
    std::vector<KItem>::const_iterator vit;

    vit = std::find_if( vecItem_.begin(), vecItem_.end(),
        boost::bind( &KItem::m_ItemID, _1 ) == dwItemID_ );
    if( vit != vecItem_.end() )
        return &(*vit);

    return NULL;
}

KMissionData* KUser::GetMissionDataFromSlot( DWORD dwMissionID_ )
{
    std::vector<KMissionData>::iterator vit;
    vit = std::find_if( m_vecMissionSlot.begin(), m_vecMissionSlot.end(),
        boost::bind( &KMissionData::m_dwMissionID, _1 ) == dwMissionID_ );

    if( vit == m_vecMissionSlot.end() )
        return NULL;
    return &(*vit);
}

bool KUser::IsRegistedMission( DWORD dwMissionID_ )
{
    return NULL != GetMissionDataFromSlot( dwMissionID_ );
}

bool KUser::EraseMissionDataFromSlot( DWORD dwMissionID_ )
{
    std::vector<KMissionData>::iterator vit;
    vit = std::find_if( m_vecMissionSlot.begin(), m_vecMissionSlot.end(),
        boost::bind( &KMissionData::m_dwMissionID, _1 ) == dwMissionID_ );
    if( vit == m_vecMissionSlot.end() )
        return false;

    m_vecMissionSlot.erase( vit );
    return true;
}

bool KUser::GetMissionListOfType( IN const int& nMissionType_, IN const char& cCharType_, OUT std::vector<DWORD>& vecMissionList_ )
{
    vecMissionList_.clear();

    if ( m_vecMissionSlot.empty() )
        return true;

    std::vector<KMissionData>::iterator vit = m_vecMissionSlot.begin();
    std::vector<KMissionData>::iterator vit_end = m_vecMissionSlot.end();

    KMissionInfo kMissionInfo;

    for ( ; vit != vit_end; ++vit )
    {
        if ( !SiKMissionManager()->GetMissionInfo( kMissionInfo, vit->m_dwMissionID ) ) 
            continue;

        if ( kMissionInfo.m_cCharType != cCharType_ )
            continue;

        if ( kMissionInfo.m_cMissionType != nMissionType_ ) 
            continue;
        
        vecMissionList_.push_back( vit->m_dwMissionID );        
    }

    return true;
}

void KUser::GetInDoorCharInfo( std::vector<KInDoorCharInfo>& vecCharInfo_ )
{
    vecCharInfo_.clear();

    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {

        KInDoorCharInfo kInDoorCharInfo;
        kInDoorCharInfo.m_cCharType             = cmit->second.m_cCharType;
        kInDoorCharInfo.m_strCharName           = cmit->second.m_strCharName;
        kInDoorCharInfo.m_cPromotion            = cmit->second.m_cPromotion;
        kInDoorCharInfo.m_cCurrentPromotion     = cmit->second.m_cCurrentPromotion;
        kInDoorCharInfo.m_biExp                 = cmit->second.m_biExp;
        kInDoorCharInfo.m_dwLevel               = cmit->second.m_dwLevel;
        kInDoorCharInfo.m_iWin                  = cmit->second.m_iWin;
        kInDoorCharInfo.m_iLose                 = cmit->second.m_iLose;
        kInDoorCharInfo.m_vecEquipItems         = cmit->second.m_vecEquipItems;
        kInDoorCharInfo.m_kSkillInfo            = cmit->second.m_kSkillInfo;
        kInDoorCharInfo.m_kChangeWeaponItem     = cmit->second.m_kChangeWeaponItem;
        kInDoorCharInfo.m_dwSlotNum             = cmit->second.m_dwSlotNum;
        kInDoorCharInfo.m_nGamePoint            = cmit->second.m_nGamePoint;
        kInDoorCharInfo.m_kBonusPoint.m_nBaseBonus = m_mapBaseBonus[kInDoorCharInfo.m_cCharType];//           = cmit->second.m_kBonusPoint;
        kInDoorCharInfo.m_kBonusPoint.m_nSpecialBonus = m_mapSpecialBonus[kInDoorCharInfo.m_cCharType]; // User 에서 관리하고 있다.
        DumpEquipItems( (int)cmit->first, kInDoorCharInfo.m_kChangeWeaponItem, kInDoorCharInfo.m_vecEquipItems );

        kInDoorCharInfo.m_vecLookEquips = cmit->second.m_vecLookEquips;

        // 기본으로 펫이 장착되지 않은 상태로 셋팅.
        kInDoorCharInfo.m_kPetInfo = cmit->second.m_kEquipPetInfo;
        GetSkillSettingData( cmit->second.m_cCharType, kInDoorCharInfo.m_mapEquipSkillSet );
        if( SiKSkillManager()->CheckAbilitySkillChar( cmit->second.m_cCharType ) ) {
            GetSkillInfoData( cmit->second.m_cCharType, kInDoorCharInfo.m_vecSPInfo );
        }

        kInDoorCharInfo.m_kELOUserData = cmit->second.m_kELOUserData;

        kInDoorCharInfo.m_vecPetGlyphCharData.clear();
        std::vector<KPetGlyphCharData>::const_iterator cit = cmit->second.m_vecPetGlyphCharData.begin();

        for (; cit != cmit->second.m_vecPetGlyphCharData.end(); ++cit)
        {
            kInDoorCharInfo.m_vecPetGlyphCharData.push_back(*cit);
        }

        kInDoorCharInfo.m_dwPetGlyphSize = cmit->second.m_dwPetGlyphSize;

        vecCharInfo_.push_back( kInDoorCharInfo );
    }
}

// 요소를 받아 개별로 삭제하는 함수?  , 아이템 타입 인자로 넘겨주고.
bool KUser::ReleaseEquipElement( DWORD dwUID_, char cItemType_ )
{
    bool bRet = false;

    std::map<char, KCharacterInfo>::iterator mit;
    for ( mit = m_mapCharacterInfo.begin() ; mit != m_mapCharacterInfo.end() ; ++mit ) {

        std::vector<KEquipItemInfo>::iterator vit;
        for ( vit = mit->second.m_vecEquipItems.begin() ; vit != mit->second.m_vecEquipItems.end() ; ++vit ) {

            if ( vit->m_dwUID == dwUID_ ) {
                mit->second.m_vecEquipItems.erase( vit );
                bRet = true;
                break;
            }
        }
    }

    return bRet;
}

bool KUser::IsExistEquipItem( IN char cCharType_, IN GCITEMID dwItemID_ )
{
    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( cCharType_ );
    if( !pkCharacterInfo )
        return false;

    return ( std::find_if( pkCharacterInfo->m_vecEquipItems.begin(), pkCharacterInfo->m_vecEquipItems.end(),
           boost::bind( &KEquipItemInfo::m_dwID, _1 ) == dwItemID_ ) != pkCharacterInfo->m_vecEquipItems.end() );
}

bool KUser::IsExistEquipItem( IN char cCharType_, IN const std::map<char, std::set<DWORD> >& mapEquips_ )
{
    const KCharacterInfo* pkCharacterInfo = GetCharacterInfo( cCharType_ );
    if( !pkCharacterInfo )
        return false;

    std::map<char, std::set<DWORD> >::const_iterator mit;
    mit = mapEquips_.find( cCharType_ );
    if( mit == mapEquips_.end() )
    {
        mit = mapEquips_.find( (char)(-1) );
        if( mit == mapEquips_.end() )
            return false;
    }

    std::set<DWORD>::const_iterator scit;
    for( scit = mit->second.begin() ; scit != mit->second.end() ; ++scit )
    {
        if( std::find_if( pkCharacterInfo->m_vecEquipItems.begin(), pkCharacterInfo->m_vecEquipItems.end(),
            boost::bind( &KEquipItemInfo::m_dwID, _1 ) == *scit ) == pkCharacterInfo->m_vecEquipItems.end() )
        {
            // 하나라도 없으면 false 처리
            return false;
        }
    }

    return true;
}

bool KUser::IsEquipedItem( IN const GCITEMUID ItemUID_ )
{
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {

        if ( std::find_if( cmit->second.m_vecEquipItems.begin(), cmit->second.m_vecEquipItems.end(),
            boost::bind( &KEquipItemInfo::m_dwUID, _1 ) == ItemUID_ ) != cmit->second.m_vecEquipItems.end() ) {
            return true;
        }
    }

    return false;
}

bool KUser::RefreshEquipItem( IN const KItem& pkRefreshItem_ )
{
    bool bRefreshed = false;

    std::map< char, KCharacterInfo >::iterator mit;
    for ( mit = m_mapCharacterInfo.begin() ; m_mapCharacterInfo.end() != mit; ++mit ) {
        std::vector< KEquipItemInfo >::iterator vit;
        vit = std::find_if( mit->second.m_vecEquipItems.begin(), mit->second.m_vecEquipItems.end(),
            boost::bind( &KEquipItemInfo::m_dwUID, _1 ) == pkRefreshItem_.m_ItemUID );

        if ( mit->second.m_vecEquipItems.end() != vit ) {
            *vit = pkRefreshItem_;

            bRefreshed = true;
        }
    }

    return bRefreshed;
}

void KUser::CheckEvents( OUT KEVENT_START_GAME_BROAD& kPacket_ )
{
    kPacket_.m_mapEvents.clear();
    JIF( m_pkRoom );
    m_pkRoom->ClearTimeEvent();
    KGameInfo kInfo;
    std::vector<KSimpleInDoorUserInfo> vecStartingInfo;
    std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> > mapActiveEventList;

    m_pkRoom->GetGameInfo( kInfo );
    m_pkRoom->GetStartingInfo( vecStartingInfo );
    if( SiKTimeEventManager()->GetCorrectEvents( kInfo, vecStartingInfo, mapActiveEventList ) )
    {
        m_pkRoom->SetTimeEvent( mapActiveEventList );
    }
    // 이터레이션 하면서 이벤트를 넣어 주자.. 패킷을 다 가져 오는것은 오버인가??
    std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >::iterator mit;
    for( mit = mapActiveEventList.begin() ; mit != mapActiveEventList.end() ; ++mit )
    {
        std::vector<sTimeEvent>::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            // 중복을 막기 위해서.
            kPacket_.m_mapEvents[ mit->first.first ].insert( vit->m_nEventID );
            //kPacket_.m_mapEvents.insert( std::make_pair() );
            //kPacket_.m_setEvents.insert( vit->m_nEventID );
        }
    }

    // 09.04.13. kkurrung. 추천(5th)
    if ( SiKRecomHelper()->IsLimitEventTerm() && m_MyRecomInfo.m_nState != KRecomHelper::RBS_WAIT_RECOM ) {        
        std::vector<std::pair<DWORD,int> >::iterator vitTeam;
        for( vitTeam = kInfo.m_vecUIDTeam.begin() ; vitTeam != kInfo.m_vecUIDTeam.end() ; ++vitTeam )
        {
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vitTeam->first );
            if( !spUser || spUser->m_MyRecomInfo.m_nState < KRecomHelper::RBS_ACCEPT_RECOM ) continue;

            if( !SiKRecomHelper()->IsAdjustTerm( spUser->m_MyRecomInfo ) ) continue;

            std::vector<std::pair<DWORD,int> >::iterator vitOrther;
            vitOrther = std::find_if( kInfo.m_vecUIDTeam.begin(), kInfo.m_vecUIDTeam.end(),
                        boost::bind( &std::pair<DWORD,int>::first, _1) == spUser->m_MyRecomInfo.m_dwLoginUID );

            if( vitOrther != kInfo.m_vecUIDTeam.end() && vitTeam->second == vitOrther->second )
                kPacket_.m_mapEvents[vitTeam->first].insert( SiKRecomHelper()->GetEventID() );
        }
    }
}

void KUser::AddEvents( IN OUT KEVENT_START_GAME_BROAD& kPacket_ )
{
    kPacket_.m_mapEvents.clear();
    JIF( m_pkRoom );
    KGameInfo kInfo;
    std::vector<KSimpleInDoorUserInfo> vecStartingInfo;
    std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> > mapActiveEventList;

    m_pkRoom->GetGameInfo( kInfo );
    m_pkRoom->GetStartingInfo( vecStartingInfo );
    if( SiKTimeEventManager()->GetCorrectEvents( kInfo, vecStartingInfo, mapActiveEventList ) )
    {
        m_pkRoom->SetTimeEvent( mapActiveEventList );
    }
    // 이터레이션 하면서 이벤트를 넣어 주자.. 패킷을 다 가져 오는것은 오버인가??
    std::map< std::pair<DWORD,char>, std::vector<sTimeEvent> >::iterator mit;
    for( mit = mapActiveEventList.begin() ; mit != mapActiveEventList.end() ; ++mit )
    {
        std::vector<sTimeEvent>::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            // 중복을 막기 위해서.
            kPacket_.m_mapEvents[ mit->first.first ].insert( vit->m_nEventID );
            //kPacket_.m_setEvents.insert( vit->m_nEventID );
        }
    }
}

void KUser::GetDefaultResultOut( OUT KGameResultOut& kResultOut_ )
{
    kResultOut_.m_strLogin = GetName();
    kResultOut_.m_nGamePoint = GetGP();
    kResultOut_.m_dwGuildUID = -1;
 
    KNGuildInfo guildInfo;
    if( GetGuildInfo( guildInfo ) )
    {
        kResultOut_.m_dwGuildUID = guildInfo.m_dwUID;
        kResultOut_.m_dwGuildPoint = guildInfo.m_dwPoint;
    }
    kResultOut_.m_dwContPoint = 0;
    kResultOut_.m_vecWin.clear();
    kResultOut_.m_vecLose.clear();

    if( SiKGSSimLayer()->CheckTypeFlag( ST_INDIGO ) )
    {
        kResultOut_.m_iIndigoWin = m_iIndigoWin;
        kResultOut_.m_iIndigoLose = m_iIndigoLose;
        kResultOut_.m_iRankingPoint = m_iRP;
    }

    kResultOut_.m_vecInv.clear();
    GetUserDifficultInfo( kResultOut_.m_mapDifficulty );
    kResultOut_.m_ucCharType = m_cCurrentCharacter;
    kResultOut_.m_vecPetInfo.clear();
    kResultOut_.m_vecExps.clear();
    kResultOut_.m_nStageBonusGP = 0;
}

void KUser::DumpResultIn( OUT std::wstringstream& stm_ , IN const KGameResultIn& kResultIn_ )
{

    std::map<int,float>::const_iterator mit;
    stm_ << std::endl << L" Login : " << kResultIn_.m_strLogin << std::endl
        << L" GP : " << kResultIn_.m_nBaseGP << std::endl
        << L" Total GP : " << kResultIn_.m_nTotalRecvGP << std::endl
        << L" ContPoint : " << kResultIn_.m_iContPoint << std::endl
        << L" bWin : " << (kResultIn_.m_bWin? L"Win" : L"Lose") << std::endl
        << L" Kill/Die (" << kResultIn_.m_iNumKills << L" / "<< kResultIn_.m_iNumDies << L")" << std::endl
        << L" Score : " << kResultIn_.m_nScore << std::endl
        << L" Gp Ratio ( " ;

    for ( mit = kResultIn_.m_mapGpBoost.begin() ; mit != kResultIn_.m_mapGpBoost.end() ; ++mit ) {
        stm_ << mit->first <<L"/" << mit->second << L", ";
    }
    stm_ << L" )" << std::endl << L" Exp Ratio ( " ;
    for ( mit = kResultIn_.m_mapExpBoost.begin() ; mit != kResultIn_.m_mapExpBoost.end() ; ++mit ) {
        stm_ << mit->first << L"/" << mit->second << L", ";
    }
    
    stm_ << L" )" << std::endl
        << L" AddtionalGp : " << kResultIn_.m_nAddtionalGp << std::endl;

    stm_ << L" Coins : ";
    std::vector<KKillCoinInfo>::const_iterator vitCoin;
    for( vitCoin = kResultIn_.m_vecKillCoin.begin() ; vitCoin != kResultIn_.m_vecKillCoin.end() ; ++vitCoin )
    {
        stm_ << L"(" << vitCoin->m_dwUserUID << L"," << (int)vitCoin->m_cCharType << L"," << vitCoin->m_nCoin << L"), ";
    }
    stm_ << std::endl;

    stm_ << L" Exps = ";
    std::vector< KCharacterExpInfo >::const_iterator vit;
    for( vit = kResultIn_.m_vecCharExp.begin() ; vit != kResultIn_.m_vecCharExp.end() ; ++vit )
    {
        stm_ << L"(" << (int)(vit->m_cCharType) << L", "
            << (int)(vit->m_cCharJob) << L", "
            << vit->m_nBaseExp << L", "
            << vit->m_nAdditionnalExp << L", [";
        for ( mit = vit->m_mapExpBoost.begin() ; mit != vit->m_mapExpBoost.end() ; ++mit ) {
            stm_ << mit->first <<L"/" << mit->second << L", ";
        }
        stm_ << L"]," << L", [";
        for ( mit = vit->m_mapExpBoost.begin() ; mit != vit->m_mapExpBoost.end() ; ++mit ) {
            stm_ << mit->first <<L"/" << mit->second << L", ";
        }
        stm_ <<  L"] ), ";
    }
    stm_ << std::endl;

    stm_ << L"Stage Bonus GP : " << kResultIn_.m_nAddtionalGp << std::endl;
}

void KUser::DumpEndGameInfo( IN const KGameInfo& kGameInfo_, IN const KEndGameReq& kReq_ )
{
    if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) )
        return;

    std::vector<sTriggerInfo> vecTriggers;
    vecTriggers.clear();
    std::map<int,int> mapKillMonsters;
    if( KRoom::IsDungeonMode( kGameInfo_.m_iGameMode ) )
        SiKGameDrop()->GetTriggerInfo( kGameInfo_.m_iGameMode, kGameInfo_.m_nDifficulty, vecTriggers );
    KUser::MakeKillMonInfo( kReq_.m_setKillMonster, vecTriggers, mapKillMonsters );

    START_LOG( cwarn, L"========= End Game Dump ========="  ) << END_LOG;
    START_LOG( clog , L" " )
        << BUILD_LOG( kGameInfo_.m_nStartUserNum )
        << BUILD_LOG( kGameInfo_.m_nEndUserNum )
        << BUILD_LOGc( kGameInfo_.m_cGameCategory )
        << BUILD_LOG( kGameInfo_.m_iGameMode )
        << BUILD_LOG( kGameInfo_.m_iSubGameMode )
        << BUILD_LOG( kGameInfo_.m_nDifficulty )
        << BUILD_LOG( kGameInfo_.m_iMapID )
        << BUILD_LOG( kGameInfo_.m_bRandomableMap )
        << BUILD_LOG( kGameInfo_.m_nMonsterID )
        << BUILD_LOG( kGameInfo_.m_nMonsterCount )
        << BUILD_LOG( kGameInfo_.m_biTotalExp )
        << BUILD_LOG( kGameInfo_.m_dwTotalGp )
        << BUILD_LOGc( kGameInfo_.m_cChannelType )
        << BUILD_LOG( kGameInfo_.m_prGuildUID.first )
        << BUILD_LOG( kGameInfo_.m_prGuildUID.second ) 
        << END_LOG;
    START_LOG( cwarn, L"========= Packet ========="  )
        << BUILD_LOG( kReq_.m_iQuestEndingStage )
        << BUILD_LOG( kReq_.m_setClearStageCount.size() )
        << BUILD_LOG( kReq_.m_mapDropResult.size() )
        << BUILD_LOG( kReq_.m_mapRouletteList.size() )
        << BUILD_LOG( kReq_.m_setKillMonster.size() )
        << BUILD_LOG( mapKillMonsters.size() )
        << END_LOG;

    START_LOG( cwarn, L"========= Result In ========="  ) << END_LOG;
    std::vector<KGameResultIn>::const_iterator vit;
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
    {
        std::wstringstream stm;
        DumpResultIn( stm, *vit );
        START_LOG( clog, L"" ) 
            << BUILD_LOG( stm.str() ) << END_LOG;

        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }

    START_LOG( cwarn, L"========= Character stats ========="  ) << END_LOG;
    std::map<std::pair<int,int>, KCharacterStats>::const_iterator cmit;
    for ( cmit = kReq_.m_mapCharStats.begin() ; cmit != kReq_.m_mapCharStats.end() ; ++cmit ) {
        START_LOG( clog, L"SkillID : " << cmit->first.first
            << L", CharLevel : " << cmit->first.second
            << L", ModeType : " << cmit->second.m_nModeType
            << L", AccCount : " << cmit->second.m_nAccCount ) << END_LOG;
    }
}

void KUser::CheckDrop( OUT KEVENT_START_GAME_BROAD& kPacket_ )
{
    KGameInfo kGameInfo;
    std::vector<sTriggerInfo> vecTriggers;
    std::vector<KDropItemInfo> vecPartyReward;
    m_pkRoom->GetGameInfo( kGameInfo );

    kPacket_.m_dropData.m_vecMissionDrop.clear();
    kPacket_.m_vecChampions.clear();
    kPacket_.m_vecEventMonster.clear();
    vecPartyReward.clear();

    // 던전모드 체크
    if ( KRoom::IsDungeonMode( kGameInfo.m_iGameMode ) ) {
        float fScale = GetDropEventScale( kGameInfo );

        SetEventMonsterInfo( kGameInfo.m_iGameMode, (int)kGameInfo.m_nDifficulty, kPacket_.m_vecEventMonster, vecPartyReward ); // 이벤트 몬스터 설정
        SiKGameDrop()->GetDropInfo( kGameInfo, kPacket_.m_dropData.m_vecMonsterDrop, kPacket_.m_dropData.m_vecGpDrop, kPacket_.m_vecChampions, fScale );
        AddEventMonsterDrop( kPacket_.m_vecEventMonster, kGameInfo.m_vecUserUIDs, kPacket_.m_dropData.m_vecMonsterDrop );

        SiKGameDrop()->GetTriggerInfo( kGameInfo.m_iGameMode, kGameInfo.m_nDifficulty, vecTriggers );
        AddEventMonsterChampion( (int)kGameInfo.m_nDifficulty, vecTriggers, kPacket_.m_vecEventMonster, kPacket_.m_vecChampions );
        AddEventMonsterTrigger( kPacket_.m_vecEventMonster, vecTriggers );

        GetSpecialMonsterInfo( kGameInfo.m_iGameMode, kGameInfo.m_nDifficulty, kPacket_.m_kSpecialMonster ); // Special Monster 설정.
        SiKMissionManager()->GetMissionDropList( kGameInfo, vecTriggers, kPacket_.m_dropData.m_vecMissionDrop );
        SiKGameDrop()->SetNumbering( kPacket_.m_dropData ); // 이녀석은 드랍의 가장 마지막에 해야한다. 이유는 ..... 유니크한 숫자를 생성해내기 때문이다.
    }

    m_pkRoom->SetDropList( kPacket_.m_dropData );
    m_pkRoom->SetChampInfo( kPacket_.m_vecChampions );
    m_pkRoom->SetEventMonInfo( kPacket_.m_vecEventMonster );
    m_pkRoom->SetSpecialMonInfo( kPacket_.m_kSpecialMonster );
    m_pkRoom->SetPartyRewardInfo( vecPartyReward ); // 파티전체 획득 아이템 정보 세팅(아이템 상자 먹었을 때 이걸로 체크 함)
}

void KUser::UpdateMissionInfo( KEndGameAck& kPacket_ )
{
    std::vector<KGameResultOut>::iterator vit;
    for( vit = kPacket_.m_vecGameResult.begin() ; vit != kPacket_.m_vecGameResult.end() ; ++vit )
    {
        vit->m_vecMissionSlot.clear();

        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser )
            continue;

        vit->m_vecMissionSlot = spUser->m_vecMissionSlot;
        spUser->DumpMissionSlot();
    }
}

void KUser::UpdateMissionInfo( IN std::vector<KMissionData>& vecMissionSlot_ )
{
     vecMissionSlot_ = m_vecMissionSlot;
}

void KUser::DumpMissionSlot()
{
    START_LOG( clog, L"Name : " << m_strName  ) << END_LOG;
    std::vector<KMissionData>::iterator vit;
    for( vit = m_vecMissionSlot.begin() ; vit != m_vecMissionSlot.end() ; ++vit )
    {
        START_LOG( clog, L" MissionID : " << vit->m_dwMissionID  ) << END_LOG;
        std::map<int,int>::iterator mit;
        for( mit = vit->m_mapCompletionInfo.begin() ; mit != vit->m_mapCompletionInfo.end() ; ++mit )
        {
            START_LOG( clog, L"{" << mit->first << L"," << mit->second << L"}," ) << END_LOG;
        }
    }
}

void KUser::DumpInventory()
{
    START_LOG( clog, L"Writing " << m_strName << "'s inventory items(LoginUID: " << m_dwUID << L" )" ) << END_LOG;

    m_kInventory.DumpInventory( m_strName, m_dwUID );

    START_LOG( clog, L"Writing request complete. Check \"UserInventoryItemList\" directory." ) << END_LOG;
}

bool KUser::CheckRequireItem() // 게임을 시작하기 위해서 필요한 아이템..
{
    _JIF( m_pkRoom, return false );
    sRequire requireData;
    
    if( !SiKRequierInfo()->GetRequierData( m_pkRoom->m_iGameMode, requireData ) )
        return true;

    sRequire::VEC_TYPE_ITEM::iterator vit;
    for( vit = requireData.m_vecItems.begin() ; vit != requireData.m_vecItems.end() ; ++vit )
    {
        switch( vit->first )
        {
        case KItemUnit::IT_PERIOD:
            if( m_kInventory.FindItemByItemID( vit->second ) == NULL )
                return false;
            break;
        case KItemUnit::IT_QUANTITY:
            if( m_kInventory.FindDurationItemByItemID( vit->second ) == NULL )
                return false;
            break;
        default:
            START_LOG( cerr, L"알수 없는 아이템 타입, Name : " << m_strName )
                << BUILD_LOG( requireData.m_nID )
                << BUILD_LOG( requireData.m_vecItems.size() )
                << BUILD_LOGc( vit->first ) << END_LOG;
            return false;
        }
    }
    return true;
}

void KUser::MakeKillMonInfo( IN const std::set<int>& setKillMonster_,
                             IN const std::vector<sTriggerInfo>& vecTriggers_,
                             OUT std::map<int,int>& mapKillMonsters_ )
{
    mapKillMonsters_.clear();
    std::set<int>::const_iterator sit;
    for( sit = setKillMonster_.begin() ; sit != setKillMonster_.end() ; ++sit )
    {
        std::vector<sTriggerInfo>::const_iterator vit;
        vit = std::find_if( vecTriggers_.begin(), vecTriggers_.end(),
            boost::bind( &sTriggerInfo::m_nTriggerID, _1 ) == *sit );
        if( vit == vecTriggers_.end() )
            continue;
        mapKillMonsters_[vit->m_nMonsterID] += 1;
    }
}

void KUser::MakeKillMonLevelInfo( IN const std::set<int>& setKillMonster_,
                            IN const std::vector<sTriggerInfo>& vecTriggers_,
                            OUT std::map<int,int>& mapKillMonstersLevel_ )
{
    mapKillMonstersLevel_.clear();
    std::set<int>::const_iterator sit;
    for ( sit = setKillMonster_.begin() ; sit != setKillMonster_.end() ; ++sit ) {
        std::vector<sTriggerInfo>::const_iterator vit;
        vit = std::find_if( vecTriggers_.begin(), vecTriggers_.end(),
            boost::bind( &sTriggerInfo::m_nTriggerID, _1 ) == *sit );
        if ( vit == vecTriggers_.end() ) {
            continue;
        }
        mapKillMonstersLevel_[vit->m_nMonLv] += 1;

    }
}

void KUser::CheckBonusRefill()
{
    // 1~20레벨     :   매일 2씩 충전.  (현재 1일 경우 3으로 충전)
    // 21~35레벨    :   매일 1씩 충전.  (현재 1일 경우 2로 충전)
    // 36 레벨 이상 :   충전되지 않음.
    // 20이하 레벨은 충전 보너스 2, 21~35레벨은 충전 보너스 1로 지정하여 매일 12시에 기본 보너스를 충전함
    // 0개 충전을 요청해도 처리하자.. 이유는 보너스 받는것 갱신을 위해서.
    KDB_EVENT_BONUS_POINT_REFILL_REQ kDBPacket;
    kDBPacket.m_nDiffBonusCount = 0;
    kDBPacket.m_cCharType = GetCurrentCharType();
    const KCharacterInfo* pkCurrentChar = GetCurrentCharacter();

    if ( NULL == pkCurrentChar ) {
        START_LOG( cerr, L" 보너스 포인트 충전 체크시 캐릭터 정보가 존재하지 않음. Name : " << m_strName )
            << BUILD_LOGc( kDBPacket.m_cCharType )
            << END_LOG;
        return;
    }

    kDBPacket.m_dwSlotNum = pkCurrentChar->m_dwSlotNum;

    // 체크하나 하자 맥스치 이상 받을수 없도록 막자..
    if ( SiKGCHelper()->GetMaxBonusCount() > GetCurrentBonusCount() ) {
        kDBPacket.m_nDiffBonusCount = SiKGCHelper()->GetRefillBonusPoint( static_cast< int >( pkCurrentChar->m_dwLevel ), m_mapBaseBonus[ GetCurrentCharType() ] );
        kDBPacket.m_nDiffBonusCount = std::min< int >( kDBPacket.m_nDiffBonusCount, SiKGCHelper()->GetMaxBonusCount() - GetCurrentBonusCount() );
    }

    // 로그성 데이터를 DB에 기록하기 위해서 충전 할 포인트가 0인 경우에도 DB 호출을 하도록 한다
    QUEUEING_EVENT_TO_DB( DB_EVENT_BONUS_POINT_REFILL_REQ, kDBPacket );

    START_LOG( clog, L"보너스 충전... Name : " << m_strName )
        << BUILD_LOG( kDBPacket.m_nDiffBonusCount )
        << BUILD_LOG( GetCurrentBonusCount() )
        << BUILD_LOG( static_cast< int >( m_mapBaseBonus[ GetCurrentCharType() ] ) )
        << BUILD_LOG( static_cast< int >( m_mapSpecialBonus[ GetCurrentCharType() ] ) )
        << END_LOG;
}

bool KUser::ReduceBonusCount( int nCount_/*= 1*/ )
{
    if( m_mapBaseBonus[GetCurrentCharType()] <= 0 && m_mapSpecialBonus[GetCurrentCharType()] <= 0 )
        return false;

    if( m_mapBaseBonus[GetCurrentCharType()] > 0 )
    {
        m_mapBaseBonus[GetCurrentCharType()] -= nCount_;
        return true;
    }

    if( m_mapSpecialBonus[GetCurrentCharType()] > 0 )
    {
        m_mapSpecialBonus[GetCurrentCharType()] -= nCount_;
        return true;
    }

    return false;
}

void KUser::GetBonusInfo( IN const char cCharType_, OUT KBonusPointInfo& kBonusInfo_ )
{
    kBonusInfo_.m_nBaseBonus    = m_mapBaseBonus[ cCharType_ ];
    kBonusInfo_.m_nSpecialBonus = m_mapSpecialBonus[ cCharType_ ];
}

bool KUser::CheckValidateSlot( IN OUT std::vector< KSlotData >& kQuickSlot_, IN bool bEmoticon_ )
{
    if( kQuickSlot_.empty() )
        return true;

    // 소팅,
    std::sort( kQuickSlot_.begin(), kQuickSlot_.end(),
        boost::bind( &KSlotData::m_nSlotIndex, _1) < boost::bind(&KSlotData::m_nSlotIndex, _2) );

    // Unique,
    std::vector< KSlotData >::iterator vit;
    vit = std::unique( kQuickSlot_.begin(), kQuickSlot_.end(),
        boost::bind( &KSlotData::m_nSlotIndex, _1) == boost::bind(&KSlotData::m_nSlotIndex, _2) );
    if( kQuickSlot_.end() != vit ) // 중복된 슬롯 ID있음.
        return false;

    // 내 인벤토리에 있는가?
    std::set<DWORD> setItems;
    for( vit = kQuickSlot_.begin() ; vit != kQuickSlot_.end() ; ++vit )
    {
        setItems.insert( vit->m_kItem.m_dwID );

        if ( m_kInventory.FindItem( vit->m_kItem.m_dwID, vit->m_kItem.m_dwUID ) == NULL ) {
            START_LOG( cerr, L"퀵슬롯에 장착하려는 아이템이 인벤토리에 없음, Name : " << m_strName )
                << BUILD_LOG( vit->m_nSlotIndex )
                << BUILD_LOGc( vit->m_kItem.m_cItemType )
                << BUILD_LOG( vit->m_kItem.m_dwID )
                << BUILD_LOG( vit->m_kItem.m_dwUID ) << END_LOG;
            return false;
        }
    }

    if( setItems.size() != kQuickSlot_.size() )
    {
        START_LOG( cerr, L"슬롯 정보 변경시 중복된 아이템이 있음.. Name : " << m_strName )
            << BUILD_LOG( setItems.size() )
            << BUILD_LOG( kQuickSlot_.size() ) << END_LOG;
        return false;
    }

    if( bEmoticon_ )
        _JIF( SiKGCHelper()->IsValidateEmoticon( kQuickSlot_, m_strName ), return false );
    else
        _JIF( SiKGCHelper()->IsValidateItem( kQuickSlot_, m_strName ), return false );

    return true;
}

bool KUser::IsInSlotByItemID( IN std::vector< KSlotData >& kQuickSlot_, IN GCITEMID dwItemID_ )
{
    std::vector< KSlotData >::const_iterator vit;
    for( vit = kQuickSlot_.begin() ; vit != kQuickSlot_.end() ; ++vit )
    {
        if( vit->m_kItem.m_dwID == dwItemID_ ) return true;
    }
    return false;
}

bool KUser::IsInSlot( IN std::vector< KSlotData >& kQuickSlot_, IN KSlotData& kSlotData_ )
{
    std::vector< KSlotData >::iterator vit;
    vit = std::find_if( kQuickSlot_.begin() , kQuickSlot_.end(),
        boost::bind( &KSlotData::m_nSlotIndex, _1) == kSlotData_.m_nSlotIndex );

    if( vit == kQuickSlot_.end() )
    {
        SET_ERROR( ERR_QUICKSLOT_02 );
        return false;
    }

    if( !(vit->m_kItem == kSlotData_.m_kItem) ) // == 오퍼레이터만 정의 되어 있음..^^..
    {
        SET_ERROR( ERR_QUICKSLOT_03 );
    }

    return true;
}

bool KUser::ReleaseQuickSlot( GCITEMUID dwUID_ )
{
    std::map< char, KQuickSlot >::iterator mit;
    mit = m_mapQuickSlot.find( m_cCurrentCharacter );
    if( mit == m_mapQuickSlot.end() ) {
        return false;
    }

    std::vector< KSlotData >::iterator vit;
    for( vit = mit->second.m_vecEmoticonSlot.begin() ; vit != mit->second.m_vecEmoticonSlot.begin() ; ) {
        if( vit->m_kItem.m_dwUID == dwUID_ )
        {
            vit = mit->second.m_vecEmoticonSlot.erase( vit );
            return true;
        }
        ++vit;
    }

    for( vit = mit->second.m_vecItemSlot.begin() ; vit != mit->second.m_vecItemSlot.begin() ; ) {
        if( vit->m_kItem.m_dwUID == dwUID_ )
        {
            vit = mit->second.m_vecItemSlot.erase( vit );
            return true;
        }
        ++vit;
    }
    return false;
}

void KUser::AddStatistics( IN KGameInfo& kGameInfo_, IN KEndGameReq& kReq_ )
{
    std::vector<KGameResultIn>::iterator vit;
    // CPL
    do{
        KCPLStatisticsPtr spStat = GET_STAT( KCPLStatistics, SI_CPL );
        if( !spStat ) break;

        MAP_CPL_STAT mapCPL;
        KStatKeyCPL kKey;
        kKey.m_nModeID      = kGameInfo_.m_iGameMode;
        kKey.m_nDifficult   = kGameInfo_.m_nDifficulty;
        if( kGameInfo_.m_cChannelType != KChannel::CT_DUNGEON )
            kKey.m_nDifficult = 0;

        for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
        {
            if( vit->m_vecCharExp.empty() ) continue;

            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
            if( !spUser ) continue;

            const KCharacterInfo* pkChar =  spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
            if( !pkChar ) continue;

            kKey.m_dwUID        = vit->m_dwUID; // UserUID 추가.
            kKey.m_bWinLose     = vit->m_bWin;
            kKey.m_cCharType    = vit->m_vecCharExp.begin()->m_cCharType;
            kKey.m_cPromotion   = vit->m_vecCharExp.begin()->m_cCharJob; // Promotion정보를 job정보로 넣게 수정.
            kKey.m_nLevel       = (int)pkChar->m_dwLevel;
            kKey.m_cDataType    = 0; // 0 : Exp, 1 : Gp

            mapCPL[kKey].first    += 1; // Count
            mapCPL[kKey].second   += (DWORD)( vit->m_vecCharExp.begin()->m_nBaseExp ); // Data

            kKey.m_cDataType    = 1; // 0 : Exp, 1 : Gp

            mapCPL[kKey].first    += 1; // Count
            mapCPL[kKey].second   += (DWORD)( vit->m_nBaseGP ); // Data
        }

        spStat->AddStat( mapCPL );

    }while( false );

    // Play Time
    do{
        KPlayTimeStatPtr spStat = GET_STAT( KPlayTimeStat, SI_PLAY_TIME );
        if( !spStat ) break;
        if( !m_pkRoom ) break;

        int nWinCount = 0;
        int nLoseCount = 0;
        MAP_PLAY_TIME_STAT mapPlayTime;
        KStatKeyPlayTime kKey;
        kKey.m_nModeID      = kGameInfo_.m_iGameMode;
        kKey.m_nDifficult   = kGameInfo_.m_nDifficulty;
        if( kGameInfo_.m_cChannelType != KChannel::CT_DUNGEON )
            kKey.m_nDifficult = 0;

        DWORD dwPlayTime = m_pkRoom->GetLatestPlayTime() / (1000 * 60 );

        // 이건 현우형과 한번더.......
        //if( nPlayTime > 10 )
        //    nPlayTime = 11;

        //{ 캐릭터PT통계
        KEVENT_STAT_CHAR_LV_PT_NOT vecCharLvPT;
        vecCharLvPT.clear();
        //}

        std::vector<int> vecLevel;
        if( !kReq_.m_vecGameResult.empty() )
            vecLevel.reserve( kReq_.m_vecGameResult.size() ); // 미리 공간을 만들어 둔다.

        // 각 유저별 해킹 체크를 위한 던전 통계 데이터 추가.
        std::vector<KStatDungeonUser> vecStatDungeonUser;
        if( !kReq_.m_vecGameResult.empty() )
            vecStatDungeonUser.reserve( kReq_.m_vecGameResult.size() ); // 미리 공간을 만들어 둔다.

        for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit )
        {
            if( vit->m_vecCharExp.empty() ) continue;

            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
            if( !spUser ) continue;

            const KCharacterInfo* pkChar =  spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
            if( !pkChar ) continue;

            vecLevel.push_back( (int)pkChar->m_dwLevel );

            //{ 캐릭터PT통계
            KCharLvPromotion kCharInfo;
            kCharInfo.m_cCharacterType = vit->m_vecCharExp.begin()->m_cCharType;
            kCharInfo.m_cPromotion = pkChar->m_cPromotion;
            kCharInfo.m_dwLv = pkChar->m_dwLevel;
            vecCharLvPT.push_back( std::make_pair(kCharInfo,dwPlayTime) );
            //}

            if( vit->m_bWin )
                ++nWinCount;
            else
                ++nLoseCount;

            // 캐릭터 레벨 플레이타임 통계
            spUser->UpdateCharLvPlayTime( static_cast<int>(pkChar->m_cCharType), pkChar->m_dwLevel );

            KStatDungeonUser kStatDungeonUser;
            kStatDungeonUser.m_nModeID = kGameInfo_.m_iGameMode;
            kStatDungeonUser.m_nDifficult = kGameInfo_.m_nDifficulty;
            kStatDungeonUser.m_dwLoginUID = vit->m_dwUID;
            kStatDungeonUser.m_dwPlayTime = m_pkRoom->GetLatestPlayTime() / ( 1000 );
            kStatDungeonUser.m_nTotalUserCount = kReq_.m_vecGameResult.size();
            kStatDungeonUser.m_cCharType = vit->m_vecCharExp.begin()->m_cCharType;
            kStatDungeonUser.m_dwLevel = pkChar->m_dwLevel;
            kStatDungeonUser.m_dwClearType = (vit->m_bWin ? 0 : 1 ); // 0 : Clear, 1 : Not Clear

            vecStatDungeonUser.push_back( kStatDungeonUser );
        }

        //{ 캐릭터PT통계
        if ( !vecCharLvPT.empty() ) {
            QUEUEING_EVENT_TO_DB( EVENT_STAT_CHAR_LV_PT_NOT, vecCharLvPT );
        }
        //}

        // 유저별 던전 통계
        if( !vecStatDungeonUser.empty() ) {
            QUEUEING_EVENT_TO_DB( DB_EVENT_USER_DUNGEON_STAT_NOT, vecStatDungeonUser );
        }

        std::vector<int>::iterator vit1;
        for( vit1 = vecLevel.begin() ; vit1 != vecLevel.end() ; ++vit1 )
        {
            kKey.m_nLevel = *vit1;

            if( nWinCount > 0 )
            {
                kKey.m_cPartyNum    = (char)nWinCount;
                kKey.m_bWinLose     = true;

                mapPlayTime[kKey].first += 1;
                mapPlayTime[kKey].second += dwPlayTime;
            }

            if( nLoseCount > 0 )
            {
                kKey.m_cPartyNum    = (char)nLoseCount;
                kKey.m_bWinLose     = false;

                mapPlayTime[kKey].first += 1;
                mapPlayTime[kKey].second += dwPlayTime;
            }

            spStat->AddStat( mapPlayTime );
        }
    }while( false );

    // 캐릭터 관련 통계 (스킬)
    {
        KStatCharacterPtr spStat = GET_STAT( KStatCharacter, SI_STAT_CHARACTER );
        if ( spStat != NULL ) {
            spStat->AddStat( kReq_.m_mapCharStats );
        }
    }

    // 장착 아이템 통계
    {
        std::vector<KGameResultIn>::iterator vit;
        for ( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
            KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( vit->m_dwUID );
            if( !spUser ) continue;

            std::vector< KCharacterExpInfo >::iterator vitExp;
            for ( vitExp = vit->m_vecCharExp.begin() ; vitExp != vit->m_vecCharExp.end() ; ++vitExp ) {
                const KCharacterInfo* pkCharacterInfo = spUser->GetCharacterInfo( vitExp->m_cCharType );
                if( !pkCharacterInfo ) continue;

                KCharLvPromotion kCharacter;
                kCharacter.m_cCharacterType = vitExp->m_cCharType;
                kCharacter.m_dwLv = pkCharacterInfo->m_dwLevel;
                kCharacter.m_cPromotion = pkCharacterInfo->m_cPromotion;

                std::map<GCITEMID,DWORD> mapEquipInfo;
                mapEquipInfo.clear();

                std::vector<KEquipItemInfo>::const_iterator vitEquip;
                for ( vitEquip = pkCharacterInfo->m_vecEquipItems.begin() ; vitEquip != pkCharacterInfo->m_vecEquipItems.end() ; ++vitEquip ) {
                    ++mapEquipInfo[vitEquip->m_dwID];
                }

                // stat
                if ( !mapEquipInfo.empty() ) {
                    KStatCharacterEquipPtr spStat = GET_STAT( KStatCharacterEquip, SI_STAT_CHAR_EQUIP );
                    if( spStat != NULL ) {
                        spStat->AddStat( kCharacter, mapEquipInfo );
                    }
                }
            }
        }
    }

    KDB_EVENT_USER_CHANGE_CHAR_STAT vecChangeCharStat;
    vecChangeCharStat.clear();

    KDB_EVENT_USER_MODE_CLEAR_STAT vecModeClearStat;
    vecModeClearStat.clear();

    KDB_EVENT_USER_UDP_PORT_INFO_REQ vecUDPInfoStat;
    vecUDPInfoStat.clear();

    // 룸 캐릭터 변경 통계.
    for( vit = kReq_.m_vecGameResult.begin() ; vit != kReq_.m_vecGameResult.end() ; ++vit ) {
        if( vit->m_vecCharExp.empty() ) continue;

        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser ) continue;

        const KCharacterInfo* pkChar =  spUser->GetCharacterInfo( vit->m_vecCharExp.begin()->m_cCharType );
        if( !pkChar ) continue;

        if( pkChar->m_cCharType != spUser->m_cCurrentCharacter ) {
            IncreaseChangeCharType();
            //spUser->m_cCurrentCharacter = pkChar->m_cCharType;

            //{ 캐릭터PT통계
            KStatChangeCharInfo kStatChangeCharInfo;
            kStatChangeCharInfo.m_wstrStartDate = (LPCTSTR)(spUser->m_cConnectionTime.Format( _T("%Y-%m-%d %H:%M:%S")));
            kStatChangeCharInfo.m_dwUserUID = vit->m_dwUID;
            kStatChangeCharInfo.m_nModeID = kGameInfo_.m_iGameMode;
            kStatChangeCharInfo.m_cCharType = vit->m_vecCharExp.begin()->m_cCharType;
            kStatChangeCharInfo.m_dwLevel = pkChar->m_dwLevel;
            kStatChangeCharInfo.m_dwChangeCharTypeCnt = spUser->GetChangeCharType();

            vecChangeCharStat.push_back( kStatChangeCharInfo );
        }

        //{ 유저 모드클리어 정보.
        KStatUserModeClear kStatUserModeClear;
        kStatUserModeClear.m_dwUserUID = vit->m_dwUID;
        kStatUserModeClear.m_cCharType = vit->m_vecCharExp.begin()->m_cCharType;
        kStatUserModeClear.m_cCharPromotion = vit->m_vecCharExp.begin()->m_cCharJob;
        kStatUserModeClear.m_dwLevel = pkChar->m_dwLevel;
        kStatUserModeClear.m_nModeID = kGameInfo_.m_iGameMode;
        kStatUserModeClear.m_nDifficult = kGameInfo_.m_nDifficulty;
        kStatUserModeClear.m_nRecvExp = vit->m_vecCharExp.begin()->m_nBaseExp;
        kStatUserModeClear.m_nAddtionalGP = vit->m_nBaseGP;

        vecModeClearStat.push_back( kStatUserModeClear );

        // 유저 UDP 사용 통계
        KStatUserUDPPortInfo kStatUserUDPPortInfo;
        kStatUserUDPPortInfo.m_dwUserUID = vit->m_dwUID;
        kStatUserUDPPortInfo.m_nModeID = kGameInfo_.m_iGameMode;
        kStatUserUDPPortInfo.m_strIP = KncUtil::toWideString( GetIPStr() );
        kStatUserUDPPortInfo.m_usUDPUsePort = GetUDPUsePort();
        kStatUserUDPPortInfo.m_nPartyUserNum = kReq_.m_vecGameResult.size();

        vecUDPInfoStat.push_back( kStatUserUDPPortInfo );
    }

    if( !vecChangeCharStat.empty() ) {
        QUEUEING_EVENT_TO_DB( DB_EVENT_USER_CHANGE_CHAR_STAT, vecChangeCharStat );
    }

    if( !vecModeClearStat.empty() ) {
        QUEUEING_EVENT_TO_DB( DB_EVENT_USER_MODE_CLEAR_STAT, vecModeClearStat );
    }

    if( !vecUDPInfoStat.empty() ) {
        QUEUEING_EVENT_TO_DB( DB_EVENT_USER_UDP_PORT_INFO_REQ, vecUDPInfoStat );
    }
}

void KUser::AddStatisticsMap()
{
    JIF( m_pkRoom );

    if ( m_pkRoom->m_iGameMode != GC_GM_TEAM_MODE    && m_pkRoom->m_iGameMode != GC_GM_SURVIVAL_MODE &&
        m_pkRoom->m_iGameMode != GC_GM_TAG_TEAM     && m_pkRoom->m_iGameMode != GC_GM_TAG_SURVIVAL  &&
        m_pkRoom->m_iGameMode != GC_GM_GUILD_BATTLE && m_pkRoom->m_iGameMode != GC_GM_INDIGO_TEAM   &&
        m_pkRoom->m_iGameMode != GC_GM_ANGELS_EGG   && m_pkRoom->m_iGameMode != GC_GM_CAPTAIN &&
        m_pkRoom->m_iGameMode != GC_GM_AUTOMATCH_TEAM ) {
            return;
    }

    KMapStatisticsPtr spStat = GET_STAT( KMapStatistics, SI_MAP_COUNT );
    JIF( spStat );
    int nMapID = m_pkRoom->m_iMapID;
    if( m_pkRoom->m_bRandomableMap ) nMapID = -1;
    spStat->AddStat( nMapID );
}

void KUser::AddStatisticsDeathMatch()
{
    JIF( m_pkRoom );
    if( m_pkRoom->m_cGameCategory != GC_GMC_DEATHMATCH ) return;

    KStatDeathMatchPtr spStat = GET_STAT( KStatDeathMatch, SI_STAT_DEATH );
    JIF( spStat );
    spStat->AddStat( m_pkRoom->m_nDeathKillCount, m_pkRoom->m_nDeathMatchTime, m_pkRoom->m_bDeathMatchIntrudable, m_pkRoom->m_bDeathMatchBalancing );
}

void KUser::AddStatisticsFatalDeathMatch()
{
    JIF( m_pkRoom );
    if( m_pkRoom->m_cGameCategory != GC_GMC_FATAL_DEATHMATCH ) return;

    KStatDeathMatchPtr spStat = GET_STAT( KStatDeathMatch, SI_STAT_FATAL_DEATH );
    JIF( spStat );
    spStat->AddStat( m_pkRoom->m_nDeathKillCount, m_pkRoom->m_nDeathMatchTime, m_pkRoom->m_bDeathMatchIntrudable, m_pkRoom->m_bDeathMatchBalancing );
}

void KUser::AddStatisticsPvPPlayCharacters()
{
    JIF( m_pkRoom );

    bool bWrite = false;

    switch ( m_pkRoom->m_iGameMode ) {
        case GC_GM_TEAM_MODE:
        case GC_GM_SURVIVAL_MODE:
        case GC_GM_TAG_TEAM:
        case GC_GM_TAG_SURVIVAL:
        case GC_GM_GUILD_BATTLE:
        case GC_GM_DEATH_TEAM:
        case GC_GM_DEATH_SURVIVAL:
        case GC_GM_FATAL_DEATH_TEAM:
        case GC_GM_FATAL_DEATH_SURVIVAL:

        case GC_GM_QUEST81: // 소멸의 탑은 던전이지만 예외적으로 추가한다. 추후 기록되는 모드들을 스크립트로 관리할 수 있도록 수정하면 좋겠다. 하게되면 클래스 및 함수 이름들에서 PVP 빼자
            bWrite = true;

            break;
        default:
            bWrite = false;

            break;
    }

    if ( false == bWrite ) {
        return;
    }

    MAP_CHAR_JOB mapPromotionInfo;
    m_pkRoom->GetPromotionInfo( mapPromotionInfo );

    std::vector< int > vecCharTypes;
    vecCharTypes.clear();
    BOOST_FOREACH( MAP_CHAR_JOB::value_type& rCharacterJob, mapPromotionInfo ) {
        if ( true == rCharacterJob.second.empty() ) {
            continue;
        }

        // 캐릭터 타입. 태그 매치를 사용하지 않고 있으므로 벡터에 항상 하나의 요소만 존재한다고 가정한다
        vecCharTypes.push_back( static_cast< int >( rCharacterJob.second.begin()->first ) );
    }

    KStatPvPPlayCharacterPtr spStat = GET_STAT( KStatPvPPlayCharacter, SI_STAT_PVP_PLAY_CHARACTER );

    JIF( spStat );

    time_t tmStartTime = KncUtil::TimeToInt( m_pkRoom->m_tmGameStartTime );
    spStat->AddStat( tmStartTime, m_pkRoom->m_iGameMode, vecCharTypes );
}

void KUser::SendIndigoRank()
{
    KSerBuffer serBuffIndigo;
    SiKGCHelper()->GetIndigoRankInfo( serBuffIndigo );
    LIF( SendPacket( serBuffIndigo, L"KUserEvent::EVENT_INDIGO_RANK_NOT" ) );
}

bool KUser::GetSquareUserInfo( KSquareUserInfo& kInfo_ )
{
    __VERIFY_STATE( (5,
        KUserFSM::STATE_CHANNELLOBBY,
        KUserFSM::STATE_MINIGAME,
        KUserFSM::STATE_MINIGAME_PLAYING,
        KUserFSM::STATE_SQUARE_JOIN, 
        KUserFSM::STATE_SQUARE), 
        true, return false );

    const KCharacterInfo* pkChar = GetCurrentCharacter();
    _JIF( pkChar != NULL, return false );

    kInfo_.m_dwUID          = GetUID();
    kInfo_.m_strNick        = m_strNickName;
    kInfo_.m_strNickColor   = m_strNickColor;
    kInfo_.m_cCharType      = pkChar->m_cCharType;
    kInfo_.m_dwLevel        = pkChar->m_dwLevel;
    CheckInventoryPremiumItem( kInfo_.m_kPremiumInfo );  // 프리미엄 아이템 정보 넣기.
    kInfo_.m_kSquarePos     = m_kSquarePos;
    kInfo_.m_cGrade         = SiKResultManager()->GetCharGrade( (DWORD)pkChar->m_iWin );
    kInfo_.m_dwServerUID    = SiKGameServer()->GetUID();

    kInfo_.m_vecEquips.reserve( pkChar->m_vecEquipItems.size() );
    kInfo_.m_dwGuildUID       = 0; // 기본 값으로 0 셋팅.

    std::vector<KEquipItemInfo>::const_iterator cvit;
    for( cvit = pkChar->m_vecEquipItems.begin() ; cvit != pkChar->m_vecEquipItems.end() ; ++cvit ) {
        kInfo_.m_vecEquips.push_back( cvit->m_dwID );
    }
    kInfo_.m_cGrade = m_cGrade;
    ExtractCurrentLookItem( pkChar->m_cCharType, kInfo_.m_vecLookEquips );

    std::map<char,KWinRateRank>::iterator mit;
    mit = m_kRankInfo.m_mapDailyRank.find( kInfo_.m_cCharType );
    if( mit != m_kRankInfo.m_mapDailyRank.end() )
        kInfo_.m_mapRankInfo[0] = mit->second.m_nRank;

    mit = m_kRankInfo.m_mapWeeklyRank.find( kInfo_.m_cCharType );
    if( mit != m_kRankInfo.m_mapWeeklyRank.end() )
        kInfo_.m_mapRankInfo[1] = mit->second.m_nRank;

    mit = m_kRankInfo.m_mapMonthlyRank.find( kInfo_.m_cCharType );
    if( mit != m_kRankInfo.m_mapMonthlyRank.end() )
        kInfo_.m_mapRankInfo[2] = mit->second.m_nRank;

    std::map<char,KExpRank>::iterator mit1;
    mit1 = m_kRankInfo.m_mapExpRank.find( kInfo_.m_cCharType );
    if( mit1 != m_kRankInfo.m_mapExpRank.end() )
        kInfo_.m_mapRankInfo[3] = mit1->second.m_nRank;

    kInfo_.m_kPetInfo.m_dwPetID = 0;
    KPetInfo* pkPetInfo = NULL;
    if( GetPetInfo( pkChar->m_kEquipPetInfo.m_dwUID, &pkPetInfo ) )
    {
        kInfo_.m_kPetInfo.m_dwPetID         = pkPetInfo->m_dwID;
        kInfo_.m_kPetInfo.m_cPetPromotion   = pkPetInfo->m_cPromotion;
        kInfo_.m_kPetInfo.m_cPetLV          = pkPetInfo->m_iLevel;
        kInfo_.m_kPetInfo.m_strPetName      = pkPetInfo->m_strName;
        kInfo_.m_kPetInfo.m_kCostume        = pkPetInfo->m_kCostume;
    }

    KQuickSlot kQuickSlot;
    GetCharQuickSlotInfo( kQuickSlot );
    if( !kQuickSlot.m_vecItemSlot.empty() )
    {
        std::vector< KSlotData >::iterator vit1;
        kInfo_.m_vecQuickSlot.reserve( kQuickSlot.m_vecItemSlot.size() );
        for( vit1 = kQuickSlot.m_vecItemSlot.begin() ; vit1 != kQuickSlot.m_vecItemSlot.end() ; ++vit1 )
            kInfo_.m_vecQuickSlot.push_back( vit1->m_kItem.m_dwID );
    }

    kInfo_.m_dwCoupleUID = 0;
    if( IsCouple() )
    {
        kInfo_.m_strPartnerNick  = m_kCoupleInfo.m_strPartnerNick;
        kInfo_.m_strPartnerNickColor = m_kCoupleInfo.m_strPartnerNickColor;
        kInfo_.m_dwCoupleUID    = GetPartnerUID();
        GetCoupleEquip( kInfo_.m_vecCoupleEquip );
    }
    KNGuildInfo kGuildInfo;
    if ( GetGuildInfo( kGuildInfo ) && m_kGuildUserInfo.m_cMemberLevel != KNGuildUserInfo::GL_JOINER ) {
        kInfo_.m_strMarkName = kGuildInfo.m_strFileName;
        kInfo_.m_strGuildName = kGuildInfo.m_strName;
        kInfo_.m_dwGuildUID = m_kGuildUserInfo.m_dwGuildUID;
    }

    kInfo_.m_cCharPromotion = pkChar->m_cPromotion;
    kInfo_.m_strCharNickName = pkChar->m_strCharName;
    return true;
}

void KUser::AddSquareNewUser( IN KSquareUserInfo& kInfo_ )
{
    std::vector<KSquareUserInfo>::iterator vitEnter;
    vitEnter = std::find_if( m_vecEnterUser.begin(), m_vecEnterUser.end(), boost::bind( &KSquareUserInfo::m_dwUID, _1 ) == kInfo_.m_dwUID );
    if( vitEnter != m_vecEnterUser.end() )
        return;

    m_vecEnterUser.push_back( kInfo_ );

    std::vector<DWORD>::iterator vitLeave;
    vitLeave = std::find( m_vecLeaveUser.begin(), m_vecLeaveUser.end(), kInfo_.m_dwUID );
    if( vitLeave != m_vecLeaveUser.end() )
    {
        m_vecLeaveUser.erase( vitLeave );
        return;
    }
}

void KUser::AddSquareLeaveUser( IN DWORD dwUID_ )
{
    std::vector<KSquareUserInfo>::iterator vitEnter;
    vitEnter = std::find_if( m_vecEnterUser.begin(), m_vecEnterUser.end(), boost::bind( &KSquareUserInfo::m_dwUID, _1 ) == dwUID_ );
    if( vitEnter != m_vecEnterUser.end() )
    {
        m_vecEnterUser.erase( vitEnter );
        return;
    }

    std::vector<DWORD>::iterator vitLeave;
    vitLeave = std::find( m_vecLeaveUser.begin(), m_vecLeaveUser.end(), dwUID_ );
    if( vitLeave == m_vecLeaveUser.end() )
    {
        m_vecLeaveUser.push_back( dwUID_ );
        return;
    }
}

int KUser::GetSquareIndex( KSquarePos& kSquarePos_ )
{
    return (int)kSquarePos_.m_fXpos;
}

int KUser::GetSquareIndex()
{
    return (int)m_kSquarePos.m_fXpos;
}

void KUser::ProcessDoubleCristalCard( IN OUT KInsertDropItem& kPacket_ )
{
    std::map<DWORD, DROP_ITEMS>::iterator mit;
    for( mit = kPacket_.m_mapEachUserDrops.begin() ; mit != kPacket_.m_mapEachUserDrops.end() ; ++mit )
    {
        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( mit->first );
        if( !spUser ) continue;

        KItemPtr pkItem = spUser->m_kInventory.FindDurationItemByItemID( DOUBLE_CRYSTAL_CARD_GOODS_ID );
        if ( pkItem == NULL ) {
            pkItem = spUser->m_kInventory.FindDurationItemByItemID( DC30_DOUBLE_CRYSTAL_CARD_GOODS_ID );
        }

        if( !pkItem ) continue;

        bool bExist = false;
        DROP_ITEMS::iterator vit;
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            if( vit->m_ItemID == CRYSTAL_GOODS_ID )
            {
                vit->m_nDuration *= 2;
                bExist = true;
            }
        }

        if( bExist )
        {
            pkItem->m_nCount = std::max<int>(0, pkItem->m_nCount -1 );

            // 엄청난 꽁수를 써야 한다. ㅡㅡ..
            std::vector<KGameResultOut>::iterator vitOut;
            vitOut = std::find_if( kPacket_.m_kEndGameAck.m_vecGameResult.begin(), kPacket_.m_kEndGameAck.m_vecGameResult.end(),
                boost::bind( &KGameResultOut::m_strLogin, _1 ) == spUser->m_strName );
            if( vitOut == kPacket_.m_kEndGameAck.m_vecGameResult.end() )
                continue;

            // 0개 짜리 아이템을 받아서 인벤토리에서 가저 오게 하는데만 사용한다. ..ㅡㅡ..
            KItem kDurItem = *pkItem;
            kDurItem.m_nCount = 0;
            kDurItem.m_nInitCount = 0;
            vitOut->m_vecInv.push_back( kDurItem );
        }
    }
}

bool KUser::LeaveSquare()
{
    DWORD dwUID = GetUID();
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_LEAVE_SQUARE_NOT, dwUID );

    bool bOK;
    _JIF( m_spSquare, return false );
    bOK = m_spSquare->LeaveSquare( GetUID() );
    StateTransition( KUserFSM::INPUT_TO_CHANNELLOBBY );
    m_spSquare.reset();

    return bOK;
}

void KUser::DumpSquareUserInfo( std::vector<KSquareUserInfo>& vecSquareUser_ )
{
    //std::wstringstream stm;
    //std::vector<KSquareUserInfo>::iterator vit;
    //for( vit = vecSquareUser_.begin() ; vit != vecSquareUser_.end() ; ++vit )
    //{
    //    stm << L"Nick : " << vit->m_strNick
    //        << L", UID : " << vit->m_dwUID
    //        << L", Type : " << (int)(vit->m_cCharType )
    //        << L", Lv : " << vit->m_dwLevel
    //        << L", Pet : " << vit->m_kPetInfo.m_dwPetID
    //        << L", Quick : " << vit->m_vecQuickSlot.size()
    //        << std::endl;   
    //}

    //std::cout << L"======== Start Dump KSquareUserInfo =========" << dbg::endl;
    //std::cout << stm.str();
    //std::cout << L"======== Finish Dump KSquareUserInfo =========" << dbg::endl;

    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

bool KUser::RollBackToSquare( KSquarePtr& spSquare_ )
{
    ForceStateTransitionTo( KUserFSM::STATE_CHANNELLOBBY );
    KEVENT_ENTER_SQUARE_ACK kPacket;
    SiKSquareManager()->EnterSquareReserve( GetThisPtr(), spSquare_->GetUID() );
    m_vecEnterUser.clear();
    m_vecLeaveUser.clear();

    if( NetError::GetLastNetError() == NetError::NET_OK )
    {
        StateTransition( KUserFSM::INPUT_ENTER_SQUARE );
        return true;
    }
    return false;
}

void KUser::DumpIndoorUserInfo( KInDoorUserInfo& kInfo_ )
{
    START_LOG( cwarn, L"Name : " << kInfo_.m_strLogin << L"(" << kInfo_.m_dwUserUID << L"|" << kInfo_.m_strNickName << L")"
        << L", Index : " << kInfo_.m_iPlayerIndex
        << L", Team : " << (kInfo_.m_iTeam==1? L"blue" :L"Red" )
        << L", Guild ID : " << kInfo_.m_dwGuildUID
        << L", State : " << kInfo_.m_nState ) <<  END_LOG;
}

void KUser::DumpIndoorUserInfo( std::vector<KInDoorUserInfo>& vecInfo_ )
{
    START_LOG( cwarn, L"======== Start Dump InDoorUserInfo ======"  ) << END_LOG;
    std::vector<KInDoorUserInfo>::iterator vit;
    for( vit = vecInfo_.begin() ; vit != vecInfo_.end() ; ++vit )
    {
        DumpIndoorUserInfo( *vit );
        DumpInDoorCharInfo( vit->m_vecCharInfo );
    }
    START_LOG( cwarn, L"======== End Dump InDoorUserInfo ======"  ) << END_LOG;
}

void KUser::UpdateGrade()
{
    std::vector<__int64> vecExp;
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {
        vecExp.push_back( cmit->second.m_biExp );
    }

    m_cGrade = SiKResultManager()->GetGrade( vecExp );
}

char KUser::GetBestCharGrade()
{
    char cGrade = 0;
    const KCharacterInfo* pkBestChar = GetBestCharacter();

    if( !pkBestChar ) return cGrade;

    return SiKResultManager()->GetCharGrade( (DWORD)pkBestChar->m_iWin );
}

void KUser::UpdateUserAuthLevel()
{
    //일반과 블럭 유저는 처리하지 않는다.
    if( m_cAuthLevel >= AL_NORMAL || m_cAuthLevel == AL_BLOCK || m_cAuthLevel == AL_CHAT_BLOCK ) {
        return;
    }

    // 아직 시간이 않되었다면...
    if( CTime( m_kBadUserInfo.m_tmLimit ) > CTime::GetCurrentTime() ) return;

    m_kBadUserInfo.m_cLastGrade = AL_NORMAL;
    m_kBadUserInfo.m_cCurrentGrade = AL_NORMAL;
    m_cAuthLevel = m_kBadUserInfo.m_cCurrentGrade;

    if( m_pkRoom != NULL && (GetStateID() == KUserFSM::STATE_ROOM || GetStateID() == KUserFSM::STATE_PLAYING) )
    {
        KEVENT_BAD_USER_INFO_CHANGE_BROAD kPacket;
        m_pkRoom->UpdateAuthLevel();
        m_pkRoom->BroadPacket( KRoom::BROAD_TO_ALL_EX_ME, m_strName, KUserEvent::EVENT_BAD_USER_INFO_CHANGE_BROAD, kPacket, false );
    }

    SEND_DATA( EVENT_BAD_USER_INFO_NOT, m_kBadUserInfo );
}

void KUser::GetMyRankInfo()
{
    std::set<char> kPacket;
    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {
        kPacket.insert( cmit->first );
    }

    QUEUEING_EVENT_TO_DB( EVENT_DB_MY_RANK_INFO_REQ, kPacket );
}

bool KUser::HellModeCheck( int nGameMode_, int nDifficulty_ )
{
    START_LOG( clog, L" Mode : " << nGameMode_ << L", Difficult : " << nDifficulty_ ) << END_LOG;

    if ( IsAdmin() ) {
        START_LOG( clog, L"운영자는 검사 안함." ) << END_LOG;
        return true;
    }

    if ( false == KRoom::IsDungeonMode( nGameMode_ ) ) {
        START_LOG( clog, L"던전 모드가 아니면 통과 : " << nGameMode_ ) << END_LOG;
        return true;
    }

    if ( nDifficulty_ < GC_GMD_CHAMPION_BEGIN ) {
        START_LOG( clog, L"던전 모드 인데 난이도가 헬 모드가 아니면 통과 : " << nDifficulty_ ) << END_LOG;
        return true;
    }

    // 100427,. 
    // 던전 프리패스 모드 추가.
    // 모든 던전에 출입가능( 클라이언트에서 작업 했음 )
    // 챔피언 모드 들어가 져야 하고.., 챔피언 모드 입장권도 깍이면 안되고..,
    if ( m_kInventory.FindItemByItemID( SiKGCHelper()->GetDungeonFreePassID() ) ) {
        START_LOG( clog, L"던전 프리패스 소유자는 통과." ) << END_LOG;
        return true;
    }

    // 난이도 정보가 없음.
    std::set<int> setDifficulty;
    _JIF( SiKResultManager()->GetDifficulty( nGameMode_, setDifficulty ), return false );

    // 모드 정보가 없음.
    KDungeonUserInfo kInfo;
    _JIF( GetDungeonUserInfo( nGameMode_, kInfo ), return false );

    std::set<int>::iterator sit;
    for ( sit = setDifficulty.begin() ; sit != setDifficulty.end() ; ++sit ) {
        if ( *sit >= nDifficulty_ ) {
            break;
        }

        if ( false == kInfo.m_kModeDifficulty.Get( *sit ) ) {
            START_LOG( cwarn, L"이전 단계를 모두 클리어하지 못했다. Difficulty : " << nDifficulty_ << L", Checked Difficulty : " << *sit ) << END_LOG;
            return false;
        }
    }

    if ( SiKGCHelper()->HellItemCheckEnable( nGameMode_ ) ) {
        _JIF( GetDurItemCount( SiKGCHelper()->GetHellItemID() ) > 0, return false );
    }

    return true;
}

bool KUser::BreakModeCheck(int nGameMode_, int nDifficulty_)
{
    START_LOG(clog, L" Mode : " << nGameMode_ << L", Difficult : " << nDifficulty_) << END_LOG;

    if (IsAdmin()) {
        START_LOG(clog, L"운영자는 검사 안함.") << END_LOG;
        return true;
    }

    if (false == KRoom::IsDungeonMode(nGameMode_)) {
        START_LOG(clog, L"던전 모드가 아니면 통과 : " << nGameMode_) << END_LOG;
        return true;
    }

    if (nDifficulty_ < GC_GMD_BREAK_BEGIN) {
        START_LOG(clog, L"던전 모드 인데 난이도가 헬 모드가 아니면 통과 : " << nDifficulty_) << END_LOG;
        return true;
    }

    // 100427,. 
    // 던전 프리패스 모드 추가.
    // 모든 던전에 출입가능( 클라이언트에서 작업 했음 )
    // 챔피언 모드 들어가 져야 하고.., 챔피언 모드 입장권도 깍이면 안되고..,
    if (m_kInventory.FindItemByItemID(SiKGCHelper()->GetDungeonFreePassID())) {
        START_LOG(clog, L"던전 프리패스 소유자는 통과.") << END_LOG;
        return true;
    }

    // 난이도 정보가 없음.
    std::set<int> setDifficulty;
    _JIF(SiKResultManager()->GetDifficulty(nGameMode_, setDifficulty), return false);

    // 모드 정보가 없음.
    KDungeonUserInfo kInfo;
    _JIF(GetDungeonUserInfo(nGameMode_, kInfo), return false);

    std::set<int>::iterator sit;
    for (sit = setDifficulty.begin(); sit != setDifficulty.end(); ++sit) {
        if (*sit >= nDifficulty_) {
            break;
        }

        if (false == kInfo.m_kModeDifficulty.Get(*sit)) {
            START_LOG(cwarn, L"이전 단계를 모두 클리어하지 못했다. Difficulty : " << nDifficulty_ << L", Checked Difficulty : " << *sit) << END_LOG;
            return false;
        }
    }

    return true;
}

void KUser::CheckHellItemConnection( int nSessionInfo_ )
{
    if( nSessionInfo_ != SI_NEW_ACCOUNT &&
        nSessionInfo_ != SI_TODAY_FIRST_LOGIN_NO_LAS &&
        nSessionInfo_ != SI_TODAY_FIRST_LOGIN_LAS ) return;

    CheckHellItemAttendance( SiKGCHelper()->GetConnectionHellCount() );
}

void KUser::CheckHellItemAttendance( IN int nCount_ )
{
    int nCurrentHellItemCount = GetDurItemCount( SiKGCHelper()->GetHellItemID() ); // 현재 보유 티켓 개수
    int nMaxTicketNum = SiKGCHelper()->GetMaximumTicketNum(); // 티켓 최대치 제한 개수(-1:제한없음)
    int nAddTicketCount = nCount_; // 받을 입장권 수량

    // 티켓 최대치 제한 있을 경우.
    if( nMaxTicketNum >= 0 )
    {
        if( nCurrentHellItemCount >= nMaxTicketNum )
        {
            START_LOG( clog, L"보유 최대치 이상이면  지급하지 않음." )
                << BUILD_LOG( nCurrentHellItemCount )
                << BUILD_LOG( nMaxTicketNum )
                << BUILD_LOG( nAddTicketCount ) << END_LOG;
            return;
        }

        // 보유 최대치 넘어가지 않도록 지급수량 조정
        if( nCurrentHellItemCount + nAddTicketCount > nMaxTicketNum )
        {
            nAddTicketCount = nMaxTicketNum - nCurrentHellItemCount;
        }
    }

    if( nAddTicketCount <= 0 ) //1개 이상 증가할때만.
    {
        START_LOG( cwarn, L"실제 받을 양이 0이하이다." )
            << BUILD_LOG( nCurrentHellItemCount )   // 현재 수량
            << BUILD_LOG( nMaxTicketNum )           // 수량 제한
            << BUILD_LOG( nCount_ )                 // 요청한 입장권 수량
            << BUILD_LOG( nAddTicketCount ) << END_LOG;        // 실제 받을 수량
        return;
    }

    QUEUEING_EVENT_TO_DB_CHECK( EVENT_GET_HELL_ITEM_REQ, nAddTicketCount );
END_PROC:
    START_LOG( clog, L"획득할 챔피언모드 티켓 개수 : " << nAddTicketCount ) << END_LOG;
}

void KUser::SetAbuseToDB()
{
    QUEUEING_ID_TO_DB( EVENT_SET_ABUSE_TO_DB_REQ );
}

void KUser::UpdateCollectionInfo( KEndGameAck& kPacket_ )
{
    std::vector<KGameResultOut>::iterator vit;
    for( vit = kPacket_.m_vecGameResult.begin() ; vit != kPacket_.m_vecGameResult.end() ; ++vit )
    {
        vit->m_vecCollectionSlot.clear();

        KUserPtr spUser = SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( vit->m_strLogin );
        if( !spUser )
            continue;

        vit->m_vecCollectionSlot = spUser->m_vecCollectionSlot;
        //spUser->DumpCollectionSlot();
    }
}

void KUser::UpdateCollectionInfo( IN std::vector<KCollectionData>& vecCollectionSlot_ )
{
    vecCollectionSlot_ = m_vecCollectionSlot;
}

void KUser::DumpCollectionSlot()
{
    //START_LOG( clog, L"Name : " << m_strName  ) << END_LOG;
    //std::vector<KCollectionData>::iterator vit;
    //for( vit = m_vecCollectionSlot.begin() ; vit != m_vecCollectionSlot.end() ; ++vit )
    //{
    //    START_LOG( clog, L"Collection MissionID : " << vit->m_dwMissionID  ) << END_LOG;
    //    std::map<int,int>::iterator mit;
    //    for( mit = vit->m_mapCompletionInfo.begin() ; mit != vit->m_mapCompletionInfo.end() ; ++mit )
    //    {
    //        std::clog << L"{" << mit->first << L"," << mit->second << L"},";
    //    }
    //    std::clog << dbg::endl;
    //}

}

// DB에 저장되어 있는 컬렉션 데이터와 스크립트에 변경사항이 생기면 처리하는 함수
void KUser::CheckAndRegistCollection( std::vector<KCollectionData>& vecCollectionSlot_ )
{ // 파라미터는 디비에서 넘어온 것
  // db 데이터와 스크립트에서 읽은 데이터 비교해서 다르면 
    std::vector<KCollectionData>::iterator vit;
    for( vit = vecCollectionSlot_.begin(); vit != vecCollectionSlot_.end(); ++vit )
    {
        KCollectionData kCollectionData; // for 밖에 빼두었다간 맵에 데이터가 누적 ㅠㅠ

        if( vit->m_dwState == 2 ) // 컬렉션 미션 완료상태이면
            continue;

        if( !SiKCollectionManager()->GetMissionData( kCollectionData, vit->m_dwMissionID, false ) )
        {   // 스크립트에서 로딩한 미션 정보 가져오기
            START_LOG( cerr, L"컬렉션 미션 데이터 얻기 실패, name : " << m_strName )
                << BUILD_LOG( vit->m_dwMissionID ) << END_LOG;
        }

        // DB에서 퍼온 컬렉션 미션의 서브미션 정보가 없을 때, 스크립트에서 가져온 서브미션을 넣음
        std::map<int,int>::iterator mit;
        if( kCollectionData.m_dwMissionID == vit->m_dwMissionID ) // 다를리가 없잖아;;
        {
            for( mit = kCollectionData.m_mapCompletionInfo.begin() ; mit != kCollectionData.m_mapCompletionInfo.end() ; ++mit )
            {
                if( vit->m_mapCompletionInfo.end() == vit->m_mapCompletionInfo.find( mit->first ) )
                { // db에서 퍼온 컬렉션 정보에 서브미션 정보에 추가되는 스크립트 서브미션이 있을 경우 추가
                    vit->m_mapCompletionInfo.insert( std::make_pair( mit->first, 0) );
                }
            }
            
        }
    }
    //DumpCollectionSlot();
}

KCollectionData* KUser::GetCollectionDataFromSlot( DWORD dwMissionID_ )
{
    std::vector<KCollectionData>::iterator vit;
    vit = std::find_if( m_vecCollectionSlot.begin(), m_vecCollectionSlot.end(),
        boost::bind( &KCollectionData::m_dwMissionID, _1 ) == dwMissionID_ );

    if( vit == m_vecCollectionSlot.end() )
        return NULL;
    return &(*vit);
}

void KUser::Indigo_Log( const KGameResultIn& kResult_, const int nGetRp_, const char* szIPStr_ )
{
    if( !SiKGSSimLayer()->m_bEnableIndigoLog ) return;
    SiKLogManager()->Get( KLogManager::LT_INDIGO )
        << KNC_TIME_STRING << kResult_.m_strLogin
        << L"," << (kResult_.m_bWin ? L"True" : L"False")
        << L"," << nGetRp_
        << L"," << kResult_.m_iNumKills
        << L"," << kResult_.m_iNumDies
        << L", " << KncUtil::toWideString( szIPStr_ )
        << GLOG::endl;
}

void KUser::End_Indigo_Log()
{
    if( !SiKGSSimLayer()->m_bEnableIndigoLog ) return;
    SiKLogManager()->Get( KLogManager::LT_INDIGO ) << L"-------------------------------------------------------------------" << GLOG::endl;
}

void KUser::LeaveGameProcess()
{
    JIF( m_pkRoom );
    KInDoorUserInfo* pkStartingInfo = m_pkRoom->FindInDoorUserInfo( m_strName );
    JIF( pkStartingInfo != NULL );

    KCharacterInfo* pkChar = GetCharacterInfo( pkStartingInfo->m_cCharIndex );
    JIF( pkChar );

    if( m_pkRoom->m_iGameMode == GC_GM_SURVIVAL_MODE || m_pkRoom->m_iGameMode == GC_GM_TEAM_MODE  ||
        m_pkRoom->m_iGameMode == GC_GM_GUILD_BATTLE  || m_pkRoom->m_iGameMode == GC_GM_TAG_TEAM   ||
        m_pkRoom->m_iGameMode == GC_GM_TAG_SURVIVAL  || m_pkRoom->m_iGameMode == GC_GM_DEATH_TEAM )
    {
        pkChar->m_iLose += 1;
    }

    if( m_pkRoom->m_iGameMode == GC_GM_INDIGO_TEAM && SiKGSSimLayer()->CheckTypeFlag( ST_INDIGO ) ) {
        m_iIndigoLose += 1;
        int nGetRp = IndigoUtil::GetRPChange( m_iRP, m_pkRoom->m_prAvgRP,
            pkStartingInfo->m_iTeam == 1, false, 0 );

        m_iRP += nGetRp;
    }

    if( KRoom::IsPvPMode(m_pkRoom->m_iGameMode)) {
        pkChar->m_kELOUserData.m_nELOLose += 1;

        if( pkChar->m_kELOUserData.m_nELOType == KELOUserData::MT_PLACEMENTTEST ) {
            SiKPVPMatchManager()->CalcPlacementTestRatingPoint( m_pkRoom->m_prELOTeamRPAvg,
                pkStartingInfo->m_iTeam == 1,
                false,
                pkChar->m_kELOUserData.m_nConstantK, 
                pkChar->m_kELOUserData.m_nMatchTotalCount,
                pkChar->m_kELOUserData.m_nLastWinLose,
                pkChar->m_kELOUserData.m_nRatingPoint );
        }
        else {
            SiKPVPMatchManager()->CalcRatingPoint( m_pkRoom->m_prELOTeamRPAvg,
                pkStartingInfo->m_iTeam == 1,
                false,
                pkChar->m_kELOUserData.m_nRatingPoint );
        }

        pkChar->m_kELOUserData.m_nMatchTotalCount += 1;
        SiKPVPMatchManager()->CalcCharELOGrade( pkChar->m_kELOUserData.m_nRatingPoint, pkChar->m_kELOUserData.m_ucGrade );
        SiKPVPMatchManager()->CalcELOType( pkChar->m_kELOUserData.m_nMatchTotalCount, pkChar->m_kELOUserData.m_nELOType );
        KELOUserData kELOUserData = pkChar->m_kELOUserData;

        // DB에 ELO 계산된 정보 저장.
        KDB_EVENT_MATCH_CHAR_DATA_REQ kDBPacket;
        SetELOResultInfo( pkChar->m_cCharType, kELOUserData, kDBPacket );
        QUEUEING_EVENT_TO_DB( DB_EVENT_MATCH_CHAR_DATA_REQ, kDBPacket );

        // 캐릭터 ELO 데이터 DB 초기값이랑 동기화 작업.
        CalcCharELOData( pkChar->m_kELOUserData );

        // 대전 매칭 통계 작업.
        KDB_EVENT_MATCH_ELO_RESULT_REQ kDBStatPacket;
        DWORD dwDurationTime = GetMatchDurationTime();
        InitMatchTick();
        SetELOResultStatInfo( KDB_EVENT_MATCH_ELO_RESULT_REQ::ST_LEAVE, GetUID(), pkChar->m_cCharType, pkChar->m_dwLevel, m_pkRoom->m_prELOTeamRPAvg, pkStartingInfo->m_iTeam, kELOUserData, m_pkRoom->m_nModeInfo, false, dwDurationTime, kDBStatPacket );
        GetMatchSuccessLoopCount( kDBStatPacket.m_nLoopCount );
        kDBStatPacket.m_nTotalPartyNum = m_pkRoom->GetNormalUserNum(); // 현재 룸에 남아 있는 유저.
        QUEUEING_EVENT_TO_DB( DB_EVENT_MATCH_ELO_RESULT_REQ, kDBStatPacket );
    }
}

bool KUser::IsBestMiniGameRecord( KMiniGameRankInfo& kMiniGameInfo_ )
{
    std::vector<KMiniGameRankInfo>::iterator vit;

    // 해당하는 미니게임 모드를 찾는다
    vit = std::find_if( m_vecMiniGameInfo.begin(), m_vecMiniGameInfo.end(), 
        boost::bind( &KMiniGameRankInfo::m_cGameMode, _1 ) == kMiniGameInfo_.m_cGameMode ); 

    if( vit == m_vecMiniGameInfo.end() )
    {
        SET_ERROR( ERR_MINIGAME_04 );
        START_LOG( cerr, L"해당되는 미니게임 없음, Name : " << m_strName )
            << BUILD_LOGc( kMiniGameInfo_.m_cGameMode )
            << BUILD_LOG( kMiniGameInfo_.m_dwScore ) << END_LOG;
        return false;
    }

    if( kMiniGameInfo_.m_dwScore > vit->m_dwBestScore ) // 기존 최고 기록을 갱신
    {
        START_LOG( clog, L"미니게임 내 최고기록 갱신, Name : " << m_strName )
            << BUILD_LOG( kMiniGameInfo_.m_dwScore )
            << BUILD_LOG( vit->m_dwBestScore ) << END_LOG;

        vit->m_dwBestScore = kMiniGameInfo_.m_dwScore;
        kMiniGameInfo_.m_dwBestScore = vit->m_dwBestScore; //패킷으로 보낼 최고 점수 갱신
        
        return true;
    }
    return false;
}

bool KUser::StartMiniGame()
{
    bool bOK;
    _JIF( m_spSquare, return false );
    bOK = m_spSquare->StartMiniGame( GetUID() );

    StateTransition( KUserFSM::INPUT_START_MINIGAME );
    
    return bOK;
}

bool KUser::GetMiniGameBestScore( IN OUT KMiniGameRankInfo& kMiniGameInfo_ )
{
    std::vector<KMiniGameRankInfo>::iterator vit;

    // 해당하는 미니게임 모드를 찾는다
    vit = std::find_if( m_vecMiniGameInfo.begin(), m_vecMiniGameInfo.end(), 
        boost::bind( &KMiniGameRankInfo::m_cGameMode, _1 ) == kMiniGameInfo_.m_cGameMode ); 

    if( vit == m_vecMiniGameInfo.end() )
    {
        SET_ERROR( ERR_MINIGAME_04 );
        START_LOG( cerr, L"해당되는 미니게임 없음, Name : " << m_strName )
            << BUILD_LOGc( kMiniGameInfo_.m_cGameMode )
            << BUILD_LOG( kMiniGameInfo_.m_dwScore ) << END_LOG;
        return false;
    }

    kMiniGameInfo_.m_dwBestScore = vit->m_dwBestScore;

    return true;

}

void KUser::ServerTimeNot()
{
    KEVENT_SERVER_TIME_NOT kPacket;
    CTime tmTime = CTime::GetCurrentTime();
    kPacket.m_tCurrentTime = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    kPacket.m_nYear = tmTime.GetYear();
    kPacket.m_nMonth = tmTime.GetMonth();
    kPacket.m_nDay = tmTime.GetDay();
    kPacket.m_nHour = tmTime.GetHour();
    kPacket.m_nMinute = tmTime.GetMinute();
    kPacket.m_nSecond = tmTime.GetSecond();

    SEND_PACKET( EVENT_SERVER_TIME_NOT );
}

void KUser::ChatNotice( std::wstring& strMsg_ )
{
    KEventPtr spEvent( new KUserEvent );

    KChatData kPacket;
    kPacket.m_cChatType         = KChatData::MSG_NOTIFY;
    kPacket.m_strChatMsg        = strMsg_;

    spEvent->m_usEventID = KUserEvent::EVENT_CHAT_NOT;

    // make event object
    KSerializer         ks;
    // serialize - only data
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( kPacket );
    ks.EndWriting();

    spEvent->m_kbuff.Compress();

    // serialize - full event object
    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();

    SiKChannelManager()->QueueingMsg( kbuff );
    //SiKSquareManager()->Send_NotMsg( kPacket.m_strChatMsg );  // 광장에 있는 유저는 Channel Lobby에 포함 되어 있으므로 다시 보낼필요 없음.
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_LOUD_MSG_REQ,  kbuff );
}

void KUser::AddStatisticsMiniGame( KMiniGameRankInfo& kMiniGameInfo_, DWORD nLevel_, DWORD dwElapsedSecond_ ) // dwElapsedSecond_ 는 playtime이다
{
    KStatMiniGamePtr spStat = GET_STAT( KStatMiniGame, SI_STAT_MINIGAME );
    KStatKeyMiniGame kKey;

    kKey.m_nLevel = nLevel_;
    kKey.m_cGameMode = kMiniGameInfo_.m_cGameMode;
    kKey.m_nHitRange = kMiniGameInfo_.m_dwScore / 30; // 0~29 : 0, 30 ~ 59 : 1 , 이런식.

    if( kKey.m_nHitRange > 33 ) // score 은 990 점이 최대
        kKey.m_nHitRange = 33;
    
    JIF( spStat );
    
    dwElapsedSecond_ /= ( 60 * 1000 ); // 플레이타임을 분단위로 바꾼다
    
    if( dwElapsedSecond_ == 0 ) // 1분 미만 플레이는 1분 플레이 한 것으로 처리
        dwElapsedSecond_ += 1;

    spStat->AddStat( kKey, dwElapsedSecond_ );
}

bool KUser::IsTrainedSkill( SSkillElement& kSkill_ )
{
    std::map<std::pair<char,char>, KSPInfo >::iterator mit;
    std::vector<int>::iterator vit;
    if( SiKSkillManager()->CheckSkillExceptionID( kSkill_.m_cCharType, kSkill_.m_nID ) ) {
        return true;
    }
    mit = m_mapTrainedSkill.find( std::make_pair(kSkill_.m_cCharType, kSkill_.m_cPromotion) );
    if( mit == m_mapTrainedSkill.end() ) return false;

    bool bResult = std::find( mit->second.m_vecSkills.begin(), mit->second.m_vecSkills.end(), kSkill_.m_nID ) != mit->second.m_vecSkills.end();
    if( bResult == false ) {
        return false;
    }
    return true;
}

bool KUser::IsTrainedPreSkill( SSkillElement& kSkill_ )
{
    if( kSkill_.m_setPreList.empty() ) return true;
    std::set<int>::iterator sit;
    bool bResult = true;
    for( sit = kSkill_.m_setPreList.begin() ; sit != kSkill_.m_setPreList.end() ; ++sit )
    {
        SSkillElement kSkill;
        if( !SiKSkillManager()->GetSkill( *sit, kSkill ) )
        {
            START_LOG( cwarn, L"해당 스킬이 스크립트에 존재하지 않음. ID: " << kSkill.m_nID ) << END_LOG;
            bResult = false;
            break;
        }
        if( kSkill.m_bBase ) continue; // 베이스 스킬은 깍뚜기..

        if ( kSkill_.m_bOrCheckPreList == true ) {
            bResult = false; //m_nCount 안쓰고 bResult 조작으로 OrCheckPreList 처리를 함.
            if( IsTrainedSkill( kSkill ) ) { 
                bResult = true;
                break;
            }
        }
        else {
            if( !IsTrainedSkill( kSkill ) ) {
                bResult = false;
                break;
            }
        }
    }

    return bResult;    
}

bool KUser::ExistTrainedNextSkill( SSkillElement& kSkill_ )
{
    if( kSkill_.m_setNextList.empty() ) return false;
    std::set<int>::iterator sit;
    for( sit = kSkill_.m_setNextList.begin() ; sit != kSkill_.m_setNextList.end() ; ++sit )
    {
        SSkillElement kSkill;
        if( !SiKSkillManager()->GetSkill( *sit, kSkill ) )
        {
            START_LOG( cwarn, L"해당 스킬이 스크립트에 존재하지 않음. ID: " << kSkill.m_nID ) << END_LOG;
            return false;
        }
        if( kSkill.m_bBase ) continue; // 베이스 스킬은 깍뚜기..
        if( IsTrainedSkill( kSkill ) ) return true;
    }
    return false;
}

int KUser::GetLeftSpPoint( char cCharType_ )
{
    const KCharacterInfo* pkChar = GetCharacterInfo( cCharType_ );
    if( !pkChar )
    {
        // Log
        return false;
    }
    //int nTotalSp = SiKSkillManager()->GetAccSPCount( pkChar->m_cCharType ,pkChar->m_kSkillInfo.m_nLv );
    int nTotalSp = pkChar->m_kSkillInfo.m_nLvSPPoint;
    int nUserSpCount = GetUseSpCount( pkChar->m_cCharType, pkChar->m_cPromotion );

    return nTotalSp - nUserSpCount; //std::max<int>( nTotalSp - nUserSpCount, 0 );
}

int KUser::GetUseSpCount( char cCharType_, char cPromotion_ )
{
    int nCount = 0;
    for( char c = -1 ; c <= cPromotion_ ; ++c )
    {
        std::map<std::pair<char,char>, KSPInfo >::iterator mit;
        mit = m_mapTrainedSkill.find( std::make_pair(cCharType_, c) );
        if( mit != m_mapTrainedSkill.end() )
        {
            nCount += SiKSkillManager()->CalcSpendSP( mit->second.m_vecSkills );
        }
    }
    return nCount;
}

bool KUser::AddTrainedSkill( SSkillElement& kSkill_ )
{
    std::map<std::pair<char,char>, KSPInfo >::iterator mit;
    std::pair<char,char> prKey(kSkill_.m_cCharType,kSkill_.m_cPromotion);
    mit = m_mapTrainedSkill.find( prKey );
    // 스킬이 없을 경우에 대하여 추가.
    if( mit == m_mapTrainedSkill.end() ) {
        KSPInfo kSpInfo;
        kSpInfo.m_cCharType = kSkill_.m_cCharType;
        kSpInfo.m_cPromotion = kSkill_.m_cPromotion;
        kSpInfo.m_vecSkills.push_back( kSkill_.m_nID );
        m_mapTrainedSkill[prKey] = kSpInfo;
    } else {
        m_mapTrainedSkill[prKey].m_vecSkills.push_back( kSkill_.m_nID );
    }

    if( !RemoveReserveHandleSkill( m_setDeleteReserveSkill, kSkill_.m_nID ) )
    {
        LIF( AddReserveHandleSkill( m_setAddReserveSkill, kSkill_.m_nID ) );
    }

    return true;
}

bool KUser::RemoveTrainedSkill( SSkillElement& kSkill_ )
{
    std::pair<char,char> prKey(kSkill_.m_cCharType,kSkill_.m_cPromotion);
    std::map<std::pair<char,char>, KSPInfo >::iterator mit;
    mit = m_mapTrainedSkill.find( prKey );
    if( mit == m_mapTrainedSkill.end() )
    {
        START_LOG( cwarn, L"스킬 삭제시 해당 스킬에 맞는 캐릭,전직이 없음. ID : " << kSkill_.m_nID ) << END_LOG;
        return false;
    }

    std::vector<int>::iterator vit;
    vit = std::find( mit->second.m_vecSkills.begin(), mit->second.m_vecSkills.end(), kSkill_.m_nID );
    if( vit == mit->second.m_vecSkills.end() )
    {
        START_LOG( cwarn, L" 배우지 않은 스킬을 삭제 하려고 함. ID : " << kSkill_.m_nID ) << END_LOG;
        return false;
    }

    mit->second.m_vecSkills.erase( vit );
    if( !RemoveReserveHandleSkill( m_setAddReserveSkill, kSkill_.m_nID ) )
    {
        LIF( AddReserveHandleSkill( m_setDeleteReserveSkill, kSkill_.m_nID ) );
    }
    return true;
}

bool KUser::VerifySetInfo( IN char cCharType_, IN char cPromotion_, IN KSPSetInfo& kSetInfo_ )
{
    const KCharacterInfo* pkChar = GetCharacterInfo( cCharType_ );
    _JIF( pkChar, SET_ERROR( ERR_SKILL_14 ); return false );
    _JIF( pkChar->m_cPromotion >= 0 && pkChar->m_cPromotion >= cPromotion_, SET_ERROR( ERR_SKILL_14 ); return false );
    _JIF( kSetInfo_.m_nSPSetID >= 0 && kSetInfo_.m_nSPSetID < SiKSkillManager()->GetMaxSkillSetNum(), SET_ERROR( ERR_SKILL_15 ); return false );

    std::vector<KSkillSlot>::iterator vit;
    for( vit = kSetInfo_.m_vecSkills.begin() ; vit != kSetInfo_.m_vecSkills.end() ; ++vit )
    {
        SSkillElement sSkill;
        _JIF( SiKSkillManager()->GetSkill( vit->m_nSkillID, sSkill ), SET_ERROR( ERR_SKILL_00 ); return false );
        _JIF( IsTrainedSkill( sSkill ), SET_ERROR( ERR_SKILL_16 ); return false );
    }
    return true;
}

bool KUser::CheckSettingID( IN std::map< std::pair<char,char>, int >& mapSetting_ )
{
    std::map< std::pair<char,char>, int >::iterator mit;
    for( mit = mapSetting_.begin() ; mit != mapSetting_.end() ; ++mit )
    {
        const KCharacterInfo* pkChar = GetCharacterInfo( mit->first.first );
        _JIF( pkChar, SET_ERROR( ERR_SKILL_14 ); return false );
        _JIF( pkChar->m_cPromotion >= 0 && pkChar->m_cPromotion >= mit->first.second, SET_ERROR( ERR_SKILL_14 ); return false );
        _JIF( mit->second >= 0 && mit->second < 2, SET_ERROR( ERR_SKILL_15 ); return false );
    }
    return true;
}

bool KUser::ApplyChangeSkillSet( KChangeSkillSet& kChangeSkillSet_ )
{
    std::map< std::pair<char,char>, int >::iterator mit;
    for( mit = kChangeSkillSet_.m_mapSetting.begin() ; mit != kChangeSkillSet_.m_mapSetting.end() ; ++mit )
    {
        KCharacterInfo* pkChar = GetCharacterInfo( mit->first.first );
        _JIF( pkChar, SET_ERROR( ERR_SKILL_14 ); return false ); // 이 일은 절대 일어나선 않된다.
        pkChar->m_mapEquipSkillSet[mit->first.second] = mit->second;
    }

    AssignSkillSets( kChangeSkillSet_.m_mapSkillSets, m_mapSkillSets );
    DumpSkillSet( m_mapSkillSets );
    return true;
}

void KUser::GetSkillSetEquip( std::map< std::pair<char,char>, int >& mapSetting_ )
{
    mapSetting_.clear();

    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {

        std::map<char, int>::const_iterator cmitSkill;
        for ( cmitSkill = cmit->second.m_mapEquipSkillSet.begin() ; cmitSkill != cmit->second.m_mapEquipSkillSet.end() ; ++cmitSkill ) {
            std::pair<char,char> prKey( cmit->first, cmitSkill->first );
            mapSetting_[prKey] = cmitSkill->second;
        }
    }
}

// KRoom 에서도 접근하고, User 에서도 불러쓰는 함수. 캐릭터의 스킬트리 (장착??) 장착처럼 보이는 함수는 위에
// 있는데. 위의 함수는 세트 번호이다. 
void KUser::GetSkillSettingData( IN const char& cCharType_, OUT std::map<char, std::vector<KSkillSlot> >& mapEquipSkillSet_ )
{
    mapEquipSkillSet_.clear();
    const KCharacterInfo* pkChar = GetCharacterInfo( cCharType_ );
    JIF( pkChar ); // 일어 나면 안됨.
    std::map<char, int>::const_iterator cmit;
    for( cmit = pkChar->m_mapEquipSkillSet.begin() ; cmit != pkChar->m_mapEquipSkillSet.end() ; ++cmit )
    {
        std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit2;
        mit2 = m_mapSkillSets.find( std::make_pair( cCharType_, cmit->first ) );
        if( mit2 == m_mapSkillSets.end() ) continue;

        std::vector<KSPSetInfo>::iterator vit;
        for( vit = mit2->second.begin() ; vit != mit2->second.end() ; ++vit )
        {
            if( vit->m_nSPSetID == cmit->second )
            {
                mapEquipSkillSet_[cmit->first] = vit->m_vecSkills;
            }
        }
    }
}

void KUser::AddNewSkillSet( char cCharType_, char cPromotion_ )
{
    std::pair<char,char> prKey( cCharType_, cPromotion_ );
    std::vector<KSPSetInfo> vecSetInfo;
    KSPSetInfo setInfo;
    for( int i = 0 ; i < SiKSkillManager()->GetMaxSkillSetNum() ; ++i )
    {
        setInfo.m_nSPSetID = i;
        vecSetInfo.push_back( setInfo );
    }
    m_mapSkillSets.insert( std::make_pair( prKey, vecSetInfo ) );
}

std::wostream& operator<<( std::wostream& stm_, const KSkillSlot& data_ )
{
    return stm_ << L"(" << data_.m_nSlotID << L" , " << data_.m_nSkillID << L")";
}

void KUser::DumpSkillSet( std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapSkillSets )
{
    //if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) return;
    //std::wstringstream stm;
    //stm << L"====== Skill Set Dump ======" << std::endl
    //    << L"Name : " << m_strName << std::endl;

    //std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;
    //for( mit = mapSkillSets.begin() ; mit != mapSkillSets.end() ; ++mit )
    //{
    //    std::vector<KSPSetInfo>::iterator vit;
    //    for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
    //    {
    //        stm << L"["
    //            << (int) mit->first.first << L","
    //            << (int) mit->first.second << L","
    //            << vit->m_nSPSetID << L"]";
    //        std::copy( vit->m_vecSkills.begin(), vit->m_vecSkills.end(), 
    //            std::ostream_iterator<std::vector<KSkillSlot>::value_type,wchar_t>( stm, L"," ) );
    //        stm << std::endl;

    //    }
    //}
    //std::Warn << stm.str();

    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

void KUser::DumpEquipSkillSet( std::wstring& strLogin_, char cCharType_, std::map<char, std::vector<KSkillSlot> >& mapEquipSkillSet_ )
{
    //if( !SiKGSSimLayer()->CheckTypeFlag( ST_DP_LOG ) ) return;
    //std::wstringstream stm;
    //std::map<char, std::vector<KSkillSlot> >::iterator mit;
    //stm << L"===== Dump Equip Skill Set =====" << std::endl;
    //for( mit = mapEquipSkillSet_.begin() ; mit != mapEquipSkillSet_.end() ; ++mit )
    //{
    //    stm << strLogin_ << L" [" <<  (int)cCharType_ << L"," << (int)mit->first << L"] : ";
    //    std::copy( mit->second.begin(), mit->second.end(), 
    //        std::ostream_iterator<std::vector<KSkillSlot>::value_type,wchar_t>( stm, L"," ) );
    //    stm << std::endl;
    //}
    //std::Warn << stm.str();

    //stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
    //stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
}

int KUser::GetSpentSP( char cChar_, char cPromotion_ )
{
    std::map<std::pair<char,char>, KSPInfo >::iterator mit;
    mit = m_mapTrainedSkill.find( std::make_pair(cChar_, cPromotion_) );
    if( mit == m_mapTrainedSkill.end() ) {

        std::map<char,KCharacterInfo>::const_iterator cmit;
        cmit = m_mapCharacterInfo.find( cChar_ );
        if ( cmit == m_mapCharacterInfo.end() ) {
            return 1000; // 무슨 숫자인고? 없는 캐릭터라면 ...1000  , 발생하면 안되는 경우인가
        }

        return 0;
    }

    return SiKSkillManager()->CalcSpendSP( mit->second.m_vecSkills );
}
  
void KUser::AssignSkillSets( IN std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapFrom,
                            OUT std::map<std::pair<char,char>, std::vector<KSPSetInfo> >& mapTo )
{
    SiKSkillManager()->InitSkillSets( m_mapCharacterInfo, mapTo );
    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;
    std::vector<KSPSetInfo>::iterator vit;
    std::vector<KSPSetInfo>::iterator vit1;
    for( mit = mapFrom.begin() ; mit != mapFrom.end() ; ++mit )
    {
        for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
        {
            for( vit1 = mapTo[mit->first].begin() ; vit1 != mapTo[mit->first].end() ; ++vit1 )
            {
                if( vit1->m_nSPSetID == vit->m_nSPSetID )
                {
                    vit1->m_vecSkills = vit->m_vecSkills;
                }
            }
        }
    }
}

void KUser::MakeChangeSkillSet( KChangeSkillSet& kChangeSkillSet_ )
{
    kChangeSkillSet_.m_mapSetting.clear();
    kChangeSkillSet_.m_mapSkillSets = m_mapSkillSets;

    std::map<char, KCharacterInfo>::const_iterator cmit;
    for ( cmit = m_mapCharacterInfo.begin() ; cmit != m_mapCharacterInfo.end() ; ++cmit ) {

        std::map<char,int>::const_iterator cmitSkill;
        for ( cmitSkill = cmit->second.m_mapEquipSkillSet.begin() ; cmitSkill != cmit->second.m_mapEquipSkillSet.end() ; ++cmitSkill ) {
            std::pair<char,char> prKey( cmit->first, cmitSkill->first );
            kChangeSkillSet_.m_mapSetting[prKey] = cmitSkill->second;
        }
    }
}

bool KUser::IsInUnLockedGroup( char cCharType_, int nLockGroupID_ )
{
    std::map<char, std::set<int> >::iterator mit;
    mit = m_mapUnLockGroupID.find( cCharType_ );
    if( mit == m_mapUnLockGroupID.end() ) return false;
    return (mit->second.find( nLockGroupID_ ) != mit->second.end());
}

void KUser::AddUnLockedGroup( char cCharType_, int nLockGroupID_ )
{
    LIF( m_mapUnLockGroupID[cCharType_].insert( nLockGroupID_ ).second );
}

void KUser::AddStatistics2( IN const KGameInfo& kGameInfo_, IN OUT KEndGameAck& kEndGameAck_, IN const std::map<std::pair<int,int>, int>& mapStageMonKillCount_ )
{
    KStatSpxPtr spStat = GET_STAT( KStatSpx, SI_STAT_SPX );
    JIF( spStat );

    std::vector<KGameResultOut>::iterator vit;
    for( vit = kEndGameAck_.m_vecGameResult.begin() ; vit != kEndGameAck_.m_vecGameResult.end() ; ++vit )
    {
        std::vector< std::pair<char,KCharacterExpInfoOut> >::iterator vit1;
        for( vit1 = vit->m_vecExps.begin() ; vit1 != vit->m_vecExps.end() ; ++vit1 )
        {
            if( vit1->second.m_kOldSkillInfo.m_nLvSPPoint == vit1->second.m_kSkillInfo.m_nLvSPPoint ) continue;
            spStat->AddStat( kGameInfo_.m_iGameMode, vit1->first, SiKResultManager()->Exp_2_Level( vit1->second.m_biExp ),
                             vit1->second.m_kSkillInfo.m_nLvSPPoint, vit1->second.m_kSkillInfo.m_nMaxSPPoint );
        }
    }

    if( !mapStageMonKillCount_.empty() ) {
        KDB_EVENT_ROOM_MONSTER_KILL_STAT kMonKillPacket;
        kMonKillPacket.m_nModeID = kGameInfo_.m_iGameMode;
        kMonKillPacket.m_nDifficult = kGameInfo_.m_nDifficulty;
        kMonKillPacket.m_dwUserUID = GetUID();
        kMonKillPacket.m_nPartyCount = kGameInfo_.m_nStartUserNum;
        kMonKillPacket.m_mapStageMonKillCount = mapStageMonKillCount_;

        QUEUEING_EVENT_TO_DB( DB_EVENT_ROOM_MONSTER_KILL_STAT, kMonKillPacket );
    }
}

void KUser::ReleaseEquipSkill( SSkillElement& kSkill_ )
{
    std::pair<char,char> prKey( kSkill_.m_cCharType, kSkill_.m_cPromotion );
    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;
    mit = m_mapSkillSets.find( prKey );
    if( mit == m_mapSkillSets.end() ) return;

    std::vector<KSPSetInfo>::iterator vit;
    for( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit )
    {
        std::vector<KSkillSlot>::iterator vit1;
        vit1 = std::find_if( vit->m_vecSkills.begin(), vit->m_vecSkills.end(),
            boost::bind( &KSkillSlot::m_nSkillID, _1 ) == kSkill_.m_nID );
        if( vit1 != vit->m_vecSkills.end() )
            vit->m_vecSkills.erase( vit1 );

    }
}

bool KUser::IsLockedGroup( SSkillElement& kSkill_ )
{
    if( kSkill_.m_nLockGroupID < 0 ) return false;
    std::map<char, std::set<int> >::iterator mit;
    mit = m_mapUnLockGroupID.find( kSkill_.m_cCharType );
    if( mit == m_mapUnLockGroupID.end() ) return true;
    if( mit->second.find( kSkill_.m_nLockGroupID ) == mit->second.end() ) return true;
    return false;
}

bool KUser::RemoveReserveHandleSkill( IN OUT std::set<int>& setReserveSkill_, int nSkillID_ )
{
    std::set<int>::iterator sit;
    sit = setReserveSkill_.find( nSkillID_ );
    if( sit == setReserveSkill_.end() ) return false;
    setReserveSkill_.erase( sit );
    return true;
}

bool KUser::AddReserveHandleSkill( IN OUT std::set<int>& setReserveSkill_, int nSkillID_ )
{
    return setReserveSkill_.insert( nSkillID_ ).second;
}

void KUser::CalcDisconnReasonOnLogin( unsigned char ucReson_ )
{
    switch( ucReson_ )
    {
    case 1: SetDisconnReason( DRW_WRONG_PASSWORD ); break;    //( 잘못된 패스워드 )                
    case 2: SetDisconnReason( DRW_DUPLICATE_CONNECTION ); break;    //( 이중 접속 )                      
    case 4: SetDisconnReason( DRW_PROTOCOL_VERION_DIFF ); break;    //( 프로토콜 버전 다름 )             
    case 5: SetDisconnReason( DRW_BAD_USER ); break;    //( 불량 유저 )                      
    case 7: SetDisconnReason( DRW_EMPTY_LOGIN ); break;    //( login 크기가 0이거나 인증 실패 )
    case 8: SetDisconnReason( DRW_ADD_NEW_USER_FAIL ); break;    //( 신규 유저 추가 실패 )
        //case 9: SetDisconnReason(  ); break;    //( esports 예선 신청자가 아님. )    
    case 10: SetDisconnReason( DRW_CHECKSUM_FAIL ); break;   //( 실행파일 checksum이 무효함 )    
    case 11: SetDisconnReason( DRW_GASH_AUTH_FAIL ); break;   //( 알 수 없는 오류. 대만 인증 실패)
    case 12: SetDisconnReason( DRW_NOT_USER ); break;   //( 존재하지 않는 login )           
    case 13: SetDisconnReason( DRW_AUTH_TICK_FAIL ); break;   //( 인증 tick Count가 다르다. )
    };
}

void KUser::AddStatLeaveGame( KGameResultIn& kResultIn )
{
    KStatLeaveGamePtr spStat = GET_STAT( KStatLeaveGame, SI_STAT_LEAVE_GAME );
    const KCharacterInfo* pkChar = GetCurrentCharacter();
    if( !m_pkRoom ) return;
    if( !pkChar ) return;
    if( !spStat ) return;
    char cLeftLife = 0;

    switch( m_pkRoom->m_iGameMode )
    {
    case GC_GM_DEATH_TEAM:
    case GC_GM_DEATH_SURVIVAL:
    case GC_GM_FATAL_DEATH_TEAM:
    case GC_GM_FATAL_DEATH_SURVIVAL:
        cLeftLife = 0;
        break;
    case GC_GM_TAG_TEAM:
    case GC_GM_TAG_SURVIVAL:
        {
            cLeftLife = 2 - kResultIn.m_iNumDies;
        }
    default:
        if( (m_pkRoom->m_iGameMode == GC_GM_TEAM_MODE ||
            m_pkRoom->m_iGameMode == GC_GM_SURVIVAL_MODE ) &&
            m_pkRoom->m_iMapID == GC_GS_VALLEY_OF_OATH  )
        {
            cLeftLife = 5 - kResultIn.m_iNumDies;
        }
        else {
            cLeftLife = 3 - kResultIn.m_iNumDies;
        }
    }

    cLeftLife = std::max<char>( 0, cLeftLife );
    int nPlayTime = (int)(::timeGetTime() - m_pkRoom->m_dwStartGameTime)/(1000 * 60);
    spStat->AddStat( m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, nPlayTime, cLeftLife, pkChar->m_cCharType, (unsigned char)pkChar->m_dwLevel );

    // 캐릭터 레벨 플레이타임 통계
    UpdateCharLvPlayTime( static_cast<int>(pkChar->m_cCharType), pkChar->m_dwLevel );
}

void KUser::UpdateGachaKeyBuyCount( int nCashDiff_, int nGPDiff_ )
{
    if( !SiKGSSimLayer()->m_bEnableLimitKeyBuy ) return;
    PAIR_INT kPacket;
    kPacket.first = nCashDiff_;
    kPacket.second = nGPDiff_;
    QUEUEING_EVENT_TO_DB( EVENT_UPDATE_GACHA_KEY_BUY_COUNT_REQ, kPacket );
}

void KUser::UpdatePeriodNotice()
{
    // tick 체크
    if( ::GetTickCount() - GetTick( PERIOD_NOTICE_TICK ) > 1000 * 60 )  // 1분 단위
    {
        KSerBuffer kNoticeBuff;
        if( SiKPeriodNotice()->GetNoticePacket( m_vecPeriodNoticeUID, kNoticeBuff ) == true )
        {
            START_LOG( clog, L"갱신된 DB공지 전달. size : " << m_vecPeriodNoticeUID.size() ) << END_LOG;
            LIF( SendPacket( kNoticeBuff, L"KUserEvent::EVENT_PERIOD_NOTICE_NOT" ) ); //클라이언트에서 받아서 처리하나?
        }

        SetTick( PERIOD_NOTICE_TICK );
    }
}

void KUser::SendVIPItemListNot()
{
    // 미국한정.
    if( SiKGSSimLayer()->m_bGachaEnable == false )
        return;

    KEVENT_VIP_ITEM_LIST_NOT kPacket;
    SiKGCHelper()->GetVipItemInfo( kPacket );
    START_LOG( clog, L"VIP Item 정보 size : " << kPacket.size() ) << END_LOG;

    SEND_PACKET( EVENT_VIP_ITEM_LIST_NOT );
}

void KUser::StartSubmission( IN const std::vector<DWORD>& vecUserUID_ )
{
    JIF( m_pkRoom );

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = vecUserUID_.begin() ; cvit != vecUserUID_.end() ; ++cvit ) {
        KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByUID<KUser>( *cvit ) );
        if( !spUser ) continue;

        SiKMissionManager()->ModeSubmission( *spUser, m_pkRoom->m_iGameMode, m_pkRoom->m_nDifficulty, m_pkRoom->m_iMapID );
    }
}

void KUser::ResetEquipSkill( IN const char& cCharType_ )
{
    std::map<std::pair<char,char>, std::vector<KSPSetInfo> >::iterator mit;
    for ( mit = m_mapSkillSets.begin() ; mit != m_mapSkillSets.end() ; ++mit ) {
        if ( mit->first.first != cCharType_ ) continue;

        std::vector<KSPSetInfo>::iterator vit;
        for ( vit = mit->second.begin() ; vit != mit->second.end() ; ++vit ) {
            vit->m_vecSkills.clear();
        }
    }
}

bool KUser::ResetTrainedSkill( IN const char& cCharType_ )
{
    bool bReset = false;

    std::map<std::pair<char,char>, KSPInfo >::iterator mit;
    for ( mit = m_mapTrainedSkill.begin() ; mit != m_mapTrainedSkill.end() ; ++mit ) {
        if ( mit->first.first != cCharType_ ) continue;

        std::vector<int>::iterator vit;
        for ( vit = mit->second.m_vecSkills.begin() ; vit != mit->second.m_vecSkills.end() ; ) {
            if( !SiKSkillManager()->CheckExceptionSkill( *vit ) ) {
                if( !RemoveReserveHandleSkill( m_setAddReserveSkill, *vit ) ) {
                    LIF( AddReserveHandleSkill( m_setDeleteReserveSkill, *vit ) );
                }
                vit = mit->second.m_vecSkills.erase( vit );
            } else {
                ++vit;
            }
        }

        bReset = true;
    }

    if ( !bReset ) {
        START_LOG( cwarn, L"스킬 삭제시 해당하는 캐릭이 없음. CharType : " << cCharType_ ) << END_LOG;
        return false;
    }

    return true;
}

void KUser::LoadVitalityInfo()
{
    if ( false == SiKVitalityManager()->IsRun() ) {
        return;
    }

    KDB_EVENT_VITALITY_INFO_REQ kPacket;
    kPacket.m_bNeedForceInit        = false;
    kPacket.m_mapCharacterInfo      = m_mapCharacterInfo;
    kPacket.m_bVitalityInfoLoaded   = false;

    QUEUEING_EVENT_TO_DB( DB_EVENT_VITALITY_INFO_REQ, kPacket );
}

void KUser::SendVitalityInfo( IN const int nVitalitySendType_ )
{
    KEVENT_VITALITY_INFO_NOT kPacket;

    // 클라이언트에서 버프 표시를 위해 활력이 차감된 시점과 충전된 시점을 구분하여 알 필요가 있어서 추가함
    switch( nVitalitySendType_ ) {
        case KVitalityManager::EVST_DUNGEON_START:
            kPacket.m_nOK = NetError::ERR_VITALITY_04;
            break;

        default:
            kPacket.m_nOK = 0;
            break;
    }

    kPacket.m_nMaxVitalityPerDay                = SiKVitalityManager()->GetMaxVitalityPerDay();
    kPacket.m_nMaxVitalityForFirstCharacter     = SiKVitalityManager()->GetVitalityForFirstCharacterCreation();
    kPacket.m_nVitalityPerRechargePoint         = SiKVitalityManager()->GetVitalityPerRechargePoint();
    kPacket.m_nRechargePoint                    = m_kVitalityUserData.m_nRechargePoint;
    kPacket.m_nRechargePointCounter             = m_kVitalityUserData.m_nRechargePointCounter;
    kPacket.m_mapRemainVitality                 = m_kVitalityUserData.m_mapRemainVitality;

    SendPacket( KUserEvent::EVENT_VITALITY_INFO_NOT, kPacket );
}

void KUser::SendVitalityCheckDungeonInfoNot()
{
    if ( false == SiKVitalityManager()->IsRun() ) {
        return;
    }

    KEVENT_VITALITY_CHECK_DUNGEON_INFO_NOT kPacket;

    SiKVitalityManager()->GetVitalityCheckDungeonInfoNot( kPacket );

    SendPacket( KUserEvent::EVENT_VITALITY_CHECK_DUNGEON_INFO_NOT, kPacket );
}

void KUser::CheckGiftFirstCharacter( IN const int nCharType_, OUT std::vector< KDropItemInfo >& vecGiftFirstCharacter_ )
{
    // 첫 캐릭터 접속 선물상자 시스템을 사용하는지 확인한다
    if ( false == SiKConnectionGiftBox()->IsRunGiftFirstCharacter() ) {
        return;
    }

    // 첫 캐릭터를 생성하는 경우에만 해당 보상을 지급한다
    if ( false == m_mapCharacterInfo.empty() ) {
        return;
    }

    SiKConnectionGiftBox()->GetGiftFirstCharacter( nCharType_, vecGiftFirstCharacter_ );
}

void KUser::GetConnectionGiftBoxUpdateData( OUT KConnectionGiftBoxUpdateData& kConnectionGiftBoxUpdateData_ )
{
    int nCharType = static_cast< int >( m_cCurrentCharacter );

    // 게임서버 입장시 신규유저 대상인지 판단하여 m_kConnectionGiftBoxUserData.m_bIsNewUser 에 저장한 상태이다
    kConnectionGiftBoxUpdateData_.m_bIsNewUser              = m_kConnectionGiftBoxUserData.m_bIsNewUser;
    kConnectionGiftBoxUpdateData_.m_nNewUserLastGetGrade    = m_kConnectionGiftBoxUserData.GetNewUserLastGetGrade( nCharType );
    kConnectionGiftBoxUpdateData_.m_nNewUserAccPlayTime     = m_kConnectionGiftBoxUserData.GetNewUserAccPlayTime( nCharType );

    kConnectionGiftBoxUpdateData_.m_bIsEventUser            = SiKConnectionGiftBox()->IsGiftEventUserTerm();
    kConnectionGiftBoxUpdateData_.m_nEventUserVersion       = SiKConnectionGiftBox()->GetGiftEventUserVersion();
    kConnectionGiftBoxUpdateData_.m_nEventUserLastGetGrade  = m_kConnectionGiftBoxUserData.GetEventUserLastGetGrade( nCharType );
    kConnectionGiftBoxUpdateData_.m_nEventUserAccPlayTime   = m_kConnectionGiftBoxUserData.GetEventUserAccPlayTime( nCharType );
}

void KUser::CheckConnectionGiftBoxAccTime()
{
    int nCharType = static_cast< int >( m_cCurrentCharacter );

    if ( true == m_kConnectionGiftBoxUserData.IsNewUser() ) {
        int nNewUserCurrentGrade = m_kConnectionGiftBoxUserData.GetNewUserCurrentGrade( nCharType );
        KGiftGradeInfo kNewUserGiftGradeInfo;

        if ( true == SiKConnectionGiftBox()->GetNewUserGiftGradeInfo( nNewUserCurrentGrade, kNewUserGiftGradeInfo ) ) {
            m_kConnectionGiftBoxUserData.IncreaseNewUserAccPlayTime( nCharType );
            int nNewUserAccPlayTime = std::min< int >( m_kConnectionGiftBoxUserData.GetNewUserAccPlayTime( nCharType ), kNewUserGiftGradeInfo.m_nCountTime );
            m_kConnectionGiftBoxUserData.SetNewUserAccPlayTime( nCharType, nNewUserAccPlayTime );
            if ( m_kConnectionGiftBoxUserData.GetNewUserAccPlayTime( nCharType ) >= kNewUserGiftGradeInfo.m_nCountTime ) {
                // 신규유저 선물을 받을 수 있다고 알림
                SEND_ID( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_ALARM_NOT );
            }
        }
    }

    if ( true == SiKConnectionGiftBox()->IsGiftEventUserTerm() ) {
        int nEventUserCurrentGrade = m_kConnectionGiftBoxUserData.GetEventUserCurrentGrade( nCharType );
        KGiftGradeInfo kEventUserGiftGradeInfo;

        if ( true == SiKConnectionGiftBox()->GetEventUserGiftGradeInfo( nEventUserCurrentGrade, kEventUserGiftGradeInfo ) ) {
            m_kConnectionGiftBoxUserData.IncreaseEventUserAccPlayTime( nCharType );
            int nEventUserAccPlayTime = std::min< int >( m_kConnectionGiftBoxUserData.GetEventUserAccPlayTime( nCharType ), kEventUserGiftGradeInfo.m_nCountTime );
            m_kConnectionGiftBoxUserData.SetEventUserAccPlayTime( nCharType, nEventUserAccPlayTime );
            if ( m_kConnectionGiftBoxUserData.GetEventUserAccPlayTime( nCharType ) >= kEventUserGiftGradeInfo.m_nCountTime ) {
                // 이벤트유저 선물을 받을 수 있다고 알림
                SEND_ID( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_ALARM_NOT );
            }
        }
    }
}

void KUser::SendConnectionGiftBoxInfoNot( IN const int nSendType_ )
{
    int nCharType = static_cast< int >( m_cCurrentCharacter );

    if ( true == m_kConnectionGiftBoxUserData.IsNewUser() ) {
        KEVENT_CONNECTION_GIFTBOX_NEWUSER_INFO_NOT kPacket;
        kPacket.m_nOK = nSendType_;
        kPacket.m_vecGetRewards.clear();

        kPacket.m_nAccPlayTime = m_kConnectionGiftBoxUserData.GetNewUserAccPlayTime( nCharType );

        int nNewUserCurrentGrade = m_kConnectionGiftBoxUserData.GetNewUserCurrentGrade( nCharType );
        SiKConnectionGiftBox()->GetNewUserNextGift( nCharType, nNewUserCurrentGrade, kPacket.m_vecNextGift );
        if ( false == kPacket.m_vecNextGift.empty() ) { // 다음 보상 정보가 있는 경우에만 패킷을 전송한다(접속시, 캐릭터 변경시)
            SEND_PACKET( EVENT_CONNECTION_GIFTBOX_NEWUSER_INFO_NOT );
        }

        KGiftGradeInfo kNewUserGiftGradeInfo;
        if ( true == SiKConnectionGiftBox()->GetNewUserGiftGradeInfo( nNewUserCurrentGrade, kNewUserGiftGradeInfo ) ) {
            if ( m_kConnectionGiftBoxUserData.GetNewUserAccPlayTime( nCharType ) >= kNewUserGiftGradeInfo.m_nCountTime ) {
                // 신규유저 선물을 받을 수 있다고 알림
                SEND_ID( EVENT_CONNECTION_GIFTBOX_NEWUSER_GET_ALARM_NOT );
            }
        }
    }

    if ( true == SiKConnectionGiftBox()->IsGiftEventUserTerm() ) {
        KEVENT_CONNECTION_GIFTBOX_EVENTUSER_INFO_NOT kPacket;
        kPacket.m_nOK = nSendType_;
        kPacket.m_vecGetRewards.clear();

        kPacket.m_nAccPlayTime = m_kConnectionGiftBoxUserData.GetEventUserAccPlayTime( nCharType );

        int nEventUserCurrentGrade = m_kConnectionGiftBoxUserData.GetEventUserCurrentGrade( nCharType );
        SiKConnectionGiftBox()->GetEventUserNextGift( nCharType, nEventUserCurrentGrade, kPacket.m_vecNextGift );
        if ( false == kPacket.m_vecNextGift.empty() ) { // 다음 보상 정보가 있는 경우에만 패킷을 전송한다(접속시, 캐릭터 변경시)
            SEND_PACKET( EVENT_CONNECTION_GIFTBOX_EVENTUSER_INFO_NOT );
        }

        KGiftGradeInfo kEventUserGiftGradeInfo;
        if ( true == SiKConnectionGiftBox()->GetEventUserGiftGradeInfo( nEventUserCurrentGrade, kEventUserGiftGradeInfo ) ) {
            if ( m_kConnectionGiftBoxUserData.GetEventUserAccPlayTime( nCharType ) >= kEventUserGiftGradeInfo.m_nCountTime ) {
                // 이벤트유저 선물을 받을 수 있다고 알림
                SEND_ID( EVENT_CONNECTION_GIFTBOX_EVENTUSER_GET_ALARM_NOT );
            }
        }
    }
}

bool KUser::CheckNormalMissionRegistEnable( IN const int& nGameMode_ )
{
    std::vector< KMissionInfo > vecMissionInfo;
    std::vector< KMissionInfo >::iterator vitMissionInfo;
    std::vector< DWORD >::iterator vitEpicMission;
    DWORD dwMissionID = 0;

    // 던전에 종속 된 미션 리스트를 얻는다
    if ( false == SiKMissionManager()->GetMissionInfoListByDungeonID( vecMissionInfo, nGameMode_ ) ) {
        return false;
    }

    for( vitMissionInfo = vecMissionInfo.begin(); vitMissionInfo != vecMissionInfo.end(); ++vitMissionInfo ) {
        // 일반미션 등록 가능한지 확인 하는 함수이므로 에픽미션 클리어 했는지 확인 함
        if ( KMissionInfo::MISSION_TYPE_EPIC == vitMissionInfo->m_cMissionType ) {
            dwMissionID = vitMissionInfo->m_dwMissionID;

            vitEpicMission = std::find( m_vecFinishedMission.begin(), m_vecFinishedMission.end(), dwMissionID );

            // 해당 던전의 에픽 미션 클리어 기록이 있으면 노멀 미션 등록 가능
            if ( m_vecFinishedMission.end() != vitEpicMission ) {
                return true;
            }
        }
    }

    // 에픽 미션 클리어한 기록이 없으므로 일반 미션 등록 불가능
    return false;
}

bool KUser::IsAlreadyFinishedMission( IN const DWORD& dwMissionID_ )
{
    std::vector< DWORD >::iterator vitFinishedMission;

    vitFinishedMission = std::find( m_vecFinishedMission.begin(), m_vecFinishedMission.end(), dwMissionID_ );

    if ( m_vecFinishedMission.end() != vitFinishedMission ) {
        START_LOG( cwarn, L"던전 룸에서 등록 요청 한 미션을 클리어한적이 있다" ) << BUILD_LOG( dwMissionID_ ) << END_LOG;
        return true;
    }

    return false;
}

bool KUser::UpdateDungeonPersonalRecordData( IN const int nModeID_, IN const int nCharType_, IN const std::map< int, KDungeonPersonalRecord >& mapDungeonPersonalRecord_ )
{
    // 기록이 새로 추가되거나 갱신될때는 diff쪽에도 동시에 담는다
    std::map< int, KDungeonPersonalRecord >& rmapBeforeRecord = m_mapDungeonPersonalRecord[ nCharType_ ][ nModeID_ ];
    bool bUpdated = false;

    std::map< int, KDungeonPersonalRecord >::const_iterator cmitNewRecord = mapDungeonPersonalRecord_.begin();
    for ( ; mapDungeonPersonalRecord_.end() != cmitNewRecord; ++cmitNewRecord ) {
        std::map< int, KDungeonPersonalRecord >::iterator mitBeforeRecord = rmapBeforeRecord.find( cmitNewRecord->first );
        if ( rmapBeforeRecord.end() == mitBeforeRecord ) { // 이전의 스테이지 클리어 기록이 없으면 추가
            rmapBeforeRecord.insert( std::make_pair( cmitNewRecord->first, cmitNewRecord->second ) );
            m_mapDungeonPersonalRecordDiff[ nCharType_ ][ nModeID_ ].insert( std::make_pair( cmitNewRecord->first, cmitNewRecord->second ) );
            bUpdated = true;
        }
        else {
            /* 이전의 스테이지 클리어 기록이 있다면 새 기록의 각 항목별로 비교하여 필요할 경우 갱신
               갱신 조건1: m_dwStageClearTime이 낮으면
               갱신 조건2: m_nControlRank이 높으면 */
            if ( mitBeforeRecord->second.m_dwStageClearTime > cmitNewRecord->second.m_dwStageClearTime ) {
                mitBeforeRecord->second.m_dwStageClearTime = cmitNewRecord->second.m_dwStageClearTime;
                m_mapDungeonPersonalRecordDiff[ nCharType_ ][ nModeID_ ][ cmitNewRecord->first ] = cmitNewRecord->second;
                bUpdated = true;
            }
            if ( mitBeforeRecord->second.m_nControlRank < cmitNewRecord->second.m_nControlRank ) {
                mitBeforeRecord->second.m_nControlRank = cmitNewRecord->second.m_nControlRank;
                m_mapDungeonPersonalRecordDiff[ nCharType_ ][ nModeID_ ][ cmitNewRecord->first ] = cmitNewRecord->second;
                bUpdated = true;
            }
        }
    }

    // 변경된 기록이 있을 경우 해당 캐릭터의 전체 데이터를 클라이언트에 전달한다
    if ( true == bUpdated ) {
        KEVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT kPacket;
        kPacket.m_nOK = NetError::NET_OK;
        kPacket.m_cCharType = static_cast< char >( nCharType_ );
        kPacket.m_mapDungeonPersonalRecord = m_mapDungeonPersonalRecord[ nCharType_ ];

        SEND_PACKET( EVENT_DUNGEON_PERSONAL_RECORD_INFO_NOT );
    }

    return true;
}
