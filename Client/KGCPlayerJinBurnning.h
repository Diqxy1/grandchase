#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerJinBurnning
{
public:
	KGCPlayerJinBurnning(void);
	virtual ~KGCPlayerJinBurnning(void);

	static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
};
