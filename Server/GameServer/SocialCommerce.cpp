#include "CnConnector.h" // winsock
#include "SocialCommerce.h"
#include <dbg/dbg.hpp >
#include <kncutil.h>
#include "Log4.h"
#include "Lua/KLuaManager.h"
#include "UserEvent.h"
#include "GSDBLayer.h"

ImplementSingleton( KSocialCommerce );
ImplOstmOperatorW2A( KSocialCommerce );
NiImplementRootRTTI( KSocialCommerce );
ImplToStringW( KSocialCommerce )
{
    KLocker lock( m_csSocialCommerce );

    return START_TOSTRINGW
        << TOSTRINGW( m_dwLastTick )
        << TOSTRINGW( m_dwTickGap )
        << TOSTRINGW( m_bRun )
        << TOSTRINGW( m_mapInfoOfSellType.size() )
        << TOSTRINGW( 0 );
}

KSocialCommerce::KSocialCommerce(void)
: m_dwTickGap(1000*60)
, m_bRun(false)
{
    m_dwLastTick = ::GetTickCount();
    m_mapInfoOfSellType.clear();
}

KSocialCommerce::~KSocialCommerce(void)
{
}

void KSocialCommerce::Tick()
{
    // 하나라도 진행 되고 있는 이벤트가 있을 때 체크
    if( !ChkOneMinTick() || true == IsRun() ) { // 1분마다 / 이벤트 기간 체크.
        return;
    }

    UpdateAllSellInfo();
}

bool KSocialCommerce::ChkOneMinTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }

    m_dwLastTick = ::GetTickCount();
    return true;
}

bool KSocialCommerce::GetSocialCommerceItemInfoFromCurrentBuyItem( IN const DWORD& dwEventUID_, OUT KDB_EVENT_SOSCIAL_COMMERCE_BUY_REQ& kPacket_ )
{
    // 구매 요청 한 아이템이 소셜커머스 아이템인지 확인 하고 맞다면 정보 세팅
    std::map<DWORD, KSCSellInfo>::iterator mitSell;

    if( true == GetSellInfoByEventUID( dwEventUID_, mitSell, kPacket_.m_nType ) ) {
        KLocker lock( m_csSocialCommerce );

        // EventUID가 컨테이너에 있으면
        kPacket_.m_dwEventUID = mitSell->first;
        kPacket_.m_dwItemID = mitSell->second.m_dwItemID;
        return true;
    }

    return false;
}

void KSocialCommerce::UpdateAllSellInfo()
{
    KLocker lock( m_csSocialCommerce );

    std::map<int, KInformationDataOfSellType>::iterator mitAll;
    bool bRun = false; // 하나라도 진행 중인 이벤트가 있는지 확인할 변수

    // 전체 아이템의 정보를 갱신한다
    for ( mitAll = m_mapInfoOfSellType.begin(); mitAll != m_mapInfoOfSellType.end(); mitAll++ ) {
        std::map<DWORD, KSCSellInfo>::iterator mitSellInfo;

        // 판매타입 컨테이너의 아이템별 루프
        for ( mitSellInfo = mitAll->second.m_mapSellItem.begin(); mitSellInfo != mitAll->second.m_mapSellItem.end(); mitSellInfo++ ) {
            // 기간에 따른 상태 갱신
            UpdateFlagByPeriod( mitSellInfo, mitAll->first );

            // 수량에 따른 상태 갱신
            UpdateFlagByDuration( mitSellInfo, mitAll->first );

            if ( KSCSellInfo::FLAG_SELL_ING == mitSellInfo->second.m_nSellInfoFlag ) {
                // 하나라도 진행 중이다
                bRun = true;
            }
        }
    }

    // 하나라도 진행 중이라면 전체가 진행으로 처리
    m_bRun = bRun;
}

void KSocialCommerce::UpdateFlagByPeriod( IN std::map<DWORD, KSCSellInfo>::iterator& mitSell_, IN const int& nType_ )
{
    const time_t tmCurrent = KncUtil::TimeToInt( CTime::GetCurrentTime() );
    const time_t tmBegin = mitSell_->second.m_tmBegin; // 아이템별 이벤트 시작 날짜
    const time_t tmEnd =  mitSell_->second.m_tmEnd;    // 아이템별 이벤트 종료 날짜

    if ( tmBegin <= tmCurrent && tmEnd >= tmCurrent ) {
        // 오늘이 이벤트 기간에 포함 되는데 설정 되지 않은 상태라면 판매중으로 변경
        if ( KSCSellInfo::FLAG_SELL_GROUPPURCHASE_FAIL == mitSell_->second.m_nSellInfoFlag ) {
            mitSell_->second.m_nSellInfoFlag = KSCSellInfo::FLAG_SELL_ING;
        }
    }
    else if( tmEnd < tmCurrent ) { // 기간이 지났으면 종료
        mitSell_->second.m_nSellInfoFlag = KSCSellInfo::FLAG_SELL_OVER;

        // 공동 구매 아닌 애들은 바로 DB에 플래그 갱신
        if ( KSCSellInfo::SELLTYPE_TIME_LIMIT == nType_
            || KSCSellInfo::SELLTYPE_COUNT_LIMIT == nType_ ) {
            // DB에 판매 상태 flag를 변경요청
            SendFlagUpdateReqToDB( mitSell_->first, mitSell_->second.m_nSellInfoFlag );
        }
    }
}

