#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include <set>
#include "CommonPacket.h"

class KLuaManager;

class KJumpingChar
{
    DeclareSingleton( KJumpingChar );
    NiDeclareRootRTTI( KJumpingChar );
    DeclToStringW;

public:
    enum JumpingType
    {
        JT_CREATE      = 0,    // 캐릭터 생성하여 점핑.
        JT_UPDATE      = 1,    // 기존 생성된 캐릭터 점핑.
        JT_REWARD      = 2,    // 점핑하지 않고 보상만 받음.
        AT_MAX,
    };

    typedef std::vector<KDropItemInfo> VEC_REWARD;

    KJumpingChar(void);
    ~KJumpingChar(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    time_t GetEventBeginTime() { KLocker lock( m_csJumpingChar ); return m_tmBegin; };
    DWORD GetJumpingCharLevel() { KLocker lock( m_csJumpingChar ); return m_dwJumpingCharLevel; };
    void GetJumpingCharEnableList( OUT std::set<char>& listJumpingCharEnableList );
    bool IsEventTerm();
    bool GetJumpingCharDefaultReward( OUT VEC_REWARD& vecReward );
    bool GetJumpingCharReward( IN const char& cCharType, OUT VEC_REWARD& vecReward );
    bool GetJumpingCharMissionClearList( IN const char& cCharType, OUT std::vector<int>& vecMissionInfo );
    bool GetJumpingCharDungeonClearList( IN const char& cCharType, OUT std::vector<int>& vecDungeonInfo );
    bool GetJumpingCharSkillList( IN const char& cCharType, OUT std::vector<int>& vecSkillInfo );
    bool GetJumpingCharSkillGroupList( IN const char& cCharType, OUT std::vector<int>& vecSkillGroupInfo );
    bool GetJumpingCharPromtion( IN const char& cCharType, OUT char& cPromotion );
    bool GetJumpingCharRegisterMission( IN const char& cCharType, OUT std::vector<int>& vecMission );

protected:
    bool LoadTime( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT time_t& tmTime );
    bool LoadSetCharData( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<char>& setInfo );
    bool LoadVectorCharData( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<char>& vecCharInfo );
    bool LoadVectorIntData( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<int>& vecIntInfo );
    bool LoadDungeonClearList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<char, std::vector<int>>& mapDungeonClearList );
    bool LoadRewardItemInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map< char, VEC_REWARD >& mapJumpingRewardItem );
    bool LoadReward( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT VEC_REWARD& vecRewardInfo );
    bool LoadRewardSkillList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<char, std::vector<int>>& mapSkillRewardList );
    bool LoadRewardUnLockSkillList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<char, std::vector<int>>& mapSkillGroupRewardList );
    bool LoadClearMissionList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<char, std::vector<int>>& mapMissionClearList );
    bool LoadJumpingCharPromotion( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<char, char>& mapJumpingCharPromotion );
    bool LoadRegisterMissionList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<char, std::vector<int>>& mapMissionRegisterList );

protected:
    mutable KncCriticalSection          m_csJumpingChar;
    time_t                              m_tmBegin;          // 이벤트 시작 날짜.
    time_t                              m_tmEnd;            // 이벤트 종료 날짜.
    DWORD                               m_dwJumpingCharLevel; // 점핑 캐릭터 레벨.
    std::map<char, char>                m_mapJumpingCharPromotion; // 점핑 캐릭터 전직정보.
    std::set<char>                      m_setJumpingCharEnableList; // 점핑 가능 캐릭터 리스트.
    std::map<char, std::vector<int>>    m_mapDungeonClearList; // 던전 클리어 리스트.
    std::map<char, VEC_REWARD>          m_mapJumpingRewardItem; // 점핑 캐릭터 보상 아이템
    std::map<char, std::vector<int>>    m_mapSkillRewardList; // 스킬 보상 리스트.
    std::map<char, std::vector<int>>    m_mapSkillGroupRewardList; // 스킬 그룹 보상 리스트.
    std::map<char, std::vector<int>>    m_mapMissionClearList; // 미션 클리어 리스트.
    VEC_REWARD                          m_vecDefaultRewardList; // 점핑하지 않을 경우 보상 리스트.
    std::map<char, std::vector<int>>    m_mapMissionRegisterList; // 미션 등록 리스트.
};

DefSingletonInline( KJumpingChar );
DeclOstmOperatorA( KJumpingChar );