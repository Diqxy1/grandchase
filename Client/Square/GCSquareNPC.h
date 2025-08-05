#pragma once
#include "gcsquare3dobj.h"

class KGCSquareNPC :
	public KGCSquare3DObj
{
private:
	static DWORD g_dwSquareNPC_UID;
	DWORD m_dwUID;
public:
	KGCSquareNPC(void);
	~KGCSquareNPC(void);

	DWORD m_dwLastSpeech;
	int m_iSpeechSize;

	virtual void FrameMove();

	void AddNPCSpeechList( std::wstring strSpeech );
	void FrameMove_NPCSpeech( void );
};
