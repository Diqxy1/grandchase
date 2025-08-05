#pragma once

#define MAX_ITEM_VIEW 6
#define ONE_LINE_ITEM_COUNT 3

class KGCItemImgWnd;

class KGCCooperationEventDlg :	public KD3DDialog,
                                public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCooperationEventDlg );
    /// Default constructor
    KGCCooperationEventDlg( void );
    /// Default destructor
    virtual ~KGCCooperationEventDlg( void );
    /// Copy constructor
    KGCCooperationEventDlg( const KGCCooperationEventDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCooperationEventDlg& operator=( const KGCCooperationEventDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    //virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd* m_pkOkBtn;
    KD3DWnd* m_pkCloseBtn;
    KD3DWnd* m_pkWebLinkBtn;
    KD3DWnd* m_pkCouponCode;
    KD3DWnd* m_pkItemPointBar[MAX_ITEM_VIEW];
    KD3DWnd* m_pkBuyBtn[MAX_ITEM_VIEW];
    KD3DWnd* m_pkScrollBarBtn;
    KD3DWnd* m_pkEventLogImg;

    KD3DEdit* m_pkEditCouponInput;
    
    KD3DStatic* m_pkStaticCouponDesc;
    KD3DStatic* m_pkStaticEventTitle;
    KD3DStatic* m_pkStaticEventDesc;
    KD3DStatic* m_pkStaticMyPointString;

    KGCItemImgWnd* m_pkItemImgView[MAX_ITEM_VIEW];

    KD3DScrollbar* m_pkScrollBar;

    KGCItemInfoBox* m_pkItemInfoBox;

//UI외에 필요한 변수
    bool m_bScriptLoadComplete;
    int m_iEventID;
    int m_iTitleStringID;
    int m_iDescStringID;
    int m_iDescToolTipStringID;
    std::string m_strLogoImgFileName;
    std::wstring m_strWebLink;
    GCDeviceTexture* m_pLogoTexture;
    DWORD m_dwMoneyItemID;

    std::map< int, KCoopEventItem> m_mapCoopEventItemInfo;

public:
    bool LoadCooperationEventInfo();
    void OnExit();
    void OnScroll();
    void OnBuy(int iIndex_);
    void InitSaleItemView();
    int GetItemViewCount(int iItemIndex_);
    void SetEventID(const int& iEventID_) { m_iEventID = iEventID_; }
    int GetEventID() { return m_iEventID; }
    void SetMoneyItemID(const DWORD& dwMoneyItemID_) { m_dwMoneyItemID = dwMoneyItemID_; }
    void SetCoopEventItemInfo(std::map< int, KCoopEventItem> mapCoopEventItem_ ){ m_mapCoopEventItemInfo.swap( mapCoopEventItem_ ); }
    void OnCodeInput();
    bool IsCorrectCode( std::string strInputCode_);
    void OnWebLink();
    int GetMyMoneyPoint();
    void CodeInputEditWndInit();
};


DEFINE_WND_FACTORY( KGCCooperationEventDlg );
DECLARE_WND_FACTORY( KGCCooperationEventDlg );
DECLARE_WND_FACTORY_NAME( KGCCooperationEventDlg );