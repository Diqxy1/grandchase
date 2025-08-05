#include "StdAfx.h"
#include ".\gateobjectgenerater.h"

void CGateObjectGenerater::Init(void)
{
    m_enType = TG_EACTION_GATE_OBJECT_GEN;
    m_bOnOff            = false;
    m_bRight            = true;
    m_bGenerate         = true;
    m_iDirection        = 0;
    m_iDelayTime        = 3000;       // 대략 3초
    m_iSlot             = 0;
    m_iType             = 0;
    m_fEntranceX        = 0.0f;
    m_fEntranceY        = 0.0f;
    m_iDotaStage        = 0;
}

void CGateObjectGenerater::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGGateObjectGennerater", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "On", m_bOnOff, iTab+1);
    WriteToFileScript(hWrite, "Right", m_bRight, iTab+1);
    WriteToFileScript(hWrite, "Generate", m_bGenerate, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "Type", m_iType, iTab+1);
    WriteToFileScript(hWrite, "Direction", m_iDirection, iTab+1);
    WriteToFileScript(hWrite, "DelayTime", m_iDelayTime, iTab+1);    
    WriteToFileScript(hWrite, "EntranceX", m_fEntranceX, iTab+1);
    WriteToFileScript(hWrite, "EntranceY", m_fEntranceY, iTab+1);
    WriteToFileScript(hWrite, "DotaStage", m_iDotaStage, iTab+1);
    
    WriteCloseBrace(hWrite, iTab, 0);
}

void CGateObjectGenerater::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CGateObjectGenerater *pGateObjectGen = new CGateObjectGenerater;
    *pGateObjectGen = *(CGateObjectGenerater*)this;
    *pDest = (CBaseEventAction*)pGateObjectGen;
}

HRESULT CGateObjectGenerater::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("Right", m_bRight, return E_FAIL);
    LUA_GET_VALUE("Generate", m_bGenerate, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("Type", m_iType, return E_FAIL);
    LUA_GET_VALUE("Direction", m_iDirection, return E_FAIL);
    LUA_GET_VALUE("DelayTime", m_iDelayTime, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_fEntranceX, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_fEntranceY, return E_FAIL);
    LUA_GET_VALUE("DotaStage", m_iDotaStage, return E_FAIL);

    return S_OK;
}