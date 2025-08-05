#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <list>
#include "MatchGroup.h"
#include "IntEvent.h"

#define LIMIT_MATCH_GROUP 15

struct KMatchInfo
{
    int                               m_nStartSearchScope; // 최초 Rating 비율값.
    int                               m_nMaxIncreaseScope; // 최대 Rating 올라가는 수치.
    int                               m_nIncreaseScope; // 증가하는 Rating 수치.
    std::map<int,std::pair<int,bool>> m_mapLimitMatchTable;

    KMatchInfo(): m_nStartSearchScope(0),
        m_nMaxIncreaseScope(0),
        m_nIncreaseScope(0)
    {
        m_mapLimitMatchTable.clear();
    }
};

class KLuaManager;
class KMatchManager
{
    DeclareSingleton( KMatchManager );
    NiDeclareRootRTTI( KMatchManager );
    DeclToStringW;
public:
    KMatchManager(void);
    ~KMatchManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    void Tick();
    KMatchGroupPtr GetMatchGroup( IN DWORD& dwServerUID, IN int& nModeInfo );
    void DistributeEvent();
    void EachTick();
    void AddMatchInfo( IN KPartyData& kPartyData );
    void PrintMatchModeInfo( std::wostream& stm ) const;
    bool LoadLimitMatchTable( IN KLuaManager& kLuaMng, OUT std::map<int,std::pair<int,bool>>& mapLimitMatchTable );

    template<typename T>
    void QueueingEvent( unsigned short usEventID, int nModeType, DWORD dwUID, T& data );
    void QueueingEvent( KIntEventPtr spEvent ) { KLocker lock( m_csEventQueue ); m_queEvent.push_back( spEvent ); }

    template<typename T>
    void QueueingEventAll( unsigned short usEventID, DWORD dwUID, T& data );

    int GetStartSearchScope();
private:
    mutable KncCriticalSection          m_csMatchInfo;
    KMatchInfo                          m_kMatchInfo;
    std::map<std::pair<DWORD,int>, KMatchGroupPtr >    m_mapMatchGroupManager; // < [ServerUID, ModeInfo], MatchGroupPtr >

    mutable KncCriticalSection          m_csEventQueue;
    std::deque<KIntEventPtr>            m_queEvent;
};

DefSingletonInline( KMatchManager );
DeclOstmOperatorA( KMatchManager );

template< typename T >
void KMatchManager::QueueingEvent( unsigned short usEventID_, int nModeType_, DWORD dwUID_, T& data_ )
{
    KIntEventPtr spEvent( new KIntEvent );

    spEvent->m_usEventID    = usEventID_;
    spEvent->m_dwSenderUID  = dwUID_;
    spEvent->m_nModeType    = nModeType_;

    KSerializer ks;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    QueueingEvent( spEvent );
}

template< typename T >
void KMatchManager::QueueingEventAll( unsigned short usEventID_, DWORD dwUID_, T& data_ )
{
    KIntEventPtr spEvent( new KIntEvent );

    spEvent->m_usEventID    = usEventID_;
    KSerializer ks;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    std::map<std::pair<DWORD,int>, KMatchGroupPtr >::iterator mit;

    KLocker lock( m_csMatchInfo );
    for( mit = m_mapMatchGroupManager.begin(); mit != m_mapMatchGroupManager.end(); ++mit ) {
        if( mit->first.first == dwUID_ ) { // 끊긴 게임서버의 객체에만 큐잉.
            spEvent->m_dwSenderUID = mit->first.first;
            spEvent->m_nModeType = mit->first.second;
            QueueingEvent( spEvent );
        }
    }
}