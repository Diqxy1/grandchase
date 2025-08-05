#include "StdAfx.h"
#include "KGCOlympicDicePlay_DiceView.h"


IMPLEMENT_CLASSNAME( KGCOlympicDicePlay_DiceView );
IMPLEMENT_WND_FACTORY( KGCOlympicDicePlay_DiceView );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicDicePlay_DiceView, "gc_dice_view" );

KGCOlympicDicePlay_DiceView::KGCOlympicDicePlay_DiceView(void)
{
	m_dwNum	= 1;
// 	m_bFresh = false;
// 	m_iFrame = 0;

	char str[256] = "";

	for( int i=0; i< MAX_DICE_NUM; ++i )
	{
		m_pkDice[i] = NULL;

		sprintf( str, "dice_%d", i);
		LINK_CONTROL( str, m_pkDice[i] );
	}
}

KGCOlympicDicePlay_DiceView::~KGCOlympicDicePlay_DiceView(void)
{
}

void KGCOlympicDicePlay_DiceView::OnCreate( void )
{
	for( int i=0; i< MAX_DICE_NUM; ++i )
	{
		if(m_pkDice[i])
			m_pkDice[i]->InitState( true );
	}

//	m_bFresh = true;
}

void KGCOlympicDicePlay_DiceView::OnDestroy( void )
{
}

void KGCOlympicDicePlay_DiceView::FrameMoveIn_DiceView( int iFrame )
{
	if( iFrame > 0 )
	{	
		if( iFrame%5 == 0 )
		MakeTopChild( m_pkDice[iFrame%6]);
	}
	else if( iFrame == 0 )
	{
		MakeTopChild( m_pkDice[m_dwNum-1] );
	}
}

void KGCOlympicDicePlay_DiceView::MakeTopChild( KD3DWnd* pWnd )
{
	std::vector<KD3DWnd*>::iterator vit = std::find( m_vecChild.begin(), m_vecChild.end(), pWnd );

	if( vit == m_vecChild.end() )
		return;

	m_vecChild.erase( vit );
	m_vecChild.push_back( pWnd );
}