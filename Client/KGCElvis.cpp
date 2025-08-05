#include "stdafx.h"
#include ".\kgcelvis.h"

ImplementSingleton( KGCElvis );

KGCElvis::KGCElvis(void)
{
    //m_kBonusPoint.m_nBaseBonus = 0;
    //m_kBonusPoint.m_nSpecialBonus = 0;
    m_nRevivalCount = 0;
#if defined( NATION_TAIWAN )
    m_nMaxRevivalCount = 10;
#elif defined( NATION_KOREA )
    m_nMaxRevivalCount = MAX_BONUS_NUM;
#else
    m_nMaxRevivalCount = 3;
#endif
}

KGCElvis::~KGCElvis(void)
{
}

void KGCElvis::SetBonusPointInfo( const KBonusPointInfo& Point )
{
    g_kGlobalValue.m_kUserInfo.GetCurrentChar().SetBonusPoint( Point );
    g_MyD3D->m_TempPlayer.GetCurrentChar().SetBonusPoint( Point );

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if ( pPlayer )
        pPlayer->GetCurrentChar().SetBonusPoint( Point );
}

void KGCElvis::SetBonusPointInfo( const int nCharType, const KBonusPointInfo& Point )
{
    SCharInfo& sCharInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar( nCharType );
    if ( sCharInfo.iCharType == nCharType )
        sCharInfo.SetBonusPoint( Point );

    SCharInfo& sCharInfo2 = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar( nCharType );
    if ( sCharInfo2.iCharType == nCharType )
        sCharInfo2.SetBonusPoint( Point );

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if ( pPlayer )
    {
        SCharInfo& sCharInfo3 = pPlayer->m_kUserInfo.GetCurrentChar( nCharType );
        if ( sCharInfo3.iCharType == nCharType )
            sCharInfo3.SetBonusPoint( Point );
    }
}

int KGCElvis::GetBaseBonusPoint( void ) const
{
    return g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetBonusPoint().m_nBaseBonus;
}

int KGCElvis::GetSpBonusPoint( void ) const
{
    return g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetBonusPoint().m_nSpecialBonus;
}

int KGCElvis::GetCurrentBonusPoint( void ) const
{
    KBonusPointInfo kInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetBonusPoint();
    return kInfo.m_nBaseBonus + kInfo.m_nSpecialBonus;
}

int KGCElvis::GetMaxChargePoint( void ) const
{
    KBonusPointInfo kInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar().GetBonusPoint();
    int nChargPoint = ( MAX_BONUS_NUM - ( kInfo.m_nBaseBonus + kInfo.m_nSpecialBonus ) );

    if ( nChargPoint < 0 )
        nChargPoint = 0;

    if ( nChargPoint > MAX_BONUS_NUM )
        nChargPoint = MAX_BONUS_NUM;

    return nChargPoint;
}