#pragma once
//#include <Windows.h>
#include "TriggerCondition.h"

class CTGTimer : public CTriggerCondition
{
public:
    CTGTimer(void);
    ~CTGTimer(void);

    int     m_iUI;
    bool    m_bTimerOn;
    float   m_fTime;
    float   m_fRemain;

	bool m_bStop;

	int		m_iType; //타이머 타입 끝날때 무엇인가 보내는가?

    void StartTrigger(void);
    void ResetTimer(void);
    //void TimerOn(bool bOn);
    void SetTime(float fTime);
    void FrameMove(void);
    void Time(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
	void EndTrigger(void);
};
