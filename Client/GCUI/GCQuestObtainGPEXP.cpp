#include "stdafx.h"
#include ".\gcquestobtaingpexp.h"
#include "GCQuestRankScore.h"


IMPLEMENT_CLASSNAME( KGCQuestObtainGPEXP );
IMPLEMENT_WND_FACTORY( KGCQuestObtainGPEXP );
IMPLEMENT_WND_FACTORY_NAME( KGCQuestObtainGPEXP, "gc_quest_result_obtain_gp_exp" );

KGCQuestObtainGPEXP::KGCQuestObtainGPEXP(void)
{
    m_pkEXPScore        = NULL;
    m_pkGPScore         = NULL;  

    LINK_CONTROL( "background_exp",      m_pkEXPScore );    
    LINK_CONTROL( "background_gp",       m_pkGPScore );
}

KGCQuestObtainGPEXP::~KGCQuestObtainGPEXP(void)
{
}

void KGCQuestObtainGPEXP::OnCreate( void )
{
    m_pkEXPScore->InitState( true, false, this );
    m_pkGPScore->InitState( true, false, this ); 
}

void KGCQuestObtainGPEXP::OnDestroy( void )
{

}

void KGCQuestObtainGPEXP::FrameMoveInEnabledState( void )
{

}

void KGCQuestObtainGPEXP::PostChildDraw( void )
{

}

void KGCQuestObtainGPEXP::SetGP(int iGp, bool bDirect /* = false*/)
{
    if( iGp < 0 )
        iGp = 0;

    if( bDirect )
        m_pkGPScore->SetScore(iGp);
    else
        m_pkGPScore->SetLinearScore(iGp);
}

void KGCQuestObtainGPEXP::SetEXP( GCEXPTYPE biExp, bool bDirect /* = false*/ )
{
    if( biExp < 0 )
        biExp = 0;

    if( bDirect )
        m_pkEXPScore->SetScore( biExp );
    else
        m_pkEXPScore->SetLinearScore( biExp );
}


bool KGCQuestObtainGPEXP::isComplete(void)
{
    return m_pkEXPScore->IsScoreAnimComplete();
}

int KGCQuestObtainGPEXP::GetGp()
{ 
    return (int)m_pkGPScore->GetScore();
}

GCEXPTYPE KGCQuestObtainGPEXP::GetExp()
{ 
    return m_pkEXPScore->GetScore(); 
}

void KGCQuestObtainGPEXP::SetExpGpColor( DWORD dwColor_ )
{
    m_pkEXPScore->SetColor(dwColor_);
    m_pkGPScore->SetColor(dwColor_);
}
