#pragma once

class KGCUINumber;

struct OlympicSongTargetInfo
{
    float m_fTargetMoveSpeed;         // 타겟 이동 속도
    DWORD m_dwTargetScore;            // 타겟 기본 점수
    float m_fTeleportPeriod;          // 텔레포트 주기 (단위 : 초)
    float m_fHitTargetAfterResetTime; // 폭탄 맞고 나서 리셋 되는 주기 (단위 : 초)
};


class KGCOlympicSongkranDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCOlympicSongkranDlg );
    /// Default constructor
    KGCOlympicSongkranDlg( void );
    /// Default destructor
    virtual ~KGCOlympicSongkranDlg( void );
    /// Copy constructor
    KGCOlympicSongkranDlg( const KGCOlympicSongkranDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOlympicSongkranDlg& operator=( const KGCOlympicSongkranDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum
	{
		TEL_CHAR_ARME = 0,
		TEL_CHAR_REY = 1,
		TEL_CHAR_SIZE = 2,
	};

	enum 
	{
		MOVE_TYPE_RUN_SIZE = 3,
		MOVE_TYPE_RANDOM_TEL_SIZE = 1,
		MOVE_TYPE_FIX_TEL_SIZE = 1,
	};

	enum 
	{
		BOMB_STATE_READY	= 0,	//	들고 있는 상태
		BOMB_STATE_SHOT		= 1,	//	날라가고 있는 상태
		BOMB_STATE_EXPLODE	= 2,	//	폭탄이 터진 상황
	};

	enum
	{
		TARGET_STATE_MOVE = 1,		//	이동상태
		TARGET_STATE_DEATH = 2,		//	죽으상태
	};

	enum 
	{
		EM_ONE_HIT_POINT	= 0,
		EM_DOUBLE_HIT_POINT = 1,
		EM_HIT_POINT_SIZE	= 2,
	};

    enum 
    {
        CHAR_ELESIS = 0,
        CHAR_ZERO	= 1,
        CHAR_LAS	= 2,
        CHAR_ARME	= 3,
        CHAR_REY	= 4,
        CHAR_SIZE	= 5,
    };

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

public:
	void Init();
	void InitTargetPos();
	void InitPlayerPos();
	void InitCharMarkPos();
	void InitBombPos();
	void InitGetPointBack();

	void UpdateBombCount();
	void UpdateScore();

	void UpdateResultScore();
	
	void UpdateBomb();
	void UpdateTarget();

	void UpdateKeyCheck();
	void UpdateTargetPos();
	void UpdatePlayerPos( D3DXVECTOR2 vPos );
	void UpdateBombPos();
	
	
	void UpdateTargeting();
	void ResetTargeting();

	void SetBombStateExplode();
	void SearchBombTarget( float fPosX );

	bool IsBombShot();
	void CalcBombSpeed();
	

	void ThrowBomb();
	
	void FailScore();
	void AddScore( int nScore, bool _bDouble, int _nIndex );
	void ReqGetScore( int nScore );
    void OnClose();

    // 매니저 부분 합침
public:
    DWORD		GetGameScore() { return m_dwScore; }
    int			GetTeleportFrame( int nChar );
    int			GetResetFrame( int nChar );
    DWORD		GetTargetScore( int nChar );
    float		GetTargetSpeed( int nChar );

    bool LoadTargetInfo();          // 타켓의 정보 클라가 가져오기
    void SetUserInfo( KEVENT_OLYMPIC_JOIN_GAME_ACK kRecv );
    void ResultSongkranInfo( KEVENT_OLYMPIC_WATERBOMB_RESULT_ACK kRecv );

    OlympicSongTargetInfo*	GetTargetInfo( int nChar );

    float               m_fPlayerCharMoveSpeed;  // 유저 캐릭터 이동 속도
    float               m_fWaterBombFlyingSpeed; // 물폭탄 비행 속도
    float				m_fWaterBombRange;       // 물폭탄 범위

    DWORD               m_dwScore;                // 점수
    DWORD				m_dwWaterBombDuration;	  // 물풍선 갯수

    std::map< USHORT, OlympicSongTargetInfo >        m_mapTargetInfoList;     // 타겟 정보 컨테이너 first는 캐릭터 타입 enum 이용

    //매니저 내용 끝
public:
	float m_fMoveSpeed[MOVE_TYPE_RUN_SIZE];
	float m_fBombXSpeed;

	int	m_nCurFrame;

	int m_nTargetState[CHAR_SIZE];
	int	m_nTargetDeathFrame[CHAR_SIZE];
	int m_nTargetTelFrame[TEL_CHAR_SIZE];
	int	m_nCharAnimationFrame;

	int	m_nBombState;
	int m_nBombFrame;

    time_t          m_tmTodayDate;      
    int             m_nGameType;        
    int             m_nTeamType;        

	int     m_nPrevTime;
	bool    m_bRenderCharMark;
	bool    m_bResultShot;
	DWORD	m_dwIndex;
	EGCBackgroundMusic m_backMusic;

public:
	KD3DWnd*	m_apkTarget[CHAR_SIZE];
	KD3DWnd*	m_apkPlayer;
	KD3DWnd*	m_apkPlayerMark;
	
	KD3DWnd*	m_apkBomb;
	KD3DWnd*	m_apkScope;

	KD3DWnd*	m_pkBtnClose;
	KD3DWnd*	m_pkBtnClose2;
	
	KD3DWnd*	m_pkBtnReward;

	KD3DWnd*		m_apkPointBack[EM_HIT_POINT_SIZE];
	KGCUINumber*	m_apkPointNum[EM_HIT_POINT_SIZE];
	
	KGCUINumber*	m_pkGameScore;
	KGCUINumber*	m_pkBombCount;

};

DEFINE_WND_FACTORY( KGCOlympicSongkranDlg );
DECLARE_WND_FACTORY( KGCOlympicSongkranDlg );
DECLARE_WND_FACTORY_NAME( KGCOlympicSongkranDlg );
