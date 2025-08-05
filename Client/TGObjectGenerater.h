#pragma once
#include "TriggerCondition.h"

class CTGObjectGenerater : public CTriggerCondition
{
public:
    CTGObjectGenerater(void);
    ~CTGObjectGenerater(void);

    int         m_iTgID;
    bool        m_bOnOff;
    bool        m_bRight;
    bool        m_bGenerate;
    bool        m_bCrashAble;
    int         m_iSlot;
    int         m_iType;
    float       m_fEntranceX;
    float       m_fEntranceY;
	float       m_fProperty;

    void DestroyObj(int iSlot);
    void GenerateObj(int iSlot, int iType);
    void GenObject(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
