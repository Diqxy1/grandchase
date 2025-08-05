#pragma once

#include "KGCMatchManager.h"
#include "KGCPartyManager.h"

class KGCMatchUserBtnS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchUserBtnS6 );
	/// Default constructor
	KGCMatchUserBtnS6( void );
	/// Default destructor
	virtual ~KGCMatchUserBtnS6( void );
	/// Copy constructor
	KGCMatchUserBtnS6( const KGCMatchUserBtnS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchUserBtnS6& operator=( const KGCMatchUserBtnS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetUserInfo( std::wstring strNick, DWORD dwUserUID, bool bOnBtn );
	void OnClickInvite( );
	void FrameMoveInEnabledState( );
private:
	KD3DStatic *m_pkNick;
	DWORD m_dwUserUID;
	KD3DWnd *m_pkBtn;

	time_t m_tmInvite;
};

DEFINE_WND_FACTORY( KGCMatchUserBtnS6 );
DECLARE_WND_FACTORY( KGCMatchUserBtnS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchUserBtnS6 );


class KGCMatchInviteUserListS6 : public KD3DDialog    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchInviteUserListS6 );
	/// Default constructor
	KGCMatchInviteUserListS6( void );
	/// Default destructor
	virtual ~KGCMatchInviteUserListS6( void );
	/// Copy constructor
	KGCMatchInviteUserListS6( const KGCMatchInviteUserListS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchInviteUserListS6& operator=( const KGCMatchInviteUserListS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KMIUL_USER_BTN_MAX = 9,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void OnClickClose( void );
	void OnClickLeft( void );
	void OnClickRight( void );
	void UpdateUserList( int iPage );
private:
	KD3DWnd *m_pkBtnClose;
	KD3DWnd *m_pkBtnLeft;
	KD3DWnd *m_pkBtnRight;
	KSafeArray<KGCMatchUserBtnS6*,KMIUL_USER_BTN_MAX> m_saUserBtn;

	int m_iCurrentPage;

};

DEFINE_WND_FACTORY( KGCMatchInviteUserListS6 );
DECLARE_WND_FACTORY( KGCMatchInviteUserListS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchInviteUserListS6 );


class KGCMatchUserIconS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchUserIconS6 );
	/// Default constructor
	KGCMatchUserIconS6( void );
	/// Default destructor
	virtual ~KGCMatchUserIconS6( void );
	/// Copy constructor
	KGCMatchUserIconS6( const KGCMatchUserIconS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchUserIconS6& operator=( const KGCMatchUserIconS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KMUI_CHECK_STATE_NOT_RENDER = 0,
		KMUI_CHECK_STATE_X = 1,
		KMUI_CHECK_STATE_V = 2,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetIconAct( bool bAct );
	void SetUserNickName( std::wstring strUserNick ,DWORD dwARGB = 0xffffffff);
	void SetCheck( int iCheckState );
private:
	KD3DWnd *m_pkIcon;
	KD3DWnd *m_pkCheck;
	KD3DWnd *m_pkBack;
	KD3DStatic *m_pkUserName;
};

DEFINE_WND_FACTORY( KGCMatchUserIconS6 );
DECLARE_WND_FACTORY( KGCMatchUserIconS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchUserIconS6 );


class KGCGaugeS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCGaugeS6 );
	/// Default constructor
	KGCGaugeS6( void );
	/// Default destructor
	virtual ~KGCGaugeS6( void );
	/// Copy constructor
	KGCGaugeS6( const KGCGaugeS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGaugeS6& operator=( const KGCGaugeS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		AMLB_GAGE_START_LEFT = 0,
		AMLB_GAGE_START_RIGHT = 1,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetGauge( int iCurrent, int iMax, int iType = 0 );
	void SetSameExtentionSize ( float &fCurrentSize, float fMaxSize, float &fCurrentTexSize, float fMaxTexSize );
private:
	KD3DMoveWnd* m_pkInImage;
	int m_dwOriginalWidth;
	int m_iPrePoint;
	D3DXVECTOR4 m_avTexDef;
	float m_fBasePosX;
};

DEFINE_WND_FACTORY( KGCGaugeS6 );
DECLARE_WND_FACTORY( KGCGaugeS6 );
DECLARE_WND_FACTORY_NAME( KGCGaugeS6 );

class KGCMatchCompleteDlgS6 : public KD3DDialog    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchCompleteDlgS6 );
	/// Default constructor
	KGCMatchCompleteDlgS6( void );
	/// Default destructor
	virtual ~KGCMatchCompleteDlgS6( void );
	/// Copy constructor
	KGCMatchCompleteDlgS6( const KGCMatchCompleteDlgS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchCompleteDlgS6& operator=( const KGCMatchCompleteDlgS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KMCD_TEAM_BLUE = 0,
		KMCD_TEAM_RED = 1,
		KMCD_USER_MAX = 6,
		KMCD_WAIT_TIME = 15,
	};



public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void FrameMoveInEnabledState( void );
	void StartTimer( DWORD dwMaxTime );
	void OverTime( );
	void InitUserIcon( );
	void SetUserInfo( );
	void OnClickOKBtn( );
	void OnCloseDlg( );
	void OnCloseDlgDirect( );
	void SetCancelUser( DWORD dwUserUID );
private:
	KD3DWnd *m_pkOKBtn;
	KD3DWnd *m_pkCancelBtn;

	KGCGaugeS6 *m_pkGauge;
	KD3DStatic *m_pkTime;

	DWORD m_tmCurrentTime;
	DWORD m_tmMaxTime;

	DWORD m_dwMaxTime;
	DWORD m_dwCloseTime;

	KSafeArray<KGCMatchUserIconS6*, KMCD_USER_MAX> m_saUserIcon;

};

