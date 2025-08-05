#pragma once

class KGCAgitShopItemBox;

class KGCAgitShop : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitShop );
    /// Default constructor
    KGCAgitShop( void );
    /// Default destructor
    virtual ~KGCAgitShop( void );
    /// Copy constructor
    KGCAgitShop( const KGCAgitShop& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitShop& operator=( const KGCAgitShop& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

public:
    enum BTN_TYPE 
    {
        BTN_CLOSE = 0,
        BTN_PLACE = 1,
        BTN_BUY   = 2,
        BTN_FIRST = 3,
        BTN_PREV  = 4,
        BTN_END   = 5,
        BTN_NEXT  = 6,

#if defined( NATION_PHILIPPINE )
        BTN_CASH_CHARGE  = 7,
#endif

        NUM_BTN,
    };

    enum TAB_TYPE
    {
        TAB_STAR_DEFAULT = 0,
        TAB_STAR_ACTIVE  = 1,
        TAB_CASH_DEFAULT = 2,
        TAB_CASH_ACTIVE  = 3,

        NUM_TAB,
    };

    enum ITEM_BOX
    {
        NUM_ITEM_BOX = 8,
    };

    struct KAgitShopItem
    {
        PAIR_USHORT_DWORD           pairIndex;
        KManufactureItem            itemInfo;
        std::vector<KDropItemInfo>  vecMaterial;
    };

private:
    KSafeArray< KD3DWnd*, NUM_BTN >    m_apkBtn;
    KSafeArray< KD3DWnd*, NUM_TAB >    m_apkTab;
    KD3DStatic*                        m_pkStaticPage;
    KSafeArray< KGCAgitShopItemBox*, NUM_ITEM_BOX > m_apkItemBox;

    // 현재 선택한 아이템 정보 표시용 
    KD3DWnd*        m_pkImgBack;
    KD3DStatic*     m_pkStaticItemName;
    KD3DStatic*     m_pkStaticItemDesc;
    KD3DStatic*     m_pkStaticItemNumDesc;
    KD3DStatic*     m_pkStaticItemNum;
    GCDeviceTexture*m_pkItemTex;
    KD3DWnd*        m_pkIconStar;
    KD3DWnd*        m_pkIconCash;

    int m_iCurTab;
    int m_iCurPage;
    int m_iTotalPage;
    
    int         m_iCurBoxIndex;
    GCItem*     m_pCurItem;
    
    int m_iTotalStarItemCount;
    int m_iTotalCashItemCount;
    
    std::vector< KAgitShopItem >    m_vecStarShopItem;
    std::vector< DWORD >            m_vecCashShopItem;

    int         m_iSavedCurPage;
    bool        m_bIsSavedCurPage;

public:
    void InitAgitShop( bool bIsStarTab );
    void SetStarShopItemInfo( std::map<PAIR_USHORT_DWORD,KManufactureItem>& mapItemInfo_, std::map<PAIR_USHORT_DWORD, std::vector<KDropItemInfo> >& mapMaterialInfo_ );
    void SetCashShopItemInfo( void );
    void ClearShopInfo( void );
    void OnClose( void );
    void OnBuy( void );
    void OnPrePlace( void );
    void OnCashCharge( void );

    void InitPage( void );
    void OnPageBtn( int iBtnType_ );
    void OnPressTab( int iTabType_ );

    void SetItemBox();
    void SetCurItemInfo( int iIndex_ );
    void ClearCurItemInfo( void );

    void SetNowNumStar( void );
	void UpdatePage( void );

    void SetSavedCurPage();
    void GetSavedCurPage();
};

DEFINE_WND_FACTORY( KGCAgitShop );
DECLARE_WND_FACTORY( KGCAgitShop );
DECLARE_WND_FACTORY_NAME( KGCAgitShop );
