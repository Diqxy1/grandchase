#include "StdAfx.h"
#include ".\Embark.h"

void CEmbark::Init(void)
{
    m_enType            = TG_EACTION_EMBARK;

    m_bIsRight = false;
    m_iEmbarkType = 0;
    m_iEmbarkSlot = 0;
    m_iEmbarkationCharSlot = 0;
    m_vPos = D3DXVECTOR2(0.0f, 0.0f);
}

void CEmbark::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);
    WriteToFileScript(hWrite, "FuctionName", "TGEmbark", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "EmbarkType", m_iEmbarkType, iTab+1);
    WriteToFileScript(hWrite, "EmbarkSlot", m_iEmbarkSlot, iTab+1);
    WriteToFileScript(hWrite, "EntranceX", m_vPos.x, iTab+1);
    WriteToFileScript(hWrite, "EntranceY", m_vPos.y, iTab+1);
    WriteToFileScript(hWrite, "Right", m_bIsRight, iTab+1);
    WriteToFileScript(hWrite, "EmbarkationCharSlot", (m_iEmbarkationCharSlot-1), iTab+1);

    WriteCloseBrace(hWrite, iTab, 0);
}

void CEmbark::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CEmbark *pEmbark = new CEmbark;
    *pEmbark = *(CEmbark*)this;
    *pDest = (CBaseEventAction*)pEmbark;
}

HRESULT CEmbark::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("EmbarkType", m_iEmbarkType, return E_FAIL);
    LUA_GET_VALUE("EmbarkSlot", m_iEmbarkSlot, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_vPos.x, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_vPos.y, return E_FAIL);
    LUA_GET_VALUE("Right", m_bIsRight, return E_FAIL);
    LUA_GET_VALUE("EmbarkationCharSlot", m_iEmbarkationCharSlot, return E_FAIL);
    m_iEmbarkationCharSlot += 1;
    return S_OK;
}
