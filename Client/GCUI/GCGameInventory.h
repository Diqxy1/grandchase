#pragma once

class KD3DWnd;
class KD3DComboBox;
class KGCGameInvenItemSlot;
class KGCGameInvenEquipSlot;

class KGCGameInventory : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGameInventory );
    /// Default constructor
    KGCGameInventory( void );
    /// Default destructor
    virtual ~KGCGameInventory( void );
    /// Copy constructor
    KGCGameInventory( const KGCGameInventory& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGameInventory& operator=( const KGCGameInventory& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum { ITEM_SLOT_START_X = 17, ITEM_SLOT_START_Y = 232 };
    enum { ITEM_SLOT_GAP_X = 53, ITEM_SLOT_GAP_Y = 55 };
    enum { ITEM_SLOT_COLS = 2, ITEM_SLOT_ROWS = 7, NUM_ITEM_SLOT = 14 };
    enum { NUM_EQUIP_SLOT = 12 };
    enum { GRADE0 = 0, GRADE1 = 1, GRADE2 = 2, GRADE3 = 3, GRADE4 = 4, NUM_GRADE = 5 } ;

private:
    struct USER_STATUS { 
        int     m_iAtk;
        int     m_iDef;
        int     m_iHP;
        int     m_iSPAtk;
        int     m_iSPDef;
        float   m_fCritical;
        float   m_fCounterDef;
        float   m_fHPRecovery;
        float   m_fMPRecovery;
        int     m_iHPDrain;
        int     m_iMPDrain;
        float   m_fGPAdd;
        float   m_fEXPAdd;
        int     m_iHellSpear;
        int     m_iTotalAtk;

        USER_STATUS() :
        m_iAtk( 0 ), m_iDef( 0 ), m_iHP( 0 ), m_iSPAtk( 0 ), m_iSPDef( 0 ), m_fCritical( 0.0f ), m_fCounterDef( 0.0f ),
        m_fHPRecovery( 0.0f ), m_fMPRecovery( 0.0f ), m_iHPDrain( 0 ), m_iMPDrain( 0 ), m_fGPAdd( 0.0f ), m_fEXPAdd( 0.0f ), m_iHellSpear( 0 ), m_iTotalAtk(0) {}

        void Clear( void ) {
            m_iAtk = 0; m_iDef = 0; m_iHP = 0;  m_iSPAtk = 0;   m_iSPDef = 0;   m_fCritical = 0.0f; m_fCounterDef = 0.0f;
            m_fHPRecovery = 0.0f;   m_fMPRecovery = 0.0f;   m_iHPDrain = 0; m_iMPDrain = 0; m_fGPAdd = 0.0f;    m_fEXPAdd = 0.0f;   m_iHellSpear = 0;
            m_iTotalAtk = 0;
        }
    };

public:
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );
    virtual void ToggleRender( bool );
    virtual void OnDestroy( void );

private:
    void OnClickBtnGrade( int iIndex_ );
    
    void InitPage( void );
    void SetPage( void );
    void OnClickBtnPrev( void );
    void OnClickBtnNext( void );
    void SetPageStatic( void );
    void SetPageItem( void );

    void RefreshItemList( void );
    void FilterComboItem( void );
    void FilterItemGrade( void );
    void FilterSwapWeapon( void );
    void FilterNonEquipItem( void );
    void SetInventoryItemSlot( void );

    bool GetCurCharacterItemList( IN PLAYER* pPlayer_, OUT SUserInfo& userEquip_ );
    bool OnClickInvenItem( const KActionEvent& event );
    bool OnCiickEquipItem( const KActionEvent& event );
    
    void InitEquipSlot( void );
    void SetEquipSlot( GCITEMUID itemUID_ );
    int  GetEquipSlotIndex( int iItemSlot_ );

    void InitComboList( void );
    void OnComboChange( void );
    int  GetComboItemSlot( void );

    void SetStatDesc( void );
    void GetUserStatus( IN PLAYER* pPlayer_, OUT USER_STATUS& userStatus_ );
    void SetStatusStatic( bool bCompare_ = false, IN PLAYER* pComparePlayer_ = NULL );
    std::wstring GetStatCompareString( int iBeforeStat_, int iNowStat_, bool bPercent_ = false );
    std::wstring GetStatCompareString( float fBeforeStat, float fNowStat_, bool bPercent_ = false );

    void SetCharImg( int iCharIndex_ );

    int GetItemGrade( const KItem::ITEM_GRADE eGrade_ );
    void SetItemState( void );
    void GetHotItem( OUT std::set< GCITEMUID >& setHotItem_ );  // < 장비 부위, ItemUID >

    void SortInvenItem( void );
    void MakeItemListAndStateList( IN const std::vector< GCITEMUID >& vecUID_, IN const bool bHot_, IN const bool bNew_ );

    bool CheckItemChange( void );
    
public:
    void InsertNewItem( GCITEMUID itemUID_ );
    void ShowItemEffect( bool _bShow );
    void OnClickBtnClose( void );

private:
    KD3DWnd*        m_pkBtnClose;
    KD3DWnd*        m_pkBtnSort;
    KD3DWnd*        m_pkBtnPrev;
    KD3DWnd*        m_pkBtnNext;
    KD3DStatic*     m_pkStaticPage;

    KD3DComboBox*   m_pkItemCombo;

    KD3DStatic*     m_pkStaticBaseTitle;
    KD3DStatic*     m_pkStaticAttributeTitle;
    KD3DStatic*     m_pkStaticRank;
    KD3DStatic*     m_pkStaticBaseDesc;
    KD3DStatic*     m_pkStaticAttributeDesc;
    KD3DStatic*     m_pkStaticBaseStat;
    KD3DStatic*     m_pkStaticAttributeStat;

    KSafeArray< KGCGameInvenItemSlot*, NUM_ITEM_SLOT > m_apkItemSlot;
    KSafeArray< KGCGameInvenEquipSlot*, NUM_EQUIP_SLOT > m_apkEquipSlot;
    KSafeArray< KD3DWnd*, GC_CHAR_NUM >  m_apkCharImg;

    KSafeArray< KD3DWnd*, NUM_GRADE >    m_apkBtnGradeDefault;
    KSafeArray< KD3DWnd*,  NUM_GRADE >    m_apkBtnGradeActive;
    KSafeArray< KD3DStatic*,  NUM_GRADE > m_apkStaticGrade;

    std::vector< CParticleEventSeqPTR >   m_vecHotEquipParticle;
    
private:
    SUserInfo               m_kBeforeEquip;
    USER_STATUS             m_kBeforeStatus;
    std::vector<GCITEMUID>  m_vecInvenItem;
    std::vector< std::pair<bool, bool> > m_vecItemState;    // Hot/New 표시 

    int             m_iCurPage;
    int             m_iTotalPage;

    PLAYER*         m_pPlayer;
};

DEFINE_WND_FACTORY( KGCGameInventory );
DECLARE_WND_FACTORY( KGCGameInventory );
DECLARE_WND_FACTORY_NAME( KGCGameInventory );
