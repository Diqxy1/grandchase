#pragma once

class KGCAgitMapSlot : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitMapSlot );
    /// Default constructor
    KGCAgitMapSlot( void );
    /// Default destructor
    virtual ~KGCAgitMapSlot( void );
    /// Copy constructor
    KGCAgitMapSlot( const KGCAgitMapSlot& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitMapSlot& operator=( const KGCAgitMapSlot& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    KAgitMap            m_MapInfo;
    KD3DSizingBox*      m_pkMainFrame;
    KD3DSizingBox*      m_pkPriceFrame;
    GCDeviceTexture*    m_pMapTex;

public:
    void SetMapInfo( KAgitMap& info_ );
    KAgitMap& GetMapInfo( void )    { return m_MapInfo; }

    void Clear( void );

private:
    void OnClick( void );
    
};
DEFINE_WND_FACTORY( KGCAgitMapSlot );
DECLARE_WND_FACTORY( KGCAgitMapSlot );
DECLARE_WND_FACTORY_NAME( KGCAgitMapSlot );
