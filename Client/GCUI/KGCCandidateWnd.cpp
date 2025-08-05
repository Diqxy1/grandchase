#include "stdafx.h"
#include "KGCCandidateWnd.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCCandidateWnd );
IMPLEMENT_WND_FACTORY( KGCCandidateWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCCandidateWnd, "gc_candidate_window" );

#define MIN_CAND_WIDTH		100
#define CAND_STR_GAP		20
#define MAX_CAND_HEIGHT		200

KGCCandidateWnd::KGCCandidateWnd(void)
{
	for(int i=0 ; i<MAX_CANDIDATE_SIZE ; ++i)
		m_pCandidate[i] = NULL;

	//m_pPage = NULL;

	m_iCurrCandidate = 0;
	LINK_CONTROL( "candidate_bg", m_pSizingBox    );
	LINK_CONTROL( "candidate1",   m_pCandidate[0] );
	LINK_CONTROL( "candidate2",   m_pCandidate[1] );
	LINK_CONTROL( "candidate3",   m_pCandidate[2] );
	LINK_CONTROL( "candidate4",   m_pCandidate[3] );
	LINK_CONTROL( "candidate5",   m_pCandidate[4] );
	//LINK_CONTROL( "candidate6",   m_pCandidate[5] );
	//LINK_CONTROL( "candidate7",   m_pCandidate[6] );
	//LINK_CONTROL( "candidate8",   m_pCandidate[7] );
	//LINK_CONTROL( "candidate9",   m_pCandidate[8] );

	//LINK_CONTROL( "page",   m_pPage );
}

KGCCandidateWnd::~KGCCandidateWnd(void)
{
}

void KGCCandidateWnd::OnCreate()
{
	m_iCurrCandidate = 0;

	for(int i=0 ; i<MAX_CANDIDATE_SIZE ; ++i)
		m_pCandidate[i]->SetFontColor(0xffffffff);

	m_pCandidate[m_iCurrCandidate]->SetFontColor(0xffffff00);

	//m_pPage->SetFontColor(0xffff0000);
	//m_pPage->SetAlign( DT_CENTER );
}

//활성화된 Candidate갱신
void KGCCandidateWnd::SetCurrCandidate(int iIndex)
{	
	if ( iIndex < 0 || iIndex > MAX_CANDIDATE_SIZE - 1 )
		return;

	m_pCandidate[m_iCurrCandidate]->SetFontColor(0xffffffff);
	m_pCandidate[iIndex]->SetFontColor(0xffffff00);

	m_iCurrCandidate = iIndex;
}

void KGCCandidateWnd::SetCandidate(int iIndex, WCHAR* strCandidate)
{
	static int s_iMaxWidth = 0;

	if ( iIndex < 0 || iIndex > MAX_CANDIDATE_SIZE - 1 )
		return;

	if ( s_iMaxWidth < g_pkFontMgr->GetWidth( strCandidate ) )
		s_iMaxWidth = g_pkFontMgr->GetWidth( strCandidate );

	if ( iIndex == MAX_CANDIDATE_SIZE - 1 )
	{
		if ( s_iMaxWidth < MIN_CAND_WIDTH - CAND_STR_GAP )
			s_iMaxWidth = MIN_CAND_WIDTH - CAND_STR_GAP;
		m_pSizingBox->SetSize( s_iMaxWidth + CAND_STR_GAP , m_pSizingBox->GetHeight() );
		s_iMaxWidth = 0;
	}
	m_pCandidate[iIndex]->SetText(strCandidate);
}

//void KGCCandidateWnd::SetPage(int iCurrPage, int iTotalPage)
//{
//	WCHAR strPage[20];
//	swprintf( strPage, 20, L"%d / %d", iCurrPage, iTotalPage );
//
//	m_pPage->SetText( strPage );
//}

void KGCCandidateWnd::SetPosition( D3DXVECTOR2 vPos, float fTextHeight )
{
	if( vPos.x + m_pSizingBox->GetWidth() + 10 > g_MyD3D->m_dwCreationWidth )
		vPos.x = vPos.x - m_pSizingBox->GetWidth();

	if( vPos.y + m_pSizingBox->GetHeight() > g_MyD3D->m_dwCreationHeight )
		vPos.y = vPos.y - m_pSizingBox->GetHeight() - fTextHeight;
	else
		vPos.y += fTextHeight;


	m_vWindowPos = vPos;
}

void KGCCandidateWnd::OnDestroy()
{
}