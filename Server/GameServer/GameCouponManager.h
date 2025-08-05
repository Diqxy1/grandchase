#pragma once
#include "UserPacket.h"
#include <vector>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

//DECL_DATA( KGameCouponInfo )
//{
//    int             m_nEventUID;    // 이벤트UID
//    bool            m_bActive;      // 동작중인지(서버시간으로 결정)
//    time_t          m_tStartDate;   // 시작시간
//    time_t          m_tEndDate;     // 끝시간
//    std::wstring    m_strEventName; // 이벤트명(ex.신학기 이벤트)
//    std::wstring    m_strContents;  // 이벤트설명(ex.신학기 쿠폰을 입력하면 교복 아이템이 펑펑~)
//};
//typedef KEVENT_GAME_COUPON_LIST_ACK KEVENT_GAME_COUPON_LIST_ACK; // <이벤트UID, 게임쿠폰정보>

class KGameCouponManager
{
    DeclareSingleton( KGameCouponManager );
    NiDeclareRootRTTI( KGameCouponManager );
    DeclToStringW;

public:
    enum INFO_TYPE
    {
        IT_UPDATE_INFO = 0,
        IT_ALL_INFO = 1,
    };
    
    enum TICK_ID
    {
        TI_TICK,
        TI_UPDATE,
        TI_MAX,
    };

    KGameCouponManager(void);
    ~KGameCouponManager(void);

    void Tick();
    void SetGameCouponInfo( IN const KEVENT_GAME_COUPON_LIST_ACK& vecGameCouponList );
    void GetGameCouponInfo( OUT KEVENT_GAME_COUPON_LIST_ACK& vecGameCouponList );
    void UpdateGameCouponInfo( IN const KEVENT_GAME_COUPON_LIST_ACK& vecGameCouponList );
    bool IsValidGameCouponInfo( IN const int nEventUID );
    void DumpGameCoupon();
    void PrintGameCouponInfo( IN const int nEventUID );
    void GetFirstDataFromDB();

protected:
    void UpdateState();
    void UpdateDataFromDB();
    // for Tick
    void SetTick( IN const int nIndex );
    DWORD GetTick( IN const int nIndex );
    void SetTickGap( IN const int nIndex );
    DWORD GetTickGap( IN const int nIndex );

protected:
    mutable KncCriticalSection          m_csGameCoupon;
    KEVENT_GAME_COUPON_LIST_ACK         m_vecGameCouponList;       // 게임쿠폰 이벤트 정보

    mutable KncCriticalSection          m_csTick;
    DWORD                               m_dwTick[TI_MAX];
    DWORD                               m_dwTickGap[TI_MAX];
};

DefSingletonInline( KGameCouponManager );
DeclOstmOperatorA( KGameCouponManager );