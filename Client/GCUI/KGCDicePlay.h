#pragma once

#define MAX_RANKING 1
#define NUM_DICE 2
#define MAX_BOARD_NUM 24

class KGCDicePlay_DiceView;

class KGCDicePlay : public KD3DDialog,      // extends
	public KActionListener    // implements
{
public:
	DECLARE_CLASSNAME( KGCDicePlay );

	KGCDicePlay(void);
	virtual ~KGCDicePlay(void);
	/// Copy constructor
	KGCDicePlay( const KGCDicePlay& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCDicePlay& operator=( const KGCDicePlay& )
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

	void InitNumberTextureCoordinate();
	void OnExit();
	void OnDicePlay();
	void OnRecive();
	void PlayResult( KEVENT_DICE_PLAY_ACK kResult );
	void SetMyBoardInfo( KUserBoardInfo kBoardinfo );
	void SettingBoard();
	void SetPlayTime();
	void SetStatic();
	void SetBoardItemInfo( std::map<DWORD, DWORD> mapBoardInfo_, std::map<DWORD, std::vector<KDropItemInfo> > mapRewardInfo_ );
    void SetFreeDiceTick( DWORD dwFreeDiceTick_ ) { m_dwFreeDiceTick = dwFreeDiceTick_; }

	void SetDiceCount( DWORD dwDiceCount ){	m_dwNumDice = dwDiceCount; }
	void SetRanker( std::vector<KDicePlayRank> vecRanker_ ){ m_vecRanker = vecRanker_;}
	void SetMyPosition();
	void SetRankerPosition();
	void SetState( int iState ){ m_iState = iState; }
	int GetState( ){ return m_iState; }

private:
	DWORD			m_dwCurrentPosition;
	DWORD			m_dwTotalCount;
	DWORD			m_dwRemainCount;
	DWORD			m_dwGiveCount;
    DWORD           m_dwFreeDiceTick;
	DWORD			m_dwRemainTime;
	DWORD			m_dwNumDice;

	DWORD			m_dwMovePosition;

	DWORD			m_dwAccTimeNot;

	std::map<DWORD, DWORD> m_mapBoardInfo;
	std::vector<KDicePlayRank> m_vecRanker;
	std::vector<KItem>         m_vecReciveItem;              // 주사위를 굴린후 획득한 비수량 아이템 리스트.

	int			m_iState;
	int			m_iFrame;
	int			m_iErrorStringNum;

	KD3DWnd*		m_pkCloseBtn;
	KD3DWnd*		m_pkPlayeBtn;
	KD3DWnd*		m_pkMyPlayer;
	KD3DWnd*		m_pkRanker[MAX_RANKING];
	KD3DNumber*		m_pkNumber[MAX_RANKING][2];
	KGCItemImgWnd*	m_pkBoardItem[MAX_BOARD_NUM];
	KD3DStatic*		m_pkStaticMin;
	KD3DStatic*		m_pkStaticSec;
	KD3DStatic*		m_pkStaticGoal;
	KD3DStatic*		m_pkStaticLimit;
	KD3DStatic*		m_pkStaticDice;

public:
	KD3DWnd*		m_pkReciveBtn;
	KGCDicePlay_DiceView*	m_pkDice[NUM_DICE];
	bool			m_bNoFreeDice;
	bool			m_bRcvDice;
};

DEFINE_WND_FACTORY( KGCDicePlay );
DECLARE_WND_FACTORY( KGCDicePlay );
DECLARE_WND_FACTORY_NAME( KGCDicePlay );
