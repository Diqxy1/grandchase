#pragma once

#define MAX_COUPON_WND 4

class KGCItemImgWnd;
class KGCCharacterUI;
class KGCDescBox;
class KLuaManager;
class KGCCouponNPCEventWnd;

class KGCCouponNPCDlg :	public KD3DDialog,
	public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCCouponNPCDlg );
	/// Default constructor
	KGCCouponNPCDlg( void );
	/// Default destructor
	virtual ~KGCCouponNPCDlg( void );
	/// Copy constructor
	KGCCouponNPCDlg( const KGCCouponNPCDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCCouponNPCDlg& operator=( const KGCCouponNPCDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
protected:
	virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

public:
	int GetMaxPage();
	void MovePage( int iPage_ );
	void DrawEventList( int iPage_, int iCurPageEventListNum_ );
	void SetCouponInfo( std::vector< KGameCouponInfo >& mapCouponInfo_ );
	bool CheckInvalidCouponString(const std::wstring& str);
	void OnClickInputBtn();
	void InitWindow();
	void LoadWebAddress();

protected:                         
	KSafeArray<KGCCouponNPCEventWnd*,MAX_COUPON_WND> m_pkEventWnd;
	KD3DSizingBox*			m_pkDisableInput;
	KD3DSizingBox*			m_pkEnableInput;
#if defined(COUPON_TWO_INPUT)
	KD3DSizingBox*			m_pkDisablePasswordInput;
	KD3DSizingBox*			m_pkEnablePasswordInput;
#endif

	KD3DWnd*				m_pkBtnEnableInput;
	KD3DWnd*				m_pkBtnDisableInput;
	KD3DWnd*				m_pkBtnPrevPage;
	KD3DWnd*				m_pkBtnNextPage;
	KD3DWnd*				m_pkBtnClose;
	KD3DStatic*				m_pkStaticPage;	
	KD3DEdit*				m_pkEdit;
#if defined(COUPON_TWO_INPUT)
	KD3DEdit*				m_pkEdit2;	
#endif
	KD3DStatic*				m_pkStaticDesc;

	KSafeArray<int,MAX_COUPON_WND> m_iCurPageEventUID;
	int						m_iSelectEventUID;
	int						m_iCheckSelect;
	int						m_iCurPage;

	std::vector< KGameCouponInfo >	m_vecCouponInfo;
	
	std::vector< std::pair<DWORD, std::string>> m_vecWebAddressInfo;
public:
	bool					m_bClickedInputBtn;
	int						m_iCountInput;
};


DEFINE_WND_FACTORY( KGCCouponNPCDlg );
DECLARE_WND_FACTORY( KGCCouponNPCDlg );
DECLARE_WND_FACTORY_NAME( KGCCouponNPCDlg );