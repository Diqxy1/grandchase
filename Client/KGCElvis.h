#pragma once

//#include "UserPacket.h"
#include <KNCSingleton.h>

#define MAX_BONUS_NUM (99)

// [12/17/2007] breadceo.
// Life Bonus System = LBS = Elvis
class KGCElvis
{
    DeclareSingleton( KGCElvis );

public:
    KGCElvis(void);
    virtual ~KGCElvis(void);

    bool IsRevivalable() const { return m_nRevivalCount < m_nMaxRevivalCount; }
    void IncreaseRevivalCount() { m_nRevivalCount++; }
    void SetRevivalCount( int nCount ) { m_nRevivalCount = nCount; }
    void SetMaxRevivalCount( int iRevivalCount ) { m_nMaxRevivalCount = iRevivalCount; }
    inline int GetMaxRevivalCount() const { return m_nMaxRevivalCount; }
    inline int GetRevivalCount() const { return m_nRevivalCount; }
    
    //void SetBonusPointInfo( const KBonusPointInfo& Point ) { m_kBonusPoint = Point; }
    //inline int GetBaseBonusPoint() const { return m_kBonusPoint.m_nBaseBonus; }
    //inline int GetSpBonusPoint() const { return m_kBonusPoint.m_nSpecialBonus; }
    //inline int GetCurrentBonusPoint() const { return m_kBonusPoint.m_nBaseBonus + m_kBonusPoint.m_nSpecialBonus; }
    //inline int GetMaxChargePoint() const { return MAX_BONUS_NUM - (m_kBonusPoint.m_nBaseBonus + m_kBonusPoint.m_nSpecialBonus); }

    void SetBonusPointInfo( const KBonusPointInfo& Point ); // { m_kBonusPoint = Point; }
    void SetBonusPointInfo( const int nCharType, const KBonusPointInfo& Point );
    int  GetBaseBonusPoint( void ) const; // { return m_kBonusPoint.m_nBaseBonus; }
    int  GetSpBonusPoint( void ) const; // { return m_kBonusPoint.m_nSpecialBonus; }
    int  GetCurrentBonusPoint( void ) const; // { return m_kBonusPoint.m_nBaseBonus + m_kBonusPoint.m_nSpecialBonus; }
    int  GetMaxChargePoint( void ) const; // { return MAX_BONUS_NUM - (m_kBonusPoint.m_nBaseBonus + m_kBonusPoint.m_nSpecialBonus); }

private:
    //KBonusPointInfo m_kBonusPoint;
    int             m_nRevivalCount;
    int             m_nMaxRevivalCount;
};

DefSingletonInline( KGCElvis );
