//#pragma once
//
//class KGCGWCShowStateDlg : public KD3DDialog,
//	public KActionListener
//{
//public:
//	DECLARE_CLASSNAME( KGCGWCShowStateDlg );
//
//	KGCGWCShowStateDlg(void);
//	virtual ~KGCGWCShowStateDlg(void);
//
//	/// Copy constructor
//	KGCGWCShowStateDlg( const KGCGWCShowStateDlg& )
//	{ ASSERT( false && "Do not use copy constructor!" ); }
//	/// Assignment operator
//	const KGCGWCShowStateDlg& operator=( const KGCGWCShowStateDlg& )
//	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
//
//// 		1 : 한국
//// 		2 : 대만
//// 		3 : 브라질
//// 		4 : 홍콩
//// 		5 : 미국
//// 		6 : 필리핀
//// 		7 : 태국
//	enum Nation
//	{
//		KR = 1,
//		TW,
//		BR,
//		HK,
//		US,
//		PH,
//		TH,
//		MAXNUM,
//	};
//
//public:
//	virtual void OnCreate( void );
//	virtual void ActionPerformed( const KActionEvent & );
//
//	void OnExit(void);
//	void SetState( std::vector< KGWCSupportData> vecData );
//
//private:
//	KD3DStatic*		m_pkStaticDesc;
//	KD3DStatic*		m_pkStaticState[MAXNUM-1];
//	KD3DWnd*		m_pkBtnClose;
//
//};
//
//DEFINE_WND_FACTORY( KGCGWCShowStateDlg );
//DECLARE_WND_FACTORY( KGCGWCShowStateDlg );
//DECLARE_WND_FACTORY_NAME( KGCGWCShowStateDlg );