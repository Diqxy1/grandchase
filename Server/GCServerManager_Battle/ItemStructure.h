#pragma once

#include "UserPacket.h"

//운영자가 id를 넣으면 아이템에 관한 정보를 모두 가져오는 구조체
struct KAdminUseItemInfo;
SERIALIZE_DEFINE_TAG(KAdminUseItemInfo, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(KAdminUseItemInfo);
struct KAdminUseItemInfo
{
    DWORD               m_dwGoodsID;
    std::wstring         m_strGoodsName;
    bool                m_bCash;
    int                 m_iStrongLevel;
    int                 m_iDurationNum;
    int                 m_iCharType;
    bool                m_bShowWeb;
    int                 m_iGem;
    int                 m_iCrystal;

    CON_COPYCON_ASSIGNOP( KAdminUseItemInfo, other )
    {
        m_dwGoodsID         = other.m_dwGoodsID;
        m_strGoodsName      = other.m_strGoodsName;
        m_bCash             = other.m_bCash;
        m_iStrongLevel      = other.m_iStrongLevel;
        m_iDurationNum      = other.m_iDurationNum;
        m_iCharType         = other.m_iCharType;
        m_bShowWeb          = other.m_bShowWeb;
        m_iGem              = other.m_iGem;
        m_iCrystal          = other.m_iCrystal;
        return *this;
    }
};

struct KSelectNormalItemInfo;
SERIALIZE_DEFINE_TAG(KSelectNormalItemInfo, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(KSelectNormalItemInfo);
struct KSelectNormalItemInfo
{
    std::wstring            m_strGoodsName;
    DWORD                   m_dwGoodsID;
    GCITEMUID               m_dwGoodsUID;
    std::wstring            m_strOwnerLogin;
    std::wstring            m_strBuyerLogin;
    time_t                  m_tRegDate;
    time_t                  m_tStartDate;
    time_t                  m_tEndDate;
    int                     m_iPeriod;
    time_t                  m_tDelDate;

    CON_COPYCON_ASSIGNOP(KSelectNormalItemInfo, other)
    {
        m_strGoodsName    = other.m_strGoodsName;
        m_dwGoodsID       = other.m_dwGoodsID;
        m_dwGoodsUID      = other.m_dwGoodsUID;
        m_strOwnerLogin   = other.m_strOwnerLogin;
        m_strBuyerLogin   = other.m_strBuyerLogin;
        m_tRegDate        = other.m_tRegDate;
        m_tStartDate      = other.m_tStartDate;
        m_tEndDate        = other.m_tEndDate;
        m_iPeriod         = other.m_iPeriod;
        m_tDelDate        = other.m_tDelDate;
        return *this;
    }
};


//select 수량성 아이템info
struct KSelectDurationItemInfo;
SERIALIZE_DEFINE_TAG(KSelectDurationItemInfo, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(KSelectDurationItemInfo);
struct KSelectDurationItemInfo
{
    std::wstring            m_strGoodsName;
    DWORD                   m_dwGoodsID;
    GCITEMUID               m_dwGoodsUID;
    std::wstring            m_strOwnerLogin;
    std::wstring            m_strBuyerLogin;
    int                     m_iDurationNum;
    time_t                  m_tRegDate;
    time_t                  m_tDelDate;

    CON_COPYCON_ASSIGNOP(KSelectDurationItemInfo, other)
    {
        m_strGoodsName    = other.m_strGoodsName;
        m_dwGoodsID       = other.m_dwGoodsID;
        m_dwGoodsUID      = other.m_dwGoodsUID;
        m_strOwnerLogin   = other.m_strOwnerLogin;
        m_strBuyerLogin   = other.m_strBuyerLogin;
        m_iDurationNum    = other.m_iDurationNum;
        m_tRegDate        = other.m_tRegDate;
        m_tDelDate        = other.m_tDelDate;
        return *this;
    }
};

struct KUserDBInfo;
SERIALIZE_DEFINE_TAG(KUserDBInfo, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(KUserDBInfo);
struct KUserDBInfo
{
    std::wstring         m_strLogin;
    int                 m_iExp0;
    int                 m_iExp1;
    int                 m_iExp2;
    int                 m_iGamePoint;
    char                m_cAuthLevel;
    int                 m_iBlessing0;
    int                 m_iBlessing1;
    int                 m_iBlessing2;

    CON_COPYCON_ASSIGNOP(KUserDBInfo, other)
    {
        m_strLogin      = other.m_strLogin;
        m_iExp0         = other.m_iExp0;
        m_iExp1         = other.m_iExp1;
        m_iExp2         = other.m_iExp2;
        m_iGamePoint    = other.m_iGamePoint;
        m_cAuthLevel    = other.m_cAuthLevel;
        m_iBlessing0    = other.m_iBlessing0;
        m_iBlessing1    = other.m_iBlessing1;
        m_iBlessing2    = other.m_iBlessing2;
        return *this;
    }
};