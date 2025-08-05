#include "StdAfx.h"
#include "CheckWall.h"

CCheckWall::CCheckWall(void)
{
    Init();
}

CCheckWall::~CCheckWall(void)
{
}

void CCheckWall::Init( void )
{
    m_enType = TG_CONDITION_WALL;

    m_iSlot = 0;
    m_bLive = false;
}

void CCheckWall::SetValue( int iSlot, bool bLive )
{
    m_iSlot = iSlot;
    m_bLive = bLive;
}

void CCheckWall::SaveToFile( HANDLE hWrite, int iTab, int iCnt, int &iTG_ID )
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGCheckHP", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "WallSlot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "IsLive", m_bLive, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CCheckWall::CopyConditionToDest( CBaseCondition **pDest )
{
    CCheckWall *pKillMaster = new CCheckWall;
    *pKillMaster = *(CCheckWall*)this;
    *pDest = (CBaseCondition*)pKillMaster;
}

HRESULT CCheckWall::LoadFromFile_Item( KLuaManager &luaMgr )
{
    LUA_GET_VALUE( "WallSlot", m_iSlot, return E_FAIL );
    LUA_GET_VALUE( "IsLive", m_bLive, return E_FAIL );

    return S_OK;
}