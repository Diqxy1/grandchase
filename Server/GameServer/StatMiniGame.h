#pragma once
#include ".\statistics.h"
#include <map>
SmartPointer( KStatMiniGame );

struct KStatKeyMiniGame; //전방 선언
class KStatMiniGame : public KStatistics
{
public:
    KStatMiniGame(void);
    virtual ~KStatMiniGame(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );

    void Flush();
    void AddStat( std::map<KStatKeyMiniGame, std::pair<DWORD,DWORD> >& mapData );
    void AddStat( KStatKeyMiniGame kKey, DWORD dwElapsedMinute );

protected:
    DWORD GetDataSize();
    std::map<KStatKeyMiniGame, std::pair<DWORD,DWORD> >    m_mapMiniGameData;

};

