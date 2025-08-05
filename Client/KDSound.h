//////////////////////////////////////////////////////////////////////////
/// @file   KDSound.h
///         interface for the KDSound class of 'GrandChase' project.
/// @author shice@kogstudios.com
/// @since  25.08.2004
///
/// @date   2004-08-25
///         - Init()
///         - Free()
///         - Load()
///         - LoadFromScript()
///         - Play()
///         - Stop()
///         - SetVol()
///         - SetVolRef()
///         - Set3DParameters()
///

//#if !defined(AFX_KDSOUND_H__FEC946CE_8978_4CB2_87C9_1FFB883DA3D7__INCLUDED_)
//#define AFX_KDSOUND_H__FEC946CE_8978_4CB2_87C9_1FFB883DA3D7__INCLUDED_

#ifndef _KDSOUND_H_
#define _KDSOUND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable: 4786 )

////#include <vector>
////#include <map>
////#include <string>
////#include <set>
#include "DSUtil.h"
//#include "KLuabinder.h"

#pragma comment( lib, "dxguid" )
#pragma comment( lib, "dsound" )
#if defined( DIRECTX_VERSIONUP_2009_AUG )
    #pragma comment( lib, "dxerr" )
#else
    #pragma comment( lib, "dxerr8" )
#endif
#pragma comment( lib, "winmm" )

/// @class KDSound Direct Sound를 이용해 음악을 로딩, 연주하는 클래스.
class KDSound// : public lua_tinker::lua_value
{
    public:
		//KDeclareLuaInvoke
        /// @struct KDSound::str_SndPack 로딩된 음원을 보관하기 위한 패키지.
        struct str_SndPack
        {
			str_SndPack()
			{
				pSound=NULL;
			}
			char					fileName[MAX_PATH];
            char                    strID[MAX_PATH];
            CSound                  *pSound;
            int                     bufCount;
			float					latency;
            int                     loop;
            int                     iIndex;
        };

        enum LOAD_POSITION
        {
            LP_CUSTOM   = -1,
            LP_ONLY_GAME = -2,
            LP_WITHOUT_GAME = -3,
            LP_ALL_STATE = -4,
        };

	public:
		KDSound();
		virtual ~KDSound();

        /// 클래스 초기화.
        /// @param  hwnd:           [in] 어플리케이션의 윈도우 핸들
        /// @param  channel:        [in] 출력 채널
        /// @param  freq:           [in] 출력 주파수
        /// @param  bitRate:        [in] 출력 비트
        /// @param  hardwareAccel:  [in] 사운드 하드웨어 가속 여부
        /// @return 초기화 성공이면 true 실패면 false
        /// @see    Free()
        bool Init( HWND hwnd, int channel = 2, int freq = 22050, int bitRate = 16, bool hardwareAccel = false );
        /// 클래스 종료, 로딩된 음원 데이터 파기.
        /// @see    Init()
        void Free();
		void Free( int iIndex );
		static void RegisterLuabind();


        /// 음원을 하나 로드한다. LoadFromScript() 에서 사용.
        /// @param  ID:         [in] 음원에 접근할 스트링 ID
        /// @param  filename:   [in] 음원 파일 이름
        /// @param  bufCount:   [in] 동시연주 가능 수
        /// @param  use3D:      [in] 3D 사운드 연주용으로 로드할지 여부
        /// @return 로딩 성공이면 true 실패면 false
        /// @see    LoadFromScript()
		bool Load( str_SndPack* sndPack );
		
        /// 스크립트를 읽어 Load()를 사용하여 다수의 음원들을 로드한다.
        /// @param  version:    [in] 읽어들일 스크립트 버전. 스크립트 파일의 버전과 일치할 때만 로드한다
        /// @param  filename:   [in] 스크립트 파일 이름
        /// @param  path:       [in] 음원 파일에 공통된 경로를 잡아야 할 경우 입력
        /// @see    Load()

        bool LoadFromLuaScript( int version, const char* filename, const char* path );


