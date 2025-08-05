#include "CenterSimLayer.h"
#include "CnItemManager.h"
#include <dbg/dbg.hpp >
#include <algorithm>
#include "CenterEvent.h"
#include "CommonPacket.h"
#include "CenterDBThread.h"
#include "Log4.h"

//FILE_NAME_FOR_LOG

ImplementSingleton( KCnItemManager );
ImplOstmOperatorW2A( KCnItemManager );
NiImplementRootRTTI( KCnItemManager );
namespace
{
    struct KSort
    {
        bool operator() ( const std::pair<int,bool>& pr1, const std::pair<int,bool>& pr2 ) const
        {
            if( pr1.first < pr2.first ) return true;
            return false;
        }
    };
};

KCnItemManager::KCnItemManager( void )
: m_dwTickGap( 60000 )
, m_bBuyItemEnable( true )
, m_bBuyEnable( true )
, m_bCashbackEventStarted( false )
, m_bCashbackEventEnded( false )
, m_kCashbackEventTimer( 10 * 1000 )
, m_tmCashbackEventStart( LONG_MAX )
, m_tmCashbackEventEnd( LONG_MAX )
{
    m_dwLastTick = ::GetTickCount();
    m_tmLastUpdateTime = KncUtil::TimeToInt( CTime::GetCurrentTime() );
}

KCnItemManager::~KCnItemManager( void )
{
}

ImplToStringW( KCnItemManager )
{
    KLocker lock( m_csBuyItemInfo );
    KLocker lock1( m_csTickInfo );
    KLocker lock2( m_csBuyLimitItem );
    return START_TOSTRINGW
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGWt( CTime(m_tmLastUpdateTime) )
        << TOSTRINGWb( m_bBuyItemEnable )
        << TOSTRINGWb( m_bBuyEnable )
        << TOSTRINGW( m_vecBuyItemInfo.size() );
}

void KCnItemManager::UpdateBuyInfoFromDB( std::vector<std::pair<DWORD,bool> >& vecBuyInfo_ )
{
    if( vecBuyInfo_.empty() ) return;
    // 정렬
    std::sort( vecBuyInfo_.begin(), vecBuyInfo_.end(), KSort() );
    // 그룹화
    std::vector<std::pair<DWORD,bool> >::iterator vit;

    std::pair<DWORD,bool> prLast(0,false);
    bool bBuyEnable = false; // 아래 쪽에서 재활용 하겠음.
    std::pair<DWORD,DWORD> prItemRange(0,0);
    std::map<bool,std::vector<std::pair<DWORD,DWORD> > > mapBuyInfo;

    DWORD dwTick = ::GetTickCount();
    for( vit = vecBuyInfo_.begin() ; vit != vecBuyInfo_.end() ; ++vit )
    {
        // begin 이 비어 있으면.. 거기 넣고, buy값 셋팅 해서 다음 스텝으로 들어간다.
        if( prItemRange.first == 0 ) // 최초 한번만 실행하게 됨..
        {
            prItemRange.first   = vit->first;
            bBuyEnable          = vit->second;
        }
        else if( bBuyEnable != vit->second ) // 이전의 Buy Enable 값이 변경 되었다면.
        {
            prItemRange.second  = prLast.first;
            mapBuyInfo[bBuyEnable].push_back( prItemRange );

            prItemRange.first    = vit->first;
            prItemRange.second   = 0;
            bBuyEnable = vit->second;
        }
        prLast = *vit;
    }

    // 마지막 요소는??
    prItemRange.second = prLast.first;
    mapBuyInfo[bBuyEnable].push_back( prItemRange );

    // Swap 할 녀석 선택.
    bBuyEnable = false;
    if( mapBuyInfo[true].size() < mapBuyInfo[false].size() )
        bBuyEnable = true;

    // Log.
    START_LOG( cerr, L"Buy Info Update.. Total Size : " << vecBuyInfo_.size()
        << L", Enable : " << mapBuyInfo[true].size()
        << L", Disable : " << mapBuyInfo[false].size()
        << L", Decision : " << (bBuyEnable?L"Enable":L"Disable")
        << L", Time : " << GetTickCount() - dwTick )  << END_LOG;

    // Swap/Change Setting..
    KLocker lock( m_csBuyItemInfo );
    SetBuyItemInfo( bBuyEnable, mapBuyInfo[bBuyEnable] );
    m_tmLastUpdateTime = KncUtil::TimeToInt( CTime::GetCurrentTime() );
}

