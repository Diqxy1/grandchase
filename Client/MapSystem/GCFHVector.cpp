////#include "stdafx.h"
//#include ".\gcfhvector.h"
//#include "player.h"
//
//KGCFHVector::KGCFHVector(void) : KFHVector<KGCFootHold>()
//{	
//}
//
//KGCFHVector::~KGCFHVector(void)
//{
//    UnLoad();
//}
//
//void KGCFHVector::UnLoad()
//{
//    std::list<KGCFootHold*>::iterator lit = m_listFH.begin();
//    while( lit != m_listFH.end() )
//    {
//        KGCFootHold* pTemp = static_cast<KGCFootHold*>(*lit);
//        if( pTemp->GetParent() == NULL )
//        {
//            SAFE_DELETE( pTemp );
//        }
//        lit++;
//    }
//    m_listFH.clear();
//}
//
//KGCFootHold* KGCFHVector::GoAllowCheck( KGCFootHold* FootHold )
//{
//    if( m_listFH.empty() )
//        return NULL;
//
//    return _Check( FootHold );
//}
//
//KGCFootHold* KGCFHVector::_Check( KGCFootHold* FootHold )
//{
//    KGCFootHold* pAlreadyFind = NULL;
//    KGCFootHold* pTemp = NULL;
//
//    std::list<KGCFootHold*>::iterator lit = m_listFH.begin();
//    while( lit != m_listFH.end() )
//    {
//        pTemp = static_cast<KGCFootHold*>(*lit++);
//
//        // 나 자신의 발판을 밟을 수 없다
//        if( pTemp == FootHold )
//            continue;
//
//        if( pTemp->GetParent() == FootHold->GetParent() )
//            continue;
//
//        // 발판 끼리도 밟을 수 없다
//        if( pTemp->GetFootHoldType() & KGCFootHold::FHT_FOOT &&
//            FootHold->GetFootHoldType() & KGCFootHold::FHT_FOOT )
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
//KGCFootHold* KGCFHVector::CrashCheck( bool bMapFoot/* = false*/, DWORD dwProp/* = KGCFootHold::FHP_JUST_FOOTHOLD*/, bool bCheckInv/* = false*/ )
//{
//    if( m_listFH.empty() )
//        return NULL;
//    
//    std::list<KGCFootHold*>::iterator lit = m_listFH.begin();
//    while( lit != m_listFH.end() )
//    {
//        if( (*lit)->GetFootHoldType() & KGCFootHold::FHT_HEAD )
//        {
//            bool bCheck;
//            if( bCheckInv )
//            {
//                bCheck = !((*lit)->GetFootHoldProp() & dwProp);
//            }
//            else
//            {
//                bCheck = (*lit)->GetFootHoldProp() & dwProp || dwProp == KGCFootHold::FHP_JUST_FOOTHOLD;
//            }
//
//            if( bCheck )
//            {
//                if( bMapFoot )
//                {
//                    if( static_cast<KGCFootHold*>(*lit)->GetParent() == NULL )
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
