#ifndef _KGCTAGPLAYER_H_
#define _KGCTAGPLAYER_H_

#if _MSC_VER > 1000
#pragma once
#endif

class PLAYER;

class KGCTagPlayer
{
public:
	KGCTagPlayer();
	virtual ~KGCTagPlayer();

    static int OnTagEnd(PLAYER* pPlayer, bool bKeyProc);
    static int OnTagStart(PLAYER* pPlayer, bool bKeyProc);

private:
	static void InitBeforeTag( PLAYER* pPlayer_ );
};

#endif // _KGCTAGPLAYER_H_
