#include "StdAfx.h"
#include ".\unitmovement.h"

CUnitMovement::CUnitMovement(void)
{
    Init();
}

CUnitMovement::~CUnitMovement(void)
{
}

void CUnitMovement::Init(void)
{
    m_enType        = TG_EACTION_UNITMOVEMENT;
    m_bPlayer       = 0;
    m_bOnOff        = 0;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
}

void CUnitMovement::SetValue(bool bPlayer, bool bOnOff, int iSlot, int iMonsterID)
{
    m_bPlayer       = bPlayer;
    m_bOnOff        = bOnOff;
    m_iSlot         = iSlot;
    m_iMonsterID    = iMonsterID;
}

void CUnitMovement::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGUnitMovement", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "IsPlayer", m_bPlayer, iTab+1);
    WriteToFileScript(hWrite, "OnOff", m_bOnOff, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "MonsterID", m_iMonsterID, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CUnitMovement::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CUnitMovement *pUnitMovement = new CUnitMovement;
    *pUnitMovement = *(CUnitMovement*)this;
    *pDest = (CBaseEventAction*)pUnitMovement;
}

HRESULT CUnitMovement::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("OnOff", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);

    return S_OK;
}