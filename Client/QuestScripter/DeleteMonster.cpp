#include "StdAfx.h"
#include ".\deletemonster.h"

CDeleteMonster::CDeleteMonster(void)
{
    Init();
}

CDeleteMonster::~CDeleteMonster(void)
{
}

void CDeleteMonster::Init(void)
{
    m_enType        = TG_EACTION_DELETEMONSTER;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
    m_bAllSlot      = false;
}

void CDeleteMonster::SetValue(bool bAllSlot, int iSlot, bool iMonsterID)
{
    m_bAllSlot      = bAllSlot;
    m_iSlot         = iSlot;
    m_iMonsterID    = iMonsterID;
}

void CDeleteMonster::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGDeleteMonster", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "AllSlot", m_bAllSlot, iTab+1);    
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "MonsterID", m_iMonsterID, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CDeleteMonster::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CDeleteMonster *pDeleteMonster = new CDeleteMonster;
    *pDeleteMonster = *(CDeleteMonster*)this;
    *pDest = (CBaseEventAction*)pDeleteMonster;
}

HRESULT CDeleteMonster::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE_DEF("AllSlot", m_bAllSlot, false);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);

    return S_OK;
}