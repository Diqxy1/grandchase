#pragma once
#include "TriggerCondition.h"

class CTGFadeInOut : public CTriggerCondition
{
public:
    CTGFadeInOut(void);
    ~CTGFadeInOut(void);

    bool        m_bOnOff;
    int         m_iSpeed;
	int			m_Red;
	int			m_Green;
	int			m_Blue;
    int         m_dwAlpha;

    void StartTrigger(void);
    void FadeInOut(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

};
