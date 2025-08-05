#pragma once
#include "BaseTrigger.h"

struct PARTICLE_WEATHER
{
    std::string m_strSeq;
    D3DXVECTOR2 m_vPos;

    PARTICLE_WEATHER()
    {
        init();
    }

    void init()
    {
        m_vPos.x = 0.0f;
        m_vPos.y = 0.0f;
        m_strSeq.clear();
    }
};

struct WEATHER_WIND
{
    D3DXVECTOR2 m_vSpeed;
    float m_fJumpRadio;

    WEATHER_WIND()
    {
        init();
    }

    void init()
    {
        m_vSpeed.x = 0.0f;
        m_vSpeed.y = 0.0f;
        m_fJumpRadio = 0.0f;
    }
};

class CWeather : public CBaseEventAction
{
public:
    CWeather() { Init();  }
    ~CWeather() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

// 날씨타입
    enum
    {
        WEATHERTYPE_NONE = 0,
        WEATHERTYPE_WIND = 1,
    };

// 기본 날씨정보
    int m_iType;
    D3DXVECTOR4 m_vRect;
    std::vector<PARTICLE_WEATHER> m_vecParticle;

// 날씨별 속성
    WEATHER_WIND m_vWindProperty;
};
