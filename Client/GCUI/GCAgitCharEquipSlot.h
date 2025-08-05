#pragma once

class KGCAgitCharEquipSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitCharEquipSlot );
    /// Default constructor
    KGCAgitCharEquipSlot( void );
    /// Default destructor
    virtual ~KGCAgitCharEquipSlot( void );
    /// Copy constructor
    KGCAgitCharEquipSlot( const KGCAgitCharEquipSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitCharEquipSlot& operator=( const KGCAgitCharEquipSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DWnd*        m_pkFrame;
    KGCItemImgWnd*  m_pkItemImg;

    bool            m_bEquip;
    GCITEMUID       m_ItemUID;
    DWORD           m_dwSlotPosition;

public:
    void SetSlot( GCITEMUID itemUID_ );
    GCITEMUID GetItemUID( void ) { return m_ItemUID; }
    
    void SetSlotPosition( DWORD dwSlotPos_ );
    DWORD GetSlotPosition( void );
    
    void OnClick( void );
    void Clear( void );

};

DEFINE_WND_FACTORY( KGCAgitCharEquipSlot );
DECLARE_WND_FACTORY( KGCAgitCharEquipSlot );
DECLARE_WND_FACTORY_NAME( KGCAgitCharEquipSlot );
