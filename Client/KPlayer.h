// KPlayer.h: interface for the KPlayer class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KPLAYER_H__19E7FC0E_6762_4954_AE7B_200C960329BD__INCLUDED_)
//#define AFX_KPLAYER_H__19E7FC0E_6762_4954_AE7B_200C960329BD__INCLUDED_

#ifndef _KPLAYER_H_
#define _KPLAYER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Windows Header Files:
//#include <windows.h>

// C RunTime Header Files
#include <cstdlib>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <mmsystem.h>

class KPlayer
{
public:
	KPlayer();
	virtual ~KPlayer();

	DWORD	Init(HINSTANCE);
	DWORD	OpenFile(WCHAR*);
	DWORD	Play();
	DWORD   Pause();
	DWORD   Resume();
	void	Close();
	int		Stop();

	void	Set_volume(int volume_num);
	int		Get_Volume();
	bool	SetRepeat(bool);
	bool	GetRepeat();

	bool	IsPause();
	bool	IsPlay();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DWORD	Status_Mode(DWORD dwFlag);
	DWORD	Get_Status();

	UINT				m_uiwDeviceID;
	int					m_nVolume;
	bool				m_bRepeat;

	MCI_OPEN_PARMS		m_mciOpenParms;
	MCI_PLAY_PARMS		m_mciPlayParms;
	MCI_GENERIC_PARMS	m_mciGenericParms;
	HWND				m_hWnd;
};

#endif // _KPLAYER_H_
