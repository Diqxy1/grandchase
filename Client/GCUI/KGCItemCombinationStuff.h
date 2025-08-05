#pragma once

class KGCItemCombinationStuff : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCItemCombinationStuff );
    /// Default constructor
    KGCItemCombinationStuff( void );
    /// Default destructor
    virtual ~KGCItemCombinationStuff( void );
    /// Copy constructor
    KGCItemCombinationStuff( const KGCItemCombinationStuff& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCItemCombinationStuff& operator=( const KGCItemCombinationStuff& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

public:
    void SetItemInfo( const GCITEMID itemID_, const GCITEMUID itemUID_ = 0 );
    void SetShowItem( bool _bShow );
    void SetSelect( bool bSelect );
    bool IsSelectedSlot() { return m_bSelect; }
    void SetBlockWnd( bool _bShow );
    GCITEMID GetItemID();
    GCITEMUID GetItemUID( void ) { return m_ItemUID; }

public:
    KGCItemImgWnd*  m_pkItemImg;
    KD3DWnd*        m_pkSeleted;
    KD3DWnd*        m_pkItemActiveFrame;
    KD3DWnd*        m_pkBlockWnd;

public:
    GCITEMID			m_ItemID;
    GCITEMUID           m_ItemUID;
    int					m_nSlotIndex;
    bool                m_bSelect;

};

DEFINE_WND_FACTORY( KGCItemCombinationStuff );
DECLARE_WND_FACTORY( KGCItemCombinationStuff );
DECLARE_WND_FACTORY_NAME( KGCItemCombinationStuff );
