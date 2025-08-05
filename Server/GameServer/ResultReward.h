#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Kairo.h"
#include "CommonPacket.h"

class KLuaManager;

struct KResultRewardInfo
{
    int                         m_nModeID; // 던전 ID
    int                         m_nDifficulty; // 던전 난이도
    std::map<PAIR_CHAR_INT, Kairo> m_mapDefaultRewardInfo; // 기본보상
    std::map<PAIR_CHAR_INT, Kairo> m_mapAddRewardInfo; // 추가보상

    KResultRewardInfo():m_nModeID( 0 )
    {
        m_mapDefaultRewardInfo.clear();
        m_mapAddRewardInfo.clear();
    }
};

struct KLevelRangeInfo
{
    int m_nBoundary; // 보상 컨테이너 참조 키값에 쓰이는 레벨 범위 값
    int m_nMinLevel;
    int m_nMaxLevel;
};

struct KAddRewardInfo
{
    bool                        m_bPCBangUser; // PC방이면 무조건 줄지 여부 결정.
    DWORD                       m_dwLimitClearTime; // 최소 ClearTime( millsecond ).
    int                         m_nMinPartyNum; // 최소 파티 인원.
};

class KResultReward
{
    DeclareSingleton( KResultReward );
    NiDeclareRootRTTI( KResultReward );
    DeclToStringW;
public:
    KResultReward(void);
    ~KResultReward(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetDefaultRewardInfo( IN const int& nGameMode, IN const int& nDifficulty, IN const char& cCharType_, IN const char& cLevel, OUT KDropItemInfo& kItem );
    bool GetAddRewardInfo( IN const int& nGameMode, IN const int& nDifficulty, IN const char& cCharType, IN const char& cLevel,
        IN const bool& bPCBang, IN const DWORD& dwPlayTime, IN const int& nPartyNum, OUT KDropItemInfo& kItem );
    bool GetLevelBoundaryInfoFromLevel( IN const int& nDungeonID, IN const int& nLevel, OUT int& nBoundaryInfo_  );

protected:
    bool LoadAddRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT KAddRewardInfo& kAddRewardInfo );
    bool LoadLevelRangeInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<int, std::vector<KLevelRangeInfo>>& mapLevelRange );
    bool LoadDungeonLevelList( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<KLevelRangeInfo>& vecLevelRangeList );
    bool LoadResultRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::map<PAIR_INT, KResultRewardInfo>& mapRewardInfo );
    bool LoadRewardInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT KResultRewardInfo& kResultRewardInfo );
    bool LoadRewardRatio( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT Kairo& kKairo );

protected:
    mutable KncCriticalSection          m_csResultReward;
    KAddRewardInfo                      m_kAddRewardInfo;
    std::map<PAIR_INT, KResultRewardInfo> m_mapRewardInfo;
    std::map<int, std::vector<KLevelRangeInfo>>	m_mapLevelRange; // ResultReward에 설정 하는 레벨 범위
};

DefSingletonInline( KResultReward );
DeclOstmOperatorA( KResultReward );
