#include "stdafx.h"
#include "KGCQuiz.h"
#include "KGCBingo.h"
#include "GCMyInfoScene.h"

#include "../Procedure.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCQuiz );
IMPLEMENT_WND_FACTORY( KGCQuiz );
IMPLEMENT_WND_FACTORY_NAME( KGCQuiz, "gc_quiz" );

KGCQuiz::KGCQuiz(void)
{
	m_pkHintBtn = NULL; 	

	//퀴즈들 다 부르자.
	LINK_CONTROL( "quiz_string", m_pkStaticQuiz);
	LINK_CONTROL( "remain_resurection_string",m_pkStaticRemainResuretion);
	LINK_CONTROL( "remain_time_string",m_pkStaticRemainTime);

	LINK_CONTROL( "circle", m_pkCircle);
	LINK_CONTROL( "cross", m_pkCross);
	LINK_CONTROL( "close_button", m_pkClose);
	LINK_CONTROL( "hint_button", m_pkHintBtn);

	char str[256] = "";
	for(unsigned int i =0; i< MAX_QUESTION; i++)
	{
		sprintf_s( str, sizeof(str), "question%d", i );
		LINK_CONTROL( str, m_pkQuestion[i]);

		sprintf_s( str, sizeof(str), "question%d_string", i );
		LINK_CONTROL( str, m_pkStaticQuestion[i]);

		sprintf_s( str, sizeof(str), "check%d", i );
		LINK_CONTROL( str, m_pkCheck[i]);
	}

	m_DrawAnswerTime = -1;
	m_bResolved = false;
	m_iRemainTime = 30;
	m_iReseructionBouns = 0;
	m_dwAccTimeNot = 0;
	m_iAnswer = -1;

	m_pClockEffect1	=	NULL;
	m_pClockEffect2 =	NULL;

    m_iMaxReseructionNum = 4;
}

void KGCQuiz::OnCreate(void)
{
	m_pkClose->InitState(true,true,this);
	m_pkStaticQuiz->InitState(true);
	m_pkStaticRemainResuretion->InitState(true);
	m_pkStaticRemainTime->InitState(true);

	m_pkCircle->InitState(false,false,this);
	m_pkCross->InitState(false,false,this);

	if ( m_pkHintBtn )
		m_pkHintBtn->InitState(false,true,this);

	for(int i =0; i< MAX_QUESTION; i++)
	{
		m_pkQuestion[i]->InitState(true,true,this);
		m_pkCheck[i]->InitState(false,false,this);
		m_pkStaticQuestion[i]->SetFontColor(0xFFFFFFFF);
	}

	m_pkStaticQuiz->SetFontColor(0xFFFFFFFF);
	m_pkStaticRemainResuretion->SetFontColor(0xFFFFFFFF);
	m_pkStaticRemainTime->SetFontColor(0xFFFFFFFF);
	m_pkStaticRemainResuretion->SetAlign(DT_CENTER);
	m_pkStaticRemainTime->SetAlign(DT_CENTER);

	m_bResurrection = false;
	m_bClockEffect = false;
	m_vecPreAnwer.clear();

	

	m_pClockEffect1	=	NULL;
	m_pClockEffect2 =	NULL;
}

void KGCQuiz::SetStatic()
{
	WCHAR strTemp[20];
	swprintf_s( strTemp, 20, L"%02d", (int)m_iReseructionBouns );
	m_pkStaticRemainResuretion->SetText( strTemp );

	swprintf_s( strTemp, 20, L"%02d", (int)m_iRemainTime );
	m_pkStaticRemainTime->SetText( strTemp );
}

void KGCQuiz::SetQuizInfo(KBingoQuestionInfo kQuestion)
{
	m_iQuizNumber = kQuestion.m_nID;
	m_wstrQuestion =kQuestion.m_strQuestion;
	m_mapChoiceWstring = kQuestion.m_mapChoices;
	m_iCorrectAnswer = kQuestion.m_nAnswer;

	m_pkStaticQuiz->SetText(m_wstrQuestion);
	m_pkStaticQuiz->SetTextAutoMultiline(m_wstrQuestion);

	std::map<int,std::wstring>::iterator mapvit = m_mapChoiceWstring.begin();
	for(;mapvit != m_mapChoiceWstring.end(); ++mapvit)
		m_pkStaticQuestion[mapvit->first - 1]->SetText(mapvit->second);
}


