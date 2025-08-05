#include "stdafx.h"
#include ".\gcresultscore.h"
//

IMPLEMENT_CLASSNAME( KGCResultScore );
IMPLEMENT_WND_FACTORY( KGCResultScore );
IMPLEMENT_WND_FACTORY_NAME( KGCResultScore, "gc_result_score" );

KGCResultScore::KGCResultScore(void)
: m_iKill( 0 )
, m_iDeath( 0 )
{

}

KGCResultScore::~KGCResultScore(void)
{
}

void KGCResultScore::OnCreate()
{
}

void KGCResultScore::SetKill( int iKill )
{
	if( m_iKill < 0 )
	{
		ASSERT( !"Kill에 음수가 가능한가요??" );
		m_iKill = 0;
	}
	m_iKill = iKill;
}

void KGCResultScore::SetDeath( int iDeath )
{
	if( iDeath < 0 )
	{
		ASSERT( !"Death에 음수가 가능한가요??" );
		iDeath = 0;
	}
	m_iDeath = iDeath;
}

void KGCResultScore::SetScore( int iKill, int iDeath )
{
	SetKill( iKill );
	SetDeath( iDeath );
}

void KGCResultScore::PostChildDraw()
{	

	D3DXVECTOR2 vPos;

	//vPos = GetFixedWindowPos();
	vPos = GetRelocatedWindowPos();
	g_pkUIMgr->RenderNumber( "small_num", vPos + D3DXVECTOR2( 70.0f * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 4.0f * m_fWindowScaleY * GC_SCREEN_DIV_HEIGHT), m_iKill, false, false );
	g_pkUIMgr->RenderNumber( "small_num", vPos + D3DXVECTOR2( 70.0f * m_fWindowScaleX * GC_SCREEN_DIV_WIDTH, 27.0f * m_fWindowScaleY * GC_SCREEN_DIV_HEIGHT), m_iDeath, false, false );
}

void KGCResultScore::Init()
{
	SetScore( 0, 0 );
}
