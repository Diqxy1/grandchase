#pragma once
#include "TriggerCondition.h"
//#include <Windows.h>

class CTGSoundOnOff : public CTriggerCondition
{
public:
    CTGSoundOnOff(void);
    ~CTGSoundOnOff(void);

    bool m_bSoundOn;
    char m_szSoundFileName[MAX_TG_STR_LENTH];

    void SetSoundOn(bool bSoundOn);
    void SetSoundFileName(char *lpszFileName);

    void Sound(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
