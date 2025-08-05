#pragma once
#include "KNCSingleton.h"

class KGCLevelResetEventManager
{
    DeclareSingleton( KGCLevelResetEventManager );

public:
    KGCLevelResetEventManager();
    ~KGCLevelResetEventManager();

public:
    void SetLevelResetEventInfo( const KEVENT_LEVEL_RESET_EVENT_INFO_ACK& kPacket );
    bool IsResetItem( char cCharType, GCITEMID itemID );
    bool IsResetItem( GCITEMID itemID );

public:
    bool   m_bInitData;        // 정보 수신 여부
    int    m_nLevel;           // 목표 달성 레벨

    std::map<int,GCITEMID>  m_mapGPResetItemList;
    std::map<int,GCITEMID>  m_mapCashResetItemList;

    std::map<int,VEC_REWARD>  m_mapGPRewardItemList;
    std::map<int,VEC_REWARD>  m_mapCashRewardItemList;

    std::map<char,std::pair<bool,bool>>     m_mapResetCharInfo;     //  리셋한 캐릭터정보 및 보상 여부

};

DefSingletonInline( KGCLevelResetEventManager );