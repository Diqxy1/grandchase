#include "StdAfx.h"
#include "CheckHP.h"

CCheckHP::CCheckHP(void)
{
	Init();
}

CCheckHP::~CCheckHP(void)
{
}

void CCheckHP::Init( void )
{
	m_enType = TG_CONDITION_HP_CHANGE;

	m_iIndex = 0;
	m_iType = 0;
	m_fPercent = 1.0f;
}

void CCheckHP::SetValue( int iIndex, int iType, float fPercent )
{
	m_iIndex = iIndex;
	m_iType = iType;
	m_fPercent = fPercent;
}

void CCheckHP::SaveToFile( HANDLE hWrite, int iTab, int iCnt, int &iTG_ID )
{
	CString strBraceTitle;
	strBraceTitle.Format(L"Function%d", iCnt);
	WriteOpenBrace(hWrite, strBraceTitle, iTab);    
	WriteToFileScript(hWrite, "FuctionName", "TGCheckHP", iTab+1);
	WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
	WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
	WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
	WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
	WriteToFileScript(hWrite, "ObjectIndex", m_iIndex, iTab+1);
	WriteToFileScript(hWrite, "HpPercent", m_fPercent, iTab+1);
	WriteToFileScript(hWrite, "ObjectType", m_iType, iTab+1);
	WriteCloseBrace(hWrite, iTab, 0);
}

void CCheckHP::CopyConditionToDest( CBaseCondition **pDest )
{
	CCheckHP *pKillMaster = new CCheckHP;
	*pKillMaster = *(CCheckHP*)this;
	*pDest = (CBaseCondition*)pKillMaster;
}

HRESULT CCheckHP::LoadFromFile_Item( KLuaManager &luaMgr )
{
	LUA_GET_VALUE( "ObjectIndex", m_iIndex, return E_FAIL );
	LUA_GET_VALUE( "HpPercent", m_fPercent, return E_FAIL );
	LUA_GET_VALUE( "ObjectType", m_iType, return E_FAIL );

	return S_OK;
}