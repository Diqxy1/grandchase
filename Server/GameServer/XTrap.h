#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include "xtrap/XTrap4Server.h"  // 09.04.20. kkurrung. X-Trap

class KXTrap
{
    DeclareSingleton( KXTrap );
    NiDeclareRootRTTI( KXTrap );
    DeclToStringW;
public:
    enum{
        MAP_FILE_MAX_NUM = 10,
    };
    KXTrap(void);
    ~KXTrap(void);

    bool Init();
    void SetConfig( IN const DWORD dwTickGap, IN const DWORD dwTimeOut, IN const char* szFileName, bool bUseThemida );

    bool LoadMapFile( bool bInit = false);
    DWORD GetClientTimeOut();
    DWORD GetMaxMapCount() { return MAP_FILE_MAX_NUM; };
    const void* GetMapBuff();
    std::string GetFileName(){ KLocker lock(m_csXTrap); return m_strFileName; }

    void Tick();
    void LoadNow();

protected:
    bool UpdateMap( const UCHAR* pucBuff );
    int GetCurrentIndex()   { return m_nCurrentIndex; };
    int GetPrevIndex();
    int GetNextIndex();
    void SetNextIndex();
    DWORD GetTickGap() { KLocker lock(m_csXTrap); return m_dwTickGap; }
    bool IsForceLoad() { KLocker lock(m_csXTrap); return m_bForce; }

protected:
    mutable KncCriticalSection  m_csXTrap;

    std::string     m_strFileName;
    DWORD           m_dwClientTimeOut;
    int             m_nCurrentIndex;
    UCHAR           m_ucaXTrapMap[MAP_FILE_MAX_NUM][XTRAP_CS4_BUFSIZE_MAP];

    DWORD           m_dwLastTick;
    DWORD           m_dwTickGap;
    DWORD           m_dwLoadCount;
    time_t          m_tmLastLoad;
    bool            m_bForce;
    bool            m_bUseThemida;
};

DefSingletonInline( KXTrap );
DeclOstmOperatorA( KXTrap );