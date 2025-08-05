#include "stdafx.h"
#include "GCReplayRecordAdvanceDlg.h"
#include "GCUI/GCRecordOptionBox.h"

IMPLEMENT_CLASSNAME( KGCReplayRecordAdvanceDlg );
IMPLEMENT_WND_FACTORY( KGCReplayRecordAdvanceDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayRecordAdvanceDlg, "gc_replay_record_advance" );

KGCReplayRecordAdvanceDlg::KGCReplayRecordAdvanceDlg( void )
{
    m_pkStaticResolutionInfo    = NULL;
    m_pkStaticFrameInfo         = NULL;
    m_pkStaticSoundInfo         = NULL;
    m_pkStaticNowResolution     = NULL;
    m_pkStaticNowFrame          = NULL;
    m_pkStaticSE                = NULL;
    m_pkStaticBGM               = NULL;

    LINK_CONTROL("advance_static_resolution_info", m_pkStaticResolutionInfo);
    LINK_CONTROL("advance_static_frame_info", m_pkStaticFrameInfo);
    LINK_CONTROL("advance_static_sound_info", m_pkStaticSoundInfo);
    LINK_CONTROL("static_now_resolution_info", m_pkStaticNowResolution);
    LINK_CONTROL("static_now_frame_info", m_pkStaticNowFrame);
    LINK_CONTROL("advance_static_SE", m_pkStaticSE);
    LINK_CONTROL("advance_static_BGM", m_pkStaticBGM);

    m_pkOpenResolutionBtn   = NULL;
    m_pkOpenFrameBtn        = NULL;   

    LINK_CONTROL("open_resolution_btn", m_pkOpenResolutionBtn);
    LINK_CONTROL("open_frame_btn", m_pkOpenFrameBtn);

    m_pkResolutionOptionBox = NULL;
    m_pkFrameOptionBox      = NULL;

    LINK_CONTROL("resolution_option_box ", m_pkResolutionOptionBox);
    LINK_CONTROL("frame_option_box", m_pkFrameOptionBox);

    m_pkCheckboxBGM = NULL;
    m_pkCheckboxSE  = NULL;
    m_pkCheckBGM    = NULL;
    m_pkCheckSE     = NULL;

    LINK_CONTROL("checkbox_BGM", m_pkCheckboxBGM);
    LINK_CONTROL("checkbox_SE", m_pkCheckboxSE);
    LINK_CONTROL("check_BGM", m_pkCheckBGM);
    LINK_CONTROL("check_SE", m_pkCheckSE);
}

KGCReplayRecordAdvanceDlg::~KGCReplayRecordAdvanceDlg( void )
{
}

void KGCReplayRecordAdvanceDlg::OnCreate( void )
{
    m_pkStaticResolutionInfo->InitState(true, false, this);
    m_pkStaticFrameInfo->InitState(true, false, this);
    m_pkStaticSoundInfo->InitState(true, false, this);
    m_pkStaticNowResolution->InitState(true, false, this);
    m_pkStaticNowFrame->InitState(true, false, this);
    m_pkStaticSE->InitState(true, false, this);
    m_pkStaticBGM->InitState(true, false, this);

    m_pkOpenResolutionBtn->InitState(true, true, this);
    m_pkOpenFrameBtn->InitState(true, true, this);

    m_pkResolutionOptionBox->InitState(false, true, this);
    m_pkFrameOptionBox->InitState(false, true, this);

    m_pkStaticResolutionInfo->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_RESOLUTION ) );
    m_pkStaticFrameInfo->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_FRAME ) );;
    m_pkStaticSoundInfo->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_SOUND ) );
    m_pkStaticSE->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_SE ) );
    m_pkStaticBGM->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_BGM ) );

    m_pkCheckboxBGM->InitState(true, true, this);
    m_pkCheckboxSE->InitState(true, true, this);
    m_pkCheckBGM->InitState(true, false, this);
    m_pkCheckSE->InitState(true, false, this);

    if( m_vecResolutionInfo.empty() )
        SetInfo();

    // 초기값 
    int iResolutionIndex = GetPrivateProfileIntA( "ADVANCE_SIZE", "Select", 5, "./BancapConfig.ini" );
    if( iResolutionIndex < 0 ) iResolutionIndex = 1;
    m_pkStaticNowResolution->SetText( m_vecResolutionInfo[iResolutionIndex - 1] );

    int iFrameIndex = GetPrivateProfileIntA( "ADVANCE_FPS", "Select", 2, "./BancapConfig.ini" );
    if( iFrameIndex < 0 ) iFrameIndex = 1;
    m_pkStaticNowFrame->SetText( m_vecFrameInfo[iFrameIndex - 1] );

    m_pkCheckSE->ToggleRender( Is_Sound );
    m_pkCheckBGM->ToggleRender( g_KMci.IsPlay() );
}

void KGCReplayRecordAdvanceDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkOpenResolutionBtn,   KD3DWnd::D3DWE_BUTTON_CLICK, OnOpenResolutionOption );
    GCWND_MSG_MAP( m_pkOpenFrameBtn,        KD3DWnd::D3DWE_BUTTON_CLICK, OnOpenFrameOption );
    GCWND_MSG_MAP( m_pkCheckboxBGM,   KD3DWnd::D3DWE_BUTTON_CLICK, OnCheckBGM );
    GCWND_MSG_MAP( m_pkCheckboxSE,    KD3DWnd::D3DWE_BUTTON_CLICK, OnCheckSE );

    GCWND_MSG_MAP( m_pkResolutionOptionBox, KGCRecordOptionBox::FET_OK, SetResoiutionOption );
    GCWND_MSG_MAP( m_pkFrameOptionBox,      KGCRecordOptionBox::FET_OK, SetFrameOption );
}