void KCnItemManager::SetBuyItemInfo( bool bBuyEnable_, std::vector< std::pair<DWORD,DWORD> >& vecBuyItemInfo_ )
{
    m_bBuyEnable = bBuyEnable_;
    m_vecBuyItemInfo.swap( vecBuyItemInfo_ );
}

bool KCnItemManager::GetBuyItemInfo( OUT bool& bBuyEnable_, OUT std::vector< std::pair<DWORD,DWORD> >& vecBuyItemInfo_ )
{
    KLocker lock(m_csBuyItemInfo);
    if( m_vecBuyItemInfo.empty() ) return false;
    bBuyEnable_     = m_bBuyEnable;
    vecBuyItemInfo_ = m_vecBuyItemInfo;
    return true;
}

void KCnItemManager::Tick()
{
    // 시간 되었으면 DB로 체크 QUEUING
    if ( IsCheckTime() ) {
        QueuingCheckDBUp();
    }

    if ( true == m_kCashbackEventTimer.CheckTime() ) {
        CheckCashbackEvent();
    }
}

bool KCnItemManager::IsCheckTime()
{
    KLocker lock(m_csBuyItemInfo);
    if( ::GetTickCount() - m_dwLastTick < m_dwTickGap )
        return false;
    m_dwLastTick = ::GetTickCount();
    return true;
}

void KCnItemManager::QueuingCheckDBUp()
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_UPDATE_CHECK_REQ, L"", 0 );
}

void KCnItemManager::SetBuyItemInfoEnable( bool bEnable_ )
{
    KLocker lock( m_csBuyItemInfo );
    m_bBuyItemEnable = bEnable_;
}

bool KCnItemManager::IsBuyItemInfoEnable()
{
    KLocker lock( m_csBuyItemInfo );
    return m_bBuyItemEnable;
}

void KCnItemManager::UpdateItemRecommInfo( IN std::map<DWORD,unsigned char >& mapRecommInfo_ )
{
    KLocker lock(m_csBuyItemInfo);
    m_mapRecommInfo.swap( mapRecommInfo_ );
}

void KCnItemManager::GetItemRecommInfo( OUT std::map<DWORD,unsigned char >& mapRecommInfo_ )
{
    KLocker lock(m_csBuyItemInfo);
    mapRecommInfo_ = m_mapRecommInfo;
}

bool KCnItemManager::LoadScript()
{
    _JIF( m_kSocketConsume.LoadScript(), return false );
    _JIF( m_kSocketConsume.LoadScript_GP(), return false );
    _JIF( m_kSocketConsume.LoadScript_Item(), return false );

    return true;
}

void KCnItemManager::UpdateBuyLimitItem( IN const std::set<GCITEMID>& setList_ )
{
    START_LOG( clog, L"BuyLimit Item List Update.. size : " << setList_.size() ) << END_LOG;

    KLocker lock( m_csBuyLimitItem );
    m_setBuyLimitItem = setList_;
}

void KCnItemManager::GetBuyLimitItem( OUT std::set<GCITEMID>& setList_ )
{
    KLocker lock( m_csBuyLimitItem );
    setList_ = m_setBuyLimitItem;
}

void KCnItemManager::Send_BuyLimitItemListNot( IN const std::set<GCITEMID>& setList_ )
{
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_UPDATE_BUY_LIMIT_ITEM_LIST_NOT, setList_ );
}

void KCnItemManager::DumpBuyLimitItem()
{
    KLocker lock( m_csBuyLimitItem );

    START_LOG( cerr, L"--- Dump BuyLimitItem (size:" << m_setBuyLimitItem.size() << L") ---"  ) << END_LOG;

    std::set<GCITEMID>::iterator sit;
    for ( sit = m_setBuyLimitItem.begin() ; sit != m_setBuyLimitItem.end() ; ++sit ) {
        START_LOG( cerr, *sit << L", " ) << END_LOG;
    }    
    START_LOG( cerr, L"--- End Dump ---"  ) << END_LOG;
}

void KCnItemManager::SendToItemUpdateNot( IN const bool& bUpdate_ )
{
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_ITEM_UPDATE_NOT, bUpdate_ );
}
void KCnItemManager::UpdateCashbackDefaultRatio( IN const std::map< GCITEMID, int >& mapCashbackDefaultRatio_ )
{
    KLocker lock( m_csCashbackRatio );

    m_mapCashbackDefaultRatio = mapCashbackDefaultRatio_;

    START_LOG( clog, L"Cashback Default Ratio Update. size: " << mapCashbackDefaultRatio_.size() ) << END_LOG;
}

