#pragma once

class KGCOlympicGawibawiboGameInfo;
class KGCOlympicGawibawiboGamePlay;
class KGCOlympicGawibawiboGameLose;


class KGCOlympicGawibawiboGame : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCOlympicGawibawiboGame );
    /// Default constructor
    KGCOlympicGawibawiboGame( void );
    /// Default destructor
    virtual ~KGCOlympicGawibawiboGame( void );
    /// Copy constructor
    KGCOlympicGawibawiboGame( const KGCOlympicGawibawiboGame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCOlympicGawibawiboGame& operator=( const KGCOlympicGawibawiboGame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum GAWIBAWIBO_STATE{
		GAWIBAWIBO_STATE_DEFAULT	= 0,	// 도전 대기 상태
		GAWIBAWIBO_STATE_GAME		= 1,	// 게임 상태
        GAWIBAWIBO_STATE_CHALLENGE  = 2,
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void		ChangeState();
	void		ClickChallengeBtn();
	void		ClickGawibawiboBtn();
	void		ClickRetryBtn();
    void		SetGawibawiboGameInfo(const KEVENT_OLYMPIC_JOIN_GAME_ACK &recvInfo_);
    void		ResultGawiBawiBoGame(const KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK &recvInfo_);
    void        SetGameInfo(int iConinCount, int iAccumulateData );
    void        SetCloseBtn();

private:
	KD3DWnd		*m_pkBtnClose;

	KGCOlympicGawibawiboGameInfo		*m_pkGameInfoBox;
	KGCOlympicGawibawiboGamePlay		*m_pkGamePlay;
	KGCOlympicGawibawiboGameLose		*m_pkGameLose;

    int         m_iCoinCount;                   //코인 카운터
    int         m_iAccumulateData;              //누적 데이터

	int			m_iStateValue;		            //재도전 상태인가 아님 대기 상태인가??		
	int			m_iState;		                //보드판 상태

    KD3DStatic*	m_pkStaticChallengePoint;
    KD3DStatic*	m_pkStaticClearCnt;

};

DEFINE_WND_FACTORY( KGCOlympicGawibawiboGame );
DECLARE_WND_FACTORY( KGCOlympicGawibawiboGame );
DECLARE_WND_FACTORY_NAME( KGCOlympicGawibawiboGame );
