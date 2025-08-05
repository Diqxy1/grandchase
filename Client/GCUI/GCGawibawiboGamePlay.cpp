#include "stdafx.h"
#include "GCGawibawiboGamePlay.h"

IMPLEMENT_CLASSNAME( KGCGawibawiboGamePlay );
IMPLEMENT_WND_FACTORY( KGCGawibawiboGamePlay );
IMPLEMENT_WND_FACTORY_NAME( KGCGawibawiboGamePlay, "gc_gawibawibo_game_play" );

#define INIT_LOOP_COUNT	7

KGCGawibawiboGamePlay::KGCGawibawiboGamePlay( void )
:	m_pkBtnGawi(NULL)
,	m_pkBtnBawi(NULL)
,	m_pkBtnBo(NULL)
,	m_pkBtnRandom(NULL)
,	m_iSelectBtn(0)
,	m_iRollFrame(5)
,	m_bRollTimeDeg(false)
,	m_iDegTime(INIT_LOOP_COUNT)
,	m_iResult(-1)
,	m_bStop(false)
,	m_bEndPlay(false)
,	m_iPreResult(0)
,	m_bClearPaticleEff(false)
{
	LINK_CONTROL("btn_gawi",m_pkBtnGawi);
	LINK_CONTROL("btn_bawi",m_pkBtnBawi);
	LINK_CONTROL("btn_bo",m_pkBtnBo);
	LINK_CONTROL("btn_random",m_pkBtnRandom);
	

	m_mapLeftBalloonGBB[0] = NULL;
	m_mapLeftBalloonGBB[1] = NULL;
	m_mapLeftBalloonGBB[2] = NULL;
	m_mapLeftBalloonGBB[3] = NULL;

	m_mapRightBalloonGBB[0] =NULL;
	m_mapRightBalloonGBB[1] =NULL;
	m_mapRightBalloonGBB[2] =NULL;

	LINK_CONTROL("gawi1",m_mapLeftBalloonGBB[0]);
	LINK_CONTROL("bawi1",m_mapLeftBalloonGBB[1]);
	LINK_CONTROL("bo1",m_mapLeftBalloonGBB[2]);
	LINK_CONTROL("wait",m_mapLeftBalloonGBB[3]);

	LINK_CONTROL("gawi2",m_mapRightBalloonGBB[0]);
	LINK_CONTROL("bawi2",m_mapRightBalloonGBB[1]);
	LINK_CONTROL("bo2",m_mapRightBalloonGBB[2]);

}

KGCGawibawiboGamePlay::~KGCGawibawiboGamePlay( void )
{

}

void KGCGawibawiboGamePlay::ClickGawiBawiBoBtn()
{
	// 버튼 선택 안했는데 들어왔다면 처리해 줄 필요 없다.
	if(m_iSelectBtn <= 0 || m_iSelectBtn > 4) return;
	
	// 랜덤일때 설정 해줍시다.
	if(m_iSelectBtn == 4) m_iSelectBtn = rand()%3+1;

	KP2P::GetInstance()->Send_GetUserGwibawiboReq();
	// 가위바위보 도는 속도를 점점 빠르게 한다.
	//m_bRollTimeDeg = true;

}

void KGCGawibawiboGamePlay::ActionPerformed( const KActionEvent& event )
{
	//GAWIBAWIBO_STATE_DEFAULT	=0, //아무것도 선택 하지 않았을때.
	//	GAWIBAWIBO_STATE_GAWI		=1, //가위
	//	GAWIBAWIBO_STATE_BAWI		=2, //바위
	//	GAWIBAWIBO_STATE_BO			=3, //보
	//	GAWIBAWIBO_STATE_RANDOM		=4, //랜덤.
	//	GAWIBAWIBO_STATE_STOP		=5  //중지.

	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		bool clickBtn =false;
		if( event.m_pWnd == m_pkBtnGawi)
		{
			m_iSelectBtn = GAWIBAWIBO_STATE_GAWI;
			clickBtn = true;
		}
		else if(event.m_pWnd == m_pkBtnBawi	)
		{
			m_iSelectBtn = GAWIBAWIBO_STATE_BAWI;
			clickBtn = true;
		}
		else if(event.m_pWnd == m_pkBtnBo)
		{
			m_iSelectBtn = GAWIBAWIBO_STATE_BO;
			clickBtn = true;
		}
		else if(event.m_pWnd == m_pkBtnRandom)
		{
			m_iSelectBtn = GAWIBAWIBO_STATE_RANDOM;
			clickBtn = true;
		}
		if(	clickBtn)
		{
			ClickGawiBawiBoBtn();
			TogglRenderBtn(false);
		}
	}
}

