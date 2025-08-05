#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <set>
#include <vector>
#include "PrimitiveTypedef.h"
#include "CommonPacket.h"

class KLuaManager;

class KEclipseCollect
{
    DeclareSingleton( KEclipseCollect );
    NiDeclareRootRTTI( KEclipseCollect );
    DeclToStringW;

public:
    typedef std::vector<KDropItemInfo> VEC_REWARD;

    KEclipseCollect(void);
    ~KEclipseCollect(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun(); // 이벤트 진행중인지 체크.
    void GetEclipseCollectItemList( OUT std::set<GCITEMID>& setEclipseCollectItemList );
    void GetRewardItem( OUT VEC_REWARD& vecRewardItem );
    void GetBeginTime( OUT time_t& tmDate );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    bool LoadEclipseCollectItemList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<DWORD>& setPresentBoxInfo );

protected:
    mutable KncCriticalSection          m_csEclipse;
    time_t                              m_tmBegin;           // 이벤트 시작 날짜.
    time_t                              m_tmEnd;             // 이벤트 종료 날짜.
    DWORD                               m_dwVersion;         // 이벤트 버전 정보.
    std::set<GCITEMID>                  m_setEclipseCollectItemList; // 고서 아이템 리스트.
    VEC_REWARD                          m_vecRewardItem; // 보상 아이템 정보.
};

DefSingletonInline( KEclipseCollect );
DeclOstmOperatorA( KEclipseCollect );