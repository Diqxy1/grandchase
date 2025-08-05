#pragma once

class KGCAgitMapSlot;

class KGCAgitMove : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { BTN_PREV = 0, BTN_NEXT = 1, BTN_CLOSE = 2, BTN_WATCH = 3, BTN_MOVE = 4, NUM_BTN };
    enum { NUM_SLOT = 4 };
    enum { NUM_FLOWER_POT = 3 };

public:
    DECLARE_CLASSNAME( KGCAgitMove );
    /// Default constructor
    KGCAgitMove( void );
    /// Default destructor
    virtual ~KGCAgitMove( void );
    /// Copy constructor
    KGCAgitMove( const KGCAgitMove& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitMove& operator=( const KGCAgitMove& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    KSafeArray< KD3DWnd*, NUM_BTN >     m_apkBtn;
    KD3DStatic*     m_pkStaticMapInfo;
    KD3DStatic*     m_pkStaticPage;

    KSafeArray< KGCAgitMapSlot*, NUM_SLOT > m_apkSlot;
    KSafeArray< KD3DWnd*, NUM_FLOWER_POT >  m_apkIconFlowerPot;
    KD3DSizingBox*          m_pkSelectFrame;
    GCITEMID                m_MaterialID;   // »≠∆ÛID
    std::vector< KAgitMap > m_vecMapInfo;   // ∆«∏≈ ∏  ¡§∫∏ 

    int     m_iCurPage;
    int     m_iTotalPage;
    int     m_iCurSlot;
    
public:
    void InitUI( KEVENT_AGIT_MAP_CATALOGUE_ACK& catalog_ );

private:
    void InitPage( void );
    void SetPageStatic( void );
    void SetMapSlot( int iPage_ );
    void SetFlowerPotIcon( void );

    void OnClickSlot( int iIndex_ );
    void OnClickPage( int iBtnType_ );
    void OnWatch( void );
    void OnMove( void );
    void OnClose( void );

};

DEFINE_WND_FACTORY( KGCAgitMove );
DECLARE_WND_FACTORY( KGCAgitMove );
DECLARE_WND_FACTORY_NAME( KGCAgitMove );
