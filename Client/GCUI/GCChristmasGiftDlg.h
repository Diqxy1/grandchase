#pragma once



const int   MAX_CHRISTMAS_GIFT_KIND    = 9;
const int   MAX_CHRISTMAS_REWARD       = 7;
const int   MENT_REFRESH_TIME          = 5000; // ms
const DWORD MAX_BONUS_BAR_LENGTH     = 325;

class KGCChristmasGiftItemBox;
class KGCChristmasRewardBox;
class KGCChristmasDonateDlg;
class KGCChristmasSendGiftDlg;



class KGCChristmasGiftDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChristmasGiftDlg );
    /// Default constructor
    KGCChristmasGiftDlg( void );
    /// Default destructor
    virtual ~KGCChristmasGiftDlg( void );
    /// Copy constructor
    KGCChristmasGiftDlg( const KGCChristmasGiftDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChristmasGiftDlg& operator=( const KGCChristmasGiftDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*            m_pkBtnDonate;
    KD3DWnd*            m_pkBtnClose;

    KD3DStatic*         m_pkStaticMerrySay;
    KD3DStatic*         m_pkStaticMySledgeStr;
    KD3DStatic*         m_pkStaticMySledgeCount;
    KD3DStatic*         m_pkStaticMyPointStr;
    KD3DStatic*         m_pkStaticMyPointCount;

    KD3DWnd*            m_pkBonusPointBar;

    std::vector<int>    m_vecMerryMent;

    KSafeArray<KGCChristmasGiftItemBox*, MAX_CHRISTMAS_GIFT_KIND>   m_apkChristmasGiftItemBox;
    KSafeArray<KGCChristmasRewardBox*, MAX_CHRISTMAS_REWARD>   m_apkChristmasRewards;

    KGCChristmasDonateDlg* m_pkChristmasDonateDlg;

    KGCChristmasSendGiftDlg* m_pkChristmasSendGiftDlg;




    // data
    GCITEMID                                        m_SledgeItemID;         // 이벤트 ItemID
    std::map<KDropItemInfo,std::pair<DWORD,DWORD>>  m_mapPresentItemInfo;
    std::map<DWORD,DWORD>                           m_mapDonationInfo;
    std::map<DWORD,VEC_REWARD>                      m_mapGradeRewardList;
    std::map<DWORD,std::pair<DWORD,DWORD>>          m_mapGradeDesc;

    DWORD                                           m_dwSledgeCount;         // 이벤트 아이템 갯수

    DWORD                                           m_dwRecentlyRecievedRewardGrade;  // 착한일 포인트로 어느 등급까지 받았나
    DWORD                                           m_dwBonusPoint;         // 현재 착한일 포인트

    std::vector<KEVENT_CHARISMAS_EVENT_PRESENT_NOT> m_vecKeepMsg;

    GAME_STATE GetCurrentGameState;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );


    void OnClickClose();
    void OnClickDonate();
    void LoadChristmasGiftEventInfo();
    void MerrySayFrameMove();
    void SetSledgeCount();
    void InitEventInfo();
    void ActivateSendGiftItemDlg( GCITEMID dwItemID );
    void RenderItemInfoBox( D3DXVECTOR2& vPos_, KItem pInvenItem_ );
    void KeepMsg( KEVENT_CHARISMAS_EVENT_PRESENT_NOT& kRecv_ ){ m_vecKeepMsg.push_back( kRecv_ ); }
    void ShowGiftMsg();



    // S->C
    void OnEventChristmasEventNot( KEVENT_CHARISMAS_EVENT_NOT& kRecv_ );
    void OnEventChristmasEventInfoAck( KEVENT_CHARISMAS_EVENT_INFO_ACK& kRecv_ );
    void OnEventChristmasEventDonationAck( KEVENT_CHARISMAS_EVENT_DONATION_ACK& kRecv_ );
    void OnEventChristmasEventPresentAck( KEVENT_CHARISMAS_EVENT_PRESENT_ACK& kRecv_ );
    void OnEventChristmasEventPresentNot( KEVENT_CHARISMAS_EVENT_PRESENT_NOT& kRecv_, bool bForce );
    void OnEventChristmasEventGradeRewadAck( KEVENT_CHARISMAS_EVENT_GRADE_REWARD_ACK& kRecv_ );


    // C->S
    void SendChristmasEventDonateReq( DWORD dwCount );


    DWORD GetCurrentBonusPoint() { return m_dwBonusPoint; }
    DWORD GetCurrentSledgeCount() { return m_dwSledgeCount; }
    DWORD GetRecentlyRecievedGrade() { return m_dwRecentlyRecievedRewardGrade; }
    KGCChristmasSendGiftDlg* GetSendGiftItemDlg() { return m_pkChristmasSendGiftDlg; }
    bool IsBonusPointMax();
    bool IsSubDlgRenderOn();

};

DEFINE_WND_FACTORY( KGCChristmasGiftDlg );
DECLARE_WND_FACTORY( KGCChristmasGiftDlg );
DECLARE_WND_FACTORY_NAME( KGCChristmasGiftDlg );
