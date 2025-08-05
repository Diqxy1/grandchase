#include "GameCouponManager.h"
#include <KncUtilLua.h> // boost::bind
#include <dbg/dbg.hpp >
#include "UserEvent.h"
#include "GSDBLayer.h"
#include "NetError.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KGameCouponManager );
ImplOstmOperatorW2A( KGameCouponManager );
NiImplementRootRTTI( KGameCouponManager );

KGameCouponManager::KGameCouponManager(void)
{
    for( int i = 0 ; i < TI_MAX ; ++i )
    {
        SetTick( i );
        m_dwTickGap[i] = 60 * 1000;
    }
}

KGameCouponManager::~KGameCouponManager(void)
{
}

ImplToStringW( KGameCouponManager )
{
    KLocker lock( m_csGameCoupon );

    return START_TOSTRINGW
        << TOSTRINGW( m_vecGameCouponList.size() );
}

void KGameCouponManager::Tick()
{
    DWORD dwCurrentTick = ::GetTickCount();

    if( dwCurrentTick - GetTick( TI_TICK ) > GetTickGap( TI_TICK ) )
    {
        UpdateState();
        SetTick( TI_TICK );
    }

    if( dwCurrentTick - GetTick( TI_UPDATE ) > GetTickGap( TI_UPDATE ) )
    {
        UpdateDataFromDB();
        SetTick( TI_UPDATE );
    }
}

void KGameCouponManager::SetGameCouponInfo( IN const KEVENT_GAME_COUPON_LIST_ACK& vecGameCouponList_ )
{
    {
        KLocker lock( m_csGameCoupon );
        m_vecGameCouponList = vecGameCouponList_;
    }

    START_LOG( cerr, L"게임쿠폰 정보 설정완료. size : " << vecGameCouponList_.size() ) << END_LOG;
}

void KGameCouponManager::GetGameCouponInfo( OUT KEVENT_GAME_COUPON_LIST_ACK& vecGameCouponList_ )
{
    KLocker lock( m_csGameCoupon );
    vecGameCouponList_ = m_vecGameCouponList;
}

void KGameCouponManager::UpdateGameCouponInfo( IN const KEVENT_GAME_COUPON_LIST_ACK& vecGameCouponList_ )
{
    KEVENT_GAME_COUPON_LIST_ACK::const_iterator cvit;
    KEVENT_GAME_COUPON_LIST_ACK::iterator vit;

    KLocker lock( m_csGameCoupon );

    for( cvit = vecGameCouponList_.begin() ; cvit != vecGameCouponList_.end() ; ++cvit )
    {
        // EventUID 로 찾아서
        vit = std::find_if( m_vecGameCouponList.begin(), m_vecGameCouponList.end(),
                            boost::bind( &KGameCouponInfo ::m_nEventUID, _1 ) == cvit->m_nEventUID );

        // 없는 경우 입력
        if( vit == m_vecGameCouponList.end() )
        {
            m_vecGameCouponList.push_back( *cvit );
            continue;
        }

        // 있는 경우 갱신
        *vit = *cvit;
    }

    START_LOG( clog, L"게임쿠폰 정보 갱신됨. size : " << m_vecGameCouponList.size() ) << END_LOG;
}

bool KGameCouponManager::IsValidGameCouponInfo( IN const int nEventUID_ )
{
    KLocker lock( m_csGameCoupon );

    KEVENT_GAME_COUPON_LIST_ACK::iterator vit;
    vit = std::find_if( m_vecGameCouponList.begin(), m_vecGameCouponList.end(),
                        boost::bind( &KGameCouponInfo ::m_nEventUID, _1 ) == nEventUID_ );

    if( vit == m_vecGameCouponList.end() )
    {
        START_LOG( cerr, L"없는 쿠폰이벤트. UID : " << nEventUID_ ) << END_LOG;
        return false;
    }

    if( vit->m_bActive == false )
    {
        START_LOG( cerr, L"종료된 쿠폰이벤트. UID : " << nEventUID_ ) << END_LOG;
        return false;
    }

    return true;
}

