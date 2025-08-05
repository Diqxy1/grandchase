#include "stdafx.h"
// KDSound.cpp: implementation of the KDSound class.
//
//////////////////////////////////////////////////////////////////////

//
//

#include "KDSound.h"
#include "GCStateInitLoading.h"
//
//

//
//

bool    Is_Sound    = false;
bool    EnableSound = false;
KDSound g_KDSound;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KDSound::KDSound()
{
    m_bInit             = false;
    m_bHardwareAccel    = false;
    m_hWnd              = NULL;
    m_iVolume           = 0;
}

KDSound::~KDSound()
{
    Free();
}

bool KDSound::Init( HWND hwnd, int channel, int freq, int bitRate, bool hardwareAccel )
{
    HRESULT hr;

    m_hWnd              = hwnd;
    m_bHardwareAccel    = hardwareAccel;
    m_pSoundManager     = new CSoundManager();

    if( m_pSoundManager == NULL )
        return false;

    if( FAILED( hr = m_pSoundManager->Initialize( m_hWnd, DSSCL_PRIORITY, channel, freq, bitRate )))
        return false;    

    if( FAILED( hr = m_pSoundManager->Get3DListenerInterface( &m_pDSListener )))
        return false;

    m_dsListenerParams.dwSize = sizeof(DS3DLISTENER);
    m_pDSListener->GetAllParameters( &m_dsListenerParams );

    m_bInit = true;
    return m_bInit;
}

void KDSound::Free()
{
    std::map<std::string, str_SndPack*>::iterator i;
    for( i=m_mapSndPool.begin() ; i != m_mapSndPool.end() ; ++i )
    {
        if(i->second != NULL &&  i->second->pSound != NULL )
        {
            i->second->pSound->Stop();
            i->second->pSound->Reset();
            SAFE_DELETE(i->second->pSound);
        }
    }

    for( i=m_mapLoadScript.begin() ; i != m_mapLoadScript.end() ; ++i )
    {
        SAFE_DELETE( i->second );
    }

    m_mapSndPool.clear();
    m_ListSndLatency.clear();
    m_mapLoadScript.clear();

    SAFE_DELETE( m_pSoundManager );

    m_setPrepareLoad.clear();
    m_setInitLoad.clear();
}

void KDSound::Free( int iIndex )
{
    std::map< std::string, str_SndPack* >::iterator itor = m_mapSndPool.begin();
	std::list<std::string> erase_list;

    while( itor != m_mapSndPool.end() )
    {
        if( itor->second->iIndex == iIndex )
        {
			if(itor->second != NULL &&  itor->second->pSound != NULL )
            {
                itor->second->pSound->Stop();
                itor->second->pSound->Reset();
                SAFE_DELETE( itor->second->pSound );
            }

            erase_list.push_back(itor ->first);
        }
    
        ++itor;
    }

	while(!erase_list.empty())
	{
		m_mapSndPool.erase(erase_list.back());
		erase_list.pop_back();
	}

    m_ListSndLatency.clear();
}

void KDSound::FreeGameSound()
{
    std::for_each( m_setPrepareLoad.begin(), m_setPrepareLoad.end(), 
        boost::bind( &KDSound::Free, boost::ref(g_KDSound), _1 ) );
    m_setPrepareLoad.clear();
}

void KDSound::RegisterLuabind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KDSound>( L, "KDSound" );
    lua_tinker::class_def<KDSound>( L, "Play", 	(LuaSoundPlay)&KDSound::Play );
    lua_tinker::class_def<KDSound>( L, "Stop", 	&KDSound::Stop );
    lua_tinker::class_def<KDSound>( L, "Load", 	&KDSound::LoadFromLua  );
	lua_tinker::class_def<KDSound>( L, "PlayNoOverlapping", 	&KDSound::PlayNoOverlapping  );	

    lua_tinker::decl( L,"g_KDSound", &g_KDSound);
}

