#include "stdafx.h"
#include "GCReplayRecordCommonDlg.h"
#include "GCUI/KGCFileFindDialog.h"

IMPLEMENT_CLASSNAME( KGCReplayRecordCommonDlg );
IMPLEMENT_WND_FACTORY( KGCReplayRecordCommonDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayRecordCommonDlg, "gc_replay_record_common" );

KGCReplayRecordCommonDlg::KGCReplayRecordCommonDlg( void )
{
    m_pkQualityPlusBtn  = NULL;
    m_pkQualityMinusBth = NULL;
    m_pkCodecLinkBtn    = NULL;
    m_pkOpenBtn         = NULL;

    LINK_CONTROL("common_plus_btn", m_pkQualityPlusBtn);
    LINK_CONTROL("common_minus_btn", m_pkQualityMinusBth);
    LINK_CONTROL("common_codec_link_btn", m_pkCodecLinkBtn);
    LINK_CONTROL("common_open_btn", m_pkOpenBtn);

    for( int i=0 ; i<NUM_QUALITY ; ++i )
    {
        char szTemp[MAX_PATH];

        m_apkRecordQuality[i]       = NULL;
        m_apkRecordQualityActive[i]    = NULL;
        
        sprintf_s(szTemp, MAX_PATH - 1, "common_quality_level%d", i+1);
        LINK_CONTROL(szTemp, m_apkRecordQuality[i]);

        sprintf_s(szTemp, MAX_PATH - 1, "common_quality_level%d_active", i+1);
        LINK_CONTROL(szTemp, m_apkRecordQualityActive[i]);
    }

    m_pkStaticControlQuality    = NULL;
    m_pkStaticShotcutKeyInfo    = NULL;
    m_pkStaticStartKeyInfo      = NULL;
    m_pkStaticEndKeyInfo        = NULL;
    m_pkStaticWarning           = NULL;
    m_pkStaticCapacity          = NULL;

    LINK_CONTROL("common_static_control_quality", m_pkStaticControlQuality);
    LINK_CONTROL("common_static_shortcut_key_info", m_pkStaticShotcutKeyInfo);
    LINK_CONTROL("common_static_start_key_info", m_pkStaticStartKeyInfo);
    LINK_CONTROL("common_static_end_key_info", m_pkStaticEndKeyInfo);
    LINK_CONTROL("common_static_warning", m_pkStaticWarning);
    LINK_CONTROL("common_static_capacity", m_pkStaticCapacity);

    m_iNowQuality = -1;
}

KGCReplayRecordCommonDlg::~KGCReplayRecordCommonDlg( void )
{
}


void KGCReplayRecordCommonDlg::OnCreate( void )
{
    // 컨트롤들 초기화 
    m_pkQualityPlusBtn->InitState(true, true, this);
    m_pkQualityMinusBth->InitState(true, true, this);
    m_pkCodecLinkBtn->InitState(true, true, this);
    m_pkOpenBtn->InitState(true, true, this);
    
#if defined( NATION_KOREA )
    int iQuality = GetPrivateProfileInt( L"KOREA", L"DefaultLevel", QUALITY_MID, L"./BancapConfig.ini" );
    m_iNowQuality = iQuality - 1;
#else
    int iQuality = GetPrivateProfileInt( L"OTHER", L"DefaultLevel", QUALITY_LOW, L"./BancapConfig.ini" );
    m_iNowQuality = iQuality - 1;
#endif

    for( int i=0 ; i<NUM_QUALITY ; ++i )
    {
        m_apkRecordQuality[i]->InitState(false, true, this);
        m_apkRecordQualityActive[i]->InitState(false, true, this);
    }
    SetQualityBar(m_iNowQuality);

    m_pkStaticControlQuality->InitState(true, false, this);
    m_pkStaticControlQuality->SetAlign(DT_LEFT);

    m_pkStaticShotcutKeyInfo->InitState(true, false, this);
    m_pkStaticShotcutKeyInfo->SetAlign(DT_LEFT);

    m_pkStaticStartKeyInfo->InitState(true, false, this);
    m_pkStaticStartKeyInfo->SetAlign(DT_LEFT);

    m_pkStaticEndKeyInfo->InitState(true, false, this);
    m_pkStaticEndKeyInfo->SetAlign(DT_LEFT);

    m_pkStaticWarning->InitState(true, false, this);
    m_pkStaticWarning->SetMultiLine(true);

    m_pkStaticCapacity->InitState(true, false, this);
    m_pkStaticCapacity->SetAlign(DT_LEFT);

    m_pkStaticControlQuality->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_QUALITY ) );
    m_pkStaticShotcutKeyInfo->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_KEY ) );
    m_pkStaticStartKeyInfo->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_STRAT ) );
    m_pkStaticEndKeyInfo->SetText( g_pkStrLoader->GetString( STR_ID_REPLAY_RECORD_END ) );

