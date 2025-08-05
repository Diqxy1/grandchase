
#include "stdafx.h"
// KShow.cpp: implementation of the KShow class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)



//
//
#include "KShow.h"
//
#include "FindSpecificationFilters.h"
////
//


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KShow::KShow()
{
    // 이 클래스를 사용하기 전에 CoCreateInstance가 호출되어 있어야 한다.
    m_pGraphBuilder = NULL;
    m_bMovie        = false;
    m_fDBVolume     = 0.0;      // 최고값이다.
    
    m_pMediaSeeking = NULL;
    m_pMediaControl = NULL;
    m_pMediaEventEx = NULL;
    m_pVideoWindow  = NULL;
    m_pBacisAudio   = NULL;

    m_Repeat        = false;

	m_bIsPlaying	= false;

    m_hrCoInitialize = S_FALSE;
}

KShow::~KShow()
{
    Clear();
    if( SUCCEEDED( m_hrCoInitialize ) )
    {
        CoUninitialize();
    }

    SAFE_DELETE( m_pGraphBuilder );
}

HRESULT KShow::InitShow( HWND hWnd, const char* filename, const char* path ,const char* tablename_  )
{
    m_hWnd = hWnd;
  
    // 재생할 미디어 타입(audio only, video..)과 파일 이름을 얻어온다.
//    HRESULT hr;
    //KLuaManager* luaMgr = KSingleton<KLuaManager>::GetInstance();
	KLuaManager m_kLuaMgr;

    if( GCFUNC::LoadLuaScript( m_kLuaMgr , filename ) == false )
		return E_FAIL;
    
    if( m_kLuaMgr.BeginTable( tablename_ ) != S_OK ) return S_FALSE;
    
    int nNumFile;
    char szTableName[20] = {0};
    std::string strPath( path );


	FileInfo    kFileInfo;
	std::string strFilePath;
	std::string strFileName;

    m_kLuaMgr.GetValue( "num", nNumFile );

    for( int i = 0; i < nNumFile; i++ )
    {
        

        sprintf( szTableName, "media%d", i );

#ifdef FEATURE_2021
        if (m_kLuaMgr.BeginTable(szTableName) == E_FAIL)
        {
            kFileInfo.m_bwindow = false;
            ::strcpy(kFileInfo.m_strFile, "");
            m_vFileInfo.push_back(kFileInfo);
            continue;
        }
#endif

        if( FAILED( m_kLuaMgr.GetValue( "window", kFileInfo.m_bwindow ) ) )
			kFileInfo.m_bwindow = false;

        m_kLuaMgr.GetValue( "file", strFileName );
        m_kLuaMgr.EndTable();

        strFilePath = strPath + strFileName;
        ::strcpy( kFileInfo.m_strFile, strFilePath.c_str() );

        m_vFileInfo.push_back( kFileInfo );
    }
    
    assert( nNumFile == m_vFileInfo.size() );

    m_kLuaMgr.EndTable();

    return S_OK;    
}