void KGCGawibawiboGamePlay::OnCreate( void )
{
	m_pkBtnGawi->InitState(true,true,this);
	m_pkBtnBawi->InitState(true,true,this);
	m_pkBtnBo->InitState(true,true,this);
	m_pkBtnRandom->InitState(true,true,this);

	InitGamePlay();
}
void KGCGawibawiboGamePlay::InitGamePlay()
{
	std::map<int,KD3DWnd*>::iterator mIter = m_mapLeftBalloonGBB.begin();

	for(;mIter != m_mapLeftBalloonGBB.end();mIter++)
	{
		mIter->second->InitState(false);
	}
	m_mapLeftBalloonGBB[3]->ToggleRender(true);

	mIter = m_mapRightBalloonGBB.begin();

	for(;mIter != m_mapRightBalloonGBB.end();mIter++)
	{
		mIter->second->InitState(false);
	}

	m_iSelectBtn = 0;
	m_iRightGawibawibo = 0;
	m_iRollFrame = 5;
	m_iDegTime =INIT_LOOP_COUNT;
	m_iResult =-1;
	m_bStop =false;
	m_bEndPlay = false;
	m_bClearPaticleEff = false;
	TogglRenderBtn(true);
	ClearEffect();
}


void KGCGawibawiboGamePlay::RollGawiBawiBo()
{
	if(o30%m_iRollFrame == 0)
	{
		m_iDegTime--;
		if(m_bRollTimeDeg && m_iDegTime <= 0)
		{
			if(m_iRollFrame > 2){
				m_iRollFrame--;
				m_iDegTime =INIT_LOOP_COUNT;
			}
			else{
				m_iDegTime =30;
				m_bRollTimeDeg = false;
			}
		}

		if(m_iResult != -1 && m_iDegTime<=0)
		{
			m_bStop =true;
			CParticleEventSeqPTR pPaticle;
			pPaticle =	g_ParticleManager->CreateSequence( "MZP_Open_01", 0.0f, 0.0f, 0.5f );
			g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
			m_vecPaticle.push_back(pPaticle);
			pPaticle =	g_ParticleManager->CreateSequence( "MZP_Open_02", 0.0f, 0.0f, 0.5f );
			g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
			m_vecPaticle.push_back(pPaticle);
			pPaticle =	g_ParticleManager->CreateSequence( "MZP_Open_03", 0.0f, 0.0f, 0.5f );
			g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
			m_vecPaticle.push_back(pPaticle);
			pPaticle =	g_ParticleManager->CreateSequence( "MZP_Open_04", 0.0f, 0.0f, 0.5f );
			g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
			m_vecPaticle.push_back(pPaticle);
			pPaticle =	g_ParticleManager->CreateSequence( "MZP_Open_05", 0.0f, 0.0f, 0.5f );
			g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
			m_vecPaticle.push_back(pPaticle);

		}
		m_iRightGawibawibo = (++m_iRightGawibawibo)%3;
		for(int i =0;i<3;i++){
			m_mapRightBalloonGBB[i]->ToggleRender(i == m_iRightGawibawibo);			
		}
	}
}

void KGCGawibawiboGamePlay::RenderPaticle()
{
	if(m_iPreResult == KEVENT_GAWIBAWIBO_ACK::WIN){
		g_KDSound.Play( "GwibawiboMinigame_RPC_Win" );

		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_01", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_02", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_03", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_04", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_05", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_06", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Win_ef_07", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
		if(m_bClearPaticleEff)
		{
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_06", -0.145818f, 0.19818f, 0.5f ) , GC_LAYER_UI);
			m_bClearPaticleEff = false;
		}
	}
	else if(m_iPreResult == KEVENT_GAWIBAWIBO_ACK::LOSE){
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Lose_ef_01", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Lose_ef_02", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
	}

}


