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
/** ���̷�Ʈ � ������ ������ǰ� ������ ����� ����ϴ� Ŭ����. �̸� ����ϱ� ����
strmiids.lib �� �������־�� �Ѵ�.  #pragma comment(lib, "strmiids")
���� �� ��Ʈ���� �����Ű�� ������ ���� resource leak�� �߻��Ѵ�. �� �ѹ� �ذ��غ���.
StackWalker���� �����ִ� �α׸� ���� ������ ���� 10������ 11���� Com Leak�� ��Ÿ����.
Resource leak : allocated by LoadLibraryA in delayhlp.cpp (119), HANDLE 0x70200000 */
class KShow  
{
public :
    enum 
    {
        WM_GRAPHNOTIFY = WM_USER + 2925,    // �̺�Ʈ�� ������ ������޽���.
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
        bool        m_bwindow;  // ���� ��Ʈ�������� ���.
        char        m_strFile[MAX_PATH];  // ���� �̸�.
    };
    std::vector<FileInfo>   m_vFileInfo;
private :
    HWND           m_hWnd;                 ///< �̺�Ʈ�� ������ ������ �ڵ�.
    bool           m_bMovie;               ///< ������ ��Ʈ���� �����ϴ� ���.(���â������ �ʿ�)
    double         m_fDBVolume;            ///< ����� ����. (-10000 ~ 0)

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
    IGraphBuilder*  m_pGraphBuilder;    ///< �׷��� ����. �׷��� �����Ϳ� ���� ������ �������̽�. 

    IMediaEventEx*  m_pMediaEventEx;
    IMediaControl*  m_pMediaControl;
    IMediaSeeking*  m_pMediaSeeking;
    IVideoWindow*   m_pVideoWindow;
    IBasicAudio*    m_pBacisAudio;
    bool            m_Repeat;

	//������ �÷��̸� ���� ��ġ ����
	DWORD	m_dwPosX;
	DWORD	m_dwPosY;
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
};

extern KShow g_Show;

#endif // _KSHOW_H_




/*

  * KShow
  
    1. ����ϱ� ��...
    
      . strmiids.lib �� �������־�� �Ѵ�.  #pragma comment(lib, "strmiids")
      
      . ���� cpp ���Ͽ���, include "math.h"�� ������ ���̿� �ִ� �ּ��� Ȱ��ȭ ��Ų��.
        
            // KShow* g_pShow = NULL;   
          
        ���α׷��� ���۰� ������ �� �����Ϳ� �޸� �Ҵ�/�����ؼ� ����.
        �ƴԹ�... singleton���� �ٲٴ���... ��� ������ �����ϵ���... �������� ^^;
            
      . InitShow( HWND ) �ѹ� �ҷ��ش�. ������ �ڵ��� ���ڷ� �����Ƿ�,
        �����찡 �����ǰ� �� �ڿ� ȣ���Ѵ�.
        �̰����� ����� ���� ����Ʈ�� LUA�� ���ؼ� �����µ�, lua�� ����Ұ� �ƴ϶�� 
        �� �Լ��� �����ؾ� �Ѵ�.
        lua���� ������ ���� �̵�� ��Ʈ������ �ּҸ� �����ش�. (���� �籸 ����...)
              
            -- �̵�� ���ϵ�.
            mediaData = {
                num = 3,
                media0={ window=1, file=".\\media\\intro.avi" },      -- �ʱ� ��Ʈ�� 
                media1={ window=0, file=".\\media\\bgm_lobby.wma" },  -- UI �����
                media2={ window=0, file=".\\media\\bgm_game.wma" },   -- ���� �����.
            }
                
                  
    2. ���
                    
      . Play( int ) �Լ��� ��Ʈ���� ����Ѵ�. �μ��� ��Ʈ���� �ε���. enum���� ����� �����ϰ�
        Play�Լ��� ���ڷ� ����ϸ� ����ȭ�� ������ �ȴ�.
                      
            g_pShow->Play( KShow::BGM_GAME );
                        
        InitShow(), Play() �Լ��� ���� �� ���ư���
                          
    3. ��Ʈ���� ���� ���
                            
      . ó���� InitShow( HWND )���� �־��� ������ �ڵ��, WM_GRAPHNOTIFY��� �޼����� ��������.
        �� �޼����� ���� ��� HandleEvent( LPARAM ) ������ �ҷ��ش�.
                              
          case KShow::WM_GRAPHNOTIFY :    // �Ѱ� : (direct show) ��Ʈ�� ����� �������� �˷��ش�.
              m_kShow.HandleEvent( lParam );  // ����� ���� ��Ʈ���� �������� ���� TRUE�� �����Ѵ�.
              break;
                                
        ����� ���� ��Ʈ���� �������� ���� TRUE�� �����Ѵ�.
                                  
    4. �������� ����� ��� �ΰ����� ó��
                                    
        . ���� �������� ũ�Ⱑ ������ ��� �������� ũ�⵵ ��ȭ���� �־�� �Ѵ�.
                                      
            case WM_SIZE:
                if( �������� ��µǰ� �ִ� ���̶�� )
                    m_kShow.OnSize();   // �Ѱ� : ������ ������ ����.
                                        
        . �������� ��Ʈ�� ȭ�鿡�� ����� ���, ������ ���� �ڵ��� ���� �����ؾ� �� �� ����.
          ������ �÷����ϴ� �����鿡�� � �ڵ��� �ְ� ������ Ȯ���� �� ����. 
                                          
        . ������ ����� ������� �ʰ� ���� ����� ������θ� ���ϰ�� 4�� ó���� �����ص� ����.

    5. �׹��� ��� - ���� ����
    
        . SetVolume( float ) �� ������ �����Ѵ�. 0.0 ~ 1.0 ������ ���� ���ڷ� �޴´�.
                                                
      -- ������ �� -- 040602. florist.
*/