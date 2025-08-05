#pragma once

class KGCOlympicScheduleSlot;
class KGCOlympicRewardSlot;
class KGCOlympicRankSlot;

class KGCOlympicEventDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCOlympicEventDlg );
    /// Default constructor
    KGCOlympicEventDlg( void );
    /// Default destructor
    virtual ~KGCOlympicEventDlg( void );
    /// Copy constructor
    KGCOlympicEventDlg( const KGCOlympicEventDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOlympicEventDlg& operator=( const KGCOlympicEventDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    enum { NUM_SCHEDULE_SLOT = 5, NUM_REWARD_SLOT = 7, NUM_RANK_SLOT = 8 };

    KD3DWnd*    m_pkBtnStart;
    KD3DWnd*    m_pkBtnClose;
    KD3DStatic* m_pkStaticGameTitle;
    KD3DStatic* m_pkStaticTimeAlram;
    
    KD3DStatic* m_pkStaticRank;
    KD3DStatic* m_pkStaticNation;
    KD3DStatic* m_pkStaticScore;
    KD3DStatic* m_pkStaticUpdateDesc;

    KD3DScrollbar* m_pkScheduleScroll;


    KSafeArray< KGCOlympicScheduleSlot*, NUM_SCHEDULE_SLOT >    m_apkScheduleSlot;
    KSafeArray< KGCOlympicRewardSlot*, NUM_REWARD_SLOT >        m_apkRewardSlot;
    KSafeArray< KGCOlympicRankSlot*, NUM_RANK_SLOT >            m_apkRankSlot;

private:
    std::vector< std::pair< KOlympicSchedule, KRegDateRankInfo > >  m_vecScheduleInfo;  // 스케쥴 정보 
    std::map< DWORD, std::vector< KDropItemInfo > >                 m_mapGradeReward;   // 등급별 보상 
    std::vector< KTotalRankInfo >                                   m_vecTotalRank;     // 국가 순위 정보 

    int m_iScheduleSize;

    int m_iScheduleScrollMax;
    int m_iScheduleScrollPos;

public:
    void SetScheduleInfo( void );
    void SetScheduleSlot( void );
    void InitScheduleScroll( void );
    void SetScheduleScrollPos( const int iPos_ );

    void SetRankInfo( void );
    void SetRankSlot( void );

    void SetRewardInfo( void );
    void SetRewardSlot( void );

    void Onclose( void );
    void SetStartBtnState( const bool bStartEnable_ );

    void OnStartBtn( void );
    void SetStartBtnToolTip( IN int nStr);
};

DEFINE_WND_FACTORY( KGCOlympicEventDlg );
DECLARE_WND_FACTORY( KGCOlympicEventDlg );
DECLARE_WND_FACTORY_NAME( KGCOlympicEventDlg );
