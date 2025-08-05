#include "stdafx.h"
#include ".\gcui\gcindigoranking.h"
//
//
//
#include "GCIndigoRankingElement.h"

IMPLEMENT_CLASSNAME( KGCIndigoRanking );
IMPLEMENT_WND_FACTORY( KGCIndigoRanking );
IMPLEMENT_WND_FACTORY_NAME( KGCIndigoRanking, "gc_indigo_ranking" );

const int INDIGO_RANKING_SIZE = 10;

KGCIndigoRanking::KGCIndigoRanking(void)
:m_pkPageRight(NULL)
,m_pkPageLeft(NULL)
,m_pkPageStatic(NULL)
,m_iPage( 0 )
,m_iTotalPage( 1 )
{
	LINK_CONTROL( "indigo_page_right", m_pkPageRight );
	LINK_CONTROL( "indigo_page_left", m_pkPageLeft );
	LINK_CONTROL( "indigo_page_static", m_pkPageStatic );

	char strName[MAX_PATH] = {0,};
	for( int i = 0 ; i < GC_INDIGO_RANKING_NUM ; ++i )
	{
		m_pkRankingElement[i] = NULL;
		sprintf( strName, "indigo_rank_element%d", i + 1 );
		LINK_CONTROL( strName, m_pkRankingElement[i] );
	}
}

KGCIndigoRanking::~KGCIndigoRanking(void)
{
}

void KGCIndigoRanking::OnCreate()
{
	m_pkPageRight->InitState( true, true, this );
	m_pkPageLeft->InitState( true, true, this );
	m_pkPageStatic->InitState( true, true, this );
	m_pkPageStatic->SetAlign( DT_CENTER );

	for( int i = 0 ; i < GC_INDIGO_RANKING_NUM ; ++i )
	{
		m_pkRankingElement[i]->InitState( true, true );
		m_pkRankingElement[i]->SetEmpty();
	}
}

void KGCIndigoRanking::ActionPerformed( const KActionEvent& event )
{
	GCWND_CODE_MAP( m_pkPageRight, OnPageRight );
	GCWND_CODE_MAP( m_pkPageLeft, OnPageLeft );
}

void KGCIndigoRanking::OnPageRight( DWORD dwCode )
{
	if( dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
		return;
	
	if( (int)g_kGlobalValue.GetIndigoRank().size() > (m_iPage + 1 ) * INDIGO_RANKING_SIZE )
	{
		++m_iPage;
	}

	UpdateRankingData( m_iPage );
}

void KGCIndigoRanking::OnPageLeft( DWORD dwCode )
{

	if( dwCode != KD3DWnd::D3DWE_BUTTON_CLICK )
		return;

	if( m_iPage > 0 )
	{
		--m_iPage;
	}

	UpdateRankingData( m_iPage );
}

void KGCIndigoRanking::UpdateRankingData( int iPage )
{
	for( int i = 0 ; i < INDIGO_RANKING_SIZE ; ++i )
	{
		m_pkRankingElement[i]->SetEmpty();
	}
	for( int i = 0 ; i < INDIGO_RANKING_SIZE && (int)g_kGlobalValue.GetIndigoRank().size() > iPage * INDIGO_RANKING_SIZE + i ; ++i )
	{
		m_pkRankingElement[i]->SetElementData( g_kGlobalValue.GetIndigoRank()[iPage * INDIGO_RANKING_SIZE + i] );
	}
	UpdatePageStatic( m_iPage, m_iTotalPage );
}

void KGCIndigoRanking::UpdateRankingData()
{
	const int iRankingSize = (int)g_kGlobalValue.GetIndigoRank().size();
	m_iTotalPage =  iRankingSize / INDIGO_RANKING_SIZE - (iRankingSize%INDIGO_RANKING_SIZE == 0?1:0);
	UpdateRankingData(0);
}

void KGCIndigoRanking::UpdatePageStatic( int iPage , int iTotalPage )
{
	std::wstring strPage= boost::str( boost::wformat(L"%d/%d")%(iPage+1)%(iTotalPage+1) );
	m_pkPageStatic->SetText(strPage);
}