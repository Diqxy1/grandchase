#include "stdafx.h"
#include "GCSphinxQuiz.h"

IMPLEMENT_CLASSNAME( KGCSphinxQuiz );
IMPLEMENT_WND_FACTORY( KGCSphinxQuiz );
IMPLEMENT_WND_FACTORY_NAME( KGCSphinxQuiz, "gc_sphinx_quiz" );

//KD3DWnd *m_pkBtnO;
//KD3DWnd *m_pkBtnX;
//
//KD3DStatic *m_pkStaticSecond;
//KD3DStatic *m_pkStaticQuestion;
// 3초 딜레이 후 문제 냄
#define SPHINX_DELAY_TIME 3000

KGCSphinxQuiz::KGCSphinxQuiz( void )
: m_pkBtnO(NULL)
, m_pkBtnX(NULL)
, m_pkStaticSecond(NULL)
, m_pkStaticQuestion(NULL)
, m_pkStaticPreQuestion(NULL)
, m_dwDelayTime(0)
, m_dwQuestionTime(0)
, m_pkSecond(NULL)
, m_iQuestionID(-1)
, m_dwDelayEndQuestionTime(0)
{
    LINK_CONTROL("o_btn", m_pkBtnO);
    LINK_CONTROL("x_btn", m_pkBtnX);
    LINK_CONTROL("second", m_pkSecond);
    LINK_CONTROL("cliprange", m_pkClipRange);
    
    
    LINK_CONTROL("static_second", m_pkStaticSecond);
    LINK_CONTROL("static_question", m_pkStaticQuestion);
    LINK_CONTROL("static_question_pre", m_pkStaticPreQuestion);
    
}

KGCSphinxQuiz::~KGCSphinxQuiz( void )
{
}

void KGCSphinxQuiz::ActionPerformed( const KActionEvent& event )
{

}

void KGCSphinxQuiz::OnCreate( void )
{
    m_pkBtnO->InitState(true,true,this);
    m_pkBtnX->InitState(true,true,this);

    m_pkSecond->InitState(false,false,NULL);

    m_pkStaticSecond->InitState(true,false,NULL);
    m_pkStaticSecond->SetAlign(DT_CENTER);
    m_pkStaticSecond->SetText(g_pkStrLoader->GetString( STR_ID_SPHINX_EVENT_8 ));

    m_pkStaticQuestion->InitState(true,false,NULL);
    m_pkStaticQuestion->SetAlign(DT_CENTER);

    m_pkStaticPreQuestion->InitState(true,false,NULL);
	m_pkStaticPreQuestion->SetMultiLine(true);
    m_pkStaticPreQuestion->SetAlign(DT_CENTER);
    m_pkStaticPreQuestion->SetText(g_pkStrLoader->GetString( STR_ID_SPHINX_EVENT_2 ));

    m_pkClipRange->InitState( true, false,NULL );
    m_pkClipRange->SetClipChild( true );

}


void KGCSphinxQuiz::OnCreateComplete( void )
{
    m_pkBtnO->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSphinxQuiz::OnClickO);
    m_pkBtnX->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSphinxQuiz::OnClickX);
}

void KGCSphinxQuiz::SetQuestion( KSphinxQuestionInfo &kQuestionInfo, int nTime )
{
    m_pkStaticQuestion->ToggleRender(false);
    m_pkStaticQuestion->SetTextAutoMultiline(kQuestionInfo.m_strQuestion);
    m_pkStaticPreQuestion->ToggleRender(true);

    m_dwDelayTime = timeGetTime();
    m_dwQuestionTime = 1000 * nTime + timeGetTime()+SPHINX_DELAY_TIME;
    m_iQuestionID = kQuestionInfo.m_nID;
    CreateSecondUI(nTime);

    m_pkBtnO->Lock(true);
    m_pkBtnX->Lock(true);


}

