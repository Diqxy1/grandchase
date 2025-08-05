#pragma once

class KGCItemPopupMenu : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCItemPopupMenu );
    /// Default constructor
    KGCItemPopupMenu( void );
    /// Default destructor
    virtual ~KGCItemPopupMenu( void );
    /// Copy constructor
    KGCItemPopupMenu( const KGCItemPopupMenu& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemPopupMenu& operator=( const KGCItemPopupMenu& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void OnMoveDepot();
    void OnSellItem();
    void OpenPopupMenu( GCITEMUID _itemUID, D3DXVECTOR2 vecPos );
   
    void InsertDepotItem( KItem* pkItem, GCITEMUID _itemUID );

    KItem&  GetItem() { return m_kItem; }

public:
    KD3DWnd* m_pkBtnMoveDepot;
    KD3DWnd* m_pkBtnSellItem;

    GCITEMUID   m_itemUID;
    KItem       m_kItem;
};

DEFINE_WND_FACTORY( KGCItemPopupMenu );
DECLARE_WND_FACTORY( KGCItemPopupMenu );
DECLARE_WND_FACTORY_NAME( KGCItemPopupMenu );