        void FrameMove( float elapsedTime = 1 );

        /// 음원을 플레이한다.
        /// @param  ID:     [in] 플레이할 음원의 ID
        /// @param  loop:   [in] 플레이 방식 0이면 onetime 1 이면 반복재생
        /// @param  latency:[in] 레이턴시
        /// @see    Stop()
        typedef void (KDSound::*LuaSoundPlay)( const char* ID, int loop , int latency, int iParm );

        void Play( const char* ID, int loop = 0, int latency = 0, int iParm = -1 );
        void Play( DWORD dwID, int loop, int latency = 0, int iParm = -1 );
        void PlayWithLoad( const char* ID, int loop = 0, int latency = 0 );
        void PlayWithLoad( DWORD dwID, int loop = 0 , int latency = 0 );

		//	재생중인 사운드가 오버랩핑 되지 않게 하는 함수 : 20091104 이동석
		void PlayNoOverlapping( const char* ID, int loop, int latency, int iParm = -1 );

        /// 재생을 중지한다. 이 경우 중복 재생되고 있는 다른 음원들이 모두 정지한다.
        /// @param  ID:     [in] 정지할 음원의 ID
        /// @see    Play()
        void Stop( const char* ID );



        /// 볼륨을 설정한다. 범위는 0 ~ -10000 이며 설정된 볼륨은 플레이되는 모든 음원에 영향을 미친다
        /// @param  vol:    [in] 볼륨
        /// @see    SetVolRef()
        void SetVol( int vol ){ m_iVolume = vol; }
        /// 설정된 볼륨을 기본으로 변화량을 설정한다.
        /// @param  volref:    [in] 변화될 볼륨 양
        /// @see    SetVol()
        void SetVolRef( int volref ){ m_iVolume += volref; }
        bool LoadFromIndex( int iIndex );

        int GetLoadLevelCount() const { return m_iLoadLevelCount; }

        void PreparetoLoad( int iIndex );
        void PreparetoInitLoad( int iIndex );
        void FullLoadfromLoadList();

        bool Reload( std::string strFileName_ );
        bool Reload( DWORD  dwID );

        void FreeGameSound();
        void LoadFromPositionList( int iPosition , int iGameMode = -1 );
        void LoadFromList( std::vector< std::string>& vecSounds_ );
        void Load( const char* strID_ );
		void LoadFromLua( const char* strID_ );

	private:
        bool								m_bInit;            ///< 현재 클래스가 초기화되어 있는가
        HWND								m_hWnd;             ///< 연주될 윈도우 핸들러
        bool                                m_bHardwareAccel;   ///< 사운드 로딩, 연주에 있어서 하드웨어 가속 사용여부
		int									m_iVolume;          ///< 설정된 볼륨( 0 ~ -10000 )

        std::map<std::string, str_SndPack*>	m_mapSndPool;       ///< 로딩된 음원을 보관
		//std::map<std::string, str_SndPack*> m_mapSndLatency;	///< latency만큼 지연시킬 음원
		std::list< std::pair< str_SndPack* , float > > m_ListSndLatency;
        std::map<std::string, str_SndPack*> m_mapLoadScript;    ///< Script에서 Load된 데이터 보관.
        CSoundManager*						m_pSoundManager;    
        LPDIRECTSOUND3DLISTENER				m_pDSListener;      ///< 3D listener object
        DS3DBUFFER							m_dsBufferParams;   ///< 3D buffer properties
        DS3DLISTENER						m_dsListenerParams; ///< Listener properties

        int                                 m_iLoadLevelCount;

        std::set< int >                  m_setPrepareLoad;
        std::set< int >                  m_setInitLoad;
        std::map< int, std::vector< std::string > >        m_mapLoadPosition;  ///< 자신을 로드할 State를 정할 수 있습니다.
        std::map< int, std::vector< std::string > >        m_mapLoadMode;
};

extern bool     Is_Sound;
extern bool     EnableSound;
extern KDSound  g_KDSound;

#endif // _KDSOUND_H_
