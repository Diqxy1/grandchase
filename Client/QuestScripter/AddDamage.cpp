#include "stdafx.h"
#include ".\AddDamage.h"

void CAddDamage::Init(void)
{
    m_enType        = TG_EACTION_ADD_DAMAGE;
    m_iDamageID     = 0;
    m_iEntranceX    = 0;
    m_iEntranceY    = 0;
    m_dwRecycleTime = 6000;

    // Options - Use Only WaterPole.
    m_iLife         = 500;
    m_iMySpeed      = 0;
    m_iStartTime    = 500;
    m_iJumpFrame    = 0;
    m_iCrashStart   = 500;
    m_iContinueance = 3000;

	m_iDir	= 0;
}

void CAddDamage::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGAddDamage", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "DamageID", m_iDamageID, iTab+1);
    WriteToFileScript(hWrite, "EntranceX", m_iEntranceX, iTab+1);
    WriteToFileScript(hWrite, "EntranceY", m_iEntranceY, iTab+1);
    WriteToFileScript(hWrite, "RecycleTime", (int)m_dwRecycleTime, iTab+1);
    WriteToFileScript(hWrite, "Life", m_iLife, iTab+1);    
    WriteToFileScript(hWrite, "StartTime", m_iStartTime, iTab+1);    
    WriteToFileScript(hWrite, "Continueance", m_iContinueance, iTab+1);
	WriteToFileScript(hWrite, "Dir", m_iDir, iTab+1);   
	
	//WriteToFileScript(hWrite, "XSpeed", m_iXSpeed, iTab+1);    
	//WriteToFileScript(hWrite, "YSpeed", m_iYSpeed, iTab+1);    
    WriteCloseBrace(hWrite, iTab, 0);
}

void CAddDamage::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CAddDamage *pAddDamage = new CAddDamage;
    *pAddDamage = *(CAddDamage*)this;
    *pDest = (CBaseEventAction*)pAddDamage;
}

HRESULT CAddDamage::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("DamageID", m_iDamageID, return E_FAIL);
    LUA_GET_VALUE("EntranceX", m_iEntranceX, return E_FAIL);
    LUA_GET_VALUE("EntranceY", m_iEntranceY, return E_FAIL);
    LUA_GET_VALUE_DEF("RecycleTime", m_dwRecycleTime, 6000);
    LUA_GET_VALUE_DEF("Life", m_iLife, 500);    
    LUA_GET_VALUE_DEF("StartTime", m_iStartTime, 0);    
    LUA_GET_VALUE_DEF("Continueance", m_iContinueance, 3000);	
	LUA_GET_VALUE_DEF("Dir", m_iDir,0);	
    return S_OK;
}
