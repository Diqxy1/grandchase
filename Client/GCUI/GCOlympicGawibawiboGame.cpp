#include "stdafx.h"
#include "GCOlympicGawibawiboGame.h"

#include "GCOlympicGawibawiboGameInfo.h"
#include "GCOlympicGawibawiboGamePlay.h"
#include "GCOlympicGawibawiboGameLose.h"

IMPLEMENT_CLASSNAME( KGCOlympicGawibawiboGame );
IMPLEMENT_WND_FACTORY( KGCOlympicGawibawiboGame );
IMPLEMENT_WND_FACTORY_NAME( KGCOlympicGawibawiboGame, "gc_gawibawibo_box_olympic" );

KGCOlympicGawibawiboGame::KGCOlympicGawibawiboGame( void )
:	m_iStateValue(0)
,	m_pkGameInfoBox(NULL)
,	m_iState(0)
,	m_pkGamePlay(NULL)
,	m_pkBtnClose(NULL)
,	m_pkGameLose(NULL)
,   m_iAccumulateData(0)
,   m_iCoinCount(0)
,	m_pkStaticChallengePoint(NULL)
,	m_pkStaticClearCnt(NULL)
{
    LINK_CONTROL("btn_close",m_pkBtnClose);

    LINK_CONTROL( "gawibawibo_game_info_box_olympic",           m_pkGameInfoBox );
    LINK_CONTROL( "gawibawibo_game_play_olympic",		        m_pkGamePlay );
    LINK_CONTROL( "gawibawibo_game_lose_info_box_olympic",		m_pkGameLose );

    LINK_CONTROL("static_challenge_point",m_pkStaticChallengePoint);
    LINK_CONTROL("static_clear_cnt",m_pkStaticClearCnt);

}

KGCOlympicGawibawiboGame::~KGCOlympicGawibawiboGame( void )
{
}
void KGCOlympicGawibawiboGame::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkBtnClose)
        {
            SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
            KGCHotEventBtn::SetHotStateOff();
        }
    }
}

void KGCOlympicGawibawiboGame::OnCreate( void )
{
    m_pkBtnClose->InitState(true,true, this);
    m_pkGameInfoBox->InitState(true,true,this);
    m_pkGamePlay->InitState(false,true,this);
    m_pkGameLose->InitState(false,true,this);

    m_pkGameInfoBox->SetActionFunc(boost::bind(&KGCOlympicGawibawiboGame::ClickChallengeBtn,this));
    m_pkGamePlay->SetActionFunc(boost::bind(&KGCOlympicGawibawiboGame::ClickGawibawiboBtn,this));
    m_pkGameLose->SetActionFunc(boost::bind(&KGCOlympicGawibawiboGame::ClickRetryBtn,this));

    ChangeState();

    m_pkBtnClose->SetHotKey(DIK_ESCAPE);

    if(m_pkStaticChallengePoint)
    {
        m_pkStaticChallengePoint->InitState(true);
        m_pkStaticChallengePoint->SetFontColor( 0xFFFFFF00 );
        m_pkStaticChallengePoint->SetAlign( DT_CENTER );
    }

    if(m_pkStaticClearCnt)
    {
        m_pkStaticClearCnt->InitState(true);
        m_pkStaticClearCnt->SetFontColor( 0xFFFFFF00 );
        m_pkStaticClearCnt->SetAlign( DT_CENTER );
    }

    m_pkStaticChallengePoint->SetAlign(DT_CENTER);
    m_pkStaticClearCnt->SetAlign(DT_CENTER);
}

void KGCOlympicGawibawiboGame::ChangeState()
{
    m_pkGameInfoBox->ToggleRender(GAWIBAWIBO_STATE_DEFAULT == m_iState);
    m_pkGamePlay->ToggleRender(GAWIBAWIBO_STATE_GAME == m_iState);
    m_pkGameLose->ToggleRender(GAWIBAWIBO_STATE_CHALLENGE == m_iState);
}

void KGCOlympicGawibawiboGame::SetGawibawiboGameInfo(const KEVENT_OLYMPIC_JOIN_GAME_ACK &recvInfo_)
{

    m_iState = GAWIBAWIBO_STATE_DEFAULT;

    m_pkGamePlay->SetReqInfo(recvInfo_);
    m_pkGameInfoBox->SetReqInfo( recvInfo_.m_tmTodayDate, recvInfo_.m_nGameType, recvInfo_.m_nTeamType );
    SetGameInfo( recvInfo_.m_nDefaultCount, recvInfo_.m_nMyAccumulateData );
    m_pkGameInfoBox->SetGameInfo(recvInfo_.m_nDefaultCount, recvInfo_.m_nMyAccumulateData);
    ChangeState();

}

void KGCOlympicGawibawiboGame::ResultGawiBawiBoGame(const KEVENT_OLYMPIC_GAWIBAWIBO_RESULT_ACK &recvInfo_)
{
    m_pkGamePlay->ResultGawiBawiBoGame(recvInfo_);
    m_pkGameLose->SetResult( recvInfo_.m_nResult );
    //SetGameInfo(recvInfo_.m_nRemainTryCount, recvInfo_.m_nMyAccumulateData);
    m_pkGameInfoBox->SetGameInfo(recvInfo_.m_nRemainTryCount, recvInfo_.m_nMyAccumulateData);
    m_pkGameLose->SetRetryCoin(recvInfo_.m_nRemainTryCount);
}

void KGCOlympicGawibawiboGame::ClickChallengeBtn()
{
    m_iState = GAWIBAWIBO_STATE_GAME;
    m_pkGamePlay->InitGamePlay();
    ChangeState();
    m_pkBtnClose->Lock(false);
}

void KGCOlympicGawibawiboGame::ClickGawibawiboBtn()
{
    m_pkGameLose->ToggleRender(true);
    m_pkBtnClose->Lock(false);
}

void KGCOlympicGawibawiboGame::ClickRetryBtn()
{
    m_iState = GAWIBAWIBO_STATE_DEFAULT;
    m_pkBtnClose->Lock(false);
    ChangeState();
}

void KGCOlympicGawibawiboGame::SetCloseBtn()
{
    m_pkBtnClose->Lock(false);
}

void KGCOlympicGawibawiboGame::SetGameInfo(int iConinCount, int iAccumulateData )
{
    WCHAR cbuf[256];
    // 도전 포인트 설정.
    wcsnset(cbuf,L'\0',256);
    swprintf(cbuf,L"%d",iConinCount);
    m_pkStaticChallengePoint->SetText(cbuf);

    // 누적 포인트 설정 이긴횟수
    wcsnset(cbuf,L'\0',256);
    swprintf(cbuf,L"%d",iAccumulateData);
    m_pkStaticClearCnt->SetText(cbuf);

}

