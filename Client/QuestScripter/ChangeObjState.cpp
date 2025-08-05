#include "StdAfx.h"
#include ".\changeobjstate.h"

void CChangeObjState::Init(void)
{
    m_enType = TG_EACTION_CHANGE_OBJ_STATE;
}

void CChangeObjState::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGWin", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "Type", m_iType, iTab+1);
	WriteToFileScript(hWrite, "State", m_iState, iTab+1);
	WriteToFileScript(hWrite, "OBJType", m_iOBJType, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CChangeObjState::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CChangeObjState *pChangeObjState = new CChangeObjState;
    *pChangeObjState = *(CChangeObjState*)this;
    *pDest = (CBaseEventAction*)pChangeObjState;
}

HRESULT CChangeObjState::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("Type", m_iType, return E_FAIL);
	LUA_GET_VALUE("State", m_iState, return E_FAIL);
	LUA_GET_VALUE_DEF("OBJType", m_iOBJType, 0);

    return S_OK;
}