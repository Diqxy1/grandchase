#pragma once
#include <Windows.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include "UserPacket.h"

class KLuaManager;

#define HDDDIVIDE 1024

class KSystemResourceManager
{
    DeclareSingleton( KSystemResourceManager );
    NiDeclareRootRTTI( KSystemResourceManager );
    DeclToStringW;

public:
    KSystemResourceManager(void);
    ~KSystemResourceManager(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );

    void Tick();
    bool SystemResourceCheckEnable(){ KLocker lock( m_csSystemResource ); return m_bEnable; }

protected:
    bool LoadHDDInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<std::wstring>& vecHDDPartitionInfo );
    bool ChkOneMinTick();
    void GetSystemMemoryInfo( OUT KProcessMemoryInfo& kProcessMemoryInfo );
    bool GetSystemHDDInfo( IN std::vector<std::wstring>& vecHDDPartitionInfo, OUT std::vector<KSystemHDDInfo>& vecSystemHDDInfo );
    ULONGLONG SubtractTimes( IN const FILETIME& ftA, IN const FILETIME& ftB );
    void GetProcessCPUUsage( OUT int& nCpuUsage );
    void GetHDDPartitionInfo( OUT std::vector<std::wstring>& vecHDDPartitionInfo );
    void GetServerGroupNumInfo( OUT DWORD& dwServerGroup, OUT DWORD& dwServerUniqueNum );
    void GetNetworkUseInfo( OUT std::map<std::wstring, __int64>& mapNetworkInfo );

protected:
    mutable KncCriticalSection                  m_csSystemResource;
    bool                                        m_bEnable; // 기능 사용여부 on/off
    DWORD                                       m_dwLastTick;
    DWORD                                       m_dwTickGap;
    std::vector<std::wstring>                   m_vecHDDPartitionInfo; // HDD 파티션 정보.
    DWORD                                       m_dwServerGroup;
    DWORD                                       m_dwServerUniqueNum;
};

DefSingletonInline( KSystemResourceManager );
DeclOstmOperatorA( KSystemResourceManager );