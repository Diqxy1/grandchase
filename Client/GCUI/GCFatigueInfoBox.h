#pragma once


#include "KNCSingleton.h"

struct SFatigueData {
	int iOnline;
	int iOffline;
	bool bAlarm;
	int iRatio;
	int iMaxOnlineTime;
	int iResetOfflineTime;
	SFatigueData()
	: iOnline(0)
	, iOffline(0)
	, bAlarm(false)
	, iRatio(0)
	, iMaxOnlineTime(1)
	, iResetOfflineTime(1)
	{
	}
};

class KGCFatigueManager
{
	DeclareSingleton( KGCFatigueManager );

public:
	enum FATIGUE_TIME {
		GREEN_TIME, // 0 ~ 180분 
		YELLOW_TIME, // 180 ~ 300 분 
		RED_TIME, // 300 ~ 무제한
	};

	enum FATIGUE_USER_TYPE {
		FUT_AUTH_NONE = 0,
		FUT_AUTH_CHILDREN = 1,
		FUT_AUTH_ADULT = 2,
	};

	enum FATIGUE_USER_CONNECT_TYPE {
		FUT_ONE_CONNECT = 0,
		FUT_MULTI_CONNECT = 1,
	};
	


public:
	KGCFatigueManager(void);
	~KGCFatigueManager(void);
	void InitFatigueSystem(KEVENT_ACCUMULRATION_TIME_NOT &kFatigueData);
	SFatigueData GetCurrentInfo();
	bool IsFatigueSystemApplyUser(); //과몰입 방지 적용 유저인가??
	void LoadLuaTable();
	bool IsViewFatigueInfoScene(); 
	int GetMaxOnlineTime(){return m_iMaxOnlineTime;} 
	int GetResetOfflineTime(){return m_iResetOfflineTime;}
	int GetCurrentFatigueSystemType(); //누적 시간으로 현재 과몰입 단계를 알아낸다 - 클라이언트에서 시간계산

	bool IsAlarm(){return m_bAlarm;}
	void SetAlarm(bool bAlarm){m_bAlarm=bAlarm;}
	void SetGameRatio(int iRatio){m_iGameRatio = iRatio;}
	int GetGameRatio(){return m_iGameRatio;}

	void SetAccOnlineTime(int iTime) {m_iAccOnlineTime = iTime;}
	int GetAccOnlineTime(){return m_iAccOnlineTime;}

	void SetAuthType(int iType) {m_iAuth = iType;}
	int GetAuthType(){return m_iAuth;}

	void SetConnectType(int iType) {m_iConnectType = iType;}
	int GetConnectType(){return m_iConnectType;}

	void SetCurrentGameFatigueType(int iType) {m_iCurrentGameFatigueType = iType;} // 대전 던전 시작시 서버에서 보내줌.
	int GetCurrentGameFatigueType(){return m_iCurrentGameFatigueType;}


	void SetBilingAccount(std::string str) {m_strBillingAccount = str;}
	std::string GetBilingAccount(){return m_strBillingAccount;}


private:
	void _GetFatigueSystemTime(OUT int &iOnlineTime, OUT int &iOfflineTime );
private:
	int m_iOnlineTime; // 센터서버 입장시 받는 전 게임 누적시간. 안씀
	int m_iOldAlarmTime; // 같은 시간에 메시지를 중복으로 띄우지 않기 위한 변수
	int m_iOfflineTime;  // 안씀
	time_t m_tmConnectServerTime; //서버 타임
	std::map<int, KFatigueTable> m_mapFatigueSystemInfo; // 과몰입 단계마다의 시간 및 정보
	bool m_bInitData; //데이터 초기화가 되었는지
	bool m_bAlarm; // 같은 시간에 메시지를 중복으로 띄우지 않기 위한 변수

	int m_iAuth; // 유저 상태 정보 (실명정보 기입을 했는지, 성인인지 아이인지)
	int m_iConnectType; //전 게임 이중접속 확인

	int m_iMaxOnlineTime; // 안씀
	int m_iResetOfflineTime; //안씀
	int m_iGameRatio; // EXP 계산을 위한 개인 정보
	std::set<int> m_setViewState;  //안씀
	int m_iCurrentGameFatigueType; // 대전 던전 시작시 서버에서 보내줌.