bool KDSound::Load( str_SndPack* sndPack )
{
    DWORD           dwCreationFlags = 0;

    if( m_bHardwareAccel == true )
        dwCreationFlags |= DSBCAPS_LOCHARDWARE;

    // Add extra flags needed for the UI
    dwCreationFlags |= DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME;

    {    
        //KGCNewMassFileManager::KGCMemoryFile* pMemFile = g_MyD3D->GetGCDeviceManager()->GetMassFileManager()->LoadMemoryFile( sndPack->fileName );
        GCMemoryFile MemFile (g_pGCDeviceManager2->GetMassFileManager(), sndPack->fileName );
        if( !MemFile.IsLoaded())
        {
            sndPack = NULL;
            return false;
        }

        if( FAILED( m_pSoundManager->CreateFromMemory( &sndPack->pSound, (BYTE*)MemFile.GetDataPointer(), 
            MemFile.GetFileSize(), NULL, dwCreationFlags, GUID_NULL, sndPack->bufCount ) ) )
        {
            sndPack = NULL;
            return false;
        }
    }

    return true;
}

void KDSound::Load( const char* strID_ )
{
    std::string strID( strID_ );
    std::map<std::string, str_SndPack*>::iterator mit;
    mit = m_mapLoadScript.find( strID );

    if( (mit != m_mapLoadScript.end()) && ( m_mapSndPool.find( strID ) == m_mapSndPool.end() ) )
    {
        if(Load( mit->second ))		
            m_mapSndPool.insert( std::make_pair(mit->first, mit->second) ) ;        
    }
}

