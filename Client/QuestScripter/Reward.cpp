#include "StdAfx.h"
#include ".\reward.h"

CReward::CReward(void)
{
    Init();
}

CReward::~CReward(void)
{
}

void CReward::Init(void)
{
    m_enType = TG_EACTION_REWARD;
    m_iEXP = 0;
    m_iGP = 0;
}

void CReward::SetValue(int iExp, int iGp)
{
    m_iEXP = iExp;
    m_iGP = iGp;
}

void CReward::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "Reward", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "EXP", m_iEXP, iTab+1);
    WriteToFileScript(hWrite, "GP", m_iGP, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CReward::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CReward *pReward = new CReward;
    *pReward = *(CReward*)this;
    *pDest = (CBaseEventAction*)pReward;
}

HRESULT CReward::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("EXP", m_iEXP, return E_FAIL);
    LUA_GET_VALUE("GP", m_iGP, return E_FAIL);

    return S_OK;
}