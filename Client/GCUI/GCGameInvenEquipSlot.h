#pragma once

class KGCGameInvenEquipSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGameInvenEquipSlot );
    /// Default constructor
    KGCGameInvenEquipSlot( void );
    /// Default destructor
    virtual ~KGCGameInvenEquipSlot( void );
    /// Copy constructor
    KGCGameInvenEquipSlot( const KGCGameInvenEquipSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGameInvenEquipSlot& operator=( const KGCGameInvenEquipSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DWnd*        m_pkBack;
    KGCItemImgWnd*  m_pkItemImg;
    KItem*          m_pItem;
    
    bool            m_bMouseBound;

public:
    void SetItemData( GCITEMUID itemUID_ );
    void Clear( void );
    void OnClick( void );
    KItem* GetEquipItem( void ) { return m_pItem; }

private:
    void SetMouseBound( bool bSet_ );

};

DEFINE_WND_FACTORY( KGCGameInvenEquipSlot );
DECLARE_WND_FACTORY( KGCGameInvenEquipSlot );
DECLARE_WND_FACTORY_NAME( KGCGameInvenEquipSlot );
