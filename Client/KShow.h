// KShow.h: interface for the KShow class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KShow_H__BCD7B63D_8AF5_46CD_BA87_76C2ED82ACDB__INCLUDED_)
//#define AFX_KShow_H__BCD7B63D_8AF5_46CD_BA87_76C2ED82ACDB__INCLUDED_

#ifndef _KSHOW_H_
#define _KSHOW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dshow.h>
////#include <vector>

#pragma comment(lib, "strmiids")
/** 다이렉트 쇼를 구현해 배경음악과 동영상 재생을 담당하는 클래스. 이를 사용하기 위해
strmiids.lib 를 연결해주어야 한다.  #pragma comment(lib, "strmiids")
또한 이 스트림을 재생시키면 다음과 같은 resource leak이 발생한다. 잘 한번 해결해보라.
StackWalker에서 남겨주는 로그를 보면 설정에 따라 10개에서 11개의 Com Leak가 나타난다.
Resource leak : allocated by LoadLibraryA in delayhlp.cpp (119), HANDLE 0x70200000 */
class KShow  
{
public :
    enum 
    {
        WM_GRAPHNOTIFY = WM_USER + 2925,    // 이벤트를 통지할 윈도우메시지.
        // WM_USER through 0x7FFF : Integer messages for use by private window classes 
        // 0x400 - 0x7FFF ( 1024 - 32767 )
        MOVIE_INTRO = 0,
        BGM_UI,
        BGM_GAME,
    };

public:
	KShow();
	virtual ~KShow();

    struct FileInfo 
    {
        bool        m_bwindow;  // 비디오 스트림인지를 기억.
        char        m_strFile[MAX_PATH];  // 파일 이름.
    };
    std::vector<FileInfo>   m_vFileInfo;
private :
    HWND           m_hWnd;                 ///< 이벤트를 전송할 윈도우 핸들.
    bool           m_bMovie;               ///< 동영상 스트림을 포함하는 경우.(재생창설정에 필요)
    double         m_fDBVolume;            ///< 오디오 볼륨. (-10000 ~ 0)

	bool			m_bIsPlaying;

    HRESULT         m_hrCoInitialize ;

    
//////////////////////////////////////////////////////////////////////////
// member function
public :
	bool IsPlaying()		{ return m_bIsPlaying; }
	HRESULT OnSize();
	BOOL HandleEvent( LPARAM lParam );
	HRESULT Play( int nIndex, bool repeat );
    HRESULT Stop( int nIndex );
    HRESULT Continue( int nIndex );
    HRESULT InitShow( HWND hWnd, const char* filename , const char* path , const char* tablename_ );
	HRESULT InitShowForMovie( HWND hWnd, const char* filename , const char* path );
    HRESULT Clear();
    HRESULT SetVolume( float fVolume );

	void SetVideoWinPos(const D3DXVECTOR2& vPos_ , const DWORD& dwWidth_ , const DWORD& dwHeight_);

//////////////////////////////////////////////////////////////////////////
// protected /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
protected :
    IGraphBuilder*  m_pGraphBuilder;    ///< 그래프 빌더. 그래프 에디터와 같은 역할의 인터페이스. 

    IMediaEventEx*  m_pMediaEventEx;
    IMediaControl*  m_pMediaControl;
    IMediaSeeking*  m_pMediaSeeking;
    IVideoWindow*   m_pVideoWindow;
    IBasicAudio*    m_pBacisAudio;
    bool            m_Repeat;

	//동영상 플레이를 위한 위치 지정
	DWORD	m_dwPosX;
	DWORD	m_dwPosY;
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
};

extern KShow g_Show;

#endif // _KSHOW_H_




/*

  * KShow
  
    1. 사용하기 전...
    
      . strmiids.lib 를 연결해주어야 한다.  #pragma comment(lib, "strmiids")
      
      . 먼저 cpp 파일에서, include "math.h"와 생성자 사이에 있는 주석을 활성화 시킨다.
        
            // KShow* g_pShow = NULL;   
          
        프로그램의 시작과 끝에서 이 포인터에 메모리 할당/해제해서 쓴다.
        아님뭐... singleton으로 바꾸던지... 멤버 변수로 선언하든지... 자유지만 ^^;
            
      . InitShow( HWND ) 한번 불러준다. 윈도우 핸들을 인자로 받으므로,
        윈도우가 생성되고 난 뒤에 호출한다.
        이곳에서 재생할 파일 리스트를 LUA를 통해서 얻어오는데, lua를 사용할게 아니라면 
        이 함수를 수정해야 한다.
        lua에는 다음과 같이 미디어 스트림파일 주소를 적어준다. (현재 당구 에서...)
              
            -- 미디어 파일들.
            mediaData = {
                num = 3,
                media0={ window=1, file=".\\media\\intro.avi" },      -- 초기 인트로 
                media1={ window=0, file=".\\media\\bgm_lobby.wma" },  -- UI 배경음
                media2={ window=0, file=".\\media\\bgm_game.wma" },   -- 게임 배경음.
            }
                
                  
    2. 재생
                    
      . Play( int ) 함수로 스트림을 재생한다. 인수는 스트림의 인덱스. enum으로 상수를 선언하고
        Play함수의 인자로 사용하면 문서화에 도움이 된다.
                      
            g_pShow->Play( KShow::BGM_GAME );
                        
        InitShow(), Play() 함수만 쓰면 잘 돌아간다
                          
    3. 스트림이 끝난 경우
                            
      . 처음에 InitShow( HWND )에서 넣어준 윈도우 핸들로, WM_GRAPHNOTIFY라는 메세지가 보내진다.
        이 메세지를 받은 경우 HandleEvent( LPARAM ) 파일을 불러준다.
                              
          case KShow::WM_GRAPHNOTIFY :    // 총각 : (direct show) 스트림 재생이 끝났음을 알려준다.
              m_kShow.HandleEvent( lParam );  // 재생이 끝난 스트림이 동영상인 경우는 TRUE를 리턴한다.
              break;
                                
        재생이 끝난 스트림이 동영상인 경우는 TRUE를 리턴한다.
                                  
    4. 동영상을 재생할 경우 부가적인 처리
                                    
        . 게임 윈도우의 크기가 변했을 경우 동영상의 크기도 변화시켜 주어야 한다.
                                      
            case WM_SIZE:
                if( 동영상이 출력되고 있는 중이라면 )
                    m_kShow.OnSize();   // 총각 : 동영상 사이즈 조정.
                                        
        . 동영상을 인트로 화면에서 사용할 경우, 동영상에 쓰인 코덱도 같이 배포해야 할 거 같다.
          게임을 플레이하는 유저들에게 어떤 코덱이 있고 없는지 확신할 수 없다. 
                                          
        . 동영상 재생을 사용하지 않고 단지 배경음 재생으로만 쓰일경우 4번 처리는 무시해도 좋다.

    5. 그밖의 기능 - 볼륨 조절
    
        . SetVolume( float ) 로 볼륨을 조절한다. 0.0 ~ 1.0 사이의 값을 인자로 받는다.
                                                
      -- 문서의 끝 -- 040602. florist.
*/