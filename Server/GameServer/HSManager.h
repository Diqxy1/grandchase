#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "hshield/AntiCpXSvr.h"

class KSerBuffer;
class KHSManager
{
    DeclareSingleton( KHSManager );
    NiDeclareRootRTTI( KHSManager );
    DeclToStringW;

public:
    KHSManager(void);
    ~KHSManager(void);

    bool Init();
    void SetConfig( IN const DWORD dwTickGap, IN const char* szFileName );
    void ShutDown();
    AHNHS_CLIENT_HANDLE CreateClientHandle();
    bool MakeRequest( IN AHNHS_CLIENT_HANDLE hClient, OUT KSerBuffer& buff, IN const std::wstring& strLogin );
    bool VerifyResponse( IN AHNHS_CLIENT_HANDLE hClient, IN KSerBuffer& buff, IN const std::wstring& strLogin );
    void CloseClientHandle( OUT AHNHS_CLIENT_HANDLE& hClient );
    void Tick();
    std::string GetFileName() { KLocker lock(m_csHSield); return m_strFileName; }
    void LoadNow() { KLocker lock(m_csHSield); m_bForce = true; }

protected:
    DWORD GetTickGap() { KLocker lock(m_csHSield); return m_dwTickGap; }
    bool IsForceLoad() { KLocker lock(m_csHSield); return m_bForce; }

protected:
    AHNHS_SERVER_HANDLE     m_hServer;

    mutable KncCriticalSection  m_csHSield;
    DWORD                       m_dwLastTick;
    DWORD                       m_dwTickGap;
    std::string                 m_strFileName;
    bool                        m_bForce;

};

DefSingletonInline( KHSManager );
DeclOstmOperatorA( KHSManager );