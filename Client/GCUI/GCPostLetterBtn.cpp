#include "stdafx.h"
#include "GCPostLetterBtn.h"

IMPLEMENT_CLASSNAME( KGCPostLetterBtn );
IMPLEMENT_WND_FACTORY( KGCPostLetterBtn );
IMPLEMENT_WND_FACTORY_NAME( KGCPostLetterBtn, "gc_postletterbutton" );

bool KGCPostLetterBtn::m_bReceiveState = false;
bool KGCPostLetterBtn::m_bRecvMsgState = false;
int  KGCPostLetterBtn::m_iMsgViewTime = 550; // 55fps * 10 => 10초

KGCPostLetterBtn::KGCPostLetterBtn( void )
{
    m_pkNewMark = NULL;
    m_pkNewLetterBalloon = NULL;
    m_pkStaticNewLetter = NULL;
    LINK_CONTROL( "mark",           m_pkNewMark );
    LINK_CONTROL( "newLetter",      m_pkNewLetterBalloon );
    LINK_CONTROL( "newLetterMsg",   m_pkStaticNewLetter );
}

KGCPostLetterBtn::~KGCPostLetterBtn( void )
{
}

void KGCPostLetterBtn::OnCreate( void )
{
    m_pkNewMark->InitState( false );
    m_pkNewLetterBalloon->InitState( false );
    m_pkStaticNewLetter->InitState( true );
    m_pkStaticNewLetter->SetAlign( DT_CENTER );

    ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPostLetterBtn::OnClickBtn );
}

void KGCPostLetterBtn::OnCreateComplete( void )
{
    SetOrder_Top();
}

void KGCPostLetterBtn::OnDestroy( void )
{
    DisconnectAllEventProcedure();
}

void KGCPostLetterBtn::FrameMoveInEnabledState( void )
{
	if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM ) { 
		ToggleRender(false);
	}

    if( m_bReceiveState ) { 
        this->SetWndMode( KD3DWnd::D3DWM_ACTIVE );
        m_pkNewMark->ToggleRender( true );
    }
    else { 
        m_pkNewMark->ToggleRender( false );
    }

    if( m_bRecvMsgState ) { 
        if( false == m_pkNewLetterBalloon->IsRenderOn() ) { 
            ShowNewLetterMsg( m_bRecvMsgState );
        }

        --m_iMsgViewTime;
    }
    else if( false == m_bRecvMsgState )  { 
        ShowNewLetterMsg( m_bRecvMsgState );
    }

    if( m_iMsgViewTime < 0 ) { 
        m_bRecvMsgState = false;
        m_iMsgViewTime = 550;
        
        ShowNewLetterMsg( m_bRecvMsgState );
    }
}

void KGCPostLetterBtn::SetReceiveOn( void )
{
    m_bReceiveState = true;
    m_bRecvMsgState = true;
}

void KGCPostLetterBtn::SetReceiveOff( void )
{
    m_bReceiveState = false;
    
    m_bRecvMsgState = false;
    m_iMsgViewTime  = 550;
}

void KGCPostLetterBtn::OnClickBtn( void )
{
    if( g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY ) { 
        if( g_MyD3D->m_pStateMachine->GetState() == GS_MY_INFO_FROM_ROOM ) { 
            return;
        }

        if( g_pkUIScene->m_pkPostLetterDlg ) { 
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_POST_LETTER );
        }

        if( m_bReceiveState ) {     
            m_bReceiveState = false;
        }

        if( m_bRecvMsgState ) { 
            m_bRecvMsgState = false;
            m_iMsgViewTime  = 550;
            
            ShowNewLetterMsg( m_bRecvMsgState );
        }
    }
}

void KGCPostLetterBtn::ShowNewLetterMsg( bool bShow_ )
{
    if( bShow_ ) { 
        m_pkNewLetterBalloon->ToggleRender( true );
        m_pkStaticNewLetter->ToggleRender( true );
        m_pkStaticNewLetter->SetFontColor( D3DCOLOR_ARGB(255, 0, 0, 0 ) );
        m_pkStaticNewLetter->SetText( g_pkStrLoader->GetString( STR_ID_NEW_LETTER_RECV ) ) ;
    }
    else { 
        m_pkNewLetterBalloon->ToggleRender( false );
        m_pkStaticNewLetter->ToggleRender( false );
        m_pkStaticNewLetter->SetText( L"" );
    }
}