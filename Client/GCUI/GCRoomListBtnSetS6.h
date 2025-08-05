#pragma once

class KGCRoomListBtnSetS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomListBtnSetS6 );
    /// Default constructor
    KGCRoomListBtnSetS6( void );
    /// Default destructor
    virtual ~KGCRoomListBtnSetS6( void );
    /// Copy constructor
    KGCRoomListBtnSetS6( const KGCRoomListBtnSetS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomListBtnSetS6& operator=( const KGCRoomListBtnSetS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KDSI_BTN_LEVEL_1 = 0,
		KDSI_BTN_LEVEL_2,
		KDSI_BTN_LEVEL_3,
		KDSI_BTN_LEVEL_NUM,
	};

	enum {
		KDSI_CLOSE_LEVEL_2 = 0,
		KDSI_CLOSE_LEVEL_3,
		KDSI_CLOSE_LEVEL_NUM,
	};
public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void SetWndState( void );
	void SetBtnState( void );
	void OnClickBtn( const KActionEvent& event );
	void SetBtnCheckMark( KD3DWnd *pkBaseWnd );
private:
	KSafeArray<KD3DWnd*, KDSI_BTN_LEVEL_NUM> m_saLevelBtn;
	KD3DWnd* m_pkEventHellLevelBtn;
	KD3DWnd* m_pkHellLevelBtn;
	KD3DWnd* m_pkEventBreakLevelBtn;
	KD3DWnd* m_pkBreakLevelBtn;

	KSafeArray<KD3DWnd*, KDSI_CLOSE_LEVEL_NUM> m_saClose;
	KD3DWnd* m_pkEventHellLevelClose;
	KD3DWnd* m_pkHellLevelClose;
	KD3DWnd* m_pkEventBreakLevelClose;
	KD3DWnd* m_pkBreakLevelClose;

	KD3DWnd *m_pkCheck;

};

DEFINE_WND_FACTORY( KGCRoomListBtnSetS6 );
DECLARE_WND_FACTORY( KGCRoomListBtnSetS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomListBtnSetS6 );
