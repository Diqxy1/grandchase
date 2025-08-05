#pragma once

class KGC2DObject;
class KGCCouplePropose : public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCouplePropose );

	KGCCouplePropose(void);
	virtual ~KGCCouplePropose(void);
	/// Copy constructor
	KGCCouplePropose( const KGCCouplePropose& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCouplePropose& operator=( const KGCCouplePropose& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum GCCoupleProposeState {
		CP_PROPOSE = 0,
		CP_RINGCHANGE,

		CP_NUM,
	};

public:
	virtual void OnCreate( void );
	virtual void PostChildDraw( void );
	virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    virtual void OnDestroyComplete();

	void SetUse( GCCoupleProposeState eState );
	void SetProposeUserUID( DWORD dwUID, std::wstring strNickName );

private:
	void DrawItemInfo();
	void OnClickRingChange( bool bRight );
	void OnClickOk();
	void OnClickCancle();

private:
	DWORD m_dwPrePartnerUID;
	GCCoupleProposeState m_eCoupleState;
	KD3DWnd*	m_pkRingSlot;

	KD3DStatic*	m_pkCoupleString;
	KD3DStatic*	m_pkMessage;

	KD3DEdit*	m_pkProposeMessage;

	KD3DWnd*	m_pkOk;
	KD3DWnd*	m_pkCancle;
	KD3DWnd*	m_pkRingRight;
	KD3DWnd*	m_pkRingLeft;

	KSafeArray<KD3DWnd*,CP_NUM> m_pkTitle;

	KGC2DObject	m_kRingImage;
	int			m_iCurrentRing;
	std::vector<KSimpleItem> m_vecRingList;
};

DEFINE_WND_FACTORY( KGCCouplePropose );
DECLARE_WND_FACTORY( KGCCouplePropose );
DECLARE_WND_FACTORY_NAME( KGCCouplePropose );
