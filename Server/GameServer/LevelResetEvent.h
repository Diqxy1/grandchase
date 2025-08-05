#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

#include <Thread/Locker.h>
#include "UserPacket.h"


class KLuaManager;

class KLevelResetEvent
{
    DeclareSingleton( KLevelResetEvent );
    NiDeclareRootRTTI( KLevelResetEvent );
    DeclToStringW;

public:
    KLevelResetEvent(void);
    ~KLevelResetEvent(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun();
    DWORD GetVersion() { return m_dwVersion; }
    

    int GetRewardLevel() { return m_nLevel; }
    void GetRewardList( OUT std::map<int,VEC_REWARD>& mapGPRewardItemList_, OUT std::map<int,VEC_REWARD>& mapCashRewardItemList_ );   
    void GetResetItemList( OUT std::map<int,GCITEMID>& mapGPResetItemList_, OUT std::map<int,GCITEMID>& mapCashResetItemList_ );

    void GetCharRewardList( IN const char& cChar_, IN const bool& bCashType_, OUT std::vector<KDropItemInfo>& vecRewardItemList_ );

    bool IsResetItemID( IN const char& cCharType, IN const GCITEMID& itemID, OUT bool& bCashType );
    GCITEMID GetResetItemID( IN const char& cCharType, IN const bool& bCashType );


private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int,VEC_REWARD>& vecRewardList_ );
    bool LoadResetItemList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::map<int,GCITEMID>& mapResetItemList_ );


private:
    mutable KncCriticalSection m_csLevelResetEvent;

    DWORD   m_dwVersion;

    time_t m_tmBegin;          // 이벤트 시작 날짜.
    time_t m_tmEnd;            // 이벤트 종료 날짜.
    int    m_nLevel;           // 목표 달성 레벨
    
    std::map<int,GCITEMID>  m_mapGPResetItemList;
    std::map<int,GCITEMID>  m_mapCashResetItemList;
    
    std::map<int,VEC_REWARD>  m_mapGPRewardItemList;
    std::map<int,VEC_REWARD>  m_mapCashRewardItemList;
};

DefSingletonInline( KLevelResetEvent );
DeclOstmOperatorA( KLevelResetEvent );
