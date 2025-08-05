#pragma once

#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <vector>

struct KGameCouponInfo;
class KCnCouponManager
{
    DeclareSingleton( KCnCouponManager );
    NiDeclareRootRTTI( KCnCouponManager );
    DeclToStringW;

public:
    KCnCouponManager(void);
    ~KCnCouponManager(void);

    void Tick();
    void SetCouponData( IN std::vector<KGameCouponInfo>& vecCouponInfo );
    void AddCouponData( IN std::vector<KGameCouponInfo>& vecCouponInfo );
    void GetCouponInfo( OUT std::vector<KGameCouponInfo>& vecCouponInfo );

protected:
    void UpdateData();
    void DBUpdateCouponInfoReq();
    void SendCouponInfoToServer();
    bool IsChanged();
    void SetChanged( IN bool bChange );
    bool CheckUpdateTime();

protected:
    mutable KncCriticalSection      m_csCoupon;
    DWORD                           m_dwTicKGap;
    DWORD                           m_dwLastTicK;
    bool                            m_bChanged;
    std::vector<KGameCouponInfo>    m_vecCouponInfo;

};

DefSingletonInline( KCnCouponManager );
DeclOstmOperatorA( KCnCouponManager );