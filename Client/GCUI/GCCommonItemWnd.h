#pragma once

class KGCItemImgWnd;
class KGCUINumber;

class KGCCommonItemWnd : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCommonItemWnd );
    /// Default constructor
    KGCCommonItemWnd( void );
    /// Default destructor
    virtual ~KGCCommonItemWnd( void );
    /// Copy constructor
    KGCCommonItemWnd( const KGCCommonItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCommonItemWnd& operator=( const KGCCommonItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void SetItem( KItem* kItem, KPetInfo* pPetInfo = NULL );
    void SetItem( KPetInfo* pPetInfo );
    void SetItem(KItem* kItem, char cGrade, KPetInfo* pPetInfo = NULL);
    void SetItem( GCItem* pItem );
    void SetItemCount( int nCount );
    void SetItemStrong( bool bStrong, int nStrong );
    void SetEnchantLevel( KItem* kItem );
    void SetCoordiCompose( bool bShow );

    void SelectedItem( bool bShow );
    void ResetItem();

    void SetColor( IN const DWORD dwColor_ );
    void SetEquip( KItem* kItem );

    GCITEMID GetItemID() { return m_itemID; }
    GCITEMUID GetItemUID() { return m_itemUID; }

public:
    KGCItemImgWnd*  m_pItemImgWnd;
    KD3DSizingBox*  m_pkGradeFrame[KItem::GRADE_NUM];
    KD3DWnd*        m_pkSelItemImg;

    KGCUINumber*    m_pkSmallDurationNum;
    KGCUINumber*    m_pkStrongNum;
    KGCUINumber*    m_pkEquipEnchantItemLv;

    KD3DWnd*        m_pkCoordiComposeImg;
    KD3DWnd*        m_pkEquipImg;   // 장착중 표시

public:
    GCITEMID        m_itemID;
    GCITEMUID       m_itemUID;
};

DEFINE_WND_FACTORY( KGCCommonItemWnd );
DECLARE_WND_FACTORY( KGCCommonItemWnd );
DECLARE_WND_FACTORY_NAME( KGCCommonItemWnd );
