#include "stdafx.h"
#include "GCReplayRecordDlg.h"
#include "GCUI/GCReplayRecordCommonDlg.h"
#include "GCUI/GCReplayRecordAdvanceDlg.h"
#include "GCUI/GCReplayExplain.h"

IMPLEMENT_CLASSNAME( KGCReplayRecordDlg );
IMPLEMENT_WND_FACTORY( KGCReplayRecordDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayRecordDlg, "gc_replay_record_dlg" );

KGCReplayRecordDlg::KGCReplayRecordDlg( void )
{
    m_pkReplayCommon    = NULL;
    m_pkReplayAdvance   = NULL;
    m_pkReplayExplain   = NULL;

    LINK_CONTROL("category_common_tab", m_pkReplayCommon);
    LINK_CONTROL("category_advance_tab", m_pkReplayAdvance);
    LINK_CONTROL("category_explain_tab", m_pkReplayExplain);

    m_pkCommon      = NULL;
    m_pkAdvance     = NULL;
    m_pkExplain     = NULL;
    m_pkOnCommon    = NULL;
    m_pkOnAdvance   = NULL;
    m_pkOnExplain   = NULL;

    LINK_CONTROL("common_tab", m_pkCommon);
    LINK_CONTROL("advance_tab", m_pkAdvance);
    LINK_CONTROL("explain_tab", m_pkExplain);
    LINK_CONTROL("common_tab_active", m_pkOnCommon);
    LINK_CONTROL("advance_tab_active", m_pkOnAdvance);
    LINK_CONTROL("explain_tab_active", m_pkOnExplain);

    m_pkOK        = NULL;
    m_pkCancel    = NULL;
    m_pkCloseDlg  = NULL;

    LINK_CONTROL("main_confirm_btn", m_pkOK);
    LINK_CONTROL("main_cancle_btn", m_pkCancel);
    LINK_CONTROL("main_close_cross_btn", m_pkCloseDlg);
    
}

KGCReplayRecordDlg::~KGCReplayRecordDlg( void )
{
}

void KGCReplayRecordDlg::OnCreate( void )
{
    int iUseDefault = GetPrivateProfileIntA( "COMMON", "UseDefault", 1, "./BancapConfig.ini" );

#ifdef NATION_CHINA
	iUseDefault = 1;
#endif

    if( 1 == iUseDefault )
    {
        m_pkReplayCommon->InitState(true, true, this);
        m_pkReplayAdvance->InitState(false, true, this);

        m_pkCommon->InitState(false, true, this);   
        m_pkOnCommon->InitState(true, true, this); 

        m_pkAdvance->InitState(true, true, this);  
        m_pkOnAdvance->InitState(false, true, this);
    }
    else
    {
        m_pkReplayCommon->InitState(false, true, this);
        m_pkReplayAdvance->InitState(true, true, this);

        m_pkCommon->InitState(true, true, this);   
        m_pkOnCommon->InitState(false, true, this); 

        m_pkAdvance->InitState(false, true, this);  
        m_pkOnAdvance->InitState(true, true, this);
    }
    m_pkReplayExplain->InitState( false, true, this);
    
    m_pkExplain->InitState(true, true, this);  
    m_pkOnExplain->InitState(false, true, this);

    m_pkOK->InitState(true, true, this);
    m_pkCancel->InitState(true, true, this);  
    m_pkCloseDlg->InitState(true, true, this);
    m_pkCloseDlg->SetHotKey(DIK_ESCAPE);
}

void KGCReplayRecordDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCommon,  KD3DWnd::D3DWE_BUTTON_CLICK, OnTabCommon );
    GCWND_MSG_MAP( m_pkAdvance, KD3DWnd::D3DWE_BUTTON_CLICK, OnTabAdvance );
    GCWND_MSG_MAP( m_pkExplain, KD3DWnd::D3DWE_BUTTON_CLICK, OnTabExplain );

    GCWND_MSG_MAP( m_pkOK,      KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
    GCWND_MSG_MAP( m_pkCancel,  KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkCloseDlg,KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCReplayRecordDlg::OnTabCommon( void )
{
    m_pkCommon->ToggleRender(false);
    m_pkAdvance->ToggleRender(true);
    m_pkExplain->ToggleRender(true);

    m_pkOnCommon->ToggleRender(true);
    m_pkOnAdvance->ToggleRender(false);
    m_pkOnExplain->ToggleRender(false);

    m_pkReplayCommon->ToggleRender(true);
    m_pkReplayAdvance->ToggleRender(false);
    m_pkReplayExplain->ToggleRender(false);
}

void KGCReplayRecordDlg::OnTabAdvance( void )
{
    if( m_pkReplayCommon->IsRenderOn() )
    {
        int iQuality = m_pkReplayCommon->GetQuality();

        switch( iQuality )
        {
        case KGCReplayRecordCommonDlg::QUALITY_LOW:
            m_pkReplayCommon->SetAdvanceOption( 1, 1 );
            break;

        case KGCReplayRecordCommonDlg::QUALITY_MID:
            m_pkReplayCommon->SetAdvanceOption( 2, 2 );
            break;

        case KGCReplayRecordCommonDlg::QUALITY_HIGH:
            m_pkReplayCommon->SetAdvanceOption( 4, 3 );
            break;
        }
    }

    m_pkCommon->ToggleRender(true);
    m_pkAdvance->ToggleRender(false);
    m_pkExplain->ToggleRender(true);

    m_pkOnCommon->ToggleRender(false);
    m_pkOnAdvance->ToggleRender(true);
    m_pkOnExplain->ToggleRender(false);

    m_pkReplayCommon->ToggleRender(false);
    m_pkReplayAdvance->ToggleRender(true);
    m_pkReplayExplain->ToggleRender(false);
}

void KGCReplayRecordDlg::OnTabExplain( void )
{
    m_pkCommon->ToggleRender(true);
    m_pkAdvance->ToggleRender(true);
    m_pkExplain->ToggleRender(false);

    m_pkOnCommon->ToggleRender(false);
    m_pkOnAdvance->ToggleRender(false);
    m_pkOnExplain->ToggleRender(true);

    m_pkReplayCommon->ToggleRender(false);
    m_pkReplayAdvance->ToggleRender(false);
    m_pkReplayExplain->ToggleRender(true);
}

void KGCReplayRecordDlg::OnOK( void )
{
    if( m_pkReplayCommon->IsRenderOn() )
    {
        TCHAR strUseDefault[10];
        TCHAR strQuality[10];

        int iQuality = m_pkReplayCommon->GetQuality() + 1;

        swprintf_s( strUseDefault, 9, L"%d", 1 ); 
        swprintf_s( strQuality, 9, L"%d", iQuality ); 

        WritePrivateProfileString( L"COMMON", L"UseDefault", strUseDefault, L"./BancapConfig.ini" );

#if defined( NATION_KOREA )
        WritePrivateProfileString( L"KOREA", L"DefaultLevel", strQuality, L"./BancapConfig.ini" );
#else
        WritePrivateProfileString( L"OTHER", L"DefaultLevel", strQuality, L"./BancapConfig.ini" );
#endif

        switch( iQuality )
        {
        case KGCReplayRecordCommonDlg::QUALITY_LOW:
            m_pkReplayCommon->SetAdvanceOption(1, 1);
            break;

        case KGCReplayRecordCommonDlg::QUALITY_MID:
            m_pkReplayCommon->SetAdvanceOption(2, 2);
            break;

        case KGCReplayRecordCommonDlg::QUALITY_HIGH:
            m_pkReplayCommon->SetAdvanceOption(4, 3);
            break;
        }
        
    }
    else if ( m_pkReplayAdvance->IsRenderOn() )
    {
        TCHAR strUseDefault[10];
        swprintf_s( strUseDefault, 9, L"%d", 0 ); 
        WritePrivateProfileString( L"COMMON", L"UseDefault", strUseDefault, L"./BancapConfig.ini" );

        int iResolutionIndex = m_pkReplayAdvance->m_pkResolutionOptionBox->GetIndex();
        int iFrameIndex = m_pkReplayAdvance->m_pkFrameOptionBox->GetIndex();

        TCHAR strResolutionSel[10];
        TCHAR strFrameSel[10];
        swprintf_s( strResolutionSel, 9, L"%d", iResolutionIndex ); 
        swprintf_s( strFrameSel, 9, L"%d", iFrameIndex ); 

        WritePrivateProfileString( L"ADVANCE_SIZE", L"Select", strResolutionSel, L"./BancapConfig.ini" );
        WritePrivateProfileString( L"ADVANCE_FPS", L"Select", strFrameSel, L"./BancapConfig.ini" );

        int iWidth, iHeight, iFrame;

        if( iResolutionIndex == 5 )
        {
            iWidth = static_cast<int>( g_pGCDeviceManager->GetWindowScaleX() * GC_SCREEN_FLOAT_WIDTH );
            iHeight = static_cast<int>( g_pGCDeviceManager->GetWindowScaleY() * GC_SCREEN_FLOAT_HEIGHT );
        }
        else
        {
            TCHAR strWidthInfo[50];
            TCHAR strHeightInfo[50];
            
            swprintf_s( strWidthInfo, 49, L"Advance_Width%d", iResolutionIndex ); 
            swprintf_s( strHeightInfo, 49, L"Advance_Height%d", iResolutionIndex ); 

            iWidth = GetPrivateProfileInt( L"ADVANCE_SIZE", strWidthInfo, GC_SCREEN_WIDTH, L"./BancapConfig.ini" );
            iHeight = GetPrivateProfileInt( L"ADVANCE_SIZE", strHeightInfo, GC_SCREEN_HEIGHT, L"./BancapConfig.ini" );
        }

        TCHAR strFrameInfo[50];
        swprintf_s( strFrameInfo, 49, L"Advance_FPS%d", iFrameIndex );
        iFrame = GetPrivateProfileInt( L"ADVANCE_FPS", strFrameInfo, 45, L"./BancapConfig.ini" );

#if defined( NATION_KOREA )
        TCHAR strWidth[10];
        TCHAR strHeight[10];
        TCHAR strFPS[10];

        swprintf_s( strWidth, 9, L"%d", iWidth);
        swprintf_s( strHeight, 9, L"%d", iHeight);
        swprintf_s( strFPS, 9, L"%d", iFrame);

        WritePrivateProfileString( L"KOREA", L"SizeWidth", strWidth, L"./BancapConfig.ini" );
        WritePrivateProfileString( L"KOREA", L"SizeHeight", strHeight, L"./BancapConfig.ini" );
        WritePrivateProfileString( L"KOREA", L"FPS", strFPS, L"./BancapConfig.ini" );
#else
        TCHAR strWidth[10];
        TCHAR strHeight[10];
        TCHAR strFPS[10];

        swprintf_s( strWidth, 9, L"%d", iWidth);
        swprintf_s( strHeight, 9, L"%d", iHeight);
        swprintf_s( strFPS, 9, L"%d", iFrame);

        WritePrivateProfileString( L"OTHER", L"SizeWidth", strWidth, L"./BancapConfig.ini" );
        WritePrivateProfileString( L"OTHER", L"SizeHeight", strHeight, L"./BancapConfig.ini" );
        WritePrivateProfileString( L"OTHER", L"FPS", strFPS, L"./BancapConfig.ini" );
#endif

        g_MyD3D->m_KGCOption.Save_Option();
    }

    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCReplayRecordDlg::OnClose( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}
