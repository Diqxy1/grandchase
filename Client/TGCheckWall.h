#pragma once
#include "TriggerCondition.h"

class CTGCheckWall : public CTriggerCondition
{
public:
	CTGCheckWall(void);
public:
	~CTGCheckWall(void);

private:
	int m_iWallSlot;
    bool m_bLive;
    bool CheckWallLive( int iSlot, bool bLive );

public:
	HRESULT LoadItemFromLua(KLuaManager &luaMgr);
	virtual void FrameMove(void);
};
