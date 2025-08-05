#ifndef _GCROOMANIMATIONMANAGER_H_
#define _GCROOMANIMATIONMANAGER_H_
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

struct RoomAnimationExtra
{
    std::string m_sExtraModelM;
    std::string m_sExtraModelF;
    std::string m_sExtraTextureM;
    std::string m_sExtraTextureF;
    std::string m_sExtraAnimationM;
    std::string m_sExtraAnimationF;
    float m_fExtraX;
    float m_fExtraY;
    float m_fExtraZ;
    float m_fExtraScale;
    bool m_bIsLoop;

    RoomAnimationExtra():
        m_sExtraModelM("")
      , m_sExtraModelF("")
      , m_sExtraTextureM("")
      , m_sExtraTextureF("")
      , m_sExtraAnimationM("")
      , m_sExtraAnimationF("")
      , m_fExtraX(0)
      , m_fExtraY(0)
      , m_fExtraZ(0)
      , m_fExtraScale(0)
      , m_bIsLoop(false)
    {}

    bool operator != ( RoomAnimationExtra const& obj )
    {
        return (
            m_sExtraModelM != obj.m_sExtraModelM &&
            m_sExtraModelF != obj.m_sExtraModelF &&
            m_sExtraTextureM != obj.m_sExtraTextureM &&
            m_sExtraTextureF != obj.m_sExtraTextureF &&
            m_sExtraAnimationM != obj.m_sExtraAnimationM &&
            m_sExtraAnimationF != obj.m_sExtraAnimationF &&
            m_fExtraX != obj.m_fExtraX &&
            m_fExtraY != obj.m_fExtraY &&
            m_fExtraZ != obj.m_fExtraZ &&
            m_fExtraScale != obj.m_fExtraScale &&
            m_bIsLoop != obj.m_bIsLoop
        );
    }
};

struct RoomAnimationInfo 
{
    GCITEMID m_iItemID;
    int m_iMotion;
    int m_iModel;
    int m_iExtra;
    int m_iType;
    bool m_bIsBuy;
    RoomAnimationExtra m_pkExtraRoomInfo;

    RoomAnimationInfo():
        m_iItemID(-1)
      , m_iModel(-1)
      , m_iMotion(-1)
      , m_iExtra(-1)
      , m_iType(-1)
      , m_bIsBuy(-1)
      , m_pkExtraRoomInfo()
    {}

    bool operator != ( RoomAnimationInfo const& obj )
    {
        return (
            m_iItemID != obj.m_iItemID &&
            m_iModel != obj.m_iModel &&
            m_iMotion != obj.m_iMotion &&
            m_iExtra != obj.m_iExtra &&
            m_iType != obj.m_iType &&
            m_bIsBuy != obj.m_bIsBuy &&
            m_pkExtraRoomInfo != obj.m_pkExtraRoomInfo
        );
    }
};

class KGCRoomAnimationManager
{
public:
    KGCRoomAnimationManager();
    ~KGCRoomAnimationManager();
    int iPlayerID;
private:
    std::vector< RoomAnimationInfo > m_vecRoomAnimationList;
public:
    void SetRoomAnimationList( void );
    RoomAnimationInfo GetCurrentRoomAnimation( GCITEMID m_iItemID );
    RoomAnimationInfo GetRoomAnimation( int iPlayerID );
    bool EquipRoomAnimation( SCharInfo* pCharInfo, int iItemID );
    void UnEquipRoomAnimation( SCharInfo* pCharInfo, bool bUpdate = true );
    std::vector< RoomAnimationInfo > GetRoomAnimationList(){ return m_vecRoomAnimationList; }
};
extern KGCRoomAnimationManager* g_pRoomAnimationMgr;
#endif