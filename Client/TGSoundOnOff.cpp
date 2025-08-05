#include "StdAfx.h"
#include ".\TGSoundOnOff.h"
#include "KMci.h"
#include "MyD3D.h"


CTGSoundOnOff::CTGSoundOnOff(void)
{
    SetComplete( false );
    m_bSoundOn          = true;
    enType              = TG_SOUND_ONOFF;
    ZeroMemory(m_szSoundFileName, sizeof(m_szSoundFileName));
}

CTGSoundOnOff::~CTGSoundOnOff(void)
{
}

void CTGSoundOnOff::SetSoundOn(bool bSoundOn)
{
    m_bSoundOn = bSoundOn;
}

void CTGSoundOnOff::SetSoundFileName(char *lpszFileName)
{
    strncpy(m_szSoundFileName, lpszFileName, sizeof(m_szSoundFileName));
}

void CTGSoundOnOff::Sound(void)
{
    if( m_bSoundOn )
    {
        KShow::FileInfo    kfileinfo;
        kfileinfo.m_bwindow = 0;
        std::string strfilepath;
        strfilepath += g_strCurrentPath;
        strfilepath += "Data\\Music\\";
        strfilepath += m_szSoundFileName;
        ::strncpy( kfileinfo.m_strFile, strfilepath.c_str(), sizeof(kfileinfo.m_strFile) );

        g_KMci.Stop();
        HANDLE hFile = CreateFileA(strfilepath.c_str(), GENERIC_READ,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE)
        {
            hFile = NULL;
        }
        else
        {
            CloseHandle(hFile);
            g_KMci.AddMusic(kfileinfo);
        }
        g_KMci.Play(strfilepath.c_str());
    }
    else
    {
        g_KMci.Stop();
    }

    SetComplete( true );
}

void CTGSoundOnOff::FrameMove(void)
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost )
    {
        Sound();

        if( !m_bSendPacket )
        {
            // 패킷을 보낸다.
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }
    else if( m_bCheckByHost )
    {
        Sound();
    }
}

HRESULT CTGSoundOnOff::LoadItemFromLua(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bSoundOn, return E_FAIL);
    if( FAILED(luaMgr.GetValue("FileName", m_szSoundFileName, MAX_TG_STR_LENTH)) ) return E_FAIL;

    return S_OK;
}