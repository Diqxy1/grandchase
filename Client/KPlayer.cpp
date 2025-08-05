#include "stdafx.h"
// KPlayer.cpp: implementation of the KPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "KPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KPlayer::KPlayer()
{
	m_uiwDeviceID = 0;
	m_nVolume = 65535;	//문기 : 볼륨맥스값

	m_bRepeat = false;
}

KPlayer::~KPlayer()
{
	Close();
}

/* 플레이어를 초기화 시켜준다. */
DWORD KPlayer::Init(HINSTANCE hInst)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = (WNDPROC)KPlayer::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"KPlayer";
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);

	m_hWnd = CreateWindow(L"KPlayer", L"Hidden", 0, 0, 0, 0, 0, NULL, NULL, hInst, (LPVOID)this);

	m_mciOpenParms.lpstrDeviceType = L"MPEGVideo";
	m_mciPlayParms.dwCallback = (DWORD)m_hWnd;

	return 0;
}

/* 파일열기 */
DWORD KPlayer::OpenFile(WCHAR* lpszMp3)
{
	Close();

	DWORD Result;
	m_mciOpenParms.lpstrElementName = lpszMp3;

	Result = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD)(LPVOID)&m_mciOpenParms);
	m_uiwDeviceID = m_mciOpenParms.wDeviceID;

	//char szError[100];
	//mciGetErrorString(Result, szError, sizeof(szError));

	if (Result)
		return 0;
	else
		return 1;
}

/*재생 하기 전에 Init와 OpenFile이 먼저 수행 되어 있어야 함*/
DWORD KPlayer::Play()
{
	if (IsPlay())
	{
		Stop();
	}
	DWORD Result = mciSendCommand(m_uiwDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&m_mciPlayParms);

	if (Result)
	{
		mciSendCommand(m_uiwDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
	}

	return Result;
}

/*재생을 중지한다. 처음의 위치로 돌아간다.*/
int KPlayer::Stop()
{
	MCI_SEEK_PARMS seek;

	DWORD stop_Result;
	MCI_GENERIC_PARMS mciGenericParms;

	stop_Result = mciSendCommand(m_uiwDeviceID, MCI_STOP,
		0, (DWORD)(LPVOID)&mciGenericParms);

	mciSendCommand(m_uiwDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)&seek);

	return stop_Result;
}

DWORD KPlayer::Pause()
{
	return mciSendCommand(m_uiwDeviceID, MCI_PAUSE,
		MCI_WAIT, (DWORD)(LPVOID)&m_mciGenericParms);
}

DWORD KPlayer::Resume()
{
	return mciSendCommand(m_uiwDeviceID, MCI_RESUME,
		MCI_WAIT, (DWORD)(LPVOID)&m_mciGenericParms);
}

void KPlayer::Close()
{
	if (m_uiwDeviceID != 0)
	{
		mciSendCommand(m_uiwDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
	}
}

void KPlayer::Set_volume(int volume_num)
{
	volume_num += 5000;	//문기 : 0 ~ 5000값으로 변경 (볼륨 컨트롤의 범위가 -5000 ~ 0이다)

	volume_num = (65535 * volume_num) / 5000;

	m_nVolume = volume_num;

	long lLeftVol, lRightVol;

	lLeftVol = volume_num;
	lRightVol = volume_num;
	volume_num = (volume_num << 16) + lRightVol;

	//문기 : mp3파일은 조절안되는듯
	//midiOutSetVolume((HMIDIOUT)m_mciOpenParms.wDeviceID, volume_num);

	waveOutSetVolume(0, volume_num);
}

int KPlayer::Get_Volume()
{
	return m_nVolume;
}

bool KPlayer::SetRepeat(bool repeat)
{
	m_bRepeat = repeat;
	return m_bRepeat;
}

bool KPlayer::GetRepeat()
{
	return m_bRepeat;
}

bool KPlayer::IsPause()
{
	bool b_pause;
	b_pause = (Get_Status() == MCI_MODE_PAUSE);

	return b_pause;
}

bool KPlayer::IsPlay()
{
	bool b_play;
	b_play = (Get_Status() == MCI_MODE_PLAY);

	return b_play;
}

DWORD KPlayer::Status_Mode(DWORD dwFlag)
{
	MCI_STATUS_PARMS mciStatusParms;

	mciStatusParms.dwItem = dwFlag;
	mciSendCommand(m_uiwDeviceID, MCI_STATUS,
		MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms);

	return mciStatusParms.dwReturn;
}

DWORD KPlayer::Get_Status()
{
	DWORD status;
	status = Status_Mode(MCI_STATUS_MODE);

	return status;
}

LRESULT CALLBACK KPlayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static KPlayer* ptr;

	switch (message)
	{
	case WM_CREATE:
		ptr = (KPlayer*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		break;

	case MM_MCINOTIFY:
		switch (wParam)
		{
		case MCI_NOTIFY_SUCCESSFUL:
			if (ptr->m_bRepeat)
				ptr->Play();
			else
				ptr->Close();
			break;
		case MCI_NOTIFY_FAILURE:
		case MCI_NOTIFY_SUPERSEDED:
		case MCI_NOTIFY_ABORTED:
			//ptr->Close();//mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL );
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}