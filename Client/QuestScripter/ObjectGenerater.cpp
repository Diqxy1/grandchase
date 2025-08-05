#include "StdAfx.h"
#include ".\objectgenerater.h"

CObjectGenerater::CObjectGenerater(void)
{
    Init();
}

CObjectGenerater::~CObjectGenerater(void)
{
}

void CObjectGenerater::Init(void)
{
    m_enType            = TG_EACTION_OBJECTGENERATER;
    m_bOnOff            = false;
    m_bRight            = true;
    m_bGenerate         = true;
    m_bCrashAble        = false;
    m_iSlot             = 0;
    m_iType             = 0;
    m_fEntranceX        = 0.0f;
    m_fEntranceY        = 0.0f;
}

void CObjectGenerater::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGObjectGenerater", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "On", m_bOnOff, iTab+1);
    WriteToFileScript(hWrite, "Right", m_bRight, iTab+1);
    WriteToFileScript(hWrite, "Generate", m_bGenerate, iTab+1);
    WriteToFileScript(hWrite, "CrashAble", m_bCrashAble, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "Type", m_iType, iTab+1);
    WriteToFileScript(hWrite, "EntranceX", m_fEntranceX, iTab+1);
    WriteToFileScript(hWrite, "EntranceY", m_fEntranceY, iTab+1);
	WriteToFileScript(hWrite, "Property", m_fProperty, iTab+1);
	
    WriteCloseBrace(hWrite, iTab, 0);
}

void CObjectGenerater::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CObjectGenerater *pObjectGenerater = new CObjectGenerater;
    *pObjectGenerater = *(CObjectGenerater*)this;
    *pDest = (CBaseEventAction*)pObjectGenerater;
}

HRESULT CObjectGenerater::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    LUA_GET_VALUE("Right", m_bRight, return E_FAIL);
    LUA_GET_VALUE("Generate", m_bGenerate, return E_FAIL);
    luaMgr.GetValue( "CrashAble", m_bCrashAble );
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("Type", m_iType, return E_FAIL);
    LUA_GET_VALUE_DEF("EntranceX", m_fEntranceX,0);
    LUA_GET_VALUE_DEF("EntranceY", m_fEntranceY, 0);
	LUA_GET_VALUE_DEF("Property", m_fProperty, 0.08);

    return S_OK;
}