void KGCReplayRecordAdvanceDlg::FrameMoveInEnabledState( void )
{
    POINT pt = g_pkInput->GetMousePos();
    D3DXVECTOR2 dxvPos( static_cast<float>(pt.x), static_cast<float>(pt.y) );

    if( g_pkInput->BtnPressed( KInput::MBLEFT ) )
    {
        bool bResolution = m_pkResolutionOptionBox->CheckPosInWindowBound(dxvPos);
        if( m_pkResolutionOptionBox->IsRenderOn() && !bResolution ) 
            SetResoiutionOption();

        bool bFrame = m_pkFrameOptionBox->CheckPosInWindowBound(dxvPos);
        if( m_pkFrameOptionBox->IsRenderOn() && !bFrame )
            SetFrameOption();
    }
}

void KGCReplayRecordAdvanceDlg::OnOpenResolutionOption( void )
{
    m_pkStaticNowResolution->ToggleRender( false );
    m_pkOpenResolutionBtn->ToggleRender( false );

    m_pkResolutionOptionBox->SetList( m_vecResolutionInfo );
    m_pkResolutionOptionBox->ToggleRender( true );
}

void KGCReplayRecordAdvanceDlg::OnOpenFrameOption( void )
{
    m_pkStaticNowFrame->ToggleRender( false );
    m_pkOpenFrameBtn->ToggleRender( false );

    m_pkFrameOptionBox->SetList( m_vecFrameInfo );
    m_pkFrameOptionBox->ToggleRender( true );
}

void KGCReplayRecordAdvanceDlg::SetResoiutionOption( void )
{
    m_pkResolutionOptionBox->ToggleRender( false );

    m_pkStaticNowResolution->SetText( m_pkResolutionOptionBox->GetIndexText() );
    m_pkStaticNowResolution->ToggleRender( true );
    m_pkOpenResolutionBtn->ToggleRender( true );
}

void KGCReplayRecordAdvanceDlg::SetFrameOption( void )
{
    m_pkFrameOptionBox->ToggleRender( false );

    m_pkStaticNowFrame->SetText( m_pkFrameOptionBox->GetIndexText() );
    m_pkStaticNowFrame->ToggleRender( true );
    m_pkOpenFrameBtn->ToggleRender( true );
}


void KGCReplayRecordAdvanceDlg::SetInfo( void )
{
   int iNumResolution = GetPrivateProfileIntA( "ADVANCE_SIZE", "Num", 4, "./BancapConfig.ini" ); 
   int iNumFrame = GetPrivateProfileIntA( "ADVANCE_FPS", "Num", 3, "./BancapConfig.ini" ); 

   for( int i=0 ; i<iNumResolution ; ++i )
   {
       char  strWidth[50] = {0, };
       char  strHeight[50] = {0, };
       sprintf_s( strWidth, 49, "Advance_Width%d", i+1);
       sprintf_s( strHeight, 49, "Advance_Height%d", i+1);
       int iWidth = GetPrivateProfileIntA( "ADVANCE_SIZE", strWidth , 400, "./BancapConfig.ini" );
       int iHeight = GetPrivateProfileIntA( "ADVANCE_SIZE", strHeight, 300, "./BancapConfig.ini" ); 

       TCHAR strResolution[100] = {0, };
       swprintf_s( strResolution, 99, L"%d x %d", iWidth, iHeight );

       m_vecResolutionInfo.push_back( strResolution );
   }

   // 현재 해상도 추가
   m_vecResolutionInfo.push_back( g_pkStrLoader->GetString(STR_ID_REPLAY_RECORD_CUR_RESOLUTION) );

   for( int i=0 ; i<iNumFrame ; ++i )
   {
       char strTemp[50] = {0, };
       sprintf_s( strTemp, 49, "Advance_FPS%d", i+1);
       int iFPS = GetPrivateProfileIntA( "ADVANCE_FPS", strTemp , 45, "./BancapConfig.ini" );

       TCHAR strFPS[50] = {0, };
       swprintf_s( strFPS, 49, L"%d", iFPS);

       m_vecFrameInfo.push_back( strFPS );

   }
}

void KGCReplayRecordAdvanceDlg::OnCheckBGM( void )
{
    m_pkCheckBGM->ToggleRender( !m_pkCheckBGM->IsRenderOn() );
    g_pOptionMusic->SetToggle();
}

void KGCReplayRecordAdvanceDlg::OnCheckSE( void)
{
    m_pkCheckSE->ToggleRender( !m_pkCheckSE->IsRenderOn() );
    g_pOptionSound->SetToggle();
}

void KGCReplayRecordAdvanceDlg::SetCommonResolutionOption( int iIndex_ )
{
    m_pkResolutionOptionBox->SetList( m_vecResolutionInfo );
    m_pkResolutionOptionBox->SetIndex(iIndex_);
    m_pkStaticNowResolution->SetText( m_pkResolutionOptionBox->GetIndexText() );
}

void KGCReplayRecordAdvanceDlg::SetCommonFrameOption( int iIndex_ )
{
    m_pkFrameOptionBox->SetList(m_vecFrameInfo);
    m_pkFrameOptionBox->SetIndex(iIndex_);
    m_pkStaticNowFrame->SetText( m_pkFrameOptionBox->GetIndexText() );
}
