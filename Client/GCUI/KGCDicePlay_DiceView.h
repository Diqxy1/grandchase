#pragma once
#define MAX_DICE_NUM 6

class KGCDicePlay_DiceView : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCDicePlay_DiceView );

	KGCDicePlay_DiceView(void);
	virtual ~KGCDicePlay_DiceView(void);
	/// Copy constructor
	KGCDicePlay_DiceView( const KGCDicePlay_DiceView& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCDicePlay_DiceView& operator=( const KGCDicePlay_DiceView& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:
	virtual void OnCreate( void );
	virtual void OnDestroy( void );
//	virtual void FrameMoveInEnabledState( void );

	void FrameMoveIn_DiceView( int iFrame );

	void SetDiceNum( DWORD dwNum){ if( !(dwNum>MAX_DICE_NUM||dwNum<=0) ) m_dwNum = dwNum; }
	DWORD GetDiceNum(){return m_dwNum;}
	void MakeTopChild( KD3DWnd* pWnd );
// 	void SetFrame( int iFrame){ m_iFrame = iFrame;}
// 	int GetFrame(){ return m_iFrame; }

private:
	KD3DWnd*	m_pkDice[MAX_DICE_NUM];

 	DWORD		m_dwNum;
// 	bool		m_bFresh;
// 	int			m_iFrame;
};

DEFINE_WND_FACTORY( KGCDicePlay_DiceView );
DECLARE_WND_FACTORY( KGCDicePlay_DiceView );
DECLARE_WND_FACTORY_NAME( KGCDicePlay_DiceView );
