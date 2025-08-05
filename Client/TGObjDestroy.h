#pragma once
#include "TriggerCondition.h"

class CTGObjDestroy : public CTriggerCondition
{
public:
    CTGObjDestroy(void);
    ~CTGObjDestroy(void);

    int         m_iObjIndex;                    // 몬스터의 인덱스
    int         m_iObjType;                     // 몬스터 종류

    void SetObject( int iObjType, int iObjIndex );
    void FrameMove(void);
    void EndTrigger();
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
