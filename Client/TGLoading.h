#pragma once
#include "TriggerCondition.h"
//#include <Windows.h>

class CTGLoading : public CTriggerCondition
{
public:
    CTGLoading();
    ~CTGLoading();

    int     m_iTexID;
    bool    m_bOnOff;                        // 로딩 화면을 띄울지... 로딩화면을 종료할지...

    void SetLoading(bool bOnOff);
    void SetLoadingID(int iTexID);
    void LoadingStage(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