#if defined( NATION_KOREA )
    m_pkStaticWarning->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_RECORD_EXPLAIN_EXPLAIN1 ) );
#else
    m_pkStaticWarning->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_RECORD_CAPACITY_EXPLAIN) );
#endif

    // 저장 경로 세팅 
    //WCHAR szDir[MAX_PATH] = {0}; 
    //::GetCurrentDirectoryW(MAX_PATH, szDir );
    //wcscat_s( szDir, MAX_PATH, L"\\Replay" );
    //m_strReplayPath = szDir;
    m_strReplayPath = L"\\Replay";

    // 남은 용량
    SetCapacity();
}


void KGCReplayRecordCommonDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkQualityPlusBtn,   KD3DWnd::D3DWE_BUTTON_CLICK, OnQualityPlus );
    GCWND_MSG_MAP( m_pkQualityMinusBth,  KD3DWnd::D3DWE_BUTTON_CLICK, OnQualityMinus );
    GCWND_MSG_MAP( m_pkCodecLinkBtn,     KD3DWnd::D3DWE_BUTTON_CLICK, OnCodecLink );
    GCWND_MSG_MAP( m_pkOpenBtn,          KD3DWnd::D3DWE_BUTTON_CLICK, OnOpenPath );

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        for( int i=0 ; i<NUM_QUALITY ; ++i )
        {
            if( event.m_pWnd == m_apkRecordQuality[i]  )
            {
                SetQualityBar(i);
                break;
            }
        }
    }
}

void KGCReplayRecordCommonDlg::SetQualityBar(int iQuality_)
{
    for( int i=0 ; i<NUM_QUALITY ; ++i )
    {
        m_apkRecordQuality[i]->ToggleRender(true);

        if( i == iQuality_ )
            m_apkRecordQualityActive[i]->ToggleRender(true);
        else
            m_apkRecordQualityActive[i]->ToggleRender(false);
    }

    m_iNowQuality = iQuality_;

    switch( m_iNowQuality )
    {
    case QUALITY_LOW:
        SetAdvanceOption(1, 1);
        break;

    case QUALITY_MID:
        SetAdvanceOption(2, 2);
        break;

    case QUALITY_HIGH:
        SetAdvanceOption(4, 3);
        break;
    }
}

