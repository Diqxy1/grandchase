// KMci.h: interface for the KMci class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KMCI_H__0D7494B4_A72F_40EC_8298_C3DC80E35CD0__INCLUDED_)
//#define AFX_KMCI_H__0D7494B4_A72F_40EC_8298_C3DC80E35CD0__INCLUDED_

#ifndef _KMCI_H_
#define _KMCI_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "KPlayer.h"
#include "KShow.h"

#define DSHOW
//#define MCI

class KMci  
{
public:
	HRESULT SetVolume(float);
	HRESULT Continue();
	HRESULT Stop();
	HRESULT Play(int, bool, bool bForcePlay = false );
	HRESULT Clear();
	HRESULT Init(HINSTANCE, HWND, const char*, const char*);

#ifdef DSHOW
	BOOL HandleEvent( LPARAM lParam ){
		m_kShow.HandleEvent( lParam );
		m_kShowForMovie.HandleEvent(lParam);
		return TRUE;
	}
#endif

	bool IsPlay()
	{
		return ( m_bPlayMusic && m_bToggleMusic );
	}
	void SetPlay(bool bToggleMusic)
	{
		m_bToggleMusic = bToggleMusic;
	}

	char GetTrack(){return m_cNowTrack;}

    KShow &GetShow(void) { return m_kShow; }
	KShow &GetShowForMovie(void) { return m_kShowForMovie; }
    void AddMusic(KShow::FileInfo fileinfo);
    void Play(const char *lpszFIleName);
	void LuaPlay( int nIndex, bool bRepeat, bool bForcePlay = false );
	KMci();
	virtual ~KMci();

private:
#ifdef DSHOW
	KShow m_kShow;			//DShow플레이
	KShow m_kShowForMovie;
#else
	KPlayer m_kPlayer;		//MCI플레이

	std::vector<std::string> m_vFilePath;
	KLuaManager m_kLuaMgr;
#endif

	bool m_bPlayMusic;		//TRUE일때 뮤직 실행가능
	bool m_bToggleMusic;	//TRUE일때 뮤직 플레이
	char m_cNowTrack;
	bool m_bRepeat;
};

extern KMci g_KMci;

#endif // _KMCI_H_
