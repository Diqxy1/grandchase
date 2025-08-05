#pragma once

class KGCSocialCommerceRewardBox;

enum SocialCommerceTab {
    SCT_LIMITED_GOODS = 0,
    SCT_SOCIAL_COMMERCE = 1,
};

enum SocialCommerceGoodsType {
    SCGT_LIMITED_COUNT = 1,
    SCGT_LIMITED_PERIOD = 2,
    SCGT_ONE_PLUS_ONE= 3,
    SCGT_IN_SALE = 4,
};

class KGCSocialCommerceDlg : public KD3DDialog    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSocialCommerceDlg );
    /// Default constructor
    KGCSocialCommerceDlg( void );
    /// Default destructor
    virtual ~KGCSocialCommerceDlg( void );
    /// Copy constructor
    KGCSocialCommerceDlg( const KGCSocialCommerceDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSocialCommerceDlg& operator=( const KGCSocialCommerceDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DWnd*                    m_pkBtnClose;
    KD3DWnd*                    m_pkBtnBuy;
    KD3DWnd*                    m_pkBtnLeft;
    KD3DWnd*                    m_pkBtnRight;
    KD3DWnd*                    m_pkTabLimitedGoods;
    KD3DWnd*                    m_pkTabSocialCommerce;
    KD3DWnd*                    m_pkBtnReward;
    KD3DWnd*                    m_pkWndOnSale;
    KD3DWnd*                    m_pkWndAchievement;
    KD3DWnd*                    m_pkWndSoldOut;
    KD3DWnd*                    m_pkWndNotForSale;

    KD3DStatic*                 m_pkStaticPageNum;
    KD3DStatic*                 m_pkStaticPeriod;
    KD3DStatic*                 m_pkStaticItemName;
    KD3DStatic*                 m_pkStaticPriceText;
    KD3DStatic*                 m_pkStaticPrice;
    KD3DStatic*                 m_pkStaticGoalText;
    KD3DStatic*                 m_pkStaticGoal;

    KGCItemImgWnd*              m_pkItemImg;

    KGCSocialCommerceRewardBox*     m_pkSocialCommerceRewardBox;



    // 선착순, 기간구매
    std::map<DWORD, KSCSellInfo>   m_mapLimitedGoods;

    // 공동구매, 1+1
    std::map<DWORD, std::pair<KSCSellInfo, VEC_REWARD>>   m_mapSocialCommerce;


    SocialCommerceTab m_eCurrentTab;              // 현재 텝 ( 공동구매냐, 선착순... 등 )
    DWORD m_dwCurrentGoodsIndex;                     // 현재 상품 인덱스
    DWORD m_dwCountLimitedGoods;                     // 제한구매 상품 숫자
    DWORD m_dwCountSocialCommerce;                   // 공동구매 상품 숫자


    void AddtoLimitedGoods( IN KInformationDataOfSellType& kData_ );
    void AddtoSocialCommerce( IN KInformationDataOfSellType& kData_ );
    void ResetUI();
    void SetItemInfo( IN DWORD dwItemID );
    void SetItemCount( IN int nCount, IN int nLimitCount );
    void SetItemPeriod( IN time_t tmBegin, IN time_t tmEnd );


    void Buy( GCITEMID dwItemID_ );

    void OnClickTab( IN SocialCommerceTab eTab );
    void OnClickNextBtn();
    void OnClickPrevBtn();
    void OnClickCloseBtn();
    void OnClickBuyBtn();
    void OnClickRewardBtn();



public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void FrameMoveInEnabledState( void );

    void SetGoodsInfo( IN KEVENT_SOSCIAL_COMMERCE_INFO_ACK& kData );
    void ShowCurrentGoods();


};

DEFINE_WND_FACTORY( KGCSocialCommerceDlg );
DECLARE_WND_FACTORY( KGCSocialCommerceDlg );
DECLARE_WND_FACTORY_NAME( KGCSocialCommerceDlg );
