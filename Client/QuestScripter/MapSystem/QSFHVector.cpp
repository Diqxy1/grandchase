#include "stdafx.h"
//#include "../stdafx.h"
//#include ".\QSFHVector.h"
////#include "player.h"
//
//KQSFHVector::KQSFHVector(void) : KFHVector<KQSFootHold>()
//{
//}
//
//KQSFHVector::~KQSFHVector(void)
//{
//    UnLoad();
//}
//
//void KQSFHVector::UnLoad()
//{
//    std::list<KQSFootHold*>::iterator lit = m_listFH.begin();
//    while( lit != m_listFH.end() )
//    {
//        KQSFootHold* pTemp = static_cast<KQSFootHold*>(*lit);
//        if( pTemp->GetParent() == NULL )
//        {
//            SAFE_DELETE( pTemp );
//        }
//        lit++;
//    }
//    m_listFH.clear();
//}
//
//KQSFootHold* KQSFHVector::GoAllowCheck( KQSFootHold* FootHold )
//{
//    if( m_listFH.empty() )
//        return NULL;
//
//    return _Check( FootHold );
//}
//
//KQSFootHold* KQSFHVector::_Check( KQSFootHold* FootHold )
//{
//    KQSFootHold* pAlreadyFind = NULL;
//    KQSFootHold* pTemp = NULL;
//
//    std::list<KQSFootHold*>::iterator lit = m_listFH.begin();
//    while( lit != m_listFH.end() )
//    {
//        pTemp = static_cast<KQSFootHold*>(*lit++);
//
//        // 나 자신의 발판을 밟을 수 없다
//        if( pTemp == FootHold )
//            continue;
//
//        if( pTemp->GetParent() == FootHold->GetParent() )
//            continue;
//
//        // 발판 끼리도 밟을 수 없다
//        if( pTemp->GetFootHoldType() & KQSFootHold::FHT_FOOT &&
//            FootHold->GetFootHoldType() & KQSFootHold::FHT_FOOT )
//            continue;
//
//        // 맵발판이 아니면 ( 오브젝트 발판을 우선으로 밟게 하고 싶었나보네 )
//        if( pTemp->GetParent() != NULL )
//            return pTemp;
//
//        pAlreadyFind = pTemp;
//    }
//
//    return pAlreadyFind;
//}
//
//KQSFootHold* KQSFHVector::CrashCheck( bool bMapFoot/* = false*/, DWORD dwProp/* = KQSFootHold::FHP_JUST_FOOTHOLD*/, bool bCheckInv/* = false*/ )
//{
//    if( m_listFH.empty() )
//        return NULL;
//    
//    std::list<KQSFootHold*>::iterator lit = m_listFH.begin();
//    while( lit != m_listFH.end() )
//    {
//        if( (*lit)->GetFootHoldType() & KQSFootHold::FHT_HEAD )
//        {
//            bool bCheck;
//            if( bCheckInv )
//            {
//                bCheck = !((*lit)->GetFootHoldProp() & dwProp);
//            }
//            else
//            {
//                bCheck = (*lit)->GetFootHoldProp() & dwProp || dwProp == KQSFootHold::FHP_JUST_FOOTHOLD;
//            }
//
//            if( bCheck )
//            {
//                if( bMapFoot )
//                {
//                    if( static_cast<KQSFootHold*>(*lit)->GetParent() == NULL )
//                    {
//                        return (*lit);
//                    }
//                }
//                else
//                {
//                    return (*lit);
//                }
//            }
//        }
//        lit++;
//    }
//
//    return NULL;
//}
