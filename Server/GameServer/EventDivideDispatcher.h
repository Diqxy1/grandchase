#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include <deque>
#include <KncTimer.h>
#include "Lua/KLuaManager.h"
#include "IntEvent.h"
#include "GSDBLayer.h"

class KEventDivideDispatcher
{
    DeclareSingleton( KEventDivideDispatcher );
    NiDeclareRootRTTI( KEventDivideDispatcher );
    DeclToStringW;

public:
    KEventDivideDispatcher(void);
    ~KEventDivideDispatcher(void);

public:
    struct KEventInfo
    {
        unsigned short  m_usEventID;
        DWORD           m_dwProcessTimeInterval;
        int             m_nQueueSizeThreshold;
        int             m_nProcessCount;
        DWORD           m_dwLastTick;

        void Reset()
        {
            m_dwLastTick = ::GetTickCount();
        }

        bool CheckTime()
        {
            if ( ::GetTickCount() - m_dwLastTick < m_dwProcessTimeInterval ) {
                return false;
            }
            Reset();

            return true;
        }
        int GetPassCount()  // 이 함수의 호출을 최대한 줄이자
        {
            int nPassCount = m_nQueueSizeThreshold - SiKGSDBLayer()->GetQueueSize();
            if ( 0 >= nPassCount ) {
                return -1;
            }
            else {
                if ( m_nProcessCount <= nPassCount ) {
                    nPassCount = m_nProcessCount;
                }
            }
            return nPassCount;
        }
    };

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void Tick();

    // queueing to db
    template< typename T >
    void QueueingEvent( IN const unsigned short usEventID_, IN const wchar_t* szLogin_, IN const DWORD dwUID_, T& data_ );
    void QueueingEvent( IN KIntEventPtr spEvent_ );
    //void QueueingID( unsigned short usEventID, const wchar_t* szLogin, DWORD dwUID );

    // actor manager 에 큐잉하는 부분은 아래 함수들을 구현하면 된다
    // queueing to user
    //template< class T > 
    //void QueueingEventTo( const std::wstring& strName, const unsigned short usEventID, const T& data, const int nFrom = KEvent::FT_NONE );
    //void QueueingEventTo( const std::wstring& strName, const KEventPtr& spEvent, const int nFrom = KEvent::FT_NONE );
    //void QueueingEventTo( const DWORD dwUID, const KEventPtr& spEvent, const int nFrom = KEvent::FT_NONE );

    void PrintStatus();
    void PrintDispatchInfo();
    void PrintRelateTable();
    void PrintEventInfo();

private:
    bool LoadEventInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map< unsigned short, KEventInfo >& mapDispatchTable_ );
    unsigned short GetEventID( IN const std::string& strEventID_ );

protected:
    mutable KncCriticalSection                                      m_csEventInfo;

    std::map< unsigned short, KEventInfo >                          m_mapDispatchTable;     // 틱 별 처리 할 이벤트 정보 리스트
    std::map< unsigned short, std::deque< KIntEventPtr > >          m_mapEvents;            // 유저 이벤트 구조체를 쌓아둘 곳

};

DefSingletonInline( KEventDivideDispatcher );
DeclOstmOperatorA( KEventDivideDispatcher );

template< typename T >
void KEventDivideDispatcher::QueueingEvent( IN const unsigned short usEventID_, IN const wchar_t* szLogin_, IN const DWORD dwUID_, IN T& data_ )
{
    KIntEventPtr spEvent( new KIntEvent );

    spEvent->m_usEventID    = usEventID_;
    spEvent->m_dwSenderUID  = dwUID_;
    if( szLogin_ != NULL )
        spEvent->m_strSender    = szLogin_;

    KSerializer ks;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    QueueingEvent( spEvent );
}
