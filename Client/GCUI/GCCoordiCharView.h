#ifdef USE_COORDI_SHOP // Kawan>

#pragma once

class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;

class KGCCoordiCharView : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCoordiCharView );
    /// Default constructor
    KGCCoordiCharView( void );
    /// Default destructor
    virtual ~KGCCoordiCharView( void );
    /// Copy constructor
    KGCCoordiCharView( const KGCCoordiCharView& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCoordiCharView& operator=( const KGCCoordiCharView& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnDestroy( void );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );
   
    void ChangeCharWndZoom( bool bZoom );
    void ProcessCharWndAngle( void );

    void SetCoordiView( int iCharType_ );
    void UpdateMeshObject( void );
    void DeleteMeshObject( void );

    void SetChangeCharacter( int iCharacter_ );

    void OnDisrobe( void );
    void OnDress( void );
    
    int GetCurrentChar( void ) { return m_iCurrentCharacter; }
    SCharInfo GetCurrentCharInfo( void );
    
    //void SetPlayer( PLAYER viewPlayer ) { m_Player = viewPlayer; }
    //PLAYER GetPlayer( void ) { return m_Player; }
    void SetPlayer( PLAYER* viewPlayer ) { m_Player = viewPlayer; }
    PLAYER* GetPlayer( void ) { return m_Player; }
    void ResetPlayer( void );

    void ResetUIforResolution( void );
    bool IsShowDefault( void ) { return m_bShowDefault; }

    void ReloadMotion();

private:
    enum
    {
        VIEWPORT_OFFSET_X   = 30,
        VIEWPORT_OFFSET_Y   = 21,
        VIEWPORT_WIDTH      = 220,
        VIEWPORT_HEIGHT     = 298,
    };

    KD3DWnd*            m_pkBtnCharSelect;
    KD3DWnd*            m_pkBtnDisrobe;
    KD3DWnd*            m_pkBtnDress;

    PLAYER*             m_Player;
    GCObject*           m_pMeshFootStage;
    D3DVIEWPORT9        m_viewport;
    D3DXMATRIX          m_matProject;
	D3DXMATRIX			m_matScreenProj;
    GCDeviceMotionFRM*  m_pShopMotion;

    GCDeviceTexture*    m_pkBackTexture;


    GCDeviceMotionFRM*  m_pWaitMotion;
    GCDeviceMotionFRM*  m_pActionMotion;

    

    int					m_iMotionCount;
    int                 m_cFrame;
    float				m_fCharX;

    POINT               m_ptStartMouse;
    float               m_fStartAngle;
    float               m_fStartStageAngle;
    float               m_fAngleMove;
    float               m_fStageAngleMove;
    float				m_fZoom;
    bool				m_bAngleChanged;

    int                 m_iExtraCharNum;
    int                 m_iCurrentCharacter;
    bool                m_bShowDefault;
};

DEFINE_WND_FACTORY( KGCCoordiCharView );
DECLARE_WND_FACTORY( KGCCoordiCharView );
DECLARE_WND_FACTORY_NAME( KGCCoordiCharView );

#endif