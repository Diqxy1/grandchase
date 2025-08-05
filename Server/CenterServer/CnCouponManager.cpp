#include "CenterSimLayer.h" // winsock2
#include "CnCouponManager.h"
#include <dbg/dbg.hpp >
#include "CenterEvent.h"
#include "CommonPacket.h"
#include "CenterDBThread.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KCnCouponManager );
ImplOstmOperatorW2A( KCnCouponManager );
NiImplementRootRTTI( KCnCouponManager );

KCnCouponManager::KCnCouponManager(void)
:m_dwTicKGap(60000)
,m_bChanged(false)
{
    m_dwLastTicK = ::GetTickCount();
}

KCnCouponManager::~KCnCouponManager(void)
{
}

ImplToStringW( KCnCouponManager )
{
    KLocker lock( m_csCoupon );
    return START_TOSTRINGW
        << TOSTRINGW( m_dwTicKGap )
        << TOSTRINGW( m_dwLastTicK )
        << TOSTRINGWb( m_bChanged )
        << TOSTRINGW( m_vecCouponInfo.size() );
}

void KCnCouponManager::Tick()
{
    if( !CheckUpdateTime() ) return;

    DBUpdateCouponInfoReq();
    UpdateData();
    if( IsChanged() )
        SendCouponInfoToServer();
}

void KCnCouponManager::SetCouponData( IN std::vector<KGameCouponInfo>& vecCouponInfo_ )
{
    KLocker lock( m_csCoupon );
    if( vecCouponInfo_.empty() ) return;
    m_vecCouponInfo.swap( vecCouponInfo_ );
    UpdateData();
    SetChanged( true );
    START_LOG( clog, L"DB로 부터 쿠폰 정보 셋팅. Size : " << m_vecCouponInfo.size() ) << END_LOG;
}

void KCnCouponManager::AddCouponData( IN std::vector<KGameCouponInfo>& vecCouponInfo_ )
{
    KLocker lock( m_csCoupon );
    if( vecCouponInfo_.empty() ) return;

    std::vector<KGameCouponInfo>::iterator vit;
    for( vit = vecCouponInfo_.begin() ; vit != vecCouponInfo_.end() ; ++vit )
    {
        std::vector<KGameCouponInfo>::iterator vit1;
        vit1 = std::find_if( m_vecCouponInfo.begin(), m_vecCouponInfo.end(),
            boost::bind(&KGameCouponInfo::m_nEventUID, _1) == vit->m_nEventUID );

        if( vit1 == m_vecCouponInfo.end() ) // 새로운 이벤트
        {
            m_vecCouponInfo.push_back( *vit );
            START_LOG( clog, L"새로운 이벤트 추가.. : " << vit->m_nEventUID ) << END_LOG;
        }
        else // 변경된 이벤트
        {
            *vit1 = *vit;
            START_LOG( clog, L"이벤트 변경됨.. : " << vit->m_nEventUID ) << END_LOG;
        }
    }

    UpdateData();
    SetChanged( true );
    START_LOG( clog, L"DB로 부터 쿠폰 정보 갱신. Size : " << m_vecCouponInfo.size() ) << END_LOG;
}

void KCnCouponManager::GetCouponInfo( OUT std::vector<KGameCouponInfo>& vecCouponInfo_ )
{
    vecCouponInfo_.clear();
    KLocker lock( m_csCoupon );
    vecCouponInfo_ = m_vecCouponInfo;
}

void KCnCouponManager::UpdateData()
{
    time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );

    std::vector<KGameCouponInfo>::iterator vit;

    KLocker lock( m_csCoupon );
    // 시간이 오버된 녀석 제거
    vit = std::remove_if( m_vecCouponInfo.begin(), m_vecCouponInfo.end(),
        boost::bind( &KGameCouponInfo::m_tEndDate, _1 ) < tmCurrent );
    if( vit != m_vecCouponInfo.end() )
    {
        m_vecCouponInfo.erase( vit, m_vecCouponInfo.end() );
        m_bChanged = true;
    }

    // 활성 상태 갱신
    for( vit = m_vecCouponInfo.begin() ; vit != m_vecCouponInfo.end() ; ++vit )
    {
        if( vit->m_bActive == false && vit->m_tStartDate <= tmCurrent && vit->m_tEndDate >= tmCurrent )
        {
            vit->m_bActive = true;
            m_bChanged = true;
        }

        if( vit->m_bActive == true && vit->m_tStartDate > tmCurrent && vit->m_tEndDate < tmCurrent )
        {
            vit->m_bActive = false;
            m_bChanged = true;
        }
    }
}

void KCnCouponManager::DBUpdateCouponInfoReq()
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_COUPON_INFO_UPDATE_REQ, L"", 0 );
}

void KCnCouponManager::SendCouponInfoToServer()
{
    std::vector<KGameCouponInfo> kPacket;
    GetCouponInfo( kPacket );
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_COUPON_INFO_NOT, kPacket );
}

bool KCnCouponManager::IsChanged()
{
    KLocker lock( m_csCoupon );
    if( m_bChanged )
    {
        m_bChanged = false;
        return true;
    }
    return false;
}

void KCnCouponManager::SetChanged( bool bChange_ )
{
    KLocker lock( m_csCoupon );
    m_bChanged = bChange_;
}

bool KCnCouponManager::CheckUpdateTime()
{
    KLocker lock( m_csCoupon );
    if( ::GetTickCount() - m_dwLastTicK < m_dwTicKGap )
        return false;

    m_dwLastTicK = ::GetTickCount();
    return true;
}