bool KDSound::LoadFromLuaScript( int version, const char* filename, const char* path )
{
    HRESULT     hr;
    char        finalPath[MAX_PATH]  = {0,};
    int         ver             = 0;


    KLuaManager                         m_kLuaMgr;

    // read script file
    if( GCFUNC::LoadLuaScript( m_kLuaMgr , "Enum.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( m_kLuaMgr , filename ) == false )
        return false;

    if( SUCCEEDED( hr = m_kLuaMgr.BeginTable( "SOUND" ) ) )
    {


        if ( FAILED( hr = m_kLuaMgr.GetValue( "SOUND_SCRIPT_VIRSION", ver ) ) )  
            return false;
        if ( FAILED( hr = m_kLuaMgr.GetValue( "LOAD_LEVEL_COUNT", m_iLoadLevelCount ) ) )
            return false;

        if( FAILED( hr = m_kLuaMgr.EndTable() ) ) 
            return false; // invalid file format
    }

    if( version != ver )
        return false;

    int iTotalBufCount = 0;

	if( SUCCEEDED( hr = m_kLuaMgr.BeginTable( "DATA" ) ) )
	{
		for( int i = 0 ; ; ++i)
		{


			str_SndPack* pSndPack = new str_SndPack();
			memset( pSndPack, 0, sizeof(str_SndPack) );
			pSndPack->latency     = -100.0f; 
			if( FAILED( m_kLuaMgr.BeginTable( i + 1 )))
			{
				SAFE_DELETE( pSndPack );
				break;
			}
			{
				m_kLuaMgr.GetValue( "FILENAME", (char*)pSndPack->fileName, MAX_PATH );

	/*
				if( FAILED(m_kLuaMgr.GetValue( "ID", (char*)pSndPack->strID, 20 ) ) )
					sprintf(pSndPack->strID ,"%d",i);*/
				if( FAILED(m_kLuaMgr.GetValue( "ID", (char*)pSndPack->strID, MAX_PATH ) ) )
				{
					ASSERT(!"There is sound table has not id code in 'sound.stg'" );
				}


				if( FAILED(m_kLuaMgr.GetValue( "BUFFER_COUNT", pSndPack->bufCount ) ) )
					pSndPack->bufCount = 10;

				bool bInitLoading = false;
				if( FAILED(m_kLuaMgr.GetValue( "INITLOADING", bInitLoading ) ) )
					bInitLoading = false;

				if( SUCCEEDED( m_kLuaMgr.BeginTable( "USE_STATE" ) ) )
				{
					for( int j = 1; ; ++j )
					{
						int iPosition = 0;
						if( FAILED( m_kLuaMgr.GetValue( j, iPosition ) ))
						{
							break;
						}
						m_mapLoadPosition[iPosition].push_back( pSndPack->strID );
					}
					m_kLuaMgr.EndTable();
				}

				//모드는 게임에서 사용되는 사운드의 경우에만 참조합니다.
				if( SUCCEEDED( m_kLuaMgr.BeginTable( "USE_MODE" ) ) )
				{
					int iMode;
					for( int j = 1 ; ; ++j )
					{
						if( FAILED( m_kLuaMgr.GetValue( j, iMode)))
						{
							break;
						}
						m_mapLoadMode[iMode].push_back( pSndPack->strID );
					}

					m_kLuaMgr.EndTable();
				}


				if( bInitLoading == true )
					PreparetoInitLoad( i );

				pSndPack->iIndex = i;
			}
			m_kLuaMgr.EndTable();
			iTotalBufCount += pSndPack->bufCount;

			UINT iTexLen = (UINT)strlen(pSndPack->fileName);
			for( UINT t = 0 ; t < iTexLen ; ++t )
			{
				if( pSndPack->fileName[t] == '/' )
				{
					pSndPack->fileName[t] = '\\'; //'\'
				}
			}

#if !defined( __PATH__ ) 
            if ( m_mapLoadScript.end() != m_mapLoadScript.find( pSndPack->strID ) ) 
            {
                MessageBoxA( NULL, "SoundID Duplication Error", pSndPack->strID, MB_OK );
            }    
#endif
			
            m_mapLoadScript.insert( std::make_pair(pSndPack->strID , pSndPack ) );

			ZeroMemory( finalPath,  sizeof(char)*MAX_PATH );
		}
	}

    std::for_each( m_setInitLoad.begin(), m_setInitLoad.end(),
        boost::bind( &KDSound::LoadFromIndex, boost::ref(g_KDSound), _1 ) );

    return true;
}

void KDSound::FrameMove( float elapsedTime )
{
    PROFILE_SET("KDSound::FrameMove");

    //std::map<std::string, str_SndPack*>::iterator i;
	std::list< std::pair< str_SndPack* , float > >::iterator i;
	///std::list< std::pair< str_SndPack* > erase_list;

    i = m_ListSndLatency.begin();
    while( i != m_ListSndLatency.end() )
    {
		if( i->second <= 0 )
        {
            CSound* p = i->first->pSound;
            if( p == NULL )
                return;

            p->GetBuffer( p->GetFreeBufferNum() )->SetVolume( m_iVolume );
            p->Play( 0, i->first->loop );

			i = m_ListSndLatency.erase( i );
			continue;
            
			//erase_list.push_back(i->first);
        }

        i->second -= elapsedTime;
        ++i;     
    }


	//while(!erase_list.empty())
	//{
	//	m_ListSndLatency.erase();
	//	erase_list.pop_back();
	//}
}

void KDSound::Play( const char* ID, int loop, int latency, int iParm )
{
    PROFILE_SET( "KDSound::Play" );

    if( -1 < iParm )
    {
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA && 
            g_MyD3D->m_pStateMachine->GetState() == GS_GAME &&    
            g_MyD3D->m_TempPlayer.GetDotaStage() != iParm )
            return;
    }

    if(strcmp(ID,"-1")==0)
        return;
    if( m_bInit == false || EnableSound == false || Is_Sound == false )
        return;

    std::map<std::string, str_SndPack*>::iterator itor;
    itor = m_mapSndPool.find(ID);

    if( itor == m_mapSndPool.end() )
    {
        if( ( itor = m_mapLoadScript.find( ID ) ) != m_mapLoadScript.end() )
        {				
            if(Load( itor->second ))
                m_mapSndPool.insert( std::make_pair( itor->first, itor->second ));		
        }

        itor = m_mapSndPool.find(ID);

        if( itor == m_mapSndPool.end() )
        {
            //로드가 안된 음원을 재생 하려고 한다.
            START_LOG( cerr, "NotLoaded Sound : "<< ID );
#if !defined( __PATH__ )
            std::string strSoundMsg( ID );
            strSoundMsg.append( " is Not Loaded!!!" );
            MessageBoxA( NULL, strSoundMsg.c_str(), NULL, NULL );

            {
                CreateDirectoryA( "./FileErrorLog", NULL );

                time_t curTime = GCUTIL_TIME::GetCurTime();                    

                // 한데 모은 로그파일
                std::wstringstream strText;
                strText << GCUTIL_TIME::GetYear(curTime) << "_" << GCUTIL_TIME::GetMonth(curTime) << "_" << GCUTIL_TIME::GetDay(curTime) << " | "
                    << GCUTIL_TIME::GetHour(curTime) << ":" << GCUTIL_TIME::GetMinute(curTime) << ":" << GCUTIL_TIME::GetSecond(curTime) << " | "
                    << L"사운드 파일 에러 | " << L"Sound ID : " << ID << " 파일이 존재하지 않음" << "\n";

                FILE *fo = NULL;
                fo = fopen("FileErrorLog\\FileErrorLogList.txt","a");
                fwprintf(fo,L"%s",strText.str().c_str());
                fclose(fo);
            }

            ASSERT( !"Sound Not Loaded!!!" );
#endif
            return;
        }
    }

    if( latency <= 0 )
    {
        if( itor != m_mapSndPool.end() )
        {
            DWORD dwBuffer = itor->second->pSound->GetFreeBufferNum();

            LPDIRECTSOUNDBUFFER pBuffer = itor->second->pSound->GetBuffer( dwBuffer );

            if( !pBuffer )
                return;

            pBuffer->SetVolume( m_iVolume );
            itor->second->pSound->Play( 0, loop );
            itor->second->loop = loop;
        }
    }
    else
    {
        if( itor != m_mapSndPool.end() )
        {
			//m_mapSndLatency[ID] = itor->second;
            //itor->second->latency = (float)latency;
			m_ListSndLatency.push_back( std::make_pair( itor->second, (float)(latency) ) );
            itor->second->loop = loop;
        }
    }
}

void KDSound::Play( DWORD dwID, int loop, int latency, int iParm )
{
    std::stringstream stream;
    stream << dwID;
    Play( stream.str().c_str(), loop, latency, iParm );
}


//------------------------------------------------------------------------
//	NAME : PlayNoOverlapping()
//	DESC : 진 사운드 중복을 막기위해 추가한 함수
//			... 진 말고 다른 애들도 필요하면 사용하면 됨. -_-
//	DATE : 20091104 이동석
//------------------------------------------------------------------------
void KDSound::PlayNoOverlapping( const char* ID, int loop, int latency, int iParm )
{
	if(strcmp(ID,"-1")==0)
		return;
	if( m_bInit == false || EnableSound == false || Is_Sound == false )
		return;

	std::map<std::string, str_SndPack*>::iterator itor;
	itor = m_mapSndPool.find(ID);

	if( itor != m_mapSndPool.end() )
	{
		if( itor->second->pSound->IsSoundPlaying() )
			return;
	}

	Play( ID, loop, latency, iParm );
}





void KDSound::Stop( const char* ID )
{
    //std::map<std::string, str_SndPack*>::iterator itor;
    //itor = m_mapSndPool.find(ID);

    //if( itor == m_mapSndPool.end() )
    //    return;

    //if( m_bInit == false || itor->second->pSound == NULL )
    //    return;

    //if( m_mapSndLatency.find( ID ) != m_mapSndLatency.end() )
    //    m_mapSndLatency.erase( ID );

    //itor->second->pSound->Stop();
    //itor->second->pSound->Reset();
	std::map<std::string, str_SndPack*>::iterator itor;
    itor = m_mapSndPool.find(ID);

    if( itor == m_mapSndPool.end() )
        return;

    if( m_bInit == false || itor->second->pSound == NULL )
        return;
	std::list< std::pair< str_SndPack* , float > >::iterator lit;
	lit = std::find_if( m_ListSndLatency.begin(), m_ListSndLatency.end(),
		boost::bind( &std::pair< str_SndPack* , float >::first, _1) == itor->second );

	if( lit != m_ListSndLatency.end() )// return;
		m_ListSndLatency.erase( lit );

    //if( m_ListSndLatency.find( ID ) != m_ListSndLatency.end() )
    //    m_ListSndLatency.erase( ID );

    itor->second->pSound->Stop();
    itor->second->pSound->Reset();
}

bool KDSound::LoadFromIndex( int iIndex )
{
    if ( m_bInit == false )
        return false;

    std::stringstream stream;
    stream << iIndex;

    std::map<std::string, str_SndPack*>::iterator iter;

    iter = m_mapLoadScript.find( stream.str().c_str() );

    if( iter != m_mapLoadScript.end() )
    {
        std::map<std::string, str_SndPack*>::iterator mit;
        mit = m_mapSndPool.find( iter->second->strID );
        if( mit == m_mapSndPool.end() || mit->second->pSound == NULL )
        {
            if(Load( iter->second ))
            {
                m_mapSndPool.insert( std::make_pair(iter->second->strID, iter->second) );
            }
            return true;
        }
    }

    return false;
}

bool KDSound::Reload( std::string strFileName_ )
{
    GCUTIL_STR::ToUpper( strFileName_ );

    std::map<std::string, str_SndPack*>::iterator mit;
    for( mit = m_mapLoadScript.begin() ; mit != m_mapLoadScript.end() ; ++mit )
    {
        std::string strTemp( mit->second->fileName );
        GCUTIL_STR::ToUpper( strTemp );
        if( strFileName_.compare( strTemp ) == 0)
        {
            Load( mit->second );
        }
    }
    return true;
}

bool KDSound::Reload( DWORD dwID )
{
    std::stringstream stream;
    stream << dwID;

    std::map<std::string, str_SndPack*>::iterator mit;
    mit = m_mapLoadScript.find( stream.str() );

    if( mit == m_mapLoadScript.end() )
    {
        return false;
    }

    Reload( mit->second->fileName );

    return true;
}

// [8/5/2007 breadceo] Load해야할 리스트에 추가함.
void KDSound::PreparetoLoad( int iIndex )
{
    m_setPrepareLoad.insert( iIndex );
}

// [8/5/2007 breadceo] 게임 초기화때 로드해야 되는 리스트에 추가함.
void KDSound::PreparetoInitLoad( int iIndex )
{
    std::vector< int >::iterator vit;
    m_setInitLoad.insert( iIndex );
}

// [8/5/2007 breadceo] LoadList에 있는 것들 Load
void KDSound::FullLoadfromLoadList()
{
    std::for_each( m_setPrepareLoad.begin(), m_setPrepareLoad.end(), 
        boost::bind( &KDSound::LoadFromIndex, boost::ref(g_KDSound), _1 ) );
}

void KDSound::PlayWithLoad( const char* ID, int loop /*= 0*/, int latency /*= 0 */ )
{
    if( m_bInit == false || EnableSound == false || Is_Sound == false )
        return;

    std::map<std::string, str_SndPack*>::iterator itor;
    itor = m_mapSndPool.find(ID);

    if( itor == m_mapSndPool.end() )
    {
        if( ( itor = m_mapLoadScript.find( ID ) ) != m_mapLoadScript.end() )
        {
            //로드가 안된 음원을 재생 하려고 한다.
            if(Load( itor->second ))
                m_mapSndPool.insert( std::make_pair( itor->first, itor->second ));
        }
    }

    Play( ID, loop, latency );
}

void KDSound::PlayWithLoad( DWORD dwID, int loop /*= 0 */, int latency /*= 0 */ )
{
    std::stringstream stream;
    stream << dwID;
    PlayWithLoad( stream.str().c_str(), loop, latency );
}

void KDSound::LoadFromPositionList( int iPosition , int iGameMode )
{
    if( m_mapLoadPosition.find( LP_ALL_STATE ) != m_mapLoadPosition.end() )
        LoadFromList( m_mapLoadPosition[LP_ALL_STATE] );

    if( iPosition == GS_GAME || iPosition == GS_GAME_LOADING )
    {
        if( m_mapLoadPosition.find( iPosition ) != m_mapLoadPosition.end() )
            LoadFromList( m_mapLoadPosition[iPosition] );

        if( m_mapLoadPosition.find( GS_GAME ) != m_mapLoadPosition.end() )
        {
            if( m_mapLoadMode.find( iGameMode ) != m_mapLoadMode.end() )
            {
                LoadFromList( m_mapLoadMode[iGameMode] );
            }
            else
            {
                LoadFromList( m_mapLoadPosition[GS_GAME] );
            }
        }
    }
    else
    {
        if( m_mapLoadPosition.find( LP_WITHOUT_GAME ) != m_mapLoadPosition.end() )
            LoadFromList( m_mapLoadPosition[LP_WITHOUT_GAME] );

        if( m_mapLoadPosition.find( iPosition ) != m_mapLoadPosition.end() )
            LoadFromList( m_mapLoadPosition[iPosition] );
    }

}

void KDSound::LoadFromList( std::vector< std::string>& vecSounds_ )
{
    int iSize = (int)vecSounds_.size();

    for( int i = 0 ; i < iSize ; ++i )
    {
        Load( vecSounds_[i].c_str() );
    }
}

void KDSound::LoadFromLua( const char* strID_ )
{
	Load( strID_ );
}
