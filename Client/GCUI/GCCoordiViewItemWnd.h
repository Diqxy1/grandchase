#pragma once

class KGCItemImgWnd;
class KGCUINumber;

class KGCCoordiViewItemWnd : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCoordiViewItemWnd );
    /// Default constructor
    KGCCoordiViewItemWnd( void );
    /// Default destructor
    virtual ~KGCCoordiViewItemWnd( void );
    /// Copy constructor
    KGCCoordiViewItemWnd( const KGCCoordiViewItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiViewItemWnd& operator=( const KGCCoordiViewItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:

public:
    void SetItem( GCITEMID iItemID, int iType, bool bIsBuy = false );
    void SetItemCustom(GCITEMID iItemID, int iType, bool bIsBuy = false);
    void SetItemPrice( int nPrice );
    void SetItemType( int iType );
    void SelectedItem( bool bShow );
    void ResetItem();
    void SetColor( IN const DWORD dwColor_ );

    //Sistema de Skin de Dano
    void SetFontSkinButtons( int iType );
    bool IsEquipedItem();
    void OnClickBuy();

    GCITEMID GetItemID() { return m_itemID; }
    int GetType() { return m_iType; }

public:
    KGCItemImgWnd*  m_pItemImgWnd;
    KD3DSizingBox*  m_pkGradeFrame[KItem::GRADE_NUM];
    KD3DWnd*        m_pkSelItemImg;
    KGCUINumber*    m_pkPriceNum;

    KD3DWnd*        m_pkCoordiVIPImg;
    KD3DWnd*        m_pkCoordiDiamondImg;
    KD3DWnd*        m_pkEquipImg;

    //Sistema de Skin de Dano
    KD3DWnd*        m_pkEquipBtn;
    KD3DWnd*        m_pkUnEquipBtn;
    KD3DWnd*        m_pkBuyBtn;

public:
    GCITEMID        m_itemID;
    int             m_iType;
    bool            m_bActive;
    bool            m_bIsBuy;
};

DEFINE_WND_FACTORY( KGCCoordiViewItemWnd );
DECLARE_WND_FACTORY( KGCCoordiViewItemWnd );
DECLARE_WND_FACTORY_NAME( KGCCoordiViewItemWnd );
