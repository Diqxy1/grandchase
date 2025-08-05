#pragma once

#define NUM_DICE 2
#define MAX_BOARD_NUM 24



class KGCOlympicDicePlay_DiceView;

class KGCOlympicDicePlay : public KD3DDialog,      // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCOlympicDicePlay );

	KGCOlympicDicePlay(void);
	virtual ~KGCOlympicDicePlay(void);
	/// Copy constructor
	KGCOlympicDicePlay( const KGCOlympicDicePlay& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCOlympicDicePlay& operator=( const KGCOlympicDicePlay& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum PlayingState
	{
		DEFAULT = -1,
		REFRESH,
		RECEIVEITEM,
		MOVEPLAYER,
		PLAYINGDICE,
	};

public:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void ActionPerformed( const KActionEvent& event );
	virtual void OnDestroy( void );
    
	void OnExit();
	void OnDicePlay();
    void SetMyBoardInfo( KEVENT_OLYMPIC_JOIN_GAME_ACK kResult );
	void PlayResult( KEVENT_OLYMPIC_DICEPLAY_RESULT_ACK kResult );  
	void SetStatic();
    void SetCloseBtn();

	void SetDiceCount( DWORD dwDiceCount ){	m_dwNumDice = dwDiceCount; }
	void SetMyPosition();
	void SetState( int iState ){ m_iState = iState; }
	int GetState( ){ return m_iState; }

private:
	DWORD			m_dwCurrentPosition;
	DWORD			m_dwTotalCount;                         
	DWORD			m_dwRemainCount;
	DWORD			m_dwNumDice;

	DWORD			m_dwMovePosition;                        

	int			    m_iState;
	int			    m_iFrame;

	KD3DWnd*		m_pkCloseBtn;
	KD3DWnd*		m_pkPlayeBtn;
	KD3DWnd*		m_pkMyPlayer;
	KGCItemImgWnd*	m_pkBoardItem[MAX_BOARD_NUM];
	KD3DStatic*		m_pkStaticGoal;
	KD3DStatic*		m_pkStaticLimit;
	KD3DStatic*		m_pkStaticDice;


    time_t          m_tmTodayDate;      
    int             m_nGameType;        
    int             m_nTeamType;        
    

public:
	KGCOlympicDicePlay_DiceView*	m_pkDice[NUM_DICE];
};

DEFINE_WND_FACTORY( KGCOlympicDicePlay );
DECLARE_WND_FACTORY( KGCOlympicDicePlay );
DECLARE_WND_FACTORY_NAME( KGCOlympicDicePlay );
