#pragma once
#include "../GCSongkranEventManager.h"

class KGCSongkranDlg : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSongkranDlg );
    /// Default constructor
    KGCSongkranDlg( void );
    /// Default destructor
    virtual ~KGCSongkranDlg( void );
    /// Copy constructor
    KGCSongkranDlg( const KGCSongkranDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSongkranDlg& operator=( const KGCSongkranDlg& )
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
		NUM_SPECIAL_REWARD_SIZE = 2,	//	스페셜 리워드 보상 갯수
	};

	enum 
	{
		EM_ONE_HIT_POINT	= 0,
		EM_DOUBLE_HIT_POINT = 1,
		EM_HIT_POINT_SIZE	= 2,
	};



public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnCreateComplete( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostChildDraw( void );

public:
	void OnClose();
	void OnLeftReward();
	void OnRightReward();

	void UpdateItemInfomation();
	void RenderItemInfoBox( D3DXVECTOR2& vPos_, GCITEMID _itemID );    

public:
	void Init();
	void InitTargetPos();
	void InitPlayerPos();
	void InitCharMarkPos();
	void InitBombPos();
	void InitGetPointBack();

	void UpdateBombCount();
	void UpdateScore();
	void UpdateReward();
	void UpdateSpecialReward();
	void UpdateRewardReq();
	void UpdateProgress();
	void UpdateRemainTime();

	void UpdateRewardData();
	void UpdateResultScore();

	void GetRewardItem();
	
	
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


    void Close();


public:
	float m_fMoveSpeed[MOVE_TYPE_RUN_SIZE];
	float m_fBombXSpeed;

	int	m_nCurFrame;

	int m_nTargetState[GCSongkranEventManager::CHAR_SIZE];
	int	m_nTargetDeathFrame[GCSongkranEventManager::CHAR_SIZE];
	int m_nTargetTelFrame[TEL_CHAR_SIZE];
	int	m_nCharAnimationFrame;

	int	m_nBombState;
	int m_nBombFrame;

	int m_nPrevTime;
	bool m_bRenderCharMark;
	bool m_bResultShot;
	DWORD	m_dwIndex;
	EGCBackgroundMusic m_backMusic;




public:
	KD3DWnd*	m_apkTarget[GCSongkranEventManager::CHAR_SIZE];
	KD3DWnd*	m_apkPlayer;
	KD3DWnd*	m_apkPlayerMark;
	
	KD3DWnd*	m_apkBomb;
	KD3DWnd*	m_apkScope;

	KD3DWnd*	m_pkBtnClose;
	KD3DWnd*	m_pkBtnClose2;

	KD3DWnd*	m_pkBtnLeftReward;
	KD3DWnd*	m_pkBtnRightReward;
	
	KD3DWnd*	m_pkBtnReward;
	KD3DWnd*	m_pkPointBar;

	KD3DWnd*		m_apkPointBack[EM_HIT_POINT_SIZE];
	KGCUINumber*	m_apkPointNum[EM_HIT_POINT_SIZE];
	
	KGCItemImgWnd*	m_pkRewardImg;
	KGCItemImgWnd*	m_apkSpecialRewardImg[NUM_SPECIAL_REWARD_SIZE];
	KD3DStatic*		m_pkSpecialRewardScore[NUM_SPECIAL_REWARD_SIZE];
	
	KGCUINumber*	m_pkGameScore;
	KGCUINumber*	m_pkBombCount;

	KD3DStatic*	m_pkItemName;
	KD3DStatic*	m_pkRewardDesc;
	KD3DStatic*	m_pkRemainTime;

};

DEFINE_WND_FACTORY( KGCSongkranDlg );
DECLARE_WND_FACTORY( KGCSongkranDlg );
DECLARE_WND_FACTORY_NAME( KGCSongkranDlg );
