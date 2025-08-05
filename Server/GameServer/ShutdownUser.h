#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "CommonPacket.h"
#include "KncTimer.h"

class KShutdownUser
{
    DeclareSingleton( KShutdownUser );
    NiDeclareRootRTTI( KShutdownUser );
    DeclToStringW;

public:
    enum CHECK_TYPE {
        CT_CENTER_LOGIN = 0, // 센터서버 로그인
        CT_GAME_LOGIN   = 1, // 게임서버 로그인
        CT_GAME_TICK    = 2, // 게임서버 Tick
    };

    KShutdownUser(void);
    ~KShutdownUser(void);

    void Tick();

    void SetConfig( IN const KShutdownUserConfig& kConfig ) { KLocker lock(m_csConfig); m_kConfig = kConfig; }
    void GetConfig( OUT KShutdownUserConfig& kConfig ) { KLocker lock(m_csConfig); kConfig = m_kConfig; }
    bool CheckAge( IN const int& nAge, IN const DWORD& dwIP, IN int nCheckType );

protected:
    mutable KncCriticalSection      m_csConfig;
    KShutdownUserConfig             m_kConfig;

    KncTimer                        m_kAlarmMsgTimer;
};
DefSingletonInline( KShutdownUser );
DeclOstmOperatorA( KShutdownUser );