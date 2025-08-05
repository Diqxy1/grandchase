#pragma once
//////#include <vector>
//#include "GCSubMission.h"

struct SMissionInfo
{
    enum MISSION_TYPE { MT_DEFAULT = 0, MT_COLLECTION };

    DWORD                       m_dwID;

    int                         m_iTitleID;
    int                         m_iDescID;
    //미션의 등록 조건
    int                         m_iCharacter;
    int                         m_iPromotion;
    std::pair<int, int >        m_pairLevelLimit;

    std::vector< DWORD >        m_vecInvalidMissions;   // 중복 등록이 안되는 미션들
    std::vector< DWORD >        m_vecInvalidCompletMissions;    // 이 목록에 있는 미션을 완료한 적 있으면 등록 안됨.
    bool                        m_bUnique;

    //미션의 조건
    int                         m_iPeriod;
    std::vector<int>            m_vecSubMissions;

    //보상
    int                         m_iNewCharacter;
    int                         m_iChangeJob;
    int                         m_iGP;
    int                         m_iExp;
    std::vector< std::pair< DWORD, int > >        m_vecRewardItem;
    std::vector< std::pair< DWORD, int > >        m_vecSelectItem;

    SMissionInfo()
    {
        m_iCharacter = -1;
        m_iPromotion = -1;
        m_pairLevelLimit = std::make_pair( -1, -1 );
        m_iPeriod = -1;
        m_iNewCharacter = -1;
        m_iChangeJob = -1;
        m_iGP = -1;
        m_iExp = -1;
    }

    void Copy( const SMissionInfo& arg )
    {
        m_dwID                              = arg.m_dwID;
        m_iTitleID                          = arg.m_iTitleID;
        m_iDescID                           = arg.m_iDescID;
        m_iCharacter                        = arg.m_iCharacter;
        m_iPromotion                        = arg.m_iPromotion;
        m_pairLevelLimit                    = arg.m_pairLevelLimit;
        m_vecInvalidMissions                = arg.m_vecInvalidMissions;
        m_vecInvalidCompletMissions         = arg.m_vecInvalidCompletMissions;
        m_bUnique                           = arg.m_bUnique;
        m_iPeriod                           = arg.m_iPeriod;
        m_vecSubMissions                    = arg.m_vecSubMissions;
        m_iNewCharacter                     = arg.m_iNewCharacter;
        m_iChangeJob                        = arg.m_iChangeJob;
        m_iGP                               = arg.m_iGP;
        m_iExp                              = arg.m_iExp;
        m_vecRewardItem                     = arg.m_vecRewardItem;
        m_vecSelectItem                     = arg.m_vecSelectItem;
    }

    SMissionInfo( const SMissionInfo& arg )
    {
        Copy( arg );
    }

    SMissionInfo& operator=( const SMissionInfo& arg )
    {
        Copy( arg );
        return *this;
    }
};

class KGCMission
{
private:
    SMissionInfo m_sMissionInfo;
public:
    KGCMission(void);
    ~KGCMission(void);
    void SetMissionInfo( const SMissionInfo& sMissionInfo )
    {
        m_sMissionInfo = sMissionInfo;
    }

    const SMissionInfo* GetMissionInfo()
    {
        return &m_sMissionInfo;
    }

    int GetMinLevel()   {return m_sMissionInfo.m_pairLevelLimit.first; }
    int GetMaxLevel()   {return m_sMissionInfo.m_pairLevelLimit.second; }
};
