#pragma once

class KD3DView;
class KGCSizingThreeBox;
class KGCCoupleWnd;
class KGCCoupleInventory;

class KGCCoupleViewer : public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoupleViewer );

	KGCCoupleViewer(void);
	virtual ~KGCCoupleViewer(void);
	/// Copy constructor
	KGCCoupleViewer( const KGCCoupleViewer& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleViewer& operator=( const KGCCoupleViewer& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		CV_COUPLE_WINDOW = 0,
		CV_COUPLE_INVEN = 1,

		CV_COUPLE_TAB,
	};

	enum{
		CV_NORMAL_HEART = 0,
		CV_ACTIVE_HEART = 1,

		CV_HEART,
	};

	enum{
		CV_VIEWER_MY_COUPLE,
		CV_VIEWER_OTHER_COUPLE,
	};

public:
	//virtual void Initialize();
	virtual void OnCreate( void );
	virtual void OnDestroy( void );
	//virtual void PostChildDraw( void );
	virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );

	void Update();
	void OnClickTab( int iTabIndex );
	void SetCoupleUID( DWORD dwLeftUID, DWORD dwRightUID );

	void UseOtherCoupleView();
	void UseMyCoupleView();

private:
	bool m_bMyCouple;
	DWORD m_dwLeftUID;
	DWORD m_dwRightUID;
	bool m_bFirst;
	int m_iCoupleDayFullLength;
	//KD3DView*	m_pkView;
	KD3DWnd*	m_pkClose;

	KSafeArray<KD3DWnd*,CV_COUPLE_TAB> m_pkActiveTab;
	KSafeArray<KD3DWnd*,CV_COUPLE_TAB> m_pkTab;
	KSafeArray<KD3DWnd*,CV_COUPLE_TAB> m_pkTitle;

	KSafeArray<KD3DWnd*,CV_HEART> m_pkHeart;

	KGCSizingThreeBox*	m_pkCoupleDayGuage;
	KD3DWnd*			m_pkCoupleDayGuageBg;
	KGCCoupleWnd*		m_pkCoupleWnd;
	KGCCoupleInventory*	m_pkCoupleInven;

	int			m_iCurrentTab;
};

DEFINE_WND_FACTORY( KGCCoupleViewer );
DECLARE_WND_FACTORY( KGCCoupleViewer );
DECLARE_WND_FACTORY_NAME( KGCCoupleViewer );
