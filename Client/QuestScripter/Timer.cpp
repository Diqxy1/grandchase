#include "stdafx.h"
#include "Timer.h"

void CTimer::Init(void)
{
    m_enType = TG_CONDITION_TIMER;
    m_iUI   = 0;
    m_iTime = 0; 
}

void CTimer::SetValue(int iTime)
{
    m_iTime = iTime;
}

void CTimer::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGTimer", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "IsUI", (int)m_iUI, iTab+1);
    WriteToFileScript(hWrite, "Timer", (int)m_iTime, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CTimer::CopyConditionToDest(CBaseCondition **pDest)
{
    CTimer *pTimer = new CTimer;
    *pTimer = *(CTimer*)this;
    *pDest = (CBaseCondition*)pTimer;
}

HRESULT CTimer::LoadFromFile_Item(KLuaManager &luaMgr)
{
    luaMgr.GetValue( "IsUI", m_iUI );
    LUA_GET_VALUE("Timer", m_iTime, return E_FAIL);

    return S_OK;
}