	int m_iAccOnlineTime; // 중국,전체 게임의 누적온라인 시간. 센터서버에서 클라로, 클라에서 게임서버로 전달해야 함
	std::string         m_strBillingAccount; // 중국 빌링에 저장된 계정. 유저가 입력한 계정과 다르다. 
};

DefSingletonInline( KGCFatigueManager );

class KGCFatigueInfoBox : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCFatigueInfoBox );
    /// Default constructor
    KGCFatigueInfoBox( void );
    /// Default destructor
    virtual ~KGCFatigueInfoBox( void );
    /// Copy constructor
    KGCFatigueInfoBox( const KGCFatigueInfoBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCFatigueInfoBox& operator=( const KGCFatigueInfoBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void OnChangeState( void );
	void UpdateInfo( void );
private:
	KD3DWnd* m_pkOnlineProgressBar;
	KD3DStatic* m_pkStaticOnline;
	KD3DWnd* m_pkOfflineProgressBar;
	KD3DStatic* m_pkStaticOffline;

	int m_iOriginalOnlinePBarWidth;
	int m_iOriginalOfflinePBarWidth;

	D3DXVECTOR4 m_dfOriginalOnlinePBarIB;
	D3DXVECTOR4 m_dfOriginalOfflinePBarIB;
};

DEFINE_WND_FACTORY( KGCFatigueInfoBox );
DECLARE_WND_FACTORY( KGCFatigueInfoBox );
DECLARE_WND_FACTORY_NAME( KGCFatigueInfoBox );

class KGCFatigueMessageBox : public KD3DDialog    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCFatigueMessageBox );
	/// Default constructor
	KGCFatigueMessageBox( void );
	/// Default destructor
	virtual ~KGCFatigueMessageBox( void );
	/// Copy constructor
	KGCFatigueMessageBox( const KGCFatigueMessageBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCFatigueMessageBox& operator=( const KGCFatigueMessageBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void ChangeUVtoTexel( D3DXVECTOR4& vUV );
	void SetNumberTex( int index, KD3DWnd *pWnd );
	void FrameMoveInEnabledState( );
	void SetNumber( void );
	void OnClickBtnOK( void );
	void OnClickBtnExit( void );
	void OnClickBtnAuth( void );
private:
	int m_iCount;
	int m_iOldTime;
	KD3DWnd* m_pkMin;
	KD3DWnd* m_pkSec10;
	KD3DWnd* m_pkSec1;

	KD3DWnd* m_pkBtnOK;
	KD3DWnd* m_pkBtnOK2;
	KD3DWnd* m_pkBtnExitGame;
	KD3DWnd* m_pkBtnAuth;

	KD3DStatic* m_pkStaticDesc;
};

DEFINE_WND_FACTORY( KGCFatigueMessageBox );
DECLARE_WND_FACTORY( KGCFatigueMessageBox );
DECLARE_WND_FACTORY_NAME( KGCFatigueMessageBox );

class KGCFatigueMessageBox2 : public KD3DDialog    // extends
	,public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCFatigueMessageBox2 );
	/// Default constructor
	KGCFatigueMessageBox2( void );
	/// Default destructor
	virtual ~KGCFatigueMessageBox2( void );
	/// Copy constructor
	KGCFatigueMessageBox2( const KGCFatigueMessageBox2& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCFatigueMessageBox2& operator=( const KGCFatigueMessageBox2& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnCreate( void );
	void OnClickOK( void );
	void OnClickUserAddInfo( void );
	void OnClickFatigueAddInfo( void );
private:
	KD3DWnd* m_pkBtnOK;
	KD3DWnd* m_pkBtnAuth;
	KD3DWnd* m_pkBtnfatigue;

	KD3DStatic* m_pkStaticDesc;
};

DEFINE_WND_FACTORY( KGCFatigueMessageBox2 );
DECLARE_WND_FACTORY( KGCFatigueMessageBox2 );
DECLARE_WND_FACTORY_NAME( KGCFatigueMessageBox2 );

