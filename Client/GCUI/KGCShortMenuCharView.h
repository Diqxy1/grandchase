#pragma once

class KGCShortMenuCharView : public KD3DWnd,
							 public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCShortMenuCharView );
	/// Default constructor
	KGCShortMenuCharView( void );
	/// Default destructor
	virtual ~KGCShortMenuCharView( void );
	/// Copy constructor
	KGCShortMenuCharView( const KGCShortMenuCharView& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCShortMenuCharView& operator=( const KGCShortMenuCharView& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate();
	virtual void OnDestroy( void );
	virtual void PostChildDraw();
	virtual void FrameMoveInEnabledState( void );
	void SetPreviewWindow( PLAYER* pPlayer );

private:
	void SetChar();
	void UpdateMeshObject();
	void DeleteMeshObject();
	void ChangeCharWndZoom( bool bZoom );
	void ProcessCharWndAngle();
	void UpdatePet();

	void ReloadMotion();

	enum
	{
		VIEWPORT_OFFSET_X   = 18,
		VIEWPORT_OFFSET_Y   = 26,
		VIEWPORT_WIDTH      = 427,
		VIEWPORT_HEIGHT     = 439,
	};

	KD3DWnd* m_pkRenderFrame;

	PLAYER      m_Player;
	KPet*		m_pkPet;

	GCObject*           m_pMeshKOGRoom;
	D3DVIEWPORT9        m_viewport;
	D3DXMATRIX          m_matProject;

	GCDeviceMotionFRM*  m_pCharMotion;
	GCDeviceTexture*    m_pBackgroundTex;

	GCDeviceMotionFRM*  m_pWaitMotion;
	GCDeviceMotionFRM*  m_pActionMotion;
	GCDeviceMotionFRM*	m_pFootholdMotion;

	int					m_iMotionCount;
	int                 m_cFrame;
	float				m_fCharX;

	POINT               m_ptStartMouse;
	float               m_fStartAngle;
	float               m_fAngleMove;
	float				m_fZoom;
	bool				m_bAngleChanged;

	int                 m_iExtraCharNum;

	KGCGlobal::ShortMenuCharInfo m_kCharInfo;
	bool				m_bRemoveWeapon;

};

DEFINE_WND_FACTORY( KGCShortMenuCharView );
DECLARE_WND_FACTORY( KGCShortMenuCharView );
DECLARE_WND_FACTORY_NAME( KGCShortMenuCharView );