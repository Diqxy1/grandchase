#pragma once

class KGCHappyNewYearNPCDlg :	public KD3DDialog,
							public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCHappyNewYearNPCDlg );
	/// Default constructor
	KGCHappyNewYearNPCDlg( void );
	/// Default destructor
	virtual ~KGCHappyNewYearNPCDlg( void );
	/// Copy constructor
	KGCHappyNewYearNPCDlg( const KGCHappyNewYearNPCDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCHappyNewYearNPCDlg& operator=( const KGCHappyNewYearNPCDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroyComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

public:
	void SetItem(int iItemID);
	void OnConfirm();
	void OnExtention();
	void OnDestroy();

	void SetExtent( bool bExtent );
	void SetGpContent(std::wstring wstrContent);
	void SetTitleContent(std::wstring wstrContent);

private:
	int                     m_iItemID;
	GCDeviceTexture*        m_pTexItem;

	KD3DWnd*	            m_pkBtnOk1;
	KD3DWnd*	            m_pkBtnOk2;
	KD3DWnd*	            m_pkBar;
	KD3DWnd*	            m_pkGp;

	KD3DStatic*             m_pkContent;
	KD3DStatic*             m_pkGpContent;

	KD3DSizingBox*			m_pkBackGround;
	KD3DSizingBox*			m_pkItemBack;
	KD3DSizingBox*			m_pkGPBack;
};


DEFINE_WND_FACTORY( KGCHappyNewYearNPCDlg );
DECLARE_WND_FACTORY( KGCHappyNewYearNPCDlg );
DECLARE_WND_FACTORY_NAME( KGCHappyNewYearNPCDlg );