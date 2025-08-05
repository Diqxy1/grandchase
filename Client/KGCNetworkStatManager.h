#pragma once

#include "KNCSingleton.h"


struct SStatData
{
    int m_nCount;
    bool m_bErrorSend;

    SStatData()
    {
        m_nCount = 0;
        m_bErrorSend = false;
    }
};

class KGCNetworkStatManager
{
    DeclareSingleton( KGCNetworkStatManager );

public:
    enum
    {
        EM_TICK_TR_PACKET_COUNT = 0,    //  TR 패킷 카운트
        EM_TICK_TR_PACKET_SIZE,         //  TR 패킷 사이즈
        EM_TICK_UR_PACKET_COUNT,        //  UR 패킷 카운트
        EM_TICK_UR_PACKET_SIZE,         //  UR 패킷 사이즈
        EM_TICK_STAT_SIZE,
    };  // 분당 카운트 체크

    enum 
    {
        EM_MODE_TR_PACKET_COUNT = 0,    //  모드별 TR 패킷 카운트
        EM_MODE_UR_PACKET_COUNT,    //  모드별 UR 패킷 카운트
        EM_MODE_STAT_SIZE,

    };  // 모드별 카운트 체크

public:
    KGCNetworkStatManager();
    ~KGCNetworkStatManager();

public:
    void FrameMove();
    void WriteLog( int emLogType );

public:
    void AddStatCount( int nType, int nCount = 1 );
    void AddStatModeCount( int nType, int nCount = 1 );

    void ClearStatData( int nType );
    void ClearStatModeDataAll();
    void ClearStatModeData( int nType );

    int GetStatCount( int nType );
    int GetStatModeCount( int nType );

    bool IsValidStat( int nType );
    bool IsValidModeStat( int nType );
    
    void SendClientError( int nType );

    void SetLogFile( bool bLogFile ) { m_bLogFile = bLogFile; }    
    std::string GetEnumString( int nType );


public:
    bool  m_bLogFile;

    DWORD m_dwTickCount[EM_TICK_STAT_SIZE];
    DWORD m_dwTickGaps[EM_TICK_STAT_SIZE];       

    std::map<int,SStatData>   m_mapStatCount;
    std::map<int,SStatData>   m_mapModeStatCount;
};

DefSingletonInline( KGCNetworkStatManager );