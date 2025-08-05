#include "MatchUser.h"
#include "MatchGroup.h"
#include <dbg/dbg.hpp >
#include "Log4.h"
#include "NetError.h"
#include <boost/bind.hpp>
NiImplementRTTI( KMatchGroup, KPerformer );

#define EVENT_TYPE KMatchEvent
#define CLASS_TYPE KMatchGroup

ImplToStringW( KMatchGroup )
{
    return START_TOSTRING_PARENTW( KPerformer );
}

KMatchGroup::KMatchGroup(void)
:m_dwServerUID( -1 )
,m_dwTotalListCount(0)
,m_dwTickGap(3000)
{
    for( int i = 0; i < MAX_PARTY_MEMBER_COUNT; ++i ) {
        m_listMatchUserInfo[i].clear();
        m_mapMatchPartyData[i].clear();
    }

    memset( m_bFindMatchParty, 0, sizeof(m_bFindMatchParty) );
    memset( m_nSearchSize, 0, sizeof(m_nSearchSize) );
    m_dwLastTick = ::GetTickCount();
}

KMatchGroup::~KMatchGroup(void)
{
}

void KMatchGroup::Tick()
{
    KPerformer::Tick();
    IntervalTick();
}

void KMatchGroup::IntervalTick()
{
    // 5초 단위 처리
    if( !CheckUpdateTick() ) {
        return;
    }

    if( m_bLimitMatchEnable && GetListCount() <= m_nLimitMatchGroupNum ) {
        return;
    }

    std::vector<KPartyData> vecPartyData;
    std::vector<KPartyData>::iterator vit;
    KEVENT_MATCH_FIND_MATCH_NOT kPacket;
    if( UpdateMatch( vecPartyData, kPacket.m_nLoopCount ) ) {
        // 게임서버에 전달한 패킷 세팅 부분 작업.
        vit = vecPartyData.begin();
        kPacket.m_nModeInfo = m_nModeInfo;
        kPacket.m_vecPartyData = vecPartyData;
        kPacket.m_nWaitListCount = GetListCount();
        SendToGameServer( vit->m_dwServerUID, KMatchEvent::EVENT_MATCH_FIND_MATCH_NOT, kPacket );
    }
}

bool KMatchGroup::CheckUpdateTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }
    m_dwLastTick = ::GetTickCount();
    return true;
}