void KGCGawibawiboGamePlay::FrameMoveInEnabledState( void )
{
	if(!m_bStop)
	{ 
		RollGawiBawiBo();
	}
	else if(m_bStop)
	{
		if(m_iResult != -1){
			ViewResultGawiBawiBoGame(m_iResult);
			m_iDegTime =60;
		}
		else if(m_iDegTime > 0)
		{
			m_iDegTime--;
		}
		else if(m_bEndPlay == false){
			m_bEndPlay = true;
//			ClearEffect();
			RenderPaticle();
			m_ActionClickFunc();	
		}

	}
}


void KGCGawibawiboGamePlay::ViewResultGawiBawiBoGame(int gamestate_)
{
	CParticleEventSeqPTR pPaticle;
	int iRightSelect =0;
	if(gamestate_ == KEVENT_GAWIBAWIBO_ACK::WIN)
	{
		pPaticle =	g_ParticleManager->CreateSequence( "MZP_Win_01", 0.0f, 0.0f, 0.5f );
		g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
		m_vecPaticle.push_back(pPaticle);
		pPaticle =	g_ParticleManager->CreateSequence( "MZP_Win_02", 0.0f, 0.0f, 0.5f );
		g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
		m_vecPaticle.push_back(pPaticle);
		pPaticle =	g_ParticleManager->CreateSequence( "MZP_Win_03", 0.0f, 0.0f, 0.5f );
		g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
		m_vecPaticle.push_back(pPaticle);

		iRightSelect = (m_iSelectBtn + 1) % 3;		
	}
	else if(gamestate_ == KEVENT_GAWIBAWIBO_ACK::DRAW)
	{
		pPaticle =	g_ParticleManager->CreateSequence( "MZP_Drow", 0.0f, 0.0f, 0.5f );
		iRightSelect = (m_iSelectBtn - 1);		
		g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
		m_vecPaticle.push_back(pPaticle);
	}
	else{
		iRightSelect = (m_iSelectBtn) % 3;		
		pPaticle =	g_ParticleManager->CreateSequence( "MZP_Lose", 0.0f, 0.0f, 0.5f );
		g_ParticleManager->SetLayer( pPaticle , GC_LAYER_UI);
		m_vecPaticle.push_back(pPaticle);
	}



	for(int i =0;i<3;i++){
		m_mapRightBalloonGBB[i]->ToggleRender(i == iRightSelect);			
	}

	m_mapLeftBalloonGBB[3]->ToggleRender(false);

	m_mapLeftBalloonGBB[m_iSelectBtn-1]->ToggleRender(true);
	m_iPreResult = m_iResult;
	m_iResult = -1;

}

void KGCGawibawiboGamePlay::ResultGawiBawiBoGame(const KEVENT_GAWIBAWIBO_ACK &recvInfo_)
{
	m_iResult = static_cast<int>(recvInfo_.m_kUserGawibawiboInfo.m_usResult);

}

void KGCGawibawiboGamePlay::ClearEffect()
{
	std::vector<CParticleEventSeqPTR>::iterator vIter = m_vecPaticle.begin();

	for(;vIter != m_vecPaticle.end();vIter++)
	{
		g_ParticleManager->DeleteSequence(*vIter);
	}
	
	m_vecPaticle.clear();

}

void KGCGawibawiboGamePlay::Destroy()
{
	ClearEffect();
}

void KGCGawibawiboGamePlay::TogglRenderBtn(bool arg_)
{

	m_pkBtnGawi->ToggleRender(arg_);
	m_pkBtnBawi->ToggleRender(arg_);
	m_pkBtnBo->ToggleRender(arg_);
	m_pkBtnRandom->ToggleRender(arg_);
}

void KGCGawibawiboGamePlay::PostChildDraw()
{
	g_ParticleManager->Render(GC_LAYER_UI);
}


