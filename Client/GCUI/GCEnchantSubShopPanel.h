#pragma once

class KGCEnchantSubShopPanel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEnchantSubShopPanel );
    /// Default constructor
    KGCEnchantSubShopPanel( void );
    /// Default destructor
    virtual ~KGCEnchantSubShopPanel( void );
    /// Copy constructor
    KGCEnchantSubShopPanel( const KGCEnchantSubShopPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEnchantSubShopPanel& operator=( const KGCEnchantSubShopPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        ESHOP_ITEM_WND_NUM = 4,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

public:
    KSafeArray<KGCItemImgWnd*,ESHOP_ITEM_WND_NUM> m_pkItemWnd;
    KSafeArray<KD3DWnd*,ESHOP_ITEM_WND_NUM> m_pkBtnBuy;    

    KD3DWnd* m_pkBtnPrev;
    KD3DWnd* m_pkBtnNext;

    std::vector<GCITEMID> m_vecShopItemList;

    int m_iCurPage;

public:
    void OnPrevPage();
    void OnNextPage();
    void OnBuy( GCITEMID dwItemID_ );

    void UpdateItemWnd();
    void InitShopInfo();

    void SetEnchantShopItemList( std::set<GCITEMID> setShopItemList );

    bool RenderItemInfoBox();

    bool CheckItemInformation();

    void LockUI( bool bLock );

};

DEFINE_WND_FACTORY( KGCEnchantSubShopPanel );
DECLARE_WND_FACTORY( KGCEnchantSubShopPanel );
DECLARE_WND_FACTORY_NAME( KGCEnchantSubShopPanel );
