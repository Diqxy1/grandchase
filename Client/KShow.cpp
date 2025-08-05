
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
    // �� Ŭ������ ����ϱ� ���� CoCreateInstance�� ȣ��Ǿ� �־�� �Ѵ�.
    m_pGraphBuilder = NULL;
    m_bMovie        = false;
    m_fDBVolume     = 0.0;      // �ְ��̴�.
    
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
  
    // ����� �̵�� Ÿ��(audio only, video..)�� ���� �̸��� ���´�.
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
    // ������ ������̴� ������ ó���Ѵ�.
    if( m_pGraphBuilder ) Clear();

    // �׷��� ���� �����͸� ����.
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
    // ���� ������. (direct show�� ���������� WCHAR�� ����Ѵ�.)
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


    // �̵�� ����� ���̴� �������̽����� ���´�. 
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
    
    // ��� ������ ����.
    hr = m_pMediaEventEx->SetNotifyWindow( (OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0);
    if( FAILED( hr ) ) return hr;
    //  ^ ����° �μ� 0�� �޽����� ���� lParam�� ��. ������� �����Ƿ� �׻� 0.

    if( m_bMovie ) // �������� ��츦 ���� ��������� ����.
    {
        m_pVideoWindow->put_Owner( (OAHWND)m_hWnd );
		m_pVideoWindow->SetWindowPosition(m_dwPosX,m_dwPosY,m_dwWidth,m_dwHeight);
        m_pVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
        m_pVideoWindow->SetWindowForeground( OATRUE );
        //OnSize();
		
    }

    // ������� ��� ����.
    hr = m_pMediaControl->Run();
    if( FAILED( hr ) ) return hr;

    if( !m_bMovie && m_pBacisAudio )    // �������� �ƴҶ� 
    {
        hr = m_pBacisAudio->put_Volume((long)m_fDBVolume);
        if( FAILED( hr ) ) return hr;
        // ^ (�������̾ ������ ������ ��Ʈ�� ������ ����� ��Ʈ���� ����.)
    }
    
	m_bIsPlaying = true;
    return S_OK;
}

HRESULT KShow::Stop( int nIndex )
{
    // ������ ������̴� ������ ó���Ѵ�.
    //if( m_pGraphBuilder ) Clear();

    /*

    // �׷��� ���� �����͸� ����. 
    JIF( CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
        (void**)&m_pGraphBuilder) );

    WCHAR wFileName[MAX_PATH];                  // media file (WCHAR)

    //////////////////////////////////////////////////////////////////////////
    // ���� ������. (direct show�� ���������� WCHAR�� ����Ѵ�.)
    MultiByteToWideChar(CP_ACP, 0, m_vFileInfo[nIndex].m_strFile, -1, wFileName, MAX_PATH);
    m_bMovie = m_vFileInfo[nIndex].m_bwindow;

    LIF( m_pGraphBuilder->RenderFile( wFileName, NULL ) ); // second must be NULL!

//    if( FAILED( hr ) )
    //{
        //KLOGL(( 0, "%s������ ����� �� �����ϴ�", m_vFileInfo[nIndex].m_strFile ));
        //SAFE_RELEASE( m_pGraphBuilder );
        //return E_FAIL;
    //}

    // �̵�� ����� ���̴� �������̽����� ���´�. 
    m_pGraphBuilder->QueryInterface( IID_IMediaEventEx, (void**)&m_pMediaEventEx );
    m_pGraphBuilder->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );
    m_pGraphBuilder->QueryInterface( IID_IMediaSeeking, (void**)&m_pMediaSeeking );
    m_pGraphBuilder->QueryInterface( IID_IVideoWindow, (void**)&m_pVideoWindow );
    m_pGraphBuilder->QueryInterface( IID_IBasicAudio, (void**)&m_pBacisAudio );
    
    // ��� ������ ����.
    JIF( m_pMediaEventEx->SetNotifyWindow( (OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0) );
    //  ^ ����° �μ� 0�� �޽����� ���� lParam�� ��. ������� �����Ƿ� �׻� 0.

    if( m_bMovie ) // �������� ��츦 ���� ��������� ����.
    {
        LIF( m_pVideoWindow->put_Owner( (OAHWND)m_hWnd ) );
        LIF( m_pVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ));
        LIF( m_pVideoWindow->SetWindowForeground( OATRUE ) );
        OnSize();
    }

  */
    // ������� ��� ����.
    HRESULT hr;
	if( m_pMediaControl != NULL )
		hr = m_pMediaControl->Stop();

    /*
    if( !m_bMovie && m_pBacisAudio )    // �������� �ƴҶ� 
    {
        JIF( m_pBacisAudio->put_Volume((long)m_fDBVolume) );
        // ^ (�������̾ ������ ������ ��Ʈ�� ������ ����� ��Ʈ���� ����.)
    }
    */
    
    return S_OK;
}

