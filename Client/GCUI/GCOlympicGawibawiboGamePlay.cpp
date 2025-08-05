#include "stdafx.h"
#include "GCOlympicGawibawiboGamePlay.h"

IMPLEMENT_CLASSNAME( KGCOlympicGawibawiboGamePlay );
IMPLEMENT_WND_FACTORY( KGCOlympicGawibawiboGamePlay );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicGawibawiboGamePlay, "gc_gawibawibo_game_play_olympic" );

#define INIT_LOOP_COUNT	7

KGCOlympicGawibawiboGamePlay::KGCOlympicGawibawiboGamePlay( void )
:	m_pkBtnGawi(NULL)
,	m_pkBtnBawi(NULL)
,	m_pkBtnBo(NULL)
,	m_pkBtnRandom(NULL)
,	m_pkBossImage(NULL)
,	m_iSelectBtn(0)
,	m_iRollFrame(5)
,	m_bRollTimeDeg(false)
,	m_iDegTime(INIT_LOOP_COUNT)
,	m_iResult(-1)
,	m_bStop(false)
,	m_bEndPlay(false)
,	m_iPreResult(0)
,	m_bClearPaticleEff(false)
,   m_tmTodayDate(0)
,   m_nGameType(0)
,   m_nTeamType(0)
,   m_iAccumulateData(0)
,   m_iCoin(0)
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

    SetBossBigImage();

}

KGCOlympicGawibawiboGamePlay::~KGCOlympicGawibawiboGamePlay( void )
{

}

void KGCOlympicGawibawiboGamePlay::SetReqInfo( KEVENT_OLYMPIC_JOIN_GAME_ACK recvInfo_ )
{    
    m_tmTodayDate     = recvInfo_.m_tmTodayDate;;
    m_nGameType       = recvInfo_.m_nGameType;
    m_nTeamType       = recvInfo_.m_nTeamType;

    m_iCoin           = recvInfo_.m_nDefaultCount;
    m_iAccumulateData = recvInfo_.m_nMyAccumulateData;
}

void KGCOlympicGawibawiboGamePlay::ClickGawiBawiBoBtn()
{
    if(m_iSelectBtn <= 0 || m_iSelectBtn > 4) return;

    if(m_iSelectBtn == 4) m_iSelectBtn = rand()%3+1;

    KP2P::GetInstance()->Send_OlympicGawibawiboReq( m_tmTodayDate, m_nGameType, m_nTeamType );
    m_bRollTimeDeg = true;

}

void KGCOlympicGawibawiboGamePlay::ActionPerformed( const KActionEvent& event )
{

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
        if(	clickBtn )
        {
            ClickGawiBawiBoBtn();
            TogglRenderBtn(false);
        }
    }
}

void KGCOlympicGawibawiboGamePlay::OnCreate( void )
{
    m_pkBtnGawi->InitState(true,true,this);
    m_pkBtnBawi->InitState(true,true,this);
    m_pkBtnBo->InitState(true,true,this);
    m_pkBtnRandom->InitState(true,true,this);

    InitGamePlay();
}
void KGCOlympicGawibawiboGamePlay::InitGamePlay()
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


void KGCOlympicGawibawiboGamePlay::RollGawiBawiBo()
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

void KGCOlympicGawibawiboGamePlay::RenderPaticle()
{
    if(m_iPreResult == KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_WIN){
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
    else if(m_iPreResult == KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_LOSE){
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Lose_ef_01", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
        g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "MZP_Lose_ef_02", 0.0f, 0.0f, 0.5f ) , GC_LAYER_UI);
    }

}


void KGCOlympicGawibawiboGamePlay::FrameMoveInEnabledState( void )
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


void KGCOlympicGawibawiboGamePlay::ViewResultGawiBawiBoGame(int gamestate_)
{
    CParticleEventSeqPTR pPaticle;
    int iRightSelect =0;
    if(gamestate_ == KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_WIN)
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
    else if(gamestate_ == KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK::ER_DRAW)
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

    g_pkUIScene->m_pkOlympicGawibawiboDlg->SetGameInfo( m_iCoin, m_iAccumulateData );

}

void KGCOlympicGawibawiboGamePlay::ResultGawiBawiBoGame(const KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK &recvInfo_)
{
    m_iResult         = recvInfo_.m_nResult;
    m_tmTodayDate     = recvInfo_.m_tmTodayDate;

    m_iAccumulateData = recvInfo_.m_nMyAccumulateData;
    m_iCoin           = recvInfo_.m_nRemainTryCount;
    

}

void KGCOlympicGawibawiboGamePlay::ClearEffect()
{
    std::vector<CParticleEventSeqPTR>::iterator vIter = m_vecPaticle.begin();

    for(;vIter != m_vecPaticle.end();vIter++)
    {
        g_ParticleManager->DeleteSequence(*vIter);
    }

    m_vecPaticle.clear();

}

void KGCOlympicGawibawiboGamePlay::Destroy()
{
    ClearEffect();

    if(m_pkBossImage)
        m_pkBossImage->RemoveAllTexture();
    SAFE_DELETE(m_pkBossImage);
}

void KGCOlympicGawibawiboGamePlay::TogglRenderBtn(bool arg_)
{

    m_pkBtnGawi->ToggleRender(arg_);
    m_pkBtnBawi->ToggleRender(arg_);
    m_pkBtnBo->ToggleRender(arg_);
    m_pkBtnRandom->ToggleRender(arg_);
}

void KGCOlympicGawibawiboGamePlay::PostChildDraw()
{
    g_ParticleManager->Render(GC_LAYER_UI);
}


void KGCOlympicGawibawiboGamePlay::SetBossBigImage( )
{
    if(m_pkBossImage == NULL){
        m_pkBossImage = new KGC2DObject();
    }

    m_pkBossImage->RemoveAllTexture();
    char	cBossImageString[256];
    sprintf(cBossImageString,"rockpaperscissors_B1.dds" );

    m_pkBossImage->AddTexture( g_pGCDeviceManager2->CreateTexture( cBossImageString ) );
    m_pkBossImage->SetAxisOffset( 0.0f, 0.0f,0.0f );
#if defined(USE_EVENT_GAWIBAWIBO_BOSSIMAGE)
    m_pkBossImage->SetSize( 350, 350 );
    m_pkBossImage->CameraMatrixOn();
    m_pkBossImage->SetPositionPixel( 400.0f, 125.0f );
#else    
    m_pkBossImage->SetSize( 500, 500 );
    m_pkBossImage->CameraMatrixOn();
    m_pkBossImage->SetPositionPixel( 239.0f, 125.0f );
#endif 

}

void KGCOlympicGawibawiboGamePlay::PreDraw( void )
{
    if( m_pkBossImage != NULL )
    {
        m_pkBossImage->Render();
    }
}
