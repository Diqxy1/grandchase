#pragma once

class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;
class KPet;

class KGCRoomCharWndS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomCharWndS6 );
    /// Default constructor
    KGCRoomCharWndS6( void );
    /// Default destructor
    virtual ~KGCRoomCharWndS6( void );
    /// Copy constructor
    KGCRoomCharWndS6( const KGCRoomCharWndS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomCharWndS6& operator=( const KGCRoomCharWndS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum
    {
        MAX_CHAR_NAME_LEN   = 8,
        VIEWPORT_OFFSET_X   = 18,
        VIEWPORT_OFFSET_Y   = 26,
        VIEWPORT_WIDTH      = 197,
        VIEWPORT_HEIGHT     = 229,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnDestroy( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

public:
    void SetPreviewWindow( PLAYER* pPlayer );
    void SetChar();
    void UpdateMeshObject();
    void DeleteMeshObject();
    void ChangeCharWndZoom( bool bZoom );
    void ProcessCharWndAngle();
    void UpdatePet();
    void SetTitleItemID( int iTitleItemID );

    void ReloadMotion();

public:

    KD3DWnd*    m_pkImgRenderPos;   //render_position
	KD3DWnd*    m_pkBack_Color;

    PLAYER      m_Player;
    KPet*		m_pkPet;

    GCObject*           m_pMeshKOGRoom;
    D3DVIEWPORT9        m_viewport;
    D3DXMATRIX          m_matProject;

    GCDeviceMotionFRM*  m_pShopMotion;
    GCDeviceTexture*    m_pTexTitleItem;

    GCDeviceMotionFRM*  m_pWaitMotion;
    GCDeviceMotionFRM*  m_pActionMotion;

    int					m_iMotionCount;
    int                 m_cFrame;
    float				m_fCharX;

    POINT               m_ptStartMouse;
    float               m_fStartAngle;
    float               m_fAngleMove;
    float				m_fZoom;
    bool				m_bAngleChanged;

    int                 m_iExtraCharNum;
};

DEFINE_WND_FACTORY( KGCRoomCharWndS6 );
DECLARE_WND_FACTORY( KGCRoomCharWndS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomCharWndS6 );
