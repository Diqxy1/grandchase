#pragma once

const DWORD MAX_POINT_BAR_LENGTH = 234;



class KGCEventEclipsePlotDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEventEclipsePlotDlg );
    /// Default constructor
    KGCEventEclipsePlotDlg( void );
    /// Default destructor
    virtual ~KGCEventEclipsePlotDlg( void );
    /// Copy constructor
    KGCEventEclipsePlotDlg( const KGCEventEclipsePlotDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEventEclipsePlotDlg& operator=( const KGCEventEclipsePlotDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    enum BTN_TYPE 
    {
        BTN_CLOSE = 0,
        BTN_REWARD_1,
        BTN_REWARD_2,
        BTN_REWARD_FINAL,

        MAX_BTN,
    };

    enum STATIC_TYPE 
    {
        STATIC_REWARD_1_STR = 0,
        STATIC_REWARD_2_STR,
        STATIC_LEFT_TIME_STR,
        STATIC_LEFT_TIME,
        STATIC_MATERIAL_STR,
        STATIC_MATERIAL,
        STATIC_REWARD_1_DESC,
        STATIC_REWARD_2_DESC,
        STATIC_REWARD_FINAL_DESC,

        MAX_STATIC,
    };

    enum REWARD_ITEM_TYPE
    {
        REWARD_1 = 0,
        REWARD_2,
        REWARD_FINAL,

        MAX_REWARD_ITEM,
    };


    KD3DWnd*            m_apkBtn[MAX_BTN];
    KD3DStatic*         m_apkStatic[MAX_STATIC];
    KGCItemImgWnd*      m_apkImgWnd[MAX_REWARD_ITEM];

    KD3DWnd*            m_pkPointBar;


    // 
    KncStopWatch        m_kStopWatch; // 
    KncStopWatch        m_kSecondCounter; // 

    // 
    VEC_REWARD  m_vecTimeRewardItem;          // 시간 보상 아이템
    VEC_REWARD  m_vecHuntRewardItem;          // 수집 보상 아이템 
    VEC_REWARD  m_vecFinalRewardItem;         // 최종 보상 아이템
    int         m_nMaxProgress;               // 최종 보상에 필요한 포인트
    int         m_nProgress;                  // 유저의 최종 보상 진행도
    GCITEMID    m_HuntItemID;                 // 재료 아이템 ID
    int         m_nNecessaryHuntItemCount;    // 보상을 위해 모아야 하는 재료 갯수
    int         m_nHuntItemCount;             // 유저가 가지고 있는 재료 갯수
    bool        m_bGotFinalReward;            // 최종보상을 받았는지 여부
    bool        m_bIsTimeToReward;            // 시간보상을 받을 수 있는 시간이 되었나
    DWORD       m_dwLeftTimeToReward;         // 다이얼로그 열었을 때 시간보상까지 남은 시간
    DWORD       m_dwEventPeriod;              // 이벤트 발동 주기, 단위 : s 30분 - 1,800



public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

//
    void Update();
    void UpdateTime();
    void UpdateHuntItem();
    void UpdateProgress();
    void UpdateRewardInfo();

//
    void OnClickClose();
    void OnClickRewardTime();
    void OnClickRewardHunt();
    void OnClickRewardFinal();

    //
    void OnEventEclipsePlotInfoAck( KEVENT_ECLIPSE_PLOT_INFO_ACK& kRecv_ );
    void OnEventEclipsePlotTimeRewardReadyNot();
    void OnEventEclipsePlotHuntRewardAck( KEVENT_ECLIPSE_PLOT_HUNT_REWARD_ACK& kRecv_ );
    void OnEventEclipsePlotTimeRewardAck( KEVENT_ECLIPSE_PLOT_TIME_REWARD_ACK& kRecv_ );
    void OnEventEclipsePlotFinalRewardAck( KEVENT_ECLIPSE_PLOT_FINAL_REWARD_ACK& kRecv_ );


};

DEFINE_WND_FACTORY( KGCEventEclipsePlotDlg );
DECLARE_WND_FACTORY( KGCEventEclipsePlotDlg );
DECLARE_WND_FACTORY_NAME( KGCEventEclipsePlotDlg );
