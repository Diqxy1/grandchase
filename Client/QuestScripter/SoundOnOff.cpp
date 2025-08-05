#include "stdafx.h"
#include "SoundOnOff.h"

void CSoundOnOff::Init(void)
{
    m_enType = TG_EACTION_SOUND_ONOFF;
    m_bSoundOn = true;
    ZeroMemory(m_szSoundFileName, sizeof(m_szSoundFileName));
}

void CSoundOnOff::SetValue(bool bSoundOn, TCHAR *lpszSoundFileName)
{
    m_bSoundOn = bSoundOn;
    StringCchCopyN( m_szSoundFileName, sizeof( m_szSoundFileName ), lpszSoundFileName, _tcslen( lpszSoundFileName ) );
}

void CSoundOnOff::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGSoundOnOff", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "On", m_bSoundOn, iTab+1);
    WriteToFileScript(hWrite, "FileName", CString(m_szSoundFileName), iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CSoundOnOff::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CSoundOnOff *pSoundOnOff = new CSoundOnOff;
    *pSoundOnOff = *(CSoundOnOff*)this;
    *pDest = (CBaseEventAction*)pSoundOnOff;
}

HRESULT CSoundOnOff::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bSoundOn, return E_FAIL);

    CHAR szSoundFileName[MAX_TG_STR_LENGTH] = {0};

    if( FAILED(luaMgr.GetValue("FileName", szSoundFileName, sizeof(szSoundFileName))) ) 
        return E_FAIL;

    std::wstring strSoundFile = GCUTIL_STR::GCStrCharToWide( szSoundFileName );

    StringCchCopyN( m_szSoundFileName, sizeof( m_szSoundFileName ), strSoundFile.c_str(), strSoundFile.length() );

    return S_OK;
}