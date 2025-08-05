#include "StdAfx.h"
#include ".\superplayer.h"

CSuperPlayer::CSuperPlayer(void)
{
    Init();
}

CSuperPlayer::~CSuperPlayer(void)
{
}

void CSuperPlayer::Init(void)
{
    m_enType = TG_EACTION_SUPERPLAYER;
    m_bOnOff = 1;
    m_iPlayerIndex = 0;
}

void CSuperPlayer::SetValue(int iPlayerIndex)
{
    m_iPlayerIndex = iPlayerIndex;
}

void CSuperPlayer::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "SuperPlayer", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "On", m_bOnOff, iTab+1);    
    WriteToFileScript(hWrite, "PlayerIndex", m_iPlayerIndex, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CSuperPlayer::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CSuperPlayer *pSuperPlayer = new CSuperPlayer;
    *pSuperPlayer = *(CSuperPlayer*)this;
    *pDest = (CBaseEventAction*)pSuperPlayer;
}

HRESULT CSuperPlayer::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("PlayerIndex", m_iPlayerIndex, return E_FAIL);

    return S_OK;
}