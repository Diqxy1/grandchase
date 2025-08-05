#pragma once

class GCDeviceMeshP3M;
class GCDeviceMotionFRM;
class GCObject;

class KGCCoordiViewChar : public KD3DWnd, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoordiViewChar );
	/// Default constructor
	KGCCoordiViewChar( void );
	/// Default destructor
	virtual ~KGCCoordiViewChar( void );
	/// Copy constructor
	KGCCoordiViewChar( const KGCCoordiViewChar& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoordiViewChar& operator=( const KGCCoordiViewChar& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void PostDraw( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void OnCreateComplete( void );
	virtual void OnDestroy();

private:
	KD3DWnd*				 m_pkBackground;
	KD3DWnd*				 m_pkRenderPosition;
	std::map<DWORD, GCItem*> m_mapItems;
	GCItem*					 m_pItem;

public:
	void ResetPlayer();
	void OnViewBasicClick();
	void SetItem( GCItem* pItem);
	void RemoveItem( GCItem* pItem );
	std::map<DWORD, GCItem*> GetItemMap() { return m_mapItems; }
	std::vector<GCITEMID> GetEquipedItemIDVector();
	
private:
	void InitPreview();
	void RenderPreview( void );
	void FrameMove_Preview( void );
	void ChangeCharWndZoom( bool bZoom );
	void ProcessCharWndAngle();
	void InitCharacterInfo();
	void SetView();
	void SetChar(int iCharType);
	void UpdateMeshObject();
	void DeleteMeshObject();
	void ReloadMotion();

private:
	enum
	{
		VIEWPORT_OFFSET_X   = 452,
		VIEWPORT_OFFSET_Y   = 57,
		VIEWPORT_WIDTH       = 202,
		VIEWPORT_HEIGHT      = 338,
	};

	PLAYER					m_Player;
	GCObject*				m_pMeshFootStage;
	D3DVIEWPORT9			m_viewport;
	D3DXMATRIX				m_matProject;
	GCDeviceMotionFRM*		m_pShopMotion;
	GCDeviceTexture*		m_pkBackTexture;

	KSafeArray<GCDeviceMotionFRM*,GC_CHAR_NUM * NUM_JOB_LEVEL> m_pWaitMotion;
	KSafeArray<GCDeviceMotionFRM*,GC_CHAR_NUM * NUM_JOB_LEVEL> m_pActionMotion;

	int                 m_iCurCharType;
	int					m_iCurCharJob;
	int					m_iMotionCount;
	int                 m_cFrame;
	float				m_fCharX;
	POINT           	m_ptStartMouse;
	float               m_fStartAngle;
	float               m_fStartStageAngle;
	float               m_fAngleMove;
	float               m_fStageAngleMove;
	float				m_fZoom;
	bool				m_bAngleChanged;
	int                 m_iExtraCharNum;
	int                 m_iCurrentCharacter;
};

DEFINE_WND_FACTORY( KGCCoordiViewChar );
DECLARE_WND_FACTORY( KGCCoordiViewChar );
DECLARE_WND_FACTORY_NAME( KGCCoordiViewChar );
