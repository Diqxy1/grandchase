#pragma once
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include "KncTimer.h"
#include "CommonPacket.h"
#include <boost/array.hpp>

class KLuaManager;
class KGambleBuyManager
{
    DeclareSingleton( KGambleBuyManager );
    NiDeclareRootRTTI( KGambleBuyManager );
    DeclToStringW;
public:
    KGambleBuyManager(void);
    ~KGambleBuyManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    void SetCostRate( IN const float fRate ) { KLocker lock( m_csCostRate ); m_fCostRate = fRate; }
    float GetCostRate()                      { KLocker lock( m_csCostRate ); return m_fCostRate; }
    int GetGradeByLevel( IN const int nLevel );
    void TestGamble( IN const int nLevel, IN const int nTry );

private:
    mutable KncCriticalSection              m_csCostRate;
    float                                   m_fCostRate; // 겜블 아이템 구매비용 비율(1.f : 100%)

    // 겜블 확률
    mutable KncCriticalSection              m_csGradeRatio;
    float                                   m_fMinGambleRatio;
    float                                   m_fMaxGambleRatio;
    float                                   m_fLevelModNum;
    boost::array<float,KItem::GRADE_NUM>    m_pGradeRatio; // 겜블구매시 등급별로 나올 확률(1.f : 100%)
};

DefSingletonInline( KGambleBuyManager );
DeclOstmOperatorA( KGambleBuyManager );