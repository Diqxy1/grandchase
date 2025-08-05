#include "StdAfx.h"
#include ".\objdestory.h"

void CObjDestory::Init(void)
{
    m_enType = TG_CONDITION_OBJ_DESTROY; m_iObjIndex = 0; m_iObjType = 0;
}

void CObjDestory::SetValue(int iObjIndex, int iObjType)
{
    m_iObjIndex = iObjIndex;
    m_iObjType = iObjType;
}

void CObjDestory::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGObjDestroy", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "ObjIndex", m_iObjIndex, iTab+1);
    WriteToFileScript(hWrite, "ObjType", m_iObjType, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CObjDestory::CopyConditionToDest(CBaseCondition **pDest)
{
    CObjDestory *pObjDestroy = new CObjDestory;
    *pObjDestroy = *(CObjDestory*)this;
    *pDest = (CBaseCondition*)pObjDestroy;
}

HRESULT CObjDestory::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("ObjIndex", m_iObjIndex, return E_FAIL);
    LUA_GET_VALUE("ObjType", m_iObjType, return E_FAIL);

    return S_OK;
}