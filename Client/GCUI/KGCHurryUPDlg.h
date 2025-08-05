#ifndef _KGC_HURRYUPDLG_H_
#define _KGC_HURRYUPDLG_H_
class KGCHurryUPDlg : public KD3DWnd,      // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCHurryUPDlg );

	KGCHurryUPDlg(void);
	virtual ~KGCHurryUPDlg(void);

	/// Copy constructor
	KGCHurryUPDlg( const KGCHurryUPDlg& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCHurryUPDlg& operator=( const KGCHurryUPDlg& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroy( void );
	virtual void PostDraw( void );

	void CheckAndRenderOff();
	void ViewStart();

private:
	DWORD m_dwStopTime;
};

DEFINE_WND_FACTORY( KGCHurryUPDlg );
DECLARE_WND_FACTORY( KGCHurryUPDlg );
DECLARE_WND_FACTORY_NAME( KGCHurryUPDlg );


#endif