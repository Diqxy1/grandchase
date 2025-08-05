#pragma once
#include <KNCSingleton.h>
#include <ToString.h>
#include <vector>
#include <Thread/Locker.h>
#include <RTTI.h>
#include "PrimitiveTypedef.h"

class KVirtualCash
{
    DeclareSingleton( KVirtualCash );
    NiDeclareRootRTTI( KVirtualCash );
    DeclToStringW;

public:
    KVirtualCash(void);
    ~KVirtualCash(void);

    enum
    {
        VC_NO_LIMIT = -1,
    };

    enum EVirtualCashMergeType {
        EVCMT_MOD_DBOT                          = 0, // DBOT으로 변경
        EVCMT_ADD_CASHBACK                      = 1, // 캐시백으로 가산
        EVCMT_ADD_COUPON                        = 2, // 쿠폰 사용하여 가산
        EVCMT_SUB_VCASH_ITEM_BUY                = 3, // 아이템 구매로 차감
        EVCMT_ADD_LA_US_AUTH_CHECK              = 4, // 남미용 미국 유저 인증 보상 가산
        EVCMT_SUB_LA_US_EXPIRED_AUTH_CHECK      = 5, // 남미용 미국 유저 인증기간 만료 초기화용 차감
        EVCMT_SUB_MIXCASH_ITEM_BUY              = 6, // 캐시와 가상캐시 섞어쓰기 아이템 구매로 차감
        EVCMT_ADD_MIXCASH_ITEM_BUY_ROLLBACK     = 7, // 캐시와 가상캐시 섞어쓰기 아이템 구매로 차감된 것 롤백 가산

        EVCMT_MAX,
    };

    void UpdateRestrictItemList( IN std::vector<GCITEMID>& vecItemList_ );
    bool FindRestrictItem( IN GCITEMID dwItemID_ );
    bool LimitVirtualCashRatio( DWORD dwItemPrice_, DWORD dwDeductPrice_ );
    void GetLimitRatio( IN int& nLimitRatio_ ) { KLocker lock( m_csVirtualCash ); nLimitRatio_ = m_nVCMixRatio; }
    void SetLimitRatio( IN int nLimitRatio_);

protected:
    mutable KncCriticalSection  m_csVirtualCash;
    std::vector< GCITEMID >     m_vecRestrictItemList;
    int                         m_nVCMixRatio; //  090716  woosh.  홍콩에서는 아이템 가격의 20 % ( x % ) 를 가상캐쉬 최대치.

};

DefSingletonInline( KVirtualCash );
DeclOstmOperatorA( KVirtualCash );