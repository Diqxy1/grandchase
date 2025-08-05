#pragma once

class KGCCharIconWnd;

class KGCAgitObjectSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    enum { TYPE_OBJ = 0, TYPE_CHAR = 1 };   // 어떤 배치에 사용???

public:
    DECLARE_CLASSNAME( KGCAgitObjectSlot );
    /// Default constructor
    KGCAgitObjectSlot( void );
    /// Default destructor
    virtual ~KGCAgitObjectSlot( void );
    /// Copy constructor
    KGCAgitObjectSlot( const KGCAgitObjectSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitObjectSlot& operator=( const KGCAgitObjectSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd*    m_pkDefaultFrame;
    KD3DWnd*    m_pkActiveFrame;
    KD3DWnd*    m_pkBtnSell;
    KD3DWnd*    m_pkBtnSetting;

    GCITEMID                m_ItemID;
    int                     m_iCount;
    
    int                     m_iCharType;
    bool                    m_bLock;

    int                     m_iSlotType;
    GCDeviceTexture*        m_pkItemTex;

    KGCCharIconWnd*         m_pkCharIcon;
    KD3DWnd*                m_pkLockIcon;

public:
    void SetObjectSlot( GCITEMID itemID_, int iCount_ );
    GCITEMID GetIObjectID( void ) { return m_ItemID; }

    void SetCharSlot( int iCharType_, bool bLock_ );
    int  GetCharType( void ) { return m_iCharType; }
    bool IsCharLock( void ) { return m_pkLockIcon->IsRenderOn(); }

    void SetActive( bool bActive_ );
    bool IsActive( void ) { return m_pkActiveFrame->IsRenderOn(); }
    int  GetSlotType( void ) { return m_iSlotType; }
   
    void OnClick( void );
    void OnSell( void );
    void OnSetting( void );
    void Clear( void );
};

DEFINE_WND_FACTORY( KGCAgitObjectSlot );
DECLARE_WND_FACTORY( KGCAgitObjectSlot );
DECLARE_WND_FACTORY_NAME( KGCAgitObjectSlot );
