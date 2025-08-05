#pragma once
#include "TriggerCondition.h"

class CTGChangeObjState : public CTriggerCondition
{
public:
    CTGChangeObjState(void);
    ~CTGChangeObjState(void);

    int m_iSlot;
    int m_iType;
	int m_iState;
	int m_iOBJType;

	void SetObjStateInfo(int iSlot, int iType, int iState);
    void ChangeObjState(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
