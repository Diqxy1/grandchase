#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Kairo.h"
#include "CommonPacket.h"
#include <unordered_set>

class KBlockedAttackIP
{
    DeclareSingleton( KBlockedAttackIP );
    NiDeclareRootRTTI( KBlockedAttackIP );
    DeclToStringW;
public:
    KBlockedAttackIP(void);
    ~KBlockedAttackIP(void);

    bool LoadScript();
    void InsertBanIP( unsigned int ipaddr );
    void InsertIPToFile( unsigned int ipaddr );
    bool IsIPBanned( unsigned int ipaddr ) { return m_IPBanSet.find( ipaddr ) != m_IPBanSet.end(); };
protected:
    std::unordered_set<unsigned int>        m_IPBanSet;
    mutable KncCriticalSection              m_csKBlockedAttackIP;
};

DefSingletonInline( KBlockedAttackIP );
DeclOstmOperatorA( KBlockedAttackIP );
