#pragma once


class KGCNewsFrame: public KD3DWnd, public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCNewsFrame );
	/// Default constructor
	KGCNewsFrame( void );
	/// Default destructor
	virtual ~KGCNewsFrame( void );
	/// Copy constructor
	KGCNewsFrame( const KGCNewsFrame& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCNewsFrame& operator=( const KGCNewsFrame& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreateComplete( void );
	virtual void OnCreate( void );
	virtual void OnInitialize();
	virtual void FrameMoveInEnabledState();

	virtual void PostChildDraw();

    void NewsBtnInit();

	enum ANI_STATE {
		NOTHING = 0,
		PROCESSING = 1,
		CATCHFOCUS = 2
	};

	enum {
		MAX_NEWS = 9,
#if defined(NATION_CHINA)
		LOTATION_TIME = GC_FPS_LIMIT*4,
#else
		LOTATION_TIME = GC_FPS_LIMIT*2,
#endif
		ANI_FRAME_TIME = GC_FPS_LIMIT
	};

	

private:

	void UpDateFrameTime();
	bool SetNewsPage();
	void OnCatchFocus(int iNextIndex_ )		{ m_bCatchFocus = true; m_iNextFocusIndex = iNextIndex_; if (m_iMaxNews >1)m_eAniState = PROCESSING; }
	void OffCatchFocus()					{ m_bCatchFocus = false; if (m_iMaxNews >1)m_eAniState = PROCESSING; }
    void OnClickNews( void );
	void OnClickNewsLink( void );
	void LoadWebLink();
    void OnClickNewsToDefaultBrowser( void );

	GCDeviceTexture*	m_pkServerNews[MAX_NEWS];
	KD3DWnd*			m_pkbntNews[MAX_NEWS];
	KD3DWnd*			m_pkNewsFocus;


	int m_iCurrentFocusIndex;
	int m_iNextFocusIndex;

	bool m_bCatchFocus;
	UINT m_iCntLotationFrame;
	UINT m_iCntAniFrame;
	ANI_STATE m_eAniState;

	D3DXVECTOR4         m_vTextureCoord;
	D3DXVECTOR2			m_vWindowPos;

	int m_iMaxNews;

	std::vector<UINT> m_vecPossibleIndex;
	std::map<int,std::string> m_mapWebLink;

    std::vector<int> m_vecNewNum;

};

DEFINE_WND_FACTORY( KGCNewsFrame );
DECLARE_WND_FACTORY( KGCNewsFrame );
DECLARE_WND_FACTORY_NAME( KGCNewsFrame );
