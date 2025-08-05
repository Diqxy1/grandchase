#pragma once

class KGCHeroItemUpgradeMaterial : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroItemUpgradeMaterial );
    /// Default constructor
    KGCHeroItemUpgradeMaterial( void );
    /// Default destructor
    virtual ~KGCHeroItemUpgradeMaterial( void );
    /// Copy constructor
    KGCHeroItemUpgradeMaterial( const KGCHeroItemUpgradeMaterial& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroItemUpgradeMaterial& operator=( const KGCHeroItemUpgradeMaterial& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
     KGCItemImgWnd* m_pkItemImg;
     KD3DStatic*    m_pkStaticItemCount;  
     KD3DWnd*       m_pkBtnSelect;
     
     KDropItemInfo  m_kMaterialInfo;
     KItem*         m_pInvenItem;
     
     bool           m_bShowItemInfo;
     int            m_iSlotNumber;

public:
    void        ClearInfo( void );
    void        SetInfo( const KDropItemInfo& Info_ );
    void        SetInfo( GCITEMUID itemUID_ );
    KItem*      GetInventoryItem( void );
    std::pair< bool, GCITEMUID > GetMaterialUID( void );
    void        ShowItemInfo( void );
    void        OnClickSelect( void );

    void        SetSlotNumber( int iSlotNumber_ ) { m_iSlotNumber = iSlotNumber_; }
    int         GetSlotNumber( void ) { return m_iSlotNumber; }
    
};

DEFINE_WND_FACTORY( KGCHeroItemUpgradeMaterial );
DECLARE_WND_FACTORY( KGCHeroItemUpgradeMaterial );
DECLARE_WND_FACTORY_NAME( KGCHeroItemUpgradeMaterial );
