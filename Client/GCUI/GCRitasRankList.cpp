#include "stdafx.h"
#include "GCRitasRankList.h"

IMPLEMENT_CLASSNAME( KGCRitasRankList );
IMPLEMENT_WND_FACTORY( KGCRitasRankList );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasRankList, "gc_ritasranklist" );

KGCRitasRankList::KGCRitasRankList( void ):
m_pkBtnScroll(NULL),
m_pkScrollRect(NULL)
{
	char szName[MAX_PATH];

	for (int i=0 ; i<NUM_SHOW_RANK ; i++)
	{
		m_pkRankInfo[i] = NULL;
		sprintf( szName, "rankinfo%d", i );
		LINK_CONTROL( szName, m_pkRankInfo[i] );
	}

	LINK_CONTROL( "btn_scroll", m_pkBtnScroll );
	LINK_CONTROL( "scroll_rect", m_pkScrollRect );

	m_iMaxStep = 0;
	m_iCurrentStep = 0;
	m_vecScrollPos.clear();

}

KGCRitasRankList::~KGCRitasRankList( void )
{
}

void KGCRitasRankList::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkScrollRect, KD3DWnd::D3DWE_BUTTON_CLICK, OnScoll );

}

void KGCRitasRankList::OnCreate( void )
{
	for (int i=0 ; i<NUM_SHOW_RANK ; i++)
		m_pkRankInfo[i]->InitState(false);

	m_pkBtnScroll->InitState(true);
	m_pkScrollRect->InitState(true,true,this);
}

void KGCRitasRankList::InitRankList()
{
	for (int i=0 ; i<NUM_SHOW_RANK ; i++)
		m_pkRankInfo[i]->ClearInfo();

	std::vector< KRitasChristmasRankInfo > vecRank = SiGCRitasChrismasManager()->GetRankInfoContainer();

	if ( vecRank.size() <= NUM_SHOW_RANK)
	{
		m_iMaxStep = 0;
	}
	else
	{
		m_iMaxStep = vecRank.size() - NUM_SHOW_RANK;
	}

	m_vecScrollPos.clear();
	m_iCurrentStep = 0;

	DWORD dwScrollRectHeight = m_pkScrollRect->GetHeight() - m_pkBtnScroll->GetHeight();

	float fGap = static_cast<float>(dwScrollRectHeight)/static_cast<float>(m_iMaxStep);

	if (m_iMaxStep != 0)
	{
		D3DXVECTOR2 vPos = m_pkScrollRect->GetFixedWindowLocalPos();
		while( vPos.y <= dwScrollRectHeight )
		{
			m_vecScrollPos.push_back(vPos);
			vPos.y += fGap;
		}
	}
	
	m_pkBtnScroll->SetFixedWindowLocalPos(m_pkScrollRect->GetFixedWindowLocalPos());
	
	std::vector< KRitasChristmasRankInfo >::iterator vit = vecRank.begin();
	int k=0;
	while( k <NUM_SHOW_RANK && vit != vecRank.end() )
	{
		m_pkRankInfo[k]->SetRankInfo( vit->m_wsNickName , vit->m_nMaxPoint , k);
		k++;
		++vit;
	}
}

void KGCRitasRankList::OnScoll()
{
	if (m_iMaxStep == 0)
		return;
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vScrollPos = m_pkBtnScroll->GetCurrentWindowPos();

	if ( (static_cast<float>(pt.y) > vScrollPos.y) && (m_iCurrentStep < static_cast<int>(m_vecScrollPos.size()-1)) )
		m_iCurrentStep++;

	if ( (static_cast<float>(pt.y) < vScrollPos.y) && (m_iCurrentStep > 0 )  )
		m_iCurrentStep--;


	UpdateRankInfo();
}

void KGCRitasRankList::UpdateRankInfo()
{
	if (m_iCurrentStep > static_cast<int>(m_vecScrollPos.size()-1) || m_iCurrentStep < 0)
		return;
	m_pkBtnScroll->SetFixedWindowLocalPos(m_vecScrollPos[m_iCurrentStep]);
	std::vector< KRitasChristmasRankInfo > vecRank = SiGCRitasChrismasManager()->GetRankInfoContainer();
	std::vector< KRitasChristmasRankInfo >::iterator vit = vecRank.begin();

	for (int k=0 ; k<m_iCurrentStep; k++)
	{
		//안전코드
		if (vit == vecRank.end())
			return;

		++vit;
	}
	int i=0;
	while( i<NUM_SHOW_RANK && vit != vecRank.end())
	{
		m_pkRankInfo[i]->SetRankInfo( vit->m_wsNickName , vit->m_nMaxPoint  , i + m_iCurrentStep );
		i++;
		++vit;
	}
	
}

void KGCRitasRankList::FrameMoveInEnabledState()
{
	if (m_iMaxStep == 0)
		return;

	if (g_pkInput->GetZMov() < 0 && m_iCurrentStep < static_cast<int>(m_vecScrollPos.size()-1))
	{
		m_iCurrentStep++;
		UpdateRankInfo();
	}
	
	if (g_pkInput->GetZMov() > 0 && m_iCurrentStep > 0 )
	{
		m_iCurrentStep--;
		UpdateRankInfo();
	}
				

}
