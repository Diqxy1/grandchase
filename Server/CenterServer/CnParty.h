#pragma once
#include <RTTI.h>
#include "KncSmartPtr.h"
#include "Performer.h"
#include "CenterEvent.h"
#include "KncTimer.h"

SmartPointer(KCnParty);
SmartPointer(KCenter); // 전방 선언

#define MINIMUM_PARTY_MEMBER 2
#define MAXIMUM_PARTY_MEMBER 6

class KCnParty : public KPerformer
{
    NiDeclareRTTI;
    DeclToStringW;
public:
    KCnParty(void);
    ~KCnParty(void);

private:
    
    mutable KncCriticalSection                  m_csPartyInfo;
    KPartyData m_kPartyInfo;
    mutable KncCriticalSection                  m_csServers;
    std::map<DWORD,std::pair<KCenterPtr,int> >  m_mapServers;       // 서버 UID,  <서버포인터, 파티 유저수>
public:
    virtual void Tick();
    void SetHost( IN KPartyMember& kMaster_, IN KCenterPtr spCenter_ );
    void AddServer( IN KCenterPtr spCenter_ );
    void RemoveServer( IN const DWORD dwUID_ );
    //void AddMember( IN DWORD dwUserUID );
    //void AddMember( IN DWORD dwUserUID_, IN KCenterPtr spCenter_ );
    void AddMember( IN std::vector<KPartyMember>& vecUser_ );
    void AddMember( IN KPartyMember& kUser_, IN KCenterPtr spCenter_ );
    bool SetNextHost( IN DWORD dwUID_ );
    bool SetNextHost();
    template< class T > void SendToAll( const unsigned short usEventID, const T& data );
    template< class T > void QueueingToAll( const unsigned short usEventID, const T& data );
    
    bool DeleteUser( IN KPartyMember& kData_ );
    size_t GetCount() { KLocker lock( m_csPartyInfo ); return m_kPartyInfo.m_vecPartyPeople.size(); };
    bool IsFull() { return ( MAXIMUM_PARTY_MEMBER <= GetCount() ); }; // 유저 꽉 찼는지

    void PrintPartyUserList();
    void SetMemberNickName( IN KPartyMember& kData_ );
    void SetMemberServerUID( IN KPartyMember& kData_ );

    void SetPartyUID() { KLocker lock( m_csPartyInfo ); m_kPartyInfo.m_dwPartyUID = GetUID(); };
    void GetPartyInfo( OUT KPartyData& kData_ ) { KLocker lock( m_csPartyInfo ); kData_ = m_kPartyInfo; };
    bool IsExistMember( IN KPartyMember& kData_ );
    bool ChangeUserState( IN KPartyMember& kData );
    void SetPartyType( IN const DWORD& dwPartyType );

private:
    virtual void ProcessEvent( const KEventPtr& spEvent );


};

template< class T >
void KCnParty::SendToAll( const unsigned short usEventID_, const T& data_ )
{
    std::vector<DWORD> vecUID;
    std::vector<DWORD>::iterator vit;
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mit;

    KLocker lock( m_csServers );

    for( mit = m_mapServers.begin(); mit != m_mapServers.end() ; ++mit )
    {
        if ( !mit->second.first ) continue;

        if ( false == mit->second.first->SendPacket( usEventID_, data_ ) ) {
            vecUID.push_back( mit->first );
        }
    }

    for ( vit = vecUID.begin() ; vit != vecUID.end() ; ++vit ) {
        m_mapServers.erase( *vit );
    }
}


template< class T >
void KCnParty::QueueingToAll( const unsigned short usEventID_, const T& data_ )
{
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mit;
    KLocker lock( m_csServers );
    for( mit = m_mapServers.begin(); mit != m_mapServers.end() ; ++mit )
    {
        if ( !mit->second.first ) continue;

        mit->second.first->QueueingEvent( usEventID_, data_ );
    }
}