void KSocialCommerce::UpdateFlagByDuration( IN std::map<DWORD, KSCSellInfo>::iterator& mitSell_, IN const int& nType_ )
{
    const int nLimitCount = mitSell_->second.m_nLimitCount;
    const int nCurSellCount = mitSell_->second.m_nCount;

    // 현재까지 판매 된 수량이 제한/달성 수량을 달성 했으면
    if ( nLimitCount <= nCurSellCount ) {
        // 수량 제한 판매의 경우 판매 종료로 상태 변경
        if ( KSCSellInfo::SELLTYPE_COUNT_LIMIT == nType_ ) {
            // 이미 판매 종료면 pass
            if ( KSCSellInfo::FLAG_SELL_OVER == mitSell_->second.m_nSellInfoFlag ) {
            return;
        }

            // 판매상태플래그를 종료로 표시
            mitSell_->second.m_nSellInfoFlag = KSCSellInfo::FLAG_SELL_OVER;

            SendFlagUpdateReqToDB( mitSell_->first, mitSell_->second.m_nSellInfoFlag );
        }
        else if ( KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE == nType_
            || KSCSellInfo::SELLTYPE_GROUP_PURCHASE_BONUS == nType_ ) {
                // 판매 중이 아니면 pass
                if ( KSCSellInfo::FLAG_SELL_ING != mitSell_->second.m_nSellInfoFlag ) {
                    return;
                }

                // 공동구매플래그를 성공으로 표시
                mitSell_->second.m_nGroupPurchaseFlag = KSCSellInfo::FLAG_SELL_GROUPPURCHASE_SUCCESS;
            // DB에 판매 상태 flag를 변경요청
            SendFlagUpdateReqToDB( mitSell_->first, mitSell_->second.m_nGroupPurchaseFlag );
        }
    }
    else { // if ( nLimitCount <= nCurSellCount ) {
        if ( KSCSellInfo::SELLTYPE_GROUP_PURCHASE_ONE_PLUS_ONE == nType_
            || KSCSellInfo::SELLTYPE_GROUP_PURCHASE_BONUS == nType_ ) {
                // 공구 판매 종료 되었고 수량 달성 실패
            if ( KSCSellInfo::FLAG_SELL_OVER == mitSell_->second.m_nSellInfoFlag ) {
                mitSell_->second.m_nGroupPurchaseFlag = KSCSellInfo::FLAG_SELL_GROUPPURCHASE_FAIL;
                // DB에 판매 상태 flag를 변경요청
                SendFlagUpdateReqToDB( mitSell_->first, mitSell_->second.m_nGroupPurchaseFlag );
            }
        }
    }
}

bool KSocialCommerce::GetSellInfoByEventUID( IN const DWORD dwEventUID_, OUT std::map<DWORD, KSCSellInfo>::iterator& mitSell_, OUT int& nType_ )
{
    std::map<int, KInformationDataOfSellType>::iterator mitAll;
    std::map<DWORD, KSCSellInfo>::iterator mitSell;

    KLocker lock( m_csSocialCommerce );
    // 전체 판매 타입에 대해서 검색 한다
    for( mitAll = m_mapInfoOfSellType.begin(); mitAll != m_mapInfoOfSellType.end(); mitAll++ ) {
        mitSell = mitAll->second.m_mapSellItem.find( dwEventUID_ );

        if ( mitAll->second.m_mapSellItem.end() != mitSell ) {
            mitSell_ = mitSell;
            nType_ = mitAll->first;
            return true;
        }
    }

    return false;
}

void KSocialCommerce::GetSocialCommerceData( OUT std::map<int, KInformationDataOfSellType>& mapInfoOfSellType_ )
{
    KLocker lock( m_csSocialCommerce );
    mapInfoOfSellType_ = m_mapInfoOfSellType;
}

