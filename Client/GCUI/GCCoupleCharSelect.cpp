#include "StdAfx.h"
#include "GCCoupleCharSelect.h"

IMPLEMENT_CLASSNAME( KGCCoupleCharSelect );
IMPLEMENT_WND_FACTORY( KGCCoupleCharSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleCharSelect, "gc_couple_char_select" );

KGCCoupleCharSelect::KGCCoupleCharSelect(void)
: m_bFirst( false )
, m_iCurIndex( 0 )
{
	for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
	{
		m_pkChar[i] = NULL;
		char temp[MAX_PATH];
		sprintf( temp, "char_%d", i );
		LINK_CONTROL( temp, m_pkChar[i] );
	}

	m_pkBtnUp = NULL;
	m_pkBtnDown = NULL;
	LINK_CONTROL( "btn_up", m_pkBtnUp );
	LINK_CONTROL( "btn_down", m_pkBtnDown );
}

KGCCoupleCharSelect::~KGCCoupleCharSelect(void)
{
}

void KGCCoupleCharSelect::OnCreate( void )
{
	if( m_bFirst == false )
	{
		m_bFirst = true;
		InitPosition();
	}

	for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
	{
		m_pkChar[i]->InitState( i < 3, true, this );
	}
	m_pkBtnUp->InitState( true, true, this );
	m_pkBtnDown->InitState( true, true, this );
	m_iCurIndex = 0;

	Update();
}

void KGCCoupleCharSelect::ActionPerformed( const KActionEvent& event )
{
	for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
	{
		if( event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
			break;

		if( event.m_pWnd != m_pkChar[i] )
			continue;

		OnClickCharBtn( i );
		break;
	}

	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK &&
		event.m_pWnd == m_pkBtnUp )
	{
		OnClickUpDownBtn( true );
	}

	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK &&
		event.m_pWnd == m_pkBtnDown )
	{
		OnClickUpDownBtn( false );
	}
}

void KGCCoupleCharSelect::InitPosition()
{
	for( int i = 0 ; i < 3 ; ++i )
	{
		m_vPos[i] = m_pkChar[i]->GetFixedWindowLocalPos();
	}
}

void KGCCoupleCharSelect::OnClickCharBtn( int i )
{
	SpeakToActionListener( KActionEvent( this, GCCCS_CHAR_BTN_CLICK, i ) );
}

void KGCCoupleCharSelect::OnClickUpDownBtn( bool bUp )
{
	bUp?--m_iCurIndex:++m_iCurIndex;

	if( m_setCharacters.empty() )
		return;

	FixCharacterRange( m_iCurIndex );

	if( m_setCharacters.find(m_iCurIndex) == m_setCharacters.end() )
	{
		OnClickUpDownBtn( bUp );
		return;
	}

	Update();
}

void KGCCoupleCharSelect::InitCharacterSet()
{
	m_setCharacters.clear();
	m_setCharacters.insert( GC_CHAR_ELESIS );
	m_setCharacters.insert( GC_CHAR_LIRE );
	m_setCharacters.insert( GC_CHAR_ARME );
	m_setCharacters.insert( GC_CHAR_RYAN );
}

void KGCCoupleCharSelect::ExcludeCharacter( int iCharType )
{
	ASSERT( iCharType >= 0 && iCharType < GC_CHAR_NUM );
	m_setCharacters.erase( iCharType );
}

void KGCCoupleCharSelect::IncludeCharacter( int iCharType )
{
	ASSERT( iCharType >= 0 && iCharType < GC_CHAR_NUM );
	m_setCharacters.insert( iCharType );
}

void KGCCoupleCharSelect::Update()
{
	if( m_bFirst == false )
	{
		m_bFirst = true;
		InitPosition();
	}

	if( m_setCharacters.empty() )
		return;

	FixCharacterRange( m_iCurIndex );

	for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
	{
		m_pkChar[i]->ToggleRender(false);
	}

	int iCharCount = 0;
	for( int i = 0 ; i < GC_CHAR_NUM && iCharCount < 3 ; ++i )
	{
		int iCharacter = (m_iCurIndex+i)%GC_CHAR_NUM;
		if( m_setCharacters.find( iCharacter ) == m_setCharacters.end() )
			continue;

		m_pkChar[iCharacter]->ToggleRender( true );
		m_pkChar[iCharacter]->SetFixedWindowLocalPos( m_vPos[iCharCount] );
		++iCharCount;
	}
}

void KGCCoupleCharSelect::FixCharacterRange( int& iCharType )
{
	if( iCharType < 0 )
		iCharType = GC_CHAR_NUM - 1;

	if( iCharType >= GC_CHAR_NUM )
		iCharType = 0;
}