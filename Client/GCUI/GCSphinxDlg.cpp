#include "stdafx.h"
#include "GCSphinxDlg.h"

#include "GCSphinxChallenge.h"
#include "GCSphinxInfoView.h"
#include "GCSphinxResultItemView.h"
#include "GCSphinxQuiz.h"
#include "GCSphinxFace.h"


IMPLEMENT_CLASSNAME( KGCSphinxDlg );
IMPLEMENT_WND_FACTORY( KGCSphinxDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCSphinxDlg, "gc_event_sphinx_dlg" );

KGCSphinxDlg::KGCSphinxDlg( void )
: m_pkSphinxFace(NULL)
, m_pkEndBtn(NULL)
, m_pkSphinxInfoView(NULL)
, m_pkSphinxQuiz(NULL)
, m_pkSphinxChallenge(NULL)
, m_pkSphinxResultItemView(NULL)
, m_iState(0)
, m_bCreateComplete( false )
{
    LINK_CONTROL("end_btn",m_pkEndBtn);
    LINK_CONTROL("sphinx_face",m_pkSphinxFace);
    LINK_CONTROL("sphinx_info_view",m_pkSphinxInfoView);
    LINK_CONTROL("sphinx_quiz",m_pkSphinxQuiz);
    LINK_CONTROL("sphinx_challenge",m_pkSphinxChallenge);
    LINK_CONTROL("sphinx_view",m_pkSphinxResultItemView);
    
}

KGCSphinxDlg::~KGCSphinxDlg( void )
{
}

void KGCSphinxDlg::OnCreate( void )
{
    m_pkEndBtn->InitState(true,true,this);
    m_pkSphinxInfoView->InitState(true,true,this);
    m_pkSphinxFace->InitState(true,true,this);
    m_pkSphinxQuiz->InitState(false,true,this);
    m_pkSphinxChallenge->InitState(false,true,this);
    m_pkSphinxResultItemView->InitState(false,true,this);

    SetSphinxState(SEDS_CHALLENGE);

}

void KGCSphinxDlg::OnCreateComplete( void )
{
    m_pkEndBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCSphinxDlg::OnCloseEventBoard);
    m_pkEndBtn->SetHotKey(DIK_ESCAPE);
    
    m_bCreateComplete = true;
    
    CreatePaticle();
}


void KGCSphinxDlg::OnDestroy( void )
{
    ClearPaticle();
}

void KGCSphinxDlg::ActionPerformed( const KActionEvent& event )
{
    
}


void KGCSphinxDlg::InitSphinxInfo( int iTotalAnswer )
{
    m_pkSphinxInfoView->SetTotalAnswer(iTotalAnswer);
}

void KGCSphinxDlg::SetSphinxState()
{
    //      SEDS_CHALLENGE = 0, // 도전 상태
    //      SEDS_QUIZ = 1, // 퀴즈
    //      SEDS_RESULT_ITEM_VIEW = 2, // 보상 아이템 뷰 - 이벤트 정보 보지 않음.

    m_pkSphinxInfoView->ToggleRender(m_iState != SEDS_RESULT_ITEM_VIEW);
    m_pkSphinxQuiz->ToggleRender(m_iState == SEDS_QUIZ);
    m_pkSphinxChallenge->ToggleRender(m_iState == SEDS_CHALLENGE);
    m_pkSphinxResultItemView->ToggleRender(m_iState == SEDS_RESULT_ITEM_VIEW);
    m_pkEndBtn->Lock(m_iState != SEDS_CHALLENGE);


    m_pkSphinxInfoView->UpdateCoin();
    SetSphinxFace(SEFS_NORMAL);
}

void KGCSphinxDlg::SetSphinxState( int iState , bool update)
{
    if(m_iState == SEDS_QUIZ && iState != SEDS_CHALLENGE) return;

    m_iState = iState;
    if(update)
        SetSphinxState();
}



void KGCSphinxDlg::SetSphinxFace( int iState )
{
    m_pkSphinxFace->SetSphinxFace(iState);
}

void KGCSphinxDlg::OnCloseEventBoard()
{
    m_pkSphinxFace->RemoveFaceTexture();
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));

}


void KGCSphinxDlg::SetQuizInfo( int iSecondCount, KSphinxQuestionInfo &kQuestionInfo )
{

    m_pkSphinxInfoView->SetDiffculty(kQuestionInfo.m_nLevel);
    m_pkSphinxQuiz->SetQuestion(kQuestionInfo,iSecondCount);

    SetSphinxState( KGCSphinxDlg::SEDS_QUIZ );
}

void KGCSphinxDlg::SetSphinxResult( int iState , std::vector<KItem> vecItem)
{
    SetSphinxState( SEDS_CHALLENGE, false );
    SetSphinxFace( iState );
    m_pkSphinxQuiz->SetResultItem(vecItem);
    m_pkSphinxQuiz->DelayEndState();
}

void KGCSphinxDlg::ClearPaticle()
{
    for(std::vector<CParticleEventSeqPTR>::iterator mIter = m_vecParticle.begin();mIter != m_vecParticle.end();mIter++)
    {
        g_ParticleManager->DeleteSequence( (*mIter) );
    }
    m_vecParticle.clear();
}

void KGCSphinxDlg::CreatePaticle()
{
    ClearPaticle();

    m_vecParticle.push_back(g_ParticleManager->CreateSequence( "UI_OX_01", 0.0f, 0.0f, 0.5f ));
    m_vecParticle.push_back(g_ParticleManager->CreateSequence( "UI_OX_02", 0.0f, 0.0f, 0.5f ));
    m_vecParticle.push_back(g_ParticleManager->CreateSequence( "UI_OX_03", 0.0f, 0.0f, 0.5f ));
    m_vecParticle.push_back(g_ParticleManager->CreateSequence( "UI_OX_04", 0.0f, 0.0f, 0.5f ));
//#if !defined  (NATION_BRAZIL)
    m_vecParticle.push_back(g_ParticleManager->CreateSequence( "UI_OX_05", 0.0f, 0.0f, 0.5f ));
//#endif

    for(std::vector<CParticleEventSeqPTR>::iterator mIter = m_vecParticle.begin();mIter != m_vecParticle.end();mIter++)
    {
        g_ParticleManager->SetLayer( (*mIter) , GC_LAYER_UI);
    }

}

void KGCSphinxDlg::FrameMoveInEnabledState()
{
    if(m_vecParticle.empty() && g_pkUIScene->IsEmptyWaitMessageBox())
    {
        CreatePaticle();
    }
}

void KGCSphinxDlg::SetResultItemList( KEVENT_SPHINX_ITEM_LIST_NOT mapItemList )
{
    m_pkSphinxResultItemView->SetRewardItem(mapItemList);
}

void KGCSphinxDlg::SetResultItemList()
{
    m_pkSphinxResultItemView->SetRewardItem();

}

void KGCSphinxDlg::UpdateCoin()
{
    m_pkSphinxInfoView->UpdateCoin();
}

bool KGCSphinxDlg::GetCreateComplete()
{
    return m_bCreateComplete;
}

void KGCSphinxDlg::close()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
    KGCHotEventBtn::SetHotStateOff();
}
