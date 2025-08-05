#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerJin1Burnning
{
public:
	KGCPlayerJin1Burnning(void);
	virtual ~KGCPlayerJin1Burnning(void);

	static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
};
