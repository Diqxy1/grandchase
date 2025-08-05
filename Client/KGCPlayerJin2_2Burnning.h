#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerJin2_2Burnning
{
public:
	KGCPlayerJin2_2Burnning(void);
	virtual ~KGCPlayerJin2_2Burnning(void);

	static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
};
