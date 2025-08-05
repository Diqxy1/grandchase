#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerJin2Burnning
{
public:
	KGCPlayerJin2Burnning(void);
	virtual ~KGCPlayerJin2Burnning(void);

	static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
};
