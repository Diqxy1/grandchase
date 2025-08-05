#pragma once
#include "TriggerCondition.h"

class CTGObjHpChange : public CTriggerCondition
{
public:
	CTGObjHpChange(void);
public:
	~CTGObjHpChange(void);

	enum{
		HC_PLAYER = 0,
		HC_MONSTER = 1,
	};

private:
	int m_iObjectIndex;
	float m_fPercent;
	int m_iObjectType;

	bool CheckPlayerHP( int iIndex, float fPercent );
	bool CheckMonsterHP( int iIndex, float fPercent );

public:
	HRESULT LoadItemFromLua(KLuaManager &luaMgr);
	virtual void FrameMove(void);
};
