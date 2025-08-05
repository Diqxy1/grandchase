#pragma once

class KGCAgitCharItemSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitCharItemSlot );
    /// Default constructor
    KGCAgitCharItemSlot( void );
    /// Default destructor
    virtual ~KGCAgitCharItemSlot( void );
    /// Copy constructor
    KGCAgitCharItemSlot( const KGCAgitCharItemSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitCharItemSlot& operator=( const KGCAgitCharItemSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DWnd*        m_pkDefaultFrame;
    KD3DWnd*        m_pkActiveFrame;
    KGCItemImgWnd*  m_pkItemImg;

    GCItem*         m_pItemData;
    KItem*          m_pInvenItem;
    bool            m_bEquip;

public:
    void SetSlot( GCITEMUID itemUID_ );
    void Onclick( void );
    void SetEquipState( bool bEquip_ );
    bool GetEquipState( void );
    void Clear( void );
    
    GCItem* GetItemData( void );
    KItem* GetInvenItem( void );
    GCITEMID GetItemID( void );
    GCITEMUID GetItemUID( void );
    
};

DEFINE_WND_FACTORY( KGCAgitCharItemSlot );
DECLARE_WND_FACTORY( KGCAgitCharItemSlot );
DECLARE_WND_FACTORY_NAME( KGCAgitCharItemSlot );
