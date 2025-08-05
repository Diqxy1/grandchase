#pragma once
#include "KncSmartPtr.h"

#define NEAR_NUM    2
// 이녀석은 무언가 처리를 빠르게 하기 위해서 그리드 방식을 활룔해야 할듯...ㅜㅜ..
SmartPointer(KUser);
SmartPointer(KSquare);
class KSquare : public KPerformer
{
    NiDeclareRTTI;
    DeclToStringW;

public:
    KSquare(void);
    ~KSquare(void);

    virtual void    ProcessEvent( const KEventPtr& );
    virtual void    Tick();

    KUserPtr GetUserPtr( const DWORD dwUID ); // m_mapUsers 여기서 유저 찾아서 반환
    
    bool SendRelayTo( const DWORD dwUID_, const unsigned short usEventID_, KSerBuffer& data_ );
    template< class T > bool SendTo( const DWORD dwUID, const unsigned short usEventID, const T& data );
    template< class T > void SendToAll( const unsigned short usEventID, const T& data );
    template< class T > void SendToAllExMe( const DWORD dwUID, const unsigned short usEventID, const T& data );
    bool SendIDTo( const DWORD dwUID, const unsigned short usEventID );
    void SendIDToAll( const unsigned short usEventID );
    void SendIDToAllExMe( const DWORD dwUID, const unsigned short usEventID );
    void SendIDToNeighborhood( const int nIndex, const unsigned short usEventID );
    template< class T > void QueueingEventTo( const DWORD dwUID, const unsigned short usEventID,  const T& data );
    template< class T > void QueueingToAll( const unsigned short usEventID, const T& data );
    template< class T > void QueueingToNeighborhood( const int nIndex, const unsigned short usEventID, const T& data );
    
    
    bool LoadInfo( const char* szTableName ); // 광장내 유저 생성 위치 정보 로드

    void GetInfo( KSquareInfo& kInfo ); // 광장 정보 얻기
    DWORD GetSize()             { KLocker lock(m_csUsers); return m_mapUsers.size(); }; // 광장유저 수
    DWORD GetMiniGameUserSize() { KLocker lock(m_csUsers); return m_mapMiniGameUsers.size(); }; // 미니게임유저 수
    bool IsFull() { return (m_dwMaxUserNum <= GetSize() + GetMiniGameUserSize()); }; // 유저 꽉 찼는지
    void EnterSquareReserve( IN OUT KUserPtr kUser ); // 광장 입장
    bool SetTRAddress( DWORD dwIPAddress, USHORT usPort ); // tcp relay 서버 주소 세팅
    DWORD GetMaxUser(){ return m_dwMaxUserNum; }; // 광장 최대 수용 인원 수 얻기
    void SetMaxUser( DWORD dwMaxNum ) { KLocker lock(m_csUsers); m_dwMaxUserNum = dwMaxNum; }; // 최대 인원 수 설정
    void SetGuildUID( DWORD dwGuildUID_ ) { m_dwGuildUID = dwGuildUID_; }

    bool LeaveSquare( DWORD dwUID ); //  유저를 m_mapUsers 에서 삭제하고 m_mapContainer 에서도 삭제
    
    std::wstring GetAddress(); // tr 주소 
    USHORT GetPortNum(){ return m_usTRPort; };

    bool IsEmpty();
    bool IsInSquare( IN const DWORD dwUID ); // 유저 중복인지 검색
    void Send_NotMsg( std::wstring& strMsg ); // 광장내 말하기? 
    void GetUserList( IN int nPage, OUT KPageInfo& kPageUser ); // 광장 유저 리스트 (초대에서..)
    void BuildUserList();
    bool StartMiniGame( DWORD dwUID ); // 광장의 유저를 미니게임 컨테이너로 이동 
    bool IsInMiniGame( IN const DWORD dwUID );

    bool BackToSquareFromMiniGame( IN KUserPtr kUser_, OUT KEVENT_ENTER_SQUARE_ACK& kPacket ); //  080709  woosh. 미니게임에서 광장으로 돌아가기
    void CompressUserList( IN std::vector<KInviteUserInfo>& vecUserList, OUT std::vector<KListPage>& vecUserListPage );
    void GetAllUserList( IN int nPage, OUT KPageInfo& kPageUser );

    KSquareInfo::SQUARE_TYPE GetType() const { return m_eType; }
    void SetType( IN int val) { m_eType = (KSquareInfo::SQUARE_TYPE)val; }

    int GetGuildSquareIndex() { return m_nGuildSquareIndex; }
    void SetGuildSquareIndex( int nIndex_ ) { m_nGuildSquareIndex = nIndex_; }

