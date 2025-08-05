#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "CommonPacket.h"

class KLuaManager;
class KCnShutdownUser
{
    DeclareSingleton( KCnShutdownUser );
    NiDeclareRootRTTI( KCnShutdownUser );
    DeclToStringW;

public:
    KCnShutdownUser(void);
    ~KCnShutdownUser(void);

    bool LoadScript();
    void GetConfig( OUT KShutdownUserConfig& kConfig ) { KLocker lock(m_csConfig); kConfig = m_kConfig; }
    bool CheckAge( IN const int& nAge, IN const DWORD& dwIP );

protected:
    mutable KncCriticalSection      m_csConfig;
    KShutdownUserConfig             m_kConfig;
};
DefSingletonInline( KCnShutdownUser );
DeclOstmOperatorA( KCnShutdownUser );