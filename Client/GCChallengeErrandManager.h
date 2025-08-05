#pragma once

#include "KNCSingleton.h"

struct ErrandItemBuyList{

    int m_nType;
    int m_nItemNum;

    ErrandItemBuyList()
    {
        m_nType = 0;
        m_nItemNum = 0;
    }

    bool operator == ( const ErrandItemBuyList& rh ) const
    {
        return ( m_nType == rh.m_nType ) &&
               ( m_nItemNum == rh.m_nItemNum );
    }
};

class GCChallengeErrandManager
{
        DeclareSingleton( GCChallengeErrandManager );

public:
    GCChallengeErrandManager(void);
    ~GCChallengeErrandManager(void);

private:
    int                              m_nPlayTime;
    int                              m_nDelayTime;
    int                              m_nTimeIncreaseRate;
    int                              m_nMaxStage;
    bool                             m_bPlayable;
    int                              m_nMarryAppearInterval;
    int                              m_nMarryKeepAliveTime;


    std::map<int, KErrandStageInfo>  m_mapErrandStageInfo;
    std::vector<GCITEMID>            m_vecRewardItemForDisplay;

    std::vector<KItem>               m_vecRewardItem;
    std::vector<KItem>               m_vecSpecialRewardItem; // 최종 보상 지급한 아이템

    bool                             m_bIsGetInfo;
    int                              m_iSyncPlayTime;
    int                              m_iRemainTime;

private:
    int m_nStageNum;
    int m_nBuyListMinCount;
    int m_nBuyListMaxCount;

    int m_MerryEmergenceTime;
    std::vector<KErrandBuyInfo> m_vecBuyInfoList;
    std::vector<ErrandItemBuyList> m_buyItemList;

public:
    void InitStageInfo( void );
    void SetErrandInfo( KEVENT_ERRAND_INIT_INFO_ACK& kRecv );
    void SetRewardItem( KEVENT_ERRAND_REWARD_ACK& kRecv );
    void SetEndGame( KEVENT_ERRAND_GAME_END_ACK& kRecv );
    void SetPlayable( bool bPlayable );
    void SetStageInfo( int iStage );

    int GetPlayTime( void )             { return m_nPlayTime;  }
    int GetDelayTime( void )            { return m_nDelayTime; }
    int GetTimeIncreaseRate( void )     { return m_nTimeIncreaseRate; }
    int GetMaxStage( void )             { return m_nMaxStage; }
    int GetMarryAppearTime( void )      { return m_nMarryAppearInterval; }
    int GetMarryKeepAliveTime( void )   { return m_nMarryKeepAliveTime; }
    bool GetPlayble( void )             { return m_bPlayable; }

    std::vector<KItem> GetRewardItem()                      { return m_vecRewardItem; }
    std::vector<KItem> GetSpecialRewardItem()               { return m_vecSpecialRewardItem; }
    std::vector<GCITEMID> GetRewardDisplayItemID()          { return m_vecRewardItemForDisplay; }

    void StageRandomInfo( void );
    int GetMerryEmergenceTime( ) { return m_MerryEmergenceTime; }
    std::vector<ErrandItemBuyList> GetBuyInfo()  { return m_buyItemList; }

    bool ErrandIsGetInfo( void ) { return m_bIsGetInfo; }
    void SetErrandGetInfo( bool bGetInfo ) { m_bIsGetInfo = bGetInfo; }
    int GetLastRewardTime( void );
    void SetLastRewardTime( int iRemainTime );

};


DefSingletonInline( GCChallengeErrandManager );