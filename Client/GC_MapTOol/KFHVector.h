//#pragma once
//
//#include <list>
//
//template <class T0>
//class KFHVector
//{
//public:
//    KFHVector(void);
//    virtual ~KFHVector(void);
//
//    virtual void            UnLoad() = 0;
//
//    virtual void            AddMember( T0* FootHold );
//    virtual void            DeleteMember( T0* FootHold );
//
//    inline bool             empty() { return m_listFH.empty(); }
//    inline int              size() { return (int)m_listFH.size(); }
//
//protected:
//    std::list<T0*>          m_listFH;
//};
//
//#include "KFHVector.inl"