void KSocialCommerce::UpdateSocialCommerceDataFromCenter( IN const std::map<int, KInformationDataOfSellType>& mapInfoOfSellType_ )
{
    KLocker lock( m_csSocialCommerce );
    if ( false == mapInfoOfSellType_.empty() ) {
        m_mapInfoOfSellType = mapInfoOfSellType_;
        UpdateAllSellInfo();

        START_LOG( cerr, L"센터로부터 소셜커머스 판매 정보 데이터 얻음 size : " << m_mapInfoOfSellType.size() ) << END_LOG;
    }
}

void KSocialCommerce::UpdateAllSellCountFromDB( IN std::vector<PAIR_DWORD_INT>& vecPrBuyCount_ )
{
    std::vector<PAIR_DWORD_INT>::iterator vit;
    std::map<int, KInformationDataOfSellType>::iterator mit;
    std::map<DWORD, KSCSellInfo>::iterator mitSell;

    // db에서 얻은 현재 판매 수량 정보 전체 컨테이너 for문
    for( vit = vecPrBuyCount_.begin(); vit != vecPrBuyCount_.end(); vit++ ) {
        DWORD dwEventUID = vit->first;
        int nSellCount = vit->second;
        int nSellType = KSCSellInfo::SELLTYPE_INVALID;

        // 현재 판매 수량 정보를 갱신 한다
        if ( true == GetSellInfoByEventUID( dwEventUID, mitSell, nSellType ) ) {
            KLocker lock( m_csSocialCommerce );

            mitSell->second.m_nCount = nSellCount;
        }
        else {
            START_LOG( cerr, L"[소셜커머스] DB에 설정 된 판매 EventUID와 현재 판매 수량 EventUID가 짝이 안맞음" << dwEventUID ) << END_LOG;
        }
    }
}

void KSocialCommerce::UpdateSellCountDiff( IN const DWORD& dwEventUID_, IN const int& nDiff_ )
{
    // 소셜커머스 수량 Diff값은 증가만 한다
    if ( 0 < nDiff_ ) {
        // iterator가 세팅이 된 경우만 true 리턴
        std::map<DWORD, KSCSellInfo>::iterator mitSell;
        int nType = KSCSellInfo::SELLTYPE_INVALID;

        if ( true == GetSellInfoByEventUID( dwEventUID_, mitSell, nType ) ) {
            KLocker lock( m_csSocialCommerce );

            mitSell->second.m_nCount += nDiff_;
            // 최대 값 보정
            std::min<int>( INT_MAX, mitSell->second.m_nCount );

            UpdateFlagByDuration( mitSell, nType );
        }
        else {
            START_LOG( cerr, L"[소셜커머스] 구매 후 수량 증가 하는데 EventUID에 해당 하는 판매 정보가 없음" ) << END_LOG;
        }
    }
    else {
        START_LOG( cerr, L"[소셜커머스] 증분 값이 0보다 작거나 같다 nDiff_ : " << nDiff_ ) << END_LOG;
    }
}

void KSocialCommerce::SendSellCountUpdateReqToDB( IN const DWORD& dwUID_, IN const KDB_EVENT_SOSCIAL_COMMERCE_BUY_REQ& kPacket_ )
{
    SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_SOSCIAL_COMMERCE_BUY_REQ, NULL, dwUID_, 0, kPacket_ );
}

void KSocialCommerce::SendFlagUpdateReqToDB( IN const DWORD& dwEventUID_, IN const int& nGroupPurchaseFlag_ )
{
    PAIR_DWORD_INT prData;
    prData = std::make_pair( dwEventUID_, nGroupPurchaseFlag_ );
    SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_SOSCIAL_COMMERCE_SELL_STATE_FLAG_UPDATE_REQ, NULL, 0, 0, prData );
}

void KSocialCommerce::UpdateSellCountAtBuyCashItem( IN const DWORD& dwEventUID_, IN const int& nCurSellCount_ )
{
    std::map<DWORD, KSCSellInfo>::iterator mitSell;
    int nSellType = KSCSellInfo::SELLTYPE_INVALID;
    
    if( true == GetSellInfoByEventUID( dwEventUID_, mitSell, nSellType ) ) {
        KLocker lock( m_csSocialCommerce );

        // DB에서 받은 판매 된 수량을 기준으로 플래그 갱신
        mitSell->second.m_nCount = nCurSellCount_;
        UpdateFlagByDuration( mitSell, nSellType );
    }
}

bool KSocialCommerce::CheckSellEnable( IN const DWORD& dwEventUID_ )
{
    std::map<DWORD, KSCSellInfo>::iterator mitSell;
    int nSellType = KSCSellInfo::SELLTYPE_INVALID;

    if( true == GetSellInfoByEventUID( dwEventUID_, mitSell, nSellType ) ) {
        KLocker lock( m_csSocialCommerce );

        if ( KSCSellInfo::FLAG_SELL_ING == mitSell->second.m_nSellInfoFlag ) {
            return true;
        }
    }

    return false;
}