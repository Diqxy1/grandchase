#ifndef _KGCPLAYERLAS2_H_
#define _KGCPLAYERLAS2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerLas2
{
public:
    KGCPlayerLas2(void);
    ~KGCPlayerLas2(void);

    static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnArrowEvasion(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
};

#endif // _KGCPLAYERLAS2_H_