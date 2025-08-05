#pragma once

class KGCHeroItemBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroItemBox );
    /// Default constructor
    KGCHeroItemBox( void );
    /// Default destructor
    virtual ~KGCHeroItemBox( void );
    /// Copy constructor
    KGCHeroItemBox( const KGCHeroItemBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroItemBox& operator=( const KGCHeroItemBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

    void SetItemBox(KHeroItemInfo& kHeroItem);
    void SetGameMode( EGCGameMode _emGameMode ) { m_emGameMode = _emGameMode; }
    void ClearItemBox( void );
    
    GCItem* GetItem( void ) { return m_pItem; }
    GCItem* GetMaterialItem( void ) { return m_pMaterialItem; }
    bool IsItemHovered( void ) { return m_bItemHovered; }
    bool IsCoinHovered(void) { return m_bCoinHovered; }
    bool IsVIPHovered(void) { return m_bVIPHovered; }
    bool IsMaterialHovered( void ) { return m_bMaterialItemHovered; }
    bool IsPreview( void ) { return m_bPreview; }
    bool IsLock( void ) { return m_bLock; }
    bool IsHeroCoin(void) { return m_pkHeroCoin->IsRenderOn(); }
    bool IsHeroCoinPiece(void) { return m_pkHeroCoinPiece->IsRenderOn(); }
    
    void SetPreview(bool bPreview); 
    void SetLock(bool bLock) { m_bLock = bLock; }
    void SetBuyType( bool _bBuy );
    void RenderSelectBtn( bool _bRender );

private:
    KHeroItemInfo m_kHeroItem;
    GCItem*     m_pItem;
    GCItem*     m_pMaterialItem;
    EGCGameMode m_emGameMode;

    bool        m_bItemHovered;
    bool        m_bCoinHovered;
    bool        m_bVIPHovered;
    bool        m_bMaterialItemHovered;
    bool        m_bPreview;
    bool        m_bLock;

    KD3DWnd*    m_pkBtnBuy;
    KD3DWnd*    m_pkBtnSelect;
    KD3DWnd*    m_pkBtnPreView;
    KD3DSizingBox* m_pkItemBackGround;
    KD3DSizingBox* m_pkMaterialItemPriceBack;
    KD3DSizingBox* m_pkMaterialItemBackground;

    KD3DWnd*    m_pkHeroCoin;
    KD3DWnd*    m_pkHeroCoinPiece;
    KD3DWnd*    m_pkHeroVIP;
    KD3DWnd*    m_pkEventMark;
    KD3DStatic* m_pkStaticItemName;
    KD3DStatic* m_pkStaticItemType;
    KD3DStatic* m_pkStaticItemPrice;
    KD3DStatic* m_pkStaticPrevItemNum;

    GCDeviceTexture*    m_pkItemTexture;
    GCDeviceTexture*    m_pkPrevItemTexture;
};

DEFINE_WND_FACTORY( KGCHeroItemBox );
DECLARE_WND_FACTORY( KGCHeroItemBox );
DECLARE_WND_FACTORY_NAME( KGCHeroItemBox );
