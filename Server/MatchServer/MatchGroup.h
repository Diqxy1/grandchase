#pragma once

#include <RTTI.h>
#include "KncSmartPtr.h"
#include "Performer.h"
#include "CommonPacket.h"
#include "MatchEvent.h"
#include <list>
#include "MatchSimLayer.h"
#include "KncTimer.h"

#define MAX_PARTY_MEMBER_COUNT 3
#define MAX_SAME_GET_COUNT 10

SmartPointer( KMatchGroup );
SmartPointer( KMatchUser );
SmartPointerStruct( KPartyData );

struct KMatchPartyData
{
    KPartyData          m_PartyData;
    int                 m_SearchScope;
    int                 m_nLoopCount;
};

class KMatchGroup : public KPerformer
{
    NiDeclareRTTI;
    DeclToStringW;

public:
    KMatchGroup(void);
    ~KMatchGroup(void);
    virtual void Tick();

    template< class T > 
    void SendToGameServer( const DWORD dwServerUID, const unsigned short usEventID, const T& data ); // 게임서버랑 통신 template
    bool AddMatch( IN const KEVENT_MATCH_ADD_USER_REQ& kPartyData );
    void DelMatch( IN const KPartyData& kPartyData );
    bool UpdateMatch( OUT std::vector<KPartyData>& vecPartyData, OUT int& nLoopCount );
    void SetServerInfo( IN const DWORD& dwServerUID, IN const int& nModeInfo );
    void SetMatchCheckInfo( IN const int& nStartSearchScope, IN const int& nMaxIncreaseScope, IN const int& nIncreaseScope, IN const bool& bLimitMatchEnable, IN const int& nLimitMatchUserNum );
    int GetListCount();
    void GetLimitInfo( OUT int& nLimitMatchGroupNum, OUT bool& bLimitMatchEnable );

private:
    int GetNeedMemberCount( IN const int& nModeInfo, IN const int& nCurrentMemberCnt );
    bool FindMatch( IN const KPartyData& kPartyData, OUT std::vector<KPartyData>& vecPartyData, OUT int& nLoopCount );
    KMatchPartyData* GetMatchPartyData( IN const KPartyData& kPartyData );
    bool FindMatch( IN const KMatchPartyData& kMatchPartyData, IN int nPartyMemeberCnt, IN int nNeedMemeberCnt, OUT std::vector<KPartyData>& vecPartyData );
    void UpdateSearchScope( IN OUT KMatchPartyData& kMatchPartyData );
    void IntervalTick();
    void IncreaseListCount();
    void DecreaseListCount();
    void InitMatchData();
    bool CheckUpdateTick();

private:
    virtual void ProcessEvent( const KEventPtr& spEvent );

protected:

private:
    mutable KncCriticalSection          m_csMatchUserInfo;
    std::list<KPartyDataPtr>            m_listMatchUserInfo[MAX_PARTY_MEMBER_COUNT];
    typedef std::map< DWORD, KMatchPartyData >   MAP_PARTYINFO; // [MatchpartyUID, MatchPartyData]
    typedef std::map< int, MAP_PARTYINFO >      MAP_RPPARTYINFO; // [RP, [MatchpartyUID, MatchPartyData]]

    // 반드시 Lock 내부에서만 사용할 것
    MAP_RPPARTYINFO m_mapMatchPartyData[MAX_PARTY_MEMBER_COUNT];

    bool m_bFindMatchParty[MAX_PARTY_MEMBER_COUNT];
    int m_nSearchSize[MAX_PARTY_MEMBER_COUNT];
    MAP_RPPARTYINFO::iterator m_mitLower[MAX_PARTY_MEMBER_COUNT];
    MAP_RPPARTYINFO::iterator m_mitUpper[MAX_PARTY_MEMBER_COUNT];

    mutable KncCriticalSection          m_csServer;
    DWORD                               m_dwServerUID;
    int                                 m_nModeInfo;

    int                                 m_nStartSearchScope; // 최초 Rating 비율값.
    int                                 m_nMaxIncreaseScope; // 최대 Rating 올라가는 수치.
    int                                 m_nIncreaseScope; // 증가하는 Rating 수치.
    bool                                m_bLimitMatchEnable; // 최소 수치 
    int                                 m_nLimitMatchGroupNum; // 최소 인원수.

    DWORD                               m_dwTotalListCount; // MatchGroup List Count.
    DWORD                               m_dwTickGap;
    DWORD                               m_dwLastTick;

private:
    DECL_ON_FUNC( EVENT_MATCH_ADD_USER_REQ );
   _DECL_ON_FUNC( EVENT_MATCH_DEL_USER_REQ, KPartyData );
   _DECL_ON_FUNC( EVENT_MATCH_DISCONNECT_GAMESERVER_NOT, DWORD );
};

template< class T >
void KMatchGroup::SendToGameServer( const DWORD dwServerUID_, const unsigned short usEventID_, const T& data_ )
{
    KMatchUserPtr spMatch = SiKMatchSimLayer()->m_kActorManager.GetByUID<KMatchUser>( dwServerUID_ );
    if ( spMatch != NULL ) {
        spMatch->SendPacket( usEventID_, data_ );
    }
}