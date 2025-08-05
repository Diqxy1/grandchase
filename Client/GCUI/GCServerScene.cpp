#include "stdafx.h"
//
#include "GCServerScene.h"

#include "KGCServerList.h"
#include "KGCServerBar.h"
#include "KGCNewsFrame.h"
//
#include "MyD3D.h"

#include "GCIconImageManager.h"
#include "GCMoveModel.h"
//


IMPLEMENT_CLASSNAME( KGCServerScene );
IMPLEMENT_WND_FACTORY( KGCServerScene );
IMPLEMENT_WND_FACTORY_NAME( KGCServerScene, "gc_server_scene" );

KGCServerScene::KGCServerScene( void )
{
    m_pkServerList      = NULL;
    m_pkBtnTutorial		= NULL;
	m_pkBtnPatch		= NULL;
    m_pkClickRange		= NULL;

    LINK_CONTROL( "server_list",        m_pkServerList );
	LINK_CONTROL( "btn_tutorial",       m_pkBtnTutorial );
	LINK_CONTROL( "btn_patch",			m_pkBtnPatch );
    LINK_CONTROL( "click_range",        m_pkClickRange ); 
}

KGCServerScene::~KGCServerScene(void)
{
	
}

void KGCServerScene::OnCreate()
{
    KGCScene::OnCreate();

    m_pkServerList->InitState(true,true,this);

	m_pkBtnTutorial->InitState(true,true,this);
	m_pkBtnPatch->InitState(true, true, this);
    m_pkClickRange->InitState(true, true, this);

	m_vecMoveModel.clear();

	m_vecMoveModel = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_scene_server.stg", "", "server_scene\\move_model", true, g_kGlobalValue.GetMoveModelCount());

	std::vector<KD3DWnd*>::iterator it = m_vecMoveModel.begin();

	int index = 0;
	for (; it != m_vecMoveModel.end(); ++it)
	{
		MoveModelData moveModelData;
		g_kGlobalValue.GetMoveModelData(g_kGlobalValue.GetStartModelIdServerSate() + index, moveModelData);

		(*it)->InitState(true, true, this);
		(*it)->SetOrder_Bottom();

		if (moveModelData.m_bUseEvent)
		{
			(*it)->SetWidth(moveModelData.m_iWidth);
			(*it)->SetHeight(moveModelData.m_iHeight);
			(*it)->SetFixedWindowLocalPos(D3DXVECTOR2(moveModelData.m_fStartX, moveModelData.m_fStartY));
		}

		index++;
	}

	m_pkBtnTutorial->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCServerScene::OnTutorial);
	m_pkBtnPatch->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCServerScene::OnPatch);
    m_pkClickRange->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCServerScene::OnClickRange);
}

void KGCServerScene::OnInitialize()
{
    UpdateServerList();

    g_pkUIScene->m_pkOnGameDlg->ToggleRender( false );
    g_pkUIScene->m_pkWorldMapContainer->ToggleRender( false );
    g_pkUIScene->EnableMessageBox( true );

    std::wstring strToolTip = g_pkStrLoader->GetString( STR_ID_PLEASE_SELECT_SERVER );
	// 얘들은 락을 건다
	SetButtonRender( BTN_WORLDMAP, false );
	SetButtonRender( BTN_MATCH, false );
	SetButtonRender( BTN_SHOP, false );
#if defined USE_COORDI_SHOP
	SetButtonRender( BTN_COORDI_SHOP, false );
#endif
	SetButtonRender( BTN_MY_INFO, false );
    SetButtonRender( BTN_SKILL_TREE, false );
}

void KGCServerScene::ActionPerformed( const KActionEvent& event )
{
    switch( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
		case KD3DWnd::D3DWE_BUTTON_CLICK:
			{
				for( int i = 0; i < g_kGlobalValue.GetMoveModelCount(); i++ )
				{
					if(event.m_pWnd == m_vecMoveModel[i])
					{
						((KGCMoveModel*)m_vecMoveModel[i])->ShowEffect(true);
						return;
					}
				}
			}
    }

    KGCScene::ActionPerformed( event ); 
}

void KGCServerScene::OnEventBtnClick()
{
    //WinExec("explorer http://chase.netmarble.net/Event/110428_NewUser/_html/index.asp", FALSE);
}

void KGCServerScene::FrameMoveInEnabledState()
{
	//RenderEffect(true);
}

void KGCServerScene::UpdateServerList()
{
    m_pkServerList->SetServerList();
}

