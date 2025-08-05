#pragma once

#include "KNCSingleton.h"

struct SGiftUserInfo{
public:
    SGiftUserInfo()
    {
        Init();
    }
    void Init()
    {
        m_vecGetRewards.clear();
        m_nAccPlayTime = 0;
        m_vecNextGift.clear();
    }
    std::vector< KItem >                            m_vecGetRewards;    // ( GET_GIFT_ACK인 경우 )이번에 받은 보상 아이템들 정보
    int                                             m_nAccPlayTime;     // 현재 단계 목표까지 남은 시간(분)
    std::vector< std::pair< int, std::vector< KDropItemInfo > > >   m_vecNextGift;
};

class KGCGiftBoxManager
{
    DeclareSingleton( KGCGiftBoxManager );

public:
    KGCGiftBoxManager(void);
    ~KGCGiftBoxManager(void);

    bool IsEvent( int iEventType );
    bool IsVeiwMode();
    void SetNewUserInfo( KEVENT_CONNECTION_GIFTBOX_NEWUSER_INFO_NOT& kNewUserInfo , bool bNewUser);
    void SetEventUserInfo( KEVENT_CONNECTION_GIFTBOX_EVENTUSER_INFO_NOT& kEventUserInfo , bool bEventUser);
    void SetNewUserRemainTime( int nTime );
    int GetNewUserRemainTime();
    void SetEventRemainTime( int nTime );
    int GetEventRemainTime();
    void Reset();
    bool ExistNextGiftForEvent();
    int CheckTime(int iEventType , bool bNextReward);
    void ExistNewUserReward( std::vector< std::pair< int, std::vector< KDropItemInfo > > >& mapNextReward , int iEventType);
    bool GetNextNewUserReward() { return m_bExistNewUserReward; }
    bool GetNextEventReward() { return m_bExistEventReward; }
    void SetEvent( int iEventtype, bool bEvent );
    int GetNewUserRewardItemID( int iEventType , int iIndex);
    void SetPossibleReward( int iEventType, bool bPossibleReward );
    bool GetPossibleReward( int iEventType);
    SGiftUserInfo sNewUser;
    SGiftUserInfo sEventUser;

private:
    bool m_bNewUser;
    bool m_bEventUser;

    bool m_bExistNewUserReward;
    bool m_bExistEventReward;

    bool m_bPossibleRewardNewUser;
    bool m_bPossibleRewardEvent;

    time_t m_tmEventBegin; //이벤트 전체 시작타임
    time_t m_tmEventEnd; //이벤트 전체 종료 타임

    time_t m_tNewUserGoalTime;
    time_t m_tEventGoalTime;

    int m_nNewUserCheckTime;
    int m_nNewUserViewTime;
    int m_nEventViewTime;

    GCITEMID m_iUserRewardItemID;
    std::pair<int, int> m_pairEventCheckTime;
};
DefSingletonInline( KGCGiftBoxManager );
