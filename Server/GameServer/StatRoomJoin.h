#pragma once
#include "Statistics.h"
#include "GenericLogfile.h"
#include "CommonPacket.h"

SmartPointer( KStatRoomJoin );

class KStatRoomJoin : public KStatistics
{

public:
    KStatRoomJoin(void);
    virtual ~KStatRoomJoin(void);
    void Tick();
    void Dump( std::wostream& stm );
    void Init( int nID, int nFlushGap, bool bWriteToDB, int nRowCount );

    void Flush();
    void AddStat( UINT joinType_ );

private:
    bool m_bFileExist;
    KGenericLogfile m_kRoomJoin;
    std::map< UINT, DWORD > m_mapRoomJoinCount;

};