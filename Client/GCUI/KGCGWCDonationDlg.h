//#pragma once
//
//class KGCGWCDonationDlg : public KD3DDialog,
//	public KActionListener
//{
//public:
//	DECLARE_CLASSNAME( KGCGWCDonationDlg );
//
//	KGCGWCDonationDlg(void);
//	virtual ~KGCGWCDonationDlg(void);
//
//	/// Copy constructor
//	KGCGWCDonationDlg( const KGCGWCDonationDlg& )
//	{ ASSERT( false && "Do not use copy constructor!" ); }
//	/// Assignment operator
//	const KGCGWCDonationDlg& operator=( const KGCGWCDonationDlg& )
//	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }
//
//public:
//	virtual void OnCreate( void );
//	virtual void ActionPerformed( const KActionEvent& event );
//
//	void InitNumberTextureCoordinate( void );
//	void OnCancle( void );
//	void OnOK( void );
//	void SetItem( DWORD dwItemID, int nAccCount );
//
//private:
//	KD3DWnd*		m_pkBtnCancle;
//	KD3DWnd*		m_pkBtnOK;
//	KD3DWnd*		m_pkBtnUp;
//	KD3DWnd*		m_pkBtnDown;
//	KD3DStatic*		m_pkStaticDesc;
//	KGCItemImgWnd*	m_pkDonationItem;
//	KD3DNumber*		m_pkNumber[2];
//	KD3DWnd*		m_pkBtnAll;
//
//private:
//	int				m_iNum;
//	DWORD			m_dwEventItemID;
//	DWORD			m_nAccCount;
//};
//
//DEFINE_WND_FACTORY( KGCGWCDonationDlg );
//DECLARE_WND_FACTORY( KGCGWCDonationDlg );
//DECLARE_WND_FACTORY_NAME( KGCGWCDonationDlg );