void KGCSphinxQuiz::FrameMoveInEnabledState( void )
{
    if(m_dwDelayTime != 0 && (timeGetTime() - m_dwDelayTime) > SPHINX_DELAY_TIME)
    {
        StartQuestion();
    }

    if(m_dwQuestionTime != 0 && m_dwQuestionTime < timeGetTime())
    {
        m_dwQuestionTime = 0;
        Result_SphinxReq = INT_MAX;
        KP2P::GetInstance()->Send_SphinxAnswerReq(m_iQuestionID,SQA_TIME_OVER);
        g_MyD3D->WaitForServerAck( Result_SphinxReq, INT_MAX, 5000, TIME_OUT_VALUE );
    }

    if(m_dwDelayEndQuestionTime != 0 && timeGetTime() - m_dwDelayEndQuestionTime > SPHINX_DELAY_TIME)
    {
        EndQuestion();
    }


    if( m_dwQuestionTime != 0 )
    {
        UpdateSecondUI();
    }

}

void KGCSphinxQuiz::StartQuestion()
{
    m_dwDelayTime = 0;
    m_pkStaticQuestion->ToggleRender(true);
    m_pkStaticPreQuestion->ToggleRender(false);

    m_pkBtnO->Lock(false);
    m_pkBtnX->Lock(false);

}

void KGCSphinxQuiz::EndQuestion()
{

    for(std::vector<KItem>::iterator kIt = m_vecResultItem.begin();kIt!= m_vecResultItem.end();++kIt)
    {            
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TREASURE_ITEM_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, kIt->m_ItemID, 0, false, true );
    }
    if(!m_vecResultItem.empty())
        g_KDSound.Play( "1392" );

    m_vecResultItem.clear();

    m_iQuestionID = -1;
    m_dwDelayEndQuestionTime = 0;
    m_pkStaticQuestion->SetText(L"");
    m_vecCreatedList.clear();
    m_pkClipRange->RemoveAllChild();
    g_pkUIScene->m_pkSphinxDlg->SetSphinxState();
    g_pkUIScene->m_pkSphinxDlg->ClearPaticle();

}

void KGCSphinxQuiz::CreateSecondUI( int nSecond )
{
    m_vecCreatedList = g_pkUIMgr->CreateTemplateUIListByFindName( m_pkClipRange, m_pkSecond->GetFileName().c_str() , "", m_pkSecond->GetPath().c_str() , true, nSecond );

    int i = 0;
    for(std::vector<KD3DWnd*>::iterator vit = m_vecCreatedList.begin() ; vit != m_vecCreatedList.end() ; vit++,i++ ) {
        (*vit)->InitState(true,false,NULL);
        (*vit)->SetWindowPos(D3DXVECTOR2(17.0f * static_cast<float>(i) ,0.0f));
        (*vit)->Lock(false);
    }

}

void KGCSphinxQuiz::UpdateSecondUI()
{
    DWORD i = 0;
    DWORD nTime = timeGetTime();
    for(std::vector<KD3DWnd*>::iterator vit = m_vecCreatedList.begin() ; vit != m_vecCreatedList.end() ; vit++,i++ ) 
	{
        (*vit)->Lock(((m_dwQuestionTime - nTime) / 1000) < i );
    }
}

void KGCSphinxQuiz::OnClickO()
{
    Result_SphinxReq = INT_MAX;
    KP2P::GetInstance()->Send_SphinxAnswerReq(m_iQuestionID,SQA_O);
    g_MyD3D->WaitForServerAck( Result_SphinxReq, INT_MAX, 5000, TIME_OUT_VALUE );
    m_dwQuestionTime = 0;

}

void KGCSphinxQuiz::OnClickX()
{
    Result_SphinxReq = INT_MAX;
    KP2P::GetInstance()->Send_SphinxAnswerReq(m_iQuestionID,SQA_X);
    g_MyD3D->WaitForServerAck( Result_SphinxReq, INT_MAX, 5000, TIME_OUT_VALUE );
    m_dwQuestionTime = 0;

}

void KGCSphinxQuiz::DelayEndState()
{

    m_dwQuestionTime = 0;
    m_dwDelayEndQuestionTime = timeGetTime();
    m_pkBtnO->Lock(true);
    m_pkBtnX->Lock(true);

}

void KGCSphinxQuiz::SetResultItem( std::vector<KItem> vecResultItem )
{
    m_vecResultItem.clear();
    m_vecResultItem = vecResultItem;
}