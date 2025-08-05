#ifndef _GCGUILDMARK_H_
#define _GCGUILDMARK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
#define SIZE_GUILMARK_POOL 30   //풀에 모아두는 길드 마크 제한.

class GCDeviceTexture;

class KGCGuildMark
{
public:
	KGCGuildMark(void);
	~KGCGuildMark(void);

	void DeleteDevice();
	HRESULT InitDevice();

	void Render(int Color, int Back, int Fore, float x, float y, float Size, int iIndex = 0);

private:
	std::vector<GCDeviceTexture *> m_vecTextrue;
	std::vector<std::wstring> m_vecGuildCode;
};

#endif //_GCGUILDMARK_H_