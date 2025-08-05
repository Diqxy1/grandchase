#pragma once
#include "BaseTrigger.h"

class CSoundOnOff : public CBaseEventAction
{
public:
    CSoundOnOff()
    {
        Init();
    }
    ~CSoundOnOff() {}

    bool            m_bSoundOn;
    TCHAR           m_szSoundFileName[MAX_TG_STR_LENGTH];

    void Init(void);
    void SetValue(bool bSoundOn, TCHAR *lpszSoundFileName);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};