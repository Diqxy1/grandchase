#include "stdafx.h"
#include ".\dialoguestate.h"

void CDialogueState::Init(void)
{
    m_enType = TG_CONDITION_DIALOGUE_STATE;
    m_bOnOff = true;
}

void CDialogueState::SetValue(bool bOnOff)
{
    m_bOnOff = bOnOff;
}

void CDialogueState::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGTimer", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "OnOff", (bool)m_bOnOff, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CDialogueState::CopyConditionToDest(CBaseCondition **pDest)
{
    CDialogueState *pDialogueState = new CDialogueState;
    *pDialogueState = *(CDialogueState*)this;
    *pDest = (CBaseCondition*)pDialogueState;
}

HRESULT CDialogueState::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE_DEF("OnOff", m_bOnOff, true);

    return S_OK;
}