#pragma once

class KGCGawibawiboGameInfo;
class KGCGawibawiboGameBossInfo;
class KGCGawibawiboGamePlay;
class KGCGawibawiboGameLose;
class KGCGawibawiboItemView;


class KGCGawibawiboGame : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGawibawiboGame );
    /// Default constructor
    KGCGawibawiboGame( void );
    /// Default destructor
    virtual ~KGCGawibawiboGame( void );
    /// Copy constructor
    KGCGawibawiboGame( const KGCGawibawiboGame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGawibawiboGame& operator=( const KGCGawibawiboGame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum GAWIBAWIBO_STATE{
		GAWIBAWIBO_STATE_DEFAULT	= 0,	// 도전 대기 상태
		GAWIBAWIBO_STATE_GAME		= 1,	// 게임 상태
		GAWIBAWIBO_STATE_CHALLENGE	= 2,	// 재도전 확인 상태
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void		ChangeState();
	int			GetVerInfo(){return m_iVer;}
	void		SetGawibawiboGameInfo(const KEVENT_GAWIBAWIBO_INFO_ACK &recvInfo_);
	void		ClickChallengeBtn();
	void		ResultGawiBawiBoGame(const KEVENT_GAWIBAWIBO_ACK &recvInfo_);
	void		ClickGawibawiboBtn();
	void		ClickRetryBtn();
	void		SetGawibawiboTryPointInfo(DWORD &recvInfo_);

    void        Close();
    
    KGCGawibawiboGamePlay* GetGamePlay()    { return m_pkGamePlay; }
private:
	KD3DWnd		*m_pkBtnViewItem;
	KD3DWnd		*m_pkBtnBuyItem;
	KD3DWnd		*m_pkBtnClose;
	
	KGCGawibawiboGameInfo		*m_pkGameInfoBox;
	KGCGawibawiboGameBossInfo	*m_pkGameBossInfo;
	KGCGawibawiboGamePlay		*m_pkGamePlay;
	KGCGawibawiboGameLose		*m_pkGameLose;
	KGCGawibawiboItemView		*m_pkGameItemView;

	KD3DStatic*	m_pkStaticDescription1;
	KD3DStatic*	m_pkStaticDescription2;
	KD3DStatic*	m_pkStaticDescription3;
	KD3DStatic*	m_pkStaticDescription4;

	int			m_iStateValue;		//재도전 상태인가 아님 대기 상태인가??		
	int			m_iVer;			//보드판 정보를 가져올 수 있는 버젼 정보
	int			m_iState;		//보드판 상태		
	int			m_iCurrentPosition; //현재 말의 위치 
	int			m_iMaxBoss;			//최대 보스 

	std::vector<KItem>			m_vecRevItemList;

	std::map< std::pair< DWORD, DWORD >, VEC_REWARD > m_mapWinReward; // 승리 보상 : < 스테이지, 몬 번호 >, 보상리스트 

};

DEFINE_WND_FACTORY( KGCGawibawiboGame );
DECLARE_WND_FACTORY( KGCGawibawiboGame );
DECLARE_WND_FACTORY_NAME( KGCGawibawiboGame );