HRESULT KShow::Play( int nIndex, bool repeat )
{
    HRESULT hr;
    

    m_Repeat = repeat;
    // 이전에 재생중이던 파일을 처리한다.
    if( m_pGraphBuilder ) Clear();

    // 그래프 빌더 포인터를 얻어옴.
    if( SUCCEEDED( m_hrCoInitialize ) )
    {
        CoUninitialize();
    }
    m_hrCoInitialize = CoInitialize( NULL);
    hr = CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraphBuilder);
    if( FAILED( hr ) ) return hr;
    
    if ( !m_pGraphBuilder ) 
        return E_FAIL;


    WCHAR wFileName[MAX_PATH];                  // media file (WCHAR)

    //////////////////////////////////////////////////////////////////////////
    // 파일 렌더링. (direct show는 내부적으로 WCHAR를 사용한다.)
	if(m_vFileInfo.size() <= static_cast<unsigned int>(nIndex) || nIndex <0 ) return E_FAIL;
    MultiByteToWideChar(CP_ACP, 0, m_vFileInfo[nIndex].m_strFile, -1, wFileName, MAX_PATH);
    m_bMovie = m_vFileInfo[nIndex].m_bwindow;

	//hr = ProgressConnectionMP3(m_pGraphBuilder);
    ProgressConnectionMP3(m_pGraphBuilder);
	if(FAILED(hr))
	{
		return E_FAIL;
    }

    hr = m_pGraphBuilder->RenderFile( wFileName, NULL ); // second must be NULL!

    if( hr == S_OK )
    {
        int a = 0;
    }
    else
    {
        MessageBoxA( NULL, m_vFileInfo[nIndex].m_strFile, NULL, NULL );
        SAFE_RELEASE( m_pGraphBuilder );
        return E_FAIL;
    }


    // 미디어 재생에 쓰이는 인터페이스들을 얻어온다. 
    m_pGraphBuilder->QueryInterface( IID_IMediaEventEx, (void**)&m_pMediaEventEx );
    if( NULL == m_pMediaEventEx )   return E_FAIL;
    m_pGraphBuilder->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );
    if( NULL == m_pMediaControl )   return E_FAIL;
    m_pGraphBuilder->QueryInterface( IID_IMediaSeeking, (void**)&m_pMediaSeeking );
    if( NULL == m_pMediaSeeking )   return E_FAIL;
    m_pGraphBuilder->QueryInterface( IID_IVideoWindow, (void**)&m_pVideoWindow );
    if( NULL == m_pVideoWindow )   return E_FAIL;
    m_pGraphBuilder->QueryInterface( IID_IBasicAudio, (void**)&m_pBacisAudio );
    if( NULL == m_pBacisAudio )   return E_FAIL;
    
    // 통신 윈도우 설정.
    hr = m_pMediaEventEx->SetNotifyWindow( (OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0);
    if( FAILED( hr ) ) return hr;
    //  ^ 세번째 인수 0은 메시지에 보낼 lParam의 값. 사용하지 않으므로 항상 0.

    if( m_bMovie ) // 동영상일 경우를 위해 재생윈도우 설정.
    {
        m_pVideoWindow->put_Owner( (OAHWND)m_hWnd );
		m_pVideoWindow->SetWindowPosition(m_dwPosX,m_dwPosY,m_dwWidth,m_dwHeight);
        m_pVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
        m_pVideoWindow->SetWindowForeground( OATRUE );
        //OnSize();
		
    }

    // 배경음악 재생 시작.
    hr = m_pMediaControl->Run();
    if( FAILED( hr ) ) return hr;

    if( !m_bMovie && m_pBacisAudio )    // 동영상이 아닐때 
    {
        hr = m_pBacisAudio->put_Volume((long)m_fDBVolume);
        if( FAILED( hr ) ) return hr;
        // ^ (동영상이어도 되지만 지금은 인트로 동영상에 오디오 스트림이 없다.)
    }
    
	m_bIsPlaying = true;
    return S_OK;
}

HRESULT KShow::Stop( int nIndex )
{
    // 이전에 재생중이던 파일을 처리한다.
    //if( m_pGraphBuilder ) Clear();

    /*

    // 그래프 빌더 포인터를 얻어옴. 
    JIF( CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
        (void**)&m_pGraphBuilder) );

    WCHAR wFileName[MAX_PATH];                  // media file (WCHAR)

    //////////////////////////////////////////////////////////////////////////
    // 파일 렌더링. (direct show는 내부적으로 WCHAR를 사용한다.)
    MultiByteToWideChar(CP_ACP, 0, m_vFileInfo[nIndex].m_strFile, -1, wFileName, MAX_PATH);
    m_bMovie = m_vFileInfo[nIndex].m_bwindow;

    LIF( m_pGraphBuilder->RenderFile( wFileName, NULL ) ); // second must be NULL!

//    if( FAILED( hr ) )
    //{
        //KLOGL(( 0, "%s파일을 재생할 수 없습니다", m_vFileInfo[nIndex].m_strFile ));
        //SAFE_RELEASE( m_pGraphBuilder );
        //return E_FAIL;
    //}

    // 미디어 재생에 쓰이는 인터페이스들을 얻어온다. 
    m_pGraphBuilder->QueryInterface( IID_IMediaEventEx, (void**)&m_pMediaEventEx );
    m_pGraphBuilder->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );
    m_pGraphBuilder->QueryInterface( IID_IMediaSeeking, (void**)&m_pMediaSeeking );
    m_pGraphBuilder->QueryInterface( IID_IVideoWindow, (void**)&m_pVideoWindow );
    m_pGraphBuilder->QueryInterface( IID_IBasicAudio, (void**)&m_pBacisAudio );
    
    // 통신 윈도우 설정.
    JIF( m_pMediaEventEx->SetNotifyWindow( (OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0) );
    //  ^ 세번째 인수 0은 메시지에 보낼 lParam의 값. 사용하지 않으므로 항상 0.

    if( m_bMovie ) // 동영상일 경우를 위해 재생윈도우 설정.
    {
        LIF( m_pVideoWindow->put_Owner( (OAHWND)m_hWnd ) );
        LIF( m_pVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ));
        LIF( m_pVideoWindow->SetWindowForeground( OATRUE ) );
        OnSize();
    }

  */
    // 배경음악 재생 시작.
    HRESULT hr;
	if( m_pMediaControl != NULL )
		hr = m_pMediaControl->Stop();

    /*
    if( !m_bMovie && m_pBacisAudio )    // 동영상이 아닐때 
    {
        JIF( m_pBacisAudio->put_Volume((long)m_fDBVolume) );
        // ^ (동영상이어도 되지만 지금은 인트로 동영상에 오디오 스트림이 없다.)
    }
    */
    
    return S_OK;
}