void KGCReplayRecordCommonDlg::OnOpenPath( void )
{
    //Replay 폴더 생성
    CreateDirectoryA("./Replay",NULL);

    // 전체화면일때 게임화면 최소화 시키고
    if( !g_MyD3D->IsWindowMode() )
    {
        HWND hWnd = g_MyD3D->Get_hWnd();
        ShowWindow( hWnd, SW_MINIMIZE );
    }

    TCHAR szDir[MAX_PATH] = L"";
    TCHAR szDir_In[MAX_PATH] = L"\\Replay";
    
    //현재 main.exe 파일이 있는 경로를 읽어온다.
    ::GetCurrentDirectoryW(MAX_PATH, szDir );
    
    //현재 경로에 리플레이 파일이 저장될 폴더의 이름을 덧 붙인다.
    wcscat(szDir, m_strReplayPath.c_str());
    //아래 GetOpenFileName시 Replay 폴더 안으로 진입하기 위해 경로에 문자를 더한다.
    wcscat(szDir, szDir_In);

    //TCHAR lpstrFile[MAX_PATH] = L"";
    OPENFILENAME OFN;
    memset(&OFN, 0, sizeof(OPENFILENAME));

    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = NULL;
    OFN.lpstrFilter = L"avi Files(*.avi)\0*.avi\0*.AVI\0";
    //OFN.lpstrInitialDir = m_strReplayPath.c_str();
    OFN.lpstrFile = szDir;
    OFN.nMaxFile = MAX_PATH;
    OFN.Flags = OFN_READONLY | OFN_NOCHANGEDIR;

    if( GetOpenFileName(&OFN) != 0) 
    {
        // 동영상 재생 
        SHELLEXECUTEINFO sInfo;
        memset( &sInfo, 0, sizeof( SHELLEXECUTEINFO ) );
        sInfo.cbSize = sizeof( sInfo );
        sInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_DDEWAIT;
        sInfo.lpFile = OFN.lpstrFile;
        sInfo.hwnd = NULL;
        sInfo.lpVerb = L"open";
        sInfo.nShow = SW_SHOWMINIMIZED;

        ShellExecuteEx( &sInfo );
    }
    ::SetCurrentDirectoryW(szDir);
}

void KGCReplayRecordCommonDlg::OnQualityPlus( void )
{
    ++m_iNowQuality;
    if( m_iNowQuality > QUALITY_HIGH )  m_iNowQuality = QUALITY_HIGH;

    SetQualityBar( m_iNowQuality);
}

void KGCReplayRecordCommonDlg::OnQualityMinus( void )
{
    --m_iNowQuality;
    if( m_iNowQuality < QUALITY_LOW ) m_iNowQuality = QUALITY_LOW;

    SetQualityBar( m_iNowQuality );

}

void KGCReplayRecordCommonDlg::OnCodecLink( void )
{
#if defined( AVI_RECODER )
    PopUpWebPage( BDMPEG1SETUP_DOWNLOAD );
#endif
}

void KGCReplayRecordCommonDlg::SetAdvanceOption( int iResolutionIndex_, int iFrameIndex_ )
{
    g_pkUIScene->m_pkReplayRecordDlg->m_pkReplayAdvance->SetCommonResolutionOption(iResolutionIndex_);
    g_pkUIScene->m_pkReplayRecordDlg->m_pkReplayAdvance->SetCommonFrameOption(iFrameIndex_);
}

void KGCReplayRecordCommonDlg::SetCapacity( void )
{   
    TCHAR strDir[ MAX_PATH ] = { 0, };

    if( GetCurrentDirectory(MAX_PATH - 1, strDir) != 0 )
    {
        TCHAR strDrive[100] = {0, };
        _wsplitpath_s(strDir, strDrive, 99, NULL, 0, NULL, 0, NULL, 0 );
        wcscat( strDrive, L"\\");

        ULARGE_INTEGER nAvail;
        const int MEGA = 1048576;   // byte -> MB

        if( GetDiskFreeSpaceExW( strDrive, &nAvail, NULL, NULL ) != 0 )
        {
            double dFreeSpace = static_cast<int>(nAvail.QuadPart/MEGA) / 1024.0;  // byte -> GB
            
            TCHAR strCapacity[100] = {0, };
            swprintf_s( strCapacity, 99, L"%.2f", dFreeSpace );
            m_pkStaticCapacity->SetText( g_pkStrLoader->GetReplacedString( STR_ID_REPLAY_RECORD_CAPACITY, "s", strCapacity ) );
        }
    }
}