HRESULT KShow::Continue( int nIndex )
{
    // �׷��� ���� �����͸� ����. 
    HRESULT hr;
    hr = CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
        (void**)&m_pGraphBuilder);
    if( FAILED( hr ) ) return hr;
	
    if ( !m_pGraphBuilder ) 
        return E_FAIL;	

    WCHAR wFileName[MAX_PATH];                  // media file (WCHAR)

    //////////////////////////////////////////////////////////////////////////
    // ���� ������. (direct show�� ���������� WCHAR�� ����Ѵ�.)
    MultiByteToWideChar(CP_ACP, 0, m_vFileInfo[nIndex].m_strFile, -1, wFileName, MAX_PATH);
    m_bMovie = m_vFileInfo[nIndex].m_bwindow;

	hr = ProgressConnectionMP3(m_pGraphBuilder);
    if( FAILED( hr ) ) return hr;

    m_pGraphBuilder->RenderFile( wFileName, NULL ); // second must be NULL!

    // �̵�� ����� ���̴� �������̽����� ���´�. 
    m_pGraphBuilder->QueryInterface( IID_IMediaEventEx, (void**)&m_pMediaEventEx );
    m_pGraphBuilder->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );
    m_pGraphBuilder->QueryInterface( IID_IMediaSeeking, (void**)&m_pMediaSeeking );
    m_pGraphBuilder->QueryInterface( IID_IVideoWindow, (void**)&m_pVideoWindow );
    m_pGraphBuilder->QueryInterface( IID_IBasicAudio, (void**)&m_pBacisAudio );
    
    // ��� ������ ����.
    hr = m_pMediaEventEx->SetNotifyWindow( (OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0);
    if( FAILED( hr ) ) return hr;
    //  ^ ����° �μ� 0�� �޽����� ���� lParam�� ��. ������� �����Ƿ� �׻� 0.

    if( m_bMovie ) // �������� ��츦 ���� ��������� ����.
    {
        m_pVideoWindow->put_Owner( (OAHWND)m_hWnd );
        m_pVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
        m_pVideoWindow->SetWindowForeground( OATRUE );
        //OnSize();
    }

    // ������� ��� ����.
    hr = m_pMediaControl->Run();
    if( FAILED( hr ) ) return hr;

    if( !m_bMovie && m_pBacisAudio )    // �������� �ƴҶ� 
    {
        hr = m_pBacisAudio->put_Volume((long)m_fDBVolume);
        if( FAILED( hr ) ) return hr;
        // ^ (�������̾ ������ ������ ��Ʈ�� ������ ����� ��Ʈ���� ����.)
    }
    
    return S_OK;
}

