#pragma once
#include ".\statistics.h"
#include "UserPacket.h"

SmartPointer( KStatCharacter );
class KStatCharacter : public KStatistics
{
public:
    KStatCharacter(void);
    ~KStatCharacter(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( IN const std::map<std::pair<int,int>, KCharacterStats>& mapStat );

protected:
    DWORD GetDataSize();

protected:
    // map[ pair[Skillid,count], map[modeType,SkillCount] ]
    std::map<std::pair<int,int>, std::map<int,int> > m_mapStat;
};
