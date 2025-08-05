#include "StdAfx.h"
#include ".\hp.h"

CHP::CHP(void)
{
    Init();
}

CHP::~CHP(void)
{
}

void CHP::Init(void)
{
    m_enType        = TG_EACTION_HP;
    m_bPlayer       = 0;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
    m_fHp           = 0;
}

void CHP::SetValue(bool bPlayer, int iSlot, int iMonsterID, float fHp)
{
    m_bPlayer       = bPlayer;
    m_iSlot         = iSlot;
    m_iMonsterID    = iMonsterID;
    m_fHp           = fHp;
}

void CHP::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGHp", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "IsPlayer", m_bPlayer, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "MonsterID", m_iMonsterID, iTab+1);
    WriteToFileScript(hWrite, "HP", m_fHp, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CHP::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CHP *pHP = new CHP;
    *pHP = *(CHP*)this;
    *pDest = (CBaseEventAction*)pHP;
}

HRESULT CHP::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);
    LUA_GET_VALUE("HP", m_fHp, return E_FAIL);

    return S_OK;
}