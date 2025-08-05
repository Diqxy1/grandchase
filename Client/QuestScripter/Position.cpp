#include "stdafx.h"
#include "Position.h"

void CPosition::Init(void)
{
    m_enType    = TG_CONDITION_POSITION;
    m_bInOut    = true;
    m_bPlayer   = true;
    m_bHalfMore = true;
    m_iIndex    = 0;
    m_iType     = 0;
    m_rtBound.left = 0, m_rtBound.right = 0;
    m_rtBound.bottom = 0, m_rtBound.top = 0;
}

void CPosition::SetValue(bool bInOut, bool bPlayer, bool bHalfMore, int iIndex, int iType, RECT rtBound)
{
    m_bInOut        = bInOut;
    m_rtBound       = rtBound;
    m_bHalfMore     = bHalfMore;
    m_bPlayer       = bPlayer;
    m_iIndex        = iIndex;
    m_iType         = iType;
}

void CPosition::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGPosition", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "InOut", m_bInOut, iTab+1);
    WriteToFileScript(hWrite, "Left", m_rtBound.left, iTab+1);
    WriteToFileScript(hWrite, "Right", m_rtBound.right, iTab+1);
    WriteToFileScript(hWrite, "Top", m_rtBound.top, iTab+1);
    WriteToFileScript(hWrite, "Bottom", m_rtBound.bottom, iTab+1);
    WriteToFileScript(hWrite, "IsPlayer", m_bPlayer, iTab+1);
    WriteToFileScript(hWrite, "HalfMore", m_bHalfMore, iTab+1);    
    WriteToFileScript(hWrite, "Index", m_iIndex, iTab+1);
    WriteToFileScript(hWrite, "Type", m_iType, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CPosition::CopyConditionToDest(CBaseCondition **pDest)
{
    CPosition *pPosition = new CPosition;
    *pPosition = *(CPosition*)this;
    *pDest = (CBaseCondition*)pPosition;
}

HRESULT CPosition::LoadFromFile_Item(KLuaManager &luaMgr)
{
    luaMgr.GetValue( "InOut", m_bInOut );
    LUA_GET_VALUE("Left", m_rtBound.left, return E_FAIL);
    LUA_GET_VALUE("Right", m_rtBound.right, return E_FAIL);
    LUA_GET_VALUE("Top", m_rtBound.top, return E_FAIL);
    LUA_GET_VALUE("Bottom", m_rtBound.bottom, return E_FAIL);
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("HalfMore", m_bHalfMore, return E_FAIL);
    LUA_GET_VALUE("Index", m_iIndex, return E_FAIL);
    LUA_GET_VALUE("Type", m_iType, return E_FAIL);

    return S_OK;
}