// #include "stdafx.h"
// #include ".\KGCBuffArmeFrostringLv2.h"
// #include "MyD3D.h"
// #include "KGCBuffManager.h"
//
//
// KGCBuffArmeFrostringLv2::KGCBuffArmeFrostringLv2(void)
// {
//     m_fOriMPRatio = 0.0f;
// }
//
// KGCBuffArmeFrostringLv2::~KGCBuffArmeFrostringLv2(void)
// {
// }
//
// bool KGCBuffArmeFrostringLv2::Init( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, float fBuffTime, int iIndex_, bool bIsCrashPlayer, DWORD dwGivePlayerUID )
// {
//     if ( KGCBuff::Init( iPlayerIndex_, pBuffInst_, fBuffTime, -1, bIsCrashPlayer, dwGivePlayerUID ) )
//     {
//         if( m_bIsCrashPlayer && g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer() ) {
//             //g_MyD3D->m_fIncMPRatio는 0보다 작을 수 없음. 디폴트가 1이어서 증가율을 늘리던 줄이든 0이상임을 보장해주어야 함.
//             if( g_MyD3D->m_fIncMPRatio  > 0.f ) {
//                 m_fOriMPRatio = g_MyD3D->m_fIncMPRatio;
//                 g_MyD3D->m_fIncMPRatio = 0.0f;
//             }
//         }
//         return true;
//     }
//
//     return false;
// }
//
// bool KGCBuffArmeFrostringLv2::Destroy( int iPlayerIndex_, KGCBuffInstance* pBuffInst_, bool bOnDie_, bool bNextStage_ )
// {
//     if ( KGCBuff::Destroy( iPlayerIndex_, pBuffInst_, bOnDie_, bNextStage_ ) )
//     {
//         if( m_fOriMPRatio > 0.f &&
//             g_MyD3D->IsPlayerIndex(iPlayerIndex_) &&
//             g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer() ) {
//             if( m_fOriMPRatio > 0.f ) {
//                 g_MyD3D->m_fIncMPRatio = m_fOriMPRatio;
//             }
//         }
//         return true;
//     }
//     return false;
// }
//
//bool KGCBuffArmeFrostringLv2::FrameMove( int iPlayerIndex_, KGCBuffInstance* pBuffInst_ )
//{
//    if( KGCBuff::FrameMove( iPlayerIndex_,pBuffInst_ ) )
//    {
//        if( g_MyD3D->MyPlayer[iPlayerIndex_]->IsLocalPlayer() &&
//            !g_pMagicEffect->IsCoolTime(iPlayerIndex_, pBuffInst_->m_iBuffEnum) )
//        {
//            g_MyD3D->m_fIncMPRatio = 0.f;
//        }
//        return true;
//    }
//    return false;
//}