HRESULT KShow::Continue( int nIndex )
{
    // 그래프 빌더 포인터를 얻어옴. 
    HRESULT hr;
    hr = CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
        (void**)&m_pGraphBuilder);
    if( FAILED( hr ) ) return hr;
	
    if ( !m_pGraphBuilder ) 
        return E_FAIL;	

    WCHAR wFileName[MAX_PATH];                  // media file (WCHAR)

    //////////////////////////////////////////////////////////////////////////
    // 파일 렌더링. (direct show는 내부적으로 WCHAR를 사용한다.)
    MultiByteToWideChar(CP_ACP, 0, m_vFileInfo[nIndex].m_strFile, -1, wFileName, MAX_PATH);
    m_bMovie = m_vFileInfo[nIndex].m_bwindow;

	hr = ProgressConnectionMP3(m_pGraphBuilder);
    if( FAILED( hr ) ) return hr;

    m_pGraphBuilder->RenderFile( wFileName, NULL ); // second must be NULL!

    // 미디어 재생에 쓰이는 인터페이스들을 얻어온다. 
    m_pGraphBuilder->QueryInterface( IID_IMediaEventEx, (void**)&m_pMediaEventEx );
    m_pGraphBuilder->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );
    m_pGraphBuilder->QueryInterface( IID_IMediaSeeking, (void**)&m_pMediaSeeking );
    m_pGraphBuilder->QueryInterface( IID_IVideoWindow, (void**)&m_pVideoWindow );
    m_pGraphBuilder->QueryInterface( IID_IBasicAudio, (void**)&m_pBacisAudio );
    
    // 통신 윈도우 설정.
    hr = m_pMediaEventEx->SetNotifyWindow( (OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0);
    if( FAILED( hr ) ) return hr;
    //  ^ 세번째 인수 0은 메시지에 보낼 lParam의 값. 사용하지 않으므로 항상 0.

    if( m_bMovie ) // 동영상일 경우를 위해 재생윈도우 설정.
    {
        m_pVideoWindow->put_Owner( (OAHWND)m_hWnd );
        m_pVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
        m_pVideoWindow->SetWindowForeground( OATRUE );
        //OnSize();
    }

    // 배경음악 재생 시작.
    hr = m_pMediaControl->Run();
    if( FAILED( hr ) ) return hr;

    if( !m_bMovie && m_pBacisAudio )    // 동영상이 아닐때 
    {
        hr = m_pBacisAudio->put_Volume((long)m_fDBVolume);
        if( FAILED( hr ) ) return hr;
        // ^ (동영상이어도 되지만 지금은 인트로 동영상에 오디오 스트림이 없다.)
    }
    
    return S_OK;
}

HRESULT KShow::Clear()
{
    
    HRESULT hr;

    do
    {
        // 필터 그래프가 생성되지 않은 경우는 무시. 
        if( !m_pGraphBuilder ) break;
        if( !m_pMediaControl ) break;


        // 중지 전에 Puase
        hr = m_pMediaControl->Pause();
        if( FAILED( hr ) ) break;

        // 재생 중지.
        hr = m_pMediaControl->Stop();
        if( FAILED( hr ) ) break;

        // 메시지 통지 윈도우 해제.
        if( m_pMediaEventEx != NULL )
        {
            hr = m_pMediaEventEx->SetNotifyWindow( NULL, WM_GRAPHNOTIFY, 0);
            if( FAILED( hr ) ) break;
        }

        if( m_bMovie ) // 동영상일 경우 재생윈도우 해제.
        {
            m_pVideoWindow->put_Visible( OAFALSE );
            //        LIF( m_pVideoWindow->put_Owner( NULL ) );
            /* 원래는, 동영상 스트림을 종료할 때 소유 윈도우와 관계를 끊어주어야 안전하지만,
            전체화면 모드로 시작할 때에 이를 실행시키면 포커스를 잃고 최소화 되기 때문에 
            주석처리한다. 관계를 끊어주는 이유는 동영상 창은 계속 살아있고, 소유 윈도우는 
            파괴되었을때 엉뚱한 곳으로 메시지를 보내려고 시도하는 경우를 방지하기 위함인데,
            게임 프로그래밍에서는 그럴 경우가 없다고 판단되므로 호출하지 않는다.
            전체화면 시작 모드가 아닐 때에는 호출해주어도 큰 상관이 없으며, 
            전체화면 시작시에 문제가 되어 주석처리해둔다. */
        }

		m_bIsPlaying = false;

    }while(false);
    
    SAFE_RELEASE( m_pMediaEventEx );
    SAFE_RELEASE( m_pMediaControl );
    SAFE_RELEASE( m_pMediaSeeking );
    SAFE_RELEASE( m_pVideoWindow );
    SAFE_RELEASE( m_pBacisAudio );
    SAFE_RELEASE( m_pGraphBuilder );
    

    return S_OK;
}

