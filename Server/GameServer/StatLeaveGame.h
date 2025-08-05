#pragma once
#include "Statistics.h"
#include <map>

struct KStatKeyLeaveGame;
SmartPointer( KStatLeaveGame );
class KStatLeaveGame : public KStatistics
{
public:
    KStatLeaveGame(void);
    ~KStatLeaveGame(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( int nGameMode, int nDifficulty, int nPlayTime, char cLeftLife, char cCharType, unsigned char ucLv );
    
protected:
    DWORD GetDataSize();
    std::map< KStatKeyLeaveGame, DWORD >   m_mapStatData;
};
