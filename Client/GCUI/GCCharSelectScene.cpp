#ifndef NEW_CHAR_SELECT_SCENE
#include "stdafx.h"
#include "GCCharSelectScene.h"
#include "KGCJumpingCharManager.h"

IMPLEMENT_CLASSNAME( KGCCharSelectScene );
IMPLEMENT_WND_FACTORY( KGCCharSelectScene );
IMPLEMENT_WND_FACTORY_NAME( KGCCharSelectScene, "gc_char_select_scene" );

KGCCharSelectScene::KGCCharSelectScene( void )
{
	m_pkCharInfoFrame = NULL;
	LINK_CONTROL( "char_info_frame", m_pkCharInfoFrame );



}

KGCCharSelectScene::~KGCCharSelectScene( void )
{
	
}

void KGCCharSelectScene::ActionPerformed( const KActionEvent& event )
{

}

void KGCCharSelectScene::OnCreate( void )
{
	m_pkCharInfoFrame->InitState(true,true,this);
}

void KGCCharSelectScene::Add_NewsFrame_Particle( std::string seq,float x/*=0.0f*/,float y/*=0.0f*/ )
{
	CParticleEventSeqPTR pParticle = NULL;
	pParticle = g_ParticleManager->CreateSequence( seq, x, y, 0.5f );
	if( pParticle )
	{
		g_ParticleManager->SetLayer( pParticle, GC_LAYER_CUSTOM_UI );
		m_vecServerSceneEffect.push_back( pParticle );
	}

}

void KGCCharSelectScene::RenderEffect( bool bRender )
{
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();

	for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
	{
		if ((*iterPtr))
			(*iterPtr)->SetShow(bRender);
	}

}

void KGCCharSelectScene::CreateEffect()
{
	Add_NewsFrame_Particle("Map_Intro_01");
}

void KGCCharSelectScene::EnterScene()
{
	KGCScene::EnterScene();
    CreateEffect();
	RenderEffect(true);

    SetVitalityInfo();

    if ( SiKGCJumpingCharManager()->IsEnableEvent() )
    {
         g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_JUMPING_CHAR_DLG );
    }
	
}

void KGCCharSelectScene::FrameMoveInEnabledState( void )
{
}

HRESULT KGCCharSelectScene::Render( void )
{
	if ( false == IsRenderOn() ) return S_OK;
	if ( true == IsNotDraw() ) return S_OK;
	if ( D3DWS_DISABLED == GetWndState() ) return S_OK;

	m_pkMainFrame->Render();
	g_ParticleManager->Render( GC_LAYER_CUSTOM_UI);
	m_pkCharInfoFrame->Render();

	return S_OK;

}

void KGCCharSelectScene::OnLeaveScene()
{
    std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();
    for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
    {
        g_ParticleManager->DeleteSequence( *iterPtr );
    }
    m_vecServerSceneEffect.clear();

	RenderEffect(false);
}

void KGCCharSelectScene::UpdateEmptySlot()
{
    m_pkCharInfoFrame->UpdateEmptySlot();
}

void KGCCharSelectScene::SetVitalityInfo()
{
    if( g_kGlobalValue.UseVitalitySystem() == false )
        return;

    m_pkCharInfoFrame->SetVitalityTooltip();
}

void KGCCharSelectScene::UpdateData()
{
    m_pkCharInfoFrame->Initialize();
}
#endif