#pragma once
#include "TriggerCondition.h"

struct WEATHER_PARTICLE
{
    std::string m_strSeq;
    D3DXVECTOR2 m_vPos;
};

class CTGWeather : public CTriggerCondition
{
public:
    CTGWeather(void);
    ~CTGWeather(void);

private:
    enum
    {
        WEATHER_NONE = 0,
        WEATHER_WIND,
    };
    int     m_iWeatherID;
    bool    m_bBeginTG;

private:// 바람속성
    // 바람영역
    float   m_fWidth;
    float   m_fHeight;
    float   m_fEntranceX;
    float   m_fEntranceY;
    
    // 바람속도
    float   m_fWindSpeedX;
    float   m_fWindSpeedY;
    float   m_fJump_WindRadio;


private:// 파티클
    std::vector<WEATHER_PARTICLE> m_vecParticle;

public:
    void InitData(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

private:
    void StartParticle();
    void CheckEffect();

private:
    void CheckWind();
};
