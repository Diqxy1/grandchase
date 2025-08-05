#pragma once

class KGCDramaPhoto : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDramaPhoto );
    /// Default constructor
    KGCDramaPhoto( void );
    /// Default destructor
    virtual ~KGCDramaPhoto( void );
    /// Copy constructor
    KGCDramaPhoto( const KGCDramaPhoto& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDramaPhoto& operator=( const KGCDramaPhoto& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostDraw( void );

    void SetDramaPhoto( SDramaPhoto& sDramaPhoto );

public:
    GCDeviceTexture*    m_pDramaTexture;

    //그림 텍스쳐의 좌표
    D3DXVECTOR2 m_vImageTexStartAxis;
    D3DXVECTOR2 m_vImageTexEndtAxis;

    //그림 사이즈
    int nXSize;
    int nYSize;

};

DEFINE_WND_FACTORY( KGCDramaPhoto );
DECLARE_WND_FACTORY( KGCDramaPhoto );
DECLARE_WND_FACTORY_NAME( KGCDramaPhoto );