BOOL KShow::HandleEvent( LPARAM lParam )
{
//    HRESULT hr;
	if (m_pMediaEventEx == NULL)
		return FALSE;
    long lEvent, lParam1, lParam2;
    long lTimeOut = 0;
    BOOL bNextMode = FALSE; // Intro -> Lobby로 넘어가야 할 시점임을 알려준다. 

    while(SUCCEEDED( m_pMediaEventEx->GetEvent( &lEvent, &lParam1, &lParam2, lTimeOut ) ) )
    {
        switch( lEvent )
        {
        case EC_COMPLETE : // 스트림의 재생이 다 끝났을 경우. 

            if( m_bMovie )  // 동영상이었다면, 이를 종료시키고 대기실로 넘어간다.
                bNextMode = TRUE;
            else if( m_Repeat == true )    // 배경음이었다면, 처음부터 다시 재생한다.
            {
                LONGLONG pos = 0;
                m_pMediaSeeking->SetPositions( &pos, AM_SEEKING_AbsolutePositioning, NULL,
                    AM_SEEKING_NoPositioning );
            }
			else
			{
				m_bIsPlaying = false;
			}
            break;
        }

        m_pMediaEventEx->FreeEventParams( lEvent, lParam1, lParam2 );
    }

    if( bNextMode ) Clear();
    
    return bNextMode;
}

HRESULT KShow::OnSize()
{
    if( ! m_bMovie ) return S_OK;   // 동영상이 아니면 그냥 리턴.

    HRESULT hr;

    // 윈도우 사이즈 재조정.
    RECT rect;
    GetClientRect( m_hWnd, &rect );

    hr = m_pVideoWindow->SetWindowPosition( rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top );
    if( FAILED( hr ) ) return hr;

    return S_OK;
}

HRESULT KShow::SetVolume( float fVolume )
{
    /*
    // 입력 범위 : [0.0f , 1.0f]
    if( fVolume < 0.0 || fVolume > 1.0 )
    {
        KLOGL(( 0, "SetVolume(), 볼륨 설정값이 이상함(%f)", fVolume ));
        return E_FAIL;
    }

    // 스트림 재생중이 아닐 때에도 입력된 값은 데시벨로 변환해 기억해둔다.
    // 0.0f ~ 1.0f 사이의 값을 -10000 ~ 0 사이의 값으로 변환해야 한다.
    const double fPower10 = pow( 10, -10 );   // 10 ^ (-10)
    m_fDBVolume = (1.0 - fPower10) * fVolume + fPower10;

    m_fDBVolume = 10 * log( m_fDBVolume );  // decibel (-100 ~ 0 )
    m_fDBVolume *= 100;                     // direct show arrange ( -10000 ~ 0 )

    // 040513. 소수점 연산이 정확하지 못해 값이 범위를 벗어나는 경우가 있다.
    if( m_fDBVolume < -10000.0 )
        m_fDBVolume = -10000.0;
    else if( m_fDBVolume > 0.0 )
        m_fDBVolume = 0.0;
        */
    
    m_fDBVolume = fVolume;
    // 스트림이 재생중일 때에는 볼륨을 설정한다.
    if( ! m_pGraphBuilder ) return S_OK;
    if( ! m_pBacisAudio )   return S_OK;

    HRESULT hr;
    hr = m_pBacisAudio->put_Volume((long)m_fDBVolume);
    if( FAILED( hr ) ) return hr;

    return S_OK;
}

void KShow::SetVideoWinPos( const D3DXVECTOR2& vPos_ , const DWORD& dwWidth_ , const DWORD& dwHeight_ )
{
	m_dwPosX = static_cast<DWORD>(vPos_.x);
	m_dwPosY = static_cast<DWORD>(vPos_.y);
	m_dwWidth = dwWidth_;
	m_dwHeight = dwHeight_;
}

