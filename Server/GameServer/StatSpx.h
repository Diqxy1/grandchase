#pragma once
#include "Statistics.h"
#include <map>

SmartPointer( KStatSpx );
struct KStatKeySPX;
class KStatSpx : public KStatistics
{
public:
    KStatSpx(void);
    ~KStatSpx(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( int nGameMode, char cCharType, int nLv, int nSPLv, int nMaxSPLv );

protected:
    DWORD GetDataSize();
    std::map<KStatKeySPX, DWORD>            m_mapSpxData;
};
