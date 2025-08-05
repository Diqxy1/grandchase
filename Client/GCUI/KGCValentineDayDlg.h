//#pragma once
//
//class KGCItemImgWnd;
//
//class KGCValentineDayDlg :	public KD3DDialog,
//							public KActionListener
//{
//public:
//	DECLARE_CLASSNAME( KGCValentineDayDlg );
//	/// Default constructor
//	KGCValentineDayDlg( void );
//	/// Default destructor
//	virtual ~KGCValentineDayDlg( void );
//	/// Copy constructor
//	KGCValentineDayDlg( const KGCValentineDayDlg& )
//	{ ASSERT( false && "Do not use copy constructor!" ); }
//	/// Assignment operator
//	const KGCValentineDayDlg& operator=( const KGCValentineDayDlg& )
//	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
//
//protected:
//	virtual void OnCreate( void );
//	virtual void OnCreateComplete( void );
//	virtual void ActionPerformed( const KActionEvent& event );
//	virtual void OnDestroyComplete( void );
//	virtual void FrameMoveInEnabledState( void );
//	virtual void PostChildDraw( void );
//
//public:
//	KD3DWnd* m_pkOkBtn;
//	KD3DWnd* m_pkCancelBtn;
//	KD3DWnd* m_pkUpArrowBtn;
//	KD3DWnd* m_pkDownArrowBtn;
//	KD3DWnd* m_pkPollCountView; // 단순히 위치만을 알기 위해 필요..
//	KD3DStatic* m_pkStaticTitle;
//	KGCItemImgWnd* m_pkPollItemImgView;
//
//	int	m_iPollCount; //기부할 아이템의 카운트
//	int	m_iBtnPressCount;
//public:
//	void OnExit();
//	void OnOk();
//	void Update();
//	void OnPressUpArrowBtn();
//	void OnPressDownArrowBtn();
//	void OnInitPressCount() { m_iBtnPressCount = -50; }
//	void SetPollCount( int iPollCount) { m_iPollCount = iPollCount;}
//	int GetPollCount() { return m_iPollCount; }
//	void SetOkBtnLock(bool bLock) { m_pkOkBtn->Lock(bLock); }
//	void SetItemImg( DWORD dwItemID);
//	void PollCountUp();
//	void PollCountDown();
//	void OnClickUpArrowBtn();
//	void OnClickDownArrowBtn();
//};
//
//
//DEFINE_WND_FACTORY( KGCValentineDayDlg );
//DECLARE_WND_FACTORY( KGCValentineDayDlg );
//DECLARE_WND_FACTORY_NAME( KGCValentineDayDlg );