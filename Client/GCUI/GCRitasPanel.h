#pragma once

#include "../GCRitasChrismasManager.h"


class KGCRitasObj;
class KGCRitasPlayer;
class KGCRitasGoalBasket;
class KGCRitasProgressBar;

class KGCRitasPanel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasPanel );
    /// Default constructor
    KGCRitasPanel( void );
    /// Default destructor
    virtual ~KGCRitasPanel( void );
    /// Copy constructor
    KGCRitasPanel( const KGCRitasPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasPanel& operator=( const KGCRitasPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum 
	{
		MAX_CNT_NUM = 4,
		MAX_OBJICON_NUM = 10,
	};
	enum eRitasState{
		BEFORE_GAME = 0,
		SET_STAGE	= 1,
		COUNT		= 2,
		PLAY		= 3,
		END_STAGE	= 4,
	};

	enum ePlayType {
		PLAYTYPE_NOMAL = 0,
		PLAYTYPE_FEVER = 1
	};

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();


	void SetStartType(int iType_)		{ m_iStartType = iType_; }
	void UpdatePlayer();
	void MoveRight();
	void MoveLeft();

	void SetCatchZone();
	void InitGameInfo();
	void SetStageInfo(int iStage_ );
	void InitStageInfo();

	void UpdateRitasObj();
	void GenerateObj();
	void CreateObj(int iObjID_ , D3DXVECTOR2 vPos_ , int iType );
	void MoveObj();
	void CheckPos(KGCRitasObj* pkObj_ , int nObjID_ , float fMoveRate_ );
	bool CheckCatchZone(float fPosY_ );
	void CheckCatchObj(KGCRitasObj* pkObj_ , int nObjID_ );

	void CountFrameMove();
	void SetStageFrameMove();
	void EndStageFramdMove();
	void PlayFrameMove();
	
	bool OnStageStart();


	void SetRitasState(eRitasState eState_ )	{ m_eNowState = eState_;  }

	void ResSetObj();

	void SetTickCntForCount( DWORD dwTickCnt_ );

	void SetNumber(int iNum_);
	
	void PopTopObj();
	void UpdateBasket();
	void ClearBasket();
	void ClearBasketEffect();

	bool CheckGoalZone();

	bool IsCleared(int iStageNum_ );

	bool EndStageCheck();
	void GameOverCheck();

	void DecreasLeftTime(float fTime_);
	void DecreasFeverLeftTime(float fTime_);

	void UpDateScore();
	void UpDateTimeProgressBar();
	void UpDateFeverObjAndBoxNum();

	void CreateParticle();
	void SetShowFeverParticle(bool bShow_ );
	void SetShowBackParticle(bool bShow_ );
	void SetShowAlramParticle(bool bShow_ );
	void SetShowSpaceParticle( bool bShow_);

	void AllSetParticle(bool bShow_);


private:

	bool m_bSetPlayInfo;

	int	m_iGotObj;

	int m_iGotFeverObj;

	KGCUINumber*	m_pkGameScore;
	KGCUINumber*	m_pkObjScore;

	eRitasState  m_eNowState;

	ePlayType	m_ePlayType;

	KGCRitasPlayer*	m_pkPlayer;

	KGCRitasGoalBasket* m_pkGoalBasket;

	KGCRitasProgressBar* m_pkProgressBar;
	KD3DWnd*	m_pkProgressBarRect;
	float		m_fValueXPerSec;

	KD3DWnd* m_pkObjIcon[MAX_OBJICON_NUM];

	KD3DStatic*	m_pkStaticStageNum;

	KD3DStatic*	m_pkStaticGoalObjNum;

	KD3DWnd*	m_pkStageRect;		//스테이지 영역
	KD3DWnd*	m_pkGoalRect;		//Space Bar 입력 간능 역역 체크 CheckGoalZone() 함수 참고

	float		m_fCatchZoneTop;
	float		m_fCatchZoneBottom;

	//<오브젝트 ID , 오브젝트 풀>
	std::map< int , std::vector<KGCRitasObj*> > m_mapObjPool;
	//<오브젝트 ID>
	std::vector<int>				m_vecBasket;
	//슬롯 포지션
	std::map<int,D3DXVECTOR2>		m_mapSlotPos;
	//클리어 한 스테이지넘버 
	std::vector<int>				m_vecClearStage;
	//ObjID 담아 놓는 컨테이너 , ID가 순차적으로 0,1,2 이런식으로 활용이 되지 않았을시를 대비 해서.
	std::vector<int>				m_vecNoamlObjID;
	std::vector<int>				m_vecFeverObjID;

	KD3DWnd*	m_pkBtnStart;

	KD3DWnd*	m_pkCnt[MAX_CNT_NUM];
	bool		m_bCountDownFlag[MAX_CNT_NUM];

	DWORD m_dwTickCntForCount;
	DWORD m_dwTickCntForPlay;
	DWORD m_dwTickCntForGenerate;


	float m_fLeftTime;
	float m_fLeftFeverTime;

	DWORD m_dwEndStageDelayTime;
	DWORD m_dwRevisionAtClearBasket;

	int m_iStartType;


	KRitasChristmasStageInfo m_kCurrenStageInfo;

	std::vector< CParticleEventSeqPTR > m_vecFeverEffect;
	std::vector< CParticleEventSeqPTR > m_vecBackEffect;
	std::vector< CParticleEventSeqPTR > m_vecAlramEffect;
	std::vector< CParticleEventSeqPTR > m_vecSpaceEffect;


	bool m_bClear;


	KD3DWnd*	m_pkImgGameover;
	KD3DWnd*	m_pkImgGmaeClear;

	KD3DWnd*	m_pkProgressBlinder;


	int			m_iTempFeverScore;	

	KD3DStatic*	m_pkStaticFeverObjNum;
	KD3DStatic*	m_pkStaticGotBoxInStage;

};

DEFINE_WND_FACTORY( KGCRitasPanel );
DECLARE_WND_FACTORY( KGCRitasPanel );
DECLARE_WND_FACTORY_NAME( KGCRitasPanel );