void KGCQuiz::FrameMoveInEnabledState(void)
{
	OnAnswerResult();	//퀴즈 입력 시 정답,오답 이펙트 출력 및 퀴즈를 부활을 써서 다시 풀 수 있는지 확인하는 함수

	if (m_iAnswer != -1)
		OnInputAnswer();	//정답,오답을 입력했을 시의 처리

	CalAnswerRemainTime();//퀴즈 대답시간 깍아주자

	if(m_iRemainTime < 5 && m_iRemainTime > 0 )
		DrawClockEffect();	//퀴즈 대답 시간이 5초 이하일 때 시계 이펙트를 그려준다.

	if(m_iRemainTime == 0)
		OnNoAnswerQuiz();	//퀴즈를 대답 못했을 때의 처리

	SetStatic();	//퀴즈 내에서 문자들을 업데이트 해준다.
}

void KGCQuiz::OnExit()
{
	//g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	ReleaseCapture();
	m_DrawAnswerTime = -1;
	m_bClockEffect = false;

	CMinMax<float> Zero(0.0f,0.0f);
	if( m_pClockEffect1 )
		m_pClockEffect1->SetShow( false );
	if( m_pClockEffect2 )
		m_pClockEffect2->SetShow( false );
}

void KGCQuiz::OnClose()
{
	OnExit();

	m_iRemainTime = 30;
	
	m_bResurrection = false;

	//if(m_bResurrection == true)
	//	InPutAnswerAndResurrection(m_bResurrection);
	//else
	//	InPutAnswerAndResurrection(m_bResurrection);


	for ( int i = 0; i < MAX_QUESTION; ++i )
		m_pkQuestion[i]->Lock(false);

	g_pkUIScene->m_pkMyInfoScene->UpdateGameData();
	g_pkUIScene->m_pkMyInfoScene->UpdateData();
}

void KGCQuiz::OnDestroy()
{

}

void KGCQuiz::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
	GCWND_MSG_MAP( m_pkHintBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnHintSearch );

	

    int iCheckNum = 0;
    for( int i = 0; i < MAX_QUESTION; ++i )
    {
        if( m_pkCheck[i]->IsRenderOn() == true )
            iCheckNum++;
    }

    if( event.m_dwCode == D3DWE_BUTTON_CLICK && ( iCheckNum == 0 || (iCheckNum <= m_iMaxReseructionNum && m_bResurrection == true)) )
	{
		for ( unsigned int i = 0; i < MAX_QUESTION; ++i )
		{
			if(event.m_pWnd == m_pkQuestion[i])
			{
				m_iAnswer = i;
				m_pkCheck[i]->ToggleRender(true);
			}
		}
	}
}

void KGCQuiz::GetPreAnswerVectorclear()
{
	bool bEmpty = m_vecPreAnwer.empty();
	bool bFull	= !bEmpty;
	if( bFull )
	{
		m_vecPreAnwer.clear();

		for ( int i = 0; i < MAX_QUESTION; ++i )
		{
			m_pkQuestion[i]->Lock(false);
			m_pkCheck[i]->ToggleRender(false);
		}
	}
}

KGCQuiz::~KGCQuiz()
{

}

void KGCQuiz::ShowReserructionEffect()
{
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_Resserection_01" , 0.0f ,0.0f, 0.5f ) , GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_Resserection_02" , 0.0f ,0.0f, 0.5f ) , GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_Resserection_03" , 0.0f ,0.0f, 0.5f ) , GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_Resserection_04" , 0.0f ,0.0f, 0.5f ) , GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_Resserection_05" , 0.0f ,0.0f, 0.5f ) , GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_Resserection_06" , 0.0f ,0.0f, 0.5f ) , GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_Resserection_07" , 0.0f ,0.0f, 0.5f ) , GC_LAYER_UI);
	g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "Bingo_Resserection_08" , 0.0f ,0.0f, 0.5f ) , GC_LAYER_UI);
}

