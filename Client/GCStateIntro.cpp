#include "stdafx.h"
//
#include "../MyD3D.h"

#include "../KMci.h"

//
#include "GCStateIntro.h"


#include "GCStateMachine.h"

//
#include "GCUI/KGCLoginScene.h"

#include "GCUI/KGCRegisterScene.h"

KGCStateIntro::KGCStateIntro(void)
{
	m_dwFrame = 0;


}

KGCStateIntro::~KGCStateIntro(void)
{
	m_mapIntroInfo.clear();
	m_vecIntroEffect.clear();
}



bool KGCStateIntro::OnInitBeforeStateChange( GAME_STATE eNowState_ )
{
    return true;
}

bool KGCStateIntro::OnInit()
{
	LoadScript();

	return true;
}


bool KGCStateIntro::FrameMove( float fElapsedTime )
{
	g_pkInput->UpdateInput();
	if ( g_pkInput->IsDown( DIK_ESCAPE ) || !IsDrawingParticle() )
	{
		OffParticle();
		g_MyD3D->m_pkUIMgr->MoveUIScene( "main" );
		if( g_kGlobalValue.m_bLoginFirst )
		{
			g_MyD3D->m_pStateMachine->GoLogin();
		}
		else
		{
			g_MyD3D->m_pStateMachine->GoServer(); 
		}

		return true;
	}


	std::map<DWORD,std::vector<std::string>>::iterator mit = m_mapIntroInfo.find(m_dwFrame);

	if (mit != m_mapIntroInfo.end())
	{
		std::vector<std::string>::iterator vit = mit->second.begin();
		for (; vit != mit->second.end() ; ++vit)
		{
			CParticleEventSeqPTR pParticle = NULL;
			pParticle = g_ParticleManager->CreateSequence( *vit, 0, 0, 0.5f );
			if( pParticle )
			{
				g_ParticleManager->SetLayer( pParticle, GC_LAYER_CUSTOM_UI );
				m_vecIntroEffect.push_back(pParticle);
			}
		}

	}

	m_dwFrame++;
    return true;
}

bool KGCStateIntro::Render()
{
	g_ParticleManager->FrameMove( g_MyD3D->m_fTime, GC_ELAPSED_TIME );
	if ( BeginScene() )
	{
		g_MyD3D->m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0L );

		g_ParticleManager->Render( GC_LAYER_CUSTOM_UI );
		EndScene();
	}

    return true;
}


bool KGCStateIntro::OnDestroyBeforeStateChange()
{
    return true;
}

void KGCStateIntro::OnSetupFSM()
{
	//#if defined( LOGIN_FIRST )
	//if( g_kGlobalValue.m_bLoginFirst )
	//{
	ON_SETUP_FSM( GS_LOGIN,         FAIL_N_SHUTDOWN );
	ON_SETUP_FSM( GS_REGISTER,      FAIL_N_SHUTDOWN );
	//}
	//else
	//#else
	//{
	ON_SETUP_FSM( GS_SERVER,        FAIL_N_SHUTDOWN );
	//}
	//#endif

    m_eStateForFail = FAIL_N_SHUTDOWN;
}

void KGCStateIntro::OnDestroy()
{

}

void KGCStateIntro::LoadScript()
{
	m_mapIntroInfo.clear();

	KLuaManager luaMgr;

	if( GCFUNC::LoadLuaScript( luaMgr , "IntroInfo.lua" ) == false )
		return;

	LUA_BEGIN_TABLE("InfoList",return);
	{
		for(int i = 1; ; i++)
		{
			DWORD dwFrame = -1;
			std::vector<std::string> vecString;
			LUA_BEGIN_TABLE(i,break)
			{
				LUA_GET_VALUE("Frame",dwFrame,return);

				LUA_BEGIN_TABLE("ParticleName",return);

				
				for (int j=1 ; ; j++)
				{
					std::string strName;
					LUA_GET_VALUE_NOASSERT( j, strName, break );
					vecString.push_back(strName);

				}
				LUA_END_TABLE(return)
				
			}
			LUA_END_TABLE(return)
			m_mapIntroInfo.insert(std::make_pair(dwFrame,vecString));
		}
	}
	LUA_END_TABLE(return)
}

bool KGCStateIntro::IsDrawingParticle()
{
	if (m_vecIntroEffect.empty())
		return false;

	std::vector< CParticleEventSeqPTR >::iterator vit = m_vecIntroEffect.begin();

	for (; vit != m_vecIntroEffect.end() ; ++vit)
	{
		if (*vit)
		{
			return true;
		}
	}

	return false;
}

void KGCStateIntro::OffParticle()
{
	std::vector< CParticleEventSeqPTR >::iterator vit = m_vecIntroEffect.begin();

	for (; vit != m_vecIntroEffect.end() ; ++vit)
	{
		if (*vit)
		{
			(*vit)->SetShow(false);
		}
	}

}


