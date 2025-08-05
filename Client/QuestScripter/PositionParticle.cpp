#include "StdAfx.h"
#include ".\positionparticle.h"

CPositionParticle::CPositionParticle(void)
{
    Init();
}

CPositionParticle::~CPositionParticle(void)
{
}

void CPositionParticle::Init(void)
{
    m_enType        = TG_EACTION_POSITIONPARTICLE;
    m_iLeft         = 0;
    m_iRight        = 0;
    m_iTop          = 0;
    m_iBottom       = 0;
    m_iParticleID   = 0;
}

void CPositionParticle::SetValue(int iLeft, int iRight, int iTop, int iBottom, int iParticleID)
{
    m_iLeft         = iLeft;
    m_iRight        = iRight;
    m_iTop          = iTop;
    m_iBottom       = iBottom;
    m_iParticleID   = iParticleID;
}

void CPositionParticle::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGPositionParticle", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Left", m_iLeft, iTab+1);
    WriteToFileScript(hWrite, "Right", m_iRight, iTab+1);
    WriteToFileScript(hWrite, "Top", m_iTop, iTab+1);
    WriteToFileScript(hWrite, "Bottom", m_iBottom, iTab+1);
    WriteToFileScript(hWrite, "ParticleID", m_iParticleID, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CPositionParticle::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CPositionParticle *pPositionParticle = new CPositionParticle;
    *pPositionParticle = *(CPositionParticle*)this;
    *pDest = (CBaseEventAction*)pPositionParticle;
}

HRESULT CPositionParticle::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("Left", m_iLeft, return E_FAIL);
    LUA_GET_VALUE("Right", m_iRight, return E_FAIL);
    LUA_GET_VALUE("Top", m_iTop, return E_FAIL);
    LUA_GET_VALUE("Bottom", m_iBottom, return E_FAIL);
    LUA_GET_VALUE("ParticleID", m_iParticleID, return E_FAIL);

    return S_OK;
}