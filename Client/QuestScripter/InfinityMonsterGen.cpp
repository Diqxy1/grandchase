#include "stdafx.h"
#include "InfinityMonsterGen.h"

void CInfinityMonsterGen::Init(void)
{
    m_enType = TG_EACTION_INFINITY_MON_GEN;
    m_bInfinityMonserGen = false;
}

void CInfinityMonsterGen::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGInfinityMonsterGen", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "INFINITY_GEN", m_bInfinityMonserGen, iTab+1);

    WriteCloseBrace(hWrite, iTab, 0);
}

void CInfinityMonsterGen::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CInfinityMonsterGen *pInfinityMonsterGen = new CInfinityMonsterGen;
    *pInfinityMonsterGen = *(CInfinityMonsterGen*)this;
    *pDest = (CBaseEventAction*)pInfinityMonsterGen;
}

HRESULT CInfinityMonsterGen::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("INFINITY_GEN", m_bInfinityMonserGen, return E_FAIL);
    return S_OK;
}