DEFINE_WND_FACTORY( KGCMatchCompleteDlgS6 );
DECLARE_WND_FACTORY( KGCMatchCompleteDlgS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchCompleteDlgS6 );

class KGCMatchMapSelectBtnS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchMapSelectBtnS6 );
	/// Default constructor
	KGCMatchMapSelectBtnS6( void );
	/// Default destructor
	virtual ~KGCMatchMapSelectBtnS6( void );
	/// Copy constructor
	KGCMatchMapSelectBtnS6( const KGCMatchMapSelectBtnS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchMapSelectBtnS6& operator=( const KGCMatchMapSelectBtnS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetInfo( int iMapID );
	void OnClickCheckInBtn( void );
	void OnClickCheckOutBtn( void );
	std::pair< int, bool > GetBtnState( void );
private:
	KD3DWnd *m_pkCheckIn;
	KD3DWnd *m_pkCheckOut;

	KD3DStatic *m_pkMapName;
	
	int m_iMapID;
};

DEFINE_WND_FACTORY( KGCMatchMapSelectBtnS6 );
DECLARE_WND_FACTORY( KGCMatchMapSelectBtnS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchMapSelectBtnS6 );


class KGCMatchMapSelectDlgS6 : public KD3DDialog    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchMapSelectDlgS6 );
	/// Default constructor
	KGCMatchMapSelectDlgS6( void );
	/// Default destructor
	virtual ~KGCMatchMapSelectDlgS6( void );
	/// Copy constructor
	KGCMatchMapSelectDlgS6( const KGCMatchMapSelectDlgS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchMapSelectDlgS6& operator=( const KGCMatchMapSelectDlgS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KMMS_MAX_HEIGHT_NUM = 12,
		KMMS_MIN_WIDTH_NUM = 2,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void OnClickCloseBtn( void );
	void OnClickOKBtn( void );
	void SetWndSizeAndWndPos( int iBtnNum );
	void InitBtnList( void );
	void OnClickMapBtn( KD3DWnd* pWnd );
private:
	KD3DSizingBox *m_pkBackBox;
	KD3DSizingBox *m_pkInBackBox;

	KD3DWnd *m_pkCloseBtn;
	KD3DWnd *m_pkOKBtn;

	std::vector<KGCMatchMapSelectBtnS6*> m_vecMapBtn;
};

DEFINE_WND_FACTORY( KGCMatchMapSelectDlgS6 );
DECLARE_WND_FACTORY( KGCMatchMapSelectDlgS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchMapSelectDlgS6 );

class KGCMatchPartyViewS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchPartyViewS6 );
	/// Default constructor
	KGCMatchPartyViewS6( void );
	/// Default destructor
	virtual ~KGCMatchPartyViewS6( void );
	/// Copy constructor
	KGCMatchPartyViewS6( const KGCMatchPartyViewS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchPartyViewS6& operator=( const KGCMatchPartyViewS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void UpdateUserList( void );
private:
	KD3DWnd *m_pkDesc;
	KSafeArray<KGCMatchUserIconS6*, KGCPartyManager::KPM_MATCH_PARTY_MAX_USER> m_saPartyList;
};

DEFINE_WND_FACTORY( KGCMatchPartyViewS6 );
DECLARE_WND_FACTORY( KGCMatchPartyViewS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchPartyViewS6 );


class KGCMatchStartPartS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchStartPartS6 );
	/// Default constructor
	KGCMatchStartPartS6( void );
	/// Default destructor
	virtual ~KGCMatchStartPartS6( void );
	/// Copy constructor
	KGCMatchStartPartS6( const KGCMatchStartPartS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchStartPartS6& operator=( const KGCMatchStartPartS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KMR_MAX_MODE = 3,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	void UpdateState( void );
	void SetUserStateNone( void );
	void SetUserStatePartyUser( void );
	void SetUserStatePartyMaster( void );

	void UpdateBtnState( void );
	void OnClickMode( int iMode );
	void OnClickMatchingBtn( );
	void OnClickLeaveParty( void );
	void OnClickReadyBtn( void );
	void OnClickCancelBtn( void );
	void SetModeStateTex( int index, KD3DWnd *pWnd );
private:
	KD3DWnd *m_pkMatchingBtn;
	KD3DWnd *m_pkLeavePartyBtn;
	KD3DWnd *m_pkReadyBtn;
	KD3DWnd *m_pkCancelBtn;

	KD3DWnd *m_pkDescUser;

	KGCMatchPartyViewS6* m_pkPartyUser;

	time_t m_tmInfoPacketReq;

	KSafeArray<KD3DWnd*, KGCMatchManager::MMATCHMODE_MAX> m_saModeBtnList;
	KSafeArray<KD3DWnd*, KGCMatchManager::MMATCHMODE_MAX> m_saModeActList;
	KSafeArray<KD3DWnd*, KGCMatchManager::MMATCHMODE_MAX> m_saDescMode;
	KSafeArray<KD3DWnd*, KGCMatchManager::MMATCHMODE_MAX> m_saDescModeBack;

	std::map<int, D3DXVECTOR4> m_mapDescWndUV;

};

DEFINE_WND_FACTORY( KGCMatchStartPartS6 );
DECLARE_WND_FACTORY( KGCMatchStartPartS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchStartPartS6 );


class KGCMatchWaitPartS6 : public KD3DWnd    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCMatchWaitPartS6 );
	/// Default constructor
	KGCMatchWaitPartS6( void );
	/// Default destructor
	virtual ~KGCMatchWaitPartS6( void );
	/// Copy constructor
	KGCMatchWaitPartS6( const KGCMatchWaitPartS6& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMatchWaitPartS6& operator=( const KGCMatchWaitPartS6& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		KMWTIME_MAX_WND = 6,
		KMWTIMEBACK_MAX_WND = 3,
		KMWTIMEDIV_MAX_WND = 2,
	};

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void SetNumberTex( int index, KD3DWnd *pWnd );
	void ChangeUVtoTexel( D3DXVECTOR4& vUV );
	void FrameMoveInEnabledState( void );
	void OnClickCancelBtn( void );
	void SetTimeRender( bool bRender );
private:
	KD3DWnd *m_pkCancelBtn;
	KSafeArray<KD3DWnd*,KMWTIME_MAX_WND> m_saTime;
	KSafeArray<KD3DWnd*,KMWTIMEBACK_MAX_WND> m_saTimeBack;
	KSafeArray<KD3DWnd*,KMWTIMEDIV_MAX_WND> m_saTimeDiv;

	time_t m_tmCurTime;

	KD3DWnd *m_pkWaitTime;
	KD3DWnd *m_pkCalcWaitTime;
	KD3DWnd *m_pkOverWaitTime;

	KD3DStatic *m_pkOvertimeDesc;

	DWORD m_dwAlpha;
};

