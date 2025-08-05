#pragma once

#include "Statistics.h"
#include "UserPacket.h"
#include <map>

SmartPointer( KStatPvPPlayCharacter );

class KStatPvPPlayCharacter : public KStatistics
{
public:
    KStatPvPPlayCharacter(void);
    virtual ~KStatPvPPlayCharacter(void);
    virtual void Tick();
    virtual void Dump( std::wostream& stm );
    void Flush();
    void AddStat( IN const time_t tmStartTime_, IN const int nModeID_, IN const std::vector< int >& vecCharTypes_ );

protected:
    DWORD GetDataSize();

    KEVENT_STAT_PVP_PLAY_CHARACTER_COUNT_NOT m_vecPvPPlayCharacterCountData; //  대전 플레이 캐릭터 통계 데이터
};
