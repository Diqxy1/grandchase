#pragma once
#include "Statistics.h"
#include <map>

struct KLoadingInfo;
SmartPointer( KStatLoadingInfo );

class KStatLoadingInfo : public KStatistics
{
public:
    KStatLoadingInfo(void);
    ~KStatLoadingInfo(void);

    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( IN DWORD dwType, IN int nSecond, IN DWORD dwTotalUser, IN DWORD dwMapID, IN int dwFailType, IN int dwFailReason );
    void AddStatSecond( IN DWORD dwType, IN int nSecond ); // Second
    void AddStatMap( IN DWORD dwType, IN DWORD dwMapID ); // MapID
    void AddStatTotalUser( IN DWORD dwType, IN DWORD dwTotalUser ); // TotalUser
    void AddStatFailInfo( IN int nFailType, IN DWORD dwFailReason ); // FailInfo

protected:
    std::map< std::pair< DWORD, int >, int > m_mapTypeSecond;
    std::map< std::pair< DWORD, DWORD>, int > m_mapTypeMapID;
    std::map< std::pair< DWORD, DWORD >, int > m_mapTypeTotalUser;
    std::map< std::pair< int, DWORD >, int > m_mapFailTypeInfo;
};
