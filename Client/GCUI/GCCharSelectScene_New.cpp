#include "stdafx.h"

#include "GCCharSelectScene_New.h"
#ifdef NEW_CHAR_SELECT_SCENE

#include "KGCJumpingCharManager.h"

IMPLEMENT_CLASSNAME( KGCCharSelectScene_New );
IMPLEMENT_WND_FACTORY( KGCCharSelectScene_New );
IMPLEMENT_WND_FACTORY_NAME( KGCCharSelectScene_New, "gc_char_select_scene_new" );

KGCCharSelectScene_New::KGCCharSelectScene_New( void )
{
	m_pkCharInfoFrame = NULL;
	LINK_CONTROL( "char_info_frame_new2", m_pkCharInfoFrame );
}

KGCCharSelectScene_New::~KGCCharSelectScene_New( void )
{
}

void KGCCharSelectScene_New::OnLeaveScene()
{
	m_pkCharInfoFrame->DisposeInfos();
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();
	for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
	{
		g_ParticleManager->DeleteSequence( *iterPtr );
	}
	m_vecServerSceneEffect.clear();

	RenderEffect(false);
}

void KGCCharSelectScene_New::ActionPerformed( const KActionEvent& event )
{
}

void KGCCharSelectScene_New::OnCreate( void )
{
	m_pkCharInfoFrame->InitState(true,true,this);
}

void KGCCharSelectScene_New::Add_NewsFrame_Particle( std::string seq,float x/*=0.0f*/,float y/*=0.0f*/ )
{
	CParticleEventSeqPTR pParticle = NULL;
	pParticle = g_ParticleManager->CreateSequence( seq, x, y, 0.5f );
	if( pParticle )
	{
		g_ParticleManager->SetLayer( pParticle, GC_LAYER_CUSTOM_UI );
		m_vecServerSceneEffect.push_back( pParticle );
	}

}

void KGCCharSelectScene_New::RenderEffect( bool bRender )
{
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();

	for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
	{
		if ((*iterPtr))
			(*iterPtr)->SetShow(bRender);
	}

}

void KGCCharSelectScene_New::CreateEffect()
{
	Add_NewsFrame_Particle("Map_Intro_01");
}

void KGCCharSelectScene_New::EnterScene()
{
	KGCScene::EnterScene();
	m_pkCharInfoFrame->DisposeInfos();
	m_pkCharInfoFrame->InitData();
    CreateEffect();
	RenderEffect(true);

    SetVitalityInfo();

    if ( SiKGCJumpingCharManager()->IsEnableEvent() )
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_JUMPING_CHAR_DLG);
}

void KGCCharSelectScene_New::FrameMoveInEnabledState( void )
{
	if (g_pkUIScene->m_pkDepot->IsWndState(D3DWS_ENABLED))
	{
		std::wstring ExitWithString1 = L"Opa!";
		std::wstring ExitWithString2 = L"Pego no pulo tentando bugar o armazém!";
		g_MyD3D->ExitWithMsgDlg(ExitWithString1.c_str(), ExitWithString2.c_str());
	}
}

HRESULT KGCCharSelectScene_New::Render( void )
{
	if ( false == IsRenderOn() ) return S_OK;
	if ( true == IsNotDraw() ) return S_OK;
	if ( D3DWS_DISABLED == GetWndState() ) return S_OK;

	m_pkMainFrame->Render();
	g_ParticleManager->Render( GC_LAYER_CUSTOM_UI);
	m_pkCharInfoFrame->Render();

	return S_OK;
}

void KGCCharSelectScene_New::UpdateEmptySlot()
{
    m_pkCharInfoFrame->UpdateEmptySlot();
}

void KGCCharSelectScene_New::SetVitalityInfo()
{
    if( g_kGlobalValue.UseVitalitySystem() == false )
        return;
}

void KGCCharSelectScene_New::UpdateData()
{
    m_pkCharInfoFrame->Initialize();
}

#endif