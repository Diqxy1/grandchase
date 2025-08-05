#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>

#include <Thread/Locker.h>
#include "UserPacket.h"

class KLuaManager;

class KEclipsePlot
{
    DeclareSingleton( KEclipsePlot );
    NiDeclareRootRTTI( KEclipsePlot );
    DeclToStringW;

public:
    KEclipsePlot(void);
    ~KEclipsePlot(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsRun();
    void GetTimeRewardItem( IN VEC_REWARD& vecItem_ );
    void GetHuntRewardItem( IN VEC_REWARD& vecItem_ );
    void GetFinalRewardItem( IN VEC_REWARD& vecItem_ );
    void GetRewardInfo( OUT KEVENT_ECLIPSE_PLOT_INFO_ACK& kData );
    int GetMaxProgress();
    int GetHuntItemCount();
    void GetBeginTime( OUT time_t& tmDate_ );
    
private:
    bool LoadTime( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadRewardList( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::vector< KDropItemInfo >& vecRewardList_ );
    bool LoadHuntItem( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT std::pair<GCITEMID,int>& prData_ );


private:
    mutable KncCriticalSection m_csEclipsePlot;
    int m_nMaxProgress; // 최종보상을 위한 목표값
    time_t m_tmBegin;          // 이벤트 시작 날짜.
    time_t m_tmEnd;            // 이벤트 종료 날짜.
    VEC_REWARD m_vecTimeRewardList; // 개기일식 이벤트 발동시 보상 아이템
    VEC_REWARD m_vecHuntRewardList; // 개기일식 수집 보상 아이템
    VEC_REWARD m_vecFinalRewardList; // 최종 보상 아이템 
    std::pair<GCITEMID,int> m_prHuntItem; // ItemID, count  수집 아이템 아이디와 보상 필요 개수

};

DefSingletonInline( KEclipsePlot );
DeclOstmOperatorA( KEclipsePlot );
