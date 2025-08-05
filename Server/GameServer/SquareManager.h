#pragma once
#include <RTTI.H>
#include <KNCSingleton.h>
#include <ToString.h>
#include <Thread/Locker.h>
#include "KncSmartPtr.h"
#include "UserPacket.h"
#include "Square.h"
#include "KncTimer.h"


SmartPointer(KUser);
class KSquareManager
{
    DeclareSingleton( KSquareManager );
    NiDeclareRootRTTI( KSquareManager );
    DeclToStringW;

public:
    enum TICK_INFO
    {
        TI_BUILD_LIST   = 0,
        TI_TIME_EVENT_LIST,
        
        TI_MAX,
    };

    KSquareManager(void);
    virtual ~KSquareManager(void);

    void Tick();
    void GetSquareList( std::vector<KSquareInfo>& vecSquareList ); // 광장 리스트 얻기
    void GetGuildSquareList( IN DWORD dwGuildUID_, std::vector<KSquareInfo>& vecSquareList_ ); // 길드 광장 리스트 얻기

    void EnterSquareReserve( IN OUT KUserPtr kUser, IN DWORD dwSquareID ); // 유저 광장 입장.
    void LeaveSquare( IN KECN_LEAVE_SQUARE_BROAD kPacket );
    bool AddSquareList( IN std::vector<KSquareInfo>& vecSquares );
    bool AddSquare( IN KSquareInfo& kInfo ); // 광장 추가
    bool RemoveSquare( IN KSquarePtr spSquare );
    void Send_NotMsg( std::wstring& strMsg );
    void Send_NotMsgA( const char* szMessage ); // for lua.
    void ClearEmptySquare();

    KSquarePtr GetSquare( DWORD dwID ); // 광장의 포인터 EVENT_SQUARE_USER_LIST_REQ 에서 사용

    template< class T > void SendToAll( const unsigned short usEventID, const T& data );
    template< class T > void SendTo(const DWORD dwUID, const unsigned short usEventID_, const T& data_);
    template< class T > void QueueingToSquare( DWORD dwSquareID, const unsigned short usEventID, const T& data );

protected:
    void BuildSquareList();
    DWORD GetTick( int nIndex )     { return m_adwTick[ nIndex ]; }
    void SetTick( int nIndex )      { m_adwTick[ nIndex ] = ::GetTickCount(); }
    DWORD GetTickGap( int nIndex )  { return m_adwTickGap[ nIndex ];}
    void SetTickGap( int nIndex, DWORD dwGap )  { m_adwTickGap[ nIndex ] = dwGap;}

protected:
    DWORD                       m_adwTick[TI_MAX];
    DWORD                       m_adwTickGap[TI_MAX];

    mutable KncCriticalSection  m_csSquares;
    std::map<DWORD,KSquarePtr>  m_mapSquares;
    std::vector<KSquarePtr>     m_vecSquares;

    mutable KncCriticalSection  m_csSquareInfo;
    std::vector<KSquareInfo>    m_vecSquareInfo;
    
    std::vector<KSquareInfo>    m_vecGuildSquareInfo;
    KncTimer                    m_kTimer;

};

template< class T >
void KSquareManager::QueueingToSquare( DWORD dwSquareID_, const unsigned short usEventID, const T& data )
{
    std::map<DWORD,KSquarePtr>::iterator mit;
    mit = m_mapSquares.find( dwSquareID_ );
    // 찾는 광장이 존재 하는가?
    _JIF( mit != m_mapSquares.end(), SET_ERROR( ERR_SQUARE_01 ); return );

    mit->second->QueueingToAll( usEventID, data );
}

template< class T >
void KSquareManager::SendToAll( const unsigned short usEventID_, const T& data_ )
{
    std::vector<KSquarePtr>::iterator vit;
    KLocker lock( m_csSquares );
    for( vit = m_vecSquares.begin() ; vit != m_vecSquares.end() ; ++vit )
    {
        (*vit)->SendToAll( usEventID_, data_ );
    }
}

template< class T >
void KSquareManager::SendTo(const DWORD dwUID, const unsigned short usEventID_, const T& data_)
{
    std::vector<KSquarePtr>::iterator vit;
    KLocker lock(m_csSquares);
    for (vit = m_vecSquares.begin(); vit != m_vecSquares.end(); ++vit)
    {
        (*vit)->SendTo(dwUID, usEventID_, data_);
    }
}

DefSingletonInline( KSquareManager );
DeclOstmOperatorA( KSquareManager );