    int GetGuildUID() { return m_dwGuildUID; }

protected:
    bool AddUser( IN KUserPtr kUser ); // m_mapUsers에 유저 추가
    void GetSquareUserInfo( OUT std::vector< KSquareUserInfo >& m_vecUserInfo ); // 유저가 입장할 때 광장유저들 정보 모으기
    bool RemoveUser( IN const DWORD dwUID ); // m_mapUsers 에서 유저 삭제
    int GetIndex( KSquarePos& kPos ) { return (int)(kPos.m_fXpos); }
    bool RemoveMiniGameUser( IN const DWORD dwUID_ ); // 미니게임 유저 삭제


protected:
    DWORD                                   m_dwTRServerIP;
    USHORT                                  m_usTRPort;

    DWORD                                   m_dwMaxUserNum;
    KncCriticalSection                      m_csUsers;
    KSquareInfo::SQUARE_TYPE                m_eType;
    std::map<DWORD,KUserPtr>                m_mapUsers;
    std::map<int, std::vector<KUserPtr> >   m_mapContainer; // 영역 컨테이너. x좌표 위치별 유저
    std::vector< KSquarePos >               m_vecSpawnPos; // 생성 포지션.

    std::vector<KListPage>                  m_vecSquareUserList; // 현재는 룸에서 초대 패킷을 보내기 위해 정보 필요..
    std::vector<KListPage>                  m_vecSquareAllUserList; // GM툴용

    KncCriticalSection                      m_csMiniGameUsers;  //  080708  woosh. 미니게임을 위한 cs
    std::map<DWORD, KUserPtr>               m_mapMiniGameUsers; //  080707  woosh. 아직 다 만든거 아님, 게임모드에 대한 정보가 있어야 할 듯

    int                                     m_dwGuildUID; // 길드 광장이라면 이 값을 가진다.
    int                                     m_nGuildSquareIndex; // 길드 광장 순서

};


template< class T >
bool KSquare::SendTo( const DWORD dwUID_, const unsigned short usEventID_, const T& data_ )
{
    KUserPtr spUser =  GetUserPtr( dwUID_ );
    if( !spUser ) return false;
    return spUser->SendPacket( usEventID_, data_ );
}

template< class T >
void KSquare::SendToAll( const unsigned short usEventID_, const T& data_ )
{
    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = usEventID_;

    // make event object
    KSerializer         ks;

    // serialize - only data
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    if( spEvent->m_kbuff.GetLength() > 150 )
        spEvent->m_kbuff.Compress();

    // serialize - full event object
    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();

    std::map<DWORD,KUserPtr>::iterator mit;
    KLocker lock( m_csUsers );
    for( mit = m_mapUsers.begin(); mit != m_mapUsers.end(); ++mit )
    {
        mit->second->SendPacket( kbuff, spEvent->GetEventIDString() );
    }
}

template< class T >
void KSquare::SendToAllExMe( const DWORD dwUID_, const unsigned short usEventID_, const T& data_ )
{
    KUserPtr spUser =  GetUserPtr( dwUID_ );
    if( !spUser ) return;

    KEventPtr spEvent( new KUserEvent );
    spEvent->m_usEventID = usEventID_;

    // make event object
    KSerializer         ks;

    // serialize - only data
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( data_ );
    ks.EndWriting();

    if( spEvent->m_kbuff.GetLength() > 150 )
        spEvent->m_kbuff.Compress();

    // serialize - full event object
    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();


    std::map<DWORD,KUserPtr>::iterator mit;
    KLocker lock( m_csUsers );
    for( mit = m_mapUsers.begin(); mit != m_mapUsers.end(); ++mit )
    {
        if( mit->second->GetUID() == dwUID_ ) continue;
        mit->second->SendPacket( kbuff, spEvent->GetEventIDString() );
    }
}

template< class T >
void KSquare::QueueingEventTo( const DWORD dwUID_, const unsigned short usEventID_,  const T& data_ )
{
    KUserPtr spUser =  GetUserPtr( dwUID_ );
    if( !spUser )
        return;

    spUser->QueueingEvent( usEventID_, data_ );
}

template< class T >
void KSquare::QueueingToAll( const unsigned short usEventID_, const T& data_ )
{
    std::map<DWORD,KUserPtr>::iterator mit;
    KLocker lock( m_csUsers );
    for( mit = m_mapUsers.begin(); mit != m_mapUsers.end(); ++mit )
    {
        mit->second->QueueingEvent( usEventID_, data_ );
    }
}