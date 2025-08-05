#pragma once

class KGCAgitShopItemBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum MONEY_TYPE { MONEY_STAR = 0, MONEY_CASH = 1 };

public:
    DECLARE_CLASSNAME( KGCAgitShopItemBox );
    /// Default constructor
    KGCAgitShopItemBox( void );
    /// Default destructor
    virtual ~KGCAgitShopItemBox( void );
    /// Copy constructor
    KGCAgitShopItemBox( const KGCAgitShopItemBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitShopItemBox& operator=( const KGCAgitShopItemBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );
    virtual void OnDestroy( void );

public:
    void SetStarItemInfo( PAIR_USHORT_DWORD pariIndex_, KManufactureItem ItemInfo_, std::vector<KDropItemInfo>& vecMaterial_ );
    void SetCashItemInfo( GCItem* pItem );

    void BoxClear( void );
    void OnClick( void );

    GCItem* GetItemData( void ) { return m_pItemData; }
    PAIR_USHORT_DWORD GetPairIndex( void ) { return m_PairIndex; }
    
    void SetMoneyType( int iType_ ); 
    int GetMoneyType( void ) { return m_iMoneyType; }
    int GetPrice( void ) { return m_iPrice; }

    void SetActive( bool bActive_ );
    bool IsActive( void ) { return m_pkImgBackActive->IsRenderOn(); }

private:
    KD3DWnd*    m_pkImgBackDefault;
    KD3DWnd*    m_pkImgBackActive;
    KD3DWnd*    m_pkIconStar;
    KD3DWnd*    m_pkIconCash;
    KD3DStatic* m_pkStaticName;
    KD3DStatic* m_pkStaticPeriod;
    KD3DStatic* m_pkStaticCount;
    KD3DSizingBox*      m_pkPriceBack;
    GCDeviceTexture*    m_pkItemTex;
    

    GCItem*             m_pItemData;
    PAIR_USHORT_DWORD   m_PairIndex;
    KManufactureItem    m_ItemInfo;
    std::vector<KDropItemInfo> m_vecMaterial;
    
    int m_iPrice;
    int m_iMoneyType;

    int m_iNowCount;
    int m_iLimitCount;

private:
    void SetBoxDetail( void );
};

DEFINE_WND_FACTORY( KGCAgitShopItemBox );
DECLARE_WND_FACTORY( KGCAgitShopItemBox );
DECLARE_WND_FACTORY_NAME( KGCAgitShopItemBox );
