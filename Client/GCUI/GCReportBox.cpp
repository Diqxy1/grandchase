#include "stdafx.h"
#include ".\gcreportbox.h"
//

//

//


#include "GCGameOverlayUI.h"
#include "MyD3D.h"
#include "Headup Display.h"
#include "../GCStateMachine.h"

IMPLEMENT_CLASSNAME( KGCReportBox );
IMPLEMENT_WND_FACTORY( KGCReportBox );
IMPLEMENT_WND_FACTORY_NAME( KGCReportBox, "gc_fairplay_report_box" );

KGCReportBox::KGCReportBox(void)
{
    m_pkStatic_Alert        = NULL;
    m_pkStatic_ReportWho    = NULL;
    m_pkStatic_ReportWhy    = NULL;
    m_pkStatic_Comment      = NULL;
    m_pkEdit_Comment        = NULL;
    m_pkCombo_ReportWhy     = NULL;
    m_pkBtn_Ok              = NULL;
    m_pkBtn_Close           = NULL;
    m_pkBtn_Exit            = NULL;

    LINK_CONTROL( "ok_btn",             m_pkBtn_Ok );
    LINK_CONTROL( "close_btn",          m_pkBtn_Close);
    LINK_CONTROL( "edit_comment",       m_pkEdit_Comment );
    LINK_CONTROL( "static_comment",     m_pkStatic_Comment );
    LINK_CONTROL( "static_report_why",  m_pkStatic_ReportWhy );
    LINK_CONTROL( "static_report_who",  m_pkStatic_ReportWho );
    LINK_CONTROL( "reason_combobox",    m_pkCombo_ReportWhy );
    LINK_CONTROL( "static_alert",       m_pkStatic_Alert );
    LINK_CONTROL( "exit_button",        m_pkBtn_Exit );
}

KGCReportBox::~KGCReportBox(void)
{
}

void KGCReportBox::OnCreate( void )
{
    m_pkStatic_Alert->InitState( true );
    m_pkStatic_ReportWho->InitState( true, false, NULL );
    m_pkStatic_ReportWhy->InitState( true, false, NULL );
    m_pkStatic_Comment->InitState( true, false, this );
    m_pkEdit_Comment->InitState( true, true, this );
    m_pkBtn_Ok->InitState( true, true, this );
    m_pkBtn_Close->InitState( true, true, this );
    m_pkBtn_Exit->InitState( true, true, this );
    m_pkCombo_ReportWhy->InitState( true, true, this );

    m_pkEdit_Comment->SetSelfInputCheck( false );
    m_pkEdit_Comment->SetText( L"" );
    m_pkEdit_Comment->SetLimitText( 300 );
    m_pkEdit_Comment->EnableSelfFocusCheck( false );

    m_pkCombo_ReportWhy->DeleteAllItem();
    m_pkCombo_ReportWhy->SetComboStatic( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT8 ), DT_CENTER );
    m_pkCombo_ReportWhy->AddString( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT5 ) );
    m_pkCombo_ReportWhy->AddString( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT6 ) );
    m_pkCombo_ReportWhy->AddString( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT7 ) );
    m_pkCombo_ReportWhy->AddString( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT15 ) );

    m_pkStatic_ReportWho->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_FAIRPLAY_REPORT4 , "ll" , m_strReporter, m_strBadGuy ));
    m_pkStatic_ReportWhy->SetText( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT2 ) );
    m_pkStatic_Comment->SetText( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT3 ) );

    m_pkStatic_Alert->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT14 ) );

    m_pkBtn_Close->SetHotKey( DIK_ESCAPE );
}

void KGCReportBox::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkBtn_Ok )
        {
            // 신고 사유를 선택하지 않았다면?
            if( m_pkCombo_ReportWhy->GetCurSel() == -1 )
            {
                // 메세지 박스에 신고 사유 선택하라고 보챈다
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT8 ),
                                        L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
            }
            else
            {
                YouAreUnderArrest();
                SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_OK ) );
            }
            return;
        }

        if( event.m_pWnd == m_pkBtn_Close ||
            event.m_pWnd == m_pkBtn_Exit )
        {
            SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_CANCEL ) );
            return;
        }

        if( event.m_pWnd == m_pkEdit_Comment )
        {
            m_pkEdit_Comment->SetFocus();
            return;
        }
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_ESCAPE )
    {

    }
}
void KGCReportBox::FrameMoveInEnabledState( void )
{
    if ( false == g_pkGameOverlayUI->HasEditFocus() && !g_MyD3D->MyHead->IsCheatMode )
    {
        if ( g_pkGameOverlayUI->HasChatEventHandling() )
        {
            // 에디트 컨트롤의 채팅 처리와의 간섭을 막는다.
            g_pkGameOverlayUI->SetChatEventHandling( false );
        }
        else
        {
            g_MyD3D->MyHead->IsCheatMode = true;
            g_pkGameOverlayUI->ShowChatToggleMsg( false );
            g_pkGameOverlayUI->SetEditText( NULL );
            g_pkGameOverlayUI->SetEditFocus();
        }
    }
    m_pkEdit_Comment->SetFocus();
}

void KGCReportBox::PostChildDraw( void )
{

}

bool KGCReportBox::SetReporterAndBadGuy( std::wstring strReporter_ /* 신고자 */, std::wstring strBadGuy_ /* 신고 당한자 */ )
{
    if( g_kGlobalValue.m_iReportAvailableCount <= 0 || 
        (g_kGlobalValue.m_bReportInGame == true && g_MyD3D->m_pStateMachine->GetState() == GS_GAME))
        return false;  

    m_strReporter = strReporter_;
    m_strBadGuy = strBadGuy_;
    m_pkStatic_ReportWho->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_FAIRPLAY_REPORT4 , "ll" , m_strReporter, m_strBadGuy ));

    //일단 신고를 눌렀으면 그상태에서 게임이 종료되어 버리거나 상태가 바뀌어 버릴 수 있다.
    // 신고를 염려한 상대방이 나가버릴 수도 있다. 일단 신고하러 들어왔으면 자료를 남기고 보낼 수 있도록 해야한다.
	SiGCPairPlaySystem()->SetBadGuyNickName(m_strBadGuy);
    SiGCPairPlaySystem()->OutPutFilesForReport();
    m_pkEdit_Comment->SetFocus();

    return true;
}

void KGCReportBox::YouAreUnderArrest()
{
    SiGCPairPlaySystem()->SendReport( m_strBadGuy, m_pkCombo_ReportWhy->GetCurSel() + 1, m_pkEdit_Comment->GetText());
}

void KGCReportBox::FrameMoveInDisabledState( void )
{
    if( SiGCPairPlaySystem()->IsFileDeleted() == false )
    {
        SiGCPairPlaySystem()->ClearReportDatas();
    }
}

bool KGCReportBox::CheckCondition( IN const KDialogInfo& kInfo_ )
{
#if defined( DISABLE_BADGUY_REPORT )
    return false;
#endif
    //if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL ||
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL ||
        g_MyD3D->MyReplay->m_iCurrState == RP_VIEW )
        return false;

    if( this->SetReporterAndBadGuy( kInfo_.m_strText1, kInfo_.m_strText2 ) == false )
    {
        if( g_MyD3D->m_pStateMachine->GetState() == GS_GAME )
        {
            //  게임중간에 신고한 경우 
            if (g_kGlobalValue.m_bReportInGame == true)
            {
                g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT16 ));
            }
            else
            {
                g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT11 ));
            }

        }
        else
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT11 ) );
        }
        return false;
    }

    return true;
}