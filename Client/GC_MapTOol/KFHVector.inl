//template <class T0>
//KFHVector<T0>::KFHVector(void)
//{
//    m_listFH.clear();
//}
//
//template <class T0>
//KFHVector<T0>::~KFHVector(void)
//{
//}
//
//template <class T0>
//void KFHVector<T0>::AddMember( T0* FootHold )
//{
//    // 있으면 리턴
//    if( m_listFH.end() != std::find( m_listFH.begin(), m_listFH.end(), FootHold ) )
//        return;
//
//    m_listFH.push_back( FootHold );
//}
//
//template <class T0>
//void KFHVector<T0>::DeleteMember( T0* FootHold )
//{
//    std::list<T0*>::iterator lit = std::find( m_listFH.begin(), m_listFH.end(), FootHold );
//    if( lit == m_listFH.end() )
//        return;
//
//    m_listFH.erase( lit );
//}