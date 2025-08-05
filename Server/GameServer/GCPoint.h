#pragma once

#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <ToString.h>
#include <RTTI.H>
#include <map>
#include <algorithm>
#include "PrimitiveTypedef.h"

struct KTimeKey
{
    time_t  m_tmStart;
    time_t  m_tmEnd;

    bool operator < ( const KTimeKey& right ) const
    {
        if ( m_tmStart > right.m_tmStart )
            return false;
        else if ( m_tmStart < right.m_tmStart )
            return true;

        if ( m_tmEnd > right.m_tmEnd )
            return false;
        else if ( m_tmEnd < right.m_tmEnd )
            return true;

        return false;
    }
};

class KLuaManager;
class KGCPoint
{
    DeclareSingleton( KGCPoint );
    NiDeclareRootRTTI( KGCPoint );
    DeclToStringW;
public:
    KGCPoint(void);
    ~KGCPoint(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool LoadConfig( IN KLuaManager& kLuaMng );
    bool LoadSetting( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT int& nDefault, OUT std::map<KTimeKey,int>& mapEvent );
    bool LoadWeekSetting( IN KLuaManager& kLuaMng, OUT int& nDefault, OUT std::map<PAIR_INT,int>& mapEvent );
    bool LoadTime( IN KLuaManager& kLuaMng, IN const int nIndex, OUT time_t& tmTime );

    bool IsGCPointSwitch()      { KLocker lock(m_csConfig); return m_bGCPointSwitch; }
    DWORD GetAccUpdateTime()    { KLocker lock(m_csConfig); return m_dwAccUpdateTime; }
    int GetAttendTime()         { KLocker lock(m_csConfig); return m_nAttendTime; }
    int GetRecommendGCPoint()   { KLocker lock(m_csConfig); return m_nGCPointRecommend; }

    int GetGCPointDay( IN const time_t& tmDate );
    int GetGCPointWeek( IN const int& nMonth, IN const int& nWeekID );
    int GetGCPointOpenCalendar( IN const time_t& tmDate );
    int GetCurrentGCPoint( IN const int& nDefault, IN const std::map<KTimeKey,int>& mapEvent, IN const time_t& tmCurrent );

    void DumpInfo();
    void DumpEventSetting( IN const std::map<KTimeKey,int>& mapEvent );

private:
    mutable KncCriticalSection  m_csConfig;
    bool                        m_bGCPointSwitch;       // GCPoint 시스템 사용여부(미적용 국가용).
    DWORD                       m_dwAccUpdateTime;      // 출석 누적시간 갱신주기(ms)
    int                         m_nAttendTime;          // 출석 인정 시간(분)
    int                         m_nGCPointRecommend;    // 추천 받았을때 GP 포인트 지급 (미사용)

    mutable KncCriticalSection  m_csDaySetting;
    int                         m_nGCPointDay;          // 1일 출석시 지급되는 GC 포인트
    std::map<KTimeKey,int>      m_mapEventGCPointDay;   // 특정기간 일일지급 GC포인트 map[date,point]

    mutable KncCriticalSection  m_csWeekSetting;
    int                         m_nGCPointWeek;         // 주개근시 지급되는 GC 포인트
    std::map<PAIR_INT,int>      m_mapEventGCPointWeek;  // 특정기간 주개근 GC포인트 map[pair[Month,weekID],Point]

    mutable KncCriticalSection  m_csOpenCalendarSetting;
    int                         m_nGCPointOpenCalendar; // 달력 오픈시, 일일1회 보너스 GC포인트
    std::map<KTimeKey,int>      m_mapEventGCPointOpenCalendar; // 특정기간 출석부오픈시 일일지급 GC포인트 map[date,point]
};
DefSingletonInline( KGCPoint );
DeclOstmOperatorA( KGCPoint );

class KUserGCPoint
{
public:
    KUserGCPoint(void):m_nInitGCPoint(0),m_nGCPoint(0){}
    ~KUserGCPoint(void){}

    int GetInitGCPoint()                        { KLocker lock(m_csGCPoint); return m_nInitGCPoint; }
    void SetInitGCPoint( IN const int& nValue ) { KLocker lock(m_csGCPoint); m_nInitGCPoint = std::max<int>( nValue, 0 ); }
    int GetGCPoint()                            { KLocker lock(m_csGCPoint); return m_nGCPoint; }
    void SetGCPoint( IN const int& nValue )     { KLocker lock(m_csGCPoint); m_nGCPoint = std::max<int>( nValue, 0); }

    void IncreaseGCPoint( IN const int& nType, IN OUT int& nValue )
    {
        KLocker lock(m_csGCPoint);
        if ( nValue <= 0 ) return;
        m_nGCPoint += nValue;
    }

    void DecreaseGCPoint( IN const int& nType, IN const int& nValue )
    {
        KLocker lock(m_csGCPoint);
        m_nGCPoint -= nValue;
        m_nGCPoint = std::max<int>( m_nGCPoint, 0 );
    }

private:
    mutable KncCriticalSection  m_csGCPoint;
    int                         m_nInitGCPoint; // DB에서 받아온 초기값
    int                         m_nGCPoint; // 현재값
};