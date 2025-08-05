#include "stdafx.h"
#include "GCMsgShutDown.h"

IMPLEMENT_CLASSNAME( KGCMsgShutDown );
IMPLEMENT_WND_FACTORY( KGCMsgShutDown );
IMPLEMENT_WND_FACTORY_NAME( KGCMsgShutDown, "gc_msgshutdown" );

const int nREMAIN_TIME = 5000;

KGCMsgShutDown::KGCMsgShutDown( void )
{
    m_pkDescText = NULL;
    m_pkTimeText = NULL;

    m_pkOK      = NULL;
    m_pkBackDesc = NULL;

    m_nRemainTime = 0;
    m_nShutdownType = 0;
    m_dwBeforeTick = 0;

    LINK_CONTROL( "message_box_text1",      m_pkDescText );
    LINK_CONTROL( "message_box_text2",      m_pkTimeText );
    LINK_CONTROL( "message_box_ok_button",  m_pkOK );

    LINK_CONTROL( "std_frame_sizing",  m_pkBackDesc );
}

KGCMsgShutDown::~KGCMsgShutDown( void )
{
}


void KGCMsgShutDown::OnCreate( void )
{
    m_nRemainTime = nREMAIN_TIME;
    
    m_pkOK->InitState( true, true, this );

    if(m_pkBackDesc != NULL)
    {
        m_pkBackDesc->InitState(true,false);
    }

    std::wstring strMsg;
    strMsg = g_pkStrLoader->GetString( STR_ID_SHUTDOWN_MSG_3 );
    strMsg += g_pkStrLoader->GetString( STR_ID_SHUTDOWN_MSG_4 );

    m_pkDescText->SetTextAutoMultiline( strMsg );
    m_pkTimeText->SetAlign( DT_CENTER );
    
    SetTimeText();

}

void KGCMsgShutDown::OnCreateComplete( void )
{
    switch( m_nShutdownType ) { 
        case 0:         // 강제 셧다운 - 이전에 사용하던것
            {
            std::wstring strMsg;
            strMsg = g_pkStrLoader->GetString( STR_ID_SHUTDOWN_MSG_3 );
            strMsg += g_pkStrLoader->GetString( STR_ID_SHUTDOWN_MSG_4 );

            m_pkDescText->SetTextAutoMultiline( strMsg );
            m_pkTimeText->SetAlign( DT_CENTER );
            SetTimeText();
            }
            break;
        case 1:         // 강제 셧다운 - 넷마블 모듈 이용
            m_pkDescText->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_FORCE_SHUTDOWN_MSG_NOW ) );
            m_pkTimeText->SetAlign( DT_CENTER );
            SetTimeText();
            break;
        case 2:         // 선택적 셧다운 - 넷마블 모듈 이용 
            m_pkDescText->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SELECTIVE_SHUTDOWN_MSG_NOW ) );
            m_pkTimeText->SetAlign( DT_CENTER );
            SetTimeText();
            break;
    }

    m_pkOK->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCMsgShutDown::OnCloseMsgBox);
    m_pkOK->SetHotKey(DIK_RETURN);
}

void KGCMsgShutDown::ActionPerformed( const KActionEvent& event )
{

}

bool KGCMsgShutDown::InitDialog( IN const KDialogInfo& kInfo_ )
{
    m_nRemainTime = static_cast< int >( kInfo_.m_lParam * 1000 );
    m_nShutdownType = static_cast< int >( kInfo_.m_lParam2 );
    m_dwBeforeTick = 0;
    return true;
}

void KGCMsgShutDown::OnCloseMsgBox()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCMsgShutDown::OnDestroy( void )
{
    g_MyD3D->m_pStateMachine->GoExitClient();
}

void KGCMsgShutDown::SetTimeText()
{
    int nTime = m_nRemainTime / 1000;
    std::wstring strMsg =  g_pkStrLoader->GetReplacedString( STR_ID_SECOND_TIME, "i", nTime );

    m_pkTimeText->SetText( strMsg );
}

void KGCMsgShutDown::FrameMoveInEnabledState( void )
{
    DWORD dwCurTick = GetTickCount();

    if ( m_dwBeforeTick != 0 ) 
    {
        m_nRemainTime -= ( dwCurTick - m_dwBeforeTick );

        if ( m_nRemainTime < 0 ) 
        {
            OnCloseMsgBox();           
        }

        SetTimeText();
    }

    m_dwBeforeTick = dwCurTick;
}
