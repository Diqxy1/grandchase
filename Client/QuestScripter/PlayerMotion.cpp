#include "StdAfx.h"
#include ".\playermotion.h"

CPlayerMotion::CPlayerMotion(void)
{
    Init();
}

CPlayerMotion::~CPlayerMotion(void)
{
}

void CPlayerMotion::Init(void)
{
    m_enType        = TG_EACTION_PLAYERMOTION;
    m_iSlot         = 0;
    m_iMotion       = 0;
}

void CPlayerMotion::SetValue(int iSlot, int iMotion)
{
    m_iSlot = iSlot;
    m_iMotion = iMotion;
}

void CPlayerMotion::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGPlayerMotion", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "Motion", m_iMotion, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CPlayerMotion::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CPlayerMotion *pPlayerMotion = new CPlayerMotion;
    *pPlayerMotion = *(CPlayerMotion*)this;
    *pDest = (CBaseEventAction*)pPlayerMotion;
}

HRESULT CPlayerMotion::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("Motion", m_iMotion, return E_FAIL);

    return S_OK;
}