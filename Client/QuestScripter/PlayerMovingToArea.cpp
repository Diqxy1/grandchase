#include "StdAfx.h"
#include ".\playermovingtoarea.h"

CPlayerMovingToArea::CPlayerMovingToArea(void)
{
    Init();
}

CPlayerMovingToArea::~CPlayerMovingToArea(void)
{
}

void CPlayerMovingToArea::Init(void)
{
    m_enType        = TG_EACTION_PLAYERMOVINGTOAREA;
    m_bPlayer       = 0;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
    m_iLeft         = 0;
    m_iRight        = 0;
    m_iTop          = 0;
    m_iBottom       = 0;
}

void CPlayerMovingToArea::SetValue(bool bPlayer, int iSlot, int iMonsterID, int iLeft, int iRight, int iTop, int iBottom)
{
    m_bPlayer       = bPlayer;
    m_iSlot         = iSlot;
    m_iMonsterID    = iMonsterID;
    m_iLeft         = iLeft;
    m_iRight        = iRight;
    m_iTop          = iTop;
    m_iBottom       = iBottom;
}

void CPlayerMovingToArea::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGPlayerMovingToArea", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "IsPlayer", m_bPlayer, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "MonsterID", m_iMonsterID, iTab+1);
    WriteToFileScript(hWrite, "Left", m_iLeft, iTab+1);
    WriteToFileScript(hWrite, "Right", m_iRight, iTab+1);
    WriteToFileScript(hWrite, "Top", m_iTop, iTab+1);
    WriteToFileScript(hWrite, "Bottom", m_iBottom, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CPlayerMovingToArea::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CPlayerMovingToArea *pUnitMovement = new CPlayerMovingToArea;
    *pUnitMovement = *(CPlayerMovingToArea*)this;
    *pDest = (CBaseEventAction*)pUnitMovement;
}

HRESULT CPlayerMovingToArea::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);
    LUA_GET_VALUE("Left", m_iLeft, return E_FAIL);
    LUA_GET_VALUE("Right", m_iRight, return E_FAIL);
    LUA_GET_VALUE("Top", m_iTop, return E_FAIL);
    LUA_GET_VALUE("Bottom", m_iBottom, return E_FAIL);

    return S_OK;
}