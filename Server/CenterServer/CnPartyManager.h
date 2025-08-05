#pragma once
#include <RTTI.h>
#include <ToString.h>
#include <KNCSingleton.h>
#include "KncSmartPtr.h"
#include "CenterPacket.h"
#include "CnParty.h"
#include "IntEvent.h"

SmartPointer(KCnParty);

class KCnPartyManager
{
    DeclareSingleton( KCnPartyManager );
    NiDeclareRootRTTI( KCnPartyManager );
    DeclToStringW;

public:
    KCnPartyManager(void);
    ~KCnPartyManager(void);

    bool CreateParty( IN KPartyData& prData, IN KCenterPtr spCenter_, IN KCenterPtr spHostCenter_ );
    KCnPartyPtr GetParty( DWORD dwPartyUID_ );
    bool BreakParty( IN KPartyData& kData_ );
    bool JoinParty( IN OUT KPartyData& kData_, IN KCenterPtr spCenter_ );
    bool CreatePartyAnswer( IN OUT KPartyData& kData_, IN KCenterPtr spCenter_, bool bBroadcast_ );
    bool SetNextHost( IN KPartyData& kData_ );
    bool LeaveParty( IN KPartyData& kData_, IN KCenterPtr spCenter_ );
    template< class T > bool SendToAll( const unsigned short usEventID, const T& data, DWORD partyUID );
    void PrintParty( DWORD dwPartyUID_ );
    void PrintAll();
    bool BanPartyUser( IN KPartyData& kData_ );
    bool ChangeUserState( IN KPartyData& kData, IN KCenterPtr spCenter );
    //void GetCount();

private:
    mutable KncCriticalSection m_csParty;
    std::map< DWORD, KCnPartyPtr > m_mapParty;
    DWORD m_dwUID;
};

DefSingletonInline( KCnPartyManager );
DeclOstmOperatorA( KCnPartyManager );

template< class T >
bool KCnPartyManager::SendToAll( const unsigned short usEventID, const T& data, DWORD dwUID_ )
{
    KCnPartyPtr spParty = GetParty( dwUID_ );
    if ( spParty ) {
        spParty->SendToAll( usEventID, data );
        return true;
    }

    START_LOG( cerr, L"파티 SendToAll 실패, 없는 Party UID : " << dwUID_ ) << END_LOG;

    return false;
}