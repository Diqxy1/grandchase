#include "stdafx.h"
#include "GCRitasRankInfo.h"

IMPLEMENT_CLASSNAME( KGCRitasRankInfo );
IMPLEMENT_WND_FACTORY( KGCRitasRankInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasRankInfo, "gc_ritasrankinfo" );

KGCRitasRankInfo::KGCRitasRankInfo( void )
{
	char szName[MAX_PATH];
	for (int i=0 ; i<NUM_SPECIAL_GRADE ; i++)
	{
		m_pkGrade[i] = NULL;
		sprintf( szName, "rank%d", i );
		LINK_CONTROL( szName, m_pkGrade[i] );
	}
	LINK_CONTROL( "static_nickname", m_pkStaticNickname );
	LINK_CONTROL( "static_score", m_pkStaticScore );

	m_pkRank = NULL;
	LINK_CONTROL( "static_rank", m_pkRank );

}

KGCRitasRankInfo::~KGCRitasRankInfo( void )
{
}

void KGCRitasRankInfo::ActionPerformed( const KActionEvent& event )
{

}

void KGCRitasRankInfo::OnCreate( void )
{
	for (int i=0 ; i<NUM_SPECIAL_GRADE ; i++)
		m_pkGrade[i]->InitState(false);

	m_pkStaticNickname->SetAlign(DT_LEFT);
	m_pkStaticScore->SetAlign(DT_LEFT);

	m_pkRank->InitState(true);
	m_pkRank->SetStringType("ritas_rank_num");
	m_pkRank->SetNumber(0);
}

void KGCRitasRankInfo::SetRankInfo(  std::wstring wstrNickName_, int iScore_ , int iGrade_ )
{
	ClearInfo();

	if (iGrade_<NUM_SPECIAL_GRADE && iGrade_>-1)
	{
		m_pkGrade[iGrade_]->ToggleRender(true);
		m_pkRank->ToggleRender(false);
	}
	else
	{
		m_pkRank->ToggleRender(true);
		m_pkRank->SetNumber(iGrade_+1);
	}

	m_pkStaticNickname->SetText(wstrNickName_);
	m_pkStaticScore->SetText(iScore_);

	ToggleRender(true);

}

void KGCRitasRankInfo::ClearInfo()
{
	for (int i=0 ; i<NUM_SPECIAL_GRADE ; i++)
		m_pkGrade[i]->ToggleRender(false);

	m_pkStaticNickname->SetText(L" ");
	m_pkStaticScore->SetText(L" ");
	m_pkRank->SetNumber(0);

	ToggleRender(false);
}