void KGCQuiz::ShowClockEffect()
{
	if( NULL == m_pClockEffect1 ){
		m_pClockEffect1 = g_ParticleManager->CreateSequence( "Bingo_Clock_01" , 0.0f, 0.0f, 0.5f );
		m_pClockEffect1->SetShow( true );
		g_ParticleManager->SetLayer( m_pClockEffect1, GC_LAYER_UI);
	}
	if( NULL == m_pClockEffect2 ){
		m_pClockEffect2 = g_ParticleManager->CreateSequence( "Bingo_Clock_02" , 0.0f, 0.0f, 0.5f );
		m_pClockEffect2->SetShow( true );
		g_ParticleManager->SetLayer( m_pClockEffect2, GC_LAYER_UI);
	}
}

void KGCQuiz::DrawAnswerResult()
{
	if(m_bResolved == true)
	{
		m_pkCircle->ToggleRender(true);
		m_pkCross->ToggleRender(false);
	}
	else
	{
		m_pkCross->ToggleRender(true);
		m_pkCircle->ToggleRender(false);
	}
}

void KGCQuiz::InPutAnswerAndResurrection(bool bResurrection)
{
	Result_BingoRequestAnswer = INT_MAX;
	KP2P::GetInstance()->Send_AnswerBingoQuestion(m_iQuizNumber,m_iAnswer + 1,g_pkUIScene->m_pkBingo->GetCurrentBingoPosition(),bResurrection);
//	g_MyD3D->WaitForServerAck( Result_BingoRequestAnswer, INT_MAX, 5000, TIME_OUT_VALUE );
}

void KGCQuiz::CalAnswerRemainTime()
{
	int iTime = (timeGetTime() - m_dwAccTimeNot );

	if( iTime >= 1000 && (  m_iRemainTime > 0 ) )	// 1초 이상 지났으면
	{
		m_dwAccTimeNot = timeGetTime();
		m_iRemainTime--;
	}
}

void KGCQuiz::OnAnswerResult()
{
	if(m_DrawAnswerTime > 0)
	{
		m_DrawAnswerTime--;
		DrawAnswerResult();
	}
	else if (m_DrawAnswerTime == 0)
	{
		for ( int i = 0; i < MAX_QUESTION; ++i )
		{
			if(find(m_vecPreAnwer.begin(),m_vecPreAnwer.end(),i) == m_vecPreAnwer.end())
				m_pkQuestion[i]->Lock(false);
		}

		m_pkCircle->ToggleRender(false);
		m_pkCross->ToggleRender(false);

		//오답이고 부활 찬스가 있으면
		if(m_bResolved == false && m_iReseructionBouns > 0 )
		{
			m_iReseructionBouns--;

			m_DrawAnswerTime = -1;
			m_iAnswer = -1;
			m_bResurrection = true;
			ShowReserructionEffect();
		}
        else if(m_bResolved == false && m_iReseructionBouns == 0 )
        {
            OnExit();
        }
		else
        {
            if( m_bResolved )
            {
                OnExit();
            }
            else
            {
                m_pkCircle->ToggleRender(false);
                m_pkCross->ToggleRender(true);
            }
        }
	}
}

void KGCQuiz::OnInputAnswer()
{
	//if( m_bResurrection  == true)
	//{
	//	InPutAnswerAndResurrection(m_bResurrection);
	//	m_bResurrection = false;
	//}
	//else
		InPutAnswerAndResurrection(m_bResurrection);

	m_vecPreAnwer.push_back(m_iAnswer);
	m_iAnswer = -1;
}

void KGCQuiz::DrawClockEffect()
{
	if(m_bClockEffect == false)
	{
		ShowClockEffect();
		m_bClockEffect = true;
	}
}

void KGCQuiz::OnNoAnswerQuiz()
{
	if(m_bResurrection == true)
		InPutAnswerAndResurrection(m_bResurrection);
	else
		InPutAnswerAndResurrection(m_bResurrection);

	OnExit();
	m_bResurrection = false;
}

void KGCQuiz::LockQuestionList( bool bLock )
{
	for(int i =0; i< MAX_QUESTION; i++)
		m_pkQuestion[i]->Lock(bLock);
}

void KGCQuiz::OnHintSearch()
{
	g_kGlobalValue.SearchProcess( GC_GLOBAL_DEFINE::strSearchURL );
}