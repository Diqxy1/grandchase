#pragma once
#define MAX_DICE_NUM 6

class KGCOlympicDicePlay_DiceView : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCOlympicDicePlay_DiceView );

	KGCOlympicDicePlay_DiceView(void);
	virtual ~KGCOlympicDicePlay_DiceView(void);
	/// Copy constructor
	KGCOlympicDicePlay_DiceView( const KGCOlympicDicePlay_DiceView& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCOlympicDicePlay_DiceView& operator=( const KGCOlympicDicePlay_DiceView& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:
	virtual void OnCreate( void );
	virtual void OnDestroy( void );
	void FrameMoveIn_DiceView( int iFrame );
	void SetDiceNum( DWORD dwNum){ if( !(dwNum>MAX_DICE_NUM||dwNum<=0) ) m_dwNum = dwNum; }
	DWORD GetDiceNum(){return m_dwNum;}
	void MakeTopChild( KD3DWnd* pWnd );


private:
	KD3DWnd*	m_pkDice[MAX_DICE_NUM];
 	DWORD		m_dwNum;

};

DEFINE_WND_FACTORY( KGCOlympicDicePlay_DiceView );
DECLARE_WND_FACTORY( KGCOlympicDicePlay_DiceView );
DECLARE_WND_FACTORY_NAME( KGCOlympicDicePlay_DiceView );
