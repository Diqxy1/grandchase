#pragma once
#include "Statistics.h"
#include <map>
#include <vector>

SmartPointer( KStatItemGet );
struct KStatKeyItem;
//struct KItemInfo;
struct KDurationItemInfo;
struct KItem;

class KStatItemGet : public KStatistics
{
public:
    KStatItemGet(void);
    ~KStatItemGet(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );

    void Flush();
    void AddStat( int nGameMode, std::vector<KItem>& vecItem );

protected:
    DWORD GetDataSize();
    std::map<KStatKeyItem,DWORD>        m_mapItemGetData;
};
