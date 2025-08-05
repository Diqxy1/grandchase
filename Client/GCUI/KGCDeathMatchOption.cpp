#include "stdafx.h"
#include "KGCDeathMatchOption.h"
#include "KGCDeathMatch.h"

IMPLEMENT_CLASSNAME( KGCDeathMatchOption );
IMPLEMENT_WND_FACTORY( KGCDeathMatchOption );
IMPLEMENT_WND_FACTORY_NAME( KGCDeathMatchOption, "gc_deathmatch_option" );

KGCDeathMatchOption::KGCDeathMatchOption( void )
: m_pkGoalKill(NULL)
, m_pkLimitTime(NULL)
, m_pkBalanceChk(NULL)
, m_pkBalanceStatic(NULL)
, m_pStateDeathMatch(NULL)
, m_bSendPacket(false)
{
    LINK_CONTROL("goal_kill_change_box", m_pkGoalKill  );
    LINK_CONTROL("limit_time_change_box",m_pkLimitTime );
    LINK_CONTROL("balance_chk", m_pkBalanceChk );
    LINK_CONTROL("balance_static", m_pkBalanceStatic );
}

KGCDeathMatchOption::~KGCDeathMatchOption( void )
{
}

void KGCDeathMatchOption::OnCreate()
{
    m_pkGoalKill->SetSelfInputCheck( false );
    m_pkGoalKill->AddActionListener( this );
    m_pkGoalKill->SetText( L"" );
    m_pkGoalKill->ConnectEventProcedure(KGCChangeBox::LEFT_BUTTON,this,&KGCDeathMatchOption::ChangeGoalKillLeft);
    m_pkGoalKill->ConnectEventProcedure(KGCChangeBox::RIGHT_BUTTON,this,&KGCDeathMatchOption::ChangeGoalKillRight);

    m_pkLimitTime->SetSelfInputCheck( false );
    m_pkLimitTime->AddActionListener( this );
    m_pkLimitTime->SetText( L"" );
    m_pkLimitTime->ConnectEventProcedure(KGCChangeBox::LEFT_BUTTON,this,&KGCDeathMatchOption::ChangeLimitTimeLeft);
    m_pkLimitTime->ConnectEventProcedure(KGCChangeBox::RIGHT_BUTTON,this,&KGCDeathMatchOption::ChangeLimitTimeRight);


    m_pkGoalKill->InitState(true);
    m_pkLimitTime->InitState(true);
    m_pkBalanceChk->InitState( true , true , this );
    m_pkBalanceChk->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCDeathMatchOption::OnCheckBalance);
    m_pkBalanceStatic->InitState(true);
    m_pkBalanceStatic->SetText( g_pkStrLoader->GetString(STR_ID_DEATHMATCH_STAT_BALANCE_SETTING) );
}

void KGCDeathMatchOption::InitData()
{
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
         SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
        KGCStateGame* pStateGame = (KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME );
        KGCDeathMatch* pStateDeathMatch = (KGCDeathMatch*)pStateGame->GetCurGameModePtr();
        
        wstringstream stmGoal;
        wstringstream stmTime;
        stmGoal << pStateDeathMatch->m_iGoal;
        stmTime << pStateDeathMatch->m_fTime;
        m_pkGoalKill->SetText( stmGoal.str() );
        m_pkLimitTime->SetText( stmTime.str() );
    }

    m_pkBalanceChk->SetCheck( SiKGCRoomManager()->GetRoomInfo().bDeathMatchBalancing );
    m_pStateDeathMatch = NULL;
    m_bSendPacket = false;

    bool bBalance = SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH;

    m_pkBalanceChk->ToggleRender( bBalance );          
    m_pkBalanceStatic->ToggleRender( bBalance );
}

void KGCDeathMatchOption::ActionPerformed(const KActionEvent& event )
{

}

void KGCDeathMatchOption::FrameMoveInEnabledState()
{

}

void KGCDeathMatchOption::OnCheckBalance()
{
    m_bSendPacket = true;
    m_pkBalanceChk->SetCheck( !m_pkBalanceChk->GetCheck() );
    SendDeathMatchInfo();
}

void KGCDeathMatchOption::ChangeGoalKillLeft()
{
    GetDeathMatchInfo();

    m_pStateDeathMatch->m_iGoal -= 10;
    
    if ( m_pStateDeathMatch->m_iGoal < 10 )
    {
        m_pStateDeathMatch->m_iGoal = 10;
        m_bSendPacket = false;
    }

    wstringstream stmGoal;
    stmGoal << m_pStateDeathMatch->m_iGoal;
    m_pkGoalKill->SetText( stmGoal.str() );

    SendDeathMatchInfo();
}

void KGCDeathMatchOption::ChangeGoalKillRight()
{
    GetDeathMatchInfo();

    m_pStateDeathMatch->m_iGoal += 10;
    
    if ( m_pStateDeathMatch->m_iGoal > 30 )
    {
        m_pStateDeathMatch->m_iGoal = 30;
        m_bSendPacket = false;
    }
    wstringstream stmGoal;
    stmGoal << m_pStateDeathMatch->m_iGoal;
    m_pkGoalKill->SetText( stmGoal.str() );

    SendDeathMatchInfo();
}

void KGCDeathMatchOption::ChangeLimitTimeLeft()
{
    GetDeathMatchInfo();

    m_pStateDeathMatch->m_fTime -= 60.0f * 5.0f;

    if ( m_pStateDeathMatch->m_fTime < 60.0f * 5.0f )
    {
        m_pStateDeathMatch->m_fTime = 60.0f * 5.0f;
        m_bSendPacket = false;
    }

    wstringstream stmTime;
    stmTime << m_pStateDeathMatch->m_fTime;
    m_pkLimitTime->SetText( stmTime.str() );

    SendDeathMatchInfo();
}

void KGCDeathMatchOption::ChangeLimitTimeRight()
{
    GetDeathMatchInfo();

    m_pStateDeathMatch->m_fTime += 60.0f * 5.0f;

    if ( m_pStateDeathMatch->m_fTime > 60.0f * 15.0f )
    {
        m_pStateDeathMatch->m_fTime = 60.0f * 15.0f;
        m_bSendPacket = false;
    }
    
    wstringstream stmTime;
    stmTime << m_pStateDeathMatch->m_fTime;
    m_pkLimitTime->SetText( stmTime.str() );

    SendDeathMatchInfo();
}

void KGCDeathMatchOption::GetDeathMatchInfo()
{
    m_bSendPacket = true;

    KGCStateGame* pStateGame = (KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME );
    m_pStateDeathMatch = (KGCDeathMatch*)pStateGame->GetCurGameModePtr();
}

void KGCDeathMatchOption::SendDeathMatchInfo()
{
    if ( m_bSendPacket) {
        
        KGCStateGame* pStateGame = (KGCStateGame*)g_MyD3D->m_pStateMachine->GetStatePtr( GS_GAME );
        KGCDeathMatch* pStateDeathMatch = (KGCDeathMatch*)pStateGame->GetCurGameModePtr();

        KRoomOption opt;
        opt.m_strPassWord = g_pStateRoom->GetRoomPassword();
        opt.m_strRoomTitle = g_pStateRoom->GetRoomName();
        opt.m_bDeathMatchBalancing = m_pkBalanceChk->GetCheck();
        opt.m_bDeathMatchIntrudable = true;

        opt.m_nDeathKillCount = pStateDeathMatch->m_iGoal;
        opt.m_nDeathLimitTime = static_cast<int> ( pStateDeathMatch->m_fTime);

        KP2P::GetInstance()->Send_Change_Room_Option_Req( opt );
    }
}