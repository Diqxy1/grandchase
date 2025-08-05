#ifndef _GCFONTSKINMANAGER_H_
#define _GCFONTSKINMANAGER_H_
#if _MSC_VER > 1000
#pragma once
#endif
#pragma warning(disable:4786)

#include "GCItemEnum.h"
#include "ProtectedType.h"
#include <atltime.h>
#include "KGCStringID.h"
#include "ItemContainer.h"

struct SCharInfo;

struct FontSkinInfo 
{
    bool m_bIsBuy;
    GCITEMID m_iItemID;
    int m_iType;
    std::string m_sFontParticle;
    std::string m_sFontExtraParticle;
    std::string m_sFontExtraNumberParticle;

    FontSkinInfo():
        m_bIsBuy(false)
      , m_iItemID(0)
      , m_iType(0)
      , m_sFontParticle("")
      , m_sFontExtraParticle("")
      , m_sFontExtraNumberParticle("")
    {}
};

class KGCFontSkinManager
{
public:
    KGCFontSkinManager();
    ~KGCFontSkinManager();

private:
    std::vector<FontSkinInfo> m_vecFontSkinList;

public:
    void SetFontSkinList( void );
    std::string GetFontSkinParticle( int iPlayerID );
    std::string GetExtraNumberFontSkinParticle( int iPlayerID );
    std::string GetExtraFontSkinParticle( int iPlayerID );
    FontSkinInfo GetCurrentFontSkin( GCITEMID m_iItemID );
    bool EquipFontSkin( SCharInfo* pCharInfo, int iItemID );
    void UnEquipFontskin( SCharInfo* pCharInfo, bool bUpdate = true );
    std::vector<FontSkinInfo> GetFontSkinList(){ return m_vecFontSkinList; }
};

extern KGCFontSkinManager* g_pFontSkinMgr;

#endif