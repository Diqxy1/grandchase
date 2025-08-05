#pragma once

#include <RTTI.h>
#include <Thread/Locker.h>
//#include "CenterSimLayer.h"
#include "Performer.h"
#include "KncSmartPtr.h"
#include "CenterEvent.h"
#include "CenterPacket.h"

#define NEAR_NUM    2

SmartPointer(KCnSquare);
SmartPointer(KCenter);
class KCnSquare: public KPerformer
{
    NiDeclareRTTI;
    DeclToStringW;
public:
    KCnSquare(void);
    ~KCnSquare(void);

private:
    KSquareInfo::SQUARE_TYPE m_eSquareType;

public:
    virtual void    ProcessEvent( const KEventPtr& ) {};
    virtual void    Tick() {};
    void SetGuildID( IN DWORD dwGuildID_ ) { m_dwGuildID = dwGuildID_; }
    DWORD GetGuildID() const { return m_dwGuildID; }

    void GetSquareInfo( OUT KSquareInfo& kInfo );
    bool SetTRAddress( IN DWORD dwIPAddress, IN unsigned short usPort );
    KSquareInfo::SQUARE_TYPE GetType() { return m_eSquareType; }
    void SetType( IN KSquareInfo::SQUARE_TYPE eType_ ) { m_eSquareType = eType_; }
    bool GetSquareUserInfo( IN DWORD dwUserUID_, OUT KSquareUserInfo& kUserInfo_ );

    void SetMaxUser( DWORD dwMaxNum ) { KLocker lock(m_csSquare); m_dwMaxUserNum = dwMaxNum; }
    DWORD GetMaxUser() const { return m_dwMaxUserNum; }

    bool AddUser( IN const KSquareUserInfo& kUserInfo ); // 광장 입장 할 때 호출
    bool RemoveUser( IN DWORD dwUserUID ); // 광장 나갈 때 호출
    bool IsInSquare( IN const DWORD dwUID_ );
    bool EnterSquareReserve( IN KSquareUserInfo& kUserInfo_, IN KCenterPtr spCenter_, OUT KECN_ENTER_SQUARE_ACK& kPacket );
    void GetSquareUserInfo( OUT std::vector< KSquareUserInfo >& m_vecUserInfo ); // 유저가 입장할 때 광장유저들 정보 모으기
    DWORD GetSize()             { KLocker lock(m_csSquare); return m_mapUserList.size() + m_mapMinigameUserList.size(); }; // 광장유저 수
    bool IsFull() { return (m_dwMaxUserNum <= GetSize() ); }; // 유저 꽉 찼는지

    bool EnterMinigame( IN DWORD dwUserUID_ );
    bool ExitMinigame( IN DWORD dwUserUID_ );

    void UpdateUserPos( IN KSquarePos& kPos_ ); // 이전 위치는 지우고 새로운 위치로 저장 (영역 컨테이너)
    //void GetNeighborhood( IN DWORD dwUserUID, OUT std::vector<DWORD>& vecNeighbor );
    void GetNeighborhood( int nIndex_, std::vector<DWORD>& vecUserUIDs_ );

    void GetNeighborhoodEachServer( IN DWORD dwUserUID, OUT std::map<DWORD,std::vector<DWORD> >& mapNeighborhood ); //  OUT < 서버 UID,  광장에 속한 유저 UID 의 벡터 >
    void GetAllUserEachServer( IN DWORD dwUserUID, OUT std::map< DWORD,std::vector<DWORD> >& mapNeighborhood_ );

    void AppendContainerUser( int nIndex_, std::vector<DWORD>& vecUserUIDs_ );

    template< class T > void SendToAll( const unsigned short usEventID, const T& data );
    template< class T > void QueueingToAll( const unsigned short usEventID, const T& data );

    bool LoadInfo( const char* szTableName ); // 광장내 유저 생성 위치 정보 로드
    bool IsEmpty(); // 광장에 유저가 없을 때 true 

    void InsertContainer( IN int nIndex_, IN DWORD dwUserUID_ );
    bool EraseFromContainer( IN DWORD dwUserUID_ );

    void GetNeighborhoodUserPos( int nIndex_, OUT std::vector<KPosUID>& vecUserPos_ );
    void AppendContainerUserPos( int nIndex_, std::vector<KPosUID>& vecUserPos_ );

    void DumpInfo();
    void SetTRServerIP( IN const std::wstring& strIP );
    std::wstring GetTRServerIP() { return m_strTRServerIP; }
    USHORT GetTRServerPort() { return m_usTRPort; }

protected:

    void GetRelayServerInfo( OUT KECN_ENTER_SQUARE_ACK& kInfo );
    void AddServer( IN KCenterPtr spCenter_ );
    void RemoveServer( IN DWORD dwUID );
    void GetBestPos( KSquarePos& kPos_ );
    int GetIndex( KSquarePos& kPos ) { return (int)(kPos.m_fXpos); }


    //std::map< >      m_mapUsers;
protected:
    mutable KncCriticalSection  m_csSquare;
    
    DWORD                       m_dwMaxUserNum;

    DWORD                       m_dwGuildID;

    DWORD                       m_dwTRServerIP;
    USHORT                      m_usTRPort;
    std::wstring                m_strTRServerIP;

    std::map<DWORD,KSquareUserInfo>                 m_mapUserList; // 유저 UID, 유저 정보
    std::map<DWORD,std::pair<KCenterPtr,int> >      m_mapServers;  // 서버 UID,  <서버포인터, 광장 유저수>
    std::map<DWORD,KSquarePos>                      m_mapUserPosList;
    std::map<DWORD,KSquareUserInfo>                 m_mapMinigameUserList;
    std::map<int, std::vector<DWORD> >              m_mapContainer;

    std::vector< KSquarePos >                       m_vecSpawnPos; // 생성 포지션.
};

template< class T >
void KCnSquare::SendToAll( const unsigned short usEventID_, const T& data_ )
{
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mit;
    for( mit = m_mapServers.begin(); mit != m_mapServers.end() ; ++mit )
    {
        mit->second.first->SendPacket( usEventID_, data_ );
    }
}


template< class T >
void KCnSquare::QueueingToAll( const unsigned short usEventID_, const T& data_ )
{
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mit;
    for( mit = m_mapServers.begin(); mit != m_mapServers.end() ; ++mit )
    {
        mit->second.first->QueueingEvent( usEventID_, data_ );
    }
}