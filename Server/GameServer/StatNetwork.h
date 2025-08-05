#pragma once
#include "Statistics.h"
#include "GenericLogfile.h"
#include <map>

SmartPointer( KStatNetwork );
class KStatNetwork : public KStatistics
{
public:
    KStatNetwork(void);
    ~KStatNetwork(void);

    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( IN const int& nType, IN const int& nCount );

protected:
    std::map<int,int>               m_mapNetworkStat;
};