void KMatchGroup::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );
    switch( spEvent_->m_usEventID ) {
         CASE( EVENT_MATCH_ADD_USER_REQ );
        _CASE( EVENT_MATCH_DEL_USER_REQ, KPartyData );
        _CASE( EVENT_MATCH_DISCONNECT_GAMESERVER_NOT, DWORD );
        default:
            START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

KMatchPartyData* KMatchGroup::GetMatchPartyData( IN const KPartyData& kPartyData_ )
{
    MAP_RPPARTYINFO::iterator mitRP = m_mapMatchPartyData[kPartyData_.m_vecPartyPeople.size() - 1].find( kPartyData_.m_nRP );
    if( mitRP != m_mapMatchPartyData[kPartyData_.m_vecPartyPeople.size() - 1].end() ) {
        MAP_PARTYINFO::iterator mitParty = mitRP->second.find( kPartyData_.m_dwMatchPartyUID );
        if( mitParty != mitRP->second.end() ) {
            return &(mitParty->second);
        }
    }

    return NULL;
}

bool KMatchGroup::AddMatch( IN const KEVENT_MATCH_ADD_USER_REQ& kPartyData_ )
{
    KPartyDataPtr spPartyData = KPartyDataPtr( new KPartyData );
    spPartyData.reset( new KPartyData );
    *spPartyData = kPartyData_.m_kPartyData;

    MAP_RPPARTYINFO::iterator mit;
    KMatchPartyData kMatchPartyData;
    kMatchPartyData.m_PartyData = kPartyData_.m_kPartyData;
    kMatchPartyData.m_SearchScope = kPartyData_.m_nStartScope;
    kMatchPartyData.m_nLoopCount = 0;

    KLocker lock( m_csMatchUserInfo );
    if( GetMatchPartyData( kPartyData_.m_kPartyData ) ) {
        DelMatch( kPartyData_.m_kPartyData ); // 매칭요청시 현재 데이터가 있으면 매칭서버 데이터를 날린다.
        SET_ERROR( ERR_MATCH_ERROR_01 );
        return false;
    }

    m_listMatchUserInfo[kPartyData_.m_kPartyData.m_vecPartyPeople.size() - 1].push_back( spPartyData );

    mit = m_mapMatchPartyData[kPartyData_.m_kPartyData.m_vecPartyPeople.size() - 1].find( spPartyData->m_nRP );
    if( mit == m_mapMatchPartyData[kPartyData_.m_kPartyData.m_vecPartyPeople.size() - 1].end() ) {
        MAP_PARTYINFO mapPartyInfo;
        mapPartyInfo[spPartyData->m_dwMatchPartyUID] = kMatchPartyData;
        m_mapMatchPartyData[kPartyData_.m_kPartyData.m_vecPartyPeople.size() - 1][spPartyData->m_nRP] = mapPartyInfo;
    } else {
        MAP_PARTYINFO& mapPartyInfo = m_mapMatchPartyData[kPartyData_.m_kPartyData.m_vecPartyPeople.size() - 1][spPartyData->m_nRP];
        mapPartyInfo[spPartyData->m_dwMatchPartyUID] = kMatchPartyData;
    }

    IncreaseListCount(); // 가진 GroupList Count 증가
    return true;
}

void KMatchGroup::DelMatch( IN const KPartyData& kPartyData_ )
{
    KLocker lock( m_csMatchUserInfo );

    m_listMatchUserInfo[kPartyData_.m_vecPartyPeople.size() - 1].remove_if( boost::bind( &KPartyData::m_dwMatchPartyUID, _1 ) == kPartyData_.m_dwMatchPartyUID );

    MAP_RPPARTYINFO::iterator mit;
    mit = m_mapMatchPartyData[kPartyData_.m_vecPartyPeople.size() - 1].find( kPartyData_.m_nRP );

    MAP_PARTYINFO::iterator mitInner;
    if ( mit != m_mapMatchPartyData[kPartyData_.m_vecPartyPeople.size() - 1].end() ) {
        mitInner = mit->second.find( kPartyData_.m_dwMatchPartyUID );
        
        if ( mitInner != mit->second.end() ) {
            mit->second.erase( mitInner );
        }

        if ( mit->second.empty() ) { // m_mapMatchPartyData 의 RP의 데이터 없을 때, 삭제하는게 맞겠지?
            m_mapMatchPartyData[kPartyData_.m_vecPartyPeople.size() - 1].erase( mit );
        }
    }
    DecreaseListCount(); // 가진 GroupList Count 증가
}

bool KMatchGroup::UpdateMatch( OUT std::vector<KPartyData>& vecPartyData_, OUT int& nLoopCount_ )
{
    // Init.
    vecPartyData_.clear();
    nLoopCount_ = 0;

    KLocker lock( m_csMatchUserInfo );

    // 제일 먼저 신청이 들어온 10% 의 파티에 대해서만 검색을 실시한다
    for( int i = 0; i < MAX_PARTY_MEMBER_COUNT; ++i ) {
        std::list<KPartyDataPtr>& listPartyData = m_listMatchUserInfo[ i ];
        int iMaxLoop = listPartyData.size() / 10 + 1;

        for( int iCount = 0; iCount < iMaxLoop; ++iCount ) {
            if ( !listPartyData.empty() ) {
                if ( FindMatch( *listPartyData.front(), vecPartyData_, nLoopCount_ ) ) {
                    return true;
                }
            } 
            else {
                continue;
            }
        }
    }

    return false;
}

void KMatchGroup::UpdateSearchScope( IN OUT KMatchPartyData& kMatchPartyData_ )
{
    // 증가하는 Rating 수치.
    kMatchPartyData_.m_SearchScope += m_nIncreaseScope;
    // 최대값 보정 처리. 정의해 놓은 Max값 이상 검색 범위가 올라가지 않는다.
    kMatchPartyData_.m_SearchScope = std::min<int>( m_nMaxIncreaseScope, kMatchPartyData_.m_SearchScope );
    if( kMatchPartyData_.m_SearchScope == m_nMaxIncreaseScope ) { // 최대값일 경우 다시 최소값으로 변동.
        kMatchPartyData_.m_SearchScope = m_nStartSearchScope;
    }
    ++kMatchPartyData_.m_nLoopCount; // 한번 Loop돌때마다 1씩 증가.
}

bool KMatchGroup::FindMatch( IN const KPartyData& kPartyData_, OUT std::vector<KPartyData>& vecPartyData_, OUT int& nLoopCount_ )
{
    bool bFind = false;
    vecPartyData_.clear();

    KMatchPartyData* pkData = GetMatchPartyData( kPartyData_ );
    if( !pkData ) {
        START_LOG( cerr, L"매칭할려는 파티 데이터가 없음." )
            << END_LOG;
        return false;
    }

    // Reset
    for( int i = 0; i < MAX_PARTY_MEMBER_COUNT; ++i ) {
        m_bFindMatchParty[ i ] = false;
    }

    const int nPartyMemeberCnt = pkData->m_PartyData.m_vecPartyPeople.size(); // 현재 파티인원수.
    const int nNeedMemeberCnt = GetNeedMemberCount( pkData->m_PartyData.m_nModeInfo, nPartyMemeberCnt ); // 필요인원수.

    int nTempNeedMemeberCnt = nNeedMemeberCnt;
    for ( int nMember = nTempNeedMemeberCnt ; nMember >= 1 ; --nMember ) {
        int nNeedCnt = nTempNeedMemeberCnt / nMember;
        if ( nNeedCnt <= 0 || nMember > 3 ) continue;
        if ( nNeedCnt == 2 && nMember == 2 ) { // 2:2:2 상황이 오기 때문에
            nNeedCnt = 1;
        }

        if ( FindMatch( *pkData, nMember, nNeedCnt, vecPartyData_ ) == false ) {
            continue;
        }

        nTempNeedMemeberCnt -= (nNeedCnt * nMember);
        if( nTempNeedMemeberCnt == 0 ) {
            bFind = true;
        }
    }

    if( bFind ) {
        nLoopCount_ = pkData->m_nLoopCount; // LoopCount 넣기.
        vecPartyData_.push_back( kPartyData_ );

        // 성공한 것들은 모두 목록에서 지워버리기
        for( int i = 0; i < (int)vecPartyData_.size(); ++i ) {
            DelMatch( vecPartyData_[i] );
        }
    }
    else {
        UpdateSearchScope( *pkData ); // 검색 범위값 늘리기.
    }

    return bFind;
}

bool KMatchGroup::FindMatch( IN const KMatchPartyData& kMatchPartyData_, IN int nPartyMemeberCnt_, IN int nNeedMemeberCnt_, OUT std::vector<KPartyData>& vecPartyData_ )
{
    if( !m_bFindMatchParty[ nPartyMemeberCnt_ - 1 ] ) {
        int nLower = kMatchPartyData_.m_PartyData.m_nRP - kMatchPartyData_.m_SearchScope;
        int nUpper = kMatchPartyData_.m_PartyData.m_nRP + kMatchPartyData_.m_SearchScope;

        // 정렬 상태를 깨지 않고 원소 삽입 위치 리턴.
        m_mitLower[nPartyMemeberCnt_ - 1] = m_mapMatchPartyData[nPartyMemeberCnt_ - 1].lower_bound( nLower );
        m_mitUpper[nPartyMemeberCnt_ - 1] = m_mapMatchPartyData[nPartyMemeberCnt_ - 1].upper_bound( nUpper );

        // first, second 사이의 거리를 리턴.
        m_nSearchSize[nPartyMemeberCnt_ - 1] = std::distance( m_mitLower[nPartyMemeberCnt_ - 1], m_mitUpper[nPartyMemeberCnt_ - 1] );
    }

    if( m_nSearchSize[nPartyMemeberCnt_ - 1] < 1 ) { // 거리가 없을 경우 return 처리.
        return false;
    }


    int nCount = MAX_SAME_GET_COUNT;
    while( --nCount > 0 ) { // 자기 자신이나 이미 뽑았던 것을 뽑는 case가 10회 이상일 경우 대기풀 부족으로 실패 처리.
        MAP_RPPARTYINFO::iterator mitRP = m_mitLower[nPartyMemeberCnt_ - 1];
        std::advance( mitRP, rand() % m_nSearchSize[nPartyMemeberCnt_ - 1] ); // mitRP를 거리의 랜덤값만큼 반복자 이동.

        MAP_PARTYINFO& mapPartyInfo = mitRP->second;
        MAP_PARTYINFO::iterator mitParty = mapPartyInfo.begin();
        std::advance( mitParty, rand() % mapPartyInfo.size() ); // mitRP가 가진 데이터중 현재 사이즈의 랜덤값만큼 반복자 이동.

        KMatchPartyData kDestMatchPartyData = mitParty->second;

        DWORD dwMatchPartyUID = mitParty->first;
        std::vector<KPartyData>::iterator vitParty;
        vitParty = std::find_if( vecPartyData_.begin(), vecPartyData_.end(), boost::bind( &KPartyData::m_dwMatchPartyUID, _1 ) == dwMatchPartyUID );

        if( dwMatchPartyUID != kMatchPartyData_.m_PartyData.m_dwMatchPartyUID && vecPartyData_.end() == vitParty ) {
            if( ( kMatchPartyData_.m_PartyData.m_nRP - kMatchPartyData_.m_SearchScope ) <= mitParty->second.m_PartyData.m_nRP && ( kMatchPartyData_.m_PartyData.m_nRP + kMatchPartyData_.m_SearchScope ) >= mitParty->second.m_PartyData.m_nRP ) {
                vecPartyData_.push_back( mitParty->second.m_PartyData );
                if( --nNeedMemeberCnt_ <= 0 ) {
                    return true;
                }
            }
        }
    }
    return false;
}

int KMatchGroup::GetNeedMemberCount( IN const int& nModeInfo_, IN const int& nCurrentMemberCnt_ )
{
    int nNeedMemberCnt = 0;
    switch( nModeInfo_ )
    {
    case KPartyData::MI_PVP_1VS1 :  nNeedMemberCnt = 2; break;
    case KPartyData::MI_PVP_2VS2 :  nNeedMemberCnt = 4; break;
    case KPartyData::MI_PVP_3VS3 :  nNeedMemberCnt = 6; break;
    default :
        nNeedMemberCnt = 2;
    }

    return nNeedMemberCnt - nCurrentMemberCnt_;
}

void KMatchGroup::SetServerInfo( IN const DWORD& dwServerUID_, IN const int& nModeInfo_ )
{
    KLocker lock( m_csServer );
    m_dwServerUID = dwServerUID_;
    m_nModeInfo = nModeInfo_;
}

void KMatchGroup::SetMatchCheckInfo( IN const int& nStartSearchScope_, IN const int& nMaxIncreaseScope_, IN const int& nIncreaseScope_, IN const bool& bLimitMatchEnable_, IN const int& nLimitMatchGroupNum_ )
{
    KLocker lock( m_csServer );
    m_nStartSearchScope = nStartSearchScope_;
    m_nMaxIncreaseScope = nMaxIncreaseScope_;
    m_nIncreaseScope = nIncreaseScope_;
    m_bLimitMatchEnable = bLimitMatchEnable_;
    m_nLimitMatchGroupNum = nLimitMatchGroupNum_;
}

void KMatchGroup::IncreaseListCount()
{
    ++m_dwTotalListCount;
}

void KMatchGroup::DecreaseListCount()
{
    if( m_dwTotalListCount > 0 ) {
        --m_dwTotalListCount;
    }
}

int KMatchGroup::GetListCount()
{
    KLocker lock( m_csServer );
    return static_cast<int>( m_dwTotalListCount );
}

void KMatchGroup::InitMatchData()
{
    KLocker lock( m_csServer );
    for( int i = 0; i < MAX_PARTY_MEMBER_COUNT; ++i ) {
        m_listMatchUserInfo[i].clear();
        m_mapMatchPartyData[i].clear();
    }
    memset( m_bFindMatchParty, 0, sizeof(m_bFindMatchParty) );
    memset( m_nSearchSize, 0, sizeof(m_nSearchSize) );
    m_dwTotalListCount = 0;
}

void KMatchGroup::GetLimitInfo( OUT int& nLimitMatchGroupNum_, OUT bool& bLimitMatchEnable_ )
{
    KLocker lock( m_csServer );
    nLimitMatchGroupNum_ = m_nLimitMatchGroupNum;
    bLimitMatchEnable_ = m_bLimitMatchEnable;
}

IMPL_ON_FUNC( EVENT_MATCH_ADD_USER_REQ )
{
    KEVENT_MATCH_ADD_USER_ACK kPacket;
    kPacket.m_nOK= -99;

    if( AddMatch( kPacket_ ) ) {
        kPacket.m_nOK = 0;
    }
    kPacket.m_kPartyData = kPacket_.m_kPartyData;
    kPacket.m_nWaitListCount = GetListCount();

    SendToGameServer( kPacket.m_kPartyData.m_dwServerUID, KMatchEvent::EVENT_MATCH_ADD_USER_ACK, kPacket );
}

_IMPL_ON_FUNC( EVENT_MATCH_DEL_USER_REQ, KPartyData )
{
    KEVENT_MATCH_DEL_USER_ACK kPacket;
    kPacket.m_nOK = 0;
    kPacket.m_kPartyData = kPacket_;

    DelMatch( kPacket_ );
    kPacket.m_nWaitListCount = GetListCount();
    SendToGameServer( kPacket.m_kPartyData.m_dwServerUID, KMatchEvent::EVENT_MATCH_DEL_USER_ACK, kPacket );
}

_IMPL_ON_FUNC( EVENT_MATCH_DISCONNECT_GAMESERVER_NOT, DWORD )
{
    InitMatchData();
}
