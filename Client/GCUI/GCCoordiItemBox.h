#pragma once

class KGCCoordiItemBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCoordiItemBox );
    /// Default constructor
    KGCCoordiItemBox( void );
    /// Default destructor
    virtual ~KGCCoordiItemBox( void );
    /// Copy constructor
    KGCCoordiItemBox( const KGCCoordiItemBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiItemBox& operator=( const KGCCoordiItemBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
    enum { LOW_PRICE = 0, HIGH_PRICE = 1, NUM_PRICE = 2 };

public:
    enum { COORDI_ITEM_BOX_NORMAL = 0, COORDI_ITEM_BOX_HOT = 1, NUM_COORDI_ITEM_BOX_TYPE = 2, };
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );
    virtual void OnCreate( void );
    virtual void OnDestroy( void );

    void SetItemInfo( KCoordiShopItem kItemInfo );
    void Clear( void );
    void OnEquip( void );

    bool IsHovered( void ) { return m_bHovered; }
    bool IsEquip( void ) { return m_bIsEquip; }
    void SetEquip( bool bEquip );
    GCItem* GetItemData( void ){ 
        if(m_kCoordiItemInfo.pOriginal)
            return m_kCoordiItemInfo.pOriginal; 
        return m_kCoordiItemInfo.pPackage7;

    }

    int GetBoxType( void ) { return m_iBoxType; }
    void SetBoxType( int iType_ );

private:
    KD3DWnd*                                m_pkItemBackDefault;
    KD3DWnd*                                m_pkItemBackActive;

    GCDeviceTexture*                        m_pkItemTexture;
    KD3DStatic*                             m_pkStaticItemName;
    KCoordiShopItem                         m_kCoordiItemInfo;
    KSafeArray<KD3DStatic*, NUM_PRICE>      m_apkStaticItemPeriod;
    KSafeArray<KD3DStatic*, NUM_PRICE>      m_apkStaticItemPrice;
    KSafeArray<KD3DWnd*,    NUM_PRICE>      m_apkBtnBuy;
    KSafeArray<KD3DWnd*,    NUM_PRICE>      m_apkCoin;

    int     m_iBoxType;
    bool    m_bHovered;
    bool    m_bIsEquip;

    void OnBuyBtn( int iBuyType );
};

DEFINE_WND_FACTORY( KGCCoordiItemBox );
DECLARE_WND_FACTORY( KGCCoordiItemBox );
DECLARE_WND_FACTORY_NAME( KGCCoordiItemBox );