DEFINE_WND_FACTORY( KGCMatchWaitPartS6 );
DECLARE_WND_FACTORY( KGCMatchWaitPartS6 );
DECLARE_WND_FACTORY_NAME( KGCMatchWaitPartS6 );


class KGCAutoMatchingS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAutoMatchingS6 );
    /// Default constructor
    KGCAutoMatchingS6( void );
    /// Default destructor
    virtual ~KGCAutoMatchingS6( void );
    /// Copy constructor
    KGCAutoMatchingS6( const KGCAutoMatchingS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAutoMatchingS6& operator=( const KGCAutoMatchingS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void UpdateState( void );
	void UpdateReadyState( void );
	void UpdateWaitState( void );
	void OnClickMapSelBtn( void );
	void OnClickInviteUserBtn( void );
	void OnClickOption( void );
	void UpdateTime( void );
private:
	KD3DWnd *m_pkInvitePartyBtn;
	KD3DWnd *m_pkMapSelectBtn;
	KGCCheckBox *m_pkOptionStart;

	KD3DStatic *m_pkOpenTimeStatic;
	KD3DStatic *m_pkOpenTime;

	KGCMatchStartPartS6 *m_pkStartScene;
	KGCMatchWaitPartS6 *m_pkWaitScene;
	
};

DEFINE_WND_FACTORY( KGCAutoMatchingS6 );
DECLARE_WND_FACTORY( KGCAutoMatchingS6 );
DECLARE_WND_FACTORY_NAME( KGCAutoMatchingS6 );
