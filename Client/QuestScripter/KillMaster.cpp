#include "stdafx.h"
#include "KillMaster.h"

void CKillMaster::Init(void)
{
    m_enType = TG_CONDITION_KILLMASTER; m_iMonIndex = 0; m_iMonType = 0; m_bAlive = false;
}

void CKillMaster::SetValue(int iMonIndex, int iMonType)
{
    m_iMonIndex = iMonIndex;
    m_iMonType = iMonType;
}

void CKillMaster::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGKillMaster", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "MonIndex", m_iMonIndex, iTab+1);
	WriteToFileScript(hWrite, "MonType", m_iMonType, iTab+1);
	WriteToFileScript(hWrite, "MonKill", m_bAlive, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CKillMaster::CopyConditionToDest(CBaseCondition **pDest)
{
    CKillMaster *pKillMaster = new CKillMaster;
    *pKillMaster = *(CKillMaster*)this;
    *pDest = (CBaseCondition*)pKillMaster;
}

HRESULT CKillMaster::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("MonIndex", m_iMonIndex, return E_FAIL);
	LUA_GET_VALUE("MonType", m_iMonType, return E_FAIL);
	LUA_GET_VALUE_DEF("MonKill", m_bAlive, 0);

    return S_OK;
}