#pragma once

class KGCItemImgWnd;

class KGCGameInvenItemSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGameInvenItemSlot );
    /// Default constructor
    KGCGameInvenItemSlot( void );
    /// Default destructor
    virtual ~KGCGameInvenItemSlot( void );
    /// Copy constructor
    KGCGameInvenItemSlot( const KGCGameInvenItemSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGameInvenItemSlot& operator=( const KGCGameInvenItemSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd*        m_pkBack;
    KD3DWnd*        m_pkActive;
    KD3DWnd*        m_pkNewIcon;
    KD3DWnd*        m_pkHotIcon;
    KGCItemImgWnd*  m_pkItemImg;
    
    KItem*          m_pItem;

    bool            m_bHot;
    bool            m_bNew;

public:
    void Clear( void );
    void SetItemInfo( GCITEMUID itemUID_, bool bNew_ = false, bool bHot_ = false );
    
    void SetActiveFrame( bool bSet_ );
    void OnClickSlot( void );

    bool IsHotItem() { return m_bHot; }
    bool IsNewItem() { return m_bNew; }

    GCITEMID GetItemID( void );
    GCITEMUID GetItemUID( void );

};

DEFINE_WND_FACTORY( KGCGameInvenItemSlot );
DECLARE_WND_FACTORY( KGCGameInvenItemSlot );
DECLARE_WND_FACTORY_NAME( KGCGameInvenItemSlot );
