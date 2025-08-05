#include "StdAfx.h"
#include ".\unitparticle.h"

CUnitParticle::CUnitParticle(void)
{
    Init();
}

CUnitParticle::~CUnitParticle(void)
{
}

void CUnitParticle::Init(void)
{
    m_enType        = TG_EACTION_UNIT_PARTICLE;
    m_bPlayer       = 0;
    m_iSlot         = 0;
    m_iMonsterID    = 0;
    m_iParticleID   = 0;
}

void CUnitParticle::SetValue(bool bPlayer, int iSlot, int iMonsterID, int iParticleID)
{
    m_bPlayer       = bPlayer;
    m_iSlot         = iSlot;
    m_iMonsterID    = iMonsterID;
    m_iParticleID   = iParticleID;
}

void CUnitParticle::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGUnitParticle", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "IsPlayer", m_bPlayer, iTab+1);
    WriteToFileScript(hWrite, "Slot", m_iSlot, iTab+1);
    WriteToFileScript(hWrite, "MonsterID", m_iMonsterID, iTab+1);
    WriteToFileScript(hWrite, "ParticleID", m_iParticleID, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CUnitParticle::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CUnitParticle *pUnitMovement = new CUnitParticle;
    *pUnitMovement = *(CUnitParticle*)this;
    *pDest = (CBaseEventAction*)pUnitMovement;
}

HRESULT CUnitParticle::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("IsPlayer", m_bPlayer, return E_FAIL);
    LUA_GET_VALUE("Slot", m_iSlot, return E_FAIL);
    LUA_GET_VALUE("MonsterID", m_iMonsterID, return E_FAIL);
    LUA_GET_VALUE("ParticleID", m_iParticleID, return E_FAIL);

    return S_OK;
}