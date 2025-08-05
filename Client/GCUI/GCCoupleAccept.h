#pragma once

class KD3DStatic;
class KGCCheckBox;
class KGCSizingThreeBox;
class KGC2DObject;
class KGCCoupleAccept : public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCoupleAccept );

	KGCCoupleAccept(void);
	virtual ~KGCCoupleAccept(void);
	/// Copy constructor
	KGCCoupleAccept( const KGCCoupleAccept& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCoupleAccept& operator=( const KGCCoupleAccept& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum GCCoupleAcceptState {
		GCCA_ACCEPT = 0,
		GCCA_RINGCHANGE,

		GCCA_NUM,
	};
public:
	virtual void OnCreate( void );
	virtual void PostChildDraw( void );
	virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
	virtual void OnDestroy( void );

	void SetData( std::wstring strRequesterNick, std::wstring strMessage, DWORD dwRingID, DWORD dwUserUID );

	void SetUse( GCCoupleAcceptState eState );

private:
	void DrawItemInfo();
	void OnClickAgree();
	void OnClickDisagree();

	void SendAccept( bool bAccept );

private:
	GCCoupleAcceptState	m_eCoupleState;

	KSafeArray<KD3DWnd*,GCCA_NUM> m_pkTitle;
	KD3DWnd*			m_pkRingSlot;

	KD3DStatic*			m_pkCoupleString;
	KD3DStatic*			m_pkMessage;

	KD3DStatic*			m_pkProposeMessage;

	KD3DWnd*			m_pkAgree;
	KD3DWnd*			m_pkDisagree;

	KGCSizingThreeBox*	m_pkDayGuage;

	KGCCheckBox*		m_pkDisagreeToday;
	KGCCheckBox*		m_pkDisagreeHuman;
	KD3DStatic*			m_pkDisagreeTodayStatic;
	KD3DStatic*			m_pkDisagreeHumanStatic;

	DWORD				m_dwRingID;
	KGC2DObject			m_kRingImage;
	DWORD				m_dwProposeUserUID;

	bool				m_bFirst;
	int					m_iGuageFullWidth;
	int					m_iDenyCounter;
};

DEFINE_WND_FACTORY( KGCCoupleAccept );
DECLARE_WND_FACTORY( KGCCoupleAccept );
DECLARE_WND_FACTORY_NAME( KGCCoupleAccept );
