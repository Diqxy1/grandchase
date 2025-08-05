#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <vector>
#include <map>
#include "PrimitiveTypedef.h"

struct KChannelRatioInfo
{
    float m_fExpRatio;
    float m_fGPRatio;

    KChannelRatioInfo() {
        m_fExpRatio = 0.f;
        m_fGPRatio = 0.f;
    }
};

struct KPetInfo;
struct KRewardPet {
    std::vector<KPetInfo>                     m_vecDefaultPet; // 접속시 지급되는 펫
    std::map<PAIR_INT,std::vector<KPetInfo> > m_mapEventPet; // 특정기간에 지급되는 펫 map[ pair[월,일], vector[펫정보] ]
};

struct KDropItemInfo;
class KLuaManager;
struct KSimpleItem;
struct KItem;

class KChannelTypeEvent
{
    DeclareSingleton( KChannelTypeEvent );
    NiDeclareRootRTTI( KChannelTypeEvent );
    DeclToStringW;

    enum {
        MININUM_PET_SATIATION = 1000
    };

public:
    typedef std::vector<KDropItemInfo > VEC_ITEMS;

    KChannelTypeEvent(void);
    ~KChannelTypeEvent(void);
    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool IsEventTerm(); // 이벤트 기간인지 체크.
    bool GetDailyConnectionReward( IN const DWORD dwChannelType,OUT VEC_ITEMS& vecItems );
    bool GetOneConnectionReward( IN const DWORD dwChannelType, OUT VEC_ITEMS& vecItems );
    bool GetNewConnectionReward( IN const DWORD dwChannelType, OUT VEC_ITEMS& vecItems );

    void GetBeginTime( OUT time_t& tmDate );
    void GetNewUserCheckTime( OUT time_t& tmDate );
    bool IsNewUserCheck( IN const time_t tmDate );
    void GetExpGpBonus( IN const DWORD dwChannelType, OUT float& fExpRatio, OUT float& fGpRatio );
    void GetChannelPetItem( IN const DWORD dwChannelType, IN OUT std::map<GCITEMUID, KPetInfo>& m_mapPetInfo, IN OUT std::vector<KItem>& vecItems ); // 펫 아이템 넣기.

protected:
    bool LoadRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< DWORD, VEC_ITEMS >& mapRewards );
    bool LoadItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_ITEMS& vecRewards );
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadChannelRatio( IN KLuaManager& kLuaMng, OUT std::map< DWORD, KChannelRatioInfo >& mapChannelRatioInfo );
    bool LoadRatioInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT KChannelRatioInfo& kRatioInfo );
    bool LoadSimpleItems( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KSimpleItem>& vecSimpleItem );
    bool LoadPetReward( IN KLuaManager& kLuaMng, OUT std::map< DWORD, KRewardPet >& mapPetReward );
    bool LoadDefaultPet( IN KLuaManager& kLuaMng, OUT std::vector<KPetInfo>& vecPet );
    bool LoadEventPet( IN KLuaManager& kLuaMng, OUT std::map<PAIR_INT,std::vector<KPetInfo> >& mapPet );
    bool LoadPetInfo( IN KLuaManager& kLuaMng, OUT KPetInfo& kPet );
    void SetChannelPetInfo( IN OUT KPetInfo& kPetInfo );
    void GetPetItem( IN const KRewardPet& kReward, IN OUT std::map<GCITEMUID, KPetInfo>& m_mapPetInfo, IN OUT std::vector<KItem>& vecItems );

protected:
    mutable KncCriticalSection                      m_csChannelType;
    time_t                                          m_tmBegin; // 이벤트 시작시간
    time_t                                          m_tmEnd; // 이벤트 종료시간
    time_t                                          m_tNewUserBegin; // 신규유저 체크시간.
    std::map< DWORD, VEC_ITEMS >                    m_mapDailyConnectionReward;
    std::map< DWORD, VEC_ITEMS >                    m_mapOneConnectionReward;
    std::map< DWORD, VEC_ITEMS >                    m_mapNewConnectionReward;
    std::map< DWORD, KChannelRatioInfo >            m_mapChannelRatioInfo;
    std::map< DWORD, KRewardPet >                   m_mapPetReward;
};

DefSingletonInline( KChannelTypeEvent );
DeclOstmOperatorA( KChannelTypeEvent );