#include "stdafx.h"
#include <pdh.h>
#include "SystemResourceManager.h"
#include <dbg/dbg.hpp>
#include "Lua/KLuaManager.h"
#include "GSDBLayer.h"
#include "UserEvent.h"
#include "Log4.h"
#include "KncUtil.h"

#pragma comment( lib, "iphlpapi" )
#pragma comment(lib, "pdh.lib")

ImplementSingleton( KSystemResourceManager );
ImplOstmOperatorW2A( KSystemResourceManager );
NiImplementRootRTTI( KSystemResourceManager );

KSystemResourceManager::KSystemResourceManager(void)
:m_bEnable(false)
,m_dwTickGap(1000*60)
,m_dwServerGroup(0)
,m_dwServerUniqueNum(0)
{
    m_dwLastTick = ::GetTickCount();
    m_vecHDDPartitionInfo.clear();
}

KSystemResourceManager::~KSystemResourceManager(void)
{
}

ImplToStringW( KSystemResourceManager )
{
    KLocker lock( m_csSystemResource );
    return START_TOSTRINGW
        << TOSTRINGWb( m_bEnable )
        << TOSTRINGW( m_dwTickGap );
}


bool KSystemResourceManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KSystemResourceManager::_LoadScript( OUT std::string& strScript_ )
{
    bool                                        bEnable;
    DWORD                                       dwTickGap;
    std::vector<std::wstring>                   vecHDDPartitionInfo;
    DWORD                                       dwServerGroup;
    DWORD                                       dwServerUniqueNum;

    KLuaManager kLuaMng;
    const std::string strScriptName = "InitSystemResource.lua";
    strScript_ = strScriptName;

    _JIF( kLuaMng.DoFile( strScriptName.c_str() ) == S_OK, return false );

    _JIF( kLuaMng.GetValue( "ServerGroup", dwServerGroup ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ServerUniqueNum", dwServerUniqueNum ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "UseEnable", bEnable ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "TickGap", dwTickGap ) == S_OK, return false );
    _JIF( LoadHDDInfo( kLuaMng, std::string("HDD_DiskDesc"), vecHDDPartitionInfo ), return false );

    START_LOG( cerr, L"시스템 자원 관리 스크립트 로드 완료. " ) << END_LOG;
    {
        KLocker lock( m_csSystemResource );
        m_dwServerGroup = dwServerGroup;
        m_dwServerUniqueNum = dwServerUniqueNum;
        m_bEnable = bEnable;
        m_dwTickGap = dwTickGap;
        m_vecHDDPartitionInfo.swap( vecHDDPartitionInfo );
    }

    return true;
}

bool KSystemResourceManager::LoadHDDInfo( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector<std::wstring>& vecHDDPartitionInfo_ )
{
    vecHDDPartitionInfo_.clear();
    _JIF( kLuaMng_.BeginTable( strTable_.c_str() ) == S_OK, return false );

    for( int i = 1 ; ; ++i ) {
        std::string strTemp;
        std::wstring wstrPartition;
        if( kLuaMng_.GetValue( i, strTemp ) != S_OK ) break;
        wstrPartition = KncUtil::toWideString( strTemp ).c_str();
        vecHDDPartitionInfo_.push_back( wstrPartition );
    }

    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KSystemResourceManager::ChkOneMinTick()
{
    if ( ::GetTickCount() - m_dwLastTick < m_dwTickGap ) {
        return false;
    }

    m_dwLastTick = ::GetTickCount();
    return true;
}

void KSystemResourceManager::GetHDDPartitionInfo( OUT std::vector<std::wstring>& vecHDDPartitionInfo_ )
{
    vecHDDPartitionInfo_.clear();

    KLocker lock( m_csSystemResource );
    vecHDDPartitionInfo_ = m_vecHDDPartitionInfo;
};

void KSystemResourceManager::Tick()
{
    if( !ChkOneMinTick() || !SystemResourceCheckEnable() ) { // 1분마다 / 사용유무 체크.
        return;
    }

    KDB_EVENT_SYSTEM_RESOURCE_INFO kDBPacket;
    std::vector<std::wstring> vecHDDPartitionInfo;
    GetServerGroupNumInfo( kDBPacket.m_dwServerGroup, kDBPacket.m_dwServerUniqueNum );
    GetSystemMemoryInfo( kDBPacket.m_kProcessMemoryInfo );
    GetHDDPartitionInfo( vecHDDPartitionInfo );
    GetSystemHDDInfo( vecHDDPartitionInfo, kDBPacket.m_vecSystemHDDInfo );
    GetProcessCPUUsage( kDBPacket.m_nCPUUsage );
    GetNetworkUseInfo( kDBPacket.m_mapNetworkInfo );

    SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_SYSTEM_RESOURCE_INFO, L"", 0, 0, kDBPacket );
}

void KSystemResourceManager::GetSystemMemoryInfo( OUT KProcessMemoryInfo& kProcessMemoryInfo_ )
{
    MEMORYSTATUSEX statex;
    KProcessMemoryInfo kProcessMemoryInfo;
    memset( &statex, 0, sizeof( MEMORYSTATUSEX ) );
    statex.dwLength = sizeof( MEMORYSTATUSEX );
    GlobalMemoryStatusEx( &statex );

    kProcessMemoryInfo.m_dwMemoryLoad = statex.dwMemoryLoad;
    kProcessMemoryInfo.m_dwTotalPhys = static_cast<DWORD>(statex.ullTotalPhys/HDDDIVIDE);
    kProcessMemoryInfo.m_dwAvailPhys = static_cast<DWORD>(statex.ullAvailPhys/HDDDIVIDE);
    kProcessMemoryInfo.m_dwTotalPageFile = static_cast<DWORD>(statex.ullTotalPageFile/HDDDIVIDE);
    kProcessMemoryInfo.m_dwAvailPageFile = static_cast<DWORD>(statex.ullAvailPageFile/HDDDIVIDE);
    kProcessMemoryInfo.m_dwTotalVirtual = static_cast<DWORD>(statex.ullTotalVirtual/HDDDIVIDE);
    kProcessMemoryInfo.m_dwAvailVirtual = static_cast<DWORD>(statex.ullAvailVirtual/HDDDIVIDE);
    kProcessMemoryInfo.m_dwAvailExtendedVirtual = static_cast<DWORD>(statex.ullAvailExtendedVirtual/HDDDIVIDE);

    kProcessMemoryInfo_ = kProcessMemoryInfo;
}

bool KSystemResourceManager::GetSystemHDDInfo( IN std::vector<std::wstring>& vecHDDPartitionInfo_, OUT std::vector<KSystemHDDInfo>& vecSystemHDDInfo_ )
{
    std::vector<KSystemHDDInfo> vecSystemHDDInfo;
    vecSystemHDDInfo.clear();
    std::vector<std::wstring>::iterator vit;
    for( vit = vecHDDPartitionInfo_.begin(); vit != vecHDDPartitionInfo_.end(); ++vit ) {
        KSystemHDDInfo kSystemHDDInfo;
        ULARGE_INTEGER  _64FreeBytesToCaller, _64TotalBytes, _64FreeBytes;
        BOOL bResult = GetDiskFreeSpaceEx((LPCTSTR)(*vit).c_str(), &_64FreeBytesToCaller, &_64TotalBytes, &_64FreeBytes);
        if( !bResult ) {
            continue;
        }
        kSystemHDDInfo.m_wstrDiskName = *vit;
        //메가바이트 단위로 환산
        kSystemHDDInfo.m_dwDiskFreeSize = (UINT)((_64FreeBytes.QuadPart)/HDDDIVIDE/HDDDIVIDE);
        kSystemHDDInfo.m_dwDiskTotalSize = (UINT)((_64TotalBytes.QuadPart)/HDDDIVIDE/HDDDIVIDE);
        vecSystemHDDInfo.push_back( kSystemHDDInfo );
    }
    vecSystemHDDInfo_.swap( vecSystemHDDInfo );

    return true;
};

ULONGLONG KSystemResourceManager::SubtractTimes( IN const FILETIME& ftA_, IN const FILETIME& ftB_ )
{
    LARGE_INTEGER a, b;
    a.LowPart = ftA_.dwLowDateTime;
    a.HighPart = ftA_.dwHighDateTime;

    b.LowPart = ftB_.dwLowDateTime;
    b.HighPart = ftB_.dwHighDateTime;

    return a.QuadPart - b.QuadPart;
}

void KSystemResourceManager::GetProcessCPUUsage( OUT int& nCpuUsage_ )
{
    // cpu 사용량 구하기
    nCpuUsage_ = 0;
    FILETIME m_ftPrevSysKernel;
    FILETIME m_ftPrevSysUser;

    FILETIME m_ftPrevProcKernel;
    FILETIME m_ftPrevProcUser;

    ZeroMemory(&m_ftPrevSysKernel, sizeof(FILETIME));
    ZeroMemory(&m_ftPrevSysUser, sizeof(FILETIME));

    ZeroMemory(&m_ftPrevProcKernel, sizeof(FILETIME));
    ZeroMemory(&m_ftPrevProcUser, sizeof(FILETIME));

    FILETIME ftSysIdle, ftSysKernel, ftSysUser;
    FILETIME ftProcCreation, ftProcExit, ftProcKernel, ftProcUser;

    if ( !GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser) || 
            !GetProcessTimes(GetCurrentProcess(), &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser) ) {
        START_LOG( cerr, L"CPU 사용량 가져오기 실패" ) << END_LOG;
    }

    ULONGLONG ftSysKernelDiff = SubtractTimes(ftSysKernel, m_ftPrevSysKernel);
    ULONGLONG ftSysUserDiff = SubtractTimes(ftSysUser, m_ftPrevSysUser);
    ULONGLONG ftProcKernelDiff = SubtractTimes(ftProcKernel, m_ftPrevProcKernel);
    ULONGLONG ftProcUserDiff = SubtractTimes(ftProcUser, m_ftPrevProcUser);
    ULONGLONG nTotalSys =  ftSysKernelDiff + ftSysUserDiff;
    ULONGLONG nTotalProc = ftProcKernelDiff + ftProcUserDiff;
    if (nTotalSys > 0) {
        nCpuUsage_ = static_cast<short>(((100.0 * nTotalProc) / nTotalSys));
    }
};

void KSystemResourceManager::GetServerGroupNumInfo( OUT DWORD& dwServerGroup_, OUT DWORD& dwServerUniqueNum_ )
{
    KLocker lock( m_csSystemResource );
    dwServerGroup_ = m_dwServerGroup;
    dwServerUniqueNum_ = m_dwServerUniqueNum;
}

void KSystemResourceManager::GetNetworkUseInfo( OUT std::map<std::wstring, __int64>& mapNetworkInfo_ )
{
    mapNetworkInfo_.clear();

    HQUERY m_hQuery = NULL;
    DWORD pcchCounterListLength = 0, pcchInstanceListLength = 0 ;
    PdhEnumObjectItems( NULL, NULL, _T("Network Interface"), NULL, &pcchCounterListLength, NULL, &pcchInstanceListLength, PERF_DETAIL_WIZARD, 0 );

    LPTSTR lpCounterList = (LPTSTR)calloc( pcchCounterListLength, sizeof(TCHAR) );
    LPTSTR lpInstanceList = (LPTSTR)calloc( pcchInstanceListLength, sizeof(TCHAR) );
    if( lpCounterList && lpInstanceList ) {
        PdhEnumObjectItems( NULL, NULL, _T("Network Interface"), lpCounterList, &pcchCounterListLength, lpInstanceList, &pcchInstanceListLength, PERF_DETAIL_WIZARD, 0 );
    }

    LPTSTR lpstr = lpInstanceList;
    int m_nNET_Count = 0;
    while( *lpstr ) {
        m_nNET_Count++;
        lpstr += 1 + _tcslen(lpstr);
    }

    HCOUNTER* m_pNET_hCounter = (HCOUNTER*)calloc( m_nNET_Count, sizeof(HCOUNTER) );
    LPTSTR* m_pNET_InstanceName = (LPTSTR*)calloc( m_nNET_Count, sizeof(LPTSTR) );

    PDH_COUNTER_PATH_ELEMENTS pcpeTotal = {0};
    pcpeTotal.szObjectName = _T("Network Interface");
    pcpeTotal.szInstanceName = lpInstanceList;
    pcpeTotal.dwInstanceIndex = -1;
    pcpeTotal.szCounterName = _T("Bytes Total/sec");

    PDH_FMT_COUNTERVALUE v;
    __int64 biTemp;
    v.longValue = 0;
    std::map<std::wstring, __int64> mapTemp;
    for( int c = 0 ; c < m_nNET_Count && *pcpeTotal.szInstanceName ; c++ ) {
        TCHAR szFullCounterPath[1024]={0};
        DWORD dwFullPathSize = 1024;
        std::wstring wstrTemp;
        PdhMakeCounterPath( &pcpeTotal, szFullCounterPath, &dwFullPathSize, 0 );
        PdhAddCounter( m_hQuery, szFullCounterPath, 1, &m_pNET_hCounter[c] );
        m_pNET_InstanceName[c] = pcpeTotal.szInstanceName;
        wstrTemp = static_cast<std::wstring>( pcpeTotal.szInstanceName );
        pcpeTotal.szInstanceName += 1 + _tcslen(pcpeTotal.szInstanceName);
        PdhCollectQueryData(m_hQuery);
        PdhGetFormattedCounterValue(m_pNET_hCounter[c], PDH_FMT_LONG, 0, &v);
        biTemp = static_cast<__int64>( v.longValue );
        mapTemp[wstrTemp] = biTemp;
    }

    mapNetworkInfo_.swap( mapTemp );

    if ( lpCounterList ) {
        free( lpCounterList );
    }

    if ( lpInstanceList ) {
        free( lpInstanceList );
    }
};