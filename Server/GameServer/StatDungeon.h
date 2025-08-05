#pragma once
#include ".\statistics.h"
#include "UserPacket.h"

SmartPointer( KStatDungeon );
class KStatDungeon : public KStatistics
{
public:
    KStatDungeon(void);
    ~KStatDungeon(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( IN const KDungeonStat& kData );
    void AddBossStat( IN const KBossDungeonStat& kData );

protected:
    DWORD GetDataSize();

protected:
    std::map<KDungeonStat,int>    m_mapDungeonStat;
    std::vector<KBossDungeonStat> m_vecBossDungeon;
};
