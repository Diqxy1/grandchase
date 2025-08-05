#include "stdafx.h"
#include "MonsterGenerater.h"

void CMonsterGenerater::Init(void)
{
    m_enType        = TG_EACTION_MON_GENNER;
    m_iMonIndex     = -1;
    m_iMonType      = 0;
    m_iMonLevel     = 0;
    m_bRight        = false;
    m_iBoss         = 0;
    m_fEntranceX    = 0;
    m_fEntranceY    = 0;
	m_bNotChampion = 0;
    m_bRandom   = false;
    m_iPercent = 100.0f;
}

void CMonsterGenerater::SetValue( bool bRight, int iBoss, int iMonIndex, int iMonType, int iMonLevel, int iX, int iY, bool bChamp_ /*= false*/, bool bRandom /*= false*/, int iPercent /*= 100 */ )
{
    m_bRight        = bRight;
    m_iBoss         = iBoss;
    m_iMonIndex     = iMonIndex;
    m_iMonType      = iMonType;
    m_iMonLevel     = iMonLevel;
    m_fEntranceX    = (float)iX;
    m_fEntranceY    = (float)iY;
	m_bNotChampion = bChamp_;
    m_bRandom   = false;
    m_iPercent = 100.0f;
}

void CMonsterGenerater::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGMonsterGenerater", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Right", m_bRight, iTab+1);
    WriteToFileScript(hWrite, "Boss", m_iBoss, iTab+1);
    WriteToFileScript(hWrite, "Index", m_iMonIndex, iTab+1);
    WriteToFileScript(hWrite, "Type", m_iMonType, iTab+1);
    WriteToFileScript(hWrite, "Level", m_iMonLevel, iTab+1);
    WriteToFileScript(hWrite, "PROBABILITY", m_iPercent, iTab+1);
    WriteToFileScript(hWrite, "RANDOM", m_bRandom, iTab+1);
    WriteToFileScript(hWrite, "EntranceX", m_fEntranceX, iTab+1);
	WriteToFileScript(hWrite, "EntranceY", m_fEntranceY, iTab+1);
	WriteToFileScript(hWrite, "NotCampion", m_bNotChampion, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CMonsterGenerater::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CMonsterGenerater *pMonsterGen = new CMonsterGenerater;
    *pMonsterGen = *(CMonsterGenerater*)this;
    *pDest = (CBaseEventAction*)pMonsterGen;
}

HRESULT CMonsterGenerater::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Right", m_bRight, return E_FAIL);
    LUA_GET_VALUE("Boss", m_iBoss, return E_FAIL);
    LUA_GET_VALUE("Index", m_iMonIndex, return E_FAIL);
    LUA_GET_VALUE("Type", m_iMonType, return E_FAIL);
    LUA_GET_VALUE_DEF("Level", m_iMonLevel, -1);
    LUA_GET_VALUE_DEF("PROBABILITY", m_iPercent, 100);
    LUA_GET_VALUE_DEF("RANDOM", m_bRandom, false);
    LUA_GET_VALUE("EntranceX", m_fEntranceX, return E_FAIL);
	LUA_GET_VALUE("EntranceY", m_fEntranceY, return E_FAIL);
	LUA_GET_VALUE_NOASSERT("NotCampion", m_bNotChampion, return E_FAIL);

    return S_OK;
}
