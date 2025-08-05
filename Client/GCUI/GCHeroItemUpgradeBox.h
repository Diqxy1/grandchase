#pragma once

class KGCHeroItemUpgradeMaterial;

class KGCHeroItemUpgradeBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCHeroItemUpgradeBox );
    /// Default constructor
    KGCHeroItemUpgradeBox( void );
    /// Default destructor
    virtual ~KGCHeroItemUpgradeBox( void );
    /// Copy constructor
    KGCHeroItemUpgradeBox( const KGCHeroItemUpgradeBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroItemUpgradeBox& operator=( const KGCHeroItemUpgradeBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

public:
    enum { MAX_MATERIAL = 5 };

private:
    KHeroItemUpgradeInfo    m_kItemInfo;
    KGCItemImgWnd*          m_pkItemImg;
    KD3DStatic*             m_pkStaticItemName;
    KD3DStatic*             m_pkStaticItemType;
    KD3DWnd*                m_pkBtnUpgrade;
    KSafeArray< KGCHeroItemUpgradeMaterial*, MAX_MATERIAL > m_apkMaterial;
    std::vector< GCITEMUID > m_vecMaterialUID;

    bool    m_bShowItemInfo;
    int     m_iBoxNumber;

public:
    void ClearInfo( void );
    void SetItemInfo( const IN KHeroItemUpgradeInfo& Info_ );
    void OnClickUpgrade( void );
    void ShowItemInfo( void );
    void UpdateMaterialInfo( void );

    void SetBoxNumber( int iBoxNumber_ ) { m_iBoxNumber = iBoxNumber_; }
    int  GetBoxNumber( void ) { return m_iBoxNumber; }

    void SetSelectMaterial( int iSlotNumber_, GCITEMUID itemUID_ );

    std::pair< GCITEMID, int > GetUpgradeItemKey( void );
    bool GetUpgradeMaterialUID( OUT std::vector< GCITEMUID >& vecUIDList_ );
};

DEFINE_WND_FACTORY( KGCHeroItemUpgradeBox );
DECLARE_WND_FACTORY( KGCHeroItemUpgradeBox );
DECLARE_WND_FACTORY_NAME( KGCHeroItemUpgradeBox );
