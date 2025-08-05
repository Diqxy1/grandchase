#include "StdAfx.h"
#include ".\dialogue.h"

CDialogue::CDialogue(void)
{
    Init();
}

CDialogue::~CDialogue(void)
{
}

void CDialogue::Init(void)
{
    m_enType        = TG_EACTION_DIALOGUE;
    m_strDialogue   = L"";
    m_bOnOff        = 0;
    m_iChar         = -1;
}

void CDialogue::SetValue(std::wstring strDialogue, bool bOnOff, int iChar)
{
    m_strDialogue = strDialogue;
    m_bOnOff = bOnOff;
    m_iChar = iChar;
}

void CDialogue::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGDialogue", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Dialogue", m_strDialogue, iTab+1);
    WriteToFileScript(hWrite, "OnOff", m_bOnOff, iTab+1);
    WriteToFileScript(hWrite, "SHOW_CHAR", m_iChar, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CDialogue::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CDialogue *pDialogue = new CDialogue;
    *pDialogue = *(CDialogue*)this;
    *pDest = (CBaseEventAction*)pDialogue;
}

HRESULT CDialogue::LoadFromFile_Item(KLuaManager &luaMgr)
{
    std::string strDialogue;

    LUA_GET_VALUE("Dialogue", strDialogue, return E_FAIL);
    LUA_GET_VALUE("OnOff", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE_DEF("SHOW_CHAR", m_iChar, -1);
    

    m_strDialogue = GCUTIL_STR::GCStrCharToWide( strDialogue.c_str() );

    return S_OK;
}