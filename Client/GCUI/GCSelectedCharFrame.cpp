#include "stdafx.h"
#include "GCSelectedCharFrame.h"
#include "P2P.H"

IMPLEMENT_CLASSNAME( KGCSelectedCharFrame );
IMPLEMENT_WND_FACTORY( KGCSelectedCharFrame );
IMPLEMENT_WND_FACTORY_NAME( KGCSelectedCharFrame, "gc_selected_char_frame" );

KGCSelectedCharFrame::KGCSelectedCharFrame( void )
: m_iParticleTime(50)
, m_bBlinkOn(false)
,m_bClickCompleBtn(false)
{

	m_pkBG = NULL;
	LINK_CONTROL( "resizebox_bg", m_pkBG );

	m_pkIntroMainFrame = NULL;
	LINK_CONTROL( "intro_main_frame", m_pkIntroMainFrame );

	m_pkNormalForm = NULL;
	LINK_CONTROL( "normal_form", m_pkNormalForm );

	m_pkBtnConfirm = NULL;
	LINK_CONTROL( "btn_confirm", m_pkBtnConfirm );
	
	m_iCharType = -1;
}

KGCSelectedCharFrame::~KGCSelectedCharFrame( void )
{
}

void KGCSelectedCharFrame::ActionPerformed( const KActionEvent& event )
{
	if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN)
	{
		if (event.m_pWnd == m_pkBtnConfirm )
		{
			m_bClickCompleBtn = true;
            if ( m_iCharType != -1) 
			{
				if (GCFUNC::HadChar(m_iCharType) )
				{
					g_kGlobalValue.m_kUserInfo.SetCurrentChar(m_iCharType);
					g_MyD3D->m_TempPlayer.m_kUserInfo.SetCurrentChar( m_iCharType );
					g_MyD3D->m_pStateMachine->GoCharSelect();

					GCFUNC::CharGainParticle(m_iCharType);
				}
				else
				{
					Result_New_Character = INT_MAX;
					KP2P::GetInstance()->Send_NewCharChoiceReq(m_iCharType, L"");
					if( false == g_MyD3D->WaitForServerAck( Result_New_Character, INT_MAX, 10000, INT_MAX ) )
					{
						// 타임아웃 처리
						g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_APPMSG_ERR_SVR_NO_RESPONSE1), L"" );
					}

				}	
			}
		}
	}

}

void KGCSelectedCharFrame::OnCreate( void )
{
	m_pkBG->InitState(true);
	m_pkIntroMainFrame->InitState(true);

	m_pkNormalForm->InitState(false);

	m_pkBtnConfirm->InitState(true,true,this);

	m_vNomalPos = m_pkNormalForm->GetFixedWindowPos();

	g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(),KD3DWnd::D3DWE_MESSAGEBOX_OPEN);
	g_pkUIMgr->ConnectGlobalEventProcedure(KD3DWnd::D3DWE_MESSAGEBOX_OPEN, this, &KGCSelectedCharFrame::OnCloseMovie);
}

void KGCSelectedCharFrame::OnCreateComplete( void )
{
}

void KGCSelectedCharFrame::OnCloseMovie( )
{
	g_KMci.GetShowForMovie().Clear();
}

void KGCSelectedCharFrame::SetSelectedCharFrame( const int& iCharType_ )
{
	DWORD dwWidth = m_pkNormalForm->GetWidth();
	DWORD dwHeight;
	D3DXVECTOR2 vPos;
	m_iCharType = iCharType_;

    dwHeight = m_pkNormalForm->GetHeight();
    vPos = m_vNomalPos;
    m_pkBtnConfirm->ToggleRender(true);
    m_pkBtnConfirm->SetFixedWindowLocalPos(m_pkBtnConfirm->GetFixedWindowLocalPos());

    //m_pkBG->SetSize(dwWidth,dwHeight);
    //SetWindowPos(vPos);

    m_pkIntroMainFrame->SetIntroMainFrame(iCharType_);
}

void KGCSelectedCharFrame::OnInitialize()
{
    m_bBlinkOn = false;
}

void KGCSelectedCharFrame::FrameMoveInEnabledState()
{
    CheckParticleTime();
        }

void KGCSelectedCharFrame::ShowParticle()
{
    if ( false == m_bClickCompleBtn) {
#if defined( NATION_PHILIPPINE ) || defined( NATION_IDN )
        g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Nickname_Complete_Button", -0.228f, -0.578f, 0.5f), GC_LAYER_UI);
#else
        g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence("UI_Nickname_Complete_Button", -0.228f, -0.52f, 0.5f), GC_LAYER_UI);
#endif
        
    }


     //완료 버튼 클릭 깜빡임
    //여기에 코드 삽입

    m_iParticleTime = 50;
    m_bBlinkOn = false;
}

void KGCSelectedCharFrame::CheckParticleTime()
{
    if( m_iParticleTime == 0)
        return;

    m_iParticleTime--;

    if( m_iParticleTime == 0 )
    {
        m_bBlinkOn = true;
    }
}