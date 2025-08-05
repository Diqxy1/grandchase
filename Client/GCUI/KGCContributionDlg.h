#pragma once

#define		MAX_CONTRIBUTION_EVENT	5
//#define		MAX_CONTRIBUTION_EVENT	17
//#define		MAX_CONTRIBUTION_RANKER	5

//class KGCWelcomeBackReward;

class KGCContributionDlg :	public KD3DDialog,
							public KActionListener
{
public:


	DECLARE_CLASSNAME( KGCContributionDlg );
	/// Default constructor
	KGCContributionDlg( void );
	/// Default destructor
	virtual ~KGCContributionDlg( void );
	/// Copy constructor
	KGCContributionDlg( const KGCContributionDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCContributionDlg& operator=( const KGCContributionDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    //virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
	virtual void PostChildDraw( void );

    // UI
	KD3DWnd* m_pkContributionBtn;
	KD3DWnd* m_pkCloseBnt;

	KSafeArray<KD3DWnd*,       MAX_CONTRIBUTION_EVENT > m_pkContributionEventStamp;       // 별
	KSafeArray<KD3DWnd*,       MAX_CONTRIBUTION_EVENT > m_pkContributionEventStampActive; // 별 ( 이벤트가 활성화 )
	KSafeArray<KD3DStatic*,    MAX_CONTRIBUTION_EVENT > m_pkContributionEventName;        // 이벤트 내용 표시
	KSafeArray<KD3DSizingBox*, MAX_CONTRIBUTION_EVENT > m_pkContributionEventBar;         // 테두리
	KSafeArray<KD3DSizingBox*, MAX_CONTRIBUTION_EVENT > m_pkContributionEventBarActive;   // 테두리 ( 이벤트가 활성화 )

	KD3DSizingBox* m_pkEventBarBack;
	
public:
	void OnExit();
	void OnClickDonationBtn();
	void Update();

};


DEFINE_WND_FACTORY( KGCContributionDlg );
DECLARE_WND_FACTORY( KGCContributionDlg );
DECLARE_WND_FACTORY_NAME( KGCContributionDlg );