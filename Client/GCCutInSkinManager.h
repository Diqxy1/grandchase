#ifndef _GCCUTINSKINMANAGER_H_
#define _GCCUTINSKINMANAGER_H_
#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning(disable:4786)

#include "GCItemEnum.h"
#include "ProtectedType.h"
#include <atltime.h>
#include "KGCStringID.h"

struct CutInSkinInfo 
{
    GCITEMID m_iItemID;
    std::string m_sCutInSkinFace;
    int m_iType;
    bool m_bIsBuy;
};

class KGCCutInSkinManager
{
public:
    KGCCutInSkinManager();
    ~KGCCutInSkinManager();

private:
    std::vector<CutInSkinInfo> m_vecCutInSkinList;

public:
    void SetCutInSkinList( void );
    std::string GetCutInSkinTexture( const int iPlayerID );
    std::string GetCutInSkinTextureID( const int iItemID );
    bool EquipCutInSkin(SCharInfo* pCharInfo, int iItemID);
    CutInSkinInfo GetCurrentCutInSkin(GCITEMID m_iItemID);
    void UnEquipCutInskin(SCharInfo* pCharInfo, bool bUpdate);
    std::vector< CutInSkinInfo > GetCutInSkinList() { return m_vecCutInSkinList; }
};

extern KGCCutInSkinManager* g_pCutInSkinMgr;

#endif