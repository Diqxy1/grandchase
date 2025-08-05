#include "stdafx.h"
#include "GCCharGainScene.h"

IMPLEMENT_CLASSNAME( KGCCharGainScene );
IMPLEMENT_WND_FACTORY( KGCCharGainScene );
IMPLEMENT_WND_FACTORY_NAME( KGCCharGainScene, "gc_char_gain_scene" );

KGCCharGainScene::KGCCharGainScene( void )
{
	m_pkCharIntroFrame = NULL;
	LINK_CONTROL( "char_intro_frame", m_pkCharIntroFrame );
}

KGCCharGainScene::~KGCCharGainScene( void )
{
	
}

void KGCCharGainScene::ActionPerformed( const KActionEvent& event )
{
	
}

void KGCCharGainScene::OnCreate( void )
{
    if( m_pkCharIntroFrame ) { 
        m_pkCharIntroFrame->InitState(false);
    }
}

void KGCCharGainScene::OnCreateComplete( void )
{
    if( m_pkCharIntroFrame ) { 
        m_pkCharIntroFrame->OnInitialize();
        m_pkCharIntroFrame->InitState( true, true, this );
    }
}

void KGCCharGainScene::Add_NewsFrame_Particle( std::string seq,float x/*=0.0f*/,float y/*=0.0f*/ )
{
	CParticleEventSeqPTR pParticle = NULL;
	pParticle = g_ParticleManager->CreateSequence( seq, x, y, 0.5f );
	if( pParticle )
	{
		g_ParticleManager->SetLayer( pParticle, GC_LAYER_CUSTOM_UI );
		m_vecServerSceneEffect.push_back( pParticle );
	}
}

void KGCCharGainScene::RenderEffect( bool bRender )
{
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();

	for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
	{
		if ((*iterPtr))
			(*iterPtr)->SetShow(bRender);
	}

}

void KGCCharGainScene::CreateEffect()
{
	Add_NewsFrame_Particle("Map_Intro_01");
}

void KGCCharGainScene::EnterScene()
{
	KGCScene::EnterScene();
    CreateEffect();
	RenderEffect(true);
}

void KGCCharGainScene::FrameMoveInEnabledState( void )
{

}


HRESULT KGCCharGainScene::Render( void )
{
	if ( false == IsRenderOn() ) return S_OK;
	if ( true == IsNotDraw() ) return S_OK;
	if ( D3DWS_DISABLED == GetWndState() ) return S_OK;

	m_pkMainFrame->Render();
	g_ParticleManager->Render( GC_LAYER_CUSTOM_UI);
	m_pkCharIntroFrame->Render();

	return S_OK;
}

void KGCCharGainScene::OnLeaveScene()
{
	g_KMci.GetShowForMovie().Clear();
	RenderEffect(false);
	m_pkCharIntroFrame->OffCashParticle();

    std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();
    for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
    {
        g_ParticleManager->DeleteSequence( *iterPtr );
    }
    m_vecServerSceneEffect.clear();

    g_pkUIScene->m_pkNickNameBox->SetNickRegComplete(true);	
}