void KCnItemManager::UpdateCashbackEventRatio( IN const time_t tmStart_, IN const time_t tmEnd_, IN const std::map< GCITEMID, int >& mapCashbackEventRatio_ )
{
    KLocker lock( m_csCashbackRatio );

    m_mapCashbackEventRatio = mapCashbackEventRatio_;

    m_tmCashbackEventStart = tmStart_;
    m_tmCashbackEventEnd = tmEnd_;

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    if ( m_tmCashbackEventStart <= tmCurrent ) {
        m_bCashbackEventStarted = true;
    }
    if ( tmCurrent < m_tmCashbackEventEnd ) {
        m_bCashbackEventEnded = true;
    }

    START_LOG( clog, L"Cashback Event Ratio Update. size: " << mapCashbackEventRatio_.size() ) << END_LOG;

    START_LOG( clog, L"Cashback Event Time Info Update" )
        << BUILD_LOGtm( CTime( tmStart_ ) )
        << BUILD_LOGtm( CTime( tmEnd_ ) )
        << BUILD_LOG( m_bCashbackEventStarted )
        << BUILD_LOG( m_bCashbackEventEnded )
        << END_LOG;
}

void KCnItemManager::CheckCashbackEvent()
{
    KLocker lock( m_csCashbackRatio );

    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    if ( false == m_bCashbackEventStarted ) {
        if ( m_tmCashbackEventStart <= tmCurrent ) {
            // 캐시백 이벤트가 시작될 때 패킷을 전송한다
            KCashbackRatioInfo kPacket;
            kPacket.m_mapCashbackDefaultRatio = m_mapCashbackDefaultRatio;
            kPacket.m_mapCashbackEventRatio = m_mapCashbackEventRatio;

            SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CASHBACK_RATIO_INFO_NOT, kPacket );

            m_bCashbackEventStarted = true;
        }
    }
    if ( false == m_bCashbackEventEnded ) {
        if ( m_tmCashbackEventEnd < tmCurrent ) {
            // 캐시백 이벤트가 종료되니 관련 정보는 빈 컨테이너를 전달한다
            KCashbackRatioInfo kPacket;
            kPacket.m_mapCashbackDefaultRatio = m_mapCashbackDefaultRatio;
            kPacket.m_mapCashbackEventRatio.clear();

            SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CASHBACK_RATIO_INFO_NOT, kPacket );

            m_bCashbackEventEnded = true;
        }
    }
}

void KCnItemManager::SendCashbackRatioInfo( IN const std::map< GCITEMID, int >& mapCashbackDefaultRatio_, IN const std::map< GCITEMID, int >& mapCashbackEventRatio_ )
{
    KCashbackRatioInfo kPacket;
    kPacket.m_mapCashbackDefaultRatio = mapCashbackDefaultRatio_;
    kPacket.m_mapCashbackEventRatio = mapCashbackEventRatio_;

    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CASHBACK_RATIO_INFO_NOT, kPacket );
}

void KCnItemManager::GetCashbackRatioInfo( OUT KCashbackRatioInfo& kCashbackRatioInfo_ )
{
    KLocker lock( m_csCashbackRatio );

    kCashbackRatioInfo_.m_mapCashbackDefaultRatio = m_mapCashbackDefaultRatio;
    kCashbackRatioInfo_.m_mapCashbackEventRatio = m_mapCashbackEventRatio;
}

void KCnItemManager::SendItemBuyInfoNot()
{
    KItemBuyInfo kPacket;
    if ( false == GetBuyItemInfo( kPacket.m_bBuyEnable, kPacket.m_vecItemRange ) ) {
        return;
    }
    GetItemRecommInfo( kPacket.m_mapRecommInfo );

    START_LOG( clog, L"DB데이터 변경시 아이템 구매 가능 정보 게임서버에 전달.."
        << L" BuyEnable : " << ( kPacket.m_bBuyEnable ? L"True" : L"False" )
        << L", Size : " << kPacket.m_vecItemRange.size() ) << END_LOG;

    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_ITEM_BUY_INFO_NOT, kPacket );
}

bool KCnItemManager::SendPackageListChangedNot()
{
    bool bUpdated = true;

    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_EVENT_PACKAGE_LIST_CHANGED_NOT, bUpdated );

    START_LOG( cerr, L"게임 서버들에게 패키지 리스트 갱신 알림" ) << END_LOG;

    return true;
}
