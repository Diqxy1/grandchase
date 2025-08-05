#pragma once
//#include <vector>
#include "GCSubMission.h"

struct SMissionInfo
{
    enum MISSION_TYPE { MT_DEFAULT = 0, MT_COLLECTION };

    struct SReward {
        GCITEMID    m_ItemID;
        int         m_nCount;
        int         m_nPeriod;
        char        m_cGrade;

        SReward( GCITEMID itemID_, int nCount_, int nPeriod_, char cGrade )
        {
            m_ItemID = itemID_;
            m_nCount = nCount_;
            m_nPeriod = nPeriod_;
            m_cGrade = cGrade;
        }
    };

    DWORD                       m_dwID;

    int                         m_iTitleID;
    int                         m_iDescID;
    //미션의 등록 조건
    int                         m_iCharacter;
    int                         m_iPromotion;
    int                         m_iCorrectLevel;
    std::pair<int, int >        m_pairLevelLimit;

    std::vector< DWORD >        m_vecInvalidMissions;   // 중복 등록이 안되는 미션들
    std::vector< DWORD >        m_vecInvalidCompletMissions;    // 이 목록에 있는 미션을 완료한 적 있으면 등록 안됨.
    bool                        m_bUnique;

    //미션의 조건
    int                         m_iPeriod;
    std::vector<int>            m_vecSubMissions;

    //미션의 타입
    int                         m_iMissionType;

    //연계미션정보(이 미션의 연계단계 / 전체 연계단계)
    int                         m_iCurrentChain;
    int                         m_iTotalChain;

    //보상
    int                         m_iNewCharacter;
    int                         m_iChangeJob;
    int                         m_iGP;
    int                         m_iExp;
    std::vector< SReward >        m_vecRewardItem;
    std::vector< SReward >        m_vecSelectItem;
	int							m_iNPCID;
    bool                        m_bEnableDelete;    //  삭제 가능 여부

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
        m_iCorrectLevel = -1;
        m_iMissionType = 0;
        m_iCurrentChain = 0;
        m_iTotalChain = 0;
		m_iNPCID =0;
        m_bEnableDelete = true;
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
        m_iCorrectLevel                     = arg.m_iCorrectLevel;
        m_iMissionType                      = arg.m_iMissionType;
        m_iCurrentChain                     = arg.m_iCurrentChain;
        m_iTotalChain                       = arg.m_iTotalChain;
		m_iNPCID							= arg.m_iNPCID;
        m_bEnableDelete                     = arg.m_bEnableDelete;
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
