#include "stdafx.h"
#include ".\water.h"

void CWater::Init(void)
{
    m_enType            = TG_EACTION_WATER;
    m_bOnOff            = 0;
    m_iWaterHeight      = 0;
}

void CWater::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);
    WriteToFileScript(hWrite, "FuctionName", "TGWater", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "OnOff", m_bOnOff, iTab+1);
    WriteToFileScript(hWrite, "WaterHeight", m_iWaterHeight, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CWater::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CWater *pAddDamage = new CWater;
    *pAddDamage = *(CWater*)this;
    *pDest = (CBaseEventAction*)pAddDamage;
}

HRESULT CWater::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("OnOff", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("WaterHeight", m_iWaterHeight, return E_FAIL);

    return S_OK;
}
