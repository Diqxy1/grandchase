#include "stdafx.h"
#include "KillAllMonster.h"

void CKillAllMonster::Init(void)
{
    m_enType = TG_CONDITION_KILLALLMONSTER; 
    m_bAlive = false;
}


void CKillAllMonster::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGKillAllMonster", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "MonKill", m_bAlive, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CKillAllMonster::CopyConditionToDest(CBaseCondition **pDest)
{
    CKillAllMonster *pKillAllMonster = new CKillAllMonster;
    *pKillAllMonster = *(CKillAllMonster*)this;
    *pDest = (CBaseCondition*)pKillAllMonster;
}

HRESULT CKillAllMonster::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE_DEF("MonKill", m_bAlive, 0);
    return S_OK;
}