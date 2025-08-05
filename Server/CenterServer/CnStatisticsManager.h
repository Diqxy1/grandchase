#pragma once
#include "KncSmartPtr.h"
#include <map>
#include <Thread/Locker.h>
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

class KSerBuffer;               // 전방 선언
SmartPointer( KStatistics );    // 전방 선언

// DB 혹은 파일에 정기적으로 남기는 통계를 관리한다.
#define SI_MAX_STRING 30
class KCnStatisticsManager
{
    DeclareSingleton( KCnStatisticsManager );
    NiDeclareRootRTTI( KCnStatisticsManager );
    DeclToStringW;
public:

#undef  SI_ENUM
#define SI_ENUM( id, comment ) id,

    enum STATISTICS_IDENTITY
    {
#include "CnStatisticsID_def.h"
    };

public:
    KCnStatisticsManager(void);
    ~KCnStatisticsManager(void);

    void AddStatistics( int nID, int nFlushGap, bool bWriteToDB, int nRowCount ); // config_server.lua 에서 호출
    void ModifyStatistics( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );

    void Tick();

    const wchar_t* GetSiName( int nID );
    KStatisticsPtr FindStatistics( int nID );
    void AddStatDisconn( int DisconnReason );

protected:
    KStatisticsPtr CreateStatObject( int nID );

protected:
    static const wchar_t                ms_szSITitle[][SI_MAX_STRING];
    static const wchar_t                ms_szSIDesc[][SI_MAX_STRING];
    mutable KncCriticalSection          m_csStatistics;
    std::map< int, KStatisticsPtr >     m_mapStatistics;
};

DefSingletonInline( KCnStatisticsManager );
DeclOstmOperatorA( KCnStatisticsManager );
#define GET_STAT( cls, id ) std::static_pointer_cast<cls>(SiKCnStatisticsManager()->FindStatistics( KCnStatisticsManager::##id ))