void KGCServerScene::OnCreateComplete()
{
}

bool KGCServerScene::MoveServer( DWORD dwServerUID )
{
    KGCServerBar* pBar = m_pkServerList->FindServerBar(dwServerUID);
    if(pBar == NULL)
        return false;

    if( false == pBar->RemainServer())
    {
        g_MyD3D->GoChannel();
        g_MyD3D->m_pStateMachine->GoServer();
        
        pBar->OnSelectServer();
    }

    return true;
}

void KGCServerScene::Add_NewsFrame_Particle( std::string seq,float x/*=0.0f*/,float y/*=0.0f*/,float z/*=0.5f*/)
{
	CParticleEventSeqPTR pParticle = NULL;
	pParticle = g_ParticleManager->CreateSequence( seq, x, y, z );
	if( pParticle )
	{
		g_ParticleManager->SetLayer( pParticle, GC_LAYER_CUSTOM_UI_2 );
		m_vecServerSceneEffect.push_back( pParticle );
	}

}

void KGCServerScene::RenderEffect( bool bRender )
{
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();

	for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
	{
		if ((*iterPtr))
			(*iterPtr)->SetShow(bRender);
	}

}

void KGCServerScene::ClearEffect()
{
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();

	for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
	{
		g_ParticleManager->DeleteSequence( *iterPtr );
	}
	m_vecServerSceneEffect.clear();
}

void KGCServerScene::ClearEffect( std::string sParticleName )
{
	std::vector< CParticleEventSeqPTR >::iterator iterPtr = m_vecServerSceneEffect.begin();

	for (; iterPtr != m_vecServerSceneEffect.end() ; ++iterPtr )
	{
		g_ParticleManager->DeleteSequence( *iterPtr );
	}
	m_vecServerSceneEffect.clear();
}



void KGCServerScene::EnterScene()
{
	KGCScene::EnterScene();

	for( int i = 0; i < g_kGlobalValue.GetMoveModelCount(); i++ )
	{
		((KGCMoveModel*)(m_vecMoveModel[i]))->SetModelID(g_kGlobalValue.GetStartModelIdServerSate() + i);
		((KGCMoveModel*)(m_vecMoveModel[i]))->SetModelImage();
	}
}

void KGCServerScene::CreateEffect()
{

}

HRESULT KGCServerScene::Render( void )
{
	if ( false == IsRenderOn() ) return S_OK;
	if ( true == IsNotDraw() ) return S_OK;
	if ( D3DWS_DISABLED == GetWndState() ) return S_OK;

    m_pkMainFrame->Render();

	for( int i = 0; i < g_kGlobalValue.GetMoveModelCount(); i++ )
	{
		((KGCMoveModel*)m_vecMoveModel[i])->PostChildDraw();
	}

	g_ParticleManager->Render(1);
	g_ParticleManager->Render(2);
	g_ParticleManager->Render(3);
	g_ParticleManager->Render(4);
	g_ParticleManager->Render(5);
	g_ParticleManager->Render(6);
	g_ParticleManager->Render(7);
	g_ParticleManager->Render(8);
	g_ParticleManager->Render(9);
	g_ParticleManager->Render(10);
	g_ParticleManager->Render(11);
	g_ParticleManager->Render(12);
	g_ParticleManager->Render(13);
	g_ParticleManager->Render(14);
	g_ParticleManager->Render(GC_LAYER_CUSTOM_UI_2);

	m_pkServerList->Render();
	m_pkBtnTutorial->Render();
	m_pkBtnPatch->Render();
    
 	return S_OK;
}

void KGCServerScene::OnClickRange()
{
}
void KGCServerScene::OnReplay()
{
}

void KGCServerScene::OnTutorial()
{
	g_kGlobalValue.SetPlayerSkillInfo( g_MyD3D->MyPlayer[0], true, false, true );
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TUTORIAL_OPTION_NEW );
}

void KGCServerScene::NewReLoad()
{

}

void KGCServerScene::OnLeaveScene()
{
	for( int i = 0; i < g_kGlobalValue.GetMoveModelCount(); i++ )
	{
		((KGCMoveModel*)m_vecMoveModel[i])->ClearAllEffects();
		((KGCMoveModel*)m_vecMoveModel[i])->OnDestroy();
	}
}

void KGCServerScene::OnPatch()
{
	PopUpWebPage(HOMEPAGE_NEWS_LINK);
}