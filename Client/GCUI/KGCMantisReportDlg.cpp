#include "stdafx.h"
#include "gcui/KGCMantisReportDlg.h"
#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCMantisReportDlg );
IMPLEMENT_WND_FACTORY( KGCMantisReportDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCMantisReportDlg, "gc_mantis_report_dlg" );

KGCMantisReportDlg::KGCMantisReportDlg(void)
: m_pkStaticTitle(NULL)
, m_pkStaticProject(NULL)
, m_pkStaticID(NULL)
, m_pkStaticPasswd(NULL)
, m_pkStaticScreenShot(NULL)
, m_pkCheckBoxScreenShot(NULL)
, m_pkEditID(NULL)
, m_pkEditPasswd(NULL)
, m_pkEditSummary(NULL)
, m_pkEditDesc(NULL)
, m_pkBtnOk(NULL)
, m_pkBtnCancel(NULL)
, m_pkBtnDropDownProject(NULL)
, m_pkPopupMenuProject(NULL)
{   
    LINK_CONTROL( "static_title",           m_pkStaticTitle );
    LINK_CONTROL( "static_project",         m_pkStaticProject );
    LINK_CONTROL( "static_id",              m_pkStaticID );
    LINK_CONTROL( "static_password",        m_pkStaticPasswd );
    LINK_CONTROL( "static_screenshot",      m_pkStaticScreenShot );

    LINK_CONTROL( "checkbox_screenshot",    m_pkCheckBoxScreenShot );
    LINK_CONTROL( "popupbox_project",       m_pkPopupMenuProject );

    LINK_CONTROL( "edit_id",                m_pkEditID );
    LINK_CONTROL( "edit_password",          m_pkEditPasswd );
    LINK_CONTROL( "edit_summary",           m_pkEditSummary );
    LINK_CONTROL( "edit_desc",              m_pkEditDesc );

    LINK_CONTROL( "btn_ok",                 m_pkBtnOk );
    LINK_CONTROL( "btn_cancel",             m_pkBtnCancel );

    LINK_CONTROL( "btn_dropdown_project",   m_pkBtnDropDownProject );
}

KGCMantisReportDlg::~KGCMantisReportDlg(void)
{
}

void KGCMantisReportDlg::OnCreate( void )
{
    m_pkStaticTitle->InitState(true);
    m_pkStaticTitle->SetAlign(DT_CENTER);
    m_pkStaticTitle->SetText(L"[ Issue the bug on the Mantis ]");

    m_pkStaticProject->InitState(true);
    m_pkStaticProject->SetText(L"Select project");

    m_pkStaticID->InitState(true);
    m_pkStaticID->SetAlign(DT_RIGHT);
    m_pkStaticID->SetText(L"ID :");

    m_pkStaticPasswd->InitState(true);
    m_pkStaticPasswd->SetAlign(DT_RIGHT);
    m_pkStaticPasswd->SetText(L"Passwd :");

    m_pkStaticScreenShot->InitState(true);
    m_pkStaticScreenShot->SetText(L"Add Screen Shot");

    m_pkEditID->InitState(true, false, this);
    m_pkEditPasswd->InitState(true, false, this);
    m_pkEditPasswd->SetPasswd(true);

    m_pkEditSummary->InitState(true, false, this);
    m_pkEditSummary->SetText(L"Please enter the title of issue.");

    m_pkEditDesc->InitState(true, true, this);
    m_pkEditDesc->SetMultiLine( true );
    m_pkEditDesc->SetNewLine( true );
    m_pkEditDesc->SetLimitText(MAX_PATH);
    m_pkEditDesc->SetLimitLine(10);    
    m_pkEditDesc->SetText(L"Please enter the description of the issue.");

    m_pkBtnOk->InitState(true, true, this);
    m_pkBtnCancel->InitState(true, true, this);
    m_pkBtnDropDownProject->InitState(true, true, this);

    m_pkCheckBoxScreenShot->InitState(true, true, this);
    m_pkCheckBoxScreenShot->SetCheck(true);

    m_pkPopupMenuProject->InitState(false, true, this);

    m_kMantisReportParm.Init();

    InitProjectList();
}

void KGCMantisReportDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnCancel,			    KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    _GCWND_MSG_MAP( m_pkCheckBoxScreenShot,	    KD3DWnd::D3DWE_BUTTON_CLICK, OnCheck );
    _GCWND_MSG_MAP( m_pkBtnDropDownProject,	    KD3DWnd::D3DWE_BUTTON_CLICK, OnPopUpMenu );
    _GCWND_MSG_MAP( m_pkBtnOk,	                KD3DWnd::D3DWE_BUTTON_CLICK, OnConfirm );

    if( event.m_pWnd == m_pkPopupMenuProject && event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED ) {
        std::map<int, std::wstring>::iterator mit = m_mapProjectList.find(static_cast<int>(event.m_dwlParam));

        if( mit != m_mapProjectList.end() ) {
            m_pkStaticProject->SetText( mit->second );
            m_kMantisReportParm.wstrProject = mit->second;
        }

        return;
    }

    if( event.m_pWnd == m_pkEditID ) {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB ) {
            m_pkEditPasswd->SetFocus();
            return;
        }
    }

    if( event.m_pWnd == m_pkEditPasswd ) {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB ) {
            m_pkEditSummary->SetFocus();
            return;
        }        
    }

    if( event.m_pWnd == m_pkEditSummary ) {
        if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB ) {
            m_pkEditDesc->SetFocus();
            return;
        }        
    }
}


void KGCMantisReportDlg::FrameMoveInEnabledState( void )
{

}

void KGCMantisReportDlg::PostDraw( void )
{

}

void KGCMantisReportDlg::OnConfirm()
{    
    if( m_pkEditID->GetText() == L"" ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, L"Please enter Mantis ID!!", 
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    } else if( m_pkStaticProject->GetText() == L"Select Project" ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, L"Please select project!!", 
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    } else if( m_pkEditPasswd->GetText() == L"" ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, L"Please enter Mantis Password!!", 
            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }


    m_kMantisReportParm.wstrCategory = L"General";
    m_kMantisReportParm.wstrServerityEng = L"minor";
    m_kMantisReportParm.wstrID = m_pkEditID->GetText();
    m_kMantisReportParm.wstrPasswd = m_pkEditPasswd->GetText();
    m_kMantisReportParm.wstrSummary = m_pkEditSummary->GetText();
    m_kMantisReportParm.wstrDesc = m_pkEditDesc->GetText();

    std::wstring wstrCmdInput = L"\"";
    wstrCmdInput.append( m_kMantisReportParm.wstrProject );
    wstrCmdInput.append( L"\"" );
    wstrCmdInput.append( L" \"" );
    wstrCmdInput.append( L"General" );
    wstrCmdInput.append( L"\"" );
    wstrCmdInput.append( L" \"" );
    wstrCmdInput.append( L"minor" );
    wstrCmdInput.append( L"\"" );
    wstrCmdInput.append( L" \"" );
    wstrCmdInput.append( m_kMantisReportParm.wstrSummary );
    wstrCmdInput.append( L"\"" );
    wstrCmdInput.append( L" \"" );
    wstrCmdInput.append( m_kMantisReportParm.wstrDesc );
    wstrCmdInput.append( L"\"" );
    wstrCmdInput.append( L" \"" );
    wstrCmdInput.append( m_kMantisReportParm.wstrID );
    wstrCmdInput.append( L"\"" );
    wstrCmdInput.append( L" \"" );
    wstrCmdInput.append( m_kMantisReportParm.wstrPasswd );
    wstrCmdInput.append( L"\"" );

    if( m_pkCheckBoxScreenShot->GetCheck() == true ) {
        std::wstring wstrScreenShotFileName;
        if( g_MyD3D->GetScreenShotFileName( wstrScreenShotFileName ) ) {
            std::string strScreenshotPath = g_strCurrentPath + "ScreenShot\\" + GCUTIL_STR::GCStrWideToChar( wstrScreenShotFileName.c_str() );

            wstrCmdInput.append( L" \"" );
            wstrCmdInput.append( wstrScreenShotFileName );
            wstrCmdInput.append( L"\"" );

            wstrCmdInput.append( L" \"" );
            wstrCmdInput.append( GCUTIL_STR::GCStrCharToWide( strScreenshotPath.c_str() ) );
            wstrCmdInput.append( L"\"" );
        }
    }

    std::string strToolPath = g_strCurrentPath + "MantisAutoReport\\MantisConnector.exe";

    ShellExecute( NULL, L"open", GCUTIL_STR::GCStrCharToWide( strToolPath.c_str()), (LPCTSTR)wstrCmdInput.c_str(), NULL, SW_SHOWNORMAL );
    OnClose();
}

