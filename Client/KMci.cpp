#include "stdafx.h"
// KMci.cpp: implementation of the KMci class.
//
//////////////////////////////////////////////////////////////////////

//
#include "KMci.h"
//
//

//
//
//
#include "GCUI/GCOptionMusic.h"
//

KMci g_KMci;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KMci::KMci()
{
	m_bPlayMusic = true;
	m_bToggleMusic = true;
	m_cNowTrack = -1;
	m_bRepeat = true;
}

KMci::~KMci()
{
	Clear();
}

HRESULT KMci::Init(HINSTANCE hInst, HWND hWnd, const char* szFilename, const char* szPath)
{
#ifdef DSHOW
	m_kShow.InitShow( hWnd, szFilename, szPath , "mediaData");
	m_kShowForMovie.InitShow( hWnd, szFilename, szPath , "CharIntroMovieData");
#else
	m_kPlayer.Init(hInst);

    if( GCFUNC::LoadLuaScript( m_kLuaMgr , szFilename ) == false )
		return;

	m_kLuaMgr.BeginTable(_T("mediaData"));

	int nNumFile;
    TCHAR szTableName[20] = {0};
    std::string strPath( szPath );

    m_kLuaMgr.GetValue( _T("num"), nNumFile );

	m_vFilePath.reserve( nNumFile );

    for( int i = 0; i < nNumFile; i++ )
    {
        std::string strFilePath;
        std::string strFileName;

        _stprintf( szTableName, "media%d", i );
        m_kLuaMgr.BeginTable( szTableName );
        m_kLuaMgr.GetValue( _T("file"), strFileName );
        m_kLuaMgr.EndTable();

        strFilePath = strPath + strFileName;

        m_vFilePath.push_back( strFilePath );
    }
    
    assert( nNumFile == m_vFilePath.size() );

    m_kLuaMgr.EndTable();
#endif

    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KMci>( L, "KMciClass" );
		lua_tinker::class_def<KMci>( L, "Play", 			&KMci::LuaPlay  );
		lua_tinker::class_def<KMci>( L, "Stop", 			&KMci::Stop  );;

	lua_tinker::decl( L, "MusicPlayer", this );

	return S_OK;
}

HRESULT KMci::Play(int nIndex, bool bRepeat, bool bForcePlay )
{
    if( g_pkGameOverlayUI->IsRenderCartoonDrama() && !bForcePlay ) {
        return S_FALSE;
    }

#if defined(NATION_KOREA)
    if ( nIndex == GC_BGM_MAIN_THEME && ::rand() % 2 == 0 )
    {
        nIndex = GC_BGM_MAIN_THEME_LYRICS;
    }
#elif defined( NATION_USA )
	if( nIndex == GC_BGM_MAIN_THEME )
	{
		nIndex = GC_BGM_MAIN_THEME_LYRICS;
	}
#endif // NATION_KOREA

	m_cNowTrack = nIndex;
	m_bRepeat = bRepeat;

	if( !m_bPlayMusic || !m_bToggleMusic )
		return S_OK;

#ifdef DSHOW
	if( m_kShow.Play( nIndex, bRepeat ) == E_FAIL )
		m_bPlayMusic = false;
	else
		m_bPlayMusic = true;
#else
	//nIndex에 해당하는 파일을 m_vFilePath로 부터 얻어와 Play시킨다
	std::vector<std::string>::iterator itor;
	itor = m_vFilePath.begin();

	char szFile[200];

	strcpy( szFile, itor[nIndex].c_str() );

	m_kPlayer.SetRepeat( bRepeat );
	m_bPlayMusic = m_kPlayer.OpenFile( szFile );

	if( m_bPlayMusic )
		m_kPlayer.Play();
#endif

	g_pOptionMusic->SetEnable( m_bPlayMusic, g_pOptionMusic->GetVolume() );

	return S_OK;
}

HRESULT KMci::Stop()
{
	if( !m_bPlayMusic )
		return S_OK;

	if(m_cNowTrack > -1)
	{
#ifdef DSHOW
		m_kShow.Stop( m_cNowTrack );
#else
		m_kPlayer.Stop();
#endif
	}

	return S_OK;
}

HRESULT KMci::Continue()
{
    if( m_bPlayMusic && m_bToggleMusic ) 
	{
		if( m_cNowTrack > -1 )
			Play( m_cNowTrack, m_bRepeat );
	}

	return S_OK;
}

HRESULT KMci::SetVolume(float fVolume)
{
	if( m_bPlayMusic )
	{
#ifdef DSHOW
		m_kShow.SetVolume( fVolume );
#else
		m_kPlayer.Set_volume( (int)fVolume );
#endif
	}

	return S_OK;
}

HRESULT KMci::Clear()
{
#ifdef DSHOW
	m_kShow.Stop( m_cNowTrack );
	m_kShow.Clear();
#else
	m_vFilePath.clear();
	m_kPlayer.Stop();
	m_kPlayer.Close();
#endif

	return S_OK;
}
void KMci::AddMusic(KShow::FileInfo fileinfo)
{
    for(int i=0;i<(int)GetShow().m_vFileInfo.size();i++)
    {
        if( !stricmp(GetShow().m_vFileInfo[i].m_strFile, fileinfo.m_strFile ) )
            return;
    }

    m_kShow.m_vFileInfo.push_back( fileinfo );
}

void KMci::Play(const char *lpszFIleName)
{
    for(int i=0;i<(int)GetShow().m_vFileInfo.size();i++)
    {
        if( !stricmp(GetShow().m_vFileInfo[i].m_strFile, lpszFIleName) )
        {
            Play(i, 1);
            break;
        }
    }
}

void KMci::LuaPlay( int nIndex, bool bRepeat, bool bForcePlay )
{
	Play( nIndex, bRepeat, bForcePlay );
}
