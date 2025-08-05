#pragma once
#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include "Kairo.h"
#include "CommonPacket.h"

class KLuaManager;

struct KStageRewardStage
{
    int m_nStageNum;
    std::vector<KDropItemInfo> m_vecStageRewardInfo;
};

struct KStageRewardInfo
{
    int m_nModeID;
    int m_nDifficulty;
    std::vector<KStageRewardStage> m_vecStageInfo;

    KStageRewardInfo()
    : m_nModeID( 0 )
    , m_nDifficulty( 0 )
    {
        m_vecStageInfo.clear();
    }
};

class KStageReward
{
    DeclareSingleton( KStageReward );
    NiDeclareRootRTTI( KStageReward );
    DeclToStringW;
public:
    KStageReward(void);
    ~KStageReward(void);

    bool LoadScript();
    bool _LoadScript( OUT std::string& strScript );
    bool GetStageRewardInfo( IN const int& nGameMode, IN const int& nDifficulty, IN const int& nStage, OUT std::vector<KDropItemInfo>& kItemList );

protected:
    bool LoadStageRewardInfo( IN KLuaManager& kLuaMng, OUT KStageRewardInfo& KStageRewardInfo_ );
    bool LoadStageRewardStage( IN KLuaManager& kLuaMng, OUT KStageRewardStage& KStageRewardStage_ );
    bool LoadStageRewardStageItemList( IN KLuaManager& kLuaMng, OUT KDropItemInfo& KStageRewardStageItemList_ );

protected:
    mutable KncCriticalSection             m_csStageReward;
    std::vector<KStageRewardInfo> m_vecStageRewardInfoList;
};

DefSingletonInline( KStageReward );
DeclOstmOperatorA( KStageReward );
