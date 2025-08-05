#include "StdAfx.h"
#include ".\Weather.h"

void CWeather::Init(void)
{
    // 다이얼로그 타입
    m_enType            = TG_EACTION_WEATHER;

    // 기본 날씨 정보
    m_iType = WEATHERTYPE_NONE;
    m_vRect.x = 0.0f;   m_vRect.y = 0.0f;
    m_vRect.z = 0.0f;   m_vRect.w = 0.0f;
    m_vecParticle.clear();

    // 날씨별 속성
    m_vWindProperty.init();
}

void CWeather::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);
    WriteToFileScript(hWrite, "FuctionName", "TGWeather", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);        //DLG에서 처리
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);

    // 기본 날씨 정보
    WriteToFileScript(hWrite, "WeatherID", m_iType, iTab+1);
    WriteToFileScript(hWrite, "EntranceX", m_vRect.x, iTab+1);
    WriteToFileScript(hWrite, "EntranceY", m_vRect.y, iTab+1);
    WriteToFileScript(hWrite, "Width", m_vRect.z, iTab+1);
    WriteToFileScript(hWrite, "Height", m_vRect.w, iTab+1);

    // 바람속성
    WriteToFileScript(hWrite, "WindSpeedX", m_vWindProperty.m_vSpeed.x, iTab+1);
    WriteToFileScript(hWrite, "WindSpeedY", m_vWindProperty.m_vSpeed.y, iTab+1);
    WriteToFileScript(hWrite, "JumpWindRadio", m_vWindProperty.m_fJumpRadio, iTab+1);

    // 파티클정보
    strBraceTitle.Format(L"WeatherParticle");
    WriteOpenBrace(hWrite, strBraceTitle, iTab+1);
    if( false == m_vecParticle.empty() )
    {
        std::vector<PARTICLE_WEATHER>::iterator vecIter = m_vecParticle.begin();
        for(; vecIter != m_vecParticle.end(); ++vecIter)
        {
            WriteOnlyOpenBrace_NoEnter(hWrite, iTab+2);
            WriteToFileScript_OnlyValue_String(hWrite, vecIter->m_strSeq, 0, false);
            WriteToFileScript_OnlyValue_Number(hWrite, vecIter->m_vPos.x, 0, false);
            WriteToFileScript_OnlyValue_Number(hWrite, vecIter->m_vPos.y, 0, false);
            WriteCloseBrace(hWrite, 1, 0);
        }
    }
    WriteCloseBrace(hWrite, iTab+1, 0);

    WriteCloseBrace(hWrite, iTab, 0);
}

void CWeather::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CWeather *pAddDamage = new CWeather;
    *pAddDamage = *(CWeather*)this;
    *pDest = (CBaseEventAction*)pAddDamage;
}

HRESULT CWeather::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("WeatherID", m_iType, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_vRect.x, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_vRect.y, return E_FAIL);
    LUA_GET_VALUE("Width", m_vRect.z, return E_FAIL);
    LUA_GET_VALUE("Height", m_vRect.w, return E_FAIL);
    LUA_GET_VALUE("WindSpeedX", m_vWindProperty.m_vSpeed.x, return E_FAIL);
    LUA_GET_VALUE("WindSpeedY", m_vWindProperty.m_vSpeed.y, return E_FAIL);
    LUA_GET_VALUE("JumpWindRadio", m_vWindProperty.m_fJumpRadio, return E_FAIL);

    m_vecParticle.clear();
    LUA_BEGIN_TABLE_RAII( "WeatherParticle" )
    {
        int iLoop=1;
        PARTICLE_WEATHER particle;
        while(1)
        {
            LUA_BEGIN_TABLE_RAII( iLoop++ )
            {
                luaMgr.GetValue(1, particle.m_strSeq);
                luaMgr.GetValue(2, particle.m_vPos.x);
                luaMgr.GetValue(3, particle.m_vPos.y);
                m_vecParticle.push_back( particle );
            }
            else
                break;
        }
    }

    return S_OK;
}