void KGCMantisReportDlg::OnCheck()
{
    m_pkCheckBoxScreenShot->SetCheck( !m_pkCheckBoxScreenShot->GetCheck() );
}

void KGCMantisReportDlg::OnClose()
{
    g_KDSound.Play( "31" );
    this->ToggleRender(false);
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCMantisReportDlg::OnPopUpMenu()
{    
    if( m_pkPopupMenuProject->IsRenderOn() ) {
        m_pkPopupMenuProject->ToggleRender(false);
    } else {
        m_pkPopupMenuProject->ToggleRender(true);
    }

    g_KDSound.Play( "31" );    
}

void KGCMantisReportDlg::InitProjectList()
{
    std::map<std::wstring, std::vector<std::wstring>> mapMantisProjectList;
    g_kGlobalValue.GetMantisProjectList(mapMantisProjectList);

    std::map<std::wstring, std::vector<std::wstring>>::iterator mit;

#if defined(NATION_BRAZIL)
    mit = mapMantisProjectList.find(L"BR");
#elif defined(NATION_USA)
    mit = mapMantisProjectList.find(L"USA");
#elif defined(NATION_PHILIPPINE)
    mit = mapMantisProjectList.find(L"PH");
#elif defined(NATION_IDN)
    mit = mapMantisProjectList.find(L"IDN");
#elif defined(NATION_LATINAMERICA)
    mit = mapMantisProjectList.find(L"LA");
#elif defined(NATION_TAIWAN)
    mit = mapMantisProjectList.find(L"TW");
#elif defined(NATION_THAILAND)
    mit = mapMantisProjectList.find(L"TH");
//#elif defined(NATION_EU)
//    mit = mapMantisProjectList.find(L"EU");
#else
    mit = mapMantisProjectList.find(L"KR");
#endif

    m_pkPopupMenuProject->ClearMenu();
    m_mapProjectList.clear();

    int iIndex = 1;

    if( mit != mapMantisProjectList.end() ) {
        for( int i = 0; i < static_cast<int>( mit->second.size() ); i++  ) {            
            m_mapProjectList.insert( std::map<int, std::wstring>::value_type( iIndex, mit->second[i] ) );
            iIndex++;
        }
    }

    mit = mapMantisProjectList.find(L"COMMON");
    if( mit != mapMantisProjectList.end() ) {
        for( int i = 0; i < static_cast<int>( mit->second.size() ); i++  ) {            
            m_mapProjectList.insert( std::map<int, std::wstring>::value_type( iIndex, mit->second[i] ) );
            iIndex++;
        }
    }

    std::map<int, std::wstring>::iterator mitList = m_mapProjectList.begin();
    for( ; mitList != m_mapProjectList.end(); ++mitList ) {
        m_pkPopupMenuProject->AddMenu( mitList->first, mitList->second );
    }        
}

void KGCMantisReportDlg::TakeScreenShot()
{
    if( g_MyD3D->MakeJPEGFile() == false ) {
        if ( g_pkChatManager != NULL ) {
            g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_SNAPSHOT_SAVE_FAILED), KGCChatManager::CHAT_TYPE_SCREEN_SHOT );
        }
    }
}
