#pragma once

class KGCRewardItemListBox;
class KGCMagicBoxItemWnd;

class KGCVipReward : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCVipReward );
    /// Default constructor
    KGCVipReward( void );
    /// Default destructor
    virtual ~KGCVipReward( void );
    /// Copy constructor
    KGCVipReward( const KGCVipReward& )
        : m_fPeriodGapY(0.0f)
        , m_fInvervalGapY(0.0f)
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCVipReward& operator=( const KGCVipReward& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreateComplete( void );
    virtual void OnClose();
    virtual void FrameMoveInEnabledState( void );
    virtual void OnCreate( void );

    void SetVipEventList(KEVENT_VIPEVENT_LIST_ACK& receiveData );
    void SetVipEventUserInfo(KEVENT_VIPEVENT_USER_INFO_ACK& receiveData );
    void SetVipEventUserRewardUpdate(KEVENT_VIPEVENT_USER_REWARD_UPDATE_NOT& receiveData );
    void SetEventPeriod(std::map<USHORT,KVipEvent>& _mapEvent, int _nEvnetType );
    void SetPayTable(std::map<USHORT,std::pair<int,int> >& _mapPayTable );
    void ViewNewUserRewardUI();
    void ViewVipUserRewardUI();
    void ViewToggleVipRewardUI( bool _view );
    void ViewToggleNewBuyUserRewardUI( bool _view );
    void OnClickInterval( int i );
    void ResizeUIBecausePayTable(int payTableCount );
    void LoadFinalVipReward( );
    void ViewNewUserBenefit( );
    void SetNewUserGuideMent();
    void VipEventUserInfoReq();
private:

    enum{
        NEW_USER_REWARD_MODE        = 0,
        VIP_USER_REWARD_MODE        = 1,
        ITEM_LIST_MAX_COUNT         = 10,
        ITEM_LIST_DEFAULT_VIEW_COUNT= 5,
        WEEK_LIST_MAX_COUNT         = 4,
    };
    KD3DWnd*            m_pkBtnClose;           //btn_close
    KD3DWnd*            m_pkTapVipUser;         //btn_vip_user
    KD3DWnd*            m_pkTapNewUser;         //btn_new_user
    KD3DWnd*            m_pkTapVipUserOff;         //btn_vip_user_lock
    KD3DWnd*            m_pkTapNewUserOff;         //btn_new_user_lock

    
    KD3DStatic*         m_pkVipBenefitAbout;     //static_vip_benefit_about
    KD3DStatic*         m_pkMyPurchaseStr;       //static_my_purchase_string
    KD3DStatic*         m_pkMyPurchaseAmount;    //static_my_purchase_amount
    KD3DStatic*         m_pkFinalRewardStr;      //static_final_reward_string


    KSafeArray<KD3DSizingBox*,ITEM_LIST_MAX_COUNT>  m_apkIntervalBG; 
    KSafeArray<KD3DStatic*,ITEM_LIST_MAX_COUNT>     m_apkIntervalStr;     
    KSafeArray<KD3DWnd*,ITEM_LIST_MAX_COUNT>        m_apkViewBenefit;  

    KEVENT_VIPEVENT_LIST_ACK                m_vipEventList;
    KEVENT_VIPEVENT_USER_INFO_ACK           m_vipUserInfo;
    KEVENT_VIPEVENT_USER_REWARD_UPDATE_NOT  m_vipUserRewardInfo;


    DWORD                   m_dwMainBGHeight;
    DWORD                   m_dwSubBGHeight;
    DWORD                   m_dwYellowFrameBGHeight;
    DWORD                   m_dwBenefitListBGHeight;


    int             m_iCurrentMode;

    const float     m_fPeriodGapY;
    const float     m_fInvervalGapY;



    KD3DSizingBox*      m_pkMainBackground;         //main_background//크기 변해야함. 
    KD3DSizingBox*      m_pkYellowFrame;         //sizing_frame_yellow//크기 변해야함. 
    KD3DSizingBox*      m_pkSubBackground;         //sub_background//크기 변해야함. 
    KD3DSizingBox*      m_pkBenefitListBG;          //sizing_benefit_list_bg



    //위치 변경
    KD3DWnd*            m_pkBtnConfirm;         //btn_confirm       //위치 변경

    KSafeArray<KD3DSizingBox*,WEEK_LIST_MAX_COUNT>  m_apkPeriodBG;
    KSafeArray<KD3DSizingBox*,WEEK_LIST_MAX_COUNT>  m_apkPeriodBGLock;
    KSafeArray<KD3DStatic*,WEEK_LIST_MAX_COUNT>     m_apkPeriodStrFirst;        
    KSafeArray<KD3DStatic*,WEEK_LIST_MAX_COUNT>     m_apkPeriodStrSecond;
    KD3DWnd*            m_pkImgPeriodStr;     //img_benefit_string
    KD3DWnd*            m_pkImgLine;     //img_line
    
    


    D3DXVECTOR2             m_vtBtnConfirmInitPos;
    D3DXVECTOR2             m_vtPeriodBGInitPos;
    D3DXVECTOR2             m_vtPeriodFirstStrOffset; 
    D3DXVECTOR2             m_vtPeriodSecondStrOffset;
    D3DXVECTOR2             m_vtImgPeriodInitPos;
    D3DXVECTOR2             m_vtImgLineInitPos;

    KGCMagicBoxItemWnd*     m_pkFinalItemWnd;


    GCITEMID                m_dwFinalItemID;


    KD3DWnd*                m_pkAboutNewbuyUser;          //img_new_buy_user_about_string
    KD3DWnd*                m_pkAboutVipUser;             //img_vip_about_string
    KD3DWnd*                m_pkImgBenefitStr;              //img_benefit_string

    KD3DWnd*                m_pkTitleNewbuyUser;              //img_new_buy_user_title_string
    KD3DWnd*                m_pkTitleVipUser;              //img_vip_title_string

    int                     m_nSizeOfPayTable;

    KGCRewardItemListBox*   m_pkRewardUI;


    KD3DSizingBox*          m_pkSizingNewUserBenefitBGBar;  //sizing_new_user_benefit_bg
    KD3DWnd*                m_pkbtnNewUserBenefit;          //btn_new_user_view_benefit
    KD3DStatic*             m_pkstaticNewUserBenefit;       //static_new_user_benefit
    KD3DSizingBox*          m_pkSizingNewUserBenefitBG;     //sizing_new_user_benefit_bg


    std::map<int, std::pair<GCITEMID, int > > m_mapVipFinalReward;  //<N주차, <최종보상 아이디, 수량>>
};

DEFINE_WND_FACTORY( KGCVipReward );
DECLARE_WND_FACTORY( KGCVipReward );
DECLARE_WND_FACTORY_NAME( KGCVipReward );
