#include "StdAfx.h"
#include ".\mp.h"

CMP::CMP(void)
{
    Init();
}

CMP::~CMP(void)
{
}

void CMP::Init(void)
{
    m_enType        = TG_EACTION_MP;
    m_bPlayer       = 0;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
    m_fMp           = 0;
}

void CMP::SetValue(bool bPlayer, int iSlot, int iMonsterID, float fMp)
{
    m_bPlayer       = bPlayer;
    m_iSlot         = iSlot;
    m_iMonsterID    = iMonsterID;
    m_fMp           = fMp;
}

void CMP::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGMp", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "IsPlayer", m_bPlayer, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "MonsterID", m_iMonsterID, iTab+1);
    WriteToFileScript(hWrite, "MP", m_fMp, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CMP::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CMP *pMP = new CMP;
    *pMP = *(CMP*)this;
    *pDest = (CBaseEventAction*)pMP;
}

HRESULT CMP::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);
    LUA_GET_VALUE("MP", m_fMp, return E_FAIL);

    return S_OK;
}