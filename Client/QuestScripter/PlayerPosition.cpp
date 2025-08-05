#include "StdAfx.h"
#include ".\playerposition.h"

CPlayerPosition::CPlayerPosition(void)
{
    Init();
}

CPlayerPosition::~CPlayerPosition(void)
{
}

void CPlayerPosition::Init(void)
{
    m_enType        = TG_EACTION_PLAYERPOSITION;
    m_bRight        = false;
    m_iPlayerIndex  = 0;
    m_fEntranceX    = 0.0f;
    m_fEntranceY    = 0.0f;
}

void CPlayerPosition::SetValue(int iPlayerIndex, float fEntranceX, float fEntranceY)
{
    m_iPlayerIndex  = iPlayerIndex;
    m_fEntranceX    = fEntranceX;
    m_fEntranceY    = fEntranceY;
}

void CPlayerPosition::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGPlayerPosition", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Right", m_bRight, iTab+1);
    WriteToFileScript(hWrite, "PlayerIndex", m_iPlayerIndex, iTab+1);
    WriteToFileScript(hWrite, "EntranceX", m_fEntranceX, iTab+1);
    WriteToFileScript(hWrite, "EntranceY", m_fEntranceY, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CPlayerPosition::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CPlayerPosition *pPlayerPosition = new CPlayerPosition;
    *pPlayerPosition = *(CPlayerPosition*)this;
    *pDest = (CBaseEventAction*)pPlayerPosition;
}

HRESULT CPlayerPosition::LoadFromFile_Item(KLuaManager &luaMgr)
{
    luaMgr.GetValue( "Right", m_bRight );
    LUA_GET_VALUE("PlayerIndex", m_iPlayerIndex, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_fEntranceX, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_fEntranceY, return E_FAIL);

    return S_OK;

}