HRESULT KShow::Clear()
{
    
    HRESULT hr;

    do
    {
        // ���� �׷����� �������� ���� ���� ����. 
        if( !m_pGraphBuilder ) break;
        if( !m_pMediaControl ) break;


        // ���� ���� Puase
        hr = m_pMediaControl->Pause();
        if( FAILED( hr ) ) break;

        // ��� ����.
        hr = m_pMediaControl->Stop();
        if( FAILED( hr ) ) break;

        // �޽��� ���� ������ ����.
        if( m_pMediaEventEx != NULL )
        {
            hr = m_pMediaEventEx->SetNotifyWindow( NULL, WM_GRAPHNOTIFY, 0);
            if( FAILED( hr ) ) break;
        }

        if( m_bMovie ) // �������� ��� ��������� ����.
        {
            m_pVideoWindow->put_Visible( OAFALSE );
            //        LIF( m_pVideoWindow->put_Owner( NULL ) );
            /* ������, ������ ��Ʈ���� ������ �� ���� ������� ���踦 �����־�� ����������,
            ��üȭ�� ���� ������ ���� �̸� �����Ű�� ��Ŀ���� �Ұ� �ּ�ȭ �Ǳ� ������ 
            �ּ�ó���Ѵ�. ���踦 �����ִ� ������ ������ â�� ��� ����ְ�, ���� ������� 
            �ı��Ǿ����� ������ ������ �޽����� �������� �õ��ϴ� ��츦 �����ϱ� �����ε�,
            ���� ���α׷��ֿ����� �׷� ��찡 ���ٰ� �ǴܵǹǷ� ȣ������ �ʴ´�.
            ��üȭ�� ���� ��尡 �ƴ� ������ ȣ�����־ ū ����� ������, 
            ��üȭ�� ���۽ÿ� ������ �Ǿ� �ּ�ó���صд�. */
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
    BOOL bNextMode = FALSE; // Intro -> Lobby�� �Ѿ�� �� �������� �˷��ش�. 

    while(SUCCEEDED( m_pMediaEventEx->GetEvent( &lEvent, &lParam1, &lParam2, lTimeOut ) ) )
    {
        switch( lEvent )
        {
        case EC_COMPLETE : // ��Ʈ���� ����� �� ������ ���. 

            if( m_bMovie )  // �������̾��ٸ�, �̸� �����Ű�� ���Ƿ� �Ѿ��.
                bNextMode = TRUE;
            else if( m_Repeat == true )    // ������̾��ٸ�, ó������ �ٽ� ����Ѵ�.
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
    if( ! m_bMovie ) return S_OK;   // �������� �ƴϸ� �׳� ����.

    HRESULT hr;

    // ������ ������ ������.
    RECT rect;
    GetClientRect( m_hWnd, &rect );

    hr = m_pVideoWindow->SetWindowPosition( rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top );
    if( FAILED( hr ) ) return hr;

    return S_OK;
}

HRESULT KShow::SetVolume( float fVolume )
{
    /*
    // �Է� ���� : [0.0f , 1.0f]
    if( fVolume < 0.0 || fVolume > 1.0 )
    {
        KLOGL(( 0, "SetVolume(), ���� �������� �̻���(%f)", fVolume ));
        return E_FAIL;
    }

    // ��Ʈ�� ������� �ƴ� ������ �Էµ� ���� ���ú��� ��ȯ�� ����صд�.
    // 0.0f ~ 1.0f ������ ���� -10000 ~ 0 ������ ������ ��ȯ�ؾ� �Ѵ�.
    const double fPower10 = pow( 10, -10 );   // 10 ^ (-10)
    m_fDBVolume = (1.0 - fPower10) * fVolume + fPower10;

    m_fDBVolume = 10 * log( m_fDBVolume );  // decibel (-100 ~ 0 )
    m_fDBVolume *= 100;                     // direct show arrange ( -10000 ~ 0 )

    // 040513. �Ҽ��� ������ ��Ȯ���� ���� ���� ������ ����� ��찡 �ִ�.
    if( m_fDBVolume < -10000.0 )
        m_fDBVolume = -10000.0;
    else if( m_fDBVolume > 0.0 )
        m_fDBVolume = 0.0;
        */
    
    m_fDBVolume = fVolume;
    // ��Ʈ���� ������� ������ ������ �����Ѵ�.
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