void KGameCouponManager::DumpGameCoupon()
{
    KLocker lock( m_csGameCoupon );
    KEVENT_GAME_COUPON_LIST_ACK::iterator vit;

    START_LOG( cwarn, L"=== Dump GameCoupon ==== "  ) << END_LOG;
    for( vit = m_vecGameCouponList.begin() ; vit != m_vecGameCouponList.end() ; ++vit )
    {
        START_LOG( cwarn, L"  " << vit->m_nEventUID << L")/"
                            << (vit->m_bActive == true ? L"T":L"F") << L"/"
                            << (vit->m_bNew == true ? L"T":L"F") << L"/"
                            << (LPCTSTR)CTime( vit->m_tStartDate ).Format( KNC_TIME_FORMAT ) << L"~"
                            << (LPCTSTR)CTime( vit->m_tEndDate ).Format( KNC_TIME_FORMAT ) << L"("
                            << KNC_TIME_STRING << L")/"
                            << vit->m_strEventName << L"/"
                            << vit->m_strContents ) << END_LOG;
    }
    START_LOG( cwarn, L"=== End Dump ==== "  ) << END_LOG;
}

void KGameCouponManager::PrintGameCouponInfo( IN const int nEventUID_ )
{
    KLocker lock( m_csGameCoupon );
    KEVENT_GAME_COUPON_LIST_ACK::iterator vit;

    vit = std::find_if( m_vecGameCouponList.begin(), m_vecGameCouponList.end(),
                        boost::bind( &KGameCouponInfo ::m_nEventUID, _1 ) == nEventUID_ );

    if( vit == m_vecGameCouponList.end() )
    {
        START_LOG( cerr, L"해당하는 게임쿠폰 이벤트가 없습니다.. UID : " << nEventUID_ ) << END_LOG;
        return;
    }

    START_LOG( cerr, L"게임쿠폰 이벤트 정보. EventUID : " << nEventUID_ )
        << BUILD_LOG( vit->m_nEventUID )
        << BUILD_LOG( vit->m_bNew )
        << BUILD_LOG( vit->m_bActive )
        << BUILD_LOG( KNC_TIME_STRING )
        << BUILD_LOG( (LPCTSTR)CTime( vit->m_tStartDate ).Format( KNC_TIME_FORMAT ) )
        << BUILD_LOG( (LPCTSTR)CTime( vit->m_tEndDate ).Format( KNC_TIME_FORMAT ) )
        << BUILD_LOG( vit->m_strEventName )
        << BUILD_LOG( vit->m_strContents ) << END_LOG;
}

void KGameCouponManager::UpdateState()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    KEVENT_GAME_COUPON_LIST_ACK::iterator vit;

    KLocker lock( m_csGameCoupon );
    for( vit = m_vecGameCouponList.begin() ; vit != m_vecGameCouponList.end() ; ++vit )
    {
        // 쿠폰이벤트 기간체크에 따른 시간별 상태갱신
        if( vit->m_tStartDate <= tmCurrent && vit->m_tEndDate >= tmCurrent )
        {
            vit->m_bActive = true; // 이벤트 동작
        }
        else
        {
            vit->m_bActive = false; // 이벤트 종료
        }

        START_LOG( clog, L"Update State. EventUID:" << vit->m_nEventUID << L",Active:" << vit->m_bActive ) << END_LOG;
    }
}

void KGameCouponManager::GetFirstDataFromDB()
{
    int kPacket = IT_ALL_INFO;
    SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_UPDATE_GAME_COUPON_FROM_DB, NULL, 0, 0, kPacket );
}

void KGameCouponManager::UpdateDataFromDB()
{
    int kPacket = IT_UPDATE_INFO;
    SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_UPDATE_GAME_COUPON_FROM_DB, NULL, 0, 0, kPacket );
}

void KGameCouponManager::SetTick( IN const int nIndex_ )
{
    JIF( nIndex_ >= 0 && nIndex_ < TI_MAX );
    KLocker lock( m_csTick );
    m_dwTick[nIndex_] = ::GetTickCount();
}

DWORD KGameCouponManager::GetTick( IN const int nIndex_ )
{
    _JIF( nIndex_ >= 0 && nIndex_ < TI_MAX , return 0 );
    KLocker lock( m_csTick );
    return m_dwTick[nIndex_];
}

void KGameCouponManager::SetTickGap( IN const int nIndex_ )
{
    JIF( nIndex_ >= 0 && nIndex_ < TI_MAX );
    KLocker lock( m_csTick );
    m_dwTickGap[nIndex_] = ::GetTickCount();
}

DWORD KGameCouponManager::GetTickGap( IN const int nIndex_ )
{
    _JIF( nIndex_ >= 0 && nIndex_ < TI_MAX , return 0xFFFFFFFF );
    KLocker lock( m_csTick );
    return m_dwTickGap[nIndex_];
}
