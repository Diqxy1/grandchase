#include "StdAfx.h"
#include ".\Wall.h"

void CWall::Init(void)
{
    m_enType            = TG_EACTION_WALL;

    m_bDestroy = false;
    m_bIsCheckInBound = false;
    m_iWallType = 0;
    m_iWallSlot = 0;
    m_fPosX = 0.0f;
}

void CWall::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);
    WriteToFileScript(hWrite, "FuctionName", "TGWall", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "WallType", m_iWallType, iTab+1);
    WriteToFileScript(hWrite, "WallSlot", m_iWallSlot, iTab+1);
    WriteToFileScript(hWrite, "EntranceX", m_fPosX, iTab+1);
    WriteToFileScript(hWrite, "IsDestroy", m_bDestroy, iTab+1);
    WriteToFileScript(hWrite, "IsCheckInBound", m_bIsCheckInBound, iTab+1);
    

    WriteCloseBrace(hWrite, iTab, 0);
}

void CWall::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CWall *pWall = new CWall;
    *pWall = *(CWall*)this;
    *pDest = (CBaseEventAction*)pWall;
}

HRESULT CWall::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("WallType", m_iWallType, return E_FAIL);
    LUA_GET_VALUE("WallSlot", m_iWallSlot, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_fPosX, return E_FAIL);
    LUA_GET_VALUE("IsDestroy", m_bDestroy, return E_FAIL);
    LUA_GET_VALUE("IsCheckInBound", m_bIsCheckInBound, return E_FAIL);
    
    
    return S_OK;
}
