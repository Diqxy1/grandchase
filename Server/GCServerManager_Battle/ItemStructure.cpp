#include "ItemStructure.h"


//운영자 id를 넣으면 아이템의 정보를 보기위한 구조체 
SERIALIZE_DEFINE_PUT(KAdminUseItemInfo, obj, ks)
{
    return  ks.Put(obj.m_dwGoodsID) &&
        ks.Put(obj.m_strGoodsName) &&
        ks.Put(obj.m_bCash) &&
        ks.Put(obj.m_iStrongLevel) &&
        ks.Put(obj.m_iDurationNum) &&
        ks.Put(obj.m_iCharType) &&
        ks.Put(obj.m_bShowWeb) &&
        ks.Put(obj.m_iGem) &&
        ks.Put(obj.m_iCrystal);

}
SERIALIZE_DEFINE_GET(KAdminUseItemInfo, obj, ks)
{
    return  ks.Get(obj.m_dwGoodsID) &&
        ks.Get(obj.m_strGoodsName) &&
        ks.Get(obj.m_bCash) &&
        ks.Get(obj.m_iStrongLevel) &&
        ks.Get(obj.m_iDurationNum) &&
        ks.Get(obj.m_iCharType) &&
        ks.Get(obj.m_bShowWeb) &&
        ks.Get(obj.m_iGem) &&
        ks.Get(obj.m_iCrystal);
}
//운영자 id를 넣으면 아이템의 정보를 보기위한 구조체 


SERIALIZE_DEFINE_PUT(KSelectNormalItemInfo, obj, ks)
{
    return  ks.Put(obj.m_strGoodsName) &&
        ks.Put(obj.m_dwGoodsID) &&
        ks.Put(obj.m_dwGoodsUID) &&
        ks.Put(obj.m_strOwnerLogin) &&
        ks.Put(obj.m_strBuyerLogin) &&
        ks.Put(static_cast<int>(obj.m_tRegDate)) &&
        ks.Put(static_cast<int>(obj.m_tStartDate)) &&
        ks.Put(static_cast<int>(obj.m_tEndDate)) &&
        ks.Put(obj.m_iPeriod) &&
        ks.Put(static_cast<int>(obj.m_tDelDate));

}
SERIALIZE_DEFINE_GET(KSelectNormalItemInfo, obj, ks)
{
    return  ks.Get(obj.m_strGoodsName) &&
        ks.Get(obj.m_dwGoodsID) &&
        ks.Get(obj.m_dwGoodsUID) &&
        ks.Get(obj.m_strOwnerLogin) &&
        ks.Get(obj.m_strBuyerLogin) &&
        ks.Get(reinterpret_cast<int&>(obj.m_tRegDate)) &&
        ks.Get(reinterpret_cast<int&>(obj.m_tStartDate)) &&
        ks.Get(reinterpret_cast<int&>(obj.m_tEndDate)) &&
        ks.Get(obj.m_iPeriod) &&
        ks.Get(reinterpret_cast<int&>(obj.m_tDelDate));
}



//수량성 아이템
SERIALIZE_DEFINE_PUT(KSelectDurationItemInfo, obj, ks)
{
    return  ks.Put(obj.m_strGoodsName) &&
        ks.Put(obj.m_dwGoodsID) &&
        ks.Put(obj.m_dwGoodsUID) &&
        ks.Put(obj.m_strOwnerLogin) &&
        ks.Put(obj.m_strBuyerLogin) &&
        ks.Put(obj.m_iDurationNum) &&
        ks.Put(static_cast<int>(obj.m_tRegDate)) &&
        ks.Put(static_cast<int>(obj.m_tDelDate));

}
SERIALIZE_DEFINE_GET(KSelectDurationItemInfo, obj, ks)
{
    return  ks.Get(obj.m_strGoodsName) &&
        ks.Get(obj.m_dwGoodsID) &&
        ks.Get(obj.m_dwGoodsUID) &&
        ks.Get(obj.m_strOwnerLogin) &&
        ks.Get(obj.m_strBuyerLogin) &&
        ks.Get(obj.m_iDurationNum) &&
        ks.Get(reinterpret_cast<int&>(obj.m_tRegDate)) &&
        ks.Get(reinterpret_cast<int&>(obj.m_tDelDate));
}


SERIALIZE_DEFINE_PUT(KUserDBInfo, obj, ks)
{
    return  ks.Put(obj.m_strLogin) &&
        ks.Put(obj.m_iExp0) &&
        ks.Put(obj.m_iExp1) &&
        ks.Put(obj.m_iExp2) &&
        ks.Put(obj.m_iGamePoint) &&
        ks.Put(obj.m_cAuthLevel) &&
        ks.Put(obj.m_iBlessing0) &&
        ks.Put(obj.m_iBlessing1) &&
        ks.Put(obj.m_iBlessing2);
}
SERIALIZE_DEFINE_GET(KUserDBInfo, obj, ks)
{
    return  ks.Get(obj.m_strLogin) &&
        ks.Get(obj.m_iExp0) &&
        ks.Get(obj.m_iExp1) &&
        ks.Get(obj.m_iExp2) &&
        ks.Get(obj.m_iGamePoint) &&
        ks.Get(obj.m_cAuthLevel) &&
        ks.Get(obj.m_iBlessing0) &&
        ks.Get(obj.m_iBlessing1) &&
        ks.Get(obj.m_iBlessing2);
}
