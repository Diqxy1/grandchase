#include "StdAfx.h"
#include ".\fadeinout.h"

CFadeInOut::CFadeInOut(void)
{
    Init();
}

CFadeInOut::~CFadeInOut(void)
{
}

void CFadeInOut::Init(void)
{
    m_enType        = TG_EACTION_FADE_INOUT;
    m_bOnOff        = 0;
	m_iSpeed		= 9;
	m_Red			= 0xff;
	m_Green			= 0xff;
	m_Blue			= 0xff;
}

void CFadeInOut::SetValue( int iSpeed, bool bOnOff, int iRed, int iGreen, int iBlue )
{
	 m_bOnOff = bOnOff;
	 m_iSpeed = iSpeed;
	 m_Red = iRed;
	 m_Green = iGreen;
	 m_Blue = iBlue;
}

void CFadeInOut::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGFadeInOut", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Speed", m_iSpeed, iTab+1);
	WriteToFileScript(hWrite, "Red", m_Red, iTab+1);
	WriteToFileScript(hWrite, "Green", m_Green, iTab+1);
	WriteToFileScript(hWrite, "Blue", m_Blue, iTab+1);
    WriteToFileScript(hWrite, "OnOff", m_bOnOff, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CFadeInOut::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CFadeInOut *pFadeInOut = new CFadeInOut;
    *pFadeInOut = *(CFadeInOut*)this;
    *pDest = (CBaseEventAction*)pFadeInOut;
}

HRESULT CFadeInOut::LoadFromFile_Item(KLuaManager &luaMgr)
{
	LUA_GET_VALUE( "Speed", m_iSpeed, return E_FAIL );
	LUA_GET_VALUE( "Red", m_Red, return E_FAIL );
	LUA_GET_VALUE( "Green", m_Green, return E_FAIL );
	LUA_GET_VALUE( "Blue", m_Blue, return E_FAIL );
    LUA_GET_VALUE("OnOff", m_bOnOff, return E_FAIL);

    return